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
    // LAB ch1.2 TODO:
    // 当前占位实现每轮都从 proc[0] 开始扫描，会偏向进程表前面的进程。
    // 请改成真正的 round-robin：
    // 1. 从 rr_next 开始扫描，而不是从 0 开始。
    // 2. 用取模把扫描范围限制在 [0, NPROC)。
    // 3. 计算出的 idx 应该表示本次要检查的 proc 下标。
    idx = i;
    p = &proc[idx];

    acquire(&p->lock);
    if(p->state == RUNNABLE) {
      // LAB ch1.2 TODO:
      // 找到可运行进程后，更新 rr_next。
      // 下一次调度应该从当前进程的下一个位置开始，避免一直选择同一个低下标进程。
      release(&rr_lock);
      return p;
    }
    release(&p->lock);
  }
  release(&rr_lock);

  return 0;
}
