# Project 1 — Xv6, Unix Utilities and System Call


## 1. Giới thiệu đồ án

Đồ án thuộc môn **Operating System (Hệ điều hành)**, được thực hiện trên **xv6**, một hệ điều hành giáo dục do MIT phát triển, nhằm tìm hiểu các nguyên lý cơ bản của hệ điều hành thông qua việc lập trình và mở rộng chức năng trên mã nguồn thực tế.

**Mục tiêu:** Làm quen với kiến trúc xv6, tìm hiểu cơ chế tương tác giữa chương trình người dùng (user space) và nhân hệ điều hành (kernel space), đồng thời thực hành xây dựng tiện ích Unix và bổ sung system call.

Đồ án gồm **4 bài tập**, chia thành hai nhóm:

- **User Programs:** `xargs`, `tree` — phát triển các chương trình tiện ích chạy trong user space.
- **System Calls:** `tracing`, `procinfo` — mở rộng chức năng của kernel thông qua cơ chế system call.

### 1.1. Nền tảng mã nguồn xv6

Dự án sử dụng mã nguồn **xv6-labs-2024** của MIT, cụ thể là branch `syscall`, làm nền tảng phát triển cho toàn bộ đồ án.

Repository kế thừa các thành phần cốt lõi của xv6, bao gồm kernel, chương trình người dùng, hệ thống file, cấu hình biên dịch và công cụ kiểm thử. Các thành phần này tạo thành một môi trường hệ điều hành hoàn chỉnh ở mức tối giản, cho phép nhóm triển khai và kiểm chứng các chức năng theo yêu cầu mà không cần xây dựng hệ điều hành từ đầu.

### 1.2. Kiến trúc và các thành phần chính

Mã nguồn xv6 được tổ chức thành các thành phần với vai trò riêng biệt:

- **`kernel/`**: Chứa mã nguồn nhân hệ điều hành, quản lý tiến trình, bộ nhớ, system call và các tài nguyên hệ thống.
- **`user/`**: Chứa các chương trình chạy trong user space và thư viện hỗ trợ.
- **`mkfs/`**: Cung cấp công cụ tạo filesystem image cho xv6.
- **`Makefile`**: Quy định quá trình biên dịch kernel, chương trình người dùng và khởi chạy hệ điều hành.
- **Các file cấu hình và công cụ kiểm thử**: Hỗ trợ quá trình phát triển, biên dịch và đánh giá kết quả thực hành.

### 1.3. Môi trường thực thi

Nhóm sử dụng **Ubuntu Linux** làm môi trường phát triển và **QEMU** để giả lập phần cứng, khởi chạy xv6.

xv6 hoạt động bên trong QEMU, không thay thế hệ điều hành Ubuntu của máy ảo.

Quy trình thực hành:

`Mã nguồn xv6 → Biên dịch bằng Makefile → Khởi chạy bằng QEMU → Phát triển và kiểm thử`



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

Repository được xây dựng trên mã nguồn **xv6-labs-2024 (branch `syscall`)**. Cấu trúc dưới đây trình bày các thành phần chính của xv6 và những file liên quan trực tiếp đến bốn bài tập.

> **Lưu ý:** Đây là cấu trúc rút gọn, không liệt kê toàn bộ mã nguồn xv6. Các file được đánh dấu `[Dự kiến]` là những file nhóm sẽ bổ sung trong quá trình triển khai, chưa tồn tại trong mã nguồn ban đầu.

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
│   ├── proc.c                # Quản lý tiến trình và fork()
│   ├── syscall.h             # Định nghĩa số hiệu system call
│   ├── syscall.c             # Điều phối system call
│   ├── sysproc.c             # Xử lý system call liên quan tiến trình
│   └── procinfo.h            # [Dự kiến] Khai báo struct procinfo
│
└── user/                     # Chương trình và thư viện user space
    ├── user.h                # Khai báo các hàm và system call
    ├── usys.pl               # Sinh mã gọi system call
    ├── ls.c                  # Chương trình ls, tham khảo cho tree
    ├── trace.c               # Chương trình trace có sẵn
    ├── xargs.c               # [Dự kiến] Bài 1: xargs
    ├── tree.c                # [Dự kiến] Bài 2: tree
    └── procinfotest.c        # [Dự kiến] Chương trình kiểm thử procinfo
```

### 3.1. Phạm vi phát triển

Các bài tập được triển khai trực tiếp trên mã nguồn xv6 hiện có:

| Bài tập | File phát triển chính | Hình thức |
|---------|-----------------------|-----------|
| xargs | `user/xargs.c`, `Makefile` | Tạo chương trình mới, cập nhật Makefile |
| tree | `user/tree.c`, `Makefile` | Tạo chương trình mới, cập nhật Makefile |
| tracing | `kernel/syscall.c`, `kernel/proc.c`, `kernel/proc.h` và các file liên quan | Mở rộng cơ chế system call |
| procinfo | `kernel/proc.c`, `kernel/proc.h`, `kernel/sysproc.c` và các file liên quan | Bổ sung system call lấy thông tin tiến trình |

Các thành viên chỉ chỉnh sửa những thành phần cần thiết cho bài tập được phân công. Những file nền tảng khác được giữ nguyên, trừ khi có yêu cầu kỹ thuật cụ thể.

> **Lưu ý tích hợp:** Hai bài `tracing` và `procinfo` có thể cùng chỉnh sửa một số file trong `kernel/` và `user/`. Các thay đổi cần được kiểm tra và phối hợp khi tạo Pull Request vào nhánh `dev` để hạn chế merge conflict.


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
