Python<br />当做一个 Web 服务时，性能的瓶颈绝大部分都在数据库上，如果一个请求从数据库中读数据的时候能够自动切换、去处理其它请求的话，是不是就能提高并发量了呢。<br />下面来看看如何使用 Python 异步操作 MySQL、PostgreSQL 以及 Redis，以上几个可以说是最常用的数据库了。至于 SQLServer、Oracle，本人没有找到相应的异步驱动，有兴趣可以自己去探索一下。<br />而操作数据库无非就是增删改查，下面来看看如何异步实现它们。
<a name="TRM05"></a>
## **异步操作 MySQL**
异步操作 MySQL 的话，需要使用一个 aiomysql，直接 `pip install aiomysql` 即可。<br />aiomysql 底层依赖于 pymysql，所以 aiomysql 并没有单独实现相应的连接驱动，而是在 pymysql 之上进行了封装。
<a name="pwHmO"></a>
### 查询记录
下面先来看看如何查询记录。
```python
import asyncio
import aiomysql.sa as aio_sa


async def main():
    # 创建一个异步引擎
    engine = await aio_sa.create_engine(host="xx.xxx.xx.xxx",
                                        port=3306,
                                        user="root",
                                        password="root",
                                        db="_hanser",
                                        connect_timeout=10)

    # 通过 engine.acquire() 获取一个连接
    async with engine.acquire() as conn:
        # 异步执行, 返回一个 <class 'aiomysql.sa.result.ResultProxy'> 对象
        result = await conn.execute("SELECT * FROM girl")
        # 通过 await result.fetchone() 可以获取满足条件的第一条记录, 一个 <class 'aiomysql.sa.result.RowProxy'> 对象
        data = await result.fetchone()

        # 可以将 <class 'aiomysql.sa.result.RowProxy'> 对象想象成一个字典
        print(data.keys())  # KeysView((1, '古明地觉', 16, '地灵殿'))
        print(list(data.keys()))  # ['id', 'name', 'age', 'place']

        print(data.values())  # ValuesView((1, '古明地觉', 16, '地灵殿'))
        print(list(data.values()))  # [1, '古明地觉', 16, '地灵殿']

        print(data.items())  # ItemsView((1, '古明地觉', 16, '地灵殿'))
        print(list(data.items()))  # [('id', 1), ('name', '古明地觉'), ('age', 16), ('place', '地灵殿')]

        # 直接转成字典也是可以的
        print(dict(data))  # {'id': 1, 'name': '古明地觉', 'age': 16, 'place': '地灵殿'}
    
    # 最后别忘记关闭引擎, 当然你在创建引擎的时候也可以通过 async with aio_sa.create_engine 的方式创建
    # async with 语句结束后会自动执行下面两行代码
    engine.close()
    await engine.wait_closed()
    

loop = asyncio.get_event_loop()
loop.run_until_complete(main())
loop.close()
```
怎么样，是不是很简单呢，和同步库的操作方式其实是类似的。但是很明显，在获取记录的时候不会只获取一条，而是会获取多条，获取多条的话使用 `await result.fetchall()` 即可。
```python
import asyncio
from pprint import pprint
import aiomysql.sa as aio_sa


async def main():
    # 通过异步上下文管理器的方式创建, 会自动帮我们关闭引擎
    async with aio_sa.create_engine(host="xx.xxx.xx.xxx",
                                    port=3306,
                                    user="root",
                                    password="root",
                                    db="_hanser",
                                    connect_timeout=10) as engine:
        async with engine.acquire() as conn:
            result = await conn.execute("SELECT * FROM girl")
            # 此时的 data 是一个列表, 列表里面是 <class 'aiomysql.sa.result.RowProxy'> 对象
            data = await result.fetchall()
            # 将里面的元素转成字典
            pprint(list(map(dict, data)))
            """
            [{'age': 16, 'id': 1, 'name': '古明地觉', 'place': '地灵殿'},
             {'age': 16, 'id': 2, 'name': '雾雨魔理沙', 'place': '魔法森林'},
             {'age': 400, 'id': 3, 'name': '芙兰朵露', 'place': '红魔馆'}]
            """


loop = asyncio.get_event_loop()
loop.run_until_complete(main())
loop.close()
```
除了 `fetchone`、`fetchall` 之外，还有一个 `fetchmany`，可以获取指定记录的条数。
```python
import asyncio
from pprint import pprint
import aiomysql.sa as aio_sa


async def main():
    # 通过异步上下文管理器的方式创建, 会自动帮我们关闭引擎
    async with aio_sa.create_engine(host="xx.xxx.xx.xxx",
                                    port=3306,
                                    user="root",
                                    password="root",
                                    db="_hanser",
                                    connect_timeout=10) as engine:
        async with engine.acquire() as conn:
            result = await conn.execute("SELECT * FROM girl")
            # 默认是获取一条, 得到的仍然是一个列表
            data = await result.fetchmany(2)
            pprint(list(map(dict, data)))
            """
            [{'age': 16, 'id': 1, 'name': '古明地觉', 'place': '地灵殿'},
             {'age': 16, 'id': 2, 'name': '雾雨魔理沙', 'place': '魔法森林'}]
            """


loop = asyncio.get_event_loop()
loop.run_until_complete(main())
loop.close()
```
以上就是通过 aiomysql 查询数据库中的记录，没什么难度。但是值得一提的是，`await conn.execute` 里面除了可以传递一个原生的 SQL 语句之外，还可以借助 SQLAlchemy。
```python
import asyncio
from pprint import pprint
import aiomysql.sa as aio_sa
from sqlalchemy.sql.selectable import Select
from sqlalchemy import text


async def main():
    async with aio_sa.create_engine(host="xx.xxx.xx.xxx",
                                    port=3306,
                                    user="root",
                                    password="root",
                                    db="_hanser",
                                    connect_timeout=10) as engine:
        async with engine.acquire() as conn:
            sql = Select([text("id, name, place")], whereclause=text("id != 1"), from_obj=text("girl"))
            result = await conn.execute(sql)
            data = await result.fetchall()
            pprint(list(map(dict, data)))
            """
            [{'id': 2, 'name': '雾雨魔理沙', 'place': '魔法森林'},
             {'id': 3, 'name': '芙兰朵露', 'place': '红魔馆'}]
            """


loop = asyncio.get_event_loop()
loop.run_until_complete(main())
loop.close()
```
<a name="UP4O0"></a>
### 添加记录
然后是添加记录，同样可以借助 SQLAlchemy 拼接 SQL 语句。
```python
import asyncio
from pprint import pprint
import aiomysql.sa as aio_sa
from sqlalchemy import Table, MetaData, create_engine


async def main():
    async with aio_sa.create_engine(host="xx.xx.xx.xxx",
                                    port=3306,
                                    user="root",
                                    password="root",
                                    db="_hanser",
                                    connect_timeout=10) as engine:
        async with engine.acquire() as conn:
            # 我们还需要创建一个 SQLAlchemy 中的引擎, 然后将表反射出来
            s_engine = create_engine("mysql+pymysql://root:root@xx.xx.xx.xxx:3306/_hanser")
            tbl = Table("girl", MetaData(bind=s_engine), autoload=True)

            insert_sql = tbl.insert().values(
                [{"name": "十六夜咲夜", "age": 17, "place": "红魔馆"},
                 {"name": "琪露诺", "age": 60, "place": "雾之湖"}])

            # 注意: 执行的执行必须开启一个事务, 否则数据是不会进入到数据库中的
            async with conn.begin():
                # 同样会返回一个 <class 'aiomysql.sa.result.ResultProxy'> 对象
                # 尽管我们插入了多条, 但只会返回最后一条的插入信息
                result = await conn.execute(insert_sql)
                # 返回最后一条记录的自增 id
                print(result.lastrowid)
                # 影响的行数
                print(result.rowcount)
        
        # 重新查询, 看看记录是否进入到数据库中
        async with engine.acquire() as conn:
            data = await (await conn.execute("select * from girl")).fetchall()
            data = list(map(dict, data))
            pprint(data)
            """
            [{'age': 16, 'id': 1, 'name': '古明地觉', 'place': '地灵殿'},
             {'age': 16, 'id': 2, 'name': '雾雨魔理沙', 'place': '魔法森林'},
             {'age': 400, 'id': 3, 'name': '芙兰朵露', 'place': '红魔馆'},
             {'age': 17, 'id': 16, 'name': '十六夜咲夜', 'place': '红魔馆'},
             {'age': 60, 'id': 17, 'name': '琪露诺', 'place': '雾之湖'}]
            """


loop = asyncio.get_event_loop()
loop.run_until_complete(main())
loop.close()
```
还是很方便的，但是插入多条记录的话只会返回插入的最后一条记录的信息，所以如果希望获取每一条的信息，那么就一条一条插入。
<a name="zdWmw"></a>
### 修改记录
修改记录和添加记录是类似的，来看一下。
```python
import asyncio
from pprint import pprint
import aiomysql.sa as aio_sa
from sqlalchemy import Table, MetaData, create_engine, text


async def main():
    async with aio_sa.create_engine(host="xx.xx.xx.xxx",
                                    port=3306,
                                    user="root",
                                    password="root",
                                    db="_hanser",
                                    connect_timeout=10) as engine:
        async with engine.acquire() as conn:
            s_engine = create_engine("mysql+pymysql://root:root@xx.xx.xx.xxx:3306/_hanser")
            tbl = Table("girl", MetaData(bind=s_engine), autoload=True)
            update_sql = tbl.update().where(text("name = '古明地觉'")).values({"place": "东方地灵殿"})

            # 同样需要开启一个事务
            async with conn.begin():
                result = await conn.execute(update_sql)
                print(result.lastrowid)  # 0
                print(result.rowcount)   # 1
  
        # 查询结果
        async with engine.acquire() as conn:
            data = await (await conn.execute("select * from girl where name = '古明地觉'")).fetchall()
            data = list(map(dict, data))
            pprint(data)
            """
            [{'age': 16, 'id': 1, 'name': '古明地觉', 'place': '东方地灵殿'}]
            """


loop = asyncio.get_event_loop()
loop.run_until_complete(main())
loop.close()
```
可以看到，记录被成功的修改了。
<a name="TTyvc"></a>
### 删除记录
删除记录就更简单了，直接看代码。
```python
import asyncio
import aiomysql.sa as aio_sa
from sqlalchemy import Table, MetaData, create_engine, text


async def main():
    async with aio_sa.create_engine(host="xx.xx.xx.xxx",
                                    port=3306,
                                    user="root",
                                    password="root",
                                    db="_hanser",
                                    connect_timeout=10) as engine:
        async with engine.acquire() as conn:
            s_engine = create_engine("mysql+pymysql://root:root@xx.xx.xx.xxx:3306/_hanser")
            tbl = Table("girl", MetaData(bind=s_engine), autoload=True)
            update_sql = tbl.delete()  # 全部删除

            # 同样需要开启一个事务
            async with conn.begin():
                result = await conn.execute(update_sql)
                # 返回最后一条记录的自增 id, 我们之前修改了 id = 0 记录, 所以它跑到最后了
                print(result.lastrowid)  # 0
                # 受影响的行数
                print(result.rowcount)   # 6


loop = asyncio.get_event_loop()
loop.run_until_complete(main())
loop.close()
```
此时数据库中的记录已经全部被删除了。<br />整体来看还是比较简单的，并且支持的功能也比较全面。
<a name="TKkUo"></a>
## **异步操作 PostgreSQL**
异步操作 PostgreSQL 的话，我们有两个选择，一个是 asyncpg 库，另一个是 aiopg 库。<br />asyncpg 是自己实现了一套连接驱动，而 aiopg 则是对 psycopg2 进行了封装，个人更推荐 asyncpg，性能和活跃度都比 aiopg 要好。<br />下面来看看如何使用 asyncpg，首先是安装，直接 `pip install asyncpg` 即可。
<a name="aIYXg"></a>
### 查询记录
首先是查询记录。
```python
import asyncio
from pprint import pprint
import asyncpg

async def main():
    # 创建连接数据库的驱动
    conn = await asyncpg.connect(host="localhost",
                                 port=5432,
                                 user="postgres",
                                 password="zg",
                                 database="postgres",
                                 timeout=10)
    # 除了上面的方式，还可以使用类似于 SQLAlchemy 的方式创建
    # await asyncpg.connect("postgres://postgres:zg@localhost:5432/postgres")

    # 调用 await conn.fetchrow 执行 select 语句，获取满足条件的单条记录
    # 调用 await conn.fetch 执行 select 语句，获取满足条件的全部记录
    row1 = await conn.fetchrow("select * from girl")
    row2 = await conn.fetch("select * from girl")

    # 返回的是一个 Record 对象，这个 Record 对象等于将返回的记录进行了一个封装
    # 至于怎么用后面会说
    print(row1)  # <Record id=1 name='古明地觉' age=16 place='地灵殿'>
    pprint(row2)
    """
    [<Record id=1 name='古明地觉' age=16 place='地灵殿'>,
     <Record id=2 name='椎名真白' age=16 place='樱花庄'>,
     <Record id=3 name='古明地恋' age=15 place='地灵殿'>]
    """

    # 关闭连接
    await conn.close()


loop = asyncio.get_event_loop()
loop.run_until_complete(main())
loop.close()
```
以上演示了如何使用 asyncpg 来获取数据库中的记录，可以看到执行 select 语句的话，可以使用 `conn.fetchrow(query)` 来获取满足条件的单条记录，`conn.fetch(query)` 来获取满足条件的所有记录。<br />**Record 对象**<br />使用 `conn.fetchone` 查询得到的是一个 Record 对象，使用 `conn.fetch` 查询得到的是多个 Record 对象组成的列表，那么这个 Rcord 对象怎么用呢？
```python
import asyncio
import asyncpg

async def main():
    conn = await asyncpg.connect("postgres://postgres:zg@localhost:5432/postgres")
    row = await conn.fetchrow("select * from girl")

    print(type(row))  # <class 'asyncpg.Record'>
    print(row)  # <Record id=1 name='古明地觉' age=16 place='地灵殿'>

    # 这个 Record 对象可以想象成一个字典
    # 我们可以将返回的字段名作为 key, 通过字典的方式进行获取
    print(row["id"], row["name"])  # 1 古明地觉

    # 除此之外，还可以通过 get 获取，获取不到的时候会返回默认值
    print(row.get("id"), row.get("name"))  # 1 古明地觉
    print(row.get("xxx"), row.get("xxx", "不存在的字段"))  # None 不存在的字段

    # 除此之外还可以调用 keys、values、items，这个不用我说，都应该知道意味着什么
    # 只不过返回的是一个迭代器
    print(row.keys())  # <tuple_iterator object at 0x000001D6FFDAE610>
    print(row.values())  # <tuple_iterator object at 0x000001D6FFDAE610>
    print(row.items())  # <RecordItemsIterator object at 0x000001D6FFDF20C0>

    # 我们需要转成列表或者元组
    print(list(row.keys()))  # ['id', 'name', 'age', 'place']
    print(list(row.values()))  # [1, '古明地觉', 16, '地灵殿']
    print(dict(row.items()))  # {'id': 1, 'name': '古明地觉', 'age': 16, 'place': '地灵殿'}
    print(dict(row))  # {'id': 1, 'name': '古明地觉', 'age': 16, 'place': '地灵殿'}

    # 关闭连接
    await conn.close()


if __name__ == '__main__':
    asyncio.run(main())
```
当然也可以借助 SQLAlchemy 拼接 SQL 语句。
```python
import asyncio
from pprint import pprint
import asyncpg
from sqlalchemy.sql.selectable import Select
from sqlalchemy import text


async def main():
    conn = await asyncpg.connect("postgres://postgres:zg@localhost:5432/postgres")
    sql = Select([text("id, name, place")], whereclause=text("id != 1"), from_obj=text("girl"))
    # 我们不能直接传递一个 Select 对象, 而是需要将其转成原生的字符串才可以
    rows = await conn.fetch(str(sql))
    pprint(list(map(dict, rows)))  
    """
    [{'id': 2, 'name': '椎名真白', 'place': '樱花庄'},
     {'id': 3, 'name': '古明地恋', 'place': '地灵殿'}]
    """

    # 关闭连接
    await conn.close()


if __name__ == '__main__':
    asyncio.run(main())
```
此外，`conn.fetch` 里面还支持占位符，使用百分号加数字的方式，举个例子：
```python
import asyncio
from pprint import pprint
import asyncpg

async def main():
    conn = await asyncpg.connect("postgres://postgres:zg@localhost:5432/postgres")
    rows = await conn.fetch("select * from girl where id != $1", 1)
    pprint(list(map(dict, rows)))
    """
    [{'age': 16, 'id': 2, 'name': '椎名真白', 'place': '樱花庄'},
     {'age': 15, 'id': 3, 'name': '古明地恋', 'place': '地灵殿'}]
    """

    # 关闭连接
    await conn.close()


if __name__ == '__main__':
    asyncio.run(main())
```
还是推荐使用 SQLAlchemy 的方式，这样更加方便一些，就像 aiomysql 一样。但是对于 asyncpg 而言，实际上接收的是一个原生的 SQL 语句，是一个字符串，因此它不能像 aiomysql 一样自动识别 Select 对象，还需要手动将其转成字符串。而且这样还存在一个问题，至于是什么我们下面介绍添加记录的时候说。
<a name="tmSbl"></a>
### 添加记录
然后是添加记录，看看如何往库里面添加数据。
```python
import asyncio
from pprint import pprint
import asyncpg
from sqlalchemy.sql.selectable import Select
from sqlalchemy import text


async def main():
    conn = await asyncpg.connect("postgres://postgres:zg@localhost:5432/postgres")
    # 执行 insert 语句我们可以使用 execute
    row = await conn.execute("insert into girl(name, age, place) values ($1, $2, $3)",
                             '十六夜咲夜', 17, '红魔馆')
    pprint(row)  # INSERT 0 1
    pprint(type(row))  # <class 'str'>

    await conn.close()


if __name__ == '__main__':
    asyncio.run(main())
```
通过 `execute` 可以插入单条记录，同时返回相关信息，但是说实话这个信息没什么太大用。除了 `execute` 之外，还有 `executemany`，用来执行多条插入语句。
```python
import asyncio
import asyncpg

async def main():
    conn = await asyncpg.connect("postgres://postgres:zg@localhost:5432/postgres")
    # executemany：第一条参数是一个模板，第二条命令是包含多个元组的列表
    # 执行多条记录的话，返回的结果为 None
    rows = await conn.executemany("insert into girl(name, age, place) values ($1, $2, $3)",
                                  [('十六夜咲夜', 17, '红魔馆'), ('琪露诺', 60, '雾之湖')])
    print(rows)  # None

    # 关闭连接
    await conn.close()


if __name__ == '__main__':
    asyncio.run(main())
```
注意：如果是执行大量 `insert` 语句的话，那么 `executemany` 要比 `execute` 快很多，但是 `executemany` 不具备事务功。
