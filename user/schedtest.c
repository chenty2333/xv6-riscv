#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define CHILDREN 3
#define RUN_TICKS 80

struct result {
  int id;
  int elapsed;
  uint64 loops;
};

static void
worker(int id, int fd)
{
  struct result r;
  int start;

  r.id = id;
  r.loops = 0;
  start = uptime();

  // LAB ch1.3a: CPU-bound 忙循环，r.loops++。
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
  // LAB ch1.3b: pipe → fork(CHILDREN) → 父进程读 pipe → 打印结果 → wait → PASS/FAIL。

  exit(1);
}
