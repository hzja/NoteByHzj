IDEA 逆向生成实体类 Groovy JPA 
<a name="JRhaR"></a>
# 环境说明
> IDEA版本：2020.1.3
> MySQL版本：8.0.16

<a name="ZviG4"></a>
# 1、通过IDEA连接数据
<a name="QoImt"></a>
## 1.打开数据库连接面板
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595690232876-776b7fa6-3455-4cf6-85d5-db59a229d37c.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=660139&status=done&style=shadow&width=1280)<br />或<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595690291430-25b9ec79-5ea4-4ca7-999b-2657fa5b2a24.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=680941&status=done&style=shadow&width=1280)
<a name="VhYV1"></a>
## 2.选择MySQL数据库连接
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595690340461-9e811813-1905-4bf5-8385-3d8dd1f52c0e.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=711050&status=done&style=shadow&width=1280)
<a name="oR3rD"></a>
## 3.配置连接信息
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595690672434-9d9d9cbb-f074-4eea-8131-c813b4e11049.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=847338&status=done&style=shadow&width=1280)
<a name="gVHND"></a>
## 4.连接成功并读取到表结构信息
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595690752851-34a51006-a793-4a8f-8a3a-36ba7db10b47.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=584419&status=done&style=shadow&width=1280)
<a name="EFWDJ"></a>
# 2、选择表逆向生成POJO
<a name="bZ5x0"></a>
## 1.选择逆向生成的表
<a name="uqhOD"></a>
## 2.表名上右键: Scripted Extensions -> Generate POJOs.groovy
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595691059631-07a93c57-19c7-4b1c-b5e7-93c2445b939b.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=537711&status=done&style=shadow&width=1280)
<a name="klO4c"></a>
## 3.选择生成POJO的文件夹（Java的包）
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595691116605-b65967d5-a84e-4392-a0c7-b4f47d81f5f6.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=690709&status=done&style=shadow&width=1280)
<a name="8SZNH"></a>
## 4.修改生成的类文件的包名（逆向生成稍有问题，包错误）
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595691319689-d0d89566-a584-4370-9433-c88029c1a082.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=951643&status=done&style=shadow&width=1280)
<a name="6uxlA"></a>
# 3、使用Hibernate/JPA框架生成有注解的实体类
<a name="rQl8p"></a>
## 1.配置jpa框架
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595691603154-69c09f2e-8597-4104-b23f-c2f4d6c620b6.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=764523&status=done&style=shadow&width=1280)
<a name="yDJqp"></a>
## 2.**使用Persistence工具生成带注解的实体类**
**![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595691802671-bc384f33-3ef6-4c2b-b961-7c1f88640558.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=576676&status=done&style=shadow&width=1280)**<br />**![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595691886447-ad83fc2f-5744-4378-a943-ec35cc54aa20.png#align=left&display=inline&height=720&originHeight=2159&originWidth=3840&size=653672&status=done&style=shadow&width=1280)**
<a name="LZEd9"></a>
## 3.设置生成的配置信息
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595692092188-4486f8d0-e82f-4ebb-801f-22bb8fb62a7d.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=754832&status=done&style=shadow&width=1280)
<a name="gjFQH"></a>
## 4.生成带有JPA注解的Java Bean
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595692170914-5f040879-5df1-4b8d-8b9c-d86f78469238.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=821143&status=done&style=shadow&width=1280)
<a name="4hSAy"></a>
# 4、使用自己的Groovy生成带注解的实体类
> 上面介绍了两个使用IDEA生成实体类的方法, 各有优缺点 生成不带注解的实体类简单高效, 但缺点就是如果使用的是Jpa/Hibernate框架就不适合 生成带注解的实体类能生成注解实体类, 但缺点就是生成的实体类的注解可能不太符合项目使用的注解, 只能生成到当前项目包下, 面对一些复杂的数据库如Oracle会显示很多系统表而且表空间切换不太方便 Idea生成不带注解的实体类是通过Groovy脚本文件生成的, 找到这个文件, 通过修改这个文件让它生成带注解的实体类

<a name="zcfzO"></a>
## (1)使用自己的Groovy生成带注解的实体类的好处

1. 在Database工具栏里, 操作简单方便
2. 可以自己对脚本进行一些编辑, 使生成的实体类达到自己的定制需求
3. 对Oracle这样的数据库生成实体类时, 避免了系统表的影响, 可以自己选择表空间进行操作
4. 可以生成在电脑上的任何位置, 不受限制
<a name="62C2J"></a>
## (2),使用自己的Groovy生成带注解的实体类的不足

1. @author 作者名需要自己修改脚本
2. @Table(Schema="") 的表空间名不能读取, 需要自己修改脚本, 设定表空间名(mysql可以直接删除这个属性的定义)(IDEA的Persistence工具可以读取(Schema=""属性并生成, 但不知道它是怎么获取的)
3. @Id的主键生成方式不可知, 需要生成后自己确定(mysql用户可以直接修改脚本设定主键生成方式, 但Oracle用户需要生成实体类后再逐个确认)
<a name="3DLNs"></a>
## (3),使用自己的Groovy生成带注解的实体类可以继续完善的地方
1, 如果需要hashcode()和equals()方法, 也修改脚本, 使其自动生成
<a name="byTTW"></a>
## (4)操作步骤
<a name="JRii3"></a>
### ①新建一个自定义的Groovy脚本文件

1. Database工具栏里, 右键右键: Scripted Extensions - Go to Scripts Directory

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595692531843-13ce68fa-d9e4-4064-a887-1b2d4abeb606.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=513255&status=done&style=shadow&width=1280)

2. 在目录下新建一个GenerateToPOJOs.groovy

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595692931078-ad2f874e-6f0d-4570-91b6-6b92c260b37e.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=859274&status=done&style=shadow&width=1280)
<a name="6MUM6"></a>
### ②脚本内容
```groovy
import com.intellij.database.model.DasTable
import com.intellij.database.model.ObjectKind
import com.intellij.database.util.Case
import com.intellij.database.util.DasUtil

import java.text.SimpleDateFormat

/*
 * Available context bindings:
 *   SELECTION   Iterable<DasObject>
 *   PROJECT     project
 *   FILES       files helper
 */
packageName = ""
typeMapping = [
        (~/(?i)tinyint|smallint|mediumint/)      : "Integer",
        (~/(?i)int/)                             : "Long",
        (~/(?i)bool|bit/)                        : "Boolean",
        (~/(?i)float|double|decimal|real/)       : "Double",
        (~/(?i)datetime|timestamp|date|time/)    : "Date",
        (~/(?i)blob|binary|bfile|clob|raw|image/): "InputStream",
        (~/(?i)/)                                : "String"
]


FILES.chooseDirectoryAndSave("Choose directory", "Choose where to store generated files") { dir ->
    SELECTION.filter { it instanceof DasTable && it.getKind() == ObjectKind.TABLE }.each { generate(it, dir) }
}

def generate(table, dir) {
    def className = javaName(table.getName(), true)
    def fields = calcFields(table)
    packageName = getPackageName(dir)
    PrintWriter printWriter = new PrintWriter(new OutputStreamWriter(new FileOutputStream(new File(dir, className + ".java")), "UTF-8"))
    printWriter.withPrintWriter { out -> generate(out, className, fields, table) }

//    new File(dir, className + ".java").withPrintWriter { out -> generate(out, className, fields,table) }
}

// 获取包所在文件夹路径
def getPackageName(dir) {
    return dir.toString().replaceAll("\\\\", ".").replaceAll("/", ".").replaceAll("^.*src(\\.main\\.java\\.)?", "") + ";"
}

def generate(out, className, fields, table) {
    out.println "package $packageName"
    out.println ""
    out.println "import javax.persistence.Column;"
    out.println "import javax.persistence.Entity;"
    out.println "import javax.persistence.Table;"
    out.println "import javax.persistence.Id;"
    out.println "import javax.persistence.GeneratedValue;"
    out.println "import java.io.Serializable;"
    Set types = new HashSet()

    fields.each() {
        types.add(it.type)
    }

    if (types.contains("Date")) {
        out.println "import java.util.Date;"
    }

    if (types.contains("InputStream")) {
        out.println "import java.io.InputStream;"
    }
    out.println ""
    out.println "/**\n" +
            " * @Description  \n" +
            " * @Author Fcant\n" + //1. 修改为自己名字
            " * @Date " + new SimpleDateFormat("yyyy-MM-dd").format(new Date()) + " \n" +
            " */"
    out.println ""
    out.println "@Entity"
    out.println "@Table ( name =\"" + table.getName() + "\")" //2. schema = \"后面添加自己的表空间名称(mysql可以不添加, 不用这个schema属性也行)
    out.println "public class $className  implements Serializable {"
    out.println ""
    out.println genSerialID()
    fields.each() {
        out.println ""
        // 输出注释
        if (isNotEmpty(it.commoent)) {
            out.println "\t/**"
            out.println "\t * ${it.commoent.toString()}"
            out.println "\t */"
        }

        if ((it.annos+"").indexOf("[@Id]") >= 0) out.println "\t@Id"

        if (it.annos != "") out.println "   ${it.annos.replace("[@Id]", "")}"


        // 输出成员变量
        out.println "\tprivate ${it.type} ${it.name};"
    }

    // 输出get/set方法
    fields.each() {
        out.println ""
        out.println "\tpublic ${it.type} get${it.name.capitalize()}() {"
        out.println "\t\treturn this.${it.name};"
        out.println "\t}"
        out.println ""

        out.println "\tpublic void set${it.name.capitalize()}(${it.type} ${it.name}) {"
        out.println "\t\tthis.${it.name} = ${it.name};"
        out.println "\t}"
    }

    // 输出toString方法
    out.println ""
    out.println "\t@Override"
    out.println "\tpublic String toString() {"
    out.println "\t\treturn \"TpApiConfig{\" +"
    fields.each() {
        out.println "\t\t\t\t\"${it.name}='\" + ${it.name} + '\\'' +"
    }
    out.println "\t\t\t\t'}';"
    out.println "\t}"

    out.println ""
    out.println "}"
}

def calcFields(table) {
    DasUtil.getColumns(table).reduce([]) { fields, col ->
        def spec = Case.LOWER.apply(col.getDataType().getSpecification())

        def typeStr = typeMapping.find { p, t -> p.matcher(spec).find() }.value
        def comm = [
                colName : col.getName(),
                name    : javaName(col.getName(), false),
                type    : typeStr,
                commoent: col.getComment(),
                annos   : "\t@Column(name = \"" + col.getName() + "\" )"]
        if ("id".equals(Case.LOWER.apply(col.getName())))
            comm.annos += ["@Id"]
        fields += [comm]
    }
}

// 已经修改为使用javaName, 如果有需要可以在def className = javaName(table.getName(), true)中修改为javaClassName
// 处理类名（这里是因为我的表都是以t_命名的，所以需要处理去掉生成类名时的开头的T，
// 如果你不需要那么请查找用到了 javaClassName这个方法的地方修改为 javaName 即可）
def javaClassName(str, capitalize) {
    def s = com.intellij.psi.codeStyle.NameUtil.splitNameIntoWords(str)
            .collect { Case.LOWER.apply(it).capitalize() }
            .join("")
            .replaceAll(/[^\p{javaJavaIdentifierPart}[_]]/, "_")
    // 去除开头的T  http://developer.51cto.com/art/200906/129168.htm
    s = s[1..s.size() - 1]
    capitalize || s.length() == 1 ? s : Case.LOWER.apply(s[0]) + s[1..-1]
}

def javaName(str, capitalize) {
//    def s = str.split(/(?<=[^\p{IsLetter}])/).collect { Case.LOWER.apply(it).capitalize() }
//            .join("").replaceAll(/[^\p{javaJavaIdentifierPart}]/, "_")
//    capitalize || s.length() == 1? s : Case.LOWER.apply(s[0]) + s[1..-1]
    def s = com.intellij.psi.codeStyle.NameUtil.splitNameIntoWords(str)
            .collect { Case.LOWER.apply(it).capitalize() }
            .join("")
            .replaceAll(/[^\p{javaJavaIdentifierPart}[_]]/, "_")
    capitalize || s.length() == 1 ? s : Case.LOWER.apply(s[0]) + s[1..-1]
}

def isNotEmpty(content) {
    return content != null && content.toString().trim().length() > 0
}

static String changeStyle(String str, boolean toCamel) {
    if (!str || str.size() <= 1)
        return str

    if (toCamel) {
        String r = str.toLowerCase().split('_').collect { cc -> Case.LOWER.apply(cc).capitalize() }.join('')
        return r[0].toLowerCase() + r[1..-1]
    } else {
        str = str[0].toLowerCase() + str[1..-1]
        return str.collect { cc -> ((char) cc).isUpperCase() ? '_' + cc.toLowerCase() : cc }.join('')
    }
}

static String genSerialID() {
    return "\tprivate static final long serialVersionUID =  " + Math.abs(new Random().nextLong()) + "L;"
}
```
<a name="6omlE"></a>
### ③选择自己的脚本文件执行
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595693053599-648ab3ad-3703-4b45-80b0-be60e38a1439.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=859720&status=done&style=shadow&width=1280)
<a name="WTKZG"></a>
### ④成功生成自定义JavaBean
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1595693246024-2b522122-c9db-4434-8e2e-7547c48a7b54.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=892072&status=done&style=shadow&width=1280)
