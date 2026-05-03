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
  // LAB ch2.2 TODO:
  // 根据 tickets 计算 stride。
  // 规则：stride = STRIDE_BIG / tickets。
  // 注意 tickets 必须大于 0，调用者应该先完成参数检查。
  (void)tickets;
  return STRIDE_BIG;
}

void
sched_stride_init(void)
{
}

void
sched_init_proc(struct proc *p)
{
  // LAB ch2.1 TODO:
  // 新进程创建时需要初始化 stride 调度字段：
  // 1. tickets 设置为 SCHED_DEFAULT_TICKETS。
  // 2. stride 根据 tickets 计算。
  // 3. pass 设置为 0。
  // 这些字段需要你先添加到 struct proc 中。
  (void)p;
}

int
sched_set_tickets(struct proc *p, int tickets)
{
  if(tickets < 1 || tickets > SCHED_MAX_TICKETS)
    return -1;

  // LAB ch2.2 TODO:
  // 完成 settickets 的内核逻辑：
  // 1. 参数检查已经在上面给出，非法 tickets 返回 -1。
  // 2. 获取 p->lock 后更新 p->tickets。
  // 3. 使用 stride_for_tickets(tickets) 更新 p->stride。
  // 4. 释放 p->lock 并返回 0。
  // 提示：这里不需要修改 p->pass。
  (void)p;
  (void)stride_for_tickets(tickets);
  return 0;
}

// LAB ch2.4 TODO:
// 实现 setpriority 系统调用的内核逻辑。
// priority 范围是 1-100，将其映射为 tickets。
// 映射规则：tickets = priority * (SCHED_MAX_TICKETS / 100)。
// 步骤：
// 1. 检查 priority 是否在 [1, 100] 范围内，非法返回 -1。
// 2. 获取 p->lock。
// 3. 将 priority 映射为 tickets 值。
// 4. 更新 p->tickets 和 p->stride。
// 5. 释放 p->lock，返回 0。
// 提示：参考 sched_set_tickets() 的写法。
int
sched_set_priority(struct proc *p, int priority)
{
  // TODO ch2.4: 在这里补全你的实现。
  (void)p;
  (void)priority;
  return -1;
}

// Return a RUNNABLE process with p->lock held, or 0 if none exists.
struct proc *
sched_pick_stride(void)
{
  // LAB ch2.3 TODO:
  // 实现 stride 调度的进程选择逻辑。
  // 目标：返回一个 RUNNABLE 进程，并且返回时保持该进程的 p->lock 已经持有。
  //
  // 建议步骤：
  // 1. 扫描 proc[] 进程表。
  // 2. 对每个进程先 acquire(&p->lock)，检查 p->state 是否为 RUNNABLE。
  // 3. 在所有 RUNNABLE 进程中选择 pass 最小的进程。
  // 4. 如果两个进程 pass 相同，可以用 pid 小的进程作为 tie-break。
  // 5. 没被选中的进程必须及时 release(&p->lock)。
  // 6. 选中 best 后，执行 best->pass += best->stride。
  // 7. 返回 best；如果没有 RUNNABLE 进程，返回 0。
  //
  // 当前占位实现退回 RR，只是为了让未完成 ch2 时系统仍然能启动。
  // 完成 ch2.3 后，请删除这一行并实现上面的逻辑。
  return sched_pick_rr();
}
