# 实验四 · 问答题参考答案

## Q1：swtch 调用闭环

1. 第一次调度时，`scheduler()` 将 CPU 调度上下文的寄存器保存到 `p->context`，并从 `c->context` 恢复。由于 `c->context` 的 `ra` 指向 `scheduler()` 循环中 `swtch` 的下一行，`sp` 指向 CPU 的内核栈，所以 "切换" 后实际上继续执行 `scheduler()` 自己，根本没有进入进程。scheduer 会立刻再次循环，反复选同一个进程但永远无法运行它。

2. 如果进程 B 在另一个 CPU 上曾正常被调度并调用 `sched()`，`sched()` 中的 `swtch(&p->context, &c->context)` 会将进程寄存器保存到 `p->context`，从 `c->context` 恢复。但 `c->context` 从未被 scheduler 的 swtch 正确保存过（因为第一次就写错了），里面是随机或过期的栈数据，`ra` 指向未知地址，系统崩溃。

3. `swtch(old， new)`：`old` 是"把当前寄存器存到哪里"，`new` 是"从哪里恢复新寄存器"。第一次调用时 old 是 CPU 的调度上下文（你正在运行的栈），new 是进程上下文（你要去的栈）；返回时参数对调。

简答：1. swtch 把 scheduler 的寄存器存进了进程 context 里，却从 CPU context 恢复了 scheduler 自己的旧寄存器，等于什么也没切，进程永远运行不了。2. 进程切回时读到 scheduler 从未被正确保存过的 context，ra 指向垃圾数据，崩溃。3. old = 当前状态存到哪里，new = 从哪恢复新状态。

---

## Q2：RR 游标与两把锁

1. 假设 rr_next 初始为 0，两个 CPU 同时执行 sched_pick_rr()：

```
CPU 0                            CPU 1
acquire(&proc[0].lock)          acquire(&proc[0].lock)  ← 一个拿不到，等
读取 rr_next = 0
选中 proc[0], rr_next = 1
release(&proc[0].lock)          acquire(&proc[0].lock)  ← 拿到了
                                读取 rr_next = 1  ← 已经被 CPU0 更新
                                选中 proc[1]
```

如果没有 rr_lock，两者读 rr_next 的时机不受保护：CPU0 和 CPU1 可能同时读到 rr_next=0，都选 proc[0]，其中一个进程被两个 CPU 同时设为 RUNNING，数据结构完全损坏。

2. 如果把 rr_next 放进每个 CPU 的 struct cpu 中，不需要 rr_lock，但会导致各 CPU 互相不知道对方扫到哪了——每个核独立维护自己的游标，可能出现 CPU0 反复扫描 proc[0..3] 而 CPU1 反复扫描 proc[4..7]，不再有全局的"轮流公平"。除非配合 per-CPU 就绪队列（如 Linux 的 per-CPU runqueue），否则会影响公平性。

3. 当共享状态是全局的、需要多核之间协调顺序时，需要独立锁。p->lock 保护的是单个进程的状态（局部），rr_lock 保护的是"下一个选谁"的全局决策顺序（全局）。两者粒度不同，不能互相替代。

简答：1. 两个 CPU 可能同时读到 rr_next=0，都选同一个进程，该进程被双核同时设为 RUNNING，数据结构损坏。2. 不需要锁但会丧失全局公平性——每个核各扫各的，互相不知道对方扫到哪。3. p->lock 保护单个进程，rr_lock 保护全局决策顺序；共享的是"下一个选谁"这个全局状态时必须用独立锁。

---

## Q3：stride 多核问题

1. 不能。per-CPU 队列意味着每个 CPU 只在自己的本地队列里按 stride 选择 pass 最小的进程，work stealing 只在某个 CPU 队列为空时才触发跨核迁移。但"全局 1：2：4 比例"要求所有进程在同一套调度决策链中竞争——tickets=1 的进程必须相对于 tickets=4 的进程被少选，而后者可能在不同 CPU 的队列里，两者不再直接比较 pass，比例只能是本地的近似。多核下的精确全局比例需要全局锁或全局同步，这又回到了单核串行瓶颈。

简答：不能。per-CPU 队列让 stride 只在每个 CPU 本地生效，tickets 不同的进程分布在不同核心的队列中，不再有全局 pass 比较。比例降级为"每个核本地近似 1：2：4"，精确全局比例只能在单核下保证。
