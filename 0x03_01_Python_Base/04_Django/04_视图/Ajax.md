# Ajax

前端世界有一种叫Ajax的东西，即“Asynchronous Javascript And XML”（异步 JavaScript 和 XML），常被用于不刷新页面情况下提交和请求数据；若Django服务器接收一通过Ajax发送的POST请求则很麻烦，因AJAX中没办法像form表单中那样携带{% csrf_token %}令牌；



## $.ajax使用方法

常用参数：

1. url  请求地址
2. type  请求方式，默认是'GET'，常用的还有'POST'
3. dataType  设置返回的数据格式，常用的是'json'格式，也可以设置为'html'
4. data  设置发送给服务器的数据
5. success  设置请求成功后的回调函数
6. error  设置请求失败后的回调函数
7. async  设置是否异步，默认值是'true'，表示异步



可模拟任何请求方式向后端发送请求

~~~ python
<script src="{% static 'adminlet-2.4.10/bower_components/jquery/dist/jquery.min.js' %}"></script>

<div class="c-datepicker-date-editor J-datepicker-range mt10">
  <i class="c-datepicker-range__icon kxiconfont icon-clock"></i>
  <input id="c_time" placeholder="开始日期" name="c_time" class="c-datepicker-data-input" value="2020-01-01 04:00:00">
    <span class="c-datepicker-range-separator">-</span>
  <input id="d_dime" placeholder="结束日期" name="d_time" class="c-datepicker-data-input" value="2020-12-31 23:59:59">
</div>

<button class="date_btn">send_Ajax</button>
<script type="text/javascript">
    $(".date_btn").click(function(){
       var x = document.getElementById('c_time').value;
       var y = document.getElementById('d_dime').value;
       $.ajax({
           url:"/record/",
           type:"POST",
           data:{
             "x_time":x,
             "y_time":y
             csrfmiddlewaretoken: '{{ csrf_token }}'
           },
           rsync:true,
           success:function(data){
               console.log(data);
           },
     　　　　　　
           error: function (jqXHR, textStatus, err) {
                    console.log(arguments);
                },

           complete: function (jqXHR, textStatus) {
                    console.log(textStatus);
            },

           statusCode: {
                '403': function (jqXHR, textStatus, err) {
                      console.log(arguments);
                 },

                '400': function (jqXHR, textStatus, err) {
                    console.log(arguments);
                }
            }
           })
        })
</script>
~~~

上面代码作用是让Ajax的POST方法带上CSRF需令牌，它依赖Jquery库故须提前加载Jquery；