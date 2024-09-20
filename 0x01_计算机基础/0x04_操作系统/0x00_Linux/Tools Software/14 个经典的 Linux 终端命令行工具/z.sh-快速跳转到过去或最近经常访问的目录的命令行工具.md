Linux z.sh <br />它会根据使用目录的频率去区分想要跳转的目录，此外，还可以跟踪何时进入目录以及在目录中花费了多少时间。<br />然后，只需要输入路径的部分字段，它就可以预测、补全想要跳转的路径。<br />例如`z src`可以跳转到`~/src/zsh`
<a name="9XiL9"></a>
### z.sh的Github
[zsh的Github](https://github.com/rupa/z)<br />[https://github.com/rupa/z](https://github.com/rupa/z)
<a name="Lvzpy"></a>
### z.sh安装使用
将源码下载复制到用户目录下
```bash
wget https://github.com/rupa/z/blob/master/z.sh
```
<a name="KgwSQ"></a>
### 在 .bashrc 这个文件的最后添加配置
```bash
vim ~/.bashrc

# 添加以下内容
. /path/to/z.sh

# 使配置立即生效
source ~/.bashrc
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1644213902604-904ff61a-eac4-4776-b617-1fc162e1b5f2.png#clientId=u2055e8f4-e148-4&from=paste&height=654&id=u0d3a14e7&originHeight=1634&originWidth=1200&originalType=binary&ratio=1&rotation=0&showTitle=false&size=51941&status=done&style=none&taskId=u55709632-9482-421b-bfcd-2dfe22bab88&title=&width=480)

