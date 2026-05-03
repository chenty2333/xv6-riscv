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

  // LAB ch1.2: 实现 Round-Robin 扫描。
  // 1. 获取 rr_lock（保护 rr_next）。
  // 2. 从 rr_next 开始：idx = (rr_next + i) % NPROC。
  // 3. 对每个进程 acquire(&p->lock) 后检查 p->state == RUNNABLE。
  // 4. 找到后：更新 rr_next = (idx + 1) % NPROC，释放 rr_lock，返回 p（持有锁）。
  // 5. 未选中的必须 release(&p->lock)。
  // 6. 全表无 RUNNABLE 则释放 rr_lock，返回 0。
  //
  // 当前占位实现：每次都从 0 开始扫描，未更新游标。
  for(i = 0; i < NPROC; i++) {
    idx = i;
    p = &proc[idx];
    if(p->state == RUNNABLE)
      return p;
  }

  return 0;
}
