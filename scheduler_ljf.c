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

    // Trata o processo que saiu da CPU
    if (current != NULL)
    {
        switch (current->state)
        {
            case READY:
                enqueue(ready, current);
                count_ready_in(current);
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

    // Se a fila estiver vazia, não há processo para escalar
    if (isempty(ready)) {
        return NULL;
    }

    // Busca o processo com maior remaining_time
    struct node * aux = ready->first;
    struct node * maior = aux;

    while (aux != NULL) {
        if (aux->proc->remaining_time > maior->proc->remaining_time) {
            maior = aux;
        }
        aux = aux->next;
    }

    // Remove da fila
    selected = removepid(ready, maior->proc->pid);
    count_ready_out(selected);
    selected->state = RUNNING;

    return selected;
}
