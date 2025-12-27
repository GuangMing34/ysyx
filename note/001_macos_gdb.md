# issue1: gdb attach fail without symbol

macos(M3) @20251227 can't use gdb to attach to related process, it will output below error

```text
Reading symbols from ./ex31...
Reading symbols from /Users/bytedance/code/ysyx/learn_c_hard_way/ex31/ex31.dSYM/Contents/Resources/DWARF/ex31...
DW_FORM_GNU_str_index or DW_FORM_strx used without .debug_str section in CU at offset 0x0 [in module /Users/bytedance/code/ysyx/learn_c_hard_way/ex31/ex31.dSYM/Contents/Resources/DWARF/ex31]
Don't know how to attach.  Try "help target".
/Users/bytedance/code/ysyx/learn_c_hard_way/ex31/44804: No such file or directory.
(gdb) help target
Connect to a target machine or process.
The first argument is the type or protocol of the target machine.
Remaining arguments are interpreted by the target protocol.  For more
information on the arguments for a particular protocol, type
`help target ' followed by the protocol name.
```

## possible reason

```bash
➜  ~/code/ysyx/learn_c_hard_way/ex31 git:(main) ✗ gcc --version
Apple clang version 17.0.0 (clang-1700.6.3.2)
Target: arm64-apple-darwin25.2.0
Thread model: posix
InstalledDir: /Library/Developer/CommandLineTools/usr/bin
```

gcc on mac os is not gnu gcc, it is llvm gcc. symbol maybe not match with gnu gdb

but if we install gcc via brew

```
brew install gcc
```

the new gcc name maybe **/opt/homebrew/bin/gcc-13**, not named **gcc**

## solution1: use lldb

use macos **lldb** tool to replace gdb to match **apple clang gcc** target.

## solution2: change CC as gnu gcc: gcc-xx

use gnu gcc, which with a version postfix, named gcc-xx to build.
in Makefile, we need specify the CC variable, since it is the default conmpiler to compile c program

```makefile
# add CC setting
CC=gcc-15
```

then use gdb to debug will be ok.

# issue 2: gdb attch fail to process

```bash
➜  ~/code/ysyx/learn_c_hard_way/ex31 git:(main) ✗ gdb ./ex31 16218 
GNU gdb (GDB) 16.3
Copyright (C) 2024 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "--host=aarch64-apple-darwin25.0.0 --target=x86_64-apple-darwin20".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./ex31...
Reading symbols from /Users/bytedance/code/ysyx/learn_c_hard_way/ex31/ex31.dSYM/Contents/Resources/DWARF/ex31...
Don't know how to attach.  Try "help target".
/Users/bytedance/code/ysyx/learn_c_hard_way/ex31/16218: No such file or directory.
(gdb) 
(gdb) 
```

here we can see the error can't find file

```text
Reading symbols from ./ex31...
Reading symbols from /Users/bytedance/code/ysyx/learn_c_hard_way/ex31/ex31.dSYM/Contents/Resources/DWARF/ex31...
Don't know how to attach.  Try "help target".
/Users/bytedance/code/ysyx/learn_c_hard_way/ex31/16218: No such file or directory.
(gdb) 
```

seems cmd error.

but in the lecture doc, it really use this cmd `<gdb exe_path pid>`

## possible reason

on linux os, it maybe ok. but on macos, at least new version gdb didn't support this type cmd `gdb <program> <pid>`

## solution: try new version cmd

### 方法 1：用 `gdb -p <pid>`

```bash
gdb -p 16218
```

这种方式在大多数现代 GDB 版本中都能跨平台工作。

### 方法 2：进入 GDB 后 attach

```bash
gdb
(gdb) attach 16218
```

### 方法 3：调试前加载符号表

```bash
gdb ./ex31
(gdb) attach 16218
```

这样 GDB 会先加载 `./ex31` 的符号，再 attach 到进程

## symbol load logic

### 1. gdb -p `<pid>` 的工作原理

当你执行：

```bash
gdb -p 16218
```

GDB 会：

1. 通过系统调用（如`ptrace`）附加到正在运行的进程`16218`。
2. 暂停该进程的所有线程。
3. 尝试从进程的内存和文件系统中查找可执行文件路径。

**关键点** ：

* GDB 会读取`/proc/<pid>/exe`（Linux）或`vmmap`（macOS）等信息，找到程序的可执行文件路径。
* 如果它能访问到该可执行文件，并且文件中包含调试符号（`.debug_*` 段），那么符号会自动加载。
* 如果可执行文件被删除、路径改变，或者没有调试符号，GDB 就无法加载符号，只能看到内存地址。

### 2. **macOS 上的符号加载限制**

在 macOS 上：

* 调试信息通常被拆分成`.dSYM` 文件夹，而不是嵌入到可执行文件中。
* GDB 对`.dSYM` 的支持有限，即使找到了可执行文件，也可能无法自动解析`.dSYM` 中的符号。
* 如果可执行文件是通过`clang` 或`gcc` 默认编译的，没有加`-fno-split-dwarf`，那么调试信息不在可执行文件里，GDB 就无法加载。

---

### 3. **如何手动加载符号**

如果你发现 `gdb -p <pid>` 没有加载符号，可以手动指定可执行文件：

```bash
gdb -p 16218
(gdb) file ./exepath
```

这样 GDB 会重新读取 `./exepath` 的符号表。

或者，你可以在启动 GDB 时就指定可执行文件：

```bash
gdb ./exepath
(gdb) attach 16218
```

这种方式更可靠，因为 GDB 会先加载符号，再附加到进程。

# issue3: macos will generate a new symbol dir

#### for clang

`-fno-split-dwarf` in Cflags will remove split symbol dir

#### for gnu gcc

在 macOS 上，调试信息的处理由 **`dsymutil`** 控制，它会在链接阶段或安装阶段被调用，把调试信息从可执行文件里抽出来，放到一个 `.dSYM` 文件夹中。

这是因为：

* **Apple 的调试器（LLDB）** 习惯使用`.dSYM` 文件夹来存放调试信息。
* **Xcode** 的构建系统会自动调用`dsymutil`，即使你用的是 GNU GCC。
* 如果你是在**Homebrew** 或其他构建环境中编译，它可能会在安装过程中运行`dsymutil`。

**总得来说，gnu gcc应该不太好去掉，-g也带了symbol了，可以不管它**


# issue4: DWARF error

```
(gdb) b 8
DWARF Error: DW_FORM_line_strp used without required section
```


## possible reason

`DW_FORM_line_strp` 是 **DWARF 5** 格式的一种新特性，用于存储行号表的字符串偏移。

在 macOS 上，GCC（甚至 GNU GCC）默认生成的调试信息是 **DWARF 5** ，而：

* GNU GDB 在 ARM64 macOS 上对 DWARF 5 的支持不完善。
* 缺少`.debug_line_str` 段（这是 DWARF 5 引入的），GDB 无法解析行号信息，于是报`DW_FORM_line_strp used without required section`。

## solution: use gcc+lldb

在 macOS 上，Apple 官方调试器 **LLDB** 对 DWARF 5 和 `.dSYM` 都有原生支持：

```bash
lldb -p <pid>
(lldb) file exepath
(lldb) breakpoint set --line 8
```

这样不会出现 `DW_FORM_line_strp` 错误。
