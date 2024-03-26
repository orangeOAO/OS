## 1-15
- (a)
    1.利用I/O來傳輸資料
    2.CPU傳遞資料到device的local buffer來讓設備動作
    3.監控特定記憶體的變化來採取動作
    4.利用polling與vectored interrupt來提高整體效率
- (b)
    裝置會發送Interrupt來通知cpu已經完成

- (c)
    會干擾
    - 情況一
        同時訪問相同的內存
    - 情況二
        同時訪問造成帶寬的競爭
    - 情況三
        因為DMA占用大量的帶寬而產生的延遲
## 2-15
1. Share Memory
    - strength
        1. 共享內存速度比較快
        2. 可以達到緊密同步
    - weakness
        1. 同步會比較複雜
        2. 難擴展
2. Message Passing
    - strength
        1. 比較簡單
        2. 沒有同步問題
        3. 可擴展
    - weakness
        1. 相比於共享記憶體需要花費比較多的資源
## 2-19
1. 可以更加容易擴充、更容易維護
2. 用message passing來進行互動
3. 比單一kernel的性能開銷高     

## 3-12
些換前保存目前的state，然後讀取之前別的process的state來進行動作

## 3-18
- ordinary pipes are more suitable than named pipes

在任務相近的情況，如讀寫文件的時候

- named pipes are more suitable than ordinary pipes

在任務不相近的時候，如讀寫文件跟上網同時進行(鬆耦合)

## 2-24
```cmd=
➜  OS strace ./Q2_24
execve("./Q2_24", ["./Q2_24"], 0x7ffd5cfc3690 /* 27 vars */) = 0      
brk(NULL)                               = 0x55d93a248000
arch_prctl(0x3001 /* ARCH_??? */, 0x7ffe48dc91e0) = -1 EINVAL (Invalid argument)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fc2d040a000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=62143, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 62143, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7fc2d03fa000      
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0P\237\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0 \0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0"..., 48, 848) = 48
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\302\211\332Pq\2439\235\350\223\322\257\201\326\243\f"..., 68, 896) = 68
newfstatat(3, "", {st_mode=S_IFREG|0755, st_size=2220400, ...}, AT_EMPTY_PATH) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2264656, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fc2d01d1000
mprotect(0x7fc2d01f9000, 2023424, PROT_NONE) = 0
mmap(0x7fc2d01f9000, 1658880, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7fc2d01f9000
mmap(0x7fc2d038e000, 360448, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1bd000) = 0x7fc2d038e000
mmap(0x7fc2d03e7000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x215000) = 0x7fc2d03e7000
mmap(0x7fc2d03ed000, 52816, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fc2d03ed000
close(3)                                = 0
mmap(NULL, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fc2d01ce000
arch_prctl(ARCH_SET_FS, 0x7fc2d01ce740) = 0
set_tid_address(0x7fc2d01cea10)         = 1068
set_robust_list(0x7fc2d01cea20, 24)     = 0
rseq(0x7fc2d01cf0e0, 0x20, 0, 0x53053053) = 0
mprotect(0x7fc2d03e7000, 16384, PROT_READ) = 0
mprotect(0x55d93828f000, 4096, PROT_READ) = 0
mprotect(0x7fc2d0444000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
munmap(0x7fc2d03fa000, 62143)           = 0
newfstatat(1, "", {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0), ...}, AT_EMPTY_PATH) = 0
getrandom("\x56\xc4\x98\xdf\x72\x46\x5d\xc2", 8, GRND_NONBLOCK) = 8   
brk(NULL)                               = 0x55d93a248000
brk(0x55d93a269000)                     = 0x55d93a269000
newfstatat(0, "", {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0), ...}, AT_EMPTY_PATH) = 0
write(1, "Enter source file name: ", 24Enter source file name: ) = 24 
read(0, 0x55d93a2486b0, 1024)           = ? ERESTARTSYS (To be restarted if SA_RESTART is set)
--- SIGWINCH {si_signo=SIGWINCH, si_code=SI_KERNEL} ---
read(0,"\n", 1024)                     = 1
```

## 3-19
```cmd=
➜  OS gcc -o time_shm time_shm.c -lrt

➜  OS ./time_shm ls

'Chap01 (1).pptx'   Chap02.pptx   Chap03.pptx   Chap04.pptx   Chap05.pptx   HW1.md   HW1.ppt   Q2_24   Q2_24.c   time_pipe.c   time_shm   time_shm.c  '~$Chap02.pptx'
Elapsed Time: 0.002102 seconds
➜  OS gcc -o time_pipe time_pipe.c

➜  OS ./time_pipe ls

'Chap01 (1).pptx'   Chap03.pptx   Chap05.pptx   HW1.ppt   Q2_24.c     time_pipe.c   time_shm.c
 Chap02.pptx        Chap04.pptx   HW1.md        Q2_24     time_pipe   time_shm     '~$Chap02.pptx'
Elapsed Time: 0.002138 seconds
```
    

