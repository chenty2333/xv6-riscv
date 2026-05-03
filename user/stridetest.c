#include "kernel/types.h"
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

static void
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

  // LAB ch2.3a: CPU-bound 忙循环，r.loops++。
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
  // LAB ch2.3b: 创建子进程、收集结果、验证 1:2:4 比例。
  // 1. 用 pipe() 创建管道。
  // 2. 用 fork() 创建 CHILDREN 个子进程，tickets = 1, 2, 4。
  // 3. 父进程关闭写端，从管道读取 CHILDREN 个 struct result。
  // 4. 打印每个子进程的 tickets、elapsed、loops。
  // 5. 如果 loops[0] < loops[1] < loops[2]，打印 "stridetest: PASS"，exit(0)，
  //    否则 exit(1)。

  // LAB ch2.3b

  exit(1);
}
