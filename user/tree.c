#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

// In khoảng trắng thụt lề: 2 dấu cách cho mỗi tầng độ sâu.
void
indent(int depth)
{
  for(int i = 0; i < depth; i++)
    printf("  ");
}

// In các mục bên trong thư mục `path`, mỗi mục thụt lề `depth` tầng.
// Gặp thư mục con thì gọi đệ quy với depth + 1.
// Trả về 0 nếu thành công, -1 nếu không mở được thư mục.
int
tree(char *path, int depth)
{
  char buf[MAXPATH], *p;
  int fd;
  struct dirent de;
  struct stat st;

  // 1. Mở thư mục để đọc.
  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "tree: cannot open %s\n", path);
    return -1;
  }

  // 2. Kiểm tra buf đủ chỗ cho: path + '/' + tên (tối đa DIRSIZ) + '\0'.
  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
    fprintf(2, "tree: path too long: %s\n", path);
    close(fd);
    return -1;
  }

  // 3. Chép path vào buf, thêm '/' (nếu chưa có). p trỏ vào chỗ sẽ ghi tên con.
  strcpy(buf, path);
  p = buf + strlen(buf);
  if(p > buf && p[-1] != '/')
    *p++ = '/';

  // 4. Đọc lần lượt từng mục (struct dirent) trong thư mục.
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)            // ô trống (file đã bị xóa) -> bỏ qua
      continue;

    // Chép tên vào sau dấu '/'. Tên có thể dài đúng DIRSIZ mà không
    // có '\0' ở cuối, nên tự đặt '\0' ở vị trí DIRSIZ cho chắc.
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;

    // Không đi vào "." và ".." để tránh lặp vô hạn.
    if(strcmp(p, ".") == 0 || strcmp(p, "..") == 0)
      continue;

    // Lấy thông tin của mục con (là file hay thư mục).
    if(stat(buf, &st) < 0){
      fprintf(2, "tree: cannot stat %s\n", buf);
      continue;                 // báo lỗi nhưng chạy tiếp
    }

    indent(depth);
    if(st.type == T_DIR){
      printf("%s/\n", p);
      tree(buf, depth + 1);     // đệ quy vào thư mục con
    } else {
      printf("%s\n", p);
    }
  }

  close(fd);
  return 0;
}

int
main(int argc, char *argv[])
{
  char *path;
  struct stat st;

  if(argc > 2){
    fprintf(2, "Usage: tree [directory]\n");
    exit(1);
  }
  path = (argc < 2) ? "." : argv[1];   // không có tham số -> dùng "."

  // Kiểm tra thư mục gốc tồn tại và đúng là thư mục.
  if(stat(path, &st) < 0){
    fprintf(2, "tree: cannot open %s\n", path);
    exit(1);
  }
  if(st.type != T_DIR){
    fprintf(2, "tree: %s is not a directory\n", path);
    exit(1);
  }

  // In dòng gốc, ví dụ "a/". Nếu path đã kết thúc bằng '/' (như "/") thì không thêm.
  if(path[strlen(path) - 1] == '/')
    printf("%s\n", path);
  else
    printf("%s/\n", path);

  if(tree(path, 1) < 0)
    exit(1);
  exit(0);
}
