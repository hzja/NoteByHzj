# Django使用线程和协程

## Django使用线程处理任务

看到django异步大家的反应应该是celery这种消息队列组件，现在用的最多的最推荐的也是这种方式。然而我这需求就是请求来了，执行一个小程序，但是又不能确定这个小程序啥时候执行完，响应又要及时，丢给队列处理当然可以，但是为了简单，决定直接起个线程跑跑。 （当然这只是实验，应用规模也很小，如果并发高，会有很多问题）



~~~ python
from django.shortcuts import render, redirect
from django.views import View
from django.http import JsonResponse, HttpResponse


##通过thread 实现django中
import threading
import time
class PrintThread(threading.Thread):
    def run(self):
        print("start.... %s"%(self.getName(),))
        print(threading.enumerate())
        for i in range(30):
            time.sleep(1)
            print(i)
        print("end.... %s"%(self.getName(),))


class Personal_Assets_View(View):
    def get(self, request, *args, **kwargs):
        prints = PrintThread()
        prints.start()
        print("我返回了")
        return HttpResponse('aaa')
~~~

![image2](D:\Note\python\Django\图片\image2.png)





## Django使用协程创建数据

首先创建一个Django，必要的表，此处省略

- 在MySQL添加数据：大概是10万条数据

~~~ sql
delimiter $$
create procedure ttst()
begin
declare i int default 0;
while i < 1000000 do
insert into api_data (name) values("nameJams");
set i = i + 1;
end while;
end $$
delimiter ;
call ttst();
~~~

这样执行完`ttst()` 执行时间如下：

![1644171-20200221231326121-1416091036](D:\Note\python\Django\图片\1644171-20200221231326121-1416091036.png)



- 删除存储过程函数

~~~sql
drop FUNCTION if exists insertSomeData;#insertSomeData函数名   删除存储过程函数
drop PROCEDURE if exists insertSomeData;# 删除存储过程
~~~



- 定义查询存入数据的存储过程

~~~sql
# 指定界定符
delimiter $$
#创建存储过程
create procedure selectCountData()
begin
# 执行sql语句
select count(*) from api_data;
end $$
# 重置界定符
delimiter ;
# 通过call 调用存储过程
call selectCountData();
~~~

- 这样我们在调用`call selectCountData();`可以查看数据多少条。

- django代码实现：

~~~python
import time
import asyncio
from django.db import connection


class getData(APIView):
    def get(self,request,*args,**kwargs):
        start = time.time()
        count = 100000
        # 创建一个新的事件循环
        loop = asyncio.new_event_loop()
        # 将 loop 设置为当前 OS 线程的当前事件循环。
        asyncio.set_event_loop(loop)
        self.loop = loop
        try:
            # 将任务对象注册到事件循环队列中并且开启了事件循环
            results = loop.run_until_complete(self.gather_tasks(list(range(count))))
        finally:
            loop.close()
        print(results)
        end = time.time()
        return Response({"code":200,"count":len(results),"msg":"获取数据成功","time":end-start})
    
    async def gather_tasks(self,limit_list):
        # 创建task任务
        task = (self.select_data(self.current_sql,item) for item in limit_list)
        # 接受task任务
        results = await asyncio.gather(*task)
        return results
    
    async def select_data(self, func, *args):
        # 可以是  ThreadPoolExecutor / ProcessPool  , 如果是None 则使用默认线程池
        future = self.loop.run_in_executor(None, func, *args)
        response = await future
        return response
    
    def current_sql(self, size_list):
        # Lock.acquire()
        with connection.cursor() as cursor:
            # 此处对于分页查询，进行优化 where id > xxx 可以缩小范围
            res = cursor.execute("INSERT INTO api_data (name) VALUES({})".format(size_list))
        return res
~~~



- 执行插入十万条数据，返回结果如下：

![1644171-20200221231344184-1889808614](D:\Note\python\Django\图片\1644171-20200221231344184-1889808614.png)

可以看到，执行时间1分半,比4分钟少了2分半。