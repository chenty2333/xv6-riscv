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

  // LAB ch1.3 TODO:
  // 这里需要让子进程在 RUN_TICKS 个时钟 tick 内持续占用 CPU。
  // 每循环一次就增加 r.loops，用来观察不同子进程是否都获得了运行机会。
  // 当前占位代码会很快退出，schedtest 不会通过。
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
    fprintf(2, "schedtest: pipe failed\n");
    exit(1);
  }

  for(i = 0; i < CHILDREN; i++) {
    pid = fork();
    if(pid < 0) {
      fprintf(2, "schedtest: fork failed\n");
      exit(1);
    }
    if(pid == 0) {
      close(p[0]);
      worker(i, p[1]);
    }
  }

  close(p[1]);
  printf("schedtest: %d cpu-bound children, %d ticks\n", CHILDREN, RUN_TICKS);

  // LAB ch1.3 TODO:
  // 父进程需要从 pipe 读取 CHILDREN 个 struct result。
  // 每读取一个结果，就打印 child id、elapsed 和 loops。
  // 最后可以检查每个 child 的 loops 都大于 0，确认多个 CPU-bound 进程都运行过。
  printf("schedtest: TODO collect child results\n");

  close(p[0]);
  for(i = 0; i < CHILDREN; i++)
    wait(0);

  exit(1);
}
