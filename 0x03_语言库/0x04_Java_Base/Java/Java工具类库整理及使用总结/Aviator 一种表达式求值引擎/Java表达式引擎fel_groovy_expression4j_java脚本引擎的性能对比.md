Java表达式引擎<br />产品中需要用到数学表达式，表达式不复杂，但是对性能要求比较高。选用了一些常用的表达式引擎计算方案，包含：java脚本引擎（javax/script）、groovy脚本引擎、Expression4j、Fel表达式引擎。 其中java脚本引擎使用了解释执行和编译执行两种方式、groovy脚本只采用了编译执行（解释执行太慢）、Fel采用了静态参数和动态参数两种方式。以下为测试代码：
```java
public class ExpressionTest extends BaseTest {
 
    private int count = 100000;
    
    //javax的编译执行，效率比解释执行略高？为什么才略高？？
    @Test
    public void testCompiledJsScript() throws Throwable {
        javax.script.ScriptEngine se = new ScriptEngineManager().getEngineByName("js");
        Compilable ce = (Compilable) se;
        CompiledScript cs = ce.compile("a*b*c");
        Bindings bindings = se.createBindings();
        bindings.put("a", 3600);
        bindings.put("b", 14);
        bindings.put("c", 4);
        long start = System.currentTimeMillis();
        for (int i = 0; i < count; i++) {
            cs.eval(bindings);
        }
        System.out.println(System.currentTimeMillis() - start);
    }
 
    //javax script解释执行
    @Test
    public void testJsScript() throws Throwable {
        javax.script.ScriptEngine se = new ScriptEngineManager().getEngineByName("js");
        Bindings bindings = se.createBindings();
        bindings.put("a", 3600);
        bindings.put("b", 14);
        bindings.put("c", 4);
        long start = System.currentTimeMillis();
        for (int i = 0; i < count; i++) {
            se.eval("a*b*c", bindings);
        }
        System.out.println(System.currentTimeMillis() - start);
    }
 
    //groovy的编译执行
    @Test
    public void testGroovy() {
        //这里的ScriptEngine和GroovyScriptEngine是自己编写的类，不是原生的
        ScriptEngine se = this.getBean(GroovyScriptEngine.class);
        Map<String, Object> paramMap = new HashMap<String, Object>();
        paramMap.put("param", 5);
        //ScriptEngine首次执行会缓存编译后的脚本，这里故意先执行一次便于缓存
        se.eval("3600*34*param", paramMap);
        
        long start = System.currentTimeMillis();
        for (int i = 0; i < count; i++) {
            se.eval("3600*34*param", paramMap);
        }
        System.out.println(System.currentTimeMillis() - start);
    }
 
    //Expression4J的表达式引擎，这里是通过函数的方式，有点特别
    @Test
    public void testExpression4j() throws Throwable {
        Expression expression = ExpressionFactory.createExpression("f(a,b,c)=a*b*c");
        System.out.println("Expression name: " + expression.getName());
 
        System.out.println("Expression parameters: " + expression.getParameters());
 
        MathematicalElement element_a = NumberFactory.createReal(3600);
        MathematicalElement element_b = NumberFactory.createReal(34);
        MathematicalElement element_c = NumberFactory.createReal(5);
        Parameters parameters = ExpressionFactory.createParameters();
        parameters.addParameter("a", element_a);
        parameters.addParameter("b", element_b);
        parameters.addParameter("c", element_c);
        long start = System.currentTimeMillis();
        for (int i = 0; i < count; i++) {
            expression.evaluate(parameters);
        }
        System.out.println(System.currentTimeMillis() - start);
    }
 
    //fel的表达式引擎（静态参数，同上面）
    @Test
    public void felTest() {
        FelEngine e = FelEngine.instance;
        final FelContext ctx = e.getContext();
        ctx.set("a", 3600);
        ctx.set("b", 14);
        ctx.set("c", 5);
        com.greenpineyu.fel.Expression exp = e.compile("a*b*c", ctx);
        long start = System.currentTimeMillis();
        Object eval = null;
        for (int i = 0; i < count; i++) {
            eval = exp.eval(ctx);
        }
        System.out.println(System.currentTimeMillis() - start);
        System.out.println(eval);
    }
 
    //fel表达式引擎（动态参数，这里动态参数的产生和变量改变都会消耗时间，因此这个测试时间不准确，只是验证对于动态参数的支持）
    @Test
    public void felDynaTest() {
        FelEngine e = FelEngine.instance;
        final FelContext ctx = e.getContext();
        ctx.set("a", 3600);
        ctx.set("b", 14);
        ctx.set("c", 5);
        com.greenpineyu.fel.Expression exp = e.compile("a*b*c", ctx);
        long start = System.currentTimeMillis();
        Object eval = null;
        Random r = new Random();
        for (int i = 0; i < count; i++) {
            ctx.set("a", r.nextInt(10000));
            ctx.set("b", r.nextInt(100));
            ctx.set("c", r.nextInt(100));
            eval = exp.eval(ctx);
        }
        System.out.println(System.currentTimeMillis() - start);
        System.out.println(eval);
    }
 
    public static void main(String[] args) throws Throwable {
        ExpressionTest et = new ExpressionTest();
        //执行100W次的测试
        et.count = 1000000;
        et.testCompiledJsScript();
        et.testJsScript();
        et.testExpression4j();
        et.testGroovy();
        et.felTest();
    }
 
}
```
测试结果如下：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1662740569679-94445b34-f2bd-4de3-99e9-685de031542b.png#clientId=u1a1e9512-ca79-4&from=paste&id=u4a28ed8a&originHeight=291&originWidth=872&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u4c41f3d5-23e3-4819-9d25-d6974efb642&title=)<br />**结论：**从以上性能对比来看（抛开表达式的功能），fel明显占据很大优势，groovy和expression4j也是可以接受的。java脚本引擎的执行偏慢。因此，对于表达式不是很复杂性能要求高的情况下，推荐使用fel或者groovy编译执行的方式。
