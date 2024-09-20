JavaWeb 端口占用
<a name="uHP4n"></a>
## 1、错误描述
<a name="vV6hM"></a>
### A、SpringBoot启动错误描述
```
***************************
APPLICATION FAILED TO START
***************************

Description:

Web server failed to start. Port 8000 was already in use.

Action:

Identify and stop the process that's listening on port 8000 or configure this application to listen on another port.

```
<a name="ZC0Xa"></a>
## 2、解决办法
<a name="K4D8D"></a>
### A、打开CMD命令行窗口
<a name="JtznH"></a>
### B、输入以下命令查找占用端口的PID
```bash
netstat -aon|findstr "8080"  
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598321537380-5e02615e-21f7-4ad1-acf4-54388151c088.png#averageHue=%23464646&height=253&id=nsjMJ&originHeight=760&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=864081&status=done&style=none&title=&width=1107.6666666666667)
<a name="QiA1p"></a>
### C、使用以下命令Kill此进程
```bash
taskkill /f /pid 13884
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598321558378-43a6f15e-4b26-4aff-9547-841b4be7edd0.png#averageHue=%23323232&height=276&id=SZe5B&originHeight=829&originWidth=3323&originalType=binary&ratio=1&rotation=0&showTitle=false&size=972516&status=done&style=none&title=&width=1107.6666666666667)
