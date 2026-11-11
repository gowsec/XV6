#ifndef PROCINFO_H
#define PROCINFO_H

// Process information returned by the procinfo() system call.
// Shared by the kernel and user programs; include kernel/types.h first.
struct procinfo {
  int pid;        // Process ID
  int ppid;       // Parent process ID
  int state;      // Process state (enum procstate in kernel/proc.h)
  uint64 sz;      // Size of process memory (bytes)
  char name[16];  // Process name
};

#endif
