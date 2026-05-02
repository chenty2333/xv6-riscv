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

  // LAB ch2.3 TODO:
  // 和 schedtest 类似，这里需要做 CPU-bound 忙循环。
  // 不同的是，每个子进程已经设置了不同 tickets。
  // 完成 stride 调度后，tickets 越多，loops 应该越大。
  while(uptime() - start < RUN_TICKS) {
    // TODO: r.loops++;
    break;
  }

  r.elapsed = uptime() - start;
  write(fd, &r, sizeof(r));
  close(fd);
  exit(0);
}

int
main(int argc, char *argv[])
{
  int p[2];
  int i;
  int pid;

  if(pipe(p) < 0) {
    fprintf(2, "stridetest: pipe failed\n");
    exit(1);
  }

  for(i = 0; i < CHILDREN; i++) {
    pid = fork();
    if(pid < 0) {
      fprintf(2, "stridetest: fork failed\n");
      exit(1);
    }
    if(pid == 0) {
      close(p[0]);
      worker(i, p[1]);
    }
  }

  close(p[1]);
  printf("stridetest: tickets 1:2:4, %d ticks\n", RUN_TICKS);

  // LAB ch2.3 TODO:
  // 父进程需要读取 CHILDREN 个 result，并把每个 child 的 loops 保存下来。
  // 运行 SCHED=STRIDE 时，期望 tickets=1、2、4 的 loops 也大致递增。
  // 一个简单检查是 loops[0] < loops[1] && loops[1] < loops[2]。
  printf("stridetest: TODO collect results and check 1:2:4 ratio\n");

  close(p[0]);
  for(i = 0; i < CHILDREN; i++)
    wait(0);

  exit(1);
}
