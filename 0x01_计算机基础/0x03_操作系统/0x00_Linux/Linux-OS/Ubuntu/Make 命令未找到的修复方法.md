Linux<br />使用 make 命令时，抛出一个错误：
```sql
The program 'make' is currently not installed. You can install it by typing:
sudo apt install make
```
这表明 make 命令还没有安装。可以用这些命令在 Ubuntu 上逐步安装 make：
```bash
sudo apt update
sudo apt install make
```
第一个命令是更新本地的软件包缓存。如果是一个新安装的 Ubuntu 系统，这是很有必要的。有了刷新的软件包缓存，系统就会知道应该从哪个仓库下载 make 包。<br />并验证 make 是否已经正确安装：
```bash
make --version
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622897926429-72c71cfd-0804-4f1c-8259-b1ae593ef574.webp#clientId=u0dbb57f3-ca1a-4&from=paste&id=u820d69b2&originHeight=293&originWidth=800&originalType=url&ratio=3&status=done&style=none&taskId=u5c78a5df-e327-4e4e-9973-00420329da9)<br />Checking make version
<a name="eGMEK"></a>
## 在 Ubuntu 上安装 make 的更好方法
安装 make 命令的一个更好的方法是使用 `build-essential` 包。这个包包含 make、gcc、g++ 和其他一些编译器和开发工具。
```bash
sudo apt install build-essential
```
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1622897926640-55c5141d-acca-470f-9fc9-fbc08a23a8c9.webp#clientId=u0dbb57f3-ca1a-4&from=paste&id=uec0747f3&originHeight=410&originWidth=800&originalType=url&ratio=3&status=done&style=none&taskId=ub7fd70b9-56b0-4b36-bb4c-f6d2b2c519c)<br />Installing Build Essential package<br />安装了这个 `build-essential` 包后，就可以在 Linux 中轻松地运行 C/C++ 程序。
<a name="Oa9QN"></a>
## 如果 make 已经安装了，但它没有工作怎么办？
在一些罕见的情况下，可能会发生 make 已经安装了，但却无法工作的情况。<br />其原因是 make 命令不在 `$PATH` 变量中。可以用这个命令重新安装 make：
```bash
sudo apt install --reinstall make
```
如果这不起作用，可以尝试 手动添加二进制文件到 PATH 中，但这应该不需要手动。
