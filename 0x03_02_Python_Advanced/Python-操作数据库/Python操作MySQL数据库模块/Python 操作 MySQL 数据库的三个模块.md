PythonMySQL<br />python使用MySQL主要有两个模块，pymysql（MySQLdb）和SQLAchemy。

- **pymysql（MySQLdb）为原生模块，直接执行sql语句，其中pymysql模块支持python 2和python3，MySQLdb只支持python2，两者使用起来几乎一样。**
- **SQLAchemy为一个ORM框架，将数据对象转换成SQL，然后使用数据API执行SQL并获取执行结果**
- **另外DBUtils模块提供了一个数据库连接池，方便多线程场景中python操作数据库。**
<a name="n5PSL"></a>
## **1、pymysql模块**
安装：`pip install pymysql`
<a name="X7Re5"></a>
### **创建表格操作(注意中文格式设置)**
```python
#coding:utf-8
import pymysql

#关于中文问题
#1. mysql命令行创建数据库，设置编码为gbk：create databse demo2 character set utf8; 
#2. python代码中连接时设置charset="gbk"
#3. 创建表格时设置default charset=utf8

#连接数据库
conn = pymysql.connect(host="localhost", user="root", passwd="", db='learningsql', charset='utf8', port=3306)  #和mysql服务端设置格式一样（还可设置为gbk, gb2312）
#创建游标
cursor = conn.cursor()
#执行sql语句
cursor.execute("""create table if not exists t_sales(
                id int primary key auto_increment not null,
                 nickName varchar(128) not null,
                 color varchar(128) not null,
                  size varchar(128) not null, 
                  comment text not null,
                  saledate varchar(128) not null)engine=InnoDB default charset=utf8;""")
                  
# cursor.execute("""insert into t_sales(nickName,color,size,comment,saledate) 
                # values('%s','%s','%s','%s','%s');""" % ("zack", "黑色", "L", "大小合适", "2019-04-20"))
                
cursor.execute("""insert into t_sales(nickName,color,size,comment,saledate) 
                values(%s,%s,%s,%s,%s);""" , ("zack", "黑色", "L", "大小合适", "2019-04-20"))
#提交
conn.commit()
#关闭游标
cursor.close()
#关闭连接
conn.close()
```
<a name="dWMd1"></a>
### **增删改查：**
注意`execute`执行sql语句参数的两种情况：

- `**execute("insert into t_sales(nickName, size) values('%s','%s');" % ("zack","L") )  #此时的%s为字符窜拼接占位符，需要引号加'%s'**`**  （有sql注入风险）**
- `**execute("insert into t_sales(nickName, size) values(%s,%s);" , ("zack","L") ) #此时的%s为sql语句占位符，不需要引号%s**`
```python
#***************************增删改查******************************************************
conn = pymysql.connect(host="localhost", user="root", passwd="", db='learningsql', charset='utf8', port=3306)  #和mysql服务端设置格式一样（还可设置为gbk, gb2312）
#创建游标
cursor = conn.cursor()

insert_sql = "insert into t_sales(nickName,color,size,comment,saledate) values(%s,%s,%s,%s,%s);"
#返回受影响的行数
row1 = cursor.execute(insert_sql,("Bob", "黑色", "XL", "便宜实惠", "2019-04-20"))

update_sql = "update t_sales set color='白色' where id=%s;"
#返回受影响的行数
row2 = cursor.execute(update_sql,(1,))

select_sql = "select * from t_sales where id>%s;"
#返回受影响的行数
row3 = cursor.execute(select_sql,(1,))

delete_sql = "delete from t_sales where id=%s;"
#返回受影响的行数
row4 = cursor.execute(delete_sql,(4,))

#提交，不然无法保存新建或者修改的数据(增删改得提交)
conn.commit()
cursor.close()
conn.close()
```
<a name="jj4CN"></a>
### **批量插入和自增id**
```python
#***************************批量插入******************************************************
conn = pymysql.connect(host="localhost", user="root", passwd="", db='learningsql', charset='utf8', port=3306)  #和mysql服务端设置格式一样（还可设置为gbk, gb2312）
#创建游标
cursor = conn.cursor()

insert_sql = "insert into t_sales(nickName,color,size,comment,saledate) values(%s,%s,%s,%s,%s);"
data = [("Bob", "黑色", "XL", "便宜实惠", "2019-04-20"),("Ted", "黄色", "M", "便宜实惠", "2019-04-20"),("Gary", "黑色", "M", "穿着舒服", "2019-04-20")]
row1 = cursor.executemany(insert_sql, data)

conn.commit()
#为插入的第一条数据的id，即插入的为5,6,7，new_id=5
new_id = cursor.lastrowid
print(new_id)
cursor.close()
conn.close()
```
<a name="m1Bx5"></a>
### **获取查询数据**
```python
#***************************获取查找sql的查询数据******************************************************
conn = pymysql.connect(host="localhost", user="root", passwd="", db='learningsql', charset='utf8', port=3306)  #和mysql服务端设置格式一样（还可设置为gbk, gb2312）
#创建游标
cursor = conn.cursor()

select_sql = "select id,nickname,size from t_sales where id>%s;"
cursor.execute(select_sql, (3,))

row1 = cursor.fetchone()      #获取第一条数据，获取后游标会向下移动一行
row_n = cursor.fetchmany(3)  #获取前n条数据，获取后游标会向下移动n行
row_all = cursor.fetchall()  #获取所有数据，获取后游标会向下移动到末尾
print(row1)
print(row_n)
print(row_all)
#conn.commit()
cursor.close()
conn.close()
```
注：在fetch数据时按照顺序进行，可以使用`cursor.scroll(num,mode)`来移动游标位置，如：

- `**cursor.scroll(1,mode='relative')  # 相对当前位置移动**`
- `**cursor.scroll(2,mode='absolute') # 相对绝对位置移动**`
<a name="redC8"></a>
### **fetch获取数据类型**
fetch获取的数据默认为元组格式，还可以获取字典类型的，如下：
```python
#***************************获取字典格式数据******************************************************
conn = pymysql.connect(host="localhost", user="root", passwd="", db='learningsql', charset='utf8', port=3306)  #和mysql服务端设置格式一样（还可设置为gbk, gb2312）
#创建游标
cursor = conn.cursor(cursor=pymysql.cursors.DictCursor)

select_sql = "select id,nickname,size from t_sales where id>%s;"
cursor.execute(select_sql, (3,))

row1 = cursor.fetchall()      

print(row1)

conn.commit()
cursor.close()
conn.close()
```
<a name="lyY86"></a>
## **2、SQLAlchmy框架**
SQLAlchemy的整体架构如下，建立在第三方的DB API上，将类和对象操作转换为数据库sql，然后利用DB API执sql语句得到结果。其适用于多种数据库。另外其内部实现了数据库连接池，方便进行多线程操作。

- **Engine，框架的引擎**
- **Connection Pooling ，数据库连接池**
- **Dialect(**[http://docs.sqlalchemy.org/en/latest/dialects/index.html](http://docs.sqlalchemy.org/en/latest/dialects/index.html)**)，选择连接数据库的DB API种类，（pymysql，mysqldb等）``**
- **Schema/Types，架构和类型**
- **SQL Exprression Language，SQL表达式语言**
- **DB API(**[https://www.python.org/dev/peps/pep-0249/](https://www.python.org/dev/peps/pep-0249/)**)：Python Database API Specification**
<a name="erT1f"></a>
### **2.1 执行原生sql**
安装：`pip install sqlalchemy`<br />SQLAlchmy也可以不利用ORM，使用数据库连接池，类似pymysql模块执行原生sql
```python
#coding:utf-8

from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, String, Integer
import threading

engine = create_engine(
            "mysql+pymysql://root@127.0.0.1:3306/learningsql?charset=utf8",
            max_overflow = 0,  #超过连接池大小外最多创建的连接，为0表示超过5个连接后，其他连接请求会阻塞 （默认为10）
            pool_size = 5,    #连接池大小（默认为5）
            pool_timeout = 30,  #连接线程池中，没有连接时最多等待的时间，不设置无连接时直接报错 （默认为30）
            pool_recycle = -1)  #多久之后对线程池中的线程进行一次连接的回收（重置） （默认为-1）
            
# def task():
    # conn= engine.raw_connection() #建立原生连接,和pymysql的连接一样
    # cur = conn.cursor()
    # cur.execute("select * from t_sales where id>%s",(2,))
    # result = cur.fetchone()
    # cur.close()
    # conn.close()
    # print(result)


    
# def task():
    # conn = engine.contextual_connect() #建立上下文管理器连接，自动打开和关闭
    # with conn:
        # cur = conn.execute("select * from t_sales where id>%s",(2,))
        # result = cur.fetchone()
    # print(result)
    
    
def task():
    cur =engine.execute("select * from t_sales where id>%s",(2,))  #engine直接执行
    result = cur.fetchone()
    cur.close()
    print(result)

if __name__=="__main__":
    for i in range(10):
        t = threading.Thread(target=task)
        t.start()
```
<a name="frCwT"></a>
### **2.2 执行ORM语句**
<a name="xcXvG"></a>
#### **A、创建和删除表**
```python
#coding:utf-8

import datetime
from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, String, Integer, DateTime, Text

Base = declarative_base()

class User(Base):
    __tablename__="users"
    id = Column(Integer,primary_key=True)
    name = Column(String(32),index=True, nullable=False) #创建索引，不为空
    email = Column(String(32),unique=True)
    ctime = Column(DateTime, default = datetime.datetime.now)  #传入方法名datetime.datetime.now
    extra = Column(Text,nullable=True)  
    
    __table_args__ = {
    
        # UniqueConstraint('id', 'name', name='uix_id_name'), #设置联合唯一约束
        # Index('ix_id_name', 'name', 'email'),               # 创建索引
    }

def create_tbs():
    engine = create_engine("mysql+pymysql://root@127.0.0.1:3306/learningsql?charset=utf8",max_overflow=2,pool_size=5)
    Base.metadata.create_all(engine)   #创建所有定义的表

def drop_dbs():
    engine = create_engine("mysql+pymysql://root@127.0.0.1:3306/learningsql?charset=utf8",max_overflow=2,pool_size=5)
    Base.metadata.drop_all(engine)   #删除所有创建的表

if __name__=="__main__":
    create_tbs() #创建表
    #drop_dbs()   #删除表
```
<a name="aBcr5"></a>
#### **B、表中定义外键关系（一对多，多对多）**
**思考**：下面代码中的一对多关系，relationship 定义在了 customer 表中，应该定义在 PurchaseOrder 更合理？<br />**注意**：mysql 数据库中避免使用 order做为表的名字，order 为一个 mysql 关键字，做为表名字时必须用反引号order (键盘数字1旁边的符号)
```python
#coding:utf-8

from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column,Integer,String,Text,DateTime,ForeignKey,Float
from sqlalchemy.orm import relationship
import datetime

engine = create_engine("mysql+pymysql://root@127.0.0.1:3306/learningsql?charset=utf8")  #数据库有密码时，//root:12345678@127.0.0.1:3306/
Base = declarative_base()

class Customer(Base):
    __tablename__="customer"  #数据库中保存的表名字
    
    id = Column(Integer,primary_key=True)
    name = Column(String(64),index=True,nullable=False)
    phone = Column(String(16),nullable=False)
    address = Column(String(256),nullable=False)
    purchase_order_id = Column(Integer,ForeignKey("purchase_order.id"))  #关键关系，关联表的__tablename__="purchase_order"
    
    # 和建立表结构无关，方便外键关系查询，backref反向查询时使用order_obj.customer
    purchase_order = relationship("PurchaseOrder",backref="customer")

    
    
class PurchaseOrder(Base):  
    __tablename__ = "purchase_order"   #mysql数据库中表的名字避免使用order，order为一个关键字，使用时必须用反引号`order` (键盘数字1旁边的符号)
    id=Column(Integer,primary_key=True)
    cost = Column(Float,nullable=True)
    ctime = Column(DateTime,default =datetime.datetime.now)
    desc = Column(String(528))
    
    #多对多关系时，secondary为中间表
    product = relationship("Product",secondary="order_to_product",backref="purchase_order")
    
class Product(Base):
    __tablename__ = "product"
    id = Column(Integer,primary_key=True)
    name = Column(String(256))
    price = Column(Float,nullable=False)
    
class OrdertoProduct(Base):
    __tablename__ = "order_to_product"
    id = Column(Integer,primary_key=True)
    product_id = Column(Integer,ForeignKey("product.id"))
    purchase_order_id  = Column(Integer,ForeignKey("purchase_order.id"))
    


if __name__ == "__main__":
    Base.metadata.create_all(engine)
    #Base.metadata.drop_all(engine)
```
<a name="USYYo"></a>
#### **C、增删改查操作**
增删改查
```python
#coding:utf-8

from sqlalchemy import create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column,Integer,String,Text,DateTime,ForeignKey,Float
from sqlalchemy.orm import relationship,sessionmaker
from sqlalchemy.sql import text
import datetime

engine = create_engine("mysql+pymysql://root@127.0.0.1:3306/learningsql?charset=utf8")  #数据库有密码时，//root:12345678@127.0.0.1:3306/, 设置utf8防止中文乱码
Base = declarative_base()

class Customer(Base):
    __tablename__="customer"  #数据库中保存的表名字
    
    id = Column(Integer,primary_key=True)
    name = Column(String(64),index=True,nullable=False)
    phone = Column(String(16),nullable=False)
    address = Column(String(256),nullable=False)
    purchase_order_id = Column(Integer,ForeignKey("purchase_order.id"))  #关键关系，关联表的__tablename__="purchase_order"
    
    # 和建立表结构无关，方便外键关系查询，backref反向查询时使用order_obj.customer
    purchase_order = relationship("PurchaseOrder",backref="customer")
    
    
class PurchaseOrder(Base):  
    __tablename__ = "purchase_order"   #mysql数据库中表的名字避免使用order，order为一个关键字，使用时必须用反引号`order` (键盘数字1旁边的符号)
    id=Column(Integer,primary_key=True)
    cost = Column(Float,nullable=True)
    ctime = Column(DateTime,default =datetime.datetime.now)
    desc = Column(String(528))
    
    #多对多关系时，secondary为中间表
    product = relationship("Product",secondary="order_to_product",backref="purchase_order")
    
class Product(Base):
    __tablename__ = "product"
    id = Column(Integer,primary_key=True)
    name = Column(String(256))
    price = Column(Float,nullable=False)
    
class OrdertoProduct(Base):
    __tablename__ = "order_to_product"
    id = Column(Integer,primary_key=True)
    product_id = Column(Integer,ForeignKey("product.id"))
    purchase_order_id  = Column(Integer,ForeignKey("purchase_order.id"))


if __name__ == "__main__":
    #Base.metadata.create_all(engine)
    #Base.metadata.drop_all(engine)
    
    Session = sessionmaker(bind=engine)

    #每次进行数据库操作时都要创建session
    session = Session()

    #*****************增加数据********************
    # pur_order = PurchaseOrder(cost=19.7,desc="python编程之路")

    # session.add(pur_order)
    # session.add_all(
                # [PurchaseOrder(cost=39.7,desc="linux操作系统"),
                # PurchaseOrder(cost=59.6,desc="python cookbook")])
    # session.commit()
    
    #*****************修改数据********************
    
    #session.query(PurchaseOrder).filter(PurchaseOrder.id>2).update({"cost":29.7})
    #session.query(PurchaseOrder).filter(PurchaseOrder.id==2).update({"cost":PurchaseOrder.cost+40.1},synchronize_session=False)  #synchronize_session用于query在进行delete or update操作时，对session的同步策略。
    #session.commit()
    
    #*****************删除数据********************
    #session.query(PurchaseOrder).filter(PurchaseOrder.id==1).delete()
    #session.commit()
    
    #*****************查询数据********************
    #ret = session.query(PurchaseOrder).all()
    # ret = session.query(PurchaseOrder).filter(PurchaseOrder.id==2).all()  #包含对象的列表
    # ret = session.query(PurchaseOrder).filter(PurchaseOrder.id==2).first() #单个对象
    # ret = session.query(PurchaseOrder).filter_by(id=2).all()  #通过列名字的表达式
    # ret = session.query(PurchaseOrder).filter_by(id=2).first()
    #ret = session.query(PurchaseOrder).filter(text("id<:value and cost>:price")).params(value=6,price=15).order_by(PurchaseOrder.cost).all()
    #ret = session.query(PurchaseOrder).from_statement(text("SELECT * FROM purchase_order WHERE cost>:price")).params(price=40).all()
    # print ret
    # for i in ret:
        # print i.id, i.cost, i.ctime,i.desc
        
    #ret2 = session.query(PurchaseOrder.id,PurchaseOrder.cost.label('totalcost')).all()  #只查询两列，ret2为列表
    #print ret2
    
    #关闭session
    session.close()
```
查询语句
```python
# 条件
ret = session.query(Users).filter_by(name='alex').all()
ret = session.query(Users).filter(Users.id > 1, Users.name == 'eric').all()
ret = session.query(Users).filter(Users.id.between(1, 3), Users.name == 'eric').all()
ret = session.query(Users).filter(Users.id.in_([1,3,4])).all()
ret = session.query(Users).filter(~Users.id.in_([1,3,4])).all()
ret = session.query(Users).filter(Users.id.in_(session.query(Users.id).filter_by(name='eric'))).all()
from sqlalchemy import and_, or_
ret = session.query(Users).filter(and_(Users.id > 3, Users.name == 'eric')).all()
ret = session.query(Users).filter(or_(Users.id < 2, Users.name == 'eric')).all()
ret = session.query(Users).filter(
    or_(
        Users.id < 2,
        and_(Users.name == 'eric', Users.id > 3),
        Users.extra != ""
    )).all()


# 通配符
ret = session.query(Users).filter(Users.name.like('e%')).all()
ret = session.query(Users).filter(~Users.name.like('e%')).all()

# 限制
ret = session.query(Users)[1:2]

# 排序
ret = session.query(Users).order_by(Users.name.desc()).all()
ret = session.query(Users).order_by(Users.name.desc(), Users.id.asc()).all()

# 分组
from sqlalchemy.sql import func

ret = session.query(Users).group_by(Users.extra).all()
ret = session.query(
    func.max(Users.id),
    func.sum(Users.id),
    func.min(Users.id)).group_by(Users.name).all()

ret = session.query(
    func.max(Users.id),
    func.sum(Users.id),
    func.min(Users.id)).group_by(Users.name).having(func.min(Users.id) >2).all()

# 连表

ret = session.query(Users, Favor).filter(Users.id == Favor.nid).all()

ret = session.query(Person).join(Favor).all()

ret = session.query(Person).join(Favor, isouter=True).all()


# 组合
q1 = session.query(Users.name).filter(Users.id > 2)
q2 = session.query(Favor.caption).filter(Favor.nid < 2)
ret = q1.union(q2).all()

q1 = session.query(Users.name).filter(Users.id > 2)
q2 = session.query(Favor.caption).filter(Favor.nid < 2)
ret = q1.union_all(q2).all()
```
补充
```python
#coding:utf-8

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from sqlalchemy.sql import text, func
from sqlalchemy_orm2 import PurchaseOrder  #导入定义的PurchaseOrder表格类

engine = create_engine("mysql+pymysql://root@127.0.0.1:3306/learningsql?charset=utf8")
Session = sessionmaker(bind=engine)
session = Session()
#查询
ret = session.execute("select * from purchase_order where id=:value",params={"value":3})
print ret
for i in ret:
    print i.id, i.cost, i.ctime,i.desc

#插入
purchase_order  = PurchaseOrder.__table__  #拿到PurchaseOrder表格对象
ret=session.execute(purchase_order.insert(),
                [{"cost":46.3,"desc":'python2'},
                {"cost":43.3,"desc":'python3'}])
session.commit()
print(ret.lastrowid)
session.close()

# 关联子查询
subqry = session.query(func.count(Server.id).label("sid")).filter(Server.id == Group.id).correlate(Group).as_scalar()
result = session.query(Group.name, subqry)
"""
SELECT `group`.name AS group_name, (SELECT count(server.id) AS sid 
FROM server 
WHERE server.id = `group`.id) AS anon_1 
FROM `group`
"""
```
<a name="XINDK"></a>
#### **D、多线程操作**
```python
#coding:utf-8

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from sqlalchemy_orm2 import Product
from threading import Thread

engine = create_engine("mysql+pymysql://root@127.0.0.1:3306/learningsql?charset=utf8",max_overflow=0,pool_size=5)
Session = sessionmaker(bind=engine)

def task(name,price):
    session = Session()
    pro = Product(name=name,price=price)
    session.add(pro)
    session.commit()
    session.close()

if __name__=="__main__":
    for i in range(6):
        t = Thread(target=task,args=("pro"+str(i),i*5))
        t.start()
```
<a name="Wd8Qr"></a>
#### **E、通过**`**relationship**`**操纵一对多和多对多关系**
一对多
```python
#coding:utf-8

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from sqlalchemy.sql import text, func
from sqlalchemy_orm2 import PurchaseOrder,Product,OrdertoProduct,Customer  #导入定义的表格类


engine = create_engine("mysql+pymysql://root@127.0.0.1:3306/learningsql?charset=utf8")
Session = sessionmaker(bind=engine)
session = Session()

# #通过定义的关键关系添加(id值)
# cus1 = Customer(name="zack",phone="13567682333",address="Nanjing",purchase_order_id=3)
# session.add(cus1)


# #通过relationship正向添加
# cus2 = Customer(name="zack2",phone="13567682333",address="Nanjing",purchase_order=PurchaseOrder(cost=53,desc="java"))
# session.add(cus2)
# session.commit()

#通过relationship反向添加
# purchase_order=PurchaseOrder(cost=53,desc="php")
# cus3 = Customer(name="zack3",phone="13567682333",address="Nanjing")
# cus4 = Customer(name="zack4",phone="13567682333",address="Nanjing")
# purchase_order.customer=[cus3,cus4]  #cus3,cus4的purchase_order_id都是purchase_order.id值,即同时添加了两组外键关系
# session.add(purchase_order)
# session.commit()

##通过relationship正向查询
cus = session.query(Customer).first()
print(cus.purchase_order_id)
print(cus.purchase_order.desc)

#通过relationship反向查询
pur = session.query(PurchaseOrder).filter(PurchaseOrder.id==3).first()
print(pur.desc)
print(pur.customer) #返回一个list
```
多对多
```python
#coding:utf-8


from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from sqlalchemy.sql import text, func
from sqlalchemy_orm2 import PurchaseOrder,Product,OrdertoProduct,Customer  #导入定义的表格类

engine = create_engine("mysql+pymysql://root@127.0.0.1:3306/learningsql?charset=utf8")
Session = sessionmaker(bind=engine)
session = Session()

# session.add_all([Product(name="java",price=24),
                # Product(name="python",price=34),
                # Product(name="php",price=27)])
# session.commit()

# #通过定义的关键关系添加(id值)
# op = OrdertoProduct(product_id=1,purchase_order_id=16)
# session.add(op)
# session.commit()

# #通过relationship添加
# pur = PurchaseOrder(cost=27,desc="xxxx")
# pur.product = [Product(name="C++",price=60),]  #正向
# session.add(pur)

# pro = Product(name="C",price=40)
# pro.purchase_order=[PurchaseOrder(cost=27,desc="xxxx"),]  #反向
# session.add(pro)
# session.commit()


#通过relationship正向查询
pur = session.query(PurchaseOrder).filter(PurchaseOrder.id==19).first()
print(pur.desc)
print(pur.product) #结果为列表

#通过relationship反向查询
pro = session.query(Product).filter(Product.id==5).first()
print(pro.name)
print(pro.purchase_order) #结果为列表

session.close()
```
<a name="XHoY5"></a>
## **3、数据库连接池**
对于ORM框架，其内部维护了链接池，可以直接通过多线程操控数据库。对于pymysql模块，通过多线程操控数据库容易出错，得加锁串行执行。进行并发时，可以利用DBUtils模块来维护数据库连接池。
<a name="cw2T7"></a>
### **3.1 多线程操控pymysql**
不采用DBUtils连接池时, pymysql多线程代码如下：<br />每个线程创建链接
```python
import pymysql
import threadind

#**************************无连接池*******************************    
#每个线程都要创立一次连接，线程并发操作间可能有问题？
def func():
    conn = pymysql.connect(host="127.0.0.1",port=3306,db="learningsql",user="root",passwd="",charset="utf8")
    cursor = conn.cursor()
    cursor.execute("select * from user where nid>1;")
    result = cursor.fetchone()
    print(result)
    cursor.close()
    conn.close()
    
if __name__=="__main__":
    for i in range(5):
        t = threading.Thread(target=func,name="thread-%s"%i)
        t.start()
```
一个连接串行执行
```python
#**************************无连接池*******************************
#创建一个连接，加锁串行执行
from threading import Lock
import pymysql
import threading
conn = pymysql.connect(host="127.0.0.1",port=3306,db="learningsql",user="root",passwd="",charset="utf8")    


lock = Lock()    
def func():
    with lock:
        cursor = conn.cursor()
        cursor.execute("select * from user where nid>1;")
        result = cursor.fetchone()
        print(result)
        cursor.close()
        
        #conn.close()不能在线程中关闭连接，否则其他线程不可用了
        
if __name__=="__main__":
    threads = []
    for i in range(5):
        t = threading.Thread(target=func,name="thread-%s"%i)
        threads.append(t)
        t.start()
        
    for t in threads:
        t.join()
    
    conn.close()
```
<a name="NoZ0g"></a>
### **3.2 DBUtils连接池**
DBUtils连接池有两种连接模式：PersistentDB和PooledDB<br />官网文档：[https://cito.github.io/DBUtils/UsersGuide.html](https://cito.github.io/DBUtils/UsersGuide.html)
<a name="pEatP"></a>
#### **模式一(**`**DBUtils.PersistentDB**`**):**
为每个线程创建一个连接，线程即使调用了close方法，也不会关闭，只是把连接重新放到连接池，供自己线程再次使用。当线程终止时，连接自动关闭。<br />PersistentDB使用代码如下：
```python
#coding:utf-8

from DBUtils.PersistentDB import PersistentDB
import pymysql
import threading

pool = PersistentDB(
    creator = pymysql,  # 使用链接数据库的模块
    maxusage = None,    # 一个链接最多被重复使用的次数，None表示无限制
    setsession=[],     # 开始会话前执行的命令列表。如：["set datestyle to ...", "set time zone ..."]
    ping = 0,           # ping MySQL服务端，检查是否服务可用。# 如：0 = None = never, 1 = default = whenever it is requested, 2 = when a cursor is created, 4 = when a query is executed, 7 = always
    closeable = False,    # 如果为False时， conn.close() 实际上被忽略，供下次使用，再线程关闭时，才会自动关闭链接。如果为True时， conn.close()则关闭链接，那么再次调用pool.connection时就会报错，因为已经真的关闭了连接（pool.steady_connection()可以获取一个新的链接）
    threadlocal = None,    # 本线程独享值得对象，用于保存链接对象，如果链接对象被重置
    host="127.0.0.1",
    port = 3306,
    user = "root",
    password="",
    database="learningsql",
    charset = "utf8"
)

def func():
    conn = pool.connection()
    cursor = conn.cursor()
    cursor.execute("select * from user where nid>1;")
    result = cursor.fetchone()
    print(result)
    cursor.close()
    conn.close()
    
if __name__ == "__main__":
    for i in range(5):
        t = threading.Thread(target=func,name="thread-%s"%i)
        t.start()
```
<a name="h9fzs"></a>
#### **模式二(**`**DBUtils.PooledDB**`**)：**
创建一批连接到连接池，供所有线程共享使用。<br />(由于pymysql、MySQLdb等threadsafety值为1，所以该模式连接池中的线程会被所有线程共享。)<br />PooledDB使用代码如下：
```python
from DBUtils.PooledDB import PooledDB
import pymysql
import threading
import time

pool = PooledDB(
    creator = pymysql,  # 使用链接数据库的模块
    maxconnections = 6,  # 连接池允许的最大连接数，0和None表示不限制连接数
    mincached = 2,   # 初始化时，链接池中至少创建的空闲的链接，0表示不创建
    maxcached = 5,   # 链接池中最多闲置的链接，0和None不限制
    maxshared = 3,   # 链接池中最多共享的链接数量，0和None表示全部共享。PS: 无用，因为pymysql和MySQLdb等模块的 threadsafety都为1，所有值无论设置为多少，_maxcached永远为0，所以永远是所有链接都共享。
    blocking = True,  # 连接池中如果没有可用连接后，是否阻塞等待。True，等待；False，不等待然后报错
    maxusage = None,   # 一个链接最多被重复使用的次数，None表示无限制
    setsession = [],   # 开始会话前执行的命令列表。如：["set datestyle to ...", "set time zone ..."]
    ping = 0,           # ping MySQL服务端，检查是否服务可用。# 如：0 = None = never, 1 = default = whenever it is requested, 2 = when a cursor is created, 4 = when a query is executed, 7 = always
    host="127.0.0.1",
    port = 3306,
    user="root",
    password="",
    database = "learningsql",
    charset = "utf8"
)

def func():
    conn = pool.connection()
    cursor = conn.cursor()
    cursor.execute("select * from user where nid>1;")
    result = cursor.fetchone()
    print(result)
    time.sleep(5)  #为了查看mysql端的线程数量
    cursor.close()
    conn.close()
    
if __name__=="__main__":
    for i in range(5):
        t = threading.Thread(target=func,name="thread-%s"%i)
        t.start()
```
上述代码中加入了`sleep(5)`使线程连接数据库时间延长，方便查看mysql数据库连接线程情况，下图分别为代码执行中和执行后的线程连接情况，可以发现，代码执行时，同时有6个线程连接上了数据库（有一个为mysql命令客户端），代码执行后，只有一个线程连接数据库，但仍有5个线程等待连接。<br />（`show status like "Threads%"` 查看线程连接情况）
