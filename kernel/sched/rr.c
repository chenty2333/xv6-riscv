#include "types.h"
#include "param.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "sched.h"

static struct spinlock rr_lock;
static int rr_next;

void
sched_rr_init(void)
{
  initlock(&rr_lock, "sched_rr");
  rr_next = 0;
}

// Return a RUNNABLE process with p->lock held, or 0 if none exists.
struct proc *
sched_pick_rr(void)
{
  struct proc *p;
  int i;
  int idx;

  acquire(&rr_lock);
  for(i = 0; i < NPROC; i++) {
    // LAB ch1.3: this is the round-robin cursor calculation students fill in.
    idx = (rr_next + i) % NPROC;
    p = &proc[idx];

    acquire(&p->lock);
    if(p->state == RUNNABLE) {
      rr_next = (idx + 1) % NPROC;
      release(&rr_lock);
      return p;
    }
    release(&p->lock);
  }
  release(&rr_lock);

  return 0;
}
