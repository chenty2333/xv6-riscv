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

void
worker(int id, int fd)
{
  struct result r;
  int start;

  r.id = id;
  r.loops = 0;
  start = uptime();

  // LAB ch1: 构造 CPU-bound 工作负载：从 start 起运行 RUN_TICKS
  // 个 tick。循环体不要 sleep 或读写文件，只累加 r.loops，
  // 让进程依赖时钟中断被 RR 调度器切换。
  while(uptime() - start < RUN_TICKS) {
    // LAB ch1: 增加 r.loops。
  }

  r.elapsed = uptime() - start;
  // LAB ch1: 将 struct result 写入 pipe 写端，父进程会读取
  // sizeof(r) 字节。写完关闭 fd，并用 exit(0) 结束子进程。
  write(fd, &r, sizeof(r));
  close(fd);
  exit(0);
}

int
main(int argc, char *argv[])
{
  // LAB ch1: 本程序需要完整补全 pipe -> fork -> read -> wait。
  // 建议准备 p[2]、i、pid，以及接收结果的 struct result r。
  //
  // pipe(p) 后，p[0] 是读端，p[1] 是写端。子进程写 struct result，
  // 父进程读取 CHILDREN 个结果并打印；pipe 失败就打印错误并 exit(1)。
  //
  // 用 for 循环 fork CHILDREN 个子进程。pid < 0 表示失败；pid == 0
  // 是子进程，需要 close(p[0]) 后调用 worker(i, p[1])；pid > 0
  // 是父进程，继续创建下一个子进程。
  //
  // 父进程创建完后关闭写端 p[1]，打印测试标题，再循环读取
  // read(p[0], &r, sizeof(r))。每次都应读到 sizeof(r) 字节；
  // 读到后打印 child id、elapsed 和 loops，读失败就 exit(1)。
  //
  // 最后关闭 p[0]，wait CHILDREN 次回收子进程。三个子进程都能
  // 输出 loops > 0，就说明 RR 基础调度路径已经跑通。

  exit(1);
}
