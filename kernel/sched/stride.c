#include "types.h"
#include "param.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "sched.h"

// LAB ch2.2a: 根据 tickets 计算 stride（STRIDE_BIG / tickets）。
static uint64
stride_for_tickets(int tickets)
{
  (void)tickets;
  return STRIDE_BIG;
}

void
sched_stride_init(void)
{
}

// LAB ch2.1: 初始化 tickets 为默认值，stride 根据 tickets 计算，pass 为 0。
void
sched_init_proc(struct proc *p)
{
  (void)p;
}

// LAB ch2.2b: 加锁 → 更新 tickets 和 stride → 解锁（不改 pass）。
int
sched_set_tickets(struct proc *p, int tickets)
{
  if(tickets < 1 || tickets > SCHED_MAX_TICKETS)
    return -1;

  (void)p;
  (void)stride_for_tickets(tickets);
  return 0;
}

// LAB ch2.4: 检查 priority 范围 [1,100]，映射为 tickets，加锁更新，返回 0 或 -1。
int
sched_set_priority(struct proc *p, int priority)
{
  (void)p;
  (void)priority;
  return -1;
}

// LAB ch2.3a: 扫描 proc[]，选 pass 最小的 RUNNABLE 进程。
// 返回时持有 p->lock；没选中的 release(&p->lock)；无 RUNNABLE 返回 0。
// pass 相同时 pid 小的优先。
static struct proc *
sched_scan_best(void)
{
  return sched_pick_rr();
}

// LAB ch2.3b: 更新 best 的 pass（pass += stride）。
static void
sched_commit(struct proc *best)
{
  if(best == 0)
    return;
}

struct proc *
sched_pick_stride(void)
{
  struct proc *best = sched_scan_best();
  sched_commit(best);
  return best;
}
