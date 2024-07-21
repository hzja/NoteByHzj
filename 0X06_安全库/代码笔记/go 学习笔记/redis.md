redis和传统的mysql数据库不一样，不需要定义表结构，只需要key-value存储就行
<a name="p1q0X"></a>
## redis基本操作
**左设置list值：lpush list名 value1 v2 v3…**<br />**获取list的长度：llen list名**<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1695311553978-3109baaf-a356-4215-814d-bab1190f1224.png#averageHue=%2303285a&clientId=u3896a96c-b9c7-4&from=paste&height=75&id=uf98d3cb9&originHeight=94&originWidth=471&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=8936&status=done&style=none&taskId=ue94ed0b9-06de-4606-81a5-72bf370e856&title=&width=376.8)

**查看列表的数据：lrange test_redis 0 1**<br />**查看所有的列表：keys ***<br />![image.png](https://cdn.nlark.com/yuque/0/2023/png/1345801/1695311618027-fa126fc7-12bb-4c94-94f2-7b9960c0c476.png#averageHue=%2304285b&clientId=u3896a96c-b9c7-4&from=paste&height=121&id=u1fd56da3&originHeight=151&originWidth=398&originalType=binary&ratio=1.25&rotation=0&showTitle=false&size=11249&status=done&style=none&taskId=ub2ec0899-8e2d-40c1-8476-59730118f2f&title=&width=318.4)

**查看redis数据库设置的密码：**config get requirepass<br />**设置reids数据库密码：**config set requirepass 密码

Redis中存储数据的默认时间是没有限制的，这意味着存储的数据可以永久存在，只要不删除。但是，在实际应用中，大多数数据都是具有时效性的，因此需要设置数据存储的时间。

<a name="jTB2S"></a>
## 使用redis分布式操作
在下述代码示例中，我们使用了RPush存入任务，BLPop函数从队列中取出任务。这个函数会阻塞，直到有新的任务到来。当消费者接收到任务时，它会开始扫描IP。
```
package main

import (
	"fmt"
	"github.com/go-redis/redis"
	"log"
)

func main() {
	// 连接到Redis服务器
	client := redis.NewClient(&redis.Options{
		Addr:     "localhost:6379",
		Password: "qweasdzxc", // no password set
		DB:       0,           // use default DB
	})

	// 添加任务到列表
	err := client.RPush("tasks", "121.5.96.6").Err()
	if err != nil {
		log.Fatal(err)
	}
	pop()
}
func pop() {
	// 连接到Redis服务器
	client := redis.NewClient(&redis.Options{
		Addr:     "localhost:6379",
		Password: "qweasdzxc", // no password set
		DB:       0,           // use default DB
	})

	for {
		// 从队列中取出任务
		task, err := client.BLPop(0, "tasks").Result()
		if err != nil {
			log.Fatal(err)
		}

		// 处理任务
		ip := task[1]
		// 扫描IP
		// ...
		fmt.Println(ip)
	}
}

```

