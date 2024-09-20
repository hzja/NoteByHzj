# Redis安装

## 1. 安装Redis

~~~ shell
wget http://download.redis.io/releases/redis-4.0.0.tar.gz
tar xvf redis-4.0.0.tar.gz
yum install gcc -y
cd redis-4.0.0
make && make install PREFIX=/usr/local/redis
cp redis.conf /usr/local/redis/bin/
cd /usr/local/redis/bin/

vim redis.conf
# 设置后台启动
daemonize yes
# 绑定IP，之后可以使用远程连接
bind 192.168.0.12
# 设置数据库的个数，可以使用SELECT <dbid>命令来切换数据库。默认16
databases 16
# 密码
requirepass 123456
~~~



## 2. 启动和关闭

~~~ shell
# 启动
./redis-server redis.conf

# 关闭
./redis-cli shutdown
netstat -anplt | grep redis
~~~

### redis 可执行文件

| 文件            | 文件作用        |
| --------------- | --------------- |
| redis-benchmark | 性能测试        |
| redis-check-aof | 检查修复aof文件 |
| redis-check-rdb | 检查修复rdb文件 |
| redis-cli       | redis客户端     |
| redis-sentinel  | redis集群       |
| redis-server    | redis服务端     |
| redis-trib.rb   | 集群管理        |



## 3. 连接Redis

~~~ shell
[root@redis1 bin]# /redis-cli -h 127.0.0.1 -p 6379 -a 123456
192.168.0.12:6379> select 15
OK
~~~



## 4. 命令

~~~ shell
key
    keys * 获取所有的key
    select 0 选择第一个库
    move myString 1 将当前的数据库key移动到某个数据库,目标库有，则不能移动
    flush db      清除指定库
    randomkey     随机key
    type key      类型
    
    set key1 value1 设置key
    get key1    获取key
    mset key1 value1 key2 value2 key3 value3
    mget key1 key2 key3
    del key1   删除key
    exists key      判断是否存在key
    expire key 10   10过期
    pexpire key 1000 毫秒
    persist key     删除过期时间

string
    set name cxx 为一个 key 设置 value，可以配合 ex/px 参数指定key 的有效期，通过nx/xx 参数对key 是否存在的情况进行区别操作，时间复杂度为 O(1)
    get name 获取 key 对应的 value 时间复杂度为 O(1)
    getrange name 0 -1        字符串分段
    getset name new_cxx       为一个key 设置value 并返回key 的原 value ，时间复杂度为 O(1)
    mset key1 key2            为 多个 key 设置 value ，时间复杂度为 O(n)
    mget key1 key2            批量获取
    setnx key value           不存在就插入（not exists）
    setex key time value      过期时间（expire）
    setrange key index value  从index开始替换value
    incr age        递增
    incrby age 10   递增
    decr age        递减
    decrby age 10   递减
    incrbyfloat     增减浮点数
    append          追加
    strlen          长度
    
hash
    hset myhash name cxx 将key 对应的 Hash 中的 field 设置为 value ，如果该 hash 不存在，则会自动创建一个 时间复杂度为 O(1)
    hget myhash name 返回指定Hash 中 field 字段的值，时间复杂度为 O(1)
    hmset myhash name cxx age 25 note "i am notes" 和 hset 和 hget 类似，可以批量操作同一个key 下的多个 field，jedis 中你可以 使用一个key 存储一个 map对象进去，其时间复杂度是 O(n),n为一次操作的 field 的数量
    hmget myhash name age note   
    hgetall myhash               返回指定hash中所有的 field-value，返回结果是数组，数组中field 和value 交替出现，O(n) 复杂度
    hexists myhash name          是否存在 field，存在返回 -1 ，不存在返回0 O(1)复杂度
    hsetnx myhash score 100      如果 field 存在，则不会进行任何的操作，O(1) 复杂度
    hincrby myhash id 1          对指定Hash 中的 field 进行 incrby 操作，O(1) 复杂度
    hdel myhash name             删除指定 hash 中的 field （一个或者 多个），时间复杂度为O(n),n为操作的 field 数量
    hkeys myhash                 只取field，返回指定 Hash 中 所有的 field/value O(n) 复杂度
    hvals myhash                 只取value，返回指定 Hash 中 所有的 field/value O(n) 复杂度
    hlen myhash                  长度

list
    lpush mylist a b c  左插入，时间复杂度为 O(n)
    rpush mylist x y z  右插入，时间复杂度为 O(n)
    lrange mylist 0 -1  返回指定 list 中 指定范围的元素，时间复杂度为 O(n), 这个命令要控制获取 list 元素的个数，太大会导致 redis 延迟响应，另外对于不可预知长度的list 禁止 lrange key 0 -1 这样的遍历操作
    lpop mylist  左弹元素，时间复杂度为 O(1)
    rpop mylist  右弹元素，时间复杂度为 O(1)
    llen mylist  长度，时间复杂度为 O(1)
    lrem mylist count value  删除
    lindex mylist 2          指定索引的值
    lset mylist 2 n          索引设值
    ltrim mylist 0 4         删除key
    linsert mylist before a  插入
    linsert mylist after a   插入
    rpoplpush list list2     转移列表的数据
    
set
    sadd myset redis 向指定的 set 中添加一个或者多个 member ，如果指定的set 不存在，自会自动创建一个，时间复杂度为 O(n),n 为添加的个数
    smembers myset       数据集合
    srem myset set1   从指定的 set 移除 1 个或者多个 member，时间复杂度为O(n), n为返回的member个数
    sismember myset set1 判断指定的 value 是否存在于指定的 set 中，时间复杂度为O(1)
    scard key_name  返回指定 set 中 member的格式 ，时间复杂度为O(1)
    sdiff | sinter | sunion 操作：集合间运算：差集 | 交集 | 并集
    srandmember   从指定的set 随机返回 1 个或者多个 member ，时间复杂度为 O(n), n为返回的member 的个数
    spop 从指定的set 移除并返回 n个 member 时间复杂度为 O(n)，n为移除的member的个数
    
zset
    zadd zset 1 one
    zadd zset 2 two
    zadd zset 3 three
    zincrby zset 1 one              增长分数
    zscore zset two                 获取分数
    zrange zset 0 -1 withscores     范围值
    zrangebyscore zset 10 25 withscores 指定范围的值
    zrangebyscore zset 10 25 withscores limit 1 2 分页
    Zrevrangebyscore zset 10 25 withscores  指定范围的值
    zcard zset  元素数量
    Zcount zset 获得指定分数范围内的元素个数
    Zrem zset one two        删除一个或多个元素
    Zremrangebyrank zset 0 1  按照排名范围删除元素
    Zremrangebyscore zset 0 1 按照分数范围删除元素
    Zrank zset 0 -1    分数最小的元素排名为0
    Zrevrank zset 0 -1  分数最大的元素排名为0
    Zinterstore
    zunionstore rank:last_week 7 rank:20150323 rank:20150324 rank:20150325  weights 1 1 1 1 1 1 1
~~~



