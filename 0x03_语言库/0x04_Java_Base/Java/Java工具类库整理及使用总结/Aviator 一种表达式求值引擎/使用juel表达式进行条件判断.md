Java
<a name="XDvBb"></a>
## 简介
Juel是统一表达语言轻量而高效级的实现，具有高性能，插件式缓存，小体积，支持方法调用和多参数调用，可插拔多种特性。具体可以去Juel官方网站进行详细阅读。
<a name="L2T9F"></a>
## 具体实现
<a name="iUWub"></a>
### 一、在pom文件中引入juel依赖
```xml
<dependency>
  <groupId>de.odysseus.juel</groupId>
  <artifactId>juel</artifactId>
  <version>2.1.3</version>
</dependency>
```
<a name="pdkwb"></a>
### 二、创建工具类
```java
public final class ConditionElUtil {

    private ConditionElUtil(){}

    public static Boolean checkFormDataByRuleEl(String el, Map<String, Object> formData) {

        ExpressionFactory factory = new ExpressionFactoryImpl();
        SimpleContext context = new SimpleContext();

        for (Map.Entry<String,Object> entry : formData.entrySet()) {
            Object value = entry.getValue();
            if (value != null) {
                context.setVariable(entry.getKey(), factory.createValueExpression(value, value.getClass()));
            }
        }

        ValueExpression e = factory.createValueExpression(context, el, Boolean.class);
        return (Boolean) e.getValue(context);
    }
}
```
<a name="bGVjC"></a>
### 三、测试
```java
@Test
public void checkFormDataByRuleEl() throws Exception {
    String el = "${请假天数>3 && 部门 == '产品部'}";
    Map<String, Object> formData = new HashMap<>();
    formData.put("请假天数", 6);
    formData.put("部门", "产品部");
    System.out.println(ConditionElUtil.checkFormDataByRuleEl(el, formData));
}
```
