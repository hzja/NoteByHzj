Java
<a name="aprIn"></a>
## 一、背景
有些业务场景下需要将 Java Bean 转成 Map 再使用。<br />本以为很简单场景，但是坑很多。
<a name="jr5fB"></a>
## 二、那些坑
<a name="ApoRl"></a>
### **2.0 测试对象**
```java
import lombok.Data;

import java.util.Date;

@Data
public class MockObject extends  MockParent{

    private Integer aInteger;

    private Long aLong;

    private Double aDouble;

    private Date aDate;
}
```
父类
```java
import lombok.Data;

@Data
public class MockParent {
    private Long parent;
}
```
<a name="hEiYE"></a>
### **2.1 JSON 反序列化了类型丢失**
<a name="tmuaT"></a>
#### 2.1.1 问题复现
将 Java Bean 转 Map 最常见的手段就是使用  JSON 框架，如 fastjson 、 gson、jackson 等。<br />但使用 JSON 将 Java Bean 转 Map 会导致部分数据类型丢失。<br />如使用 fastjson ，当属性为 Long 类型但数字小于 Integer 最大值时，反序列成 Map 之后，将变为  Integer 类型。<br />maven 依赖：
```xml
<!-- https://mvnrepository.com/artifact/com.alibaba/fastjson -->
<dependency>
	<groupId>com.alibaba</groupId>
	<artifactId>fastjson</artifactId>
	<version>2.0.8</version>
</dependency>
```
示例代码：
```java
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.TypeReference;

import java.util.Date;
import java.util.Map;

public class JsonDemo {
	
	public static void main(String[] args) {
		MockObject mockObject = new MockObject();
		mockObject.setAInteger(1);
		mockObject.setALong(2L);
		mockObject.setADate(new Date());
		mockObject.setADouble(3.4D);
		mockObject.setParent(3L);
		
		String json = JSON.toJSONString(mockObject);
		
		Map<String,Object> map =  JSON.parseObject(json, new TypeReference<Map<String,Object>>(){});
		
		System.out.println(map);
	}
}
```
结果打印：
```
{"parent":3,"ADouble":3.4,"ALong":2,"AInteger":1,"ADate":1657299916477}
```
调试截图：<br />![2022-07-23-18-43-31.694934300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658573420093-19162e44-53ec-4283-86d9-cb9e1c1bc778.png#clientId=uf2fc60f7-48c3-4&from=ui&id=FGOVF&originHeight=768&originWidth=1062&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3268587&status=done&style=none&taskId=u434cbff2-01ae-4477-b2ef-35a0a77a899&title=)<br />通过 Java Visualizer 插件进行可视化查看：<br />![2022-07-23-18-43-31.743448000.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658573419929-4f805ebd-602e-4a8e-a1d6-e9a99f1fd451.png#clientId=uf2fc60f7-48c3-4&from=ui&id=uc6f6801c&originHeight=539&originWidth=1154&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2492658&status=done&style=none&taskId=u98d652a4-d973-481a-abba-2e14011f2ca&title=)
<a name="cyeae"></a>
#### 2.2.2 问题描述
存在两个问题：

- **通过 fastjson 将 Java Bean 转为 Map ，类型会发生转变。如 Long 变成 Integer ，Date 变成 Long，Double 变成  Decimal 类型等。**
- **在某些场景下，Map 的  key 并非和属性名完全对应，像是通过 get set 方法“推断”出来的属性名。**
<a name="TYkBf"></a>
### **2.2 BeanMap 转换属性名错误**
<a name="FUhdx"></a>
#### 2.2.1 commons-beanutils 的 BeanMap
maven 版本：
```xml
<!-- https://mvnrepository.com/artifact/commons-beanutils/commons-beanutils -->
<dependency>
	<groupId>commons-beanutils</groupId>
	<artifactId>commons-beanutils</artifactId>
	<version>1.9.4</version>
</dependency>
```
代码示例：
```java
import org.apache.commons.beanutils.BeanMap;
import third.fastjson.MockObject;

import java.util.Date;

public class BeanUtilsDemo {
	public static void main(String[] args) {
		MockObject mockObject = new MockObject();
		mockObject.setAInteger(1);
		mockObject.setALong(2L);
		mockObject.setADate(new Date());
		mockObject.setADouble(3.4D);
		mockObject.setParent(3L);

		BeanMap beanMap = new BeanMap(mockObject);
		System.out.println(beanMap);
	}
}
```
调试截图：<br />![2022-07-23-18-43-31.843437200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658573420088-3f54b519-38d2-400b-ae3e-ea1654ee3295.png#clientId=uf2fc60f7-48c3-4&from=ui&id=wo7qx&originHeight=762&originWidth=1058&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3230836&status=done&style=none&taskId=u7b64afce-ed1a-4f03-aa19-47c20f35f27&title=)存在和 cglib 一样的问题，虽然类型没问题但是属性名还是不对。<br />原因分析：
```java
/**
     * Constructs a new <code>BeanMap</code> that operates on the
     * specified bean.  If the given bean is <code>null</code>, then
     * this map will be empty.
     *
     * @param bean  the bean for this map to operate on
     */
public BeanMap(final Object bean) {
	this.bean = bean;
	initialise();
}
```
关键代码：
```java
private void initialise() {
	if(getBean() == null) {
		return;
	}

	final Class<? extends Object>  beanClass = getBean().getClass();
	try {
		//BeanInfo beanInfo = Introspector.getBeanInfo( bean, null );
		final BeanInfo beanInfo = Introspector.getBeanInfo( beanClass );
		final PropertyDescriptor[] propertyDescriptors = beanInfo.getPropertyDescriptors();
		if ( propertyDescriptors != null ) {
			for (final PropertyDescriptor propertyDescriptor : propertyDescriptors) {
				if ( propertyDescriptor != null ) {
					final String name = propertyDescriptor.getName();
					final Method readMethod = propertyDescriptor.getReadMethod();
					final Method writeMethod = propertyDescriptor.getWriteMethod();
					final Class<? extends Object> aType = propertyDescriptor.getPropertyType();

					if ( readMethod != null ) {
						readMethods.put( name, readMethod );
					}
					if ( writeMethod != null ) {
						writeMethods.put( name, writeMethod );
					}
					types.put( name, aType );
				}
			}
		}
	}
	catch ( final IntrospectionException e ) {
		logWarn(  e );
	}
}
```
调试一下就会发现，问题出在 `BeanInfo` 里面 `PropertyDescriptor` 的 name 不正确。<br />![2022-07-23-18-43-32.103086800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658573561153-a151c05a-10bc-48e7-9401-192472022d5d.png#clientId=uf2fc60f7-48c3-4&from=ui&id=u275f31a9&originHeight=673&originWidth=1075&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2899333&status=done&style=none&taskId=u273a348a-7956-4d4a-8df2-62e41e2785b&title=)<br />经过分析会发现 `java.beans.Introspector#getTargetPropertyInfo` 方法是字段解析的关键<br />![2022-07-23-18-43-32.385364200.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658573471836-fcb1c2d6-bdc1-4299-92e4-5b59e0563832.png#clientId=uf2fc60f7-48c3-4&from=ui&id=SJ5uJ&originHeight=822&originWidth=1062&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3498404&status=done&style=none&taskId=u216044d5-83cb-4604-b9b7-abc981145f4&title=)<br />对于无参的以 get 开头的方法名从 `index =3` 处截取，如 getALong 截取后为 ALong， 如 `getADouble` 截取后为 `ADouble`。<br />然后去构造 `PropertyDescriptor`:
```java
/**
     * Creates <code>PropertyDescriptor</code> for the specified bean
     * with the specified name and methods to read/write the property value.
     *
     * @param bean   the type of the target bean
     * @param base   the base name of the property (the rest of the method name)
     * @param read   the method used for reading the property value
     * @param write  the method used for writing the property value
     * @exception IntrospectionException if an exception occurs during introspection
     *
     * @since 1.7
     */
PropertyDescriptor(Class<?> bean, String base, Method read, Method write) throws IntrospectionException {
	if (bean == null) {
		throw new IntrospectionException("Target Bean class is null");
	}
	setClass0(bean);
	setName(Introspector.decapitalize(base));
	setReadMethod(read);
	setWriteMethod(write);
	this.baseName = base;
}
```
底层使用 `java.beans.Introspector#decapitalize` 进行解析：
```java
/**
     * Utility method to take a string and convert it to normal Java variable
     * name capitalization.  This normally means converting the first
     * character from upper case to lower case, but in the (unusual) special
     * case when there is more than one character and both the first and
     * second characters are upper case, we leave it alone.
     * <p>
     * Thus "FooBah" becomes "fooBah" and "X" becomes "x", but "URL" stays
     * as "URL".
     *
     * @param  name The string to be decapitalized.
     * @return  The decapitalized version of the string.
     */
public static String decapitalize(String name) {
	if (name == null || name.length() == 0) {
		return name;
	}
	if (name.length() > 1 && Character.isUpperCase(name.charAt(1)) &&
		Character.isUpperCase(name.charAt(0))){
		return name;
	}
	char chars[] = name.toCharArray();
	chars[0] = Character.toLowerCase(chars[0]);
	return new String(chars);
}
```
从代码中可以看出

- **当 name 的长度 > 1，且第一个字符和第二个字符都大写时，直接返回参数作为PropertyDescriptor name。**
- **否则将 name 转为首字母小写**

这种处理本意是为了不让属性为类似 URL 这种缩略词转为  uRL ，结果“误伤”了这种场景。
<a name="EHlm6"></a>
#### 2.2.2 使用 cglib 的 BeanMap
cglib 依赖
```xml
<!-- https://mvnrepository.com/artifact/cglib/cglib -->
<dependency>
	<groupId>cglib</groupId>
	<artifactId>cglib-nodep</artifactId>
	<version>3.2.12</version>
</dependency>
```
代码示例：
```java
import net.sf.cglib.beans.BeanMap;
import third.fastjson.MockObject;

import java.util.Date;

public class BeanMapDemo {
	
	public static void main(String[] args) {
		
		MockObject mockObject = new MockObject();
		mockObject.setAInteger(1);
		mockObject.setALong(2L);
		mockObject.setADate(new Date());
		mockObject.setADouble(3.4D);
		mockObject.setParent(3L);
		
		
		BeanMap beanMapp = BeanMap.create(mockObject);
		
		System.out.println(beanMapp);
	}
}
```
结果展示：<br />![2022-07-23-18-43-32.659483800.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658573471699-d28306f2-4dd2-4e99-93a0-9bb9ff2f78b8.png#clientId=uf2fc60f7-48c3-4&from=ui&id=uOofW&originHeight=721&originWidth=940&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2716147&status=done&style=none&taskId=u383eac2f-61d2-4572-a227-f3b80fbc56a&title=)<br />可以发现类型对了，但是属性名依然不对。<br />关键代码：`net.sf.cglib.core.ReflectUtils#getBeanGetters`底层也会用到   `java.beans.Introspector#decapitalize`  所以属性名存在一样的问题就不足为奇了。
<a name="MKLtF"></a>
## 三、解决办法
<a name="mekM2"></a>
### **3.1 解决方案**
解决方案有很多，本文提供一个基于  dubbo的解决方案。<br />maven 依赖：
```xml
<!-- https://mvnrepository.com/artifact/org.apache.dubbo/dubbo -->
<dependency>
	<groupId>org.apache.dubbo</groupId>
	<artifactId>dubbo</artifactId>
	<version>3.0.9</version>
</dependency>
```
示例代码：
```java
import org.apache.dubbo.common.utils.PojoUtils;
import third.fastjson.MockObject;

import java.util.Date;

public class DubboPojoDemo {
    public static void main(String[] args) {
        MockObject mockObject = new MockObject();
        mockObject.setAInteger(1);
        mockObject.setALong(2L);
        mockObject.setADate(new Date());
        mockObject.setADouble(3.4D);
        mockObject.setParent(3L);

        Object generalize = PojoUtils.generalize(mockObject);

        System.out.println(generalize);
    }
}
```
调试效果：<br />![2022-07-23-18-43-32.703417500.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658573471790-7d00bb9f-8eec-4fc2-ae77-e4ac74b03398.png#clientId=uf2fc60f7-48c3-4&from=ui&id=DSt2t&originHeight=711&originWidth=1112&originalType=binary&ratio=1&rotation=0&showTitle=false&size=3168435&status=done&style=none&taskId=u97fb9c1a-e981-4c54-91d3-e9b0f9758ad&title=)Java Visualizer 效果：<br />![2022-07-23-18-43-32.926898000.jpg](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658573635240-a39d3834-60c3-4d3e-ab46-5ca788186846.jpeg#clientId=uf2fc60f7-48c3-4&from=ui&id=uc7acc424&originHeight=757&originWidth=1027&originalType=binary&ratio=1&rotation=0&showTitle=false&size=170731&status=done&style=none&taskId=ubfc03ecc-f8a2-47dc-826d-e6910735312&title=)
<a name="Apqyv"></a>
### **3.2 原理解析**
大家可以下载源码来简单研究下。<br />[https://github.com/apache/dubbo](https://github.com/apache/dubbo)<br />核心代码：org.apache.dubbo.common.utils.PojoUtils#generalize(java.lang.Object)
```java
public static Object generalize(Object pojo) {
	eturn generalize(pojo, new IdentityHashMap());
}
```
关键代码：
```java
// pojo 待转换的对象
// history 缓存 Map，提高性能
private static Object generalize(Object pojo, Map<Object, Object> history) {
	if (pojo == null) {
		return null;
	}

	// 枚举直接返回枚举名
	if (pojo instanceof Enum<?>) {
		return ((Enum<?>) pojo).name();
	}

	// 枚举数组，返回枚举名数组
	if (pojo.getClass().isArray() && Enum.class.isAssignableFrom(pojo.getClass().getComponentType())) {
		int len = Array.getLength(pojo);
		String[] values = new String[len];
		for (int i = 0; i < len; i++) {
			values[i] = ((Enum<?>) Array.get(pojo, i)).name();
		}
		return values;
	}

	// 基本类型返回 pojo 自身
	if (ReflectUtils.isPrimitives(pojo.getClass())) {
		return pojo;
	}

	// Class 返回 name
	if (pojo instanceof Class) {
		return ((Class) pojo).getName();
	}

	Object o = history.get(pojo);
	if (o != null) {
		return o;
	}
	history.put(pojo, pojo);

	// 数组类型，递归
	if (pojo.getClass().isArray()) {
		int len = Array.getLength(pojo);
		Object[] dest = new Object[len];
		history.put(pojo, dest);
		for (int i = 0; i < len; i++) {
			Object obj = Array.get(pojo, i);
			dest[i] = generalize(obj, history);
		}
		return dest;
	}
	// 集合类型递归
	if (pojo instanceof Collection<?>) {
		Collection<Object> src = (Collection<Object>) pojo;
		int len = src.size();
		Collection<Object> dest = (pojo instanceof List<?>) ? new ArrayList<Object>(len) : new HashSet<Object>(len);
		history.put(pojo, dest);
		for (Object obj : src) {
			dest.add(generalize(obj, history));
		}
		return dest;
	}
	// Map 类型，直接 对 key 和 value 处理
	if (pojo instanceof Map<?, ?>) {
		Map<Object, Object> src = (Map<Object, Object>) pojo;
		Map<Object, Object> dest = createMap(src);
		history.put(pojo, dest);
		for (Map.Entry<Object, Object> obj : src.entrySet()) {
			dest.put(generalize(obj.getKey(), history), generalize(obj.getValue(), history));
		}
		return dest;
	}
	Map<String, Object> map = new HashMap<String, Object>();
	history.put(pojo, map);

	// 开启生成 class 则写入 pojo 的class
	if (GENERIC_WITH_CLZ) {
		map.put("class", pojo.getClass().getName());
	}

	// 处理 get 方法 
	for (Method method : pojo.getClass().getMethods()) {
		if (ReflectUtils.isBeanPropertyReadMethod(method)) {
			ReflectUtils.makeAccessible(method);
			try {
				map.put(ReflectUtils.getPropertyNameFromBeanReadMethod(method), generalize(method.invoke(pojo), history));
			} catch (Exception e) {
				throw new RuntimeException(e.getMessage(), e);
			}
		}
	}
	// 处理公有属性
	for (Field field : pojo.getClass().getFields()) {
		if (ReflectUtils.isPublicInstanceField(field)) {
			try {
				Object fieldValue = field.get(pojo);
				// 对象已经解析过，直接从缓存里读提高性能
				if (history.containsKey(pojo)) {
					Object pojoGeneralizedValue = history.get(pojo);
					// 已经解析过该属性则跳过（如公有属性，且有 get 方法的情况）
					if (pojoGeneralizedValue instanceof Map
						&& ((Map) pojoGeneralizedValue).containsKey(field.getName())) {
						continue;
					}
				}
				if (fieldValue != null) {
					map.put(field.getName(), generalize(fieldValue, history));
				}
			} catch (Exception e) {
				throw new RuntimeException(e.getMessage(), e);
			}
		}
	}
	return map;
}
```
关键截图<br />![2022-07-23-18-43-33.134962600.jpg](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658573635283-a25f7a9f-e41e-4833-bab9-9705207b2e7b.jpeg#clientId=uf2fc60f7-48c3-4&from=ui&id=TP7eV&originHeight=732&originWidth=1072&originalType=binary&ratio=1&rotation=0&showTitle=false&size=292591&status=done&style=none&taskId=ud94e82f5-86be-492c-8923-cc62d2db65e&title=)<br />![2022-07-23-18-43-33.268232300.jpg](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658573668773-67e2f8c4-f039-4590-b38a-c87bf68a186e.jpeg#clientId=uf2fc60f7-48c3-4&from=ui&id=u67bda031&originHeight=738&originWidth=1342&originalType=binary&ratio=1&rotation=0&showTitle=false&size=339400&status=done&style=none&taskId=u0e7a6f3b-82bc-4722-9986-0c994b68e7e&title=)<br />org.apache.dubbo.common.utils.ReflectUtils#getPropertyNameFromBeanReadMethod
```java
public static String getPropertyNameFromBeanReadMethod(Method method) {
	if (isBeanPropertyReadMethod(method)) {
		// get 方法，则从 index =3 的字符小写 + 后面的字符串
		if (method.getName().startsWith("get")) {
			return method.getName().substring(3, 4).toLowerCase()
				+ method.getName().substring(4);
		}
		// is 开头方法， index =2 的字符小写 + 后面的字符串
		if (method.getName().startsWith("is")) {
			return method.getName().substring(2, 3).toLowerCase()
				+ method.getName().substring(3);
		}
	}
	return null;
}
```
因此， `getALong` 方法对应的属性名被解析为 `aLong`。<br />![2022-07-23-18-43-33.389131600.jpg](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1658573668775-ae3ec7d0-a789-4f26-9126-a0c92f565138.jpeg#clientId=uf2fc60f7-48c3-4&from=ui&id=Q9usB&originHeight=584&originWidth=1111&originalType=binary&ratio=1&rotation=0&showTitle=false&size=226969&status=done&style=none&taskId=ue1809c3b-af63-4511-8f8a-57492caead5&title=)同时，这么处理也会存在问题。如当属性名叫 URL 时，转为 Map 后 key 就会被解析成 uRL。<br />![2022-07-23-18-43-33.493388300.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1658573708584-314a6871-8422-4838-b365-a973a31ef54f.png#clientId=uf2fc60f7-48c3-4&from=ui&id=ubc0328d7&originHeight=691&originWidth=823&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2279221&status=done&style=none&taskId=u45fd0fbb-90c5-4357-96cd-483ccad3aa9&title=)<br />从这里看出，当属性名比较特殊时也很容易出问题，但 dubbo 这个工具类更符合预期。更多细节，大家可以根据 DEMO 自行调试学习。<br />如果想严格和属性保持一致，可以使用反射获取属性名和属性值，加缓存机制提升解析的效率。
<a name="pdH95"></a>
## 四、总结
Java Bean 转 Map 的坑很多，最常见的就是类型丢失和属性名解析错误的问题。<br />大家在使用  JSON 框架和 Java Bean 转 Map 的框架时要特别小心。<br />平时使用某些框架时，多写一些 DEMO 进行验证，多读源码，多调试，少趟坑。
