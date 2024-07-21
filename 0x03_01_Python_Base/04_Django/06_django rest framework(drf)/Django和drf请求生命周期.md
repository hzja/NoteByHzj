# Django和drf请求生命周期

## Django请求生命周期

![1581952780830-243d29bc-0170-41c2-ac2e-e720218ea877](D:\Note\python\Django\图片\1581952780830-243d29bc-0170-41c2-ac2e-e720218ea877.png)

- 前端发送请求
- wsgi, 他就是socket服务端，用于接收用户请求并将请求进行初次封装，然后将请求交给web框架（Flask、Django）
- 中间件处理请求，帮助我们对请求进行校验或在请求对象中添加其他相关数据，例如：csrf、request.session
- 路由匹配，根据当前请求的URL找到视图函数，如果是FBV写法，通过判断method两类型，找到对应的视图函数；如果是CBV写法，匹配成功后会自动去找dispatch方法，然后Django会通过dispatch反射的方式找到类中对应的方法并执行
- 视图函数，在视图函数中进行业务逻辑的处理，可能涉及到：orm、view视图将数据渲染到template模板
- 视图函数执行完毕之后，会把客户端想要的数据返回给dispatch方法，由dispatch方法把数据返回经客户端
- 中间件处理响应
- wsgi，将响应的内容发送给浏览器
- 浏览器渲染



## drf请求生命周期

![drf请求生命周期](D:\Note\python\Django\django rest framework(drf)\图片\drf请求生命周期.png)