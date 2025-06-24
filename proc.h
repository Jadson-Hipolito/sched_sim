#include <time.h>
#include <sys/time.h>
#include <pthread.h>

// Estados possíveis do processo
enum procstate { READY, RUNNING, BLOCKED, FINISHED };

// Estrutura de Controle de Processo (PCB)
struct proc
{
    // Identificação e estado
    unsigned int pid;
    enum procstate state;
    unsigned int queue;

    // Marcação de tempo
    struct timeval time_start;
    struct timeval time_end;
    struct timeval time_ready;
    struct timeval time_blocked;

    // Métricas de tempo
    int turnaround_time;
    int waiting_time;
    int blocked_time;
    int process_time;
    int process_time_total;
    int remaining_time;
    int used_quantum;           // ← Quantum utilizado na última execução

    // Estatísticas de escalonamento
    unsigned int num_ready;
    unsigned int num_blocked;

    // Thread associada ao processo
    pthread_t tid;

    // Encadeamento nas filas
    struct proc *prev;
    struct proc *next;
};

// Funções relacionadas a processos
void * process_thread(void *);
pthread_t createproc_thread(struct proc *p);
struct proc * createproc();
void killproc(struct proc *p);
void printproc(struct proc *p);
void procend(struct queue * q);