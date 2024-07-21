Jupyter Notebook<br />在使用Jupyter notebook的时候，总是看着白色的默认主题，久而久之可能会感到有些乏味。毕竟默认主题太普通，也不够酷炫，如下图所示：<br />![默认主题风格](./img/1647216926015-42c328a8-37a5-42ce-a02b-825bc9d076c6.png "默认主题风格")
<a name="dRDFR"></a>
### 安装主题
首先，通过命令行窗口或Anaconda Prompt窗口，输入如下命令安装主题：
```bash
pip install jupyterthemes
```
![安装主题](./img/1647216925928-1f992835-69e9-4dcf-8ced-d68cf3330639.png "安装主题")
<a name="MKCui"></a>
### 查看主题
```bash
jt -l
```
![查看可选主题](./img/1647216925960-93d78c86-c16c-489b-8712-16984fc65c39.png "查看可选主题")<br />可选主题如下：
```bash
Available Themes:
   chesterish
   grade3
   gruvboxd
   gruvboxl
   monokai
   oceans16
   onedork
   solarizedd
   solarizedl
```
<a name="mEfwe"></a>
### 更换主题
`-t 主题 -f(字体) -fs(字体大小) -cellw(占屏比或宽度) -ofs(输出段的字号) -T(显示工具栏) -T(显示自己主机名)`
```bash
jt -t oceans16
```
![oceans16主题](./img/1647216925985-35c914a7-e345-4a09-b11d-d45c21234c79.png "oceans16主题")
```bash
jt -t monokai
```
![monokai主题](./img/1647216926008-b40a7940-13a9-41fb-9c06-e580a4a85ae6.png "monokai主题")
```bash
jt -t monokai -f fira -fs ``16` `-cellw ``90``% -ofs ``15` `-dfs ``15` `-T -T
```
更改多个主题参数
<a name="ezjEQ"></a>
### 恢复默认主题
```bash
jt -r
```

