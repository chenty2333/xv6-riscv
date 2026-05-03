#include "types.h"
#include "param.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "sched.h"

static uint64
stride_for_tickets(int tickets)
{
  if(tickets < 1)
    tickets = 1;
  return STRIDE_BIG / tickets;
}

void
sched_stride_init(void)
{
}

void
sched_init_proc(struct proc *p)
{
  p->tickets = SCHED_DEFAULT_TICKETS;
  p->stride = stride_for_tickets(p->tickets);
  p->pass = 0;
}

int
sched_set_tickets(struct proc *p, int tickets)
{
  if(tickets < 1 || tickets > SCHED_MAX_TICKETS)
    return -1;

  acquire(&p->lock);
  p->tickets = tickets;
  p->stride = stride_for_tickets(tickets);
  release(&p->lock);
  return 0;
}

int
sched_set_priority(struct proc *p, int priority)
{
  if(priority < 1 || priority > 100)
    return -1;

  int tickets = priority * (SCHED_MAX_TICKETS / 100);
  acquire(&p->lock);
  p->tickets = tickets;
  p->stride = stride_for_tickets(tickets);
  release(&p->lock);
  return 0;
}

static struct proc *
sched_scan_best(void)
{
  struct proc *p;
  struct proc *best = 0;

  for(p = proc; p < &proc[NPROC]; p++) {
    acquire(&p->lock);
    if(p->state == RUNNABLE &&
       (best == 0 ||
        p->pass < best->pass ||
        (p->pass == best->pass && p->pid < best->pid))) {
      if(best != 0)
        release(&best->lock);
      best = p;
    } else {
      release(&p->lock);
    }
  }

  return best;
}

static void
sched_commit(struct proc *best)
{
  if(best != 0)
    best->pass += best->stride;
}

struct proc *
sched_pick_stride(void)
{
  struct proc *best = sched_scan_best();
  sched_commit(best);
  return best;
}
