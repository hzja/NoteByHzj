Linux printf
<a name="lr3tU"></a>
### 1、printf的语法格式
```bash
# printf 格式 参数
```
下面是一个例子：
```bash
[root@iZuligp6e1dyzfZ / 09:33]# printf "姓名：%s\n身高：%dcm\n体重：%dkg\n" "小明" "180" "75"
姓名：小明
身高：180cm
体重：75kg
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598578691300-fd175881-144f-434f-b0e5-f0f2ed8e933f.png#align=left&display=inline&height=110&originHeight=331&originWidth=3323&size=322678&status=done&style=none&width=1107.6666666666667)<br />"姓名：%s\n身高：%dcm\n体重：%dkg\n"是格式，"小明" "180" "75"是参数。格式包含了%s , %d都是格式转换符，%d对应的参数必须是十进制数。%s对应的参数必须是字符串。还包括了三个\n换行符，printf命令默认不会添加还行符号，需要手动添加。
<a name="koASL"></a>
### 2、常用的转义字符

- \" - 转义后的双引号
- \\ - 转义后的反斜杠
- \b - 退格符
- \n - 换行符
- \r - 回车符
- \t - 水平制表符
- \v - 垂直制表符
- %% - 单个%符号
<a name="Wre70"></a>
### 3、常用的类型转换符
类型转换说明符是一个字符，用于指定如何解释相应的参数，该字符是必须添加的，以下是显示所有类型转换及其作用的列表：

- %d - 将参数打印为十进制整数
- %f - 将参数打印为浮点数
- %s - 将参数打印为字符串
- %x - 将参数打印为十六进制整数
- %o - 将参数打印为八进制整数
<a name="UIbiY"></a>
### 4、实例
实例一：使用%d,%x,%o将后面提供的参数转化为十进制、十六进制、八进制。
```
[root@iZuligp6e1dyzfZ / 09:34]# printf "Decimal: %d\nHex: %x\nOctal: %o\n" 100 100 100
Decimal: 100
Hex: 64
Octal: 144
```
实例二：使用%.2f，其中.2指参数保留两位小数，f将参数打印为浮点数。
```
[root@iZuligp6e1dyzfZ / 09:34]# printf "%.2f\n" 3.1415926
3.14
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598578725725-fa68a6a3-f698-40c5-b457-01c10aa9ad86.png#align=left&display=inline&height=73&originHeight=218&originWidth=3323&size=179660&status=done&style=none&width=1107.6666666666667)<br />实例三：使用了水平制表符\t。例子中给的参数超过了格式中给出的转换符的数量，多出的参数仍会按照格式输出。
```
[root@iZuligp6e1dyzfZ / 09:34]# printf "%s \t %s \t %s\n" "姓名" "性别" "年龄" "小明" "男" "18" "小红" "女" "19" "小蓝" "男" "18"
姓名     性别    年龄
小明     男      18
小红     女      19
小蓝     男      18
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598578789040-2d2f8cdf-b035-4940-9127-da2a2a1db3ed.png#align=left&display=inline&height=145&originHeight=435&originWidth=3323&size=468324&status=done&style=none&width=1107.6666666666667)<br />实例四：%-10s , %-8d其中-表示靠左对齐，默认情况下是靠右对齐。10和8表示占用10或者8个字符宽度。\e[1,30;47m ..... \e[0m表示使用粗体字、黑色字体、白色背景，后面的\e[0,m表示重置。
```
[root@localhost ~]# cat p.sh 
#!/bin/bash
printf "\e[1;30;47m%-10s %-10s\e[0m\n" "姓名" "年龄"
printf "\e[36;47m%-10s %-8d\e[0m\n" "小明" "18"
printf "\e[31;47m%-10s %-8d\e[0m\n" "小红" "19"
printf "\e[34;47m%-10s %-8d\e[0m\n" "小蓝" "19"
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598578839495-aef29887-1d69-461f-bf3b-36965644ec60.png#align=left&display=inline&height=253&originHeight=758&originWidth=3323&size=830672&status=done&style=none&width=1107.6666666666667)
