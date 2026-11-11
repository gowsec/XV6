#include "kernel/types.h"
#include "kernel/procinfo.h"
#include "user/user.h"

// Process states matching enum procstate in kernel/proc.h:
// UNUSED = 0, USED = 1, SLEEPING = 2, RUNNABLE = 3, RUNNING = 4, ZOMBIE = 5
static char *state_names[] = {
  [0] "UNUSED",
  [1] "USED",
  [2] "SLEEPING",
  [3] "RUNNABLE",
  [4] "RUNNING",
  [5] "ZOMBIE"
};

char*
format_state(int state)
{
  if(state >= 0 && state < 6 && state_names[state])
    return state_names[state];
  return "UNKNOWN";
}

void
test_current(void)
{
  printf("test_current: starting...\n");
  struct procinfo info;
  int my_pid = getpid();

  if(procinfo(my_pid, &info) < 0){
    printf("test_current: FAILED procinfo(%d) returned -1\n", my_pid);
    exit(1);
  }

  if(info.pid != my_pid){
    printf("test_current: FAILED expected pid %d, got %d\n", my_pid, info.pid);
    exit(1);
  }

  // A currently executing user program must have RUNNING state (4)
  if(info.state != 4){
    printf("test_current: FAILED expected state RUNNING(4), got %d (%s)\n",
           info.state, format_state(info.state));
    exit(1);
  }

  if(strcmp(info.name, "procinfotest") != 0){
    printf("test_current: FAILED expected name 'procinfotest', got '%s'\n", info.name);
    exit(1);
  }

  if(info.sz == 0){
    printf("test_current: FAILED process memory size is 0\n");
    exit(1);
  }

  printf("  Process: %s\n", info.name);
  printf("  PID: %d, PPID: %d\n", info.pid, info.ppid);
  printf("  State: %s\n", format_state(info.state));
  printf("  Memory: %d bytes\n", (int)info.sz);

  // Check parent process (e.g. sh)
  if(info.ppid > 0){
    struct procinfo parent_info;
    if(procinfo(info.ppid, &parent_info) < 0){
      printf("test_current: FAILED querying parent procinfo(%d) failed\n", info.ppid);
      exit(1);
    }
    if(parent_info.pid != info.ppid){
      printf("test_current: FAILED parent_info.pid %d != info.ppid %d\n",
             parent_info.pid, info.ppid);
      exit(1);
    }
    printf("  Parent Process: %s (PID: %d)\n", parent_info.name, parent_info.pid);
  }

  // Check init process (PID 1, must have ppid = 0)
  struct procinfo init_info;
  if(procinfo(1, &init_info) == 0){
    if(init_info.pid != 1 || init_info.ppid != 0){
      printf("test_current: FAILED init process PID %d expected PPID 0, got %d\n",
             init_info.pid, init_info.ppid);
      exit(1);
    }
    printf("  Init process verified: PID: %d, PPID: %d\n", init_info.pid, init_info.ppid);
  }

  printf("test_current: PASSED\n\n");
}

void
test_fork(void)
{
  printf("test_fork: testing fork child/parent relationships...\n");
  int parent_pid = getpid();
  int pid = fork();

  if(pid < 0){
    printf("test_fork: fork failed\n");
    exit(1);
  }

  if(pid == 0){
    // Child process
    struct procinfo cinfo;
    struct procinfo pinfo;
    int child_pid = getpid();

    if(procinfo(child_pid, &cinfo) < 0){
      printf("test_fork [child]: FAILED procinfo failed\n");
      exit(1);
    }

    if(cinfo.ppid != parent_pid){
      printf("test_fork [child]: FAILED expected ppid %d, got %d\n", parent_pid, cinfo.ppid);
      exit(1);
    }

    if(procinfo(cinfo.ppid, &pinfo) < 0){
      printf("test_fork [child]: FAILED query parent procinfo failed\n");
      exit(1);
    }

    if(pinfo.pid != parent_pid){
      printf("test_fork [child]: FAILED parent pid mismatch: %d != %d\n", pinfo.pid, parent_pid);
      exit(1);
    }

    exit(0);
  }

  int status = 0;
  wait(&status);
  if(status != 0){
    printf("test_fork: child failed with status %d\n", status);
    exit(1);
  }
  printf("test_fork: PASSED\n\n");
}

void
test_sbrk(void)
{
  printf("test_sbrk: testing memory size growth with sbrk...\n");
  struct procinfo before, after;

  if(procinfo(getpid(), &before) < 0){
    printf("test_sbrk: FAILED initial procinfo query\n");
    exit(1);
  }

  // Allocate 1 page (4096 bytes)
  sbrk(4096);

  if(procinfo(getpid(), &after) < 0){
    printf("test_sbrk: FAILED post-sbrk procinfo query\n");
    exit(1);
  }

  if(after.sz != before.sz + 4096){
    printf("test_sbrk: FAILED expected sz %d, got %d\n",
           (int)(before.sz + 4096), (int)after.sz);
    exit(1);
  }

  // Deallocate memory back
  sbrk(-4096);
  printf("  Before: %d bytes -> After: %d bytes (+4096 bytes verified)\n",
         (int)before.sz, (int)after.sz);
  printf("test_sbrk: PASSED\n\n");
}

void
test_invalid(void)
{
  printf("test_invalid: testing invalid arguments and boundary conditions...\n");
  struct procinfo info;

  // Invalid PIDs (must return -1)
  if(procinfo(-1, &info) != -1){
    printf("test_invalid: FAILED pid=-1 did not return -1\n");
    exit(1);
  }

  if(procinfo(0, &info) != -1){
    printf("test_invalid: FAILED pid=0 did not return -1\n");
    exit(1);
  }

  if(procinfo(9999, &info) != -1){
    printf("test_invalid: FAILED non-existent pid=9999 did not return -1\n");
    exit(1);
  }

  // Invalid user pointers (must return -1 without kernel panic)
  if(procinfo(getpid(), (struct procinfo*)0) != -1){
    printf("test_invalid: FAILED NULL pointer did not return -1\n");
    exit(1);
  }

  if(procinfo(getpid(), (struct procinfo*)0xffffffff) != -1){
    printf("test_invalid: FAILED high invalid pointer did not return -1\n");
    exit(1);
  }

  printf("test_invalid: PASSED\n\n");
}

void
test_stress(void)
{
  printf("test_stress: testing repeated calls and concurrency...\n");
  struct procinfo info;

  // 100 consecutive calls
  for(int i = 0; i < 100; i++){
    if(procinfo(getpid(), &info) < 0){
      printf("test_stress: FAILED iteration %d\n", i);
      exit(1);
    }
  }

  // Concurrent queries from multiple children
  for(int i = 0; i < 3; i++){
    int pid = fork();
    if(pid < 0){
      printf("test_stress: fork failed\n");
      exit(1);
    }
    if(pid == 0){
      for(int j = 0; j < 50; j++){
        struct procinfo cinfo;
        if(procinfo(getpid(), &cinfo) < 0)
          exit(1);
      }
      exit(0);
    }
  }

  for(int i = 0; i < 3; i++){
    int status = 0;
    wait(&status);
    if(status != 0){
      printf("test_stress: child failed\n");
      exit(1);
    }
  }

  printf("test_stress: PASSED\n\n");
}

int
main(int argc, char *argv[])
{
  printf("========================================\n");
  printf("procinfotest: starting test suite\n");
  printf("========================================\n");

  test_current();
  test_fork();
  test_sbrk();
  test_invalid();
  test_stress();

  printf("========================================\n");
  printf("procinfotest: ALL TESTS PASSED!\n");
  printf("========================================\n");
  exit(0);
}
