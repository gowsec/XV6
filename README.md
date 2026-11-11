<div align="center">

# Project 1 — Xv6, Unix Utilities and System Call

**Hệ Điều Hành – CSC10007 · Trường ĐH Khoa học Tự nhiên, ĐHQG-HCM · Nhóm 20**

<p>
  <img src="https://img.shields.io/badge/build-passing-brightgreen">
  <img src="https://img.shields.io/badge/grade--lab--syscall_trace-6%2F6_passing-brightgreen">
  <img src="https://img.shields.io/badge/xv6-labs--2024-111111">
  <img src="https://img.shields.io/badge/arch-RISC--V_64--bit-111111">
  <img src="https://img.shields.io/badge/QEMU-%E2%89%A5_8.2-111111">
  <img src="https://img.shields.io/badge/toolchain-riscv64--unknown--elf--gcc-111111">
  <img src="https://img.shields.io/badge/language-C-111111">
  <img src="https://img.shields.io/badge/license-MIT-111111">
</p>

</div>

---

## Mục lục

1. [Giới thiệu đồ án](#1-giới-thiệu-đồ-án)
2. [Thành viên nhóm](#2-thành-viên-nhóm)
3. [Cấu hình môi trường](#3-cấu-hình-môi-trường)
4. [Cấu trúc thư mục đồ án](#4-cấu-trúc-thư-mục-đồ-án)
5. [Nội dung các bài tập](#5-nội-dung-các-bài-tập)
6. [Hướng dẫn cài đặt môi trường](#6-hướng-dẫn-cài-đặt-môi-trường)
7. [Hướng dẫn build và chạy xv6](#7-hướng-dẫn-build-và-chạy-xv6)
8. [Hướng dẫn kiểm thử](#8-hướng-dẫn-kiểm-thử)
9. [Hạn chế đã biết](#9-hạn-chế-đã-biết)
10. [Git Workflow](#10-git-workflow)
11. [Phân công thành viên](#11-phân-công-thành-viên)
12. [Tài liệu tham khảo](#12-tài-liệu-tham-khảo)
13. [Giấy phép](#13-giấy-phép)

---

## 1. Giới thiệu đồ án

Đồ án thuộc môn **Operating System (Hệ điều hành)**, được thực hiện trên **xv6**, một hệ điều hành giáo dục do MIT phát triển, nhằm tìm hiểu các nguyên lý cơ bản của hệ điều hành thông qua việc lập trình và mở rộng chức năng trên mã nguồn thực tế.

**Mục tiêu:** Làm quen với kiến trúc xv6, tìm hiểu cơ chế tương tác giữa chương trình người dùng (user space) và nhân hệ điều hành (kernel space), đồng thời thực hành xây dựng tiện ích Unix và bổ sung system call.

Đồ án gồm **4 bài tập**, chia thành hai nhóm:

| Nhóm | Bài tập | Mô tả ngắn |
|---|---|---|
| User Programs | `xargs` | Đọc stdin theo dòng và chạy lệnh cho từng dòng |
| User Programs | `tree` | In cây thư mục, duyệt đệ quy |
| System Calls | `tracing` | Theo dõi system call theo bitmask |
| System Calls | `procinfo` | Truy vấn thông tin tiến trình theo PID |

### 1.1. Nền tảng mã nguồn xv6

Dự án sử dụng mã nguồn **xv6-labs-2024** của MIT, cụ thể là branch `syscall`, làm nền tảng phát triển cho toàn bộ đồ án.

Repository kế thừa các thành phần cốt lõi của xv6, bao gồm kernel, chương trình người dùng, hệ thống file, cấu hình biên dịch và công cụ kiểm thử. Các thành phần này tạo thành một môi trường hệ điều hành hoàn chỉnh ở mức tối giản, cho phép nhóm triển khai và kiểm chứng các chức năng theo yêu cầu mà không cần xây dựng hệ điều hành từ đầu.

### 1.2. Kiến trúc và các thành phần chính

- **`kernel/`**: Mã nguồn nhân hệ điều hành, quản lý tiến trình, bộ nhớ, system call và các tài nguyên hệ thống.
- **`user/`**: Các chương trình chạy trong user space và thư viện hỗ trợ.
- **`mkfs/`**: Công cụ tạo filesystem image cho xv6.
- **`Makefile`**: Quy định quá trình biên dịch kernel, chương trình người dùng và khởi chạy hệ điều hành.
- **Các file cấu hình và công cụ kiểm thử**: Hỗ trợ quá trình phát triển, biên dịch và đánh giá kết quả thực hành.

### 1.3. Môi trường thực thi

Nhóm sử dụng **Ubuntu Linux** làm môi trường phát triển và **QEMU** để giả lập phần cứng, khởi chạy xv6. xv6 hoạt động bên trong QEMU, không thay thế hệ điều hành Ubuntu của máy ảo.

```text
Mã nguồn xv6 → Biên dịch bằng Makefile → Khởi chạy bằng QEMU → Phát triển và kiểm thử
```

---

## 2. Thành viên nhóm

**Giảng viên hướng dẫn:** ThS. Lê Quốc Hòa

| STT | Họ và tên | MSSV | Vai trò |
|:---:|---|:---:|---|
| 1 | Trương Minh Quang | 24120420 | Nhóm trưởng |
| 2 | Nguyễn Trọng An | 24120017 | Thành viên |
| 3 | Võ Văn Cường | 24120029 | Thành viên |
| 4 | Đào Ngọc Hưng | 24120319 | Thành viên |

Chi tiết phân công xem tại [mục 11](#11-phân-công-thành-viên).

---

## 3. Cấu hình môi trường

Môi trường dùng chung để phát triển và kiểm thử (Local VM):

| Thành phần | Thông tin |
|---|---|
| Hệ điều hành máy ảo (Guest OS) | Ubuntu 24.04 |
| Số vCPU | 2 |
| RAM | Khoảng 3.8 GiB |
| Dung lượng ổ đĩa | 20 GB |
| Git | 2.34.1 hoặc tương thích |
| Phiên bản QEMU | >= 8.x.x (kiểm thử trên 8.2.2) |
| RISC-V toolchain | `riscv64-unknown-elf-gcc` |
| Repository xv6 gốc | `git://g.csail.mit.edu/xv6-labs-2024` |
| Branch xv6 gốc | `syscall` |
| Commit xv6 gốc | `d5b09ff4571e07f6d996fdf7ce1533afc19a6978` |
| Commit nhập mã nguồn vào repository nhóm | `efcb79c` |

> **Lưu ý về QEMU:** kernel của bản lab 2024 dùng thanh ghi `stimecmp` (phần mở rộng Sstc) nên cần QEMU từ **7.2** trở lên. QEMU cũ hơn không khởi động được xv6.

> **Lưu ý về trình biên dịch:** với GCC 15 (ví dụ Ubuntu 26.04), mã nguồn gốc không build được ngay vì GCC 15 mặc định theo chuẩn C23. Nhóm đã bổ sung `-march=rv64gc -mabi=lp64` trong `Makefile` và sửa khai báo `rwsbrk()` thành `rwsbrk(char *s)` trong `user/usertests.c`. Các thay đổi này vẫn build sạch trên GCC 13 với `-Werror`.

---

## 4. Cấu trúc thư mục đồ án

Repository được xây dựng trên mã nguồn **xv6-labs-2024 (branch `syscall`)**. Cấu trúc dưới đây là bản rút gọn, chỉ liệt kê các thành phần liên quan trực tiếp đến bốn bài tập.

```text
XV6/
├── .editorconfig             # Quy tắc định dạng mã nguồn
├── .gitignore                # Các file và thư mục Git bỏ qua
├── .dir-locals.el            # Cấu hình cho Emacs
├── .gdbinit.tmpl-riscv       # Cấu hình mẫu cho GDB
├── LICENSE                   # Giấy phép mã nguồn xv6
├── README                    # Tài liệu gốc của xv6
├── README.md                 # Tài liệu đồ án của nhóm
├── Makefile                  # Biên dịch xv6 và đăng ký UPROGS
├── grade-lab-syscall         # Công cụ kiểm tra bài lab syscall
├── gradelib.py               # Thư viện hỗ trợ kiểm thử
│
├── conf/                     # Các file cấu hình
├── mkfs/                     # Công cụ tạo filesystem image
│
├── kernel/                   # Mã nguồn kernel xv6
│   ├── main.c                # Khởi tạo và khởi động kernel
│   ├── param.h               # Các hằng số hệ thống
│   ├── proc.h                # Khai báo cấu trúc tiến trình
│   ├── proc.c                # Quản lý tiến trình, fork(), getprocinfo()
│   ├── syscall.h             # Định nghĩa số hiệu system call
│   ├── syscall.c             # Điều phối system call
│   ├── sysproc.c             # Xử lý system call liên quan tiến trình
│   ├── defs.h                # Prototype dùng chung trong kernel
│   └── procinfo.h            # Khai báo struct procinfo
│
└── user/                     # Chương trình và thư viện user space
    ├── user.h                # Khai báo các hàm và system call
    ├── usys.pl               # Sinh mã gọi system call
    ├── ls.c                  # Chương trình ls, tham khảo cho tree
    ├── trace.c               # Chương trình trace có sẵn trong mã nguồn xv6
    ├── xargs.c               # Bài 1: xargs
    ├── tree.c                # Bài 2: tree
    └── procinfotest.c        # Chương trình kiểm thử procinfo

```

### 4.1. Phạm vi phát triển

| Bài tập | File phát triển chính | Công việc |
|---|---|---|
| xargs | `user/xargs.c`, `Makefile` | Tạo chương trình xargs và đăng ký trong `UPROGS` |
| tree | `user/tree.c`, `Makefile` | Tạo chương trình tree và đăng ký trong `UPROGS` |
| tracing | `kernel/syscall.h`, `kernel/syscall.c`, `kernel/sysproc.c`, `kernel/proc.h`, `kernel/proc.c`, `user/user.h`, `user/usys.pl`, `Makefile` | Triển khai system call `trace(mask)`, dùng `user/trace.c` có sẵn và đăng ký `_trace` trong `UPROGS` |
| procinfo | `kernel/procinfo.h`, `kernel/syscall.h`, `kernel/syscall.c`, `kernel/sysproc.c`, `kernel/proc.c`, `kernel/defs.h`, `user/user.h`, `user/usys.pl`, `user/procinfotest.c`, `Makefile` | Triển khai system call `procinfo`, chương trình kiểm thử `procinfotest` và đăng ký trong `UPROGS` |


> **Lưu ý tích hợp:** Hai bài `tracing` và `procinfo` cùng chỉnh sửa một số file đăng ký system call. Nhóm dùng một commit nền chung để thống nhất số hiệu syscall (`SYS_trace = 22`, `SYS_procinfo = 23`) và các khai báo cần thiết trước khi triển khai logic riêng. Chi tiết xem tại [Phối hợp triển khai System Call](#phối-hợp-triển-khai-system-call-tracing-và-procinfo).


---

## 5. Nội dung các bài tập

### 5.1. xargs — User Program

Viết chương trình `user/xargs.c` đọc từng dòng từ **standard input (stdin)** và chạy lệnh được chỉ định với nội dung dòng đó được bổ sung vào danh sách tham số. Bài tập sử dụng các system call `fork()`, `exec()` và `wait()`.

### 5.2. tree — User Program

Viết chương trình `tree` hiển thị cấu trúc file và thư mục dưới dạng cây. Chương trình duyệt đệ quy các thư mục con, thụt lề theo cấp, thêm dấu `/` sau tên thư mục, bỏ qua `.` và `..`, đồng thời xử lý lỗi truy cập file hoặc thư mục.

### 5.3. tracing — System Call

Bổ sung system call `trace(mask)` để theo dõi các system call được chọn theo bitmask. Khi một system call được theo dõi trả về, kernel in **PID, tên system call và giá trị trả về**. Cấu hình trace áp dụng cho tiến trình gọi và các tiến trình con được tạo sau đó.

### 5.4. procinfo — System Call

Bổ sung system call `int procinfo(int pid, struct procinfo *info)` để lấy thông tin của tiến trình theo PID, gồm **PID, PPID, trạng thái tiến trình, kích thước bộ nhớ và tên tiến trình**, sau đó trả dữ liệu về chương trình người dùng. Trả về `0` nếu thành công, `-1` nếu thất bại.

```c
struct procinfo {
  int pid;        // Process ID
  int ppid;       // Parent process ID
  int state;      // Process state (enum procstate)
  uint64 sz;      // Size of process memory (bytes)
  char name[16];  // Process name
};
```

---

## 6. Hướng dẫn cài đặt môi trường

Các lệnh dưới đây dành cho **Ubuntu 24.04**.

**Bước 1. Cài các công cụ cần thiết**

```bash
sudo apt update
sudo apt install -y git build-essential gdb-multiarch python3 \
    qemu-system-misc gcc-riscv64-unknown-elf binutils-riscv64-unknown-elf
```

**Bước 2. Kiểm tra phiên bản**

```bash
riscv64-unknown-elf-gcc --version
qemu-system-riscv64 --version     # cần >= 7.2, nhóm dùng 8.2.2
git --version
```

**Bước 3. Tải mã nguồn**

```bash
git clone https://github.com/gowsec/XV6.git
cd XV6
git checkout dev                  # nhánh tích hợp; dùng main để lấy bản ổn định
```

---

## 7. Hướng dẫn build và chạy xv6

```bash
make clean          # xóa bản build cũ và file system image
make qemu           # biên dịch và khởi chạy xv6 trong QEMU
```

Khi thấy dòng `init: starting sh` và dấu nhắc `$` là xv6 đã khởi động xong. Để thoát QEMU, nhấn `Ctrl-A` rồi `X`.

> Thay đổi trong file system của xv6 được giữ lại giữa các lần chạy `qemu`. Muốn có file system sạch, chạy `make clean` rồi `make qemu`.

---

## 8. Hướng dẫn kiểm thử

Tất cả lệnh dưới đây gõ trong shell của xv6 (sau `make qemu`), trừ khi có ghi chú khác. Kết quả thực tế của nhóm trùng khớp với kết quả mong đợi ở mọi trường hợp.

### Bài 1 — xargs

**File:** `user/xargs.c`, đăng ký `$U/_xargs` trong `UPROGS`.

```text
$ echo hello too | xargs echo bye
bye hello too

$ (echo 1 ; echo 2) | xargs -n 1 echo
1
2

$ (echo hello ; echo ; echo world) | xargs echo
hello
world

$ (echo one ; echo two ; echo three ; echo four) | xargs echo
one
two
three
four
```

| Trường hợp | Kết quả |
|---|---|
| Ghép dòng đọc được vào sau các đối số ban đầu | Đúng |
| Mỗi dòng đầu vào chạy lệnh đúng một lần | Đúng |
| Chấp nhận tùy chọn `-n 1` | Đúng |
| Bỏ qua dòng rỗng | Đúng |

### Bài 2 — tree

**File:** `user/tree.c`, đăng ký `$U/_tree` trong `UPROGS`.

**Mô tả:** `tree [thư mục]` in cấu trúc thư mục dạng cây: duyệt đệ quy, thụt lề 2 dấu cách mỗi tầng, thư mục có `/` ở cuối, bỏ qua `.` và `..`. Không có tham số thì dùng `.`.

```text
$ mkdir a
$ echo > a/b
$ mkdir a/aa
$ echo > a/aa/b
$ echo > a/aa/c
$ mkdir a/ab
$ echo > a/ab/d
$ tree a
```

**Kết quả mong đợi** (theo đề) — **kết quả thực tế giống hoàn toàn**:

```text
a/
  b
  aa/
    b
    c
  ab/
    d
```

**Các trường hợp kiểm thử thêm:**

| Lệnh | Kết quả mong đợi | Kết quả thực tế |
|---|---|---|
| `tree .` | In `./`, sau đó toàn bộ file trong thư mục hiện tại, kể cả `a/` và các thư mục con | Đúng |
| `tree` (không tham số) | Giống `tree .` | Đúng |
| `tree /` | Dòng gốc là `/` (không bị `//`), in toàn bộ filesystem | Đúng |
| `mkdir a/aa/aaa`, `echo > a/aa/aaa/file`, `tree a` | `aaa/` thụt 2 tầng, `file` thụt 3 tầng | Đúng |
| `tree khongco` | In `tree: cannot open khongco` và thoát với mã lỗi 1 | Đúng |
| `tree README` | In `tree: README is not a directory` | Đúng |
| `tree a a` | In `Usage: tree [directory]` | Đúng |

### Bài 3 — tracing

**File:** `kernel/syscall.h`, `kernel/syscall.c`, `kernel/sysproc.c`, `kernel/proc.h`, `kernel/proc.c`, `user/user.h`, `user/usys.pl`; đăng ký `$U/_trace` trong `UPROGS`.

```text
$ trace 32 grep hello README
3: syscall read -> 1023
3: syscall read -> 971
3: syscall read -> 298
3: syscall read -> 0

$ trace 2147483647 grep hello README
4: syscall trace -> 0
4: syscall exec -> 3
4: syscall open -> 3
4: syscall read -> 1023
4: syscall read -> 971
4: syscall read -> 298
4: syscall read -> 0
4: syscall close -> 0

$ grep hello README
$                                  (không có dòng vết nào)

$ trace 2 usertests forkforkfork   (vết fork của mọi tiến trình con cháu)
```

> Số byte của các lần `read` khác ví dụ trong đề (1023, 966, 70) vì `README` của phiên bản xv6 nhóm dùng dài 2292 byte (1023 + 971 + 298). PID có thể khác giữa các lần chạy.

**Chấm tự động** (chạy trên Linux, trong thư mục mã nguồn, không phải trong xv6):

```bash
./grade-lab-syscall trace
```

Kết quả: cả 6 bài kiểm tra (`trace 32 grep`, `trace close grep`, `trace exec + open grep`, `trace all grep`, `trace nothing`, `trace children`) đều **OK**.

### Bài 4 — procinfo

**File:** `kernel/procinfo.h`, `kernel/proc.c` (`getprocinfo()`), `kernel/sysproc.c` (`sys_procinfo()`), `kernel/defs.h`, `user/procinfotest.c`; đăng ký `$U/_procinfotest` trong `UPROGS`.

```text
$ procinfotest
```

Chương trình gồm 5 nhóm test:

| Nhóm test | Nội dung kiểm tra | Kết quả |
|---|---|---|
| `test_current` | `pid`, `name`, `state = RUNNING`, `sz > 0`; tra cứu được tiến trình cha; `init` có `ppid = 0` | Đạt |
| `test_fork` | Tiến trình con có `ppid` bằng PID của cha | Đạt |
| `test_sbrk` | `sbrk(4096)` làm `sz` tăng đúng 4096 byte | Đạt |
| `test_invalid` | `pid = -1`, `0`, `9999`, con trỏ `NULL`, địa chỉ ngoài vùng nhớ đều trả `-1`, kernel không panic | Đạt |
| `test_stress` | 100 lần gọi liên tiếp và 3 tiến trình con gọi đồng thời, không deadlock | Đạt |

**Kết quả thực tế** (đoạn đầu):

```text
test_current: starting...
  Process: procinfotest
  PID: 3, PPID: 2
  State: RUNNING
  Memory: 20480 bytes
  Parent Process: sh (PID: 2)
  Init process verified: PID: 1, PPID: 0
test_current: PASSED
...
procinfotest: ALL TESTS PASSED!
```

**Kiểm thử tích hợp với `trace`.** `8388608` là `1 << 23`, tức bit của `SYS_procinfo`:

```text
$ trace 8388608 procinfotest
```

Hệ thống in các dòng `<pid>: syscall procinfo -> 0` ở các lần gọi thành công, `-> -1` ở các lần gọi sai tham số, và bộ test vẫn kết thúc với `ALL TESTS PASSED!`.

### Kiểm thử toàn bộ bằng usertests

```text
$ usertests
```

Khi chạy toàn bộ `usertests`, chỉ có hai test `sbrkmuch` và `bsstest` báo lỗi. Hai test này **cũng lỗi trên mã nguồn gốc** của lab khi chưa có thay đổi nào của nhóm. Nguyên nhân: `Makefile` build với cờ `-DLAB_SYSCALL`, khiến `uvmalloc()` trong `kernel/vm.c` bỏ qua bước xóa về 0 trang nhớ mới cấp, và `kernel/kalloc.c` bỏ qua bước ghi dữ liệu rác lên trang nhớ. Lab cố ý làm vậy cho bài "attack". Nhóm không sửa `vm.c` và `kalloc.c` vì hai tệp này nằm ngoài phạm vi bốn bài tập.

> `./grade-lab-syscall` không có đối số sẽ chạy cả phần `sysinfo` của lab gốc, phần này không thuộc đồ án. Vì vậy chỉ dùng `./grade-lab-syscall trace`.

---

## 9. Hạn chế đã biết

| Bài | Hạn chế |
|---|---|
| xargs | `-n` chỉ nhận giá trị `1`; mỗi dòng đầu vào luôn ứng với một lần chạy lệnh; dòng dài tối đa 511 ký tự; chưa hỗ trợ dấu nháy |
| tree | Chỉ hiển thị tối đa 13 cấp thư mục lồng nhau do giới hạn `NOFILE = 16`; đường dẫn không dài quá `MAXPATH = 128` |
| tracing | Mask 32 bit nên chỉ theo dõi system call có số hiệu nhỏ hơn 32; dòng vết của kernel có thể xen kẽ với dữ liệu user in ra |
| procinfo | Cấu trúc tạm trên kernel stack chưa được `memset` về 0 trước khi `copyout`; bộ test chưa kiểm tra trực tiếp trường hợp shell đầu tiên có `ppid = 1` |

---

## 10. Git Workflow

Nhóm sử dụng Git và GitHub để quản lý mã nguồn, theo dõi tiến độ và phối hợp phát triển.

### Quy ước Branch

| Branch | Mục đích |
|---|---|
| `main` | Lưu trữ phiên bản ổn định, dùng để nộp bài |
| `dev` | Tích hợp và kiểm thử các chức năng |
| `feature/xargs` | Phát triển bài xargs |
| `feature/tree` | Phát triển bài tree |
| `feature/tracing` | Phát triển bài tracing |
| `feature/procinfo` | Phát triển bài procinfo |

### Quy trình làm việc

1. Mỗi thành viên làm việc trên feature branch tương ứng.
2. Commit và push các thay đổi lên GitHub.
3. Tạo Pull Request (PR) từ feature branch vào `dev`.
4. Các thành viên review code trước khi merge.
5. Sau khi tích hợp, nhóm build và kiểm thử toàn bộ chức năng trên `dev`.
6. Khi phiên bản ổn định, nhóm trưởng merge `dev` vào `main`.

### Phối hợp triển khai System Call (Tracing và Procinfo)

Hai bài `tracing` và `procinfo` cùng chỉnh sửa một số file đăng ký system call (`syscall.h`, `syscall.c`, `user.h`, `usys.pl`, `Makefile`). Để tránh trùng số hiệu, sai lệch khai báo và hạn chế merge conflict, nhóm thống nhất quy trình sau:

**1. Chuẩn bị phần khai báo chung**

- Thống nhất số hiệu `SYS_trace = 22` và `SYS_procinfo = 23` dựa trên các số hiệu hiện có trong `kernel/syscall.h`.
- Chuẩn bị một commit nền chung chứa các khai báo và đăng ký cần thiết cho cả hai system call (số hiệu, mảng tên syscall, stub, prototype).
- Commit nền được tích hợp vào `dev` trước khi hai thành viên triển khai phần logic riêng.

**2. Đồng bộ feature branch**

Sau khi commit nền được merge vào `dev`:

- Đồng bộ `dev` vào `feature/procinfo` và `feature/tracing`.
- Không tự ý thay đổi số hiệu system call đã thống nhất.

**3. Phân chia phần triển khai**

- **Procinfo:** `getprocinfo()`, `sys_procinfo()` và chương trình kiểm thử `procinfotest`.
- **Tracing:** `sys_trace()`, trace mask, cơ chế kế thừa qua `fork()` và logic in log syscall.

**4. Tích hợp và kiểm thử**

- Mỗi thành viên tạo Pull Request từ feature branch vào `dev`.
- Kiểm tra và xử lý merge conflict nếu có.
- Sau khi cả hai chức năng được tích hợp, build và kiểm thử lại toàn bộ.
- Đặc biệt kiểm tra `procinfo` khi chạy thông qua `trace` (`trace 8388608 procinfotest`).

**Lưu ý:**

- Không trực tiếp push code lên `main`.
- Các thay đổi liên quan đến System Call cần được thống nhất giữa thành viên phụ trách `tracing` và `procinfo`.
- Tất cả thành viên sử dụng cùng một phiên bản xv6 và cùng commit gốc.

---

## 11. Phân công thành viên

| Thành viên | MSSV | Bài phụ trách | Công việc bổ sung |
|---|:---:|---|---|
| Trương Minh Quang (nhóm trưởng) | 24120420 | xargs | Quản lý GitHub, review PR, tích hợp và đóng gói sản phẩm; tổng hợp báo cáo |
| Nguyễn Trọng An | 24120017 | tree | Chuẩn hóa tài liệu README |
| Võ Văn Cường | 24120029 | tracing | Chốt số hiệu syscall và gộp phần đăng ký chung vào `dev`; hỗ trợ tích hợp và kiểm thử system call; tổng hợp báo cáo |
| Đào Ngọc Hưng | 24120319 | procinfo | Xây dựng test case, phối hợp kiểm thử |

**Review chéo theo vòng:** Quang review `procinfo`, An review `xargs`, Cường review `tree`, Hưng review `tracing`.

### Trách nhiệm chung

- Mỗi thành viên chịu trách nhiệm triển khai và kiểm thử bài được phân công.
- Mỗi thành viên tự viết nội dung báo cáo cho phần mình phụ trách.
- Các thành viên phối hợp review code và xử lý lỗi tích hợp.
- Nhóm trưởng chịu trách nhiệm kiểm tra phiên bản cuối trước khi nộp.

---

## 12. Tài liệu tham khảo

1. MIT PDOS, [xv6 Utilities Lab](https://pdos.csail.mit.edu/6.1810/2024/labs/util.html), 2024.
2. MIT PDOS, [xv6 System Calls Lab](https://pdos.csail.mit.edu/6.1810/2023/labs/syscall.html), 2023.
3. [File Descriptors and Open Files](https://www.youtube.com/watch?v=oWuVGDese4k), YouTube.

---

## 13. Giấy phép

Mã nguồn nền xv6 được phát hành theo giấy phép **MIT**, xem file [`LICENSE`](LICENSE).