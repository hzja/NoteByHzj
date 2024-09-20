Java<br />介绍一个非常nice的word导出工具包：poi-tl。<br />这款工具包，可以说是用过的最简单、最好用、最丝滑的word导出工具。
<a name="AGrbp"></a>
### **poi-tl简介**
poi-tl是在apache-poi的基础上进行了封装的一款工具，比原生的apache-poi好用很多。<br />poi-tl最大的特点是：所见即所得。<br />word模板是什么样子、设置了什么格式，那么根据模板导出的文档就是什么格式。<br />同时poi-tl的社区也还是比较活跃的，在Github上也积累了2.7k的Star。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661173343484-4678b2b8-9157-455c-a1c8-aa38190fa93f.png#clientId=ue3cf51ab-26d8-4&from=paste&id=ufde64b66&originHeight=481&originWidth=870&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ucfe8a36d-a41e-49ab-a967-45bdfca664d&title=)<br />历史更新版本：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661173343425-4c7f0e45-a0d3-4fe0-a6a1-6dbd8bd9fa8e.png#clientId=ue3cf51ab-26d8-4&from=paste&id=ub49b500e&originHeight=481&originWidth=915&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u11f176fa-5147-41f0-b726-148d5241801&title=)<br />下面给大家详细介绍一下poi-tl的功能和用法。<br />本文使用的poi-tl的版本为1.10.0。
```xml
<dependency>
  <groupId>com.deepoove</groupId>
  <artifactId>poi-tl</artifactId>
  <version>1.10.0</version>
</dependency>
```
<a name="zuEtd"></a>
### **导出文本**
首先，新建一个word文档，然后用 `{{变量名}}`的格式定义好模板。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661173343421-e36cf1c6-4202-45df-9b87-4f0df4d7a9e0.png#clientId=ue3cf51ab-26d8-4&from=paste&id=ufbea7ae5&originHeight=148&originWidth=701&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=ue149a497-7a6e-4838-ad49-4469adf6829&title=)<br />然后，在代码中根据变量名，填入需要填充的值。
```java
public static void main(String[] args) {
    try{
        //设置参数
        Map<String,Object> param = new HashMap<>();
        param.put("paperName","期末试卷");
        //读取模板文件
        InputStream inputStream = new FileInputStream("d:\\template\\templateExportWord.docx");
        //根据参数，生成word文档
        XWPFTemplate template = XWPFTemplate.compile(inputStream).render(param);
        template.writeAndClose(new FileOutputStream("d:\\111.doc"));
    }catch (Exception e){
        System.out.println(e);
    }
}
```
最后，执行完成后，在指定文件夹下就生成了一份word文件。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661173343461-e028dfcd-dda1-4676-bb86-dec700b423ee.png#clientId=ue3cf51ab-26d8-4&from=paste&id=ue60c2254&originHeight=42&originWidth=149&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u0d9bd039-5670-4deb-a4eb-1b629212ba6&title=)<br />可以看到，文件内显示的内容，就是代码里输入的内容。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661173343471-3e516e77-199e-43b2-96c9-f8c906f0de6c.png#clientId=ue3cf51ab-26d8-4&from=paste&id=uebff06c4&originHeight=237&originWidth=607&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u50d3e3a7-67e5-44fb-8391-a2698082f29&title=)<br />导出文本是最基本的功能，下面给大家介绍几个高阶用法。
<a name="fAe73"></a>
### **导出列表**
导出列表，类似于前端标签`<ul>`和`<li>`标签，poi-tl支持在指定的内容前，自动添加序号。<br />比如自动添加：

- **1、2、3...**
- **A、B、C...**
- **i 、ⅱ、 ⅲ...**

下面给大家演示一个例子。<br />首先，定制导出模板。<br />使用列表导出功能，模板标签采用`{{*变量名}}`来定制。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661173343753-73e28ec1-e9a4-4fd1-bf1c-0cd4440f6f5d.png#clientId=ue3cf51ab-26d8-4&from=paste&id=u065edb65&originHeight=57&originWidth=251&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2c461543-8627-4f84-ade8-6a15b5c25df&title=)<br />根据变量名的设置，相比大家也能猜到，这次放入的参数，应该是一个数组之类的。<br />代码如下：
```java
public static void main(String[] args) {
    try{
        //填充的数据
        List<String> options = new ArrayList<>();
        options.add("光之子");
        options.add("斗罗大陆");
        options.add("大奉打更人");

        //设置列表的格式为 A B C
        NumberingRenderData optionData = Numberings.of(options.toArray(new String[0])).create();
        optionData.setFormats(Arrays.asList(new NumberingFormat[]{NumberingFormat.UPPER_LETTER}));

        //设置参数
        Map<String,Object> param = new HashMap<>();
        param.put("options",optionData);

        //根据参数，生成word文档
        InputStream inputStream = new FileInputStream("d:\\template\\templateExportWord.docx");
        XWPFTemplate template = XWPFTemplate.compile(inputStream).render(param);
        template.writeAndClose(new FileOutputStream("d:\\111.doc"));
    }catch (Exception e){
        System.out.println(e);
    }
}
```
可以看到，列表相比文本，多了一个`NumberingRenderData`的设置，这里是为了指定列表前显示的序号类型。<br />是显示1、2、3还是a、b、c，由这个多出来的设置决定。<br />这里设置的类型是`NumberingFormat.UPPER_LETTER`，所以序号类型是大写的字母。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661173343795-dceaedee-303d-4d49-814f-d07696c42afc.png#clientId=ue3cf51ab-26d8-4&from=paste&id=uc7663af8&originHeight=115&originWidth=212&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=udf239642-a9fc-4dc2-9947-a25a991b8d7&title=)
<a name="BIN4j"></a>
### **导出区块对**
上面的导出文本、导出列表，大家应该很容易理解。<br />而一看区块对三个字，是不是稍微有点蒙了？<br />其实用简单的语言来描述一下区块对：区块对就是一些有规律的、格式重复的内容模块。<br />比如，咱们的试卷，第一题、第二题... 第n题，就是有规律的内容。<br />区块对模板的设置方法是用{{?变量名}}中间内容{{/变量名}}，在中间内容这部分，可以插入列表内容、文本内容、图片内容等等。<br />接下来结合着上面的导出文本、导出列表的功能，再加上导出区块对，总共三种类型的格式。<br />给大家演示一个导出试卷的功能，这个例子是根据自己项目中的功能简化而来的，对于大家理解poi-tl的使用会有比较大的帮助。<br />首先，定义一个基础模板。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661173343830-c4fd7b6a-4f5f-425a-86c6-887a6bcf916d.png#clientId=ue3cf51ab-26d8-4&from=paste&id=uaa0be2aa&originHeight=398&originWidth=794&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u3a930798-f228-4ea3-b16b-553cc1b77dd&title=)<br />来看一下导出效果。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661173343842-5f2b66ac-d9f5-4290-b68c-84c18e1aad8c.png#clientId=ue3cf51ab-26d8-4&from=paste&id=ua2dcc720&originHeight=502&originWidth=763&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u6022c0d8-efd0-4c8a-a8d1-879be6988cb&title=)<br />导出代码全部贴出来。
```java
public static void main(String[] args) {
    try{
        //所有试题数据
        List<Map<String,Object>> allQuestionList = new ArrayList();

        //第一题
        Map<String,Object> questionMap = new HashMap<>();//试题添加进数据结构
        questionMap.put("titleFirst","一、单选题");
        questionMap.put("title","1.以下网络小说不是唐家三少作品的是哪一部？");
        //填充的数据
        List<String> options = new ArrayList<>();
        options.add("光之子");
        options.add("斗罗大陆");
        options.add("大奉打更人");
        //设置列表的格式为 A B C
        NumberingRenderData optionData = Numberings.of(options.toArray(new String[0])).create();
        optionData.setFormats(Arrays.asList(new NumberingFormat[]{NumberingFormat.UPPER_LETTER}));
        questionMap.put("options",optionData);
        //加入试题集合
        allQuestionList.add(questionMap);

        //第二题
        Map<String,Object> questionMap2 = new HashMap<>();//试题添加进数据结构
        questionMap2.put("titleFirst","二、多选题");
        questionMap2.put("title","1.以下网络小说是唐家三少作品的有哪些？");
        //填充的数据
        List<String> options2 = new ArrayList<>();
        options2.add("光之子");
        options2.add("斗罗大陆");
        options2.add("大奉打更人");
        //设置列表的格式为 A B C
        NumberingRenderData optionData2 = Numberings.of(options.toArray(new String[0])).create();
        optionData2.setFormats(Arrays.asList(new NumberingFormat[]{NumberingFormat.UPPER_LETTER}));
        questionMap2.put("options",optionData2);
        //加入试题集合
        allQuestionList.add(questionMap2);

        //组装所有数据
        Map<String,Object> param = new HashMap<>();
        param.put("paperName","资深网络小说读者测试题");
        param.put("paperInfo","(总分：100，总题数：100)");
        param.put("papers",allQuestionList);

        //执行导出操作
        InputStream inputStream = new FileInputStream("d:\\template\\templateExportWord.docx");
        XWPFTemplate template = XWPFTemplate.compile(inputStream).render(param);
        template.writeAndClose(new FileOutputStream("d:\\111.doc"));
    }catch (Exception e){
        System.out.println(e);
    }
}
```
代码稍微多了一些，实际项目中，肯定需要借助循环来完成才行。<br />看看导出的效果，是不是感觉导出的内容单调了一些？<br />没关系，poi-tl**所见即所得**的特性，只需要修改模板，就能轻松实现导出内容的格式定制。<br />这里简单调整一下模板的文字颜色，给大家演示一下。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661173343864-a0d15ee1-c08a-419e-9233-06a97092c97a.png#clientId=ue3cf51ab-26d8-4&from=paste&id=ub3033773&originHeight=422&originWidth=921&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u802bfd49-3a11-4524-a521-b7b90af1d44&title=)<br />根据上面的模板，导出的效果就不是单调的白纸黑字了。<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661173344053-63b8fa35-6e0f-410d-a33d-e3b77d7d640d.png#clientId=ue3cf51ab-26d8-4&from=paste&id=u8c33eff7&originHeight=506&originWidth=742&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u1d620921-2437-4480-8bbe-781e6b2c161&title=)
<a name="fSk4G"></a>
### **更多高级功能**
除开上面的示例，poi-tl还支持许多其他类型的导出：导出图片、导出表格... ...<br />同样也只需要定制模板，再辅助一些简单的API调用就能完成。<br />更多功能如下所示：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661173344195-a6e56ff4-eb7b-4b17-b4e7-68be4735338f.png#clientId=ue3cf51ab-26d8-4&from=paste&id=ud99f9956&originHeight=478&originWidth=1013&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=u2d050d17-c1f0-4b47-be5c-14fcee2b99e&title=)
<a name="lqUCO"></a>
### **小贴士**
由于poi-tl是依赖于apache-poi来进行开发的，所以在项目中引入时，需要注意使用的 apache-poi版本和poi-tl是否兼容。<br />如果不兼容，控制台会报出详细的错误信息，可尝试调整二者的版本来解决。
<a name="Cvn1k"></a>
### **项目地址**
poi-tl官网：[http://deepoove.com/poi-tl/](http://deepoove.com/poi-tl/)<br />github地址：[https://github.com/Sayi/poi-tl](https://github.com/Sayi/poi-tl)
