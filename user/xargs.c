
#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAX_LINE 512

// Kiem tra ky tu phan cach
int
is_space(char c)
{
  return c == ' ' || c == '\t';
}

// Tach mot dong thanh cac arguments.
// Tra ve -1 neu so arguments vuot qua max_tokens.
int
split_line(char *line, char *tokens[], int max_tokens)
{
  int count = 0;
  int i = 0;

  while(line[i] != '\0') {
    // Bo qua khoang trang
    while(line[i] != '\0' && is_space(line[i])) {
      line[i] = '\0';
      i++;
    }

    // Da den cuoi chuoi
    if(line[i] == '\0') {
      break;
    }

    // Kiem tra gioi han truoc khi ghi vao mang
    if(count >= max_tokens) {
      return -1;
    }

    // Luu dia chi bat dau cua mot tu
    tokens[count++] = &line[i];

    // Di chuyen den cuoi tu
    while(line[i] != '\0' && !is_space(line[i])) {
      i++;
    }
  }

  return count;
}

// Xu ly mot dong stdin va thuc thi lenh
void
run_command(char *line, char *base_argv[], int base_argc)
{
  char *tokens[MAXARG];

  // So vi tri con lai danh cho arguments tu stdin
  int max_tokens = MAXARG - 1 - base_argc;

  int token_count = split_line(line, tokens, max_tokens);

  // Bo qua dong rong
  if(token_count == 0) {
    return;
  }

  // Kiem tra loi vuot qua gioi han arguments
  if(token_count < 0) {
    fprintf(2, "xargs: too many arguments\n");
    exit(1);
  }

  // Tao mang arguments cuoi cung
  char *exec_argv[MAXARG];
  int exec_argc = 0;

  // Sao chep arguments ban dau
  for(int i = 0; i < base_argc; i++) {
    exec_argv[exec_argc++] = base_argv[i];
  }

  // Noi arguments tu stdin
  for(int i = 0; i < token_count; i++) {
    exec_argv[exec_argc++] = tokens[i];
  }

  // Ket thuc mang arguments
  exec_argv[exec_argc] = 0;

  // Tao tien trinh con
  int pid = fork();

  if(pid < 0) {
    fprintf(2, "xargs: fork failed\n");
    exit(1);
  }

  if(pid == 0) {
    // Tien trinh con thuc thi lenh
    exec(base_argv[0], exec_argv);

    // Chi chay neu exec that bai
    fprintf(2, "xargs: exec %s failed\n", base_argv[0]);
    exit(1);
  }

  // Tien trinh cha cho tien trinh con ket thuc
  if(wait(0) < 0) {
    fprintf(2, "xargs: wait failed\n");
    exit(1);
  }
}

int
main(int argc, char *argv[])
{
  int start = 1;

  // Xu ly tuy chon -n 1
  if(argc >= 2 && strcmp(argv[1], "-n") == 0) {
    if(argc < 4 || strcmp(argv[2], "1") != 0) {
      fprintf(2, "Usage: xargs [-n 1] command [args...]\n");
      exit(1);
    }

    start = 3;
  }

  // Kiem tra command
  if(start >= argc) {
    fprintf(2, "Usage: xargs [-n 1] command [args...]\n");
    exit(1);
  }

  // Luu cac arguments ban dau
  char *base_argv[MAXARG];
  int base_argc = 0;

  for(int i = start; i < argc; i++) {
    if(base_argc >= MAXARG - 1) {
      fprintf(2, "xargs: too many arguments\n");
      exit(1);
    }

    base_argv[base_argc++] = argv[i];
  }

  base_argv[base_argc] = 0;

  // Doc stdin tung ky tu
  char line[MAX_LINE];
  int len = 0;
  char c;
  int n;

  while((n = read(0, &c, 1)) > 0) {
    if(c == '\n') {
      line[len] = '\0';

      run_command(line, base_argv, base_argc);

      len = 0;
    } else {
      // Kiem tra tran bo dem
      if(len >= MAX_LINE - 1) {
        fprintf(2, "xargs: input line too long\n");
        exit(1);
      }

      line[len++] = c;
    }
  }

  // Xu ly dong cuoi neu khong co '\n'
  if(len > 0) {
    line[len] = '\0';
    run_command(line, base_argv, base_argc);
  }

  // Kiem tra loi doc stdin
  if(n < 0) {
    fprintf(2, "xargs: read failed\n");
    exit(1);
  }

  exit(0);
}
