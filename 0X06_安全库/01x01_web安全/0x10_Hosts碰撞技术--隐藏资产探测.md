[https://github.com/fofapro/Hosts_scan](https://github.com/fofapro/Hosts_scan)  用来Hosts  + 域名进行碰撞的脚本<br />[https://mp.weixin.qq.com/s/Yn2UDQtARaLZVLltQ4QjZw](https://mp.weixin.qq.com/s/Yn2UDQtARaLZVLltQ4QjZw)



直接访问ip，资产无法访问，但是可以用域名请求却能返回正常的业务系统，因为这大多数都是需要绑定host才能正常请求访问的。直接指定HOST头为某个域名时访问该IP回显正常时，可判断可进行HOSTS碰撞。 当一些域名只允许在内网访问时(通过oneforall，或者其他一些子域名爆破的工具能解析出内网ip的，把内ip收集起来)，可通过这种碰撞如果突破了边界限制就可以访问到内网系统进行下一步渗透测试。

爆出来后访问浏览器，需要在本地的/etc/hosts做hosts文件。才能在服务器访问

![image.png](https://cdn.nlark.com/yuque/0/2021/png/1345801/1621699225855-bcf8466a-c2ed-43ce-a9a0-97159a8dfc73.png#height=315&id=MgrhR&originHeight=420&originWidth=615&originalType=binary&ratio=1&rotation=0&showTitle=false&size=53504&status=done&style=none&title=&width=461)
