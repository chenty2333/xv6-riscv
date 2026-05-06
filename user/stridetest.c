#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"

#define CHILDREN 3
#define RUN_TICKS 120

struct result {
  int id;
  int tickets;
  int elapsed;
  uint64 loops;
};

static int tickets[CHILDREN] = { 1, 2, 4 };

int
check_pinfo(int pids[CHILDREN])
{
  struct pstat ps[NPROC];

  // 子进程，并检查 tickets。建议准备 seen[CHILDREN] 和循环变量 i/j；
  // seen[j] 表示 pids[j] 是否已经在 ps[] 中出现。
  //
  // getpinfo 失败时打印 "stridetest: getpinfo failed" 并返回 -1。
  // 遍历 ps[] 时，如果 ps[i].pid == pids[j]，继续检查
  // ps[i].tickets 是否等于 tickets[j]；不一致说明 settickets()
  // 或 getpinfo() 有问题，打印错误并返回 -1。
  //
  // 最后检查 seen[]：缺少某个 pid 就打印 missing 错误并返回 -1；
  // 全部找到且 tickets 正确时返回 0。
  (void)pids;
  (void)ps;
  return -1;
}

void
worker(int id, int fd)
{
  struct result r;
  int start;

  if(settickets(tickets[id]) < 0) {
    fprintf(2, "stridetest: settickets(%d) failed\n", tickets[id]);
    exit(1);
  }

  r.id = id;
  r.tickets = tickets[id];
  r.loops = 0;
  start = uptime();

  // 也不要主动 yield；运行 RUN_TICKS 个 tick，每轮只增加 r.loops。
  // tickets 越多的子进程，长期 loops 应该越大。
  while(uptime() - start < RUN_TICKS) {
  }

  r.elapsed = uptime() - start;
  // 保存 loops，用来检查 1:2:4 的长期趋势。
  write(fd, &r, sizeof(r));
  close(fd);
  exit(0);
}

int
main(int argc, char *argv[])
{
  // 当作第二个 pipe 练习；pipe/fork/read/wait 可以直接复用 schedtest.c
  // 的结构。建议准备 p[2]、i、pid、pids[CHILDREN]、struct result r
  // 和 loops[CHILDREN]。
  //
  // 父进程 fork 后保存 pids[i]，供 check_pinfo(pids) 匹配 ps[]。
  // 子进程关闭 p[0] 后调用 worker(i, p[1])；worker() 会先
  // settickets(tickets[i])，再忙循环并写回结果。
  //
  // 父进程关闭 p[1] 后读取 CHILDREN 个结果。子进程结束顺序不一定
  // 等于 id 顺序，建议用 loops[r.id] = r.loops 保存，再打印结果。
  //
  // 读完后关闭 p[0]，调用 check_pinfo(pids) 确认 getpinfo() 能看到
  // 这些子进程，且 tickets 分别是 1、2、4；随后 wait CHILDREN 次。
  //
  // 最后检查长期趋势：loops[0] < loops[1] < loops[2]。不要求精确 1:2:4，
  // 只要求 tickets 越大，CPU-bound 忙循环次数大致越多。

  exit(1);
}
