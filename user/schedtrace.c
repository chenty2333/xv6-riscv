#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"

#define CHILDREN 3
#define RUN_TICKS 400
#define SNAPSHOTS 8
#define SAMPLE_TICKS 20

static int tickets[CHILDREN] = { 1, 2, 4 };

static char*
state_name(int state)
{
  switch(state) {
  case 0:
    return "unused";
  case 1:
    return "used";
  case 2:
    return "sleep";
  case 3:
    return "runble";
  case 4:
    return "run";
  case 5:
    return "zombie";
  default:
    return "?";
  }
}

static int
find_proc(struct pstat ps[NPROC], int pid)
{
  int i;

  for(i = 0; i < NPROC; i++) {
    if(ps[i].pid == pid)
      return i;
  }
  return -1;
}

static void
worker(int id)
{
  int start;
  volatile uint64 loops;

  if(settickets(tickets[id]) < 0) {
    fprintf(2, "schedtrace: settickets(%d) failed\n", tickets[id]);
    exit(1);
  }

  loops = 0;
  start = uptime();
  while(uptime() - start < RUN_TICKS)
    loops++;

  exit(0);
}

static int
print_snapshot(int pids[CHILDREN], uint64 last_count[CHILDREN])
{
  struct pstat ps[NPROC];
  int i, idx;
  uint64 count, delta;

  if(getpinfo(ps) < 0) {
    fprintf(2, "schedtrace: getpinfo failed\n");
    return -1;
  }

  printf("\ntick %d\n", uptime());
  printf("pid tickets state  count delta stride pass\n");
  for(i = 0; i < CHILDREN; i++) {
    idx = find_proc(ps, pids[i]);
    if(idx < 0) {
      printf("%d missing\n", pids[i]);
      continue;
    }

    count = ps[idx].sched_count;
    delta = count - last_count[i];
    last_count[i] = count;
    printf("%d   %d       %s  %lu    +%lu   %lu %lu\n",
           ps[idx].pid,
           ps[idx].tickets,
           state_name(ps[idx].state),
           count,
           delta,
           ps[idx].stride,
           ps[idx].pass);
  }
  return 0;
}

int
main(int argc, char *argv[])
{
  int i, pid;
  int pids[CHILDREN];
  uint64 last_count[CHILDREN];

  memset(last_count, 0, sizeof(last_count));
  printf("schedtrace: SCHED=STRIDE CPUS=1 is recommended\n");

  for(i = 0; i < CHILDREN; i++) {
    pid = fork();
    if(pid < 0) {
      fprintf(2, "schedtrace: fork failed\n");
      exit(1);
    }
    if(pid == 0)
      worker(i);
    pids[i] = pid;
  }

  pause(5);
  for(i = 0; i < SNAPSHOTS; i++) {
    if(print_snapshot(pids, last_count) < 0)
      break;
    pause(SAMPLE_TICKS);
  }

  for(i = 0; i < CHILDREN; i++)
    kill(pids[i]);
  for(i = 0; i < CHILDREN; i++)
    wait(0);

  printf("\nschedtrace: trace complete\n");
  exit(0);
}
