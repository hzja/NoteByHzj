<a name="htQnt"></a>
## 前言
保留了很久的笔记，今天更新发布一下，不在乙方了，公司也没net研发的业务，这部分学习笔记到此为止了。
<a name="Mmt23"></a>
## 正向审计
先把代码的aspx弄出来<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1658048225664-060580cd-59e4-4eae-87b3-1f1f1bf0b7dc.png#averageHue=%23f9f7f5&clientId=u9c09329d-c228-4&from=paste&height=474&id=ua135ff7e&originHeight=592&originWidth=450&originalType=binary&ratio=1&rotation=0&showTitle=false&size=52116&status=done&style=none&taskId=uce89b5c0-58e1-4474-bcab-f5369931803&title=&width=360)

根据这个找bin目录下对应的dll文件<br />Inherits="iOffice.ioRepSelPic" Title="Untitled Page" CodeBehind="ioRepSelPic.aspx.vb" %><br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1658048288620-97e3e6c3-84e3-45d4-9b42-d31500fcb817.png#averageHue=%23f4f2ef&clientId=u9c09329d-c228-4&from=paste&height=178&id=u6b61cd8a&originHeight=223&originWidth=1146&originalType=binary&ratio=1&rotation=0&showTitle=false&size=29828&status=done&style=none&taskId=u709e4a11-05f2-47c6-b119-a1514914858&title=&width=916.8)

对应dll文件中的方法<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1658048321143-667a868f-a714-4c4b-bfd3-782b32479da8.png#averageHue=%23f3f0ee&clientId=u9c09329d-c228-4&from=paste&height=100&id=u84d877f2&originHeight=125&originWidth=1087&originalType=binary&ratio=1&rotation=0&showTitle=false&size=17138&status=done&style=none&taskId=u0d419ae1-a0c9-4790-8c87-0785eee94ce&title=&width=869.6)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1658049433345-02c288b3-5e0c-496b-af1f-1d175345de26.png#averageHue=%2326292a&clientId=u9c09329d-c228-4&from=paste&height=283&id=u3cfc02d5&originHeight=354&originWidth=421&originalType=binary&ratio=1&rotation=0&showTitle=false&size=30079&status=done&style=none&taskId=u17a349b4-5802-4416-b176-bf497a434a9&title=&width=336.8)


<a name="jTJWM"></a>
## 入门
hello word<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1661957455312-c1702fe8-1115-4b82-a261-1ecb09566fcb.png#averageHue=%23252424&clientId=u84a2205b-8dc9-4&from=paste&height=212&id=ua5f71b06&originHeight=265&originWidth=770&originalType=binary&ratio=1&rotation=0&showTitle=false&size=21029&status=done&style=none&taskId=u04c919b6-96ad-4785-a9c7-355bcfaad30&title=&width=616)

<a name="K2T9h"></a>
### 定义变量
string a = "aufeng nb";<br />double[] balance = new double[10];
```csharp
double d = 5673.74;
int i;
// 强制转换 double 为 int
i = (int)d;
Console.WriteLine(i);
Console.ReadKey();
```
```csharp
int i = 75;
float f = 53.005f;
double d = 2345.7652;
bool b = true;
Console.WriteLine(i.ToString());
Console.WriteLine(f.ToString());
Console.WriteLine(d.ToString());
Console.WriteLine(b.ToString());
Console.ReadKey();
```

<a name="EGdv0"></a>
### 匹配输出
```csharp
string a = "aufeng nb";
int b, c, d;
b = 1;
c = 2;
d = 3;
Console.WriteLine("a = {0}, b = {1}, c = {2},d = {3}", a, b, c,d);
```
<a name="ZhtID"></a>
### 循环
无限循环
```csharp
{
   
    class Program
    {
        static void Main(string[] args)
        {
            for (; ; )
            {
                Console.WriteLine("Hey! I am Trapped");
            }
 
        }
    }
}
```

<a name="RWB9J"></a>
### Path.Combine 拼接路径
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664260948537-7b345bdb-6d40-4e15-a947-79a2e24d8b88.png#averageHue=%23eae9e8&clientId=u823a0a68-63ae-4&from=paste&height=458&id=u7758e5db&originHeight=573&originWidth=589&originalType=binary&ratio=1&rotation=0&showTitle=false&size=35569&status=done&style=none&taskId=u2466567b-8739-4d48-b032-55ec16c1b98&title=&width=471.2)
<a name="nmrva"></a>
## aspx的oa
红帆oa、万户oa、exchange、信和oa

<a name="OkKmz"></a>
## 文件复制
<a name="FF5m4"></a>
### CopyTo
```
public void saveFile(string filePathName , string toFilesPath , string newFileName)
{
    FileInfo file = new FileInfo(filePathName);
    file.CopyTo(toFilesPath + @"\" + newFileName, true);
}
```

<a name="dd2ea"></a>
### MoveTo
```
//被移动的文件或文件目录
string movePath = @"E:\文件\\文件1";
//目标目录
string moveToPath = @"E:\移动目标文件\\"+DateTime.Now.ToString("yyyy-MM-dd_HHmmss");
//文件夹移动不能同名目录
DirectoryInfo di = new DirectoryInfo(movePath);
di.MoveTo(moveToPath);
```
<a name="CGGtd"></a>
## 文件写入
<a name="F0oAT"></a>
### WriteAllText
```csharp
namespace write_anything
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("please enter filepath: ");
            string str = Console.ReadLine();
            System.IO.File.WriteAllText("E:\\c#\\test2.txt", str);
            Console.WriteLine("write success");
        }
    }
}
```

<a name="fw3jf"></a>
### StreamWriter
```csharp
using System.IO;
using System.Text;

namespace write_anything
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("please enter: ");
            string str = Console.ReadLine();
            StreamWriter sr = new StreamWriter("E:\\c#\\source\\WebApplication1\\ConsoleApp1\\test.txt", true, Encoding.Default);  // 保留文件原来的内容
            sr.WriteLine(str);
            sr.Flush();  // 清空缓存
            sr.Close();  // 关闭文件
        }
    }
}
```

<a name="DnbRy"></a>
### BinaryWriter writer
```c
        static void Test()
        {
            string fileName = "E:\\c#\\source\\ConsoleApp1\\ConsoleApp1\\binaryfile.txt";
            using (BinaryWriter writer = new BinaryWriter(File.Open(fileName, FileMode.Create)))
            {
                writer.Write(1024);
                writer.Write("this is string data-");
                writer.Write(true);
            }
            Console.WriteLine("Data written successfully...");
        }
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662994703547-28cf9144-326a-4841-a953-7cc5ba2eaf9c.png#averageHue=%23828180&clientId=u09cd249b-78be-4&from=paste&height=422&id=ud2e76d2e&originHeight=527&originWidth=1428&originalType=binary&ratio=1&rotation=0&showTitle=false&size=85044&status=done&style=none&taskId=u6a0d8837-39ed-4c7e-98cf-2b367774cac&title=&width=1142.4)
<a name="Ii1Sr"></a>
### TextWriter writer
```c
using System;
using System.IO;
namespace TextWriterExample
{
    class Program
    {
        static void Main(string[] args)
        {
            using (TextWriter writer = File.CreateText("e:\\TextWriter.txt"))
            {
                writer.WriteLine("Hello C#, TextWriter");
                writer.WriteLine("C# File Handling by Yiibai.com");
            }
            Console.WriteLine("Data written successfully...");
        }
    }
}
//更多请阅读：https://www.yiibai.com/csharp/c-sharp-textwriter.html


```

<a name="mD1bZ"></a>
###  StringWriter writer  
```c
using System;
using System.IO;
using System.Text;
namespace CSharpProgram
{
    class Program
    {
        static void Main(string[] args)
        {
            string text = "Hello, Welcome to the yiibai.com \n" +
                "It is nice site. \n" +
                "It provides IT tutorials";
            // Creating StringBuilder instance  
            StringBuilder sb = new StringBuilder();
            // Passing StringBuilder instance into StringWriter  
            StringWriter writer = new StringWriter(sb);
            // Writing data using StringWriter  
            writer.WriteLine(text);
            writer.Flush();
            // Closing writer connection  
            writer.Close();
            // Creating StringReader instance and passing StringBuilder  
            StringReader reader = new StringReader(sb.ToString());
            // Reading data  
            while (reader.Peek() > -1)
            {
                Console.WriteLine(reader.ReadLine());
            }
        }
    }
}
//更多请阅读：https://www.yiibai.com/csharp/c-sharp-stringwriter.html


```

<a name="B07lE"></a>
### FileStream memoryStream.WriteTo
```
			try
			{
				using (FileStream fileStream = new FileStream(path, FileMode.Create, FileAccess.Write))
				{
					memoryStream.WriteTo(fileStream);
					fileStream.Flush();
				}
				jsonMsg = "ok";
				result = true;
			}
```

<a name="ptLW1"></a>
### FileStream
```
fileStream = new FileStream(strFilePath + strFileName, FileMode.OpenOrCreate, FileAccess.Write);
fileStream.Write(ImgIn, 0, ImgIn.Length);
```

<a name="hCCV3"></a>
## 任意文件解压
<a name="Dqe2a"></a>
### zipInputStream

获取压缩包内的文件名
```
ZipEntry nextEntry = zipInputStream.GetNextEntry(); nextEntry != null; nextEntry = zipInputStream.GetNextEntry()
```

例如下述代码就是获取压缩包内的文件名，然后再写入到路径中，相当于解压的功能<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1680446607766-4766f98a-aa5f-43d2-b331-e3d55cfcec22.png#averageHue=%23201f1f&clientId=u80a0917d-2dce-4&from=paste&height=536&id=ue916a241&originHeight=670&originWidth=1223&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=127933&status=done&style=none&taskId=ua374f540-3eaa-407e-97ea-13fa4e22439&title=&width=978.4)
<a name="jEQ8d"></a>
## c# web接收参数
<a name="mkCDe"></a>
### get请求获取
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662208110377-27f8c1df-451c-4e5e-92bc-873da75d8b53.png#averageHue=%23f1e3b5&clientId=udea671b8-52ba-4&from=paste&height=102&id=u23c0ac83&originHeight=128&originWidth=823&originalType=binary&ratio=1&rotation=0&showTitle=false&size=7531&status=done&style=none&taskId=u74847611-4a04-4036-ab76-17e244d07e8&title=&width=658.4)<br />ID对应的labelmessage参数是要对接到后端处理  this.labelmessage.Text = Request.QueryString["user"];
```csharp
<%@ Page Language="C#" AutoEventWireup="true" CodeFile="ShowData.aspx.cs" Inherits="ShowData" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
        <div>
            <asp:Label runat="server" Text="Label" ID="labelmessage"></asp:Label>
        </div>
    </form>
</body>
</html>

```
```csharp
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class ShowData : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (!this.IsPostBack)
        {
            if (Request.QueryString["user"] != null)
            {
                this.labelmessage.Text = Request.QueryString["user"];

            }
        }
    }
}

```

<a name="xEif5"></a>
### post请求获取参数(xss漏洞原理)
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662209292775-d2c88520-903d-4735-87e9-b50ab5857792.png#averageHue=%23d9a255&clientId=udea671b8-52ba-4&from=paste&height=140&id=ue5dabe48&originHeight=175&originWidth=554&originalType=binary&ratio=1&rotation=0&showTitle=false&size=7331&status=done&style=none&taskId=u0cace325-8b98-4b7a-8526-5bc20e103d3&title=&width=443.2)<br />Request.Form["userName"];<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662789219104-3b093551-2faf-4bdc-95b4-0b484bd4e224.png#averageHue=%23202020&clientId=u1b0cdce6-cd3b-4&from=paste&height=213&id=u893338bb&originHeight=247&originWidth=638&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10220&status=done&style=none&taskId=u7bc60517-d1e9-401d-afab-b3098aca21d&title=&width=549.3999938964844)
<a name="QjPfN"></a>
## SQL创建
<a name="OqBUh"></a>
### 实体类
UserInfo.cs  实体类  数据库对应的字段  一般是Model文件夹<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662792747554-70542809-2ace-40b8-9972-7d32f8a34e4f.png#averageHue=%23eeede8&clientId=ufa545a18-2990-4&from=paste&height=388&id=u2733040c&originHeight=485&originWidth=1173&originalType=binary&ratio=1&rotation=0&showTitle=false&size=234752&status=done&style=none&taskId=u53f1d4a8-c3f8-4707-8bee-a4c4db4ed38&title=&width=938.4)

<a name="mTX5p"></a>
### 数据访问层
这里的DBHepler.cs 设置为数据库连接的密码，但是一般情况下是在 web.config中设置<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662793058512-a0b1cb9c-4888-4270-91c4-42fc33234b20.png#averageHue=%23edefe9&clientId=ufa545a18-2990-4&from=paste&height=460&id=u6c5f8dd0&originHeight=575&originWidth=1156&originalType=binary&ratio=1&rotation=0&showTitle=false&size=332737&status=done&style=none&taskId=u4c3719d4-1ee1-42f2-bafc-0b931ab7b4a&title=&width=924.8)<br />UserinfoService.cs      这里往往是找SQL注入的地方     视频中创建的文件夹为DAL<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662792926236-9291806b-2b9c-4c52-873b-30a002f98f7a.png#averageHue=%23ecede6&clientId=ufa545a18-2990-4&from=paste&height=438&id=u539f4e6d&originHeight=548&originWidth=1223&originalType=binary&ratio=1&rotation=0&showTitle=false&size=361909&status=done&style=none&taskId=u3b03bc59-6433-42df-a550-cf969686216&title=&width=978.4)

<a name="gnrze"></a>
### 业务逻辑层
UserInfoManager.cs  用来调用UserInfoService.cs把参数传进去然后进行判断(这里作为SQL的传参)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662793348775-cf623e1b-40fa-4246-9786-5e227dab6b4c.png#averageHue=%23eef0ea&clientId=ufa545a18-2990-4&from=paste&height=290&id=u14f6db0b&originHeight=363&originWidth=1202&originalType=binary&ratio=1&rotation=0&showTitle=false&size=186550&status=done&style=none&taskId=u330381f6-c0a4-4096-9d41-4bfb5dd33fa&title=&width=961.6)

<a name="cV4Hj"></a>
### 表现层

引入前面实体类、数据访问层、业务逻辑层<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662794319434-e366c212-fc53-4f41-b7fe-8c278d2af68b.png#averageHue=%23eeefe7&clientId=ufa545a18-2990-4&from=paste&height=350&id=ud383f411&originHeight=438&originWidth=791&originalType=binary&ratio=1&rotation=0&showTitle=false&size=229273&status=done&style=none&taskId=u2266eb49-5c26-48e0-a439-fdb9fde299c&title=&width=632.8)

这里调用UserInfoManager传入判断<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662794262931-6e45fbd8-8c43-4d50-90a2-db2f09e2aeac.png#averageHue=%23e8e8e2&clientId=ufa545a18-2990-4&from=paste&height=410&id=u5600fce6&originHeight=513&originWidth=1119&originalType=binary&ratio=1&rotation=0&showTitle=false&size=377305&status=done&style=none&taskId=u0769f378-e42a-4a7b-9cb4-9b15a081133&title=&width=895.2)

login.aspx.cs→UserInfoManager→UserInfoServices(数据库查询)
<a name="kkgOb"></a>
## 文件上传
SaveAs
```csharp
{
string filepath = FileUpload1.PostedFile.FileName;
string filename = filepath.Substring(filepath.LastIndexOf("\") + 1);
string serverpath = Server.MapPath("images/") + filename;
FileUpload1.PostedFile.SaveAs(serverpath);
this.lb_info.Text = "上传成功！";
}
```
<a name="UuWeG"></a>
### .net框架 文件上传代码
代审tips  找aspx前端中的FileUpload  这个是固定的上传的前端参数<br />this.前端文件上传的接口.方法    <br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662179783696-f8c0a6b0-cd7d-433e-8190-37c2acc3413b.png#averageHue=%232c2c2b&clientId=u451d8662-94eb-4&from=paste&height=495&id=u5de112a2&originHeight=619&originWidth=1243&originalType=binary&ratio=1&rotation=0&showTitle=false&size=76792&status=done&style=none&taskId=uc4e524ec-bc1d-46a3-ba1a-26d6f7da316&title=&width=994.4)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662179831411-7779522e-bbb7-41b5-b1a2-022ffdb830e2.png#averageHue=%23daa257&clientId=u451d8662-94eb-4&from=paste&height=170&id=u1006d48c&originHeight=213&originWidth=706&originalType=binary&ratio=1&rotation=0&showTitle=false&size=24518&status=done&style=none&taskId=u0e1540a1-d1d9-4a76-9185-f336498ee35&title=&width=564.8)
```csharp
<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Upload.aspx.cs" Inherits="Upload" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
        <div>
            <asp:Image runat="server" ID="imgPic"></asp:Image></br>
            <asp:FileUpload runat="server" ID="fileUp"></asp:FileUpload>
            <asp:Button runat="server" Text="文件上传" OnClick="Unnamed2_Click" ID="btnUpload"></asp:Button>
            <asp:Label runat="server" Text="Label" ID="lblMessage"></asp:Label>
        </div>
    </form>
</body>
</html>

```
```csharp
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

public partial class Upload : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }

    protected void Unnamed2_Click(object sender, EventArgs e)
    {
        if (this.fileUp.HasFile)
        {
            string filename = this.fileUp.FileName;
            string fixname = filename.Substring(filename.LastIndexOf('.') + 1).ToLower();
            if(fixname == "png" || fixname == "jpg")
            {
                string filepath = Server.MapPath(".") + "//upload//" + filename;
                this.fileUp.SaveAs(filepath);
                this.imgPic.ImageUrl = "~/upload/" + filename;
            }
            else
            {
                this.lblMessage.Text = "upload err,图片不允许后缀";
            }
        }
    }
}
```
对应的文件上传请求包<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662179861955-bdc80eeb-76fd-4992-8067-35aebc222db6.png#averageHue=%23faf7f7&clientId=u451d8662-94eb-4&from=paste&height=650&id=ua29fbd02&originHeight=813&originWidth=1536&originalType=binary&ratio=1&rotation=0&showTitle=false&size=74656&status=done&style=none&taskId=uc35d4b29-49d9-4721-bb98-8d3234ae535&title=&width=1228.8)
```csharp
POST /Upload.aspx HTTP/1.1
Host: localhost:3152
User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64; rv:56.0) Gecko/20100101 Firefox/56.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
Content-Type: multipart/form-data; boundary=---------------------------31474319012958689546504783967
Content-Length: 1428
Origin: http://localhost:3152
Connection: close
Referer: http://localhost:3152/Upload.aspx
Cookie: JSESSIONID=1B7FA0B68D1ECD18740139065AAD6B73; XSRF-TOKEN=1370610e-4622-4dab-a860-3aaa244f7c8a
Upgrade-Insecure-Requests: 1
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: same-origin
Sec-Fetch-User: ?1

-----------------------------31474319012958689546504783967
Content-Disposition: form-data; name="__VIEWSTATE"

7PP+F13uStZPnBZXHTaJVa3Z9HtmOpsEt+DEJ94DYBXmSWA2Ng1/P2qQZM/TqWBLmwYSYcwLveFOoyBM9LPtppmFzhau7VjoW7UlebXBV3lnS1Pt4bMiE5yralKftxXrA/X9xR+WxZHU3CcJnnqNCfGoL1E5/Bz9tCcNikBzo/OiE8imigrmnXhkCBR/e+HMJ4cm8o6Q3dElZms1I/2tAg==
-----------------------------31474319012958689546504783967
Content-Disposition: form-data; name="fileUp"; filename="空白.png"
Content-Type: image/png

�PNG

B`�
-----------------------------31474319012958689546504783967
Content-Disposition: form-data; name="btnUpload"

文件上传
-----------------------------31474319012958689546504783967
Content-Disposition: form-data; name="__VIEWSTATEGENERATOR"

69164837
-----------------------------31474319012958689546504783967
Content-Disposition: form-data; name="__EVENTVALIDATION"

Ms4qy45wZhohuTRgr9/d8hIBMBApzqnfzk/lpCV6xPuiyMEX9g4sQ5ul5Bqhqq1lFo7k/Ei7GMoFerNns+goF8DvBhbOWx4u5VCiF98W1E0OWaEMzNsWg9OSxe5jv8sR
-----------------------------31474319012958689546504783967--
```

<a name="EUYPi"></a>
### lastindexof判断
lastindexof是取最后出现的<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662276074431-2c11ecf5-57df-4d1c-a4b5-a99fbeca47ea.png#averageHue=%23292828&clientId=ub21e93cf-aa02-4&from=paste&height=354&id=uf2e155a4&originHeight=442&originWidth=645&originalType=binary&ratio=1&rotation=0&showTitle=false&size=18893&status=done&style=none&taskId=u76a8d91a-a941-4aff-a57e-e9d626ca8b6&title=&width=516)

但是审计的时候要看清楚，是lastindexof最后的.还是其他字符，例如下图就是存在文件上传漏洞<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662276140913-77fce765-c47c-4968-b36e-fa4b92dc19f4.png#averageHue=%232d2c26&clientId=ub21e93cf-aa02-4&from=paste&height=110&id=u722ddf09&originHeight=138&originWidth=772&originalType=binary&ratio=1&rotation=0&showTitle=false&size=148184&status=done&style=none&taskId=u11352281-9f34-4a38-8db8-8b0352a714e&title=&width=617.6)

<a name="jOV5D"></a>
## 任意文件读取(mvc)
<a name="pPvNT"></a>
### new BinaryReader
FileStream fileStream = new FileStream(str, FileMode.Open);<br />BinaryReader binaryReader = new BinaryReader(fileStream);
```csharp
namespace ConsoleApp1
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("please enter filepath: ");
            string str = Console.ReadLine();
            FileStream fileStream = new FileStream(str, FileMode.Open);
            BinaryReader binaryReader = new BinaryReader(fileStream);
            //读取文件的一个字符
            int a = binaryReader.Read();
            //判断文件中是否含有字符，若不含字符，a 的值为 -1
            while (a != -1)
            {
                //输出读取到的字符
                Console.Write((char)a);
                a = binaryReader.Read();
            }
        }
    }
}
```
<a name="bjxgI"></a>
### OpenRead
path没有校验传入的路径，要追踪controller层哪里调用他才能构造poc<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1663304283115-d6b155a2-8016-457a-97a3-33213f7ed871.png#averageHue=%23232322&clientId=u78698c96-68a9-4&from=paste&height=474&id=ubb09f2de&originHeight=592&originWidth=1578&originalType=binary&ratio=1&rotation=0&showTitle=false&size=112327&status=done&style=none&taskId=u3c77e0ac-5844-4bed-9e1e-e7b36f72d40&title=&width=1262.4)
```
			{
				try
				{
					Stream stream = File.OpenRead(path);
					byte[] array = new byte[stream.Length];
					stream.Read(array, 0, array.Length);
					stream.Seek(0L, SeekOrigin.Begin);
					result = array;
				}
				catch
				{
					result = null;
				}
			}
```
可以发现 Map和App调用了，所以这两个路径都存在漏洞点<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1663304412308-e311134c-d71d-4ab3-982d-58f550fa2b0b.png#averageHue=%232f2a26&clientId=u78698c96-68a9-4&from=paste&height=188&id=ufe3219ca&originHeight=235&originWidth=1599&originalType=binary&ratio=1&rotation=0&showTitle=false&size=54258&status=done&style=none&taskId=uc874fdfd-aa86-4fea-aa4f-a2fff9b3616&title=&width=1279.2)

App的controller中调用了，所以存在任意文件下载，并且没有进行权限校验<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1663304501535-61a11c16-2a0a-47d9-a9ab-e6a10a7d370e.png#averageHue=%23242322&clientId=u78698c96-68a9-4&from=paste&height=402&id=u679bf445&originHeight=502&originWidth=946&originalType=binary&ratio=1&rotation=0&showTitle=false&size=72090&status=done&style=none&taskId=u6f40baab-dd21-4afa-af52-af8f65e2017&title=&width=756.8)<br />Map中也调用了，但是因为有[SingleUserAuthorize]这个自动有的权限校验方式认证了，所以目前没办法未授权利用漏洞<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1663304565076-16adbc95-960e-45d5-ba02-47f91ce19a92.png#averageHue=%23232322&clientId=u78698c96-68a9-4&from=paste&height=414&id=ud8128e9f&originHeight=517&originWidth=1114&originalType=binary&ratio=1&rotation=0&showTitle=false&size=94929&status=done&style=none&taskId=u521213bb-79ea-4991-8071-a1ebfacf9ca&title=&width=891.2)

poc<br />/AppVersion/DownloadFile?filename=../../web.config<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1663304662552-e9e075be-c705-4bd0-a578-3b9394b7c76e.png#averageHue=%23fdfcfc&clientId=u78698c96-68a9-4&from=paste&height=227&id=u57fd7da9&originHeight=284&originWidth=1052&originalType=binary&ratio=1&rotation=0&showTitle=false&size=19041&status=done&style=none&taskId=uff918eff-5747-4859-89ae-a94eadac535&title=&width=841.6)
<a name="Kcr8e"></a>
### FileAccess.Read
stream = new FileStream(path, FileMode.Open, FileAccess.Read, FileShare.Read);<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1675351523446-c1450381-b0e4-4978-a093-08e582d4c720.png#averageHue=%23212020&clientId=uccaf58a4-1a3d-4&from=paste&height=396&id=u9aa4d8c4&originHeight=495&originWidth=975&originalType=binary&ratio=1&rotation=0&showTitle=false&size=71906&status=done&style=none&taskId=u5efb889c-920e-40d7-84e9-0efabcd5c94&title=&width=780)

<a name="cj3lG"></a>
## 远程文件请求下载
<a name="Nsny1"></a>
### WebRequest.Create
例如ueditor中这段代码HttpWebRequest.Create
```
        var request = HttpWebRequest.Create(this.SourceUrl) as HttpWebRequest;
        using (var response = request.GetResponse() as HttpWebResponse)
        {
            if (response.StatusCode != HttpStatusCode.OK)
            {
                State = "Url returns " + response.StatusCode + ", " + response.StatusDescription;
                return this;
            }
            if (response.ContentType.IndexOf("image") == -1)
            {
                State = "Url is not an image";
                return this;
            }
            ServerUrl = PathFormatter.Format(Path.GetFileName(this.SourceUrl), Config.GetString("catcherPathFormat"));
            var savePath = Server.MapPath(ServerUrl);
            if (!Directory.Exists(Path.GetDirectoryName(savePath)))
            {
                Directory.CreateDirectory(Path.GetDirectoryName(savePath));
            }
            try
            {
                var stream = response.GetResponseStream();
                var reader = new BinaryReader(stream);
                byte[] bytes;
                using (var ms = new MemoryStream())
                {
                    byte[] buffer = new byte[4096];
                    int count;
                    while ((count = reader.Read(buffer, 0, buffer.Length)) != 0)
                    {
                        ms.Write(buffer, 0, count);
                    }
                    bytes = ms.ToArray();
                }
                File.WriteAllBytes(savePath, bytes);
                State = "SUCCESS";
            }
            catch (Exception e)
            {
                State = "抓取错误：" + e.Message;
            }
            return this;
        }
    }
```

<a name="qAug7"></a>
### WebClient
```
string url = "http://www.mozilla.org/images/feature-back-cnet.png";
WebClient myWebClient = new WebClient();
myWebClient.DownloadFile(url,"C:\\temp\\feature-back-cnet.png");
```
<a name="VBTew"></a>
## c#命令执行
Process
```
        {
            //實例一個Process類，啟動一個獨立進程
            Process p = new Process();
            //Process類有一個StartInfo屬性，這個是ProcessStartInfo類，包括了一些屬性和方法，下面我們用到了他的幾個屬性：
            p.StartInfo.FileName = "cmd.exe";           //設定程序名
            p.StartInfo.Arguments = "/c " + command;    //設定程式執行參數
            p.StartInfo.UseShellExecute = false;        //關閉Shell的使用
            p.StartInfo.RedirectStandardInput = true;   //重定向標準輸入
            p.StartInfo.RedirectStandardOutput = true;  //重定向標準輸出
            p.StartInfo.RedirectStandardError = true;   //重定向錯誤輸出
            p.StartInfo.CreateNoWindow = true;          //設置不顯示窗口
            p.Start();   //啟動
            //p.StandardInput.WriteLine(command);       //也可以用這種方式輸入要執行的命令
            //p.StandardInput.WriteLine("exit");        //不過要記得加上Exit要不然下一行程式執行的時候會當機
            return p.StandardOutput.ReadToEnd();        //從輸出流取得命令執行結果
        }
```

<a name="yadqO"></a>
## FileInfo.MoveTo 修改文件名称 文件重命名
```
FileInfo fileInfo = new FileInfo(strSourceFileName);
if (strSourceFileName.ToUpper() != strReNameFileName.ToUpper())
{
new FileInfo(strReNameFileName).MoveTo(strReNameFileName + DateTime.Now.ToString("yyyyMMddHHmmss"));
fileInfo.MoveTo(strReNameFileName);
result = "True";
}
```

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1665933396573-8e985c7d-523f-432d-b576-f6e40a311c04.png#averageHue=%23fcfbfa&clientId=u57614f56-b353-4&from=paste&height=462&id=u3cab0bc4&originHeight=578&originWidth=924&originalType=binary&ratio=1&rotation=0&showTitle=false&size=113112&status=done&style=none&taskId=u44588ceb-6268-4859-bcaf-0b56bac5251&title=&width=739.2)
<a name="bwloG"></a>
## 文件上传测试用的aspx

<%Response.Write("hello word")%>
```c
<html>
<body bgcolor="yellow">
<center>
<h2>Hello aaaaaaaaaaaaaaaaaaf!</h2>
<p><%Response.Write(now())%></p>
</center>
</body>
</html>
```
<a name="XOVQN"></a>
## from身份验证
web.config中配置如下文件<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662262496632-beff87ac-1853-4ca7-a90a-3ed1a0590d7b.png#averageHue=%23f9f8f2&clientId=ub21e93cf-aa02-4&from=paste&height=481&id=uff308d15&originHeight=601&originWidth=925&originalType=binary&ratio=1&rotation=0&showTitle=false&size=275302&status=done&style=none&taskId=u0783a45a-efb0-4632-986f-7f4d0f04b09&title=&width=740)

对应的后端处理代码两种方法<br />FormsAuthentication.SetAuthCookie<br />FormsAuthentication.RedirectFromLoginPage<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662266614465-2b69a1d1-70d5-43fc-b28f-7a201e25f817.png#averageHue=%23f1f0ea&clientId=ub21e93cf-aa02-4&from=paste&height=474&id=u5bd17ebd&originHeight=593&originWidth=932&originalType=binary&ratio=1&rotation=0&showTitle=false&size=290264&status=done&style=none&taskId=u9768df58-d124-4047-8cc3-ccfba06aab0&title=&width=745.6)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662267737033-0ee241c1-f54d-4628-a2f2-ad633422645e.png#averageHue=%23e7ecdf&clientId=ub21e93cf-aa02-4&from=paste&height=481&id=ubef2dea5&originHeight=601&originWidth=959&originalType=binary&ratio=1&rotation=0&showTitle=false&size=344933&status=done&style=none&taskId=u1c0e7116-af49-4953-b222-97ab4bce1ce&title=&width=767.2)

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662468201913-c5b83698-88e2-4fef-a6f7-e4752242de4f.png#averageHue=%23dcfdfb&clientId=u7b5b2949-c27c-4&from=paste&height=676&id=udb687dbf&originHeight=845&originWidth=1266&originalType=binary&ratio=1&rotation=0&showTitle=false&size=73660&status=done&style=none&taskId=uf80fc22f-09e3-4cc7-8681-6d16b86d720&title=&width=1012.8)

<a name="abNd4"></a>
## 关于绕过权限校验问题
[https://github.com/aleenzz/.NET_study/blob/master/asp.net_bug/%E7%AC%AC%E4%BA%8C%E7%AB%A0%EF%BC%9A%E8%B6%8A%E6%9D%83.md](https://github.com/aleenzz/.NET_study/blob/master/asp.net_bug/%E7%AC%AC%E4%BA%8C%E7%AB%A0%EF%BC%9A%E8%B6%8A%E6%9D%83.md)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662792563730-aacd8187-a141-4d6f-9667-e6952b03a1a4.png#averageHue=%23fdfcfa&clientId=ufa545a18-2990-4&from=paste&height=158&id=u78468400&originHeight=197&originWidth=701&originalType=binary&ratio=1&rotation=0&showTitle=false&size=24855&status=done&style=none&taskId=u6d348b2e-2bd4-4867-85a9-242f9277600&title=&width=560.8)
<a name="CKWgh"></a>
## ConfigurationManager.AppSettings
ls_FilePath = ConfigurationManager.AppSettings["UploadFilePath"]<br />这个ConfigurationManager.AppSettings其实就是web.config中对应的"UploadFilePath"<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662021548156-a58154a7-5e48-41de-8abf-d3a787041876.png#averageHue=%23e8e493&clientId=u0f932856-661b-4&from=paste&height=96&id=u92c5265b&originHeight=85&originWidth=567&originalType=binary&ratio=1&rotation=0&showTitle=false&size=10843&status=done&style=none&taskId=u838692f4-a175-4602-b8c9-60eac290ad6&title=&width=643.6000061035156)

<a name="sbMG2"></a>
## 预编译问题(上传的aspx无法执行)
由于根目录下的PrecompiledApp.config 中的updatable值设置成了false。导致后面上传的文件无法进行编译。aspx程序无法正常执行。


<a name="X0qJB"></a>
## 注释符(可以绕waf)
只能空格、 .左右  (左右加注释符<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662101712971-570c56ba-1a8d-4522-87b6-115be25c3137.png#averageHue=%231f1f1f&clientId=u446f3ddc-96fc-4&from=paste&height=371&id=u24c0cee5&originHeight=464&originWidth=963&originalType=binary&ratio=1&rotation=0&showTitle=false&size=15231&status=done&style=none&taskId=ud4050f58-1f5a-471c-a29c-a5cf6e308b1&title=&width=770.4)

<a name="gKshY"></a>
## unicode编码(绕waf
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1663469608494-fdf1c989-e822-4979-9465-1ef43709449a.png#averageHue=%23212020&clientId=u4be8ebd2-c215-4&from=paste&height=112&id=ueb41d5c1&originHeight=140&originWidth=883&originalType=binary&ratio=1&rotation=0&showTitle=false&size=19928&status=done&style=none&taskId=u592744f3-fca5-476b-982f-845d0608ed4&title=&width=706.4)

<a name="Xtc26"></a>
## unicode+注释符绕waf
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1663469849074-0b346a06-e1dc-4c66-9fbd-5e4495645bc9.png#averageHue=%23201f1f&clientId=u4be8ebd2-c215-4&from=paste&height=101&id=u2835ec11&originHeight=126&originWidth=1039&originalType=binary&ratio=1&rotation=0&showTitle=false&size=19353&status=done&style=none&taskId=u8c85deff-2669-484b-92fd-83b8638044a&title=&width=831.2)

<a name="I9QQ4"></a>
## @绕waf
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664079593111-c2932fb1-f897-4c8e-bb44-ae44e0058f67.png#averageHue=%23212120&clientId=u608ba18d-699d-4&from=paste&height=107&id=u92685d29&originHeight=134&originWidth=998&originalType=binary&ratio=1&rotation=0&showTitle=false&size=24805&status=done&style=none&taskId=u43c85bcb-bf15-4ac0-8654-5f486d7ad56&title=&width=798.4)
<a name="vkWtI"></a>
## 创建 asp.net以及vs注意的地方
要勾上红框的项目，才能有ASP.NET web应用程序的项目<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662136255091-573e7710-49ae-4440-8746-c3c6d541a218.png#averageHue=%23e9e9e9&clientId=u451d8662-94eb-4&from=paste&height=686&id=ue9dc1ba5&originHeight=858&originWidth=1598&originalType=binary&ratio=1&rotation=0&showTitle=false&size=204672&status=done&style=none&taskId=uf8a45298-5c34-49c1-91db-5e5a4056458&title=&width=1278.4)

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662136335783-149531a1-4b8e-4200-8944-cd408735fd8a.png#averageHue=%232c2c2c&clientId=u451d8662-94eb-4&from=paste&height=675&id=u80ca2a8e&originHeight=844&originWidth=1268&originalType=binary&ratio=1&rotation=0&showTitle=false&size=111046&status=done&style=none&taskId=ue66ee311-5341-4590-bb22-64c308ed058&title=&width=1014.4)<br />选择web from 就会有aspx之类的文件了<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662136370414-615ae02f-4bd1-4c22-8cbb-ccfd8e7e76a7.png#averageHue=%23292827&clientId=u451d8662-94eb-4&from=paste&height=475&id=ua61c002c&originHeight=594&originWidth=422&originalType=binary&ratio=1&rotation=0&showTitle=false&size=41468&status=done&style=none&taskId=u82cc83bb-db87-452b-8480-662caf1a3fd&title=&width=337.6)

![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662136593591-e41196c5-796a-4c86-a4c8-3d7322394cc1.png#averageHue=%23373635&clientId=u451d8662-94eb-4&from=paste&height=558&id=u14444ce7&originHeight=698&originWidth=595&originalType=binary&ratio=1&rotation=0&showTitle=false&size=70907&status=done&style=none&taskId=ua132454d-f507-408a-9736-8450caba2af&title=&width=476)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662136615984-2befb60a-a06b-48b9-ac27-b22aae215cc3.png#averageHue=%23e6e5e4&clientId=u451d8662-94eb-4&from=paste&height=720&id=u38a86bae&originHeight=900&originWidth=1482&originalType=binary&ratio=1&rotation=0&showTitle=false&size=89042&status=done&style=none&taskId=uf3c694d2-1bcd-4886-897c-84a45bdb9a0&title=&width=1185.6)

<a name="z5vgM"></a>
## 学习视频\文章
[https://www.bilibili.com/video/BV16X4y1V7qH?p=37&vd_source=d6ca406befa13042df520f83540ef25a](https://www.bilibili.com/video/BV16X4y1V7qH?p=37&vd_source=d6ca406befa13042df520f83540ef25a)

[https://www.jb51.net/article/84915.htm](https://www.jb51.net/article/84915.htm)

[https://github.com/aleenzz/.NET_study/blob/master/asp.net_bug/%E7%AC%AC%E4%B8%80%E7%AB%A0%EF%BC%9A%E6%B3%A8%E5%85%A5.md](https://github.com/aleenzz/.NET_study/blob/master/asp.net_bug/%E7%AC%AC%E4%B8%80%E7%AB%A0%EF%BC%9A%E6%B3%A8%E5%85%A5.md)


[https://www.yiibai.com/csharp/c-sharp-binarywriter.html](https://www.yiibai.com/csharp/c-sharp-binarywriter.html)   c#基础
<a name="yAhNf"></a>
## ASP.NET和MVC
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662387397754-cacfe7e7-4864-4211-a459-ef448c3696ee.png#averageHue=%23fbfbfb&clientId=ue374a284-b5be-4&from=paste&height=435&id=ube08b208&originHeight=544&originWidth=444&originalType=binary&ratio=1&rotation=0&showTitle=false&size=101633&status=done&style=none&taskId=u73e4de08-dade-4176-a7a6-2776041ca0a&title=&width=355.2)

<a name="eVUNy"></a>
### mvc路由

controller和view的对应关系<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662388641727-f9de4aa5-0ff5-4cf7-a0ad-66df180d61e7.png#averageHue=%23252424&clientId=ue374a284-b5be-4&from=paste&height=552&id=u32500260&originHeight=690&originWidth=1065&originalType=binary&ratio=1&rotation=0&showTitle=false&size=69384&status=done&style=none&taskId=u668ea6ee-f633-4c70-b34d-3e1181393bf&title=&width=852)<br />简单来说<br />controller对应的方法名 路由 /Home/About<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662825929387-8e8fe2fa-4bc3-48fd-91bb-2d3f44549287.png#averageHue=%23242323&clientId=ue24f72f8-9247-4&from=paste&height=469&id=ub50f6c1b&originHeight=586&originWidth=976&originalType=binary&ratio=1&rotation=0&showTitle=false&size=100606&status=done&style=none&taskId=ud9672949-6198-4ef8-9940-3ecae395f42&title=&width=780.8)

FirsController对应的方法名 Index22    路由 → /firs/Index22<br />而且对应的前端cshtml名字要对应才不会报错(当然有的源码不是一定的)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662826148523-e514de7a-96ff-46d4-8391-910ebca76f7d.png#averageHue=%23242322&clientId=ue24f72f8-9247-4&from=paste&height=370&id=u8497efe1&originHeight=463&originWidth=800&originalType=binary&ratio=1&rotation=0&showTitle=false&size=57655&status=done&style=none&taskId=u255b09c0-c484-4c2f-9833-ce15173455b&title=&width=640)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662826190954-8d01fd58-d792-4a6a-b230-d27d644e74ce.png#averageHue=%23cecdcc&clientId=ue24f72f8-9247-4&from=paste&height=264&id=ue707a41c&originHeight=330&originWidth=586&originalType=binary&ratio=1&rotation=0&showTitle=false&size=22321&status=done&style=none&taskId=ued7cb82f-7672-48d3-8489-2807042d884&title=&width=468.8)

例如下图  路由就是Files/Post<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662826402612-55a07090-11bd-4130-a043-86517ddc7d17.png#averageHue=%23222121&clientId=uce7e2317-8e83-4&from=paste&height=455&id=u2404cf72&originHeight=569&originWidth=792&originalType=binary&ratio=1&rotation=0&showTitle=false&size=81801&status=done&style=none&taskId=uaafd5755-23a6-4358-9438-572b893118b&title=&width=633.6)

<a name="rjssb"></a>
### mvc文件上传也解析aspx
mvc文件上传[https://www.jb51.net/article/84915.htm](https://www.jb51.net/article/84915.htm)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662863816922-6d7bb170-d914-4e28-bac3-2d1e63e62a77.png#averageHue=%23fefe00&clientId=uce7e2317-8e83-4&from=paste&height=512&id=u1b33d166&originHeight=640&originWidth=1253&originalType=binary&ratio=1&rotation=0&showTitle=false&size=19486&status=done&style=none&taskId=u3c9ba9c6-bfd2-4a5a-b9e9-2fb802d79f7&title=&width=1002.4)
<a name="mUEzU"></a>
### .net路由
就是目录对应的aspx
<a name="t0n0S"></a>
## 批量反编译
直接把文件夹bin目录拖进去dnspy.exe里边<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662600591131-3145e993-efea-4ef4-9e83-ae55adc10b96.png#averageHue=%237eaeb2&clientId=u948428a7-a325-4&from=paste&height=804&id=ub9702eb4&originHeight=1005&originWidth=471&originalType=binary&ratio=1&rotation=0&showTitle=false&size=103606&status=done&style=none&taskId=ue626c4c0-1c18-4bbd-9161-ebd7b0cfb77&title=&width=376.8)

然后选中这些，点击文件→导出到工程  ，这样子就可以批量反编译源码了<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662600640462-472f6fdc-5dfb-415b-9f2f-95947ea63627.png#averageHue=%23292b2f&clientId=u948428a7-a325-4&from=paste&height=330&id=uf03244da&originHeight=412&originWidth=500&originalType=binary&ratio=1&rotation=0&showTitle=false&size=25973&status=done&style=none&taskId=u82711869-99b5-4c8a-9f73-2aba71d56f2&title=&width=400)

然后在vs里面打开文件夹就可以进行全局搜了<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662600695126-f8ff6bb1-1963-44c8-8da8-77f95aaed85f.png#averageHue=%23252424&clientId=u948428a7-a325-4&from=paste&height=804&id=ue855d6a9&originHeight=1005&originWidth=1716&originalType=binary&ratio=1&rotation=0&showTitle=false&size=233484&status=done&style=none&taskId=u3b5ba0c2-ee7d-48b3-b755-88c34f9b291&title=&width=1372.8)



<a name="XuaKa"></a>
## 批量查找aspx、asmx、asa、ashx路径
dir /s /b | find "aspx"<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662879024687-e6237ed9-097f-4af4-a237-0b4a4fd3b49b.png#averageHue=%231b3b68&clientId=uce7e2317-8e83-4&from=paste&height=565&id=u0df51c8b&originHeight=706&originWidth=1060&originalType=binary&ratio=1&rotation=0&showTitle=false&size=314583&status=done&style=none&taskId=u789e0c22-d9bd-4f4f-9bb1-26a772cef6c&title=&width=848)

查找weisha关键字在所有的aspx文件中<br />findstr /msi /c:"WeiSha" *.aspx<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1662878716297-324eb31c-887b-474d-bdb4-9e512944ec1c.png#averageHue=%2304285a&clientId=uce7e2317-8e83-4&from=paste&height=692&id=u78a7bd0f&originHeight=865&originWidth=844&originalType=binary&ratio=1&rotation=0&showTitle=false&size=130209&status=done&style=none&taskId=u628456f0-3561-476f-8a42-27186f96487&title=&width=675.2)

<a name="lyfhy"></a>
## asmx的路由问题

主要看[WebMethod] 这个是指公开的方法<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1665932804616-6a08f196-fa7a-469e-bcd0-a726bc278a27.png#averageHue=%2325211c&clientId=u57614f56-b353-4&from=paste&height=60&id=u7c1d9da5&originHeight=62&originWidth=597&originalType=binary&ratio=1&rotation=0&showTitle=false&size=8837&status=done&style=none&taskId=u4c88afc9-9eb1-47a2-aecc-80b6b904b87&title=&width=579.6000061035156)

soap协议
```
POST /MedExMedInHandECG/Service.asmx HTTP/1.1
Host: ecg.sywsinfo.cn
Content-Type: text/xml; charset=utf-8
Content-Length: length
SOAPAction: "http://tempuri.org/ReNameFileName"

<?xml version="1.0" encoding="utf-8"?>
<soap:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
  <soap:Body>
    <ReNameFileName xmlns="http://tempuri.org/">
      <strSourceFileName>string</strSourceFileName>
      <strReNameFileName>string</strReNameFileName>
    </ReNameFileName>
  </soap:Body>
</soap:Envelope>
```
url get请求<br />GET /MedExMedInHandECG/Service.asmx/ReNameFileName?strSourceFileName=string&strReNameFileName=string HTTP/1.1
