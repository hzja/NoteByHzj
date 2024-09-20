Java8 Nashorn JavaScript 引擎<br />从 JDK 1.8 开始，Nashorn 取代 Rhino(JDK 1.6, JDK1.7) 成为 Java 的嵌入式 JavaScript 引擎。它使用基于 JSR 292 的新语言特性，将 JavaScript 编译成 Java 字节码。<br />与先前的 Rhino 实现相比，这带来了 2 到 10 倍的性能提升，实例如下：
```javascript
public class JavaScriptTester {
    public static void main(String[] args) {
        ScriptEngineManager scriptEngineManager = new ScriptEngineManager();
        ScriptEngine nashorn = scriptEngineManager.getEngineByName("nashorn");
        String name = "Hello World";
        try {
            nashorn.eval("print('" + name + "')");
        }catch(ScriptException e){
            System.out.println("执行脚本错误: "+ e.getMessage());
        }
    }
}
```
输出结果：
```
Hello World
```
但是在实际的开发中，使用的比较少！
