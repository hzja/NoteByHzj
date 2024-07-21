# drf解析器

## 本质

-  根据content-type，选择对应的解析器去解析request.body中的数据格式，并将其赋值到request.data中



## 流程

1. 进入了APIView类的dispatch方法
2. 通过request.data获取用户请求头
3. 根据用户请求头和 parser_classes = [JSONParser, FormParser] 中支持的请求头进行比较



## 示例

1. 局部

~~~ python
from rest_framework.parsers import JSONParser, FormParser

class ParserView(APIView):
    parser_classes = [JSONParser]

    def post(self, request, *args, **kwargs):
        """
        允许用户发送JSON格式的数据
            a. content-type: application/json
            b. {"name": 'alex', "age": 18}
        :param request:
        :param args:
        :param kwargs:
        :return:
        """
        """
        1. 获取用户请求
        2. 获取用户请求体
        3. 根据用户请求头和 parser_classes = [JSONParser, FormParser] 中支持的请求头进行比较
        4. JSONParser对象中去请求体
        5. request.data
        """
        print(request.data)
        return HttpResponse('JSON测试')
~~~



2. 全局

~~~ python
REST_FRAMEWORK = {
    # JSONParser: 表示只能解析 application/json 请求头
    # FormParser: 表示只能解析 application/x-www-form-urlencoded 请求头
	"DEFAULT_PARSER_CLASSES":  ["rest_framework.JSONParser", "rest_framework.FormParser"]
}
~~~