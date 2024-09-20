# gcc

## 编译的过程
写个 hello.c 代码。
```c
#include <stdio.h>
#define STR "hello world\n"

void main() {
	printf(STR);
}
```

### 第一步：预处理（preprocess）
其实就是以下三个动作：

1. 头文件展开；
2. 宏定义直接替换；
3. 条件编译不成立就去掉。
```bash
gcc -E hello.c -o hello.i
```

### 第二步：编译（compile）
转换成汇编语言。
```bash
gcc -S hello.i -o hello.s
```


### 第三步：汇编（assemble）

转换成二进制（ELF relocatable）。
```bash
gcc -c hello.s -o hello.o
```

### 第四步：链接（link）
具体分为动态链接和静态链接。
```bash
# 动态链接
gcc hello.o -o hello
# 静态链接
gcc hello.o -o hello -static
```

## 静态库制作
写一个加法函数。
```c
int add(int a, int b) {
    return a+b;
}
```
编译成 .o。
```bash
gcc -c add.c -o add.o
```
制作成静态库。
```bash
ar rcs libadd.a add.o
```
编写测试代码。
```c
#include <stdio.h>
void main(){
    printf("%d", add(1,2));
}
```
编译成可执行文件。
```bash
#写法一
gcc test.c -o test libadd.a
#写法二
gcc test.c -o test -static -ladd -L ./
```
执行。
```bash
./test
3
```

## 动态库制作
写一个加法函数。
```c
int add(int a, int b) {
    return a+b;
}
```
编译成 .o
```bash
gcc -c add.c -o add.o -fPIC
```
制作成动态库
```bash
gcc -shared -o libadd.so add.o
```
上面两步也可以直接从源文件一步到位。
```bash
gcc -fPIC -shared -o libadd.so add.c
```
编写测试代码。
```c
#include <stdio.h>
void main(){
    printf("%d", add(1,2));
}
```
编译成可执行文件。
```bash
gcc test.c -o test -ladd -L ./
```
执行发现报错。
```bash
./test
error while loading shared libraries: libadd.so: 
cannot open shared object file: No such file or directory
```
因为执行的时候找不到指定的动态库。

那把 **libadd.so** 放在执行时的动态库默认搜索路径下，比如 **/lib64**。

```bash
cp libadd.so /lib64
```
再次执行就成功了。
```bash
./test
3
```
查看二进制文件的链接信息，也可以发现 libadd.so 生效了。
```bash
ldd test
   linux-vdso.so.1 =>  (0x00007ffe0f597000)
   libadd.so => /lib64/libadd.so (0x00007fa5ab29f000)
   libc.so.6 => /lib64/libc.so.6 (0x00007fa5aaed1000)
   /lib64/ld-linux-x86-64.so.2 (0x00007fa5ab4a1000)
```
