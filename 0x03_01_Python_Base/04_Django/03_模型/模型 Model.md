# 模型 Model

使用django进行数据库开发的步骤如下：

1. 在models.py中定义模型类
2. 迁移
3. 通过类和对象完成数据增删改查操作



## 1.定义模型类

模型类定义在models.py文件中，继承自models.Model类。

~~~ python
from django.db import models

# 一类
class BookInfo(models.Model):
    btitle = models.CharField(max_length=20)
    bpub_date = models.DateField()

# 多类
class HeroInfo(models.Model):
    hname = models.CharField(max_length=20)
    hgender = models.BooleanField(default=False)
    hcomment = models.CharField(max_length=128)
    hbook = models.ForeignKey('BookInfo', on_delete= models.SET_NULL, null=True)
    
    # 自定义模型类对应的表名
    class Meat：
    	db_table = '图书表'

    def __str__(self):
        return self.name
~~~



**说明**：不需要定义主键列，在生成时会自动添加，并且值为自动增长。

BookInfo类和HeroInfo类之间具有一对多的关系，这个一对多的关系应该定义在多的那个类，也就是HeroInfo类中。



**注意：**django2.0+版本配置模型类外键关系必须指定**on_delete**参数

- **级联删除**：models.CASCADE
  当关联表中的数据删除时，该外键也删除
- **置空**：models.SET_NULL
  当关联表中的数据删除时，该外键置空，当然，你的这个外键字段得允许为空，null=True
- **设置默认值**：models.SET_DEFAULT
  删除的时候，外键字段设置为默认值，所以定义外键的时候注意加上一个默认值。



## 2.迁移

~~~ python
# 生成迁移文件：根据模型类生成创建表的迁移文件。
python manage.py makemigrations

# 执行迁移：根据第一步生成的迁移文件在数据库中创建表。
python manage.py migrate
~~~
