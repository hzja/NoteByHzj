<a name="NFmcx"></a>
## 介绍
Webmagic 是一个非常酷的开源Java垂直爬虫框架！它的目标就是让爬虫开发变得更简单，让你能够专注于开发各种好玩的功能。<br />Webmagic 的核心非常简洁，但它覆盖了爬虫的整个流程，真的非常适合用来学习爬虫开发哦。
<a name="eRzhS"></a>
## 优点

- 采用的是完全模块化的设计，可以根据自己的需求随时扩展它的功能。
- 简单易学，包含了爬虫的全部流程，非常适合新手入门。
- 提供了丰富的抽取页面API，可以轻松地提取所需要的数据啦！
- 不需要额外的配置，只需要通过POJO+注解的形式就能实现一个自己的爬虫，真是太方便了！
- 支持多线程和分布式爬取，还能爬取js动态渲染的页面，这可是很多其他爬虫做不到的哦
- 无框架依赖，可以灵活地嵌入到项目中，实用！
<a name="GXvqS"></a>
## 使用说明
<a name="XgcVh"></a>
### 1、Maven依赖引入
```xml
<dependency>
    <groupId>us.codecraft</groupId>
    <artifactId>webmagic-core</artifactId>
    <version>0.7.5</version>
</dependency>
<dependency>
    <groupId>us.codecraft</groupId>
    <artifactId>webmagic-extension</artifactId>
    <version>0.7.5</version>
</dependency>
```
<a name="vhSCJ"></a>
### 2、项目结构说明
核心模块：**「webmagic-core」**<br />只包含爬虫基本模块和基本抽取器。<br />扩展模块：**「webmagic-extension」**<br />提供一些更方便的编写爬虫的工具。包括注解格式定义爬虫、JSON、分布式等支持。<br />扩展包：**「webmagic-saxon」**<br />webmagic与Saxon结合的模块。webmagic依赖Saxon来进行XPath2.0语法解析支持。<br />扩展包：**「webmagic-selenium」**<br />webmagic与Selenium结合的模块。webmagic依赖Selenium进行动态页面的抓取。
<a name="PQxVC"></a>
### 3、代码编写样例
基于一个POJO增加注解即可完成一个爬虫。以下是抓取test博客的一段代码。Spider是爬虫的入口类。<br />Pipeline是结果输出和持久化的接口，这里ConsolePipeline表示结果输出到控制台。
```java
@TargetUrl("http://my.test.net/flashsword/blog/\\d+")
public class OschinaBlog {

    @ExtractBy("//title")
    private String title;

    @ExtractBy(value = "div.BlogContent",type = ExtractBy.Type.Css)
    private String content;

    @ExtractBy(value = "//div[@class='BlogTags']/a/text()", multi = true)
    private List<String> tags;

    public static void main(String[] args) {
        OOSpider.create(
          Site.me(),
      new ConsolePageModelPipeline(), OschinaBlog.class).addUrl("http://my.test.net/flashsword/blog").run();
    }
}
```
<a name="FlRGh"></a>
## 写在最后
在这个信息爆炸的时代，如何快速获取高质量的资源成为了一个重要的问题。掌握爬虫技术，举足轻重，  真的是太神奇了！它就像一个即插即用的爬虫神器，不需要繁琐的配置，方便得让人想跳起舞来！而且它还特地为二次开发量身定制，提供了超简洁的API，只需要简单地挥挥手，少量代码就能生成一个爬虫，当然了，要注意不用将爬虫应用到任何可能会违反法律规定和道德约束的工作中。<br />**项目地址：**[https://gitee.com/flashsword20/webmagic](https://gitee.com/flashsword20/webmagic)
