# restful

## web接口

~~~ python
# 请求工具：postman => https://www.getpostman.com/

# 接口：url链接，通过向链接发生不同的类型请求与数据得到相应的响应数据

# http://127.0.0.1:8888/test/
# https://api.map.baidu.com/place/v2/search
'''
ak: 6E823f587c95f0148c19993539b99295
region: 上海
query: 肯德基
output: json
'''
~~~



## RESTful规范

~~~ python
# REST: 表征性状态转移(Representational State Transfer)
# RESTful规范：web数据请求接口设计规范

'''
1）通常使用https请求
2）域名：有api关键字出现
	-- https://api.example.com  (存在跨域问题)
	-- https://example.com/api
3）版本：不同版本需要标注
	-- https://example.com/api/v1 | -- https://example.com/api/1
	-- https://example.com/api/v2 | -- https://example.com/api/2
4）资源：请求的目标数据称之为资源，资源一般都有名词复数表示
	-- https://example.com/api/v1/books  (之前不规范的案例: /get_books/)
5）操作方式：不从请求链接体现操作方式，从请求方式上决定操作方式
	-- get：https://example.com/api/v1/books  获取所有
	-- post：https://example.com/api/v1/books  新增一本
	-- put：https://example.com/api/v1/book/1  更新id=1的一本
	-- patch：https://example.com/api/v1/book/1  更新id=1的一本
	-- delete：https://example.com/api/v1/book/1  删除id=1的一本
6）资源过滤：通过接口传递参数来过滤资源
	-- https://example.com/api/v1/books?limit=10  限制10条
7）状态码：返回数据要标准状态码，通过在数据中 {"status": 200}
	-- SUCCESS(0, "查询成功")
	-- NODATA(1, "非正确，无数据，显示基本信息")
	-- FEAILED(2, "查询失败")
8）错误信息：请求失败需要标注错误信息  {"message": "请求参数不合法"}
9）操作结果：请求操作成功的返回结果 {"results": []}
	-- get：返回资源列表 | 返回单一资源
	-- post：返回单一新增资源
	-- put：返回更新的资源
	-- patch：返回更新的资源
	-- delete：返回空文档
10）子资源返回资源接口：返回的资源如果有子资源，返回子资源的链接地址，如查找书，书的封面图片就可以url表示
'''
~~~



![image7](..\图片\image7.png)