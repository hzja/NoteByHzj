Java

1. 日志库
2. JSON解析库
3. 单元测试库
4. 通用库
5. HTTP 库
6. XML 解析库
7. Excel 阅读库
8. 字节码库
9. 数据库连接池库
10. 消息库
11. PDF 库
12. 日期和时间库
13. 集合库
14. 电子邮件 API
15. HTML 解析库
16. 密码库
17. 嵌入式 SQL 数据库库
18. JDBC 故障排除库
19. 序列化库
20. 网络库

---

优秀且经验丰富的 Java 开发人员的特点之一是对 API 的广泛了解，包括 JDK 和第三方库。如何使用现有的 API 进行开发，而不是为常见的东西编写新的代码。是提升开发效率必选之路。<br />一般来说，会为日常项目提供有用的库，包括 Log4j 等日志库、Jackson 等 JSON 解析库以及 JUnit 和 Mockito 等单元测试 API。如果需要在项目中使用它们，则可以在项目的类路径中包含这些库的 JAR 以开始使用它们，也可以使用Maven进行依赖管理。<br />**对 Java 程序员有用的开源库**<br />下面是收集的一些有用的第三方库，Java 开发人员可以在他们的应用程序中使用它们来完成很多有用的任务。为了使用这些库，Java 开发人员应该熟悉这一点，这就是本文的重点。如果有一个想法，那么可以研究该库并使用它。
<a name="LwlIW"></a>
## **1、日志库**
日志库非常常见，因为在每个项目中都需要它们。它们对于服务器端应用程序来说是最重要的，因为日志只放置在可以看到应用程序正在发生什么的地方。尽管 JDK 附带了自己的日志库，但仍有更好的替代方案可用，例如 Log4j、SLF4j 和 LogBack。<br />Java 开发人员应该熟悉日志库的优缺点，并知道为什么使用 SLF4j 比普通的 Log4j 更好。
<a name="qwjkj"></a>
## **2、JSON解析库**
在当今的 Web 服务和物联网世界中，JSON 已成为将信息从客户端传输到服务器的首选协议。它们已取代 XML，成为以独立于平台的方式传输信息的首选方式。<br />不幸的是，JDK 没有JSON 库。但是，有许多优秀的第三方库可以解析和创建 JSON 消息，例如 Jackson 和 Gson。<br />Java Web 开发人员应该至少熟悉这些库中的一个。
<a name="hTgwX"></a>
## **3、单元测试库**
单元测试是将普通开发人员与优秀开发人员区分开来的最重要的事情。程序员经常得到不编写单元测试的借口，但避免单元测试的最常见借口是缺乏流行单元测试库的经验和知识，包括 JUnit、Mockito 和 PowerMock。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676693248684-8662ffaa-34b2-46b4-bf76-0de3fb7fd949.png#averageHue=%234d4642&clientId=u75648218-3e5d-4&from=paste&id=u40c687b9&originHeight=344&originWidth=400&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2ca60fc9-6714-4c49-9de6-7b241a1b111&title=)
<a name="r4UDU"></a>
## **4、通用库**
Java 开发人员可以使用一些优秀的通用第三方库，例如 Apache Commons 和 Google Guava。在项目中包含这些库，因为它们简化了很多任务。<br />重新发明轮子是没有意义的。应该更喜欢使用久经考验的库，而不是时不时地编写自己的例程。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676693248667-3c7bda77-5300-48e4-b519-3a3851342ff7.png#averageHue=%23e6bcab&clientId=u75648218-3e5d-4&from=paste&id=uc1141c97&originHeight=288&originWidth=400&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucd6598af-a7d5-4562-8aed-73c32748b12&title=)<br />Java 开发人员最好熟悉 Google Guava 和 Apache Commons 库。
<a name="sZFRN"></a>
## **5、HTTP 库**
JDK 缺乏对 HTTP 的支持。虽然可以使用包中的类建立 HTTP 连接 java.net，但使用开源第三方库（如 Apache HttpClient 和 HttpCore）并不容易或无缝。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676693248730-64e23c2d-9410-4d33-94d2-10dc3a02b5cc.png#averageHue=%23ebeae4&clientId=u75648218-3e5d-4&from=paste&id=u84d04d7d&originHeight=343&originWidth=400&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8814bb37-f03a-4962-9e7c-842cc7d5b2e&title=)<br />尽管 JDK 9 带来了对 HTTP 2.0 的支持以及对 HTTP 的更好支持，但强烈建议所有 Java 开发人员熟悉流行的 HTTP 客户端库，包括 HttpClient 和 HttpCore。
<a name="XUk7x"></a>
## **6、XML 解析库**
有许多 XML 解析库，包括 Xerces、JAXB、JAXP、Dom4j 和 Xstream。Xerces2 是 Apache Xerces 系列中的下一代高性能、完全兼容的 XML 解析器。这个新版本的 Xerces 引入了 Xerces Native Interface (XNI)，这是一个用于构建解析器组件和配置的完整框架，它非常模块化且易于编程。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676693248728-426b66ec-40c8-438c-8bc3-85526e4eaf5e.png#averageHue=%2391cadf&clientId=u75648218-3e5d-4&from=paste&id=u189faf60&originHeight=302&originWidth=400&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u91fab0f3-2b00-496d-a60e-1cf6957dfbc&title=)<br />Apache Xerces2 解析器是 XNI 的参考实现，但其他解析器组件、配置和解析器可以使用 Xerces Native Interface 编写。Dom4j 是另一个用于 Java 应用程序的灵活 XML 框架。
<a name="Hakmq"></a>
## **7、Excel 阅读库**
所有现实世界的应用程序都必须以某种形式与 Microsoft Office 交互。许多应用程序需要提供在 Excel 中导出数据的功能，如果必须从 Java 应用程序中执行相同操作，则需要 Apache POI API。<br />这是一个非常丰富的库，允许 从 Java 程序读取和写入 XLS 文件。可以查看该链接以获取在核心 Java 应用程序中读取 Excel 文件的工作示例。
<a name="QE7V8"></a>
## **8、字节码库**
如果正在编写生成代码或与字节码交互的框架或库，那么需要一个字节码库。<br />它们允许读取和修改应用程序生成的字节码。Java 世界中一些流行的字节码库是 javassist 和 Cglib Nodep。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676693248734-674b954a-43e0-40f5-a4cc-4c138995ca6a.png#averageHue=%23f8f8f8&clientId=u75648218-3e5d-4&from=paste&id=u5f1843aa&originHeight=300&originWidth=400&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uefdd59da-c7e1-4096-b922-785d5d83d4e&title=)<br />Javassist（JAVA 编程助手）使 Java 字节码操作变得非常简单。它是一个用于在 Java 中编辑字节码的类库。ASM 是另一个有用的字节码编辑库。
<a name="ZfjxJ"></a>
## **9、数据库连接池库**
如果从 Java 应用程序与数据库进行交互，但不使用数据库连接池库，那么，会丢失一些东西。<br />由于在运行时创建数据库连接需要时间并且使请求处理速度变慢，因此始终建议使用数据库连接库。一些流行的是 Commons Pool 和 DBCP。<br />在 Web 应用程序中，它的 Web 服务器通常提供这些功能，但在核心 Java 应用程序中，需要将这些连接池库包含到类路径中才能使用数据库连接池。
<a name="a1YS3"></a>
## **10、消息库**
与日志记录和数据库连接类似，消息传递也是许多实际 Java 应用程序的共同特征。<br />Java 提供 JMS 或 Java 消息传递服务，它不是 JDK 的一部分。对于此组件，需要包含一个单独的 jms.jar<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676693249129-864b31e1-6999-48e1-9bd2-db0c25b6a40a.png#averageHue=%23f9f9f9&clientId=u75648218-3e5d-4&from=paste&id=ue42c835f&originHeight=344&originWidth=400&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u4dde58bc-fcc6-488b-8e58-49ab7ae99a9&title=)<br />同样，如果使用第三方消息传递协议，例如 Tibco RV，那么需要 tibrv.jar 在应用程序类路径中使用第三方 JAR 。
<a name="FDPVj"></a>
## **11、PDF 库**
与 Microsoft Excel 类似，PDF 库是另一种普遍存在的格式。如果需要在应用程序中支持 PDF 功能，例如 在 PDF 文件中导出数据，可以使用 iText 和 Apache FOP 库。<br />两者都提供有用的 PDF 相关功能，但 iText 更丰富更好。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676693249209-85275e70-e5d1-495a-9f11-f88b0ee4fa3c.png#averageHue=%23f7f2eb&clientId=u75648218-3e5d-4&from=paste&id=ubd053ed8&originHeight=199&originWidth=253&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u0f1de410-757e-4a12-b6f3-3797154bcc0&title=)
<a name="zlIQ3"></a>
## **12、日期和时间库**
在 Java 8 之前，JDK 的数据和时间库有很多缺陷，因为它们不是线程安全的、不可变的和容易出错的。许多 Java 开发人员依靠 JodaTime 来实现他们的日期和时间要求。<br />从 JDK 8 开始，没有理由使用 Joda，因为可以在 JDK 8 的新日期和时间 API中获得所有这些功能，但是如果使用的是较旧的 Java 版本，那么 JodaTime 是一个值得学习的库。
<a name="Sd1OT"></a>
## **13、集合库**
尽管 JDK 拥有丰富的集合库，但也有一些第三方库提供了更多选项，例如 Apache Commons 集合、Goldman Sachs 集合、Google 集合和 Trove。<br />Trove 库特别有用，因为它为 Java 提供了高速的常规和原始集合。<br />![](https://cdn.nlark.com/yuque/0/2023/jpeg/396745/1676693249192-41250a19-1405-4b61-b85d-b99f65deed77.jpeg#averageHue=%23f5f5f5&clientId=u75648218-3e5d-4&from=paste&id=u0dfeefb2&originHeight=234&originWidth=396&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf2a678a4-200f-43b6-b3ef-eaa4cb87aab&title=)<br />FastUtil 是另一个类似的 API。它通过提供特定类型的映射、集合、列表和优先级队列来扩展 Java 集合框架，这些映射、集合、列表和优先级队列具有较小的内存占用、快速访问和插入；它还提供大（64 位）数组、集合和列表，以及用于二进制和文本文件的快速、实用的 I/O 类。
<a name="BIT2K"></a>
## **14、电子邮件 API**
javax.mail 和 Apache Commons Email 都提供了用于从 Java 发送电子邮件的 API 。它建立在 JavaMail API 之上，旨在简化它。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676693249205-60fc8262-eecc-40ba-adba-1f2d03cea7f4.png#averageHue=%23f9f9f6&clientId=u75648218-3e5d-4&from=paste&id=u09a80d3e&originHeight=166&originWidth=400&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uba2e4a94-ae05-4c4b-a1c5-10af55722ee&title=)
<a name="MZRcQ"></a>
## **15、HTML 解析库**
与JSON和XML类似，HMTL 是许多人必须处理的另一种常见格式。值得庆幸的是，有 JSoup，它极大地简化了在 Java 应用程序中使用 HTML。<br />可以使用JSoup不仅解析 HTML，还可以创建 HTML 文档<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676693249468-e5c5b36e-cba4-4ab1-b210-bf6491f0532b.png#averageHue=%234ba872&clientId=u75648218-3e5d-4&from=paste&id=u0e138058&originHeight=284&originWidth=400&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u231aa87d-f95f-4b9d-9814-8791522c116&title=)<br />它提供了一个非常方便的 API 用于提取和操作数据，使用最好的DOM、CSS 和类似 jquery 的方法。JSoup 实现了 WHATWG HTML5 规范并将HTML解析为与现代浏览器相同的 DOM。
<a name="GvgXE"></a>
## **16、密码库**
Apache Commons Codec 包包含各种格式的简单编码器和解码器，例如Base64和 Hexadecimal。<br />除了这些广泛使用的编码器和解码器之外，编解码器包还维护了一组语音编码实用程序。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676693249581-feefb66a-7450-4741-bc9c-91c899409344.png#averageHue=%23edecec&clientId=u75648218-3e5d-4&from=paste&id=udc681442&originHeight=219&originWidth=400&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u3d2879e0-c391-4baf-ba5c-c51bd6f3aec&title=)<br />图片
<a name="rKPbK"></a>
## **17、嵌入式 SQL 数据库库**
像 H2 这样的内存数据库，可以将它嵌入到 Java 应用程序中。它们非常适合测试 SQL 脚本和运行需要数据库的单元测试。但是，H2 不是唯一的 DB，还可以选择 Apache Derby 和 HSQL。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676693249560-9382078d-0fb1-4c4a-be7b-248fdfe762d9.png#averageHue=%23f4f3f2&clientId=u75648218-3e5d-4&from=paste&id=ub4e76b4b&originHeight=202&originWidth=400&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uf5a684cf-54fe-4fa9-bcae-5b6ff00cbd8&title=)
<a name="seWvW"></a>
## **18、JDBC 故障排除库**
有一些很好的 JDBC 扩展库可以让调试更容易，比如 P6spy。<br />这是一个库，可以无缝拦截和记录数据库数据，而无需更改应用程序的代码。可以使用它们来记录 SQL 查询及其时间。<br />例如，如果在代码中使用`PreparedStatment`和`CallableStatement`，这些库可以记录带有参数的准确调用以及执行所需的时间。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676693249748-7b2ec234-48e2-4c3a-9e07-0fb8ebc00c8c.png#averageHue=%23faede3&clientId=u75648218-3e5d-4&from=paste&id=u9b5de01a&originHeight=253&originWidth=400&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uec4b9755-9819-4fd6-85a7-731301c879f&title=)
<a name="wd6ex"></a>
## **19、序列化库**
Google 协议缓冲区是一种以高效且可扩展的格式对结构化数据进行编码的方法。它是Java 序列化的更丰富和更好的替代方案。强烈建议有经验的 Java 开发人员学习 Google Protobuf。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676693249728-5bf96ddc-be10-4226-8438-19d7b43bf9b5.png#averageHue=%23dcd7d2&clientId=u75648218-3e5d-4&from=paste&id=u440624c5&originHeight=242&originWidth=400&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud97e5552-be4a-4757-9e7c-c11a501d04f&title=)
<a name="bvGbY"></a>
## **20、网络库**
一些有用的网络库是 Netty 和 Apache MINA。如果正在编写需要执行低级网络任务的应用程序，请考虑使用这些库。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1676693249885-8577185f-88f8-40d2-9ae6-2c885418fd8d.png#averageHue=%23f1efe3&clientId=u75648218-3e5d-4&from=paste&id=ud5190dde&originHeight=227&originWidth=400&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6a37511e-3cfc-4a85-9cae-089f88a84fd&title=)
