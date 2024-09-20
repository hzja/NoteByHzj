IDEA 
<a name="HAw8j"></a>
## 1、打开IDEA中的HTTP Client
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597039031469-26a8501b-5004-4046-9343-49031c16f536.png#averageHue=%23efeeed&height=570&id=J6XH9&originHeight=1709&originWidth=1390&originalType=binary&ratio=1&rotation=0&showTitle=false&size=294079&status=done&style=shadow&title=&width=463.3333333333333)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597039137491-e64ca2a2-5d2d-46e3-9640-09c76e784ae2.png#averageHue=%23f4f4f1&height=687&id=IxfF0&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=798184&status=done&style=none&title=&width=1280)
<a name="fIK7p"></a>
## 2、构建基本的HTTP请求
<a name="sJrKz"></a>
### HTTP Client中HTTP请求的结构

- 第一行：每个HTTP请求的脚本需要###作为分隔符或者注释（每个文件中的第一个请求可以不用###来表示）
- 第二行：请求的方法类型（GET、POST、DELETE、PUT等支持Restful风格的请求）、使用空格将请求的方法类型和URL分开
- 第三行：第三行开始是请求头的设置，以key，value的格式进行设置
- 数据体Body：在请求头设置完成后隔一行设置数据体body

![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599351545573-5d23a189-2b19-4670-890b-99dc04b0a80d.png#averageHue=%23f9f9f9&height=540&id=LDl5t&originHeight=1620&originWidth=2922&originalType=binary&ratio=1&rotation=0&showTitle=false&size=491857&status=done&style=shadow&title=&width=974)

:::info
每个HTTP请求需要以###作为分割符，否则不能识别<br />正确的做法<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599287877555-c47acdd9-a3d2-47e3-b516-37970807737e.png#averageHue=%23f9f8f8&height=216&id=u1Si1&originHeight=649&originWidth=2481&originalType=binary&ratio=1&rotation=0&showTitle=false&size=165839&status=done&style=shadow&title=&width=827)
:::
:::danger
错误的做法示例<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1599287780546-f678c76b-2cb5-4a58-9cd8-a7e22f931179.png#averageHue=%23f7f7f5&height=264&id=BSkgE&originHeight=791&originWidth=2351&originalType=binary&ratio=1&rotation=0&showTitle=false&size=165921&status=done&style=shadow&title=&width=783.6666666666666)
:::
<a name="71yo8"></a>
### GET请求
```http
### GET request with a header
GET https://httpbin.org/ip
Accept: application/json
```
```http
### GET request with parameter
GET https://httpbin.org/get?show_env=1
Accept: application/json
```
```http
### GET request with environment variables
GET {{host}}/get?show_env={{show_env}}
Accept: application/json
```
```http
### GET request with disabled redirects
# @no-redirect
GET http://httpbin.org/status/301
```
```http
### GET request with dynamic variables
GET http://httpbin.org/anything?id={{$uuid}}&ts={{$timestamp}}
###
```
<a name="CFqwY"></a>
### POST请求
:::danger
请求体的Body数据需要在请求头（Request Header）下面隔一行写才能被识别
:::
<a name="8f3Bx"></a>
#### JSON格式的请求数据体
```http
POST https://httpbin.org/post
Content-Type: application/json;charset=UTF-8

{
  "id": 999,
  "value": "content"
}
```
<a name="GTdSB"></a>
#### 拼接参数的请求数据体
```http
POST https://httpbin.org/post
Content-Type: application/x-www-form-urlencoded

id=999&value=content
```
<a name="kJwoP"></a>
#### 文字或者文本文件格式的请求数据体
```http
POST https://httpbin.org/post
Content-Type: multipart/form-data; boundary=WebAppBoundary

--WebAppBoundary
Content-Disposition: form-data; name="element-name"
Content-Type: text/plain

Name
--WebAppBoundary
Content-Disposition: form-data; name="data"; filename="data.json"
Content-Type: application/json

< ./request-form-data.json
--WebAppBoundary--
```
<a name="M9h6j"></a>
#### 文件类型的请求数据体
```http
POST http://localhost:8085/wm/file/upload/OSS
Content-Type: multipart/form-data; boundary=WebAppBoundary

--WebAppBoundary
Content-Disposition: form-data; name="file"; filename="areaPoint.png"

< src/main/resources/img/areaPoint.png
--WebAppBoundary--
```
通过WebAppBoundary进行数据的绑定，通过左箭头将文件写入<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597037972031-ac5fa52e-c6f7-4405-9df8-4fea3665a16f.png#averageHue=%23f9f8f7&height=687&id=wkPaJ&originHeight=2060&originWidth=3828&originalType=binary&ratio=1&rotation=0&showTitle=false&size=912082&status=done&style=shadow&title=&width=1276)
:::danger
注意：在`Content-Disposition`字段中，文件名为中文后台接收会乱码，因为中文文件名需要通过`URLEncoder.encode("FileName", "UTF-8")`进行重新编码，而IDEA的HTTP Client未实现，仍然使用默认的编码，在后台接收时会导致乱码，需要注意。<br />可以使用22.3中的新特性，在请求前对文件名进行URL编码，然后将编码后的变量存储在变量中进行引用：
:::
```http
### 文件上传
< {%
    fileName = encodeURIComponent("中文文件名.xlsx")
    request.variables.set("fileName", fileName)
%}
POST {{basePath}}file/upload
Content-Type: multipart/form-data; boundary=WebAppBoundary

--WebAppBoundary
Content-Disposition: form-data; name="uploadFile"; filename={{fileName}}

< D:\LinkSpace\Pictures\Saved Pictures\img\中文文件名.xlsx
--WebAppBoundary--
```
:::danger
注意此时处理的后端也需要对拿到的文件名进行解码处理，在Go中解码的代码如下：`url.PathUnescape(fileHeader.Filename)`。
:::
<a name="dIpMP"></a>
## 3、配置HTTP Client的变量定义及使用
<a name="2L1J9"></a>
### ①创建变量配置文件
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597039578251-f35a2aac-cd2d-4209-919b-9a3ec9e1fa80.png#averageHue=%23f8f7f6&height=468&id=tFWyZ&originHeight=1404&originWidth=2842&originalType=binary&ratio=1&rotation=0&showTitle=false&size=447479&status=done&style=shadow&title=&width=947.3333333333334)
<a name="ETSTy"></a>
### ②在配置文件配置变量值
```json
{
  "dev": {
    "baseUrl": "http://localhost:4000"
  },
  "uat": {
    "baseUrl": "http://117.62.135.253:4000"
  }
}
```
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638022658667-e37d9e94-de75-424c-ad2a-e526f021ecbe.png#averageHue=%23fbfaf8&clientId=uca6f72c6-e117-4&from=paste&height=298&id=u6c128daa&originHeight=595&originWidth=1777&originalType=binary&ratio=1&rotation=0&showTitle=false&size=96824&status=done&style=shadow&taskId=u2b1df0c9-71c0-43d1-bc51-e50cf631057&title=&width=888.5)
<a name="d16bb27a"></a>
### ③在请求中使用变量参数值
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638022560871-cd6a2bf0-f691-477c-8d5a-d30406aefc69.png#averageHue=%23eeedea&clientId=uca6f72c6-e117-4&from=paste&height=148&id=u97675d53&originHeight=296&originWidth=1713&originalType=binary&ratio=1&rotation=0&showTitle=false&size=92663&status=done&style=shadow&taskId=u20e45b45-c11e-4fc9-9e99-a69a76c915c&title=&width=856.5)
<a name="0AA8p"></a>
### ④在数据体中使用自定义的变量
```http
### Send request with dynamic variables in request's body
POST https://httpbin.org/post
Content-Type: application/json;charset=UTF-8

{
  "id": {{$uuid}},
  "price": {{$randomInt}},
  "ts": {{$timestamp}},
  "value": "content"
}
```
<a name="AH4l1"></a>
### ⑤选择执行HTTP请求的环境
![image.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1638022742586-7306523e-4a7b-4149-91cc-85275866a246.png#averageHue=%23f8f8f7&clientId=uca6f72c6-e117-4&from=paste&height=176&id=u6edc25e4&originHeight=352&originWidth=1918&originalType=binary&ratio=1&rotation=0&showTitle=false&size=65801&status=done&style=shadow&taskId=u132ebaa5-8c1e-442e-a050-afd2b6db04e&title=&width=959)
<a name="7P9M3"></a>
## 4、添加Cookie以及Headers
<a name="6B51U"></a>
### 在当前请求直接添加
```http
POST {{baseUrl}}customerField/list
Content-Type: application/json;charset=UTF-8
Cookie: Idea-94083848=f69d9e8e-f6c8-42a1-8190-1d91c846dd3a; _ECONTRACT__USER_IDENTITY={}; Webstorm-303cb90f=9c7fe15a-d442-447e-850b-1c00f52bdf55; _ECONTRACT__USER_AVATOR=; _ECONTRACT__TOKEN=eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJnb25neWluZ3NoYW5nIiwiYXV0aCI6IuS8geS4muS_oeaBr-e7tOaKpCzkv67mlLnlr4bnoIEs5YWR5LuY6LSm5oi3566h55CGLOWNj-iurueuoeeQhizljbDnq6DnrqHnkIYs5Y-R56Wo5YiX6KGoLOWPkeelqOeuoeeQhizlt6XkvZzlj7As5byA56uL562-5pS2LOaIkeeahOWNj-iurizmiJHnmoTmlbDlrZflh63or4Es5pON5L2c5ZGY5L-h5oGv566h55CGLOaVsOWtl-WHreivgeWIl-ihqCzmlbDlrZfotYTkuqfono3otYQs5rWB6L2s566h55CGLOa2iOaBr-WIl-ihqCzmtojmga_pgJrnn6Us6J6N6LWE566h55CGLOi0puaIt-euoeeQhizotYTkuqflj5forqks6LWE5Lqn6L2s6K6pLOmqjOelqOiusOW9lSIsInVzZXJfaWQiOjE4MywiZXhwIjoxNTk1NTg2MTAxfQ.tOjiqbdy8ROzweXeM3EeyzrMfH5x-lLqFyk2pWUF3FInxeufl3GfTyKoBiK4kkE2xHgG34bNAarhxb_30SD46w; _ECONTRACT__USER_NAME=%E6%9D%9C%E5%AE%87%E9%98%B3; _ECONTRACT__USER_COMPANY_NAME=undefined; _ECONTRACT__USER_SIGN_STATUS=undefined; _ECONTRACT__USER_ACCESS=SUPPLIER; _ECONTRACT__USER_ID=183; _ECONTRACT__USER_EXECUTIVES=0; _ECONTRACT__USER_PERMISSION=247%2C248%2C252%2C253%2C254%2C257%2C264%2C267%2C275%2C296%2C297%2C299%2C300%2C302%2C303%2C304%2C305%2C306%2C307%2C308%2C329%2C340; _ECONTRACT__USER_LOGIN_NAME=gongyingshang; _ECONTRACT__USER_PERMISSION_CODE=supplier-flow-manager%2Csupplier-open-receipt-manage%2Csupplier-my-digital-assets%2Csupplier-digital-assets-list%2Csupplier-my-digital-certificates%2Csupplier-finance-manage-list%2Csupplier-digital-receive-list%2Csupplier-digital-flow-manage-list%2Csupplier-home%2Csupplier-invoice-manager%2Csupplier-invoice-manager-list%2Csupplier-invoice-inspection-record%2Csupplier-agreement-manager%2Csupplier-account-manager%2Csupplier-account-manager-info%2Csupplier-operator-manager%2Csupplier-account-password%2Csupplier-payment-account-list%2Csupplier-message-manager%2Csupplier-message-list%2Csupplier-sign-agreement-list%2Csupplier-seal-detail; _FINCHAIN__USER_IDENTITY={}; _FINCHAIN__USER_AVATOR=; _FINCHAIN__TOKEN=eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJzeXN0ZW0iLCJhdXRoIjoi5LyB5Lia5YiX6KGoLOS_oeaBr-WuoeaguCzlhZHku5jliJ3lrqEs5YWR5LuY5aSN5a6hLOWNj-iuruWIl-ihqCzljY_orq7nrqHnkIYs5Y2w56ug5a6h5qC4LOWNsOeroOeuoeeQhizlt6XkvZzlj7As5bmz5Y-w5Y2P6K6uLOW5s-WPsOi0puWPt-euoeeQhizlvIDnq4vliJ3lrqEs5byA56uL5aSN5a6hLOW8gOeri-mineW6puWkjeaguCzlvIDnq4vpop3luqborr7nva4s5o6I5p2D566h55CGLOaTjeS9nOWRmOS_oeaBr-euoeeQhizmlbDlrZflh63or4HnrqHnkIYs5paw5aKe6LSm5Y-3LOaXpeW_l-WIl-ihqCzml6Xlv5fnrqHnkIYs5raI5oGvLOa2iOaBr-mAmuefpSzoj5zljZXnrqHnkIYs6KeS6Imy566h55CGLOi9rOiuqeWuoeaguCIsInVzZXJfaWQiOjEsImFwcF9pZCI6MSwidXNlcl91dWlkIjoiMjNjd2R2YyIsImV4cCI6MTU5NTkxMzM1MX0.d__DVU-0o2rmW3-tJ4frBI_sOo2JDhmlOn_8t9PdXYTRzsJOfUgJG14jBdkkMQvTf2_FaAGEu95JiLgVmMdTiQ; _FINCHAIN__USER_COMPANY_NAME=undefined; _FINCHAIN__USER_NAME=%E6%8E%A7%E5%88%B6%E5%8F%B0%E7%AE%A1%E7%90%86%E5%91%98; _FINCHAIN__USER_SIGN_STATUS=undefined; _FINCHAIN__USER_ID=1; _FINCHAIN__USER_ACCESS=PLATFORM; _FINCHAIN__USER_EXECUTIVES=1; _FINCHAIN__USER_PERMISSION=24%2C28%2C29%2C220%2C222%2C223%2C230%2C236%2C241%2C242%2C243%2C258%2C259%2C260%2C261%2C268%2C269%2C274%2C314%2C323%2C324%2C325%2C326%2C327%2C342%2C343; _FINCHAIN__USER_LOGIN_NAME=system; _FINCHAIN__USER_PERMISSION_CODE=menu-manager%2Cgrant-manager%2Crole-manager%2Cplatform-company-manager%2Cplatform-company-add%2Cplatform-agreement-manager%2Cplatform-company-list%2Cplatform-agreement-manager-list%2Cplatform-digital-voucher-manage%2Cplatform-digital-open-trial-manage%2Cplatform-digital-open-recheck-manage%2Cplatform-operation-log-manager%2Cplatform-operation-log-list%2Cplatform-message-manager%2Cplatform-message-list%2Cplatform-digital-pay-trial-list%2Cplatform-digital-pay-recheck-list%2Cplatform-home%2Cplatform-operator-manager%09%2Cplatform-digital-check-list%2Cplatform-quota-setting-list%2Cplatform-quota-check-list%2Cplatform-company-info-check%2Cplatform-sign-agreement-list%2Cplatform-seal-detail%2Cplatform-seal-list; Idea-94083849=62fa8c78-3253-4503-8405-0d8dc9b40bff; hasGreet=1; user_avator=; user_access=; user_company_name=[]; USER_EXECUTIVES=; user_permission=; token=eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJhZG1pbiIsImF1dGgiOiJubyBBdXRoZW50aWNhdGlvbiIsInVzZXJfaWQiOjgsImNvbXBhbnlfaWQiOjEsImV4cCI6MTU5OTYxODY3OX0.WV4ONi2bHpYj3tXovsUOrPPAZvC7HyIxK3jcs80tEGhMMe1ye0GR0HQLguxRfdo3wKmxKK6UQPdTae-JX21uGg; user_identity={%22id%22:8%2C%22userNo%22:%226%22%2C%22userName%22:%22admin%22%2C%22password%22:%22$2a$10$kkIIxpqLxyrjebEwnhhvfeW686FUzTEQDaE9RLWL8AaTN8AREXytW%22%2C%22salt%22:%22%E7%9B%90%22%2C%22birthday%22:1514822400000%2C%22name%22:%22%E8%B6%85%E7%BA%A7%E7%AE%A1%E7%90%86%E5%91%98%22%2C%22sex%22:1%2C%22certificate%22:%22%22%2C%22idNo%22:%22320581198911011234%22%2C%22wxId%22:%22321321321312321132123%22%2C%22mobNo%22:%2218000000000%22%2C%22email%22:%2211%22%2C%22speciality%22:%221%22%2C%22profession%22:2%2C%22education%22:4%2C%22photo%22:null%2C%22profile%22:%225%22%2C%22identity%22:1%2C%22leaveTime%22:1527651409000%2C%22entryTime%22:1527651464000%2C%22state%22:1%2C%22isDeleted%22:0%2C%22createTime%22:%222018-05-31%2011:52:54%22%2C%22updateTime%22:%222020-07-29%2015:43:09%22%2C%22userResult%22:1%2C%22comId%22:3%2C%22openId%22:%22255554445454%22%2C%22createBy%22:null%2C%22updateBy%22:null%2C%22version%22:null%2C%22status%22:null%2C%22type%22:%22%22%2C%22loginName%22:%22admin%22%2C%22roles%22:null%2C%22roleIds%22:[10]%2C%22permissionList%22:[]%2C%22companyNames%22:null%2C%22roleNames%22:null}; user_name=%E8%B6%85%E7%BA%A7%E7%AE%A1%E7%90%86%E5%91%98; user_id=8

{
"type": 1
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597041206278-44d1b64c-47e4-4f06-9f27-02ae13d5bd52.png#averageHue=%23f6f5f2&height=190&id=lEEQg&originHeight=570&originWidth=2823&originalType=binary&ratio=1&rotation=0&showTitle=false&size=194754&status=done&style=shadow&title=&width=941)
<a name="YJDJC"></a>
### 在配置文件创建变量进行引用
<a name="NqXiz"></a>
#### ①创建私有变量配置文件
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597041483558-782ebd04-b886-488c-b3c4-3463af348d81.png#averageHue=%23f7f6f4&height=293&id=ROoAd&originHeight=880&originWidth=2831&originalType=binary&ratio=1&rotation=0&showTitle=false&size=313158&status=done&style=shadow&title=&width=943.6666666666666)
<a name="MnXoF"></a>
#### ②创建Cookie或者session、Token的私有变量的值
```json
{
  "dev": {
    "Cookie": "Idea-94083848=f69d9e8e-f6c8-42a1-8190-1d91c846dd3a; _ECONTRACT__USER_IDENTITY={}; Webstorm-303cb90f=9c7fe15a-d442-447e-850b-1c00f52bdf55; _ECONTRACT__USER_AVATOR=; _ECONTRACT__TOKEN=eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJnb25neWluZ3NoYW5nIiwiYXV0aCI6IuS8geS4muS_oeaBr-e7tOaKpCzkv67mlLnlr4bnoIEs5YWR5LuY6LSm5oi3566h55CGLOWNj-iurueuoeeQhizljbDnq6DnrqHnkIYs5Y-R56Wo5YiX6KGoLOWPkeelqOeuoeeQhizlt6XkvZzlj7As5byA56uL562-5pS2LOaIkeeahOWNj-iurizmiJHnmoTmlbDlrZflh63or4Es5pON5L2c5ZGY5L-h5oGv566h55CGLOaVsOWtl-WHreivgeWIl-ihqCzmlbDlrZfotYTkuqfono3otYQs5rWB6L2s566h55CGLOa2iOaBr-WIl-ihqCzmtojmga_pgJrnn6Us6J6N6LWE566h55CGLOi0puaIt-euoeeQhizotYTkuqflj5forqks6LWE5Lqn6L2s6K6pLOmqjOelqOiusOW9lSIsInVzZXJfaWQiOjE4MywiZXhwIjoxNTk1NTg2MTAxfQ.tOjiqbdy8ROzweXeM3EeyzrMfH5x-lLqFyk2pWUF3FInxeufl3GfTyKoBiK4kkE2xHgG34bNAarhxb_30SD46w; _ECONTRACT__USER_NAME=%E6%9D%9C%E5%AE%87%E9%98%B3; _ECONTRACT__USER_COMPANY_NAME=undefined; _ECONTRACT__USER_SIGN_STATUS=undefined; _ECONTRACT__USER_ACCESS=SUPPLIER; _ECONTRACT__USER_ID=183; _ECONTRACT__USER_EXECUTIVES=0; _ECONTRACT__USER_PERMISSION=247%2C248%2C252%2C253%2C254%2C257%2C264%2C267%2C275%2C296%2C297%2C299%2C300%2C302%2C303%2C304%2C305%2C306%2C307%2C308%2C329%2C340; _ECONTRACT__USER_LOGIN_NAME=gongyingshang; _ECONTRACT__USER_PERMISSION_CODE=supplier-flow-manager%2Csupplier-open-receipt-manage%2Csupplier-my-digital-assets%2Csupplier-digital-assets-list%2Csupplier-my-digital-certificates%2Csupplier-finance-manage-list%2Csupplier-digital-receive-list%2Csupplier-digital-flow-manage-list%2Csupplier-home%2Csupplier-invoice-manager%2Csupplier-invoice-manager-list%2Csupplier-invoice-inspection-record%2Csupplier-agreement-manager%2Csupplier-account-manager%2Csupplier-account-manager-info%2Csupplier-operator-manager%2Csupplier-account-password%2Csupplier-payment-account-list%2Csupplier-message-manager%2Csupplier-message-list%2Csupplier-sign-agreement-list%2Csupplier-seal-detail; _FINCHAIN__USER_IDENTITY={}; _FINCHAIN__USER_AVATOR=; _FINCHAIN__TOKEN=eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJzeXN0ZW0iLCJhdXRoIjoi5LyB5Lia5YiX6KGoLOS_oeaBr-WuoeaguCzlhZHku5jliJ3lrqEs5YWR5LuY5aSN5a6hLOWNj-iuruWIl-ihqCzljY_orq7nrqHnkIYs5Y2w56ug5a6h5qC4LOWNsOeroOeuoeeQhizlt6XkvZzlj7As5bmz5Y-w5Y2P6K6uLOW5s-WPsOi0puWPt-euoeeQhizlvIDnq4vliJ3lrqEs5byA56uL5aSN5a6hLOW8gOeri-mineW6puWkjeaguCzlvIDnq4vpop3luqborr7nva4s5o6I5p2D566h55CGLOaTjeS9nOWRmOS_oeaBr-euoeeQhizmlbDlrZflh63or4HnrqHnkIYs5paw5aKe6LSm5Y-3LOaXpeW_l-WIl-ihqCzml6Xlv5fnrqHnkIYs5raI5oGvLOa2iOaBr-mAmuefpSzoj5zljZXnrqHnkIYs6KeS6Imy566h55CGLOi9rOiuqeWuoeaguCIsInVzZXJfaWQiOjEsImFwcF9pZCI6MSwidXNlcl91dWlkIjoiMjNjd2R2YyIsImV4cCI6MTU5NTkxMzM1MX0.d__DVU-0o2rmW3-tJ4frBI_sOo2JDhmlOn_8t9PdXYTRzsJOfUgJG14jBdkkMQvTf2_FaAGEu95JiLgVmMdTiQ; _FINCHAIN__USER_COMPANY_NAME=undefined; _FINCHAIN__USER_NAME=%E6%8E%A7%E5%88%B6%E5%8F%B0%E7%AE%A1%E7%90%86%E5%91%98; _FINCHAIN__USER_SIGN_STATUS=undefined; _FINCHAIN__USER_ID=1; _FINCHAIN__USER_ACCESS=PLATFORM; _FINCHAIN__USER_EXECUTIVES=1; _FINCHAIN__USER_PERMISSION=24%2C28%2C29%2C220%2C222%2C223%2C230%2C236%2C241%2C242%2C243%2C258%2C259%2C260%2C261%2C268%2C269%2C274%2C314%2C323%2C324%2C325%2C326%2C327%2C342%2C343; _FINCHAIN__USER_LOGIN_NAME=system; _FINCHAIN__USER_PERMISSION_CODE=menu-manager%2Cgrant-manager%2Crole-manager%2Cplatform-company-manager%2Cplatform-company-add%2Cplatform-agreement-manager%2Cplatform-company-list%2Cplatform-agreement-manager-list%2Cplatform-digital-voucher-manage%2Cplatform-digital-open-trial-manage%2Cplatform-digital-open-recheck-manage%2Cplatform-operation-log-manager%2Cplatform-operation-log-list%2Cplatform-message-manager%2Cplatform-message-list%2Cplatform-digital-pay-trial-list%2Cplatform-digital-pay-recheck-list%2Cplatform-home%2Cplatform-operator-manager%09%2Cplatform-digital-check-list%2Cplatform-quota-setting-list%2Cplatform-quota-check-list%2Cplatform-company-info-check%2Cplatform-sign-agreement-list%2Cplatform-seal-detail%2Cplatform-seal-list; Idea-94083849=62fa8c78-3253-4503-8405-0d8dc9b40bff; hasGreet=1; user_avator=; user_access=; user_company_name=[]; USER_EXECUTIVES=; user_permission=; token=eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJhZG1pbiIsImF1dGgiOiJubyBBdXRoZW50aWNhdGlvbiIsInVzZXJfaWQiOjgsImNvbXBhbnlfaWQiOjEsImV4cCI6MTU5OTYxODY3OX0.WV4ONi2bHpYj3tXovsUOrPPAZvC7HyIxK3jcs80tEGhMMe1ye0GR0HQLguxRfdo3wKmxKK6UQPdTae-JX21uGg; user_identity={%22id%22:8%2C%22userNo%22:%226%22%2C%22userName%22:%22admin%22%2C%22password%22:%22$2a$10$kkIIxpqLxyrjebEwnhhvfeW686FUzTEQDaE9RLWL8AaTN8AREXytW%22%2C%22salt%22:%22%E7%9B%90%22%2C%22birthday%22:1514822400000%2C%22name%22:%22%E8%B6%85%E7%BA%A7%E7%AE%A1%E7%90%86%E5%91%98%22%2C%22sex%22:1%2C%22certificate%22:%22%22%2C%22idNo%22:%22320581198911011234%22%2C%22wxId%22:%22321321321312321132123%22%2C%22mobNo%22:%2218000000000%22%2C%22email%22:%2211%22%2C%22speciality%22:%221%22%2C%22profession%22:2%2C%22education%22:4%2C%22photo%22:null%2C%22profile%22:%225%22%2C%22identity%22:1%2C%22leaveTime%22:1527651409000%2C%22entryTime%22:1527651464000%2C%22state%22:1%2C%22isDeleted%22:0%2C%22createTime%22:%222018-05-31%2011:52:54%22%2C%22updateTime%22:%222020-07-29%2015:43:09%22%2C%22userResult%22:1%2C%22comId%22:3%2C%22openId%22:%22255554445454%22%2C%22createBy%22:null%2C%22updateBy%22:null%2C%22version%22:null%2C%22status%22:null%2C%22type%22:%22%22%2C%22loginName%22:%22admin%22%2C%22roles%22:null%2C%22roleIds%22:[10]%2C%22permissionList%22:[]%2C%22companyNames%22:null%2C%22roleNames%22:null}; user_name=%E8%B6%85%E7%BA%A7%E7%AE%A1%E7%90%86%E5%91%98; user_id=8"
  }
}
```
<a name="LaAIY"></a>
#### ③在文件中通过变量值动态引用
```json
POST {{baseUrl}}customerField/list
Content-Type: application/json;charset=UTF-8
Cookie: {{Cookie}}

{
"type": 1
}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1597041727617-f641871b-d5cf-40d9-9942-7612f6055596.png#averageHue=%23f7f7f6&height=201&id=NYzNy&originHeight=603&originWidth=2836&originalType=binary&ratio=1&rotation=0&showTitle=false&size=171928&status=done&style=shadow&title=&width=945.3333333333334)
<a name="lIcq0"></a>
## 5、设置全局值
例：获取到Token后设置到Client的全局变量，通过请求获取Token将其通过箭头符重定向写入全局设置
```http
POST {{baseUrl}}/authenticate
Content-Type: application/json;charset=UTF-8

{
"username": "admin",
"password": "admin"
}
> {% client.global.set("auth_token", response.body.data) %}
```
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1598969553666-9ae93534-70ec-47dc-9ae2-598c3303652b.png#averageHue=%23f9f8f7&height=687&id=aQbHh&originHeight=2060&originWidth=3829&originalType=binary&ratio=1&rotation=0&showTitle=false&size=752791&status=done&style=shadow&title=&width=1276.3333333333333)<br />然后在后续的请求中通过变量引用的方式使用该值即可
```http
POST {{baseUrl}}/profile
Content-Type: application/json;charset=UTF-8
X-ACCESS-TOKEN: {{auth_token}}

{
	"userId":"1"
}
```
<a name="V5ZGe"></a>
## 6、取消自动重定向操作
只需要在请求上面添加`_# _**_@no-redirect_**`就行
```http
### 获取用户信息
# @no-redirect
GET  {{bashUrl}}user
```
<a name="7ad2e192"></a>
## 6、结果断言
```http
GET {{baseUrl}}/status/200

> {%
client.test("Request executed successfully", function() {
  client.assert(response.status === 200, "Response status is not 200");
});
%}
```
<a name="LvQRs"></a>
## 22.3新特性
<a name="L4tGn"></a>
### 新的动态变量
```http
### New 'random.*' and 'isoTimestamp' variables
POST https://httpbin.org/post
Content-Type: application/json

{
    "login": "{{$random.alphabetic(10)}}",
    "password": "{{$random.alphanumeric(8)}}",
    "email": "{{$random.email}}",
    "score": "{{$random.float(-10.2, 12.3)}}",
    "id": "{{$random.uuid}}",
    "attachment": "{{$random.hexadecimal(30)}}",
    "phone_number": "+{{$random.integer(100)}} ({{$random.integer(100, 999)}}) {{$random.integer(100, 999)}} {{$random.integer(10, 99)}}-{{$random.integer(10, 99)}}",
    "timestamp": "{{$isoTimestamp}}"
}
```
<a name="FQuBX"></a>
### 请求前置处理
```http
# @name Retrieve request components
< {%
    client.log("Accessing request structure!")

    client.log(`Raw url: ${request.url.getRaw()}`);
    client.log(`Substituted url: ${request.url.tryGetSubstituted()}`);
    for (let header of request.headers.all()) {
        client.log(`${header.name}: ${header.getRawValue()}, ${header.tryGetSubstitutedValue()}`)
    }
    client.log(`Raw body:\n${request.body.getRaw()}`);
    client.log(`Substituted:\n${request.body.tryGetSubstituted()}`);
%}
POST https://httpbin.org/post?requestId={{$random.integer(10, 1000)}}
X-My-Header-1: Header-Value; id={{$random.hexadecimal(10)}}
Content-Type: application/json

{
  "name": "{{$random.alphabetic(10)}}",
  "transaction_time": "{{$isoTimestamp}}"
}

```
<a name="QwcYY"></a>
### 初始化请求前处理程序脚本中的请求变量
```http
< {%
    client.log(request.url.tryGetSubstituted())
    request.variables.set("var-from-script", "my-value-from-script!")

    client.log("Request variable value = " + request.variables.get("var-from-script"))
%}
GET https://httpbin.org/get?q1={{$isoTimestamp}}&q2={{var-from-script}}
```
<a name="TX0WX"></a>
### 预请求处理程序脚本的加密API
```http
# @name Constructing HMAC and Hash from request body
< {%
    const signature = crypto.hmac.sha256()
        .withTextSecret(request.environment.get("secret")) // get variable from http-client.private.env.json
        .updateWithText(request.body.tryGetSubstituted())
        .digest().toHex();
    request.variables.set("signature", signature)

    const hash = crypto.sha256()
        .updateWithText(request.body.tryGetSubstituted())
        .digest().toHex();
    request.variables.set("hash", hash)
%}
POST https://httpbin.org/post
X-My-Signature: {{signature}}
X-My-Hash: {{hash}}
Content-Type: application/json

{
    "prop": "value"
}
```
<a name="qzm6Z"></a>
### 响应处理程序脚本中的更新
```http
# @name Request object and environment variables in response handler script
< {%
    request.variables.set("var-from-pre-script", "variable_value!!!")
%}
POST https://httpbin.org/post
Content-Type: application/json

{
    "prop": "value"
}

> {%
    client.log(request.environment.get("show_env")); // retrieve variable from `http-client.env.json`
    client.log(request.variables.get("var-from-pre-script"));

    client.log(request.url())
    for (let header of request.headers.all()) {
        client.log(header.name + ": " + header.value());
    }
    client.log(request.body())
%}
```
