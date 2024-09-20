# 操作Redis

## 1.introduction



## 2. 发布订阅(pub/sub)

一种消息通信模式：发送者(pub)发送消息，订阅者(sub)接收消息。

![1578903708950-c5cda789-23a3-4886-8a96-94ee4cc38184](D:\Note\python\数据库交互\图片\1578903708950-c5cda789-23a3-4886-8a96-94ee4cc38184.png)

> 订阅一个或多个频道 `SUBSCRIBE ``channel [channel ...]` 
> 退订一个或多个频道 `UNSUBSCRIBE [channel [channel ...]] ` [Redis Unsubscribe 命令 | 菜鸟教程 (runoob.com)](https://www.runoob.com/redis/pub-sub-unsubscribe.html)
>
> 发布消息到频道中 `PUBLISH channel message`[Redis Publish 命令 | 菜鸟教程 (runoob.com)](https://www.runoob.com/redis/pub-sub-publish.html)



# Python

## install redis module

指定国内清华镜像地址

~~~ bash
sudo pip install -i https://pypi.tuna.tsinghua.edu.cn/simple redis
~~~



## 获取redis连接池中的连接

~~~ python
import redis
import threading


def connect():
    """
    获取 Redis 连接
    :return: Redis Client
    """
    pool = redis.ConnectionPool(host='39.96.215.69',
                                port=6379,
                                db=14,
                                password="qwer1011",
                                socket_timeout=None,
                                socket_connect_timeout=None)
    return redis.Redis(connection_pool=pool)
~~~



## 普通 set/get/delete

~~~ python
redis.set("a", "b")
_str = redis.get('a')
redis.delete("a")
~~~



## 订阅端

~~~ python
def pub_sub(redis_client: redis, channel: str):
    """
    订阅
    :param redis_client: 客户端
    :param channel: 频道名称
    :return:
    """
    pubsub = redis_client.pubsub()
    pubsub.subscribe(channel)
    for item in pubsub.listen():
        print("正在监听频道 : %s " % item['channel'].decode())
        if item['type'] == 'message':
            data = item['data'].decode()
            print("从频道 %s 获取的消息 : %s" % (item['channel'].decode(), item['data'].decode()))
            if item['data'] == 'over':
                print(item['channel'].decode(), '停止发布')
                break
    pubsub.unsubscribe('botConfig')
    print("取消订阅")
    pass
~~~



## 发布端

~~~ python
def pub(redis_client: redis, channel: str):
    """
    发布
    :param channel: 频道名称
    :param redis_client: 发布客户端
    :return:
    """
    while True:
        msg = input("publish: >>")
        if msg == "over":
            print("停止发布")
            break
        redis_client.publish(channel, msg)
    pass
~~~



## 主方法

~~~ python
if __name__ == "__main__":
    redis = connect()
    # 启用线程
    t1 = threading.Thread(target=pub_sub, args=(redis, "botConfig"))
    t1.start()
    pub(redis, "botConfig")
    # pub_sub(redis, "botConfig")
~~~

