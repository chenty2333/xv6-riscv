#include "types.h"
#include "param.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "sched.h"

// LAB ch2: 根据 tickets 计算 stride；无效 tickets 不应导致除零。
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

// LAB ch2: 给新进程设置默认 tickets、stride 和 pass。
void
sched_init_proc(struct proc *p)
{
  (void)p;
}

// LAB ch2: 更新进程的 tickets/stride，保持 pass 不变。
// 修改进程调度字段时需要遵守 p->lock 协议。
int
sched_set_tickets(struct proc *p, int tickets)
{
  if(tickets < 1 || tickets > SCHED_MAX_TICKETS)
    return -1;

  (void)p;
  (void)stride_for_tickets(tickets);
  return 0;
}

// LAB ch2: 选择 pass 最小的 RUNNABLE 进程。
// 返回时持有 best->lock；未选中的进程锁必须释放。
// pass 相同时 pid 小的进程优先。
static struct proc *
sched_scan_best(void)
{
  return sched_pick_rr();
}

// LAB ch2: 被选中的进程需要推进自己的虚拟时间。
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
