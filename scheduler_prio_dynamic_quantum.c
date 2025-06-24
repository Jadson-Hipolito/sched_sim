#include <stdio.h>

#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"
#include <stdlib.h>

// Filas globais
extern struct queue * ready;
extern struct queue * ready2;
extern struct queue * blocked;
extern struct queue * finished;

// Variáveis globais auxiliares
extern int MAX_TIME;
extern int QUANTUM;

struct proc * scheduler(struct proc * current)
{
    struct proc * selected = NULL;

    // Realimentação baseada no uso do QUANTUM
    if (current != NULL)
    {
        int quantum_usado = current->used_quantum;  // campo que armazena quanto foi usado
        float porcentagem = ((float)quantum_usado / QUANTUM) * 100.0;

        switch (current->state)
        {
            case READY:
            case BLOCKED:
                if (porcentagem > 50.0) {
                    current->queue = 0;  // volta para ready
                    enqueue(ready, current);
                    count_ready_in(current);
                } else {
                    current->queue = 1;  // volta para ready2
                    enqueue(ready2, current);
                    count_ready2_in(current);
                }
                break;

            case FINISHED:
                enqueue(finished, current);
                count_finished_in(current);
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    //  Se ambas as filas estiverem vazias
    if (isempty(ready) && isempty(ready2)) {
        return NULL;
    }

    // Sorteio de fila com probabilidade 80% (fila 0) vs 20% (fila 1)
    int sorteio = rand() % 10;

    if (!isempty(ready) && (sorteio < 8 || isempty(ready2))) {
        selected = dequeue(ready);
        count_ready_out(selected);
    } else if (!isempty(ready2)) {
        selected = dequeue(ready2);
        count_ready2_out(selected);
    }

    // Define processo selecionado como em execução
    if (selected != NULL) {
        selected->state = RUNNING;
    }

    return selected;
}
