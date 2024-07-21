<a name="I8x4c"></a>
## 问题原因
Nginx默认会把header中带`_`的参数给过滤掉。比如请求头中有一个TOKEN_HEADER，那么它会把这个忽略掉，导致后台获取不到这个请求头。
<a name="M8prt"></a>
## 解决方案
解决方法主要有两个：<br />（1）、把 `_` 改为 - 或者不要 `_`。（显然这种方式不是一劳永逸的方式）<br />（2）、在Nginx中http模块中配置。（**重点：以下配置缺一不可**）
```nginx
http {
    underscores_in_headers on;
    server {
        # 重写请求头部host字段
        proxy_set_header Host $host;
        # 重写来源IP
        proxy_set_header X-Real-IP $remote_addr;
        # 重写http请求来源
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    }
}
```
