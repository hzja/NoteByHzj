Linux exa 
<a name="npNFI"></a>
## 1、exa的介绍
exa能够高亮的列出文件以及目录<br />exa和ls、ll的对比<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596856976559-c0f556c0-2816-4ae3-9de1-fa95731afb0d.png#height=583&id=urq7N&originHeight=1750&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2304345&status=done&style=none&title=&width=1107.6666666666667)
<a name="21qLY"></a>
## 2、exa的官网
[exa的官网地址](https://the.exa.website/)<br />[https://the.exa.website/](https://the.exa.website/)
<a name="NFxqO"></a>
## 3、exa的安装包
[exa-linux-x86_64-v0.10.0.zip](https://www.yuque.com/attachments/yuque/0/2022/zip/396745/1644194849205-81398b00-9ac4-41d3-bebd-6b69ccdffd66.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2022%2Fzip%2F396745%2F1644194849205-81398b00-9ac4-41d3-bebd-6b69ccdffd66.zip%22%2C%22name%22%3A%22exa-linux-x86_64-v0.10.0.zip%22%2C%22size%22%3A717968%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22status%22%3A%22done%22%2C%22taskId%22%3A%22u4c495274-7395-4290-accc-e0ca93dea5c%22%2C%22taskType%22%3A%22upload%22%2C%22id%22%3A%22u64655e51%22%2C%22card%22%3A%22file%22%7D)<br />[exa-linux-x86_64-0.9.0.zip](https://www.yuque.com/attachments/yuque/0/2020/zip/396745/1596445979888-9d4098d1-4c01-4f70-ab0e-17974e6daeab.zip?_lake_card=%7B%22src%22%3A%22https%3A%2F%2Fwww.yuque.com%2Fattachments%2Fyuque%2F0%2F2020%2Fzip%2F396745%2F1596445979888-9d4098d1-4c01-4f70-ab0e-17974e6daeab.zip%22%2C%22name%22%3A%22exa-linux-x86_64-0.9.0.zip%22%2C%22size%22%3A675661%2C%22type%22%3A%22application%2Fx-zip-compressed%22%2C%22ext%22%3A%22zip%22%2C%22status%22%3A%22done%22%2C%22uid%22%3A%221596445979627-0%22%2C%22progress%22%3A%7B%22percent%22%3A99%7D%2C%22percent%22%3A0%2C%22id%22%3A%226hS8g%22%2C%22card%22%3A%22file%22%7D)
<a name="8i0sX"></a>
## 4、exa的安装
<a name="zj2Fm"></a>
### A.将下载的zip文件包里的解压文件修改名称为exa
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596855282771-8f86c464-9576-4d09-a928-3614616f061a.png#height=525&id=lMyDh&originHeight=1574&originWidth=2193&originalType=binary&ratio=1&rotation=0&showTitle=false&size=329339&status=done&style=none&title=&width=731)
<a name="RVmKf"></a>
### B.将exa文件上传至Linux的具有可执行权限的目录下
官网解释如下<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596855518914-5295e3ca-787a-4299-850a-a543c9e5d0e6.png#height=315&id=puhNN&originHeight=945&originWidth=1769&originalType=binary&ratio=1&rotation=0&showTitle=false&size=171375&status=done&style=none&title=&width=589.6666666666666)
```bash
[root@iZuligp6e1dyzfZ upload]# mv exa /usr/local/bin/
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596855600942-af09f739-9453-41b3-82f7-206e2c9d7d47.png#height=221&id=fyL1o&originHeight=663&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=900136&status=done&style=none&title=&width=1107.6666666666667)
<a name="DReUG"></a>
### C.执行exa报错
```bash
[root@iZuligp6e1dyzfZ bin]# ll
total 1452
-rwxrwx--- 1 rsync rsync 1486840 Jul 15  2019 exa
[root@iZuligp6e1dyzfZ bin]# exa
exa: /lib64/libc.so.6: version `GLIBC_2.18' not found (required by exa)
```
<a name="7BfEK"></a>
### D.下载安装相关缺少的依赖
```bash
curl -O http://ftp.gnu.org/gnu/glibc/glibc-2.18.tar.gz
tar zxf glibc-2.18.tar.gz 
cd glibc-2.18/
mkdir build
cd build/
../configure --prefix=/usr
make -j2
make install
```
<a name="5t2qM"></a>
### E.再次执行exa即可看到效果
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596856513024-b54f2ecc-a55f-44c1-b368-3a36f78e05e2.png#height=433&id=zhUWB&originHeight=1300&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1712922&status=done&style=none&title=&width=1107.6666666666667)
<a name="oauso"></a>
## 5、exa的使用
<a name="wFto6"></a>
### A.exa的语法
```bash
exa [OPTIONS] [FILES]
```
<a name="wSNdP"></a>
### B.exa的参数
[命令参考官方文档](https://the.exa.website/docs/command-line-options)<br />[https://the.exa.website/docs/command-line-options](https://the.exa.website/docs/command-line-options)
<a name="wHxLI"></a>
#### -1 列出文件名，每行显示一个文件
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596857459661-56ab2047-fa29-4bff-9b41-047f3d61b539.png#height=432&id=YD7G3&originHeight=1295&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1555777&status=done&style=none&title=&width=1107.6666666666667)
<a name="kbYBK"></a>
#### -l 按行列出文件详情
```bash
exa -l
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596856740687-b15d09aa-aea2-4adf-84d8-0da129b9ceca.png#height=433&id=cncl7&originHeight=1300&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1712922&status=done&style=none&title=&width=1107.6666666666667)
<a name="nqNku"></a>
##### 当与--long（-l）一起运行时，这些选项可用：
-b，--binary 列出带有二进制前缀的文件大小。<br />-B，--bytes 列出文件大小（以字节为单位），不带任何前缀。<br />-g，--group 列出每个文件的组。<br />-h，--header 将标题行添加到表中的每一列。<br />-H，-链接 列出每个文件的硬链接数。<br />-i，-inode 列出每个文件的索引节点号。<br />-m，-修改 使用修改后的时间戳字段。<br />-S，-块 列出每个文件的文件系统阻止计数。<br />-t，--time = WORD 配置要列出（修改，访问，创建）的时间戳字段。<br />--time-style =样式 配置时间戳记应采用的格式。<br />default使用当前语言环境来打印月份名称，将时间戳记指定为当年年份的分钟，而指定为前几年的日期。<br />iso执行相同的操作，只是使用一个月作为数字，因此不需要查找语言环境。<br />long-iso在不使用语言环境或当前年份的情况下将时间戳指定为分钟。<br />full-iso可将时间戳指定为毫秒（包括向下偏移为分钟），而无需使用语言环境或当前年份。<br />-u，-已访问 使用访问的时间戳字段。<br />-U，-创建 使用创建的时间戳字段。<br />-@，-扩展 列出每个文件的扩展属性和大小。<br />--git 列出每个文件的Git状态（如果已跟踪）。<br />--color-scale，--colour-scale 突出显示文件大小级别。
<a name="lf4Lp"></a>
#### -G 网格状展示
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596857441388-0940fc7f-f543-43e0-9711-1c939ab02e5e.png#height=270&id=Limhj&originHeight=811&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1104421&status=done&style=none&title=&width=1107.6666666666667)
<a name="-a"></a>
#### -a 全部显示，包括隐藏的文件以及以.开头的文件
<a name="f252eb63"></a>
#### -x，--across对网格进行排序，而不是向下排序
<a name="TfLU3"></a>
#### -R，-递归，递归到目录
<a name="BiAZS"></a>
#### -T，--tree以树的形式递归到目录中
<a name="PuzHY"></a>
#### -L，--level =深度限制递归的深度，因此exa仅下降到给定的次数。
<a name="eEPZu"></a>
#### -F，-分类按文件名显示文件类型指示符
<a name="ZR8EQ"></a>
#### --color，--colour 配置客户端渲染的颜色。默认选项是自动的，这意味着exa写入终端时会使用颜色，否则将关闭
<a name="rSyGG"></a>
#### -d，-无递归，-list-dirs 列出目录而不递归到目录中，就像它们是常规文件一样
<a name="A2V0l"></a>
#### -r，--reverse 颠倒排序顺序
<a name="g1Cp4"></a>
#### -s，--sort =排序字段 配置要排序的字段
<a name="fcXVZ"></a>
#### -I，--ignore-glob = GLOBS 忽略文件的通配符模式（用管道分隔）。
<a name="IgGrS"></a>
#### --git-ignore 忽略.gitignore中提到的文件。
<a name="oMNwW"></a>
#### --group-directories-first 排序时在其他文件之前列出目录。
