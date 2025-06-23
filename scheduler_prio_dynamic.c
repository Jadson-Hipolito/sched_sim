#include <stdio.h>

#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"

extern struct queue * ready;
extern struct queue * ready2;
extern struct queue * blocked;
extern struct queue * finished;

extern int MAX_TIME;

struct proc * scheduler(struct proc * current)
{
    struct proc * selected = NULL;

    // Realimentação baseada no motivo de saída da CPU
    if (current != NULL)
    {
        switch (current->state)
        {
            case READY:  // saiu por preempção → vai para ready2
                current->queue = 1;
                enqueue(ready2, current);
                count_ready2_in(current);
                break;

            case BLOCKED:  // saiu para E/S → vai para ready
                current->queue = 0;
                enqueue(ready, current);
                count_ready_in(current);
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

    // Sorteio de fila com probabilidade 80% vs 20%
    int sorteio = rand() % 10;

    if (!isempty(ready) && (sorteio < 8 || isempty(ready2))) {
        selected = dequeue(ready);
        count_ready_out(selected);
    } else {
        selected = dequeue(ready2);
        count_ready2_out(selected);
    }

    if (selected != NULL) {
        selected->state = RUNNING;
    }

    return selected;
}
