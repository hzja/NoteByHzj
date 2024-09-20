JDK JShell
<a name="WtDvp"></a>
## 用JShell像Unix脚本一样执行Java代码
<a name="Cj0j4"></a>
### 创建一个 `java_shell_code.txt` 的文件
不一定必须声明一个Class，可以直接写一句代码，或者创建一个方法。
```java
// java_shell_code.txt contents.

/* If you observe it does not need a class declaration.
 Using the power of jshell, we can write functions and call functions with out creating class, just like functional programming */

 // You can also include comments like above, anywhere in the script

String var1 = "Hello";

System.out.println(var1);

public int getInt1(){
    return 2;
}

public int getInt2(){
    return 4;
}

getInt1() + getInt2();

public class MyClass{
    public void sayHelloWorld(){
        System.out.println("HelloWorld");
    }
}

new MyClass().sayHelloWorld()
```
<a name="CgA8K"></a>
### 在文件中写一些Java代码，并执行以下命令
```bash
java ex.jshell.extension.JShellScriptExecutor ./java_shell_code.txt
```
<a name="1uIGo"></a>
### 执行结果
```
"Hello"
Hello
6
HelloWorld
```
<a name="jU7H1"></a>
## JShell的详细介绍
在JDK9中提供了一个新的类 `JShell.java`，它属于`jdk.jshell`模块。可以使用它执行Java代码片段，或创建一个Java方法，而不用创建一个类。如果这个Java代码片段中有错误，可以通过 Snippet.status状态（只有两种状态：REJECTED、VALID）来检查。`SourceCodeAnalysis`是用来解析代码的，它使用分号、解析方法、或类的声明等。<br />下面是`JShellScriptExecutor`代码。可以拷贝，并修改它。也可以从GitHub仓库中克隆。<br />[https://github.com/kotari4u/jshell_script_executor](https://github.com/kotari4u/jshell_script_executor)
```java
package ex.jshell.extension;
import jdk.jshell.JShell;
import jdk.jshell.Snippet;
import jdk.jshell.SnippetEvent;
import jdk.jshell.SourceCodeAnalysis;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Objects;
/**
 * This class can execute jshell expressions in sequence
 * We can write java commands like shell script and execute it.
 * Just write commands in a way that we give in jshell and save it in a file and execute it.
 *
 * @author Hemamabara Vamsi, Kotari
 * @since 5/27/2017.
 */
public class JShellScriptExecutor {
    public static void main(String[] args){
        new JShellScriptExecutor().evaluate(args[0]);
    }
    public void evaluate(String scriptFileName){
        try(JShell jshell = JShell.create()){
          // Handle snippet events. We can print value or take action if evaluation failed.
            jshell.onSnippetEvent(snippetEvent -> snippetEventHandler(snippetEvent));
            String scriptContent = new String(Files.readAllBytes(Paths.get(scriptFileName)));
            String s = scriptContent;
            while (true) {
                // Read source line by line till semicolon (;)
                SourceCodeAnalysis.CompletionInfo an = jshell.sourceCodeAnalysis().analyzeCompletion(s);
                if (!an.completeness().isComplete()) {
                    break;
                }
                // If there are any method declaration or class declaration 
                // in new lines, resolve it.
                jshell.eval(trimNewlines(an.source()));
                // EOF
                if (an.remaining().isEmpty()) {
                    break;
                }
                // If there is semicolon, execute next seq
                s = an.remaining();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public void snippetEventHandler(SnippetEvent snippetEvent){
        String value = snippetEvent.value();
        if(!Objects.isNull(value) && value.trim().length() > 0) {
          // Prints output of code evaluation
            System.out.println(value);
        }
        // If there are any erros print and exit
        if(Snippet.Status.REJECTED.equals(snippetEvent.status())){
            System.out.println("Evaluation failed : "+snippetEvent.snippet().toString()
                                +"\nIgnoring execution of above script");
        }
    }
    private String trimNewlines(String s) {
        int b = 0;
        while (b < s.length() && s.charAt(b) == '\n') {
            ++b;
        }
        int e = s.length() -1;
        while (e >= 0 && s.charAt(e) == '\n') {
            --e;
        }
        return s.substring(b, e + 1);
    }
}
```
<a name="YI3eM"></a>
### module-info.java
```java
module ex.jshell.extension{   
  requires jdk.jshell;
}
```
