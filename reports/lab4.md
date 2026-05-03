# 实验四 · 问答题参考答案

## Q1：swtch 调用闭环

1. 第一次调度时，`scheduler()` 将 CPU 调度上下文的寄存器保存到 `p->context`，并从 `c->context` 恢复。由于 `c->context` 的 `ra` 指向 `scheduler()` 循环中 `swtch` 的下一行，`sp` 指向 CPU 的内核栈，所以 "切换" 后实际上继续执行 `scheduler()` 自己，根本没有进入进程。scheduer 会立刻再次循环，反复选同一个进程但永远无法运行它。

2. 如果进程 B 在另一个 CPU 上曾正常被调度并调用 `sched()`，`sched()` 中的 `swtch(&p->context, &c->context)` 会将进程寄存器保存到 `p->context`，从 `c->context` 恢复。但 `c->context` 从未被 scheduler 的 swtch 正确保存过（因为第一次就写错了），里面是随机或过期的栈数据，`ra` 指向未知地址，系统崩溃。

3. `swtch(old, new)`：`old` 是"把当前寄存器存到哪里"，`new` 是"从哪里恢复新寄存器"。第一次调用时 old 是 CPU 的调度上下文（你正在运行的栈），new 是进程上下文（你要去的栈）；返回时参数对调。

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

---

## Q3：stride 多核问题

1. 两个 CPU 同时执行 sched_scan_best()，都遍历 proc[] 并比较 pass。可能出现：
   - CPU0 选中 proc[0]（pass=100），CPU1 也选中 proc[0]（因为它看到的 pass 也是 100，还没被更新）。
   - 两个 CPU 各自给 proc[0] 执行 pass += stride，proc[0] 跑了两个时间片才让 pass 增加一次应有的量。
   - 更严重的是，proc[0] 的 p->lock 被一个 CPU 持有，另一个 CPU 拿不到锁，要么跳过 proc[0]（导致"最小 pass"的进程被错过），要么阻塞等待（丧失多核并行优势）。

2. stride 算法的核心是"全局最小值选择器"——必须在所有 RUNNABLE 进程中比较出一个全局最小 pass。这在多核下意味着每次调度都需要全局同步，是最差的并行场景。本质上，stride 是为单核设计的：单核下每次只调度一个进程，pass 的更新是串行的；多核下多个调度器并行选进程，pass 更新的顺序不再符合算法的串行语义。

3. 思路：将 pass 的更新变为原子操作（用锁或 CAS 保护），确保 sched_scan_best() 在比较 pass 和更新 pass 之间是原子的。但更根本的方案是改为 per-CPU 就绪队列——每个 CPU 维护自己的就绪进程列表和本地 pass，CPU 之间通过 work stealing 做负载均衡。这样大部分调度决策是本地的，不需要全局扫描和全局比较，只有负载均衡时才跨核交互。但此时"全局 1:2:4 比例"的语义变为"每个 CPU 本地近似 1:2:4"，多核下的精确比例不再是保证目标。
