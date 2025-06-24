#include <stdio.h>

#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"
#include <stdlib.h>

extern struct queue * ready;
extern struct queue * ready2;
extern struct queue * blocked;
extern struct queue * finished;

extern int MAX_TIME;

struct proc * scheduler(struct proc * current)
{
    struct proc * selected = NULL;

    // Trata o processo que acabou de sair da CPU
    if (current != NULL)
    {
        float percent_restante = ((float)current->remaining_time / MAX_TIME) * 100.0;

        // Classifica o processo de acordo com seu tempo restante
        if (percent_restante <= 20.0) {
            current->queue = 0;
        } else {
            current->queue = 1;
        }

        switch (current->state)
        {
            case READY:
                if (current->queue == 0) {
                    enqueue(ready, current);
                    count_ready_in(current);
                } else {
                    enqueue(ready2, current);
                    count_ready2_in(current);
                }
                break;

            case BLOCKED:
                enqueue(blocked, current);
                count_blocked_in(current);
                break;

            case FINISHED:
                enqueue(finished, current);
                count_finished_in(current);
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    // Se ambas as filas estiverem vazias, não há o que fazer
    if (isempty(ready) && isempty(ready2)) {
        return NULL;
    }

    // Sorteio com probabilidade: 80% para fila 0 (ready), 20% para fila 1 (ready2)
    int sorteio = rand() % 10;

    if (!isempty(ready) && (sorteio < 8 || isempty(ready2))) {
        selected = dequeue(ready);
        count_ready_out(selected);
    } else {
        selected = dequeue(ready2);
        count_ready2_out(selected);
    }

    // Atualiza o estado do processo selecionado
    if (selected != NULL) {
        selected->state = RUNNING;
    }

    return selected;
}
