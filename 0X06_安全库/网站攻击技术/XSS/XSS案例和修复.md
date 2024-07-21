<a name="Yz5xB"></a>
## 一、XSS的测试技巧和工具
<a name="oLmGb"></a>
### 测试方法

1. 工具扫描：APPscan、awvs、burpsuite、XSSER XSSF
2. 手动测试：Burpsuite、firefox(hackbar)<br />1）在目标站点上找到输入点，比如查询接口，留言板等；<br />2）输入一组"特殊字符+唯一识别字符"，点击提交后，查看返回的源码，是否有做对应的处理；<br />3）通过搜索定位到唯一字符，结合唯一字符前后语法确认是否可以构造执行js的条件(构造闭合)；提交构造的脚本代码，看是否可以成功执行，如果成功执行则说明存在XSS漏洞；<br />**第一种情况：可得知输出位置**<br />输入一些敏感字符，例如、"、'、()等等，在提交请求后去查看HTML源代码，看这些输入的字符是否被转义。<br />在输出这些敏感字符的时候，很有可能程序已经做过了过滤，那么你在寻找这些字符的时候就不是那么容易了，这时候你可以直接输入XXSER<>"'&，然后在查找源代码的时候直接进行查找XXSER或许比较方便一些。<br />**第二种情况：无法得知输出位置**<br />非常多的Web应用程序源代码是不对外开放的，这时在进行测试XSS时就有可能无法得知输入数据到底在何处显示，比如测试某留言本是否存在XSS，那么在留言之后，你必须要经过管理员的审核才能进行显示，这时你是无法得知，你输入的数据在后台管理页面处于何种状态。
<a name="o5L4h"></a>
### 万能XSS漏洞payload
```
"/></textarea><script>alert(1)</script>
```
**三种弹窗函数**

- 仅弹窗：alart
- 弹输入框：prompt
- 弹是否：confirm
<a name="sg6ZH"></a>
### XSS辅助工具

1. beef：[https://beefproject.com/](https://beefproject.com/)<br />一个XSS攻击框架，参考[https://www.freebuf.com/articles/web/175755.html](https://www.freebuf.com/articles/web/175755.html)
2. xssor：[http://xssor.io](http://xssor.io)<br />免费的JavaScript在线入侵工具
3. XSS平台：<br />在线的XSS平台，有很多，自行搜索
4. ie tester：[http://www.ietester.cn/](http://www.ietester.cn/)<br />可以模拟测试各种版本的老IE浏览器
<a name="SPulB"></a>
## 二、三种XSS实战
<a name="SZbbW"></a>
### A、 存储型XSS实战

1. 存储型XSS原理<br />攻击者在页面上插入XSS代码，服务端将数据插入数据库，当用户访问到XSS漏洞的页面时，服务端从数据库中取出数据展示到页面上，导致XSS代码执行，达到攻击效果
2. 可能存在存储型XSS的功能点<br />在页面上所有可以输入并保存数据的地方，都可以尝试使用XSS代码进行测试
3. 存储型XSS漏洞的挖掘方法<br />尝试在输入的内容中加入如下字段，如果保存后的页面，打开弹出提示XSS提示框，就表示有XSS漏洞
4. 存储型XSS部分场景修复<br />使用过滤函数，过滤输入内容中的html标签等
```html
<img scr=x onerror=alert(1)>
```
<a name="VUt1Q"></a>
### B、反射型XSS

1. 反射型XSS原理<br />攻击者在url中插入XSS代码，服务端将url中的XSS代码输出到页面上，攻击者将带有XSS代码的url，想办法欺骗用户点击，用户打开后收到XSS攻击
2. 可能存在反射型XSS的功能<br />url地址中带有参数的网页，是重点关注的区域、页面的搜索框是重中之重
3. 反射型XSS漏洞挖掘方法<br />以某页面的搜索功能为例，在搜索框中输入任意字符，如“aaaaa”，然后在浏览器源码中搜索输入的 关键字，找到关键字所在的标签，使用闭合标签的方式闭合掉标签，并插入XSS代码，看是否会有XSS漏洞，如：
4. 反射型XSS部分场景的修复<br />使用函数转义输入内容中的html代码
```
url=xxxx/search.html?ks=aaaa
关键字在title中，如<title>aaaa</title>,可以将url如下闭合并测试xss
url=xxxx/search.html?ks=aaaa</title><script>alert(1)</script>

关键字在imput中，如<input placeholder="搜索" type="texg" name="ks" value="aaaa">，可以如下闭合
url=xxxx/search.html?ks=aaaa"<script>alert(1);</script>
```
<a name="F46FJ"></a>
### C、DOM型XSS
攻击者在url中插入XSS代码，前端页面直接从url中获取XSS代码输出到页面上，攻击者将带有XSS代码的url，想办法欺骗用户点击，用户打开后收到XSS攻击<br />其他和反射型类似（略）
<a name="aawOh"></a>
## 三、XSS的防御策略

---

**只要有输入数据的地方，就可能存在 XSS 危险。永远不相信用户的输入。需要对用户的输入进行处理，只允许输入合法的值，其它值一概过滤掉。**<br />**XSS防御的总体思路是：** 对输入进行过滤，对输出进行编码<br />httpOnly：在 cookie 中设置 HttpOnly 属性后，js脚本将无法读取到 cookie 信息。<br />输入过滤：一般是用于对于输入格式的检查，例如：邮箱，电话号码，用户名，密码……等，按照规定的格式输入。<br />不仅仅是前端负责，后端也要做相同的过滤检查。因为攻击者完全可以绕过正常的输入流程，直接利用相关接口向服务器发送设置。<br />转义 HTML：如果拼接 HTML 是必要的，就需要对于url中的引号，尖括号，斜杠进行转义，但这还不是很完善，想对 HTML 模板各处插入点进行充分的转义，就需要采用合适的转义库。<br />例如：`htmlspecialchars()`函数把一些预定义的字符转换为 HTML 实体
```
#预定义的字符是：
& (和号)   成为 &amp         
" (双引号) 成为 &quot           
’ (单引号) 成为 &#039                     
< (小于)   成为 &lt                    
> (大于)   成为 &gt
```
白名单：对于显示富文本来说，不能通过上面的办法来转义所有字符，因为这样会把需要的格式也过滤掉。<br />这种情况通常采用白名单过滤的办法，当然也可以通过黑名单过滤，但是考虑到需要过滤的标签和标签属性实在太多，更加推荐使用白名单的方式。
