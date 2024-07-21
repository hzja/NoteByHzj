PythonFTP
<a name="sI82Z"></a>
## 1、包引用
使用python自带的包ftplib。
```python
from ftplib import FTP
```
添加对中文路径的支持：
```python
"""
代码里头设置下 ftp.encoding = 'utf-8' 
"""
```
否则，遇到中文路径会报错：
```python
UnicodeEncodeError: 'latin-1' codec can't encode characters in position 24-25: ordinal not in range(256)
```
<a name="ZvSXZ"></a>
## 2、函数说明
<a name="AE8IS"></a>
### 2.1、连接登录
**登录需要配置参数：ftp_ip、ftp_port、ftp_user、ftp_password。**
```python
ftp = FTP() 
ftp.connect(ftp_ip, ftp_port) # 连接ftp 
ftp.login(ftp_user, ftp_password) # ftp登录 
print(ftp.getwelcome()) # 查看欢迎信息
```
成功则打印：
```python
220 Serv-U FTP Server v15.0 ready...
```
<a name="kWtsp"></a>
### 2.2、常规操作指令（对标linux指令）
```python
ftp.cwd(path)  # 进入目录：cd
ftp.pwd()      # 返回当前目录路径：pwd
ftp.dir()      # 显示目录下所有信息：ls -alh
ftp.nlst()     # 获取目录下文件名列表: ls => ['file1', 'file2',...]
ftp.mkd(directory)    # 新建远程目录: mkdir(directory)
ftp.rmd(directory)    # 删除远程目录: rm -rf directory
ftp.delete(file_name) # 删除远程文件: rm file_name
ftp.rename(old, new)  # 将远程文件old重命名为new: mv old new
```
<a name="OqPxd"></a>
### 2.3、上传文件
上传文件，cmd是一个存储命令，可以为"STOR filename.txt"，fp为类文件对象（有read方法），bufsize设置缓冲大小。
```python
ftp.storbinary(cmd, fp, bufsize)
```
<a name="UMiNf"></a>
### 2.4、下载文件
下载文件，cmd是一个获取命令，可以为"RETR filename.txt"， callback是一个回调函数，用于读取获取到的数据块。
```python
ftp.retrbinary(cmd, callback, bufsize)
```
<a name="x5DT9"></a>
### 2.5、退出 
```python
ftp.quit() # 退出连接
```
<a name="cDlc7"></a>
## 3、示例测试
```python
#!/usr/bin/env python3
# coding: utf-8

import os
import re
import ftplib

class MyFtp(object):
    """
    Ftp上传下载接口类
    """
    conn = ftplib.FTP()

    def __init__(self, host, port=21):
        self.conn.connect(host, port)
        self.conn.encoding = "utf-8"  # "GB2312"
        # self.conn.set_debuglevel(2)  # 打开调试级别2，显示详细信息
        # self.conn.set_pasv(True) # 0 主动模式 1 #被动模式

    def login(self, user, passwd):
        """
        登录
        :param user: 用户名
        :param passwd: 密码
        :return: 返回self，支持链试调用
        """
        self.conn.login(user, passwd)
        print(self.conn.welcome)
        return self

    def close(self):
        self.conn.quit()

    def download_file(self, local_file, remote_file):
        """
        下载单个文件，将 remote_file 下载到 local_file。
        :param local_file: 本地文件地址
        :param remote_file: 远程目录文件地址
        :return: 是否成功标志
        """

        ret = True
        try:
            file_handler = open(local_file, "wb")
            self.conn.retrbinary("RETR " + remote_file, file_handler.write)
            file_handler.close()
        except Exception as err:
            ret = False
            print("file down load err:", err)
        return ret

    def download_dir(self, local_dir, remote_dir, filter=""):  # 下载文件夹
        """
        下载单个文件，将 remote_dir 下载到 local_dir。
        :param local_dir: 本地文件夹地址
        :param remote_dir: 远程目录文件夹地址
        :param filter: 正则过滤器
        :return: 是否成功标志
        """

        print("远程文件夹 remote_dir:", remote_dir)
        if not os.path.exists(local_dir):
            os.makedirs(local_dir)

        self.conn.cwd(remote_dir)
        remote_names = self.conn.nlst()
        if 0 == len(remote_names):
            print("远程文件目录：", remote_names)
            return False

        for file in remote_names:
            local = os.path.join(local_dir, file)
            print("正在下载", self.conn.nlst(file))
            if file.find(".") == -1:  # 子文件夹递归
                if not os.path.exists(local):
                    os.makedirs(local)
                self.download_dir(local, file)
            else:
                if filter and not re.search(filter, file):
                    continue
                self.download_file(local, file)
        self.conn.cwd("..")
        return True

    def list_dir(self, remote_dir, filter=None, real_path=False):
        org_dir = self.conn.pwd()
        self.conn.cwd(remote_dir)
        file_names = self.conn.nlst()

        if filter is not None:
            file_names = [v for v in file_names if re.search(filter, v)]
        if real_path:
            file_names = [os.path.join(remote_dir, v) for v in file_names]

        self.conn.cwd(org_dir)
        return file_names

    def put_file(self, local_file, remote_path="."):
        """
        上传文件。如果远程路径是个目录，则上传文件到这个目录，文件名不变；
                 如果远程路径的上层是个目录，则上传文件，文件名按照给定命名。
        :param local_file: 本地文件
        :param remote_path: 远程目录或文件
        :return: 是否成功标志
        """

        ret = True
        ftp_path = remote_path.rstrip('/')
        if os.path.isfile(local_file):
            file_handler = open(local_file, 'rb')
            local_file_name = os.path.basename(local_file)

            # 如果远程路径是个目录，则上传文件到这个目录，文件名不变
            if self._is_ftp_dir(ftp_path):
                self.conn.storbinary('STOR %s' % os.path.join(ftp_path, local_file_name), file_handler)

            # 如果远程路径的上层是个目录，则上传文件，文件名按照给定命名
            elif self._is_ftp_dir(os.path.dirname(ftp_path)):
                print('STOP %s' % ftp_path)
                self.conn.storbinary('STOR %s' % ftp_path, file_handler)
            # 如果远程路径不是目录，且上一层的目录也不存在，则提示给定远程路径错误
            else:
                ret = False
                print('STOR %s' % ftp_path, file_handler)
        return ret

    def put_dir(self, local_dir, remote_dir=".", begin=True):
        """
        上传文件夹
        :param local_dir: 本地文件夹
        :param remote_dir: 远程文件夹
        :param begin:
        :return: 是否成功标志
        """

        ftp_path = remote_dir.rstrip('/')
        if not os.path.isdir(local_dir):
            print('ERROR:The dir:%s is not exist' % local_dir)
            return False

        # 当本地目录存在时上传
        # 上传初始化：如果给定的ftp路径不存在需要创建，同时将本地的目录存放在给定的ftp目录下。
        # 本地目录下文件存放的路径为ftp_path = ftp_path + os.path.basename(local_path)
        # 例如，将本地的文件夹a上传到ftp的a/b目录下，则本地a目录下的文件将上传的ftp的a/b/a目录下
        if begin:
            if not self._is_ftp_dir(ftp_path):
                try:
                    self.conn.mkd(ftp_path)
                except Exception as e:
                    print(e)
            ftp_path = os.path.join(ftp_path, os.path.basename(local_dir))

        # 如果上传路径是文件夹，则创建目录
        if not self._is_ftp_dir(ftp_path):
            try:
                self.conn.mkd(ftp_path)
            except Exception as e:
                print(e)

        # 进入本地目录，开始递归查询
        os.chdir(local_dir)
        local_files = os.listdir('.')
        for file in local_files:
            ftp_file = os.path.join(ftp_path, file)
            # 如果file本地路径是目录则递归上传文件（不需要再进行初始化begin的标志修改为False）
            # 如果file本地路径是文件则直接上传文件
            if os.path.isdir(file):
                self.put_dir(file, ftp_file, False)
            elif "idea" in file:
                pass
            else:
                self.put_file(file, ftp_path)

        # 如果当前本地目录文件已经遍历完毕返回上一层目录
        os.chdir('..')
```
