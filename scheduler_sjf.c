#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"

extern struct queue * ready;
extern struct queue * blocked;
extern struct queue * finished;
extern int MAX_TIME;

struct proc * dequeue_bypid(struct queue * q, int pid);  // Garante que compilador reconheça

struct proc * scheduler(struct proc * current)
{
    struct proc * selected = NULL;

    // Trata o processo que estava rodando
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

    if (isempty(ready)) {
        return NULL;
    }

    // SJF: seleciona o processo com menor remaining_time
    struct proc * aux = ready->head;
    struct proc * menor = aux;

    while (aux != NULL) {
        if (aux->remaining_time < menor->remaining_time) {
            menor = aux;
        }
        aux = aux->next;
    }

    // Remove da fila o processo com menor tempo restante
    selected = dequeue_bypid(ready, menor->pid);
    count_ready_out(selected);
    selected->state = RUNNING;

    return selected;
}