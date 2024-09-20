Java Fastjson<br />在开发过程中遇到一个问题：服务器经过排序返回后的字符串数据在使用fastjson解析后，数据顺序发生了变化，这个问题也就是：使用fastjson解析数据后导致顺序改变或者说是如何保持String字符串转为json对象时顺序不变
<a name="Wv1tl"></a>
## 解决方法：
<a name="XaOq6"></a>
### 方法一：解析时增加参数不调整顺序（亲测使用有效）
```java
JSONObject respondeBodyJson = JSONObject.parseObject(str, Feature.OrderedField);
```
<a name="Qb7CK"></a>
### 方法二：配置有序对象
```java
JSONObject.parseObject(str,LinkedHashMap.class,Feature.OrderedField);
```
<a name="FMDY5"></a>
### 方法三：初始化json对象为有序对象
```java
JSONObject retObj = new JSONObject(true);
```
<a name="lm18D"></a>
### 方法四：使用Gson解析
```java
JsonObject returnData = new JsonParser().parse(str).getAsJsonObject();
```
这样生成的json对象就与放入数据时保持一致了
:::tips
注意：引入的fastjson相关的jar包版本要高于1.2.3，因为Feature.OrderedField是从1.2.3开始的
:::

