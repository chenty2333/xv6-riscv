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

  // rr_next 是全局游标，需要用 rr_lock 保护；返回 RUNNABLE 进程时
  // 必须保持 p->lock，未选中的进程锁必须释放。
  // 当前占位实现每次都从 0 开始扫描，且没有维护锁协议。
  for(i = 0; i < NPROC; i++) {
    idx = i;
    p = &proc[idx];
    if(p->state == RUNNABLE)
      return p;
  }

  return 0;
}
