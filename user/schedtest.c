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

  while(uptime() - start < RUN_TICKS)
    r.loops++;

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
  struct result r;

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
  for(i = 0; i < CHILDREN; i++) {
    if(read(p[0], &r, sizeof(r)) != sizeof(r)) {
      fprintf(2, "schedtest: read failed\n");
      exit(1);
    }
    printf("child %d: elapsed=%d loops=%lu\n", r.id, r.elapsed, r.loops);
  }

  close(p[0]);
  for(i = 0; i < CHILDREN; i++)
    wait(0);

  exit(0);
}
