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

  // LAB ch2: 复用 ch1 的进程创建和结果收集结构；这里重点是调用
  // getpinfo(ps)，在 ps[] 中找到每个子进程 pid，并检查 tickets。
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

  // LAB ch2: CPU-bound 忙循环，r.loops++。
  while(uptime() - start < RUN_TICKS) {
    // TODO
  }

  r.elapsed = uptime() - start;
  write(fd, &r, sizeof(r));
  close(fd);
  exit(0);
}

int
main(int argc, char *argv[])
{
  // LAB ch2: 创建 tickets 为 1、2、4 的 CPU-bound 子进程。不要把这里
  // 当作第二个 pipe 练习；重点是 tickets、getpinfo 和比例检查。

  // LAB ch2

  exit(1);
}
