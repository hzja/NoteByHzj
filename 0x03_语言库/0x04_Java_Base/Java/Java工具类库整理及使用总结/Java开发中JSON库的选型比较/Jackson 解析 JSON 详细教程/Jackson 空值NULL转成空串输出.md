JavaJackson
<a name="EX50a"></a>
## 需求背景
原始输出的JSON
```json
{
  "name": "age",
  "label": "年龄",
  "type": "Integer",
  "defaultValue": null,
  "act": "InOut"
}
```
需要转换成
```json
{
  "name": "age",
  "label": "年龄",
  "type": "Integer",
  "defaultValue": "",
  "act": "InOut"
}
```
<a name="deElV"></a>
## 实现方法
增加转换器
```java
protected void writeObjectToJsonWithNull(HttpServletResponse resp,Object obj) throws ServletException, IOException{
    resp.setHeader("Access-Control-Allow-Origin", "*");
    resp.setContentType("text/json");
    resp.setCharacterEncoding("UTF-8");
    ObjectMapper mapper=new ObjectMapper();

    mapper.getSerializerProvider().setNullValueSerializer(
        new JsonSerializer<Object>() {
            @Override
            public void serialize(Object param, JsonGenerator jsonGenerator,
                                  SerializerProvider paramSerializerProvider) throws IOException {
                jsonGenerator.writeString("");
            }
        });
    mapper.setSerializationInclusion(Inclusion.ALWAYS);
    mapper.configure(SerializationConfig.Feature.WRITE_DATES_AS_TIMESTAMPS,false);
    mapper.setDateFormat(new SimpleDateFormat(Configure.getDateFormat()));
    OutputStream out = resp.getOutputStream();
    try {
        mapper.writeValue(out, obj);
    } finally {
        out.flush();
        out.close();
    }
}
```
