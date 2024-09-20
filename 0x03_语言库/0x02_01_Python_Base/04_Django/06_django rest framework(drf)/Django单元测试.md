# Django单元测试

在Django项目中，开发完一些功能模块之后，通常需要去写单元测试来检测代码的bug，最开始学习Django的时候我就发现app目录下面有个test.py的文件，但一直没人告诉我它是干什么用的，直到有一天我打开了它的源码。。。。。
代码在提交给测试工程师或者和前端对接之前，自己也要先测一下自己的接口，不要让大家觉得你是个不可靠的菜鸡。

~~~ python
from django.test import TestCase


class MyTest(TestCase):
    @classmethod
    # 这个方法用于做类级别的准备工作，他会在测试执行之前被调用，且一个类中，只被调用一次
    def setUpClass(cls):
        print('setUpClass')

    @classmethod
    # 这个方法用于做类级别的准备工作，他会在测试执行结束后被调用，且一个类中，只被调用一次
    def tearDownClass(cls):
        print('tearDownClass')

    # 这个方法会在每一个测试方法执行之前被调用，通常用来做一些准备工作
    def setUp(self) -> None:
        print('setUp')

    # 这个方法会在每一个测试用法执行之后被被调用，通常用来做一些清理工作
    def tearDown(self) -> None:
        print('tearDown')

    # 下方的都是各个业务接口
    def test_Personal_Assets_View(self):
        path = '/api/v1/assets/personal/list/'
        data = {
            'username': 'hongze',
            'page': 1,
            'limit': 10
        }
        res = self.client.get(path=path, data=data)
        status_code = res.status_code
        res_data = res.json()
        # 检查登录结果
        self.assertEqual(status_code, 200)
        self.assertEqual(res_data.get('code'), 200)

    def test_Power_Status_View(self):
        path = '/api/v1/power/instances/status/'
        data = {
            'username': 'hongze',
            'ip_address': ["10.210.136.79", "10.210.66.45", "10.210.128.19", "10.71.6.44"]
        }
        res = self.client.post(path=path, data=data, content_type='application/json')
        status_code = res.status_code
        res_data = res.json()
        self.assertEqual(status_code, 200)
        self.assertEqual(res_data.get('code'), 200)
~~~



~~~ python
# 执行测试
python manage.py test apps/assets

# 返回结果
System check identified no issues (0 silenced).
setUpClass
setUp
tearDown
.setUp
tearDown
.setUp
tearDown
.setUp
tearDown
.setUp
tearDown
.tearDownClass
----------------------------------------------------------------------
Ran 5 tests in 1.218s
OK
Destroying test database for alias 'default'...
~~~



