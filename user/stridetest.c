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
  uint64 loops[CHILDREN];

  memset(loops, 0, sizeof(loops));

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
  for(i = 0; i < CHILDREN; i++) {
    if(read(p[0], &r, sizeof(r)) != sizeof(r)) {
      fprintf(2, "stridetest: read failed\n");
      exit(1);
    }
    loops[r.id] = r.loops;
    printf("child %d: tickets=%d elapsed=%d loops=%lu\n",
           r.id, r.tickets, r.elapsed, r.loops);
  }

  close(p[0]);
  for(i = 0; i < CHILDREN; i++)
    wait(0);

  if(loops[0] < loops[1] && loops[1] < loops[2])
    printf("stridetest: PASS\n");
  else
    printf("stridetest: check ratios manually; build with SCHED=STRIDE CPUS=1\n");

  exit(0);
}
