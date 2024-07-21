# lua

> jianshu.com/p/fec18f59ff8f



**使用lua脚本好处**

- 主要优势： 减少网络开销：多个请求通过脚本一次发送，减少网络延迟
- 原子操作：将脚本作为一个整体执行，中间不会插入其他命令，无需使用事务
- 复用：客户端发送的脚本永久存在redis中，其他客户端可以复用脚本
- 可嵌入性：可嵌入JAVA，C#等多种编程语言，支持不同操作系统跨平台交互



~~~ shell
// KEYS[1]: 锁key
// ARGV[1]: 锁value,随机字符串
// ARGV[2]: 过期时间

[root@server script]# cat set.lua 
return redis.call('SET', KEYS[1], ARGV[1], 'NX', 'EX', ARGV[2])

[root@server script]# redis-cli --eval set.lua age,18 60  // 会报格式问题
(error) ERR Error running script (call to f_1ee8eca25a0c9466ea9bddd4c8d50569d5c49565): 
@user_script:1: @user_script: 1: Lua redis() command arguments must be strings or integers 

[root@server script]# redis-cli --eval set.lua age , 18 60 // 正确写法
OK

[root@server script]# redis-cli --eval set.lua age , 18 60 // setnx语法在前一个没过期之前，不能覆盖写入
(nil)

[root@server script]# echo "get age" | redis-cli
"18"
~~~

