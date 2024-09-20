| 方法 | 描述 |
| --- | --- |
| [os.access(path, mode)](https://www.runoob.com/python/os-access.html) | 检验权限模式 |
| [os.chdir(path)](https://www.runoob.com/python/os-chdir.html) | 改变当前工作目录 |
| [os.chflags(path, flags)](https://www.runoob.com/python/os-chflags.html) | 设置路径的标记为数字标记。 |
| [os.chmod(path, mode)](https://www.runoob.com/python/os-chmod.html) | 更改权限 |
| [os.chown(path, uid, gid)](https://www.runoob.com/python/os-chown.html) | 更改文件所有者 |
| [os.chroot(path)](https://www.runoob.com/python/os-chroot.html) | 改变当前进程的根目录 |
| [os.close(fd)](https://www.runoob.com/python/os-close.html) | 关闭文件描述符 fd |
| [os.closerange(fd_low, fd_high)](https://www.runoob.com/python/os-closerange.html) | 关闭所有文件描述符，从 fd_low (包含) 到 fd_high (不包含), 错误会忽略 |
| [os.dup(fd)](https://www.runoob.com/python/os-dup.html) | 复制文件描述符 fd |
| [os.dup2(fd, fd2)](https://www.runoob.com/python/os-dup2.html) | 将一个文件描述符 fd 复制到另一个 fd2 |
| [os.fchdir(fd)](https://www.runoob.com/python/os-fchdir.html) | 通过文件描述符改变当前工作目录 |
| [os.fchmod(fd, mode)](https://www.runoob.com/python/os-fchmod.html) | 改变一个文件的访问权限，该文件由参数fd指定，参数mode是Unix下的文件访问权限。 |
| [os.fchown(fd, uid, gid)](https://www.runoob.com/python/os-fchown.html) | 修改一个文件的所有权，这个函数修改一个文件的用户ID和用户组ID，该文件由文件描述符fd指定。 |
| [os.fdatasync(fd)](https://www.runoob.com/python/os-fdatasync.html) | 强制将文件写入磁盘，该文件由文件描述符fd指定，但是不强制更新文件的状态信息。 |
| [os.fdopen(fd[, mode[, bufsize]])](https://www.runoob.com/python/os-fdopen.html) | 通过文件描述符 fd 创建一个文件对象，并返回这个文件对象 |
| [os.fpathconf(fd, name)](https://www.runoob.com/python/os-fpathconf.html) | 返回一个打开的文件的系统配置信息。name为检索的系统配置的值，它也许是一个定义系统值的字符串，这些名字在很多标准中指定（POSIX.1, Unix 95, Unix 98, 和其它）。 |
| [os.fstat(fd)](https://www.runoob.com/python/os-fstat.html) | 返回文件描述符fd的状态，像stat()。 |
| [os.fstatvfs(fd)](https://www.runoob.com/python/os-fstatvfs.html) | 返回包含文件描述符fd的文件的文件系统的信息，像 statvfs() |
| [os.fsync(fd)](https://www.runoob.com/python/os-fsync.html) | 强制将文件描述符为fd的文件写入硬盘。 |
| [os.ftruncate(fd, length)](https://www.runoob.com/python/os-ftruncate.html) | 裁剪文件描述符fd对应的文件, 所以它最大不能超过文件大小。 |
| [os.getcwd()](https://www.runoob.com/python/os-getcwd.html) | 返回当前工作目录 |
| [os.getcwdu()](https://www.runoob.com/python/os-getcwdu.html) | 返回一个当前工作目录的Unicode对象 |
| [os.isatty(fd)](https://www.runoob.com/python/os-isatty.html) | 如果文件描述符fd是打开的，同时与tty(-like)设备相连，则返回true, 否则False。 |
| [os.lchflags(path, flags)](https://www.runoob.com/python/os-lchflags.html) | 设置路径的标记为数字标记，类似 chflags()，但是没有软链接 |
| [os.lchmod(path, mode)](https://www.runoob.com/python/os-lchmod.html) | 修改连接文件权限 |
| [os.lchown(path, uid, gid)](https://www.runoob.com/python/os-lchown.html) | 更改文件所有者，类似 chown，但是不追踪链接。 |
| [os.link(src, dst)](https://www.runoob.com/python/os-link.html) | 创建硬链接，名为参数 dst，指向参数 src |
| [os.listdir(path)](https://www.runoob.com/python/os-listdir.html) | 返回path指定的文件夹包含的文件或文件夹的名字的列表。 |
| [os.lseek(fd, pos, how)](https://www.runoob.com/python/os-lseek.html) | 设置文件描述符 fd当前位置为pos, how方式修改: SEEK_SET 或者 0 设置从文件开始的计算的pos; SEEK_CUR或者 1 则从当前位置计算; os.SEEK_END或者2则从文件尾部开始. 在unix，Windows中有效 |
| [os.lstat(path)](https://www.runoob.com/python/os-lstat.html) | 像stat(),但是没有软链接 |
| [os.major(device)](https://www.runoob.com/python/os-major.html) | 从原始的设备号中提取设备major号码 (使用stat中的st_dev或者st_rdev field)。 |
| [os.makedev(major, minor)](https://www.runoob.com/python/os-makedev.html) | 以major和minor设备号组成一个原始设备号 |
| [os.makedirs(path[, mode])](https://www.runoob.com/python/os-makedirs.html) | 递归文件夹创建函数。像mkdir(), 但创建的所有intermediate-level文件夹需要包含子文件夹。 |
| [os.minor(device)](https://www.runoob.com/python/os-minor.html) | 从原始的设备号中提取设备minor号码 (使用stat中的st_dev或者st_rdev field )。 |
| [os.mkdir(path[, mode])](https://www.runoob.com/python/os-mkdir.html) | 以数字mode的mode创建一个名为path的文件夹.默认的 mode 是 0777 (八进制)。 |
| [os.mkfifo(path[, mode])](https://www.runoob.com/python/os-mkfifo.html) | 创建命名管道，mode 为数字，默认为 0666 (八进制) |
| [os.mknod(filename[, mode=0600, device])](https://www.runoob.com/python/os-mknod.html) | 创建一个名为filename文件系统节点（文件，设备特别文件或者命名pipe）。 |
| [os.open(file, flags[, mode])](https://www.runoob.com/python/os-open.html) | 打开一个文件，并且设置需要的打开选项，mode参数是可选的 |
| [os.openpty()](https://www.runoob.com/python/os-openpty.html) | 打开一个新的伪终端对。返回 pty 和 tty的文件描述符。 |
| [os.pathconf(path, name)](https://www.runoob.com/python/os-pathconf.html) | 返回相关文件的系统配置信息。 |
| [os.pipe()](https://www.runoob.com/python/os-pipe.html) | 创建一个管道. 返回一对文件描述符(r, w) 分别为读和写 |
| [os.popen(command[, mode[, bufsize]])](https://www.runoob.com/python/os-popen.html) | 从一个 command 打开一个管道 |
| [os.read(fd, n)](https://www.runoob.com/python/os-read.html) | 从文件描述符 fd 中读取最多 n 个字节，返回包含读取字节的字符串，文件描述符 fd对应文件已达到结尾, 返回一个空字符串。 |
| [os.readlink(path)](https://www.runoob.com/python/os-readlink.html) | 返回软链接所指向的文件 |
| [os.remove(path)](https://www.runoob.com/python/os-remove.html) | 删除路径为path的文件。如果path 是一个文件夹，将抛出OSError; 查看下面的rmdir()删除一个 directory。 |
| [os.removedirs(path)](https://www.runoob.com/python/os-removedirs.html) | 递归删除目录。 |
| [os.rename(src, dst)](https://www.runoob.com/python/os-rename.html) | 重命名文件或目录，从 src 到 dst |
| [os.renames(old, new)](https://www.runoob.com/python/os-renames.html) | 递归地对目录进行更名，也可以对文件进行更名。 |
| [os.rmdir(path)](https://www.runoob.com/python/os-rmdir.html) | 删除path指定的空目录，如果目录非空，则抛出一个OSError异常。 |
| [os.stat(path)](https://www.runoob.com/python/os-stat.html) | 获取path指定的路径的信息，功能等同于C API中的stat()系统调用。 |
| [os.stat_float_times([newvalue])](https://www.runoob.com/python/os-stat_float_times.html) | 决定stat_result是否以float对象显示时间戳 |
| [os.statvfs(path)](https://www.runoob.com/python/os-statvfs.html) | 获取指定路径的文件系统统计信息 |
| [os.symlink(src, dst)](https://www.runoob.com/python/os-symlink.html) | 创建一个软链接 |
| [os.tcgetpgrp(fd)](https://www.runoob.com/python/os-tcgetpgrp.html) | 返回与终端fd（一个由os.open()返回的打开的文件描述符）关联的进程组 |
| [os.tcsetpgrp(fd, pg)](https://www.runoob.com/python/os-tcsetpgrp.html) | 设置与终端fd（一个由os.open()返回的打开的文件描述符）关联的进程组为pg。 |
| [os.tempnam([dir[, prefix]])](https://www.runoob.com/python/os-tempnam.html) | 返回唯一的路径名用于创建临时文件。 |
| [os.tmpfile()](https://www.runoob.com/python/os-tmpfile.html) | 返回一个打开的模式为(w+b)的文件对象 .这文件对象没有文件夹入口，没有文件描述符，将会自动删除。 |
| [os.tmpnam()](https://www.runoob.com/python/os-tmpnam.html) | 为创建一个临时文件返回一个唯一的路径 |
| [os.ttyname(fd)](https://www.runoob.com/python/os-ttyname.html) | 返回一个字符串，它表示与文件描述符fd 关联的终端设备。如果fd 没有与终端设备关联，则引发一个异常。 |
| [os.unlink(path)](https://www.runoob.com/python/os-unlink.html) | 删除文件 |
| [os.utime(path, times)](https://www.runoob.com/python/os-utime.html) | 返回指定的path文件的访问和修改的时间。 |
| [os.walk(top[, topdown=True[, onerror=None[, followlinks=False]]])](https://www.runoob.com/python/os-walk.html) | 输出在文件夹中的文件名通过在树中游走，向上或者向下。 |
| [os.write(fd, str)](https://www.runoob.com/python/os-write.html) | 写入字符串到文件描述符 fd中. 返回实际写入的字符串长度 |
| [os.path 模块](https://www.runoob.com/python/python-os-path.html) | 获取文件的属性信息。 |

