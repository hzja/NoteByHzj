Nginx
<a name="rllVZ"></a>
## 分析前准备：
前端网站地址：http://localhost:8080<br />服务端网址：http://localhost:59200<br />首先保证服务端是没有处理跨域的，其次，先用postman测试服务端接口是正常的<br />![2022-03-29-13-54-04-193817.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1648534053056-5f187f13-304b-4d3e-a590-e43990c81d82.png#clientId=u7c9496db-3e96-4&from=ui&id=ay2gk&originHeight=487&originWidth=760&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1112716&status=done&style=shadow&taskId=u4be660df-12d1-4984-b0c3-8954bac8e1e&title=)<br />当网站8080去访问服务端接口时，就产生了跨域问题，那么如何解决？接下来把跨域遇到的各种情况都列举出来并通过nginx代理的方式解决（后台也是一样的，只要理解的原理）。<br />跨域主要涉及4个响应头：

- `Access-Control-Allow-Origin` 用于设置允许跨域请求源地址 （预检请求和正式请求在跨域时候都会验证）
- `Access-Control-Allow-Headers` 跨域允许携带的特殊头信息字段 （只在预检请求验证）
- `Access-Control-Allow-Methods` 跨域允许的请求方法或者说HTTP动词 （只在预检请求验证）
- `Access-Control-Allow-Credentials` 是否允许跨域使用cookies，如果要跨域使用cookies，可以添加上此请求响应头，值设为true（设置或者不设置，都不会影响请求发送，只会影响在跨域时候是否要携带cookies，但是如果设置，预检请求和正式请求都需要设置）。不过不建议跨域使用（项目中用到过，不过不稳定，有些浏览器带不过去），除非必要，因为有很多方案可以代替。

网上很多文章都是说直接Nginx添加这几个响应头信息就能解决跨域，当然大部分情况是能解决，但是还是有很多情况，明明配置上了，也同样会报跨域问题。
<a name="peY28"></a>
### 什么是预检请求？
当发生跨域条件时候，览器先询问服务器，当前网页所在的域名是否在服务器的许可名单之中，以及可以使用哪些HTTP动词和头信息字段。只有得到肯定答复，浏览器才会发出正式的`XMLHttpRequest`请求，否则就报错。
<a name="CrmwF"></a>
## 开始动手模拟：
Nginx代理端口：22222，配置如下
```nginx
server {
    listen       22222;
    server_name  localhost;
    location  / {
        proxy_pass  http://localhost:59200;
    }
}
```
测试代理是否成功，通过Nginx代理端口2222再次访问接口，可以看到如下图通过代理后接口也是能正常访问<br />![2022-03-29-13-54-04-299575.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1648534088509-d05890a0-b42c-4dfa-83ba-e29bbc181b7d.png#clientId=u7c9496db-3e96-4&from=ui&id=GHi6b&originHeight=386&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1253124&status=done&style=shadow&taskId=uc64508e1-c962-41dd-b475-200a307676b&title=)<br />接下来开始用网站8080访问Nginx代理后的接口地址，报错情况如下↓↓↓
<a name="oVkfN"></a>
### 情况1：
Access to XMLHttpRequest at 'http://localhost:22222/api/Login/TestGet' from origin 'http://localhost:8080' has been blocked by CORS policy: Response to preflight request doesn't pass access control check: No 'Access-Control-Allow-Origin' header is present on the requested resource.<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648533094844-cd894798-41d3-4a61-ad1e-cfbea39bcb28.png#clientId=u7c9496db-3e96-4&from=paste&id=u1f9e0503&originHeight=33&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u91aa0a4c-b770-462e-9176-7fe69a65b74&title=)<br />通过错误信息可以很清晰的定位到错误（注意看标红部分）priflight说明是个预请求，CORS 机制跨域会首先进行 preflight（一个 OPTIONS 请求）， 该请求成功后才会发送真正的请求。这一设计旨在确保服务器对 CORS 标准知情，以保护不支持 CORS 的旧服务器<br />通过错误信息，可以得到是预检请求的请求响应头缺少了 `Access-Control-Allow-Origin`，错哪里，改哪里就好了。修改Nginx配置信息如下（红色部分为添加部分），缺什么就补什么，很简单明了
```nginx
server {
    listen       22222;
    server_name  localhost;
    location  / {
       add_header Access-Control-Allow-Origin 'http://localhost:8080';
       proxy_pass  http://localhost:59200;
    }
}
```
当以为能解决后，发现还是报了同样的问题<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648533094837-97df1dc7-dc8d-47ff-8041-041138051f48.png#clientId=u7c9496db-3e96-4&from=paste&id=u6411aa2b&originHeight=33&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=ub964df8a-29b6-4cf8-954c-ab9d1500071&title=)<br />不过配置没什么问题，问题在Nginx，下图链接[http://nginx.org/en/docs/http/ngx_http_headers_module.html](http://nginx.org/en/docs/http/ngx_http_headers_module.html)<br />![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1648534007388-1279c16f-71e4-4ca9-8a57-d17ae7f457ff.png#clientId=u7c9496db-3e96-4&from=paste&height=356&id=u981544d8&originHeight=891&originWidth=2046&originalType=binary&ratio=1&rotation=0&showTitle=false&size=129204&status=done&style=shadow&taskId=u2b7343c1-6d20-48d1-b1e1-a36257ba5d6&title=&width=818.4)<br />add_header 指令用于添加返回头字段，当且仅当状态码为图中列出的那些时有效。如果想要每次响应信息都携带头字段信息，需要在最后添加always（经测试，只有`Access-Control-Allow-Origin`这个头信息需要加always，其他的不加always也会携带回来），加上试试
```nginx
server {
    listen       22222;
    server_name  localhost;
    location  / {
       add_header Access-Control-Allow-Origin 'http://localhost:8080' always;
       proxy_pass  http://localhost:59200;
    }
}
```
修改了配置后，发现生效了，当然不是跨域就解决了，是上面这个问题已经解决了，因为报错内容已经变了。
<a name="yg4sV"></a>
### 情况2：
Access to XMLHttpRequest at 'http://localhost:22222/api/Login/TestGet' from origin 'http://localhost:8080' has been blocked by CORS policy: Response to preflight request doesn't pass access control check: It does not have HTTP ok status.<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648533095166-3562dd75-479a-4977-b584-9fadca94df5e.png#clientId=u7c9496db-3e96-4&from=paste&id=u0b1683e1&originHeight=39&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u57025032-e32e-436e-a59d-88b0bcd9df6&title=)<br />通过报错信息提示可以得知，是跨域浏览器默认行为的预请求（option请求）没有收到ok状态码，此时再修改配置文件，当请求为option请求时候，给浏览器返回一个状态码（一般是204）
```nginx
server {
    listen       22222;
    server_name  localhost;
    location  / {
       add_header Access-Control-Allow-Origin 'http://localhost:8080' always;
       if ($request_method = 'OPTIONS') {
            return 204;
       }
       proxy_pass  http://localhost:59200;
    }
}
```
当配置完后，发现报错信息变了
<a name="Dty3d"></a>
### 情况3：
Access to XMLHttpRequest at 'http://localhost:22222/api/Login/TestGet' from origin 'http://localhost:8080' has been blocked by CORS policy: Request header field authorization is not allowed by Access-Control-Allow-Headers in preflight response.<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648533095218-5b320398-16dc-41ab-86af-1c70dd7ff065.png#clientId=u7c9496db-3e96-4&from=paste&id=u87ab124b&originHeight=35&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u41016faf-20dc-4514-9806-8e202872d4c&title=)<br />意思就是预请求响应头`Access-Control-Allow-Headers`中缺少头信息authorization（各种情况会不一样，在发生跨域后，在自定义添加的头信息是不允许的，需要添加到请求响应头`Access-Control-Allow-Headers`中，以便浏览器知道此头信息的携带是服务器承认合法的，这里携带的是authorization，其他的可能是token之类的，缺什么加什么），知道了问题所在，然后修改配置文件，添加对应缺少的部分，再试试
```nginx
server {
	listen       22222;
	server_name  localhost;
	location  / {
		add_header Access-Control-Allow-Origin 'http://localhost:8080' always;
		if ($request_method = 'OPTIONS') {
			add_header Access-Control-Allow-Headers 'authorization'; #为什么写在if里面而不是接着Access-Control-Allow-Origin往下写？因为这里只有预检请求才会检查
			return 204;
		}
		proxy_pass http://localhost:59200;
	}
}
```
此时发现报错问题又回到了情况1<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648533095234-58dd6a46-2b37-4688-888d-ae1697157e4d.png#clientId=u7c9496db-3e96-4&from=paste&id=u23a83d57&originHeight=38&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4dd93682-adfd-47fd-97ad-cfb56c9e1f0&title=)<br />经测试验证，只要`if ($request_method = 'OPTIONS')` 里面写了 add_header ，当为预检请求时外部配置的都会失效，为什么？↓↓。<br />官方文档是这样说的：
> There could be several add_header directives. These directives are inherited from the previous level if and only if there are no add_header directives defined on the current level.

意思就是当前层级无 add_header 指令时，则继承上一层级的add_header。相反的若当前层级有了add_header，就应该无法继承上一层的add_header。<br />![2022-03-29-13-54-04-689401.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1648533730527-d56214c1-4485-4e12-a6b2-925466a31a43.png#clientId=u7c9496db-3e96-4&from=ui&id=u491fc2ff&originHeight=482&originWidth=864&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1251924&status=done&style=shadow&taskId=u8e3ee3ab-f3c3-408f-9a97-f2bfaa267f3&title=)<br />配置修改如下：
```nginx
server {
    listen       22222;
    server_name  localhost;
    location  / {
        add_header Access-Control-Allow-Origin 'http://localhost:8080' always;
        if ($request_method = 'OPTIONS') {
            add_header Access-Control-Allow-Origin 'http://localhost:8080';
            add_header Access-Control-Allow-Headers 'authorization';
            return 204;
        }
        proxy_pass  http://localhost:59200;
    }
}
```
_此时改完发现跨域问题已经解决了，_<br />![2022-03-29-13-54-04-750988.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1648533699876-17e56aaa-f8de-4722-8aa4-914ef8b8e2de.png#clientId=u7c9496db-3e96-4&from=ui&id=u2126bd05&originHeight=441&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1431668&status=done&style=shadow&taskId=uccdb9869-93e4-43e7-8968-200d60f54ba&title=)<br />不过以上虽然解决了跨域问题，但是考虑后期可能Nginx版本更新,不知道这个规则会不会被修改，考虑到这样的写法可能会携带上两个 `Access-Control-Allow-Origin` ，这种情况也是不允许的，下面会说到。所以配置适当修改如下：
```nginx
server {
    listen       22222;
    server_name  localhost;
    location  / {
        if ($request_method = 'OPTIONS') {
            add_header Access-Control-Allow-Origin 'http://localhost:8080';
            add_header Access-Control-Allow-Headers 'authorization';
            return 204;
        }
        if ($request_method != 'OPTIONS') {
            add_header Access-Control-Allow-Origin 'http://localhost:8080' always;
        }
        proxy_pass  http://localhost:59200;
    }
}
```
还没完，继续看
<a name="KmtyQ"></a>
### 情况4：
比较早期的API可能只用到了POST和GET请求，而`Access-Control-Allow-Methods`这个请求响应头跨域默认只支持POST和GET，当出现其他请求类型时候，同样会出现跨域异常。<br />比如，这里将请求的API接口请求方式从原来的GET改成PUT，在发起一次试试。在控制台上会抛出错误：<br />Access to XMLHttpRequest at 'http://localhost:22222/api/Login/TestGet' from origin 'http://localhost:8080' has been blocked by CORS policy: Method PUT is not allowed by Access-Control-Allow-Methods in preflight response.<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648533095487-39af335e-f4b3-4930-9b23-997bd2cbd527.png#clientId=u7c9496db-3e96-4&from=paste&id=u0b732cf5&originHeight=36&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=shadow&taskId=uf40e8e17-7f1a-44ce-abe1-09b74038888&title=)<br />报错内容也讲的很清楚，在这个预请求中，PUT方法是不允许在跨域中使用的，需要改下`Access-Control-Allow-Methods`的配置(缺什么加上么，这里只加了PUT，可以自己加全一点)，让浏览器知道服务端是允许的
```nginx
server {
    listen 22222;
    server_name localhost;
    location / {
        if ($request_method = 'OPTIONS') {
            add_header Access-Control-Allow-Origin 'http://localhost:8080';
            add_header Access-Control-Allow-Headers 'content-type,authorization';
            add_header Access-Control-Allow-Methods 'PUT';#为这么只加在这个if中，不再下面的if也加上？因为这里只有预检请求会校验，当然你加上也没事。
            return 204;
        }
        if ($request_method != 'OPTIONS') {
            add_header Access-Control-Allow-Origin 'http://localhost:8080' always;
        }
        proxy_pass http://localhost:59200;
    }
}
```
这里注意一下，改成PUT类型后，`Access-Control-Allow-Headers`请求响应头又会自动校验content-type这个请求头，和情况3是一样的，缺啥补啥就行了。如果不加上content-type，则会报如下错误。（想简单的话，`Access-Control-Allow-Headers`和`Access-Control-Allow-Methods`可以设置为 `*`，表示全都匹配。但是`Access-Control-Allow-Origin`就不建议设置成 `*` 了，为了安全考虑，限制域名是很有必要的。）<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648533095558-74879bbe-2124-419f-9d6e-1c92ebaf9058.png#clientId=u7c9496db-3e96-4&from=paste&id=uf9f2deb3&originHeight=35&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u60f8cd65-4c50-482b-97b3-d3ed41d3c93&title=)<br />都加上后，问题就解决了，这里报405是服务端这个接口只开放了GET，没有开放PUT，而此刻将此接口用PUT方法去请求，所以接口会返回这个状态码。<br />![2022-03-29-13-54-04-957894.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1648533540842-74c12179-64ee-42f7-9408-2f39252e9727.png#clientId=u7c9496db-3e96-4&from=ui&id=u8748b86a&originHeight=381&originWidth=1080&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1236890&status=done&style=shadow&taskId=u4c733744-6ed9-4ebf-9a8f-9b7cdc1583a&title=)
<a name="OOysQ"></a>
### 情况5：
最后再说一种情况，就是后端处理了跨域，就不需要自己在处理了（这里吐槽下，某些后端工程师自己改服务端代码解决跨域，但是又不理解其中原理，网上随便找段代码黏贴，导致响应信息可能处理不完全，如method没添加全，headers没加到点上，自己用的那个可能复制过来的并不包含实际项目所用到的，没有添加options请求返回状态码等，导致Nginx再用通用的配置就会可能报以下异常）<br />Access to XMLHttpRequest at 'http://localhost:22222/api/Login/TestGet' from origin 'http://localhost:8080' has been blocked by CORS policy: The 'Access-Control-Allow-Origin' header contains multiple values '*, http://localhost:8080', but only one is allowed.<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1648533095673-02e3f369-4553-4a02-9851-654b5b65e8f0.png#clientId=u7c9496db-3e96-4&from=paste&id=u9f97e793&originHeight=33&originWidth=1080&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u17d4705c-984d-4df7-b080-af70bc56bb8&title=)<br />![2022-03-29-13-54-05-055050.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1648533450366-b37478bc-5568-4087-98a5-93559493895a.png#clientId=u7c9496db-3e96-4&from=ui&id=ua7fc68c4&originHeight=422&originWidth=1078&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1367451&status=done&style=none&taskId=u9c6bedad-e292-4628-87ce-0a00193f46e&title=)<br />意思就是此刻`Access-Control-Allow-Origin`请求响应头返回了多个，而只允许有一个，这种情况当然修改配置去掉`Access-Control-Allow-Origin`这个配置就可以了，不过遇到这种情况，建议Nginx配置和服务端自己解决跨域只选其一。（这里注意如果按上面的写法，`if $request_method = 'OPTIONS'` 这个里面的`Access-Control-Allow-Origin`可不能删除，删除`!='OPTIONS'`里面的就好了，因为这里如果是预检请求直接就ruturn了，请求不会再转发到59200服务，如果也删除了，就会报和情况1一样的错误。所以为什么说要不服务端代码层面解决跨域，要不就Nginx代理解决，不要混着搞，不然不明白原理的人，网上找一段代码贴就很可能解决不了问题）<br />↓ ↓ ↓ ↓ ↓<br />再贴一份完整配置（`*`号根据自己‘喜好’填写）：
```nginx
server {
    listen       22222;
    server_name  localhost;
    location  / {
        if ($request_method = 'OPTIONS') {
            add_header Access-Control-Allow-Origin 'http://localhost:8080';
            add_header Access-Control-Allow-Headers '*';
            add_header Access-Control-Allow-Methods '*';
            add_header Access-Control-Allow-Credentials 'true';
            return 204;
        }
        if ($request_method != 'OPTIONS') {
            add_header Access-Control-Allow-Origin 'http://localhost:8080' always;
            add_header Access-Control-Allow-Credentials 'true';
        }
        proxy_pass  http://localhost:59200;
    }
}
```
或者：
```nginx
server {
    listen       22222;
    server_name  localhost;
    location  / {
        add_header Access-Control-Allow-Origin 'http://localhost:8080' always;
        add_header Access-Control-Allow-Headers '*';
        add_header Access-Control-Allow-Methods '*';
        add_header Access-Control-Allow-Credentials 'true';
        if ($request_method = 'OPTIONS') {
            return 204;
        }
        proxy_pass  http://localhost:59200;
    }
}
```
