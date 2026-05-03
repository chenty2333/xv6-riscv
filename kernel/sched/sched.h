#ifndef XV6_SCHED_H
#define XV6_SCHED_H

struct proc;

#define SCHED_DEFAULT_TICKETS 100
#define SCHED_MAX_TICKETS 10000
#define STRIDE_BIG 1000000

void sched_rr_init(void);
void sched_stride_init(void);

static inline void
schedinit(void)
{
  sched_rr_init();
  sched_stride_init();
}

void sched_init_proc(struct proc *p);
int sched_set_tickets(struct proc *p, int tickets);
int sched_set_priority(struct proc *p, int priority);

struct proc *sched_pick_rr(void);
struct proc *sched_pick_stride(void);

#ifdef SCHED_STRIDE
#define sched_pick_next sched_pick_stride
#else
#define sched_pick_next sched_pick_rr
#endif

#endif
