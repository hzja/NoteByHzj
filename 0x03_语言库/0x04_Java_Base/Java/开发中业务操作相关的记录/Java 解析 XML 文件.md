Java
<a name="aeR4x"></a>
## XML 基础
XML 是一个带有开始和结束标记的分层标记语言，每一个标记（标签）内可以包含零个或更多数据。下面是一个 XML 的简单示例片段：
```xml
<xml>
  <node>
    <element>Penguin</element>
  </node>
</xml>
```
在这个 自我描述的(self-descriptive) 例子中，XML 解析器使用了以下几个概念：<br />◈ 文档(Document)：`<xml>` 标签标志着一个 文档 的开始，`</xml>` 标签标志着这个文档的结束。<br />◈ 节点(Node)：`<node>` 标签代表了一个 节点。<br />◈ 元素(Element)：`<element>Penguin</element>` 中，从开头的 `<` 到最后的 `>` 表示了一个 元素。<br />◈ 内容(Content)：在 `<element>` 元素里，字符串 Penguin 就是 内容。<br />只要了解了以上几个概念，就可以开始编写、解析 XML 文件了。
<a name="UKnn4"></a>
## 创建一个示例配置文件
要学习如何解析 XML 文件，只需要一个极简的示例文件就够了。假设现在有一个配置文件，里面保存的是关于一个图形界面窗口的属性：
```xml
<xml>
  <window>
    <theme>Dark</theme>
    <fullscreen>0</fullscreen>
    <icons>Tango</icons>
</window>
</xml>
```
创建一个名为 ~/.config/DemoXMLParser 的目录：
```bash
$ mkdir ~/.config/DemoXMLParser
```
在 Linux 中，~/.config 目录是存放配置文件的默认位置，这是在 自由桌面工作组🔗 specifications.freedesktop.org 的规范中定义的。如果正在使用一个不遵守 自由桌面工作组(Freedesktop)标准的操作系统，仍然可以使用这个目录，只不过需要自己创建这些目录了。<br />复制 XML 的示例配置文件，粘贴并保存为 ~/.config/DemoXMLParser/myconfig.xml 文件。
<a name="a5wcV"></a>
## 使用 Java 解析 XML
刚开始先不要太关注依赖导入和异常捕获这些，可以先尝试用 javax 和 java.io 包里的标准 Java 扩展来实例化一个解析器。如果使用了 IDE，它会提示导入合适的依赖。
```java
Path configPath = Paths.get(System.getProperty("user.home"), ".config", "DemoXMLParser");
File configFile = new File(configPath.toString(), "myconfig.xml");
DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
DocumentBuilder builder = null;
builder = factory.newDocumentBuilder();
Document doc = null;
doc = builder.parse(configFile);
doc.getDocumentElement().normalize();
```
这段示例代码使用了 java.nio.Paths 类来找到用户的主目录，然后在拼接上默认配置文件的路径。接着，它用 java.io.File 类来把配置文件定义为一个 File 对象。<br />紧接着，它使用了 `javax.xml.parsers.DocumentBuilder` 和 `javax.xml.parsers.DocumentBuilderFactory` 这两个类来创建一个内部的文档构造器，这样 Java 程序就可以导入并解析 XML 数据了。<br />最后，Java 创建一个叫 doc 的文档对象，并且把 configFile 文件加载到这个对象里。通过使用 org.w3c.dom 包，它读取并规范化了 XML 数据。<br />基本上就是这样啦。理论上来讲，已经完成了数据解析的工作。可是，如果不能够访问数据的话，数据解析也没有多少用处。所以，再来写一些查询，从配置中读取重要的属性值。
<a name="w4lRl"></a>
## 使用 Java 访问 XML 的值
从已经读取的 XML 文档中获取数据，其实就是要先找到一个特定的节点，然后遍历它包含的所有元素。通常会使用多个循环语句来遍历节点中的元素，但是为了保持代码可读性，会尽可能少地使用循环语句：
```java
NodeList nodes = doc.getElementsByTagName("window");
for (int i = 0; i < nodes.getLength(); i++) {
	Node mynode = nodes.item(i);
	System.out.println("Property = " + mynode.getNodeName());
	
	if (mynode.getNodeType() == Node.ELEMENT_NODE) {
		Element myelement = (Element) mynode;
		
		System.out.println("Theme = " + myelement.getElementsByTagName("theme").item(0).getTextContent());
		System.out.println("Fullscreen = " + myelement.getElementsByTagName("fullscreen").item(0).getTextContent());
		System.out.println("Icon set = " + myelement.getElementsByTagName("icons").item(0).getTextContent());
	}
}
```
这段示例代码使用了 org.w3c.dom.NodeList 类，创建了一个名为 nodes 的 NodeList 对象。这个对象包含了所有名字匹配字符串 window 的子节点，实际上这样的节点只有一个，因为本文的示例配置文件中只配置了一个。<br />紧接着，它使用了一个 for 循环来遍历 nodes 列表。具体过程是：根据节点出现的顺序逐个取出，然后交给一个 if-then 子句处理。这个 if-then 子句创建了一个名为 myelement 的 Element 对象，其中包含了当前节点下的所有元素。可以使用例如 `getChildNodes` 和 `getElementById` 方法来查询这些元素，项目中还 记录了 www.w3.org 其他查询方法。<br />在这个示例中，每个元素就是配置的键。而配置的值储存在元素的内容中，可以使用 `.getTextContent` 方法来提取出配置的值。<br />在 IDE 中运行代码（或者运行编译后的二进制文件）：
```java
$ java ./DemoXMLParser.java
Property = window
Theme = Dark
Fullscreen = 0
Icon set = Tango
```
下面是完整的代码示例：
```java
package myConfigParser;
import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
public class ConfigParser {
        public static void main(String[] args) {
                Path configPath = Paths.get(System.getProperty("user.home"), ".config", "DemoXMLParser");
                File configFile = new File(configPath.toString(), "myconfig.xml");
                DocumentBuilderFactory factory =
                DocumentBuilderFactory.newInstance();
                DocumentBuilder builder = null;
               
                try {
                        builder = factory.newDocumentBuilder();
                } catch (ParserConfigurationException e) {
                        e.printStackTrace();
                }
       
                Document doc = null;
       
                try {
                        doc = builder.parse(configFile);
                } catch (SAXException e) {
                        e.printStackTrace();
                } catch (IOException e) {
                        e.printStackTrace();
                }
        doc.getDocumentElement().normalize();
       
        NodeList nodes = doc.getElementsByTagName("window");
        for (int i = 0; i < nodes.getLength(); i++) {
           Node mynode = nodes.item(i);
           System.out.println("Property = " + mynode.getNodeName());
           
           if (mynode.getNodeType() == Node.ELEMENT_NODE) {
               Element myelement = (Element) mynode;
               System.out.println("Theme = " + myelement.getElementsByTagName("theme").item(0).getTextContent());
               System.out.println("Fullscreen = " + myelement.getElementsByTagName("fullscreen").item(0).getTextContent());
               System.out.println("Icon set = " + myelement.getElementsByTagName("icons").item(0).getTextContent());
           } // close if
        } // close for
    } // close method
} //close class
```
<a name="HkkCN"></a>
## 使用 Java 更新 XML
用户时不时地会改变某个偏好项，这时候 org.w3c.dom 库就可以更新某个 XML 元素的内容。只需要选择这个 XML 元素，就像读取它时那样。不过，此时不再使用 `.getTextContent` 方法，而是使用 `.setTextContent` 方法。
```java
updatePref = myelement.getElementsByTagName("fullscreen").item(0);
updatePref.setTextContent("1");
System.out.println("Updated fullscreen to " + myelement.getElementsByTagName("fullscreen").item(0).getTextContent());  
```
这么做会改变应用程序内存中的 XML 文档，但是还没有把数据写回到磁盘上。配合使用 javax 和 w3c 库，就可以把读取到的 XML 内容写回到配置文件中。
```java
TransformerFactory transformerFactory = TransformerFactory.newInstance();
Transformer xtransform;
xtransform = transformerFactory.newTransformer();
DOMSource mydom = new DOMSource(doc);
StreamResult streamResult = new StreamResult(configFile);
xtransform.transform(mydom, streamResult);
```
这么做会没有警告地写入转换后的数据，并覆盖掉之前的配置。<br />下面是完整的代码，包括更新 XML 的操作：
```java
package myConfigParser;
import java.io.File;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
public class ConfigParser {
        public static void main(String[] args) {
                Path configPath = Paths.get(System.getProperty("user.home"), ".config", "DemoXMLParser");
                File configFile = new File(configPath.toString(), "myconfig.xml");
                DocumentBuilderFactory factory =
                DocumentBuilderFactory.newInstance();
                DocumentBuilder builder = null;
               
                try {
                        builder = factory.newDocumentBuilder();
                } catch (ParserConfigurationException e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                }
       
                Document doc = null;
       
                try {
                        doc = builder.parse(configFile);
                } catch (SAXException e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                } catch (IOException e) {
                        // TODO Auto-generated catch block
                        e.printStackTrace();
                }
        doc.getDocumentElement().normalize();
        Node updatePref = null;
//        NodeList nodes = doc.getChildNodes();
        NodeList nodes = doc.getElementsByTagName("window");
        for (int i = 0; i < nodes.getLength(); i++) {
           Node mynode = nodes.item(i);
           System.out.println("Property = " + mynode.getNodeName());
           
           if (mynode.getNodeType() == Node.ELEMENT_NODE) {
               Element myelement = (Element) mynode;
               System.out.println("Theme = " + myelement.getElementsByTagName("theme").item(0).getTextContent());
               System.out.println("Fullscreen = " + myelement.getElementsByTagName("fullscreen").item(0).getTextContent());
               System.out.println("Icon set = " + myelement.getElementsByTagName("icons").item(0).getTextContent());
               updatePref = myelement.getElementsByTagName("fullscreen").item(0);
               updatePref.setTextContent("2");
               System.out.println("Updated fullscreen to " + myelement.getElementsByTagName("fullscreen").item(0).getTextContent());          
           } // close if
           
        }// close for
        // write DOM back to the file
        TransformerFactory transformerFactory = TransformerFactory.newInstance();
        Transformer xtransform;
        DOMSource mydom = new DOMSource(doc);
        StreamResult streamResult = new StreamResult(configFile);
        try {
                xtransform = transformerFactory.newTransformer();
                xtransform.transform(mydom, streamResult);
        } catch (TransformerException e) {
                e.printStackTrace();
        }
                       
    } // close method
} //close class
```
