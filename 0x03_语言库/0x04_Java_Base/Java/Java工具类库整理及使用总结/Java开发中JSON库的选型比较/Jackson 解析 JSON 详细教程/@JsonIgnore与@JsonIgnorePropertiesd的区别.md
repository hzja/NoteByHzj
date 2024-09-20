<a name="9lRyO"></a>
### `@JsonIgnore`
1、在json序列化时将java bean中的一些属性忽略掉，序列化和反序列化都受影响；<br />2、使用方法：一般标记在属性或者方法上，返回的json数据即不包含该属性；<br />3、如果注解失效，可能是因为使用的是fastJson，尝试使用对应的注解来忽略字段，注解为`@JSONField(serialize = false)`，使用方法一样；
<a name="3FvkG"></a>
### `@JsonIgnoreProperties`
`@JsonIgnoreProperties`用在类上，`@JsonIgnore`在属性上，`@JsonIgnoreProperties(value = "{password}")`效果同`@JsonIgnore`一样。
