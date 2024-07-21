搭建一个jsp之后，弄一段代码<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1678068222087-60cccbdd-4549-4da5-84a4-4aa025a210a7.png#averageHue=%23f6eecb&clientId=u6017c325-944d-4&from=paste&height=341&id=u55e25388&originHeight=426&originWidth=1181&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=56728&status=done&style=none&taskId=u2fc80b95-1548-4684-9c39-b6ff585f9d8&title=&width=944.8)

go.jsp
```
<%
    String keykey = request.getParameter("keykey");

    out.println("getContextPath:"+request.getContextPath()+"<br>");
    out.println("getServletPath:"+request.getServletPath()+"<br>");
    out.println("getRequestURI:"+request.getRequestURI()+"<br>");
    out.println("getRequestURL:"+request.getRequestURL()+"<br>");
    out.println("getQueryString:"+request.getQueryString()+"<br>");

%>
```

访问如下<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1678068192298-ac79f4d4-7dc7-40a9-86af-fe3f0a8e7915.png#averageHue=%23d4ab76&clientId=u6017c325-944d-4&from=paste&height=166&id=u23c6f11f&originHeight=208&originWidth=967&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=36640&status=done&style=none&taskId=uc711fa36-744b-436d-9d0d-ccb27bdd545&title=&width=773.6)
```
getContextPath，获取当前的相对路径,不带/;
getServletPath，获取当前文件名称
getRequestURI，获取当前url的路径文件，不加参数，带/;
getRequestURL，全路径加域名，不带参数。
getQueryString，获取参数
```
<a name="TCj2I"></a>
## Tomcat中的URL解析
随便写一个jsp页面，test.jsp访问如下<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1678068561213-dbc18e7e-d160-4248-9e9b-ab9cf5cd4f04.png#averageHue=%23f7f7f7&clientId=u6017c325-944d-4&from=paste&height=442&id=u2bf00ce2&originHeight=552&originWidth=1221&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=30556&status=done&style=none&taskId=uc8646f38-eabf-42b7-9c06-ad612340e7f&title=&width=976.8)
<a name="hwm9a"></a>
### 使用./进行访问
![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1678068592447-15d1d461-5766-4447-baf6-657bcb8b34a9.png#averageHue=%23f8f8f8&clientId=u6017c325-944d-4&from=paste&height=509&id=u03a1f9cb&originHeight=636&originWidth=1226&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=31966&status=done&style=none&taskId=u90009b16-fc35-44e1-bbaa-2cd8af7c333&title=&width=980.8)

<a name="BxLtH"></a>
### 使用/xxx/../进行访问
![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1678068619503-64894178-dcda-41c6-b411-3649b8b0c770.png#averageHue=%23f7f6f6&clientId=u6017c325-944d-4&from=paste&height=432&id=u9ec7a477&originHeight=540&originWidth=1185&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=29975&status=done&style=none&taskId=u34d81cbd-b3dd-4ef5-b1b2-fec66510562&title=&width=948)

<a name="nU2PG"></a>
### 使用/;/进行访问
![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1678068664421-aca99d92-de95-442c-9479-4c65aa1ec0e5.png#averageHue=%23f6f6f6&clientId=u6017c325-944d-4&from=paste&height=363&id=ud262289c&originHeight=454&originWidth=1155&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=24546&status=done&style=none&taskId=uc01a06e3-8fd3-4e21-8db2-2933a0a4ec4&title=&width=924)

<a name="yQATN"></a>
### 使用/;asdwd/进行访问
![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1678068690739-45920073-a661-4718-8ee0-7d49439836b0.png#averageHue=%23f7f6f6&clientId=u6017c325-944d-4&from=paste&height=412&id=u027227ab&originHeight=515&originWidth=1188&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=28746&status=done&style=none&taskId=u6c8997b7-40ef-4977-872c-40c8b3fcf5f&title=&width=950.4)
<a name="tvhRZ"></a>
### 使用/xxx/..;/进行访问
![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1678068731615-2248767c-977b-4a7b-b2a0-f436c6ec2573.png#averageHue=%23f6f6f6&clientId=u6017c325-944d-4&from=paste&height=372&id=ucfc991b8&originHeight=465&originWidth=1161&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=25276&status=done&style=none&taskId=u12c32672-45f0-49b4-bde3-446dc469709&title=&width=928.8)


根据以上测试<br />Tomcat中的URL解析是支持嵌入“./”、“../”、“;/”、“;xx/”、“..;/”、“//”进行访问的，所以getRequestURL()和getRequestURI()这两个函数是可以进行一些权限绕过的操作<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1678068338328-f1eacfed-3113-4e64-bcab-6646a70b7780.png#averageHue=%23faf9f9&clientId=u6017c325-944d-4&from=paste&height=369&id=u027d0cba&originHeight=461&originWidth=887&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=23902&status=done&style=none&taskId=u13028617-8b56-4ee8-b089-3430e85ecf6&title=&width=709.6)

那么当碰到一些白名单路径的时候，可以进行/abc/xxx/..;//////;/test.jsp绕过<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1678081698137-62cdf64e-e14e-4daa-bb3e-0483d712a2ba.png#averageHue=%23f8f7f7&clientId=u82cad2b7-d4b8-4&from=paste&height=476&id=u0de4f56c&originHeight=595&originWidth=1238&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=31880&status=done&style=none&taskId=ud61071f8-3591-4ca4-8fb2-8d1c0964d10&title=&width=990.4)

例如下述代码
```
String uri = request.getRequestURI();
if(uri.startsWith("/abc/xxx")) {
    //登陆接口设置白名单
    filterChain.doFilter(request, response);
}
```
