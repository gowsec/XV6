# Project 1 — Xv6, Unix Utilities and System Call

## 1. Giới thiệu đồ án

Đồ án thuộc môn **Operating System (Hệ điều hành)**, thực hiện trên hệ điều hành giáo dục **xv6** trong môi trường Linux.

**Mục đích:** Làm quen với xv6, tìm hiểu cách chương trình người dùng sử dụng system call và cách bổ sung system call vào kernel. Đồ án gồm **4 bài tập**, chia thành hai nhóm:

- **User Program:** `xargs`, `tree` — xây dựng tiện ích chạy trong user space.
- **System Call:** `tracing`, `procinfo` — mở rộng chức năng của kernel xv6.

## 2. Cấu hình môi trường (Local VM)


| Thành phần | Thông tin |
|---|---|
| Máy chủ (Host OS) | _Chưa cập nhật_ |
| Phần mềm ảo hóa | _Chưa cập nhật_ |
| Hệ điều hành máy ảo (Guest OS) | _Chưa cập nhật_ |
| Kiến trúc CPU máy ảo | _Chưa cập nhật_ |
| Số vCPU | _Chưa cập nhật_ |
| RAM | _Chưa cập nhật_ |
| Dung lượng ổ đĩa | _Chưa cập nhật_ |
| Phiên bản QEMU | _Chưa cập nhật_ |
| RISC-V toolchain | _Chưa cập nhật_ |
| Repository / branch / commit xv6 gốc | _Chưa cập nhật_ |

## 3. Cấu trúc thư mục đồ án

> **Cấu trúc minh họa:** Chỉ liệt kê các thư mục và file liên quan trực tiếp đến bốn bài. Cấu trúc chính xác sẽ được cập nhật theo repository xv6 mà nhóm sử dụng. Những file ghi `(mới)` là file dự kiến bổ sung, không phải tất cả đều được đề bài quy định tên.

```text
XV6/
├── Makefile                 # Đăng ký chương trình người dùng trong UPROGS
├── kernel/
│   ├── param.h              # Hằng số hệ thống (ví dụ MAXARG)
│   ├── proc.h               # Cấu trúc và thông tin tiến trình
│   ├── proc.c               # Quản lý tiến trình, fork()
│   ├── syscall.h            # Mã số system call
│   ├── syscall.c            # Điều phối system call
│   ├── sysproc.c            # Các hàm xử lý system call liên quan tiến trình
│   └── procinfo.h           # (mới, đề xuất) Khai báo struct procinfo
└── user/
    ├── user.h               # Khai báo các hàm dùng ở user space
    ├── usys.pl              # Sinh system call stubs
    ├── ls.c                 # Mã tham khảo cho bài tree
    ├── trace.c              # Chương trình trace được đề cung cấp
    ├── xargs.c              # (mới) Bài 1: xargs
    ├── tree.c               # (mới) Bài 2: tree
    └── procinfotest.c       # (mới, đề xuất) Kiểm thử procinfo
```

## 4. Nội dung các bài tập

### 4.1. xargs — User Program

Viết chương trình `user/xargs.c` đọc từng dòng từ **standard input (stdin)** và chạy lệnh được chỉ định với nội dung dòng đó được bổ sung vào danh sách tham số. Bài tập sử dụng các system call như `fork()`, `exec()` và `wait()`.

### 4.2. tree — User Program

Viết chương trình `tree` hiển thị cấu trúc file và thư mục dưới dạng cây. Chương trình duyệt đệ quy các thư mục con, thụt lề theo cấp, thêm dấu `/` sau tên thư mục, bỏ qua `.` và `..`, đồng thời xử lý lỗi truy cập file hoặc thư mục.

### 4.3. tracing — System Call

Bổ sung system call `trace(mask)` để theo dõi các system call được chọn theo bitmask. Khi một system call được theo dõi trả về, kernel in **PID, tên system call và giá trị trả về**. Cấu hình trace áp dụng cho tiến trình gọi và các tiến trình con được tạo sau đó.

### 4.4. procinfo — System Call

Bổ sung system call `procinfo(int pid, struct procinfo *info)` để lấy thông tin của tiến trình theo PID, gồm **PID, PPID, trạng thái tiến trình, kích thước bộ nhớ và tên tiến trình**, sau đó trả dữ liệu về chương trình người dùng.

---

## Hướng dẫn cài đặt môi trường

> TODO: Cập nhật sau khi hoàn tất cấu hình Local VM và cài đặt các công cụ cần thiết.

## Hướng dẫn Build và chạy xv6

> TODO: Bổ sung các lệnh biên dịch và khởi chạy xv6 sau khi thiết lập môi trường thành công.

## Hướng dẫn kiểm thử

> TODO: Bổ sung lệnh kiểm thử, kết quả mong đợi và kết quả thực tế sau khi hoàn thành từng bài.

### Bài 1 — xargs

> TODO: Cập nhật sau khi hoàn thành.

### Bài 2 — tree

> TODO: Cập nhật sau khi hoàn thành.

### Bài 3 — tracing

> TODO: Cập nhật sau khi hoàn thành.

### Bài 4 — procinfo

> TODO: Cập nhật sau khi hoàn thành.


## Git Workflow

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

**Lưu ý:**
- Không trực tiếp push code lên `main`.
- Các thay đổi liên quan đến System Call cần được thống nhất giữa thành viên phụ trách `tracing` và `procinfo`.
- Tất cả thành viên sử dụng cùng một phiên bản xv6 và cùng commit gốc.

---

## Phân công thành viên

| Thành viên | Bài phụ trách | Công việc bổ sung |
|---|---|---|
| Thành viên 1  | xargs | Quản lý GitHub, review PR, tích hợp và đóng gói sản phẩm |
| Thành viên 2 | tree | Tổng hợp báo cáo, chuẩn hóa tài liệu README |
| Thành viên 3 | procinfo | Xây dựng test case, phối hợp kiểm thử |
| Thành viên 4 | tracing | Hỗ trợ tích hợp và kiểm thử System Call |

### Trách nhiệm chung

- Mỗi thành viên chịu trách nhiệm triển khai và kiểm thử bài được phân công.
- Mỗi thành viên tự viết nội dung báo cáo cho phần mình phụ trách.
- Các thành viên phối hợp review code và xử lý lỗi tích hợp.
- Nhóm trưởng chịu trách nhiệm kiểm tra phiên bản cuối trước khi nộp.
