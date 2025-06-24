#include <stdio.h>
#include "queue.h"
#include "proc.h"
#include "time.h"

extern int NPROC;

// Fila de aptos (ready)
void count_ready_in(struct proc *p)
{
    p->num_ready++;
    set_time_now(&p->time_ready);
}

void count_ready_out(struct proc *p)
{
    p->waiting_time += set_time_diff_now(p->time_ready);
}

// Fila de aptos nível 2 (ready2)
void count_ready2_in(struct proc *p)
{
    p->num_ready++;
    set_time_now(&p->time_ready);
}

void count_ready2_out(struct proc *p)
{
    p->waiting_time += set_time_diff_now(p->time_ready);
}

// Fila de bloqueados
void count_blocked_in(struct proc *p)
{
    p->num_blocked++;
    set_time_now(&p->time_blocked);
}

void count_blocked_out(struct proc *p)
{
    p->blocked_time += set_time_diff_now(p->time_blocked);
}

// Fila de finalizados
void count_finished_in(struct proc *p)
{
    p->turnaround_time = set_time_diff_now(p->time_start);
}

// Estatísticas finais
void accounting(struct queue *q)
{
    struct proc *aux;
    double TME = 0, TMB = 0, TMT = 0;
    double num_ready = 0, num_blocked = 0;

    printf("\n_________ ESTATISTICAS ________\n\n");

    for(aux = q->head; aux != NULL; aux = aux->next)
    {
        TME += aux->waiting_time;
        num_ready += aux->num_ready;

        TMB += aux->blocked_time;
        num_blocked += aux->num_blocked;

        TMT += aux->turnaround_time;
    }

    printf("TME: %f\n", TME / NPROC);
    printf("TMB: %f\n", TMB / NPROC);
    printf("TMT: %f\n", TMT / NPROC);
    printf("_______________________________\n");
}