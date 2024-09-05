Linux<br />在 Unix 的世界里，有句很经典的话：一切对象皆是文件。这句话的意思是说，可以将 Unix 操作系统中所有的对象都当成文件，然后使用操作文件的接口来操作它们。Linux 作为一个类 Unix 操作系统，也努力实现这个目标。
<a name="pDqvO"></a>
## 虚拟文件系统简介
为了实现 一切对象皆是文件 这个目标，Linux 内核提供了一个中间层：虚拟文件系统（Virtual File System）。<br />如果大家使用过面向对象编程语言（如C++/Java等）的话，应该对 接口 这个概念并不陌生。而虚拟文件系统类似于面向对象中的接口，定义了一套标准的接口。开发者只需要实现这套接口，即可以使用操作文件的接口来操作对象。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1672490380153-04db171c-d943-4d43-95cb-67a389a6b0e2.jpeg#averageHue=%23f3d9ba&clientId=u6a559f9f-cc24-4&from=paste&id=u37b65dfa&originHeight=801&originWidth=681&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u1acd0602-b823-4a0d-806c-af5b413dd9f&title=)<br />上图中的蓝色部分就是虚拟文件系统所在位置。<br />从上图可以看出，虚拟文件系统为上层应用提供了统一的接口。如果某个文件系统实现了虚拟文件系统的接口，那么上层应用就能够使用诸如 `open()`、`read()` 和 `write()` 等函数来操作它们。<br />来了解虚拟文件系统的原理与实现。
<a name="BIl7p"></a>
## 虚拟文件系统原理
在阐述虚拟文件系统的原理前，先来介绍一个 Java 例子。通过这个 Java 例子，能够更容易理解虚拟文件系统的原理。
<a name="XNBXI"></a>
### 一个Java例子
如果大家使用过 Java 编写程序的话，那么就很容易理解虚拟文件系统了。使用 Java 的接口来模拟虚拟文件系统的定义：
```java
public interface VFSFile {
    int open(String file, int mode);
    int read(int fd, byte[] buffer, int size);
    int write(int fd, byte[] buffer, int size);
    ...
}
```
上面定义了一个名为 VFSFile 的接口，接口中定义了一些方法，如 `open()`、`read()` 和 `write()` 等。现在来定义一个名为 Ext3File 的对象来实现这个接口：
```java
public class Ext3File implements VFSFile {
    @Override
    public int open(String file, int mode) {
        ...
    }

    @Override
    public int read(int fd, byte[] buffer, int size) {
        ...
    }

    @Override
    public int write(int fd, byte[] buffer, int size) {
        ...
    }

    ...
}
```
现在就能使用 VFSFile 接口来操作 Ext3File 对象了，如下代码：
```java
public class Main() {
    public static void main(String[] args) {
        VFSFile file = new Ext3File();

        int fd = file.open("/tmp/file.txt", 0);
        ...
    }
}
```
从上面的例子可以看出，底层对象只需要实现 VFSFile 接口，就可以使用 VFSFile 接口相关的方法来操作对象，用户完全不需要了解底层对象的实现过程。
<a name="lVbTh"></a>
### 虚拟文件系统原理
上面的 Java 例子已经大概说明虚拟文件系统的原理，但由于 Linux 是使用 C 语言来编写的，而 C 语言并没有接口这个概念。所以，Linux 内核使用了一些技巧来模拟接口这个概念。<br />下面来介绍一下 Linux 内核是如何实现的。
<a name="dzuDA"></a>
#### 1、file结构
为了模拟接口，Linux 内核定义了一个名为 file 的结构体，其定义如下：
```c
struct file {
    ...
    const struct file_operations *f_op;
    ...
};
```
在 file 结构中，最为重要的一个字段就是 f_op，其类型为 file_operations 结构。而 file_operations 结构是由一组函数指针组成，其定义如下：
```c
struct file_operations {
    ...
    loff_t (*llseek) (struct file *, loff_t, int);
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
    ...
    int (*open) (struct inode *, struct file *);
    ...
};
```
从 file_operations 结构的定义可以隐约看到接口的影子，所以可以猜想出，如果实现了 file_operations 结构中的方法，应该就能接入到虚拟文件系统中。<br />在 Linux 内核中，file 结构代表着一个被打开的文件。所以，只需要将 file 结构的 f_op 字段设置成不同文件系统实现好的方法集，那么就能够使用不同文件系统的功能。<br />这个过程在 __dentry_open() 函数中实现，如下所示：
```c
static struct file *
__dentry_open(struct dentry *dentry, 
              struct vfsmount *mnt, 
              truct file *f, 
              int (*open)(struct inode *, struct file *), 
              const struct cred *cred)
{
    ...
    inode = dentry->d_inode;
    ...
    // 设置file结构的f_op字段为底层文件系统实现的方法集
    f->f_op = fops_get(inode->i_fop);
    ...
    return f;
}
```
设置好 file 结构的 f_op 字段后，虚拟文件系统就能够使用通用的接口来操作此文件了。调用过程如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1672490380165-af3b1162-db06-46ec-9561-7983c82c828a.png#averageHue=%23fcfcfc&clientId=u6a559f9f-cc24-4&from=paste&id=u64055219&originHeight=524&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ua293375c-5cfe-4180-aef5-9b5d90d6dac&title=)
<a name="aT6mC"></a>
#### 2、file_operations结构
底层文件系统需要实现虚拟文件系统的接口，才能被虚拟文件系统使用。也就是说，底层文件系统需要实现 file_operations 结构中的方法集。<br />一般底层文件系统会在其内部定义好 file_operations 结构，并且填充好其方法集中的函数指针。如 minix文件系统 就定义了一个名为 minix_file_operations 的 file_operations 结构。其定义如下：
```c
// 文件：fs/minix/file.c

const struct file_operations minix_file_operations = {
    .llseek         = generic_file_llseek,
    .read           = do_sync_read,
    .aio_read       = generic_file_aio_read,
    .write          = do_sync_write,
    .aio_write      = generic_file_aio_write,
    .mmap           = generic_file_mmap,
    .fsync          = generic_file_fsync,
    .splice_read    = generic_file_splice_read,
};
```
也就是说，如果当前使用的是 minix 文件系统，当使用 `read()` 函数读取其文件的内容时，那么最终将会调用 `do_sync_read()` 函数来读取文件的内容。
<a name="bzYAm"></a>
#### 3、dentry结构
到这里，虚拟文件系统的原理基本分析完毕，但还有两个非常重要的结构要介绍一下的：dentry 和 inode。<br />dentry 结构表示一个打开的目录项，当打开文件 /usr/local/lib/libc.so 文件时，内核会为文件路径中的每个目录创建一个 dentry 结构。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1672490381009-d2002e8f-a432-4ca2-8387-8f6a4cd96ad8.png#averageHue=%23faf9f8&clientId=u6a559f9f-cc24-4&from=paste&id=u35d6a826&originHeight=506&originWidth=798&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=uab8575af-5e02-4612-b119-4a4d5efd495&title=)<br />可以看到，file 结构有个指向 dentry 结构的指针，如下所示：
```c
struct file {
    ...
    struct path f_path;
    ...
    const struct file_operations *f_op;
    ...
};

struct path {
    ...
    struct dentry *dentry;
};
```
与文件类似，目录也有相关的操作接口，所以在 dentry 结构中也有操作方法集，如下所示：
```c
struct dentry {
    ...
    struct dentry *d_parent;              // 父目录指针
    struct qstr d_name;                   // 目录名字
    struct inode *d_inode;                // 指向inode结构
    ...
    const struct dentry_operations *d_op; // 操作方法集
    ...
};
```
其中的 d_op 字段就是目录的操作方法集。<br />内核在打开文件时，会为路径中的每个目录创建一个 dentry 结构，并且使用 d_parent 字段来指向其父目录项，这样就能通过 d_parent 字段来追索到根目录。
<a name="pF67F"></a>
#### 4、inode结构
在 Linux 内核中，inode 结构表示一个真实的文件。为什么有了 dentry 结构还需要 inode 结构呢？这是因为 Linux 存在硬链接的概念。<br />例如使用以下命令为 /usr/local/lib/libc.so 文件创建一个硬链接：
```bash
ln /usr/local/lib/libc.so /tmp/libc.so
```
现在 /usr/local/lib/libc.so 和 /tmp/libc.so 指向同一个文件，但它们的路径是不一样的。所以，就需要引入 inode 结构了。如下图所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1672490380201-4203120a-ec05-40d6-8878-adcde481c109.png#averageHue=%23fbfbfa&clientId=u6a559f9f-cc24-4&from=paste&id=ue4e7673c&originHeight=1130&originWidth=856&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u338440c9-40b7-4c3b-811f-af146987e1f&title=)<br />由于 /usr/local/lib/libc.so 和 /tmp/libc.so 指向同一个文件，所以它们都使用同一个 inode 对象。<br />inode 结构保存了文件的所有属性值，如文件的创建时间、文件所属用户和文件的大小等。其定义如下所示：
```c
struct inode {
    ...
    uid_t           i_uid;               // 文件所属用户
    gid_t           i_gid;               // 文件所属组
    ...
    struct timespec i_atime;             // 最后访问时间
    struct timespec i_mtime;             // 最后修改时间
    struct timespec i_ctime;             // 文件创建时间
    ...
    unsigned short  i_bytes;             // 文件大小
    ...
    const struct file_operations *i_fop; // 文件操作方法集（用于设置file结构）
    ...
};
```
注意到 inode 结构有个类型为 file_operations 结构的字段 i_fop，这个字段保存了文件的操作方法集。当用户调用 open() 系统调用打开文件时，内核将会使用 inode 结构的 i_fop 字段赋值给 file 结构的 f_op 字段。再来重温下赋值过程：
```c
static struct file *
__dentry_open(struct dentry *dentry, 
              struct vfsmount *mnt, 
              truct file *f, 
              int (*open)(struct inode *, struct file *), 
              const struct cred *cred)
{
    ...
    // 文件对应的inode对象
    inode = dentry->d_inode;
    ...
    // 使用inode结构的i_fop字段赋值给file结构的f_op字段
    f->f_op = fops_get(inode->i_fop);
    ...
    return f;
}
```
<a name="LE1lE"></a>
## 总结
主要介绍了 虚拟文件系统 的基本原理，从分析中可以发现，虚拟文件系统使用了类似于面向对象编程语言中的接口概念。正是有了 虚拟文件系统，Linux 才能支持各种各样的文件系统。
