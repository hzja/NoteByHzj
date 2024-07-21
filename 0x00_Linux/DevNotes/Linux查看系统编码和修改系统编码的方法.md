Linux 编码
<a name="XPI4u"></a>
## Linux中查看支持的字符编码
```bash
[root@iZuligp6e1dyzfZ pro]# locale
LANG=en_US.UTF-8
LC_CTYPE="en_US.UTF-8"
LC_NUMERIC="en_US.UTF-8"
LC_TIME="en_US.UTF-8"
LC_COLLATE="en_US.UTF-8"
LC_MONETARY="en_US.UTF-8"
LC_MESSAGES="en_US.UTF-8"
LC_PAPER="en_US.UTF-8"
LC_NAME="en_US.UTF-8"
LC_ADDRESS="en_US.UTF-8"
LC_TELEPHONE="en_US.UTF-8"
LC_MEASUREMENT="en_US.UTF-8"
LC_IDENTIFICATION="en_US.UTF-8"
LC_ALL=
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1609395503187-8103a87a-a2c4-4bde-ae4d-fe95f6fae0a3.png#align=left&display=inline&height=327&originHeight=980&originWidth=3323&size=1129353&status=done&style=none&width=1107.6666666666667)<br />1、 语言符号及其分类(LC_CTYPE)<br />2、 数字(LC_NUMERIC)<br />3、 比较和排序习惯(LC_COLLATE)<br />4、 时间显示格式(LC_TIME)<br />5、 货币单位(LC_MONETARY)<br />6、 信息主要是提示信息,错误信息, 状态信息, 标题, 标签, 按钮和菜单等(LC_MESSAGES)<br />7、 姓名书写方式(LC_NAME)<br />8、 地址书写方式(LC_ADDRESS)<br />9、 电话号码书写方式(LC_TELEPHONE)<br />10、度量衡表达方式(LC_MEASUREMENT)<br />11、默认纸张尺寸大小(LC_PAPER)<br />12、对locale自身包含信息的概述(LC_IDENTIFICATION)。
<a name="NWYdX"></a>
## locale设置的优先级
设定locale就是设定12大类的locale分类属性，即 12个LC_*。除了这12个变量可以设定以外，为了简便起见，还有两个变量：LC_ALL和LANG。<br />它们之间有一个优先级的关系：LC_ALL > LC_* > LANG<br />可以这么说，LC_ALL是最上级设定或者强制设定，而LANG是默认设定值。
<a name="SaKYd"></a>
## 修改编码
```bash
vim ~/.profile

export LANG zh_CN.GBK
```

