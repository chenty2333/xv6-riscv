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

  // LAB ch1.3a TODO:
  // 实现 CPU-bound 忙循环：在 RUN_TICKS 个 tick 内持续累加 r.loops。
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
  // LAB ch1.3b TODO:
  // 创建测试子进程，收集它们的运行结果：
  // 1. 用 pipe() 创建一个管道。
  // 2. 用 fork() 创建 CHILDREN 个子进程，每个子进程执行 worker(i, p[1])。
  // 3. 父进程关闭写端，从管道读取 CHILDREN 个 struct result。
  // 4. 打印每个子进程的 id、elapsed、loops。
  // 5. wait() 回收所有子进程。
  // 6. 如果三个子进程的 loops 都 > 0，打印 "schedtest: PASS"，exit(0)，
  //    否则 exit(1)。
  // 提示：参考 xv6 中 pipe 和 fork 的标准用法（如 user/sh.c）。

  // TODO ch1.3b: 在这里补全你的实现。

  exit(1);
}
