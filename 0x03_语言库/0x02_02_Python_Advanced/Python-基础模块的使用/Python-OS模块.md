Python OS
<a name="aalRa"></a>
### 一、文件操作（os直属常用方法）
```python
# 关闭文件描述符 hw
os.close(hw)  

# 关闭所有文件描述符，从 hw1(包含) 到 hw2(不包含), 错误会忽略
os.closerange(hw1,hw2) 

# 通过文件描述符改变当前工作目录
os.fchdir(hw)      

# 改变一个文件的访问权限，该文件由参数hw指定，参数mode是Unix下的文件访问权限。
os.fchmod(hw, mode)    

# 修改一个文件的所有权，这个函数修改一个文件的用户ID和用户组ID，该文件由文件描述符hw指定。
os.fchown(hw, id1, id2) 

# 强制将文件写入磁盘，该文件由文件描述符hw指定，但是不强制更新文件的状态信息。
os.fdatasync(hw)      

#通过文件描述符hw 创建一个文件对象，并返回这个文件对象
os.fdopen(hw[, mode[, bufsize]])

#返回一个打开的文件的系统配置信息。name为检索的系统配置的值，
#它也许是一个定义系统值的字符串，这些名字在很多标准中指定（POSIX.1, Unix 95, Unix 98, 和其它）。
os.fpathconf(hw, name)         

#返回文件描述符hw的状态，像stat()。
os.fstat(hw)              

#返回包含文件描述符hw的文件的文件系统的信息，像 statvfs()
os.fstatvfs(hw)                

#强制将文件描述符为hw的文件写入硬盘。
os.fsync(hw)            

#裁剪文件描述符hw对应的文件, 所以它最大不能超过文件大小。
os.ftruncate(hw,length)    

#复制文件描述符hw
os.dup(hw)                     

# 将一个文件描述符hw复制到另一个hw1
os.dup2(hw,hw1)    

# 返回与终端hw（一个由os.open()返回的打开的文件描述符）关联的进程组
os.tcgetpgrp(hw)    

# 设置与终端hw（一个由os.open()返回的打开的文件描述符）关联的进程组为hw1。
os.tcsetpgrp(hw, hw1)  

# 返回唯一的路径名用于创建临时文件。
os.tempnam([dir[, prefix]])    

# 返回一个打开的模式为(w+b)的文件对象 .这文件对象没有文件夹入口，没有文件描述符，将会自动删除。
os.tmpfile()                   

# 为创建一个临时文件返回一个唯一的路径
os.tmpnam()                   

# 返回一个字符串，它表示与文件描述符hw 关联的终端设备。如果hw 没有与终端设备关联，则引发一个异常。
os.ttyname(hw)                

# 删除文件路径
os.unlink(path)     

# 返回指定的path文件的访问和修改的时间。
os.utime(path, times)         

# 输出在文件夹中的文件名通过在树中游走，向上或者向下。
os.walk(top[, topdown=True[, onerror=None[, followlinks=False]]])  

# 写入字符串到文件描述符hw中. 返回实际写入的字符串长度
os.write(hw, str)        

# 获取指定路径的文件系统统计信息
os.statvfs(path)              

# 返回当前工作目录
os.getcwd()        

# 返回一个当前工作目录的Unicode对象
os.getcwdu()                  

# 如果文件描述符hw是打开的，同时与tty(-like)设备相连，则返回true, 否则False。
os.isatty(hw)       

# 设置路径的标记为数字标记，类似 chflags()，但是没有软链接
os.lchflags(path, flags)   

# 修改连接文件权限
os.lchmod(path, mode)    

# 更改文件所有者，类似 chown，但是不追踪链接。
os.lchown(path, uid, gid)       

# 返回path指定的文件夹包含的文件或文件夹的名字的列表。
os.listdir(path)                

# 设置文件描述符 fd当前位置为pos, how方式修改: SEEK_SET 或者 0 
#设置从文件开始的计算的pos; SEEK_CUR或者 1 则从当前位置计算; 
#os.SEEK_END或者2则从文件尾部开始. 在unix，Windows中有效
os.lseek(fd, pos, how)          

# 打开一个文件，并且设置需要的打开选项，mode参数是可选的
os.open(file, flags[, mode])    

# 打开一个新的伪终端对。返回 pty 和 tty的文件描述符。
os.openpty()                    

# 返回相关文件的系统配置信息。
os.pathconf(path, name)        

# 用于分割文件路径的字符串
os.pathsep                      

# 获取当前目录的父目录字符串名：('..')
os.pardir                       

# 创建一个管道. 返回一对文件描述符(r, w) 分别为读和写
os.pipe()                  

# 从一个 command 打开一个管道
os.popen(command[, mode[, bufsize]]) 

# 字符串指示当前使用平台。win->'nt'; Linux->'posix'
os.name                        

# 从文件描述符hw中读取最多n个字节，返回包含读取字节的字符串，
#文件描述符hw对应文件已达到结尾, 返回一个空字符串。
os.read(hw, n)                 

#返回路径所在的根目录，主目录主文件
os.walk(path)

#返回路径下的目录和文件
os.listdir(path)

#切换当前程序操作的路径
os.chdir(path)     

#重命名文件
os.rename(oldfile,newfile)
```
<a name="QpBok"></a>
### 二、路径操作
```python
#将文件路径和文件名分割(会将最后一个目录作为文件名而分离)
os.path.split(filename)

#将文件路径和文件扩展名分割成一个元组
os.path.splitext(filename) 

#返回文件路径的目录部分
os.path.dirname(filename) 

#返回文件路径的文件名部分
os.path.basename(filename) 

#将文件路径和文件名凑成完整文件路径
os.path.join(dirname,basename) 

#获得绝对路径
os.path.abspath(name) 

#把路径分割为挂载点和文件名
os.path.splitunc(path) 

#规范path字符串形式
os.path.normpath(path) 

#判断文件或目录是否存在
os.path.exists() 

#如果path是绝对路径，返回True
os.path.isabs() 

#返回path的真实路径
os.path.realpath(path)

#从start开始计算相对路径
os.path.relpath(path[, start])  

#转换path的大小写和斜杠
os.path.normcase(path) 

#判断name是不是一个目录，name不是目录就返回false
os.path.isdir() 

#判断name是不是一个文件，不存在返回false
os.path.isfile() 

#判断文件是否连接文件,返回boolean
os.path.islink() 

#指定路径是否存在且为一个挂载点，返回boolean
os.path.ismount() 

#是否相同路径的文件，返回boolean
os.path.samefile() 

#返回最近访问时间 浮点型
os.path.getatime() 

#返回上一次修改时间 浮点型
os.path.getmtime() 

#返回文件创建时间 浮点型
os.path.getctime() 

#返回文件大小 字节单位
os.path.getsize() 

#返回list(多个路径)中，所有path共有的最长的路径
os.path.commonprefix(list) 

#路径存在则返回True,路径损坏也返回True
os.path.lexists 

#把path中包含的”~”和”~user”转换成用户目录
os.path.expanduser(path) 

#根据环境变量的值替换path中包含的”$name”和”${name}”
os.path.expandvars(path) 

#判断fp1和fp2是否指向同一文件
os.path.sameopenfile(fp1, fp2) 

#判断stat tuple stat1和stat2是否指向同一个文件
os.path.samestat(stat1, stat2)

#一般用在windows下，返回驱动器名和路径组成的元组
os.path.splitdrive(path) 

#遍历path，给每个path执行一个函数
os.path.walk(path, visit, arg) 

#设置是否支持unicode路径名
os.path.supports_unicode_filenames() 
```
<a name="89acf766"></a>
### 三、进程管理
```python
# 创建一个软链接
os.symlink(src, dst)     

# 创建硬链接，名为参数 dst，指向参数 src
os.link(src, dst)               

# 像stat(),但是没有软链接
os.lstat(path)                  


# 运行shell命令，直接显示
os.system("bash command")   

# 改变当前进程的根目录
os.chroot(path)    

#shell命令，不返回结果，但是可以进行读取
os.popen(command).read()

# 当前平台使用的行终止符，win下为"\t\n",Linux下为"\n"
os.linesep       

#路径连接符
os.sep

#从原始的设备号中提取设备major号码 (使用stat中的st_dev或者st_rdev field)。
os.major(device)        

# 以major和minor设备号组成一个原始设备号
os.makedev(major, minor)        

# 递归文件夹创建函数。像mkdir()，但创建的所有intermediate-level文件夹需要包含子文件夹。
os.makedirs(path[, mode])     

# 从原始的设备号中提取设备minor号码(使用stat中的st_dev或者st_rdev field )。
os.minor(device)

# 以数字mode的mode创建一个名为path的文件夹.默认的 mode 是 0777 (八进制)。
os.mkdir(path[, mode])    

# 创建命名管道，mode 为数字，默认为 0666 (八进制)
os.mkfifo(path[, mode])        

# 创建一个名为filename文件系统节点（文件，设备特别文件或者命名pipe）。
os.mknod(filename[, mode=0600, device])
```
<a name="9f4f0982"></a>
### 四、环境参数
```python
#检验权限模式 
os.access(path, mode) 

# 设置路径的标记为数字标记。
os.chflags(path, flags)  

# 更改权限
os.chmod(path, mode)     

# 更改文件所有者
os.chown(path, uid, gid)       

#获得当前系统登陆用户名
os.getlogin()       

#获得当前系统的CPU数量
os.cpu_count()    

# 获得n个字节长度的随机字符串，通常用于加解密运算
os.urandom(n)           

# 获取系统所有环境变量
os.environ                     

#获取某个特定的环境变量的值
os.getenv('CYGWIN')
os.environ.get('CYGWIN')

#设置环境变量
os.environ.setdefault(key,value)

#获取某个特定的环境变量的值的byte形式
os.getenvb('CYGWIN')
```
<a name="UhphD"></a>
### 五、stat子库
```python
#获取到的文件属性列表
fileStats = os.stat(path) 

#获取文件的模式
fileStats[stat.ST_MODE] 

#文件大小
fileStats[stat.ST_SIZE] 

#文件最后修改时间
fileStats[stat.ST_MTIME] 

#文件最后访问时间
fileStats[stat.ST_ATIME] 

#文件创建时间
fileStats[stat.ST_CTIME] 

#是否目录
stat.S_ISDIR(fileStats[stat.ST_MODE]) 

#是否一般文件
stat.S_ISREG(fileStats[stat.ST_MODE]) 

#是否连接文件
stat.S_ISLNK(fileStats[stat.ST_MODE]) 

#是否COCK文件
stat.S_ISSOCK(fileStats[stat.ST_MODE]) 

#是否命名管道
stat.S_ISFIFO(fileStats[stat.ST_MODE]) 

#是否块设备
stat.S_ISBLK(fileStats[stat.ST_MODE])

#是否字符设置
stat.S_ISCHR(fileStats[stat.ST_MODE])
```
<a name="OgfqI"></a>
### 六、一键死机
os 能很轻松打开一个程序，只要知道这个源程序根目录的位置。所以一般都会：
```python
os.system('notepad')  #打开记事本
os.system('start notepad')#打开记事本
```
这样可以很轻松打开一个记事本，那么要想让它无限打开。给它整个死循环，给它个无限递归不就完了。<br />还有一种比较优雅的打开程序的方法，那就是popen方法：
```python
pp=os.popen('notebook')
```
不仅如此，还能看到它执行结果后的返回值。
```python
print(pp.read())
```
