# Báo cáo Triển khai System Call `procinfo` (Issue #5)

**Thành viên thực hiện:** Đào Ngọc Hưng  
**MSSV:** 24120319  
**Branch:** `feature/procinfo`  
**Liên kết Issue:** Refs #5  

---

## 1. Mục đích và Yêu cầu

Triển khai system call `procinfo(int pid, struct procinfo *info)` trong kernel xv6 để thu thập thông tin của một tiến trình dựa theo PID:
- **PID:** ID của tiến trình cần truy vấn.
- **PPID:** ID của tiến trình cha (nếu không có cha như `init`, trả về `0`).
- **Trạng thái (State):** Giá trị số `enum procstate` (`UNUSED`, `USED`, `SLEEPING`, `RUNNABLE`, `RUNNING`, `ZOMBIE`).
- **Kích thước bộ nhớ (sz):** Dung lượng bộ nhớ ảo hiện tại của tiến trình tính theo byte.
- **Tên tiến trình (name):** Chuỗi ký tự tên chương trình (tối đa 16 byte).

---

## 2. Thiết kế và Giải pháp Kỹ thuật

### 2.1. Cấu trúc dữ liệu
Định nghĩa trong `kernel/procinfo.h`:
```c
struct procinfo {
  int pid;        // Process ID
  int ppid;       // Parent process ID
  int state;      // Process state (enum procstate)
  uint64 sz;      // Size of process memory (bytes)
  char name[16];  // Process name
};
```

### 2.2. Chiến lược đồng bộ và Quản lý Khóa (Lock Management)
- **Tránh Deadlock tuyệt đối:**
  - `p->parent` được bảo vệ bởi `wait_lock`, trong khi các trường khác của tiến trình được bảo vệ bởi `p->lock`.
  - Hàm `getprocinfo()` luôn tuân thủ nghiêm ngặt thứ tự: **Lấy `wait_lock` trước vòng lặp, sau đó mới lấy `p->lock` cho từng tiến trình**. Tuyệt đối không lấy `wait_lock` khi đang giữ `p->lock`.
  - Không lấy khóa của cha (`parent->lock`); khi đã giữ `wait_lock`, đọc `p->parent->pid` là an toàn.
  - Đảm bảo **nhả toàn bộ khóa** (`release(&p->lock)` và `release(&wait_lock)`) ở mọi nhánh thoát (tìm thấy hoặc không tìm thấy).

### 2.3. Tách biệt Kernel Space và User Space (`copyout`)
- `sys_procinfo()` trong `kernel/sysproc.c` cấp phát một cấu trúc tạm `struct procinfo info` trên kernel stack.
- Thu thập dữ liệu qua `getprocinfo(pid, &info)`.
- Chỉ sau khi `getprocinfo()` đã hoàn thành và **nhả toàn bộ khóa**, kernel mới gọi:
  ```c
  copyout(myproc()->pagetable, uaddr, (char *)&info, sizeof(info))
  ```
- Nếu `uaddr` không hợp lệ (con trỏ `NULL`, ngoài giới hạn bộ nhớ ảo), `copyout` trả về `-1` an toàn, kernel không bị crash hoặc panic.

---

## 3. Nội dung các thay đổi trong mã nguồn

| File | Thay đổi chính |
|---|---|
| `kernel/defs.h` | Khai báo forward `struct procinfo;` và prototype `int getprocinfo(int, struct procinfo*);` |
| `kernel/proc.c` | Include `procinfo.h`, cài đặt hàm `getprocinfo()` duyệt bảng `proc[]` an toàn với locking |
| `kernel/sysproc.c` | Include `procinfo.h`, cài đặt `sys_procinfo()` với `argint`, `argaddr` và `copyout` |
| `user/procinfotest.c` | Xây dựng bộ test toàn diện kiểm tra mọi tiêu chí nghiệm thu |

---

## 4. Kịch bản và Kết quả Kiểm thử (`user/procinfotest.c`)

Chương trình kiểm thử thực hiện 5 nhóm test case:

| Nhóm Test | Mục đích | Kết quả mong đợi | Trạng thái |
|---|---|---|:---:|
| `test_current` | Lấy thông tin tiến trình hiện tại và tiến trình `init` (PID 1) | `pid == getpid()`, `state == RUNNING(4)`, `name == "procinfotest"`, `sz > 0`; `init` có `ppid == 0` | Đạt |
| `test_fork` | Kiểm tra quan hệ cha - con qua `fork()` | Tiến trình con có `child.ppid == parent_pid`, truy vấn cha trả về đúng PID cha | Đạt |
| `test_sbrk` | Kiểm tra kích thước bộ nhớ mở rộng | `sbrk(4096)` làm `info.sz` tăng chính xác 4096 byte | Đạt |
| `test_invalid` | Kiểm tra tham số không hợp lệ và con trỏ lỗi | `pid <= 0`, `pid = 9999`, con trỏ `NULL`, địa chỉ ngoài biên đều trả về `-1`, kernel không crash | Đạt |
| `test_stress` | Kiểm tra gọi liên tục và đồng thời (concurrency) | Chạy 100 lần liên tiếp và 3 tiến trình con đồng thời; không deadlock, không rò rỉ khóa | Đạt |

### Hình ảnh minh chứng kết quả kiểm thử thực tế

#### 4.1. Kiểm thử toàn diện chức năng (`procinfotest`)
Toàn bộ 5 nhóm kiểm thử (`test_current`, `test_fork`, `test_sbrk`, `test_invalid`, `test_stress`) thực thi thành công, pass 100%:

![Kết quả chạy procinfotest](https://raw.githubusercontent.com/gowsec/XV6/feature/procinfo/report/procinfo_report/evidences/evidence_procinfotest.png)

#### 4.2. Kiểm thử tích hợp cùng tính năng `trace` (`trace 8388608 procinfotest`)
Hệ thống ghi nhận và in log đầy đủ các lệnh gọi hệ thống `procinfo` thành công (`-> 0`) và khi bắt lỗi tham số (`-> -1` trong `test_invalid`), kết thúc thành công với `ALL TESTS PASSED!`:

- **Đoạn đầu (khởi chạy test suite, kiểm tra các case đơn lẻ và bắt lỗi tham số):**
![Minh chứng trace procinfo đoạn đầu](https://raw.githubusercontent.com/gowsec/XV6/feature/procinfo/report/procinfo_report/evidences/evidence_trace_procinfo_start.png)

- **Đoạn cuối (hoàn tất kiểm thử đồng thời/stress và hoàn thành bộ test):**
![Minh chứng trace procinfo đoạn cuối](https://raw.githubusercontent.com/gowsec/XV6/feature/procinfo/report/procinfo_report/evidences/evidence_trace_procinfo_end.png)

---

## 5. Hướng dẫn chạy kiểm thử

Trong môi trường máy ảo Ubuntu/QEMU:
```bash
make clean
make qemu
```

Trong shell của xv6:
```sh
$ procinfotest
```

Kiểm tra tương thích với `trace`:
```sh
$ trace 8388608 procinfotest
```
*(Số `8388608` tương ứng với bit `1 << SYS_procinfo = 1 << 23`).*
