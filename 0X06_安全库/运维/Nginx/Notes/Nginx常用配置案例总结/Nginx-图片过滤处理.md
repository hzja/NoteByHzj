Nginx<br />http_image_filter_module是Nginx提供的集成图片处理模块，支持nginx-0.7.54以后的版本，在网站访问量不是很高；磁盘有限不想生成多余的图片文件的前提下，就可以用它实时缩放图片，旋转图片，验证图片有效性以及获取图片宽高以及图片类型信息，由于是实时计算的结果，所以网站访问量大的话，不建议使用。<br />安装还是很简单的，默认http_image_filter_module模块是不会编译进nginx的，所以要在configure时候指定编译http_image_filter_module模块。另外http_image_filter_module模块需要依赖gd-devel的支持，可以使用yum install gd-devel先安装，如果未安装会报“/configure: error: the HTTP image filter module requires the GD library.”错误。
<a name="5gzof"></a>
### Nginx图片处理的优缺点
<a name="nheCG"></a>
#### 优点
（1）操作简单。通过简单配置，省去了后端裁剪程序的复杂性；<br />（2）实时裁剪。可以实时访问在线裁剪图片；<br />（3）灵活性强。后端程序裁剪图片时需要知道裁剪图片的尺寸和质量，使用nginx裁剪可以实时裁剪任意尺寸的图片；<br />（4）不占用硬盘空间；
<a name="2e769a75"></a>
#### 缺点
（1）消耗CPU和内存，访问量大的时候就会给服务器带来很大的负担。(可以通过使用Nginx缓存和缓存服务器来解决)；<br />（2）功能不是很强大，支持的处理图片类型只包括JPEG、GIF、PNG or WebP；
<a name="6l5WM"></a>
### 1、安装http_image_filter_module
先安装gd-devel依赖库<br />
```bash
yum install gd-devel
```
Nginx添加http_image_filter_module模块

先查看旧版本Nginx的configure参数
```bash
nginx -V
```
configure时添加http_image_filter_module模块
```bash
/configure --prefix=/usr/local/nginx --user=nginx \
--group=nginx --with-http_secure_link_module \
--with-http_stub_status_module  \
--with-stream_ssl_preread_module  \
--with-http_ssl_module  \
--with-http_image_filter_module
```
编译安装nginx
```bash
make 
cp ./objs/nginx /usr/local/nginx/sbin/
```
<a name="YuXsp"></a>
### 2、语法说明

- image_filter<br />
```nginx
语法:    image_filter off;
image_filter test;
image_filter size;
image_filter rotate 90 | 180 | 270;
image_filter resize width height;
image_filter crop width height;
默认值:    —
上下文:    location
```
设置图像变换的操作：<br />off  : 在所在location关闭模块处理。<br />test :确保应答是JPEG，GIF或PNG格式的图像。否则错误 415 (Unsupported Media Type) 将被返回。<br />size: 以json格式返回图像信息。例如：
```json
{ "img" : 
 { 
   "width": 100, 
   "height": 100,
   "type": "gif" 
 } 
}
```
如果有错误发生，将会返回如下：
```json
{}
```
rotate 90|180|270 : 将图像逆时针旋转指定角度。参数的值可以包含变量。可以单独使用，或与 resize 和 crop 变换同时使用.<br />resize width height: 按比例缩小图像至指定大小。如果想只指定其中一维，另一维可以指定为：“-”。如果有错误发生，服务器会返回 415 (Unsupported Media Type). 参数的值可以包含变量。当与 rotate 参数同时使用时, 旋转发生在缩放 之后。<br />crop width height : 按比例以图像的最短边为准对图像大小进行缩小，然后裁剪另一边多出来的部分。如果想只指定其中一维，另一维可以指定为：“-”。如果有错误发生，服务器会返回 415 (Unsupported Media Type). 参数的值可以包含变量。当与 rotate 参数同时使用时, 旋转发生在裁剪 之前。

- image_filter_buffer <br />
```
语法:    image_filter_buffer size;
默认值:    image_filter_buffer 1M;
上下文:    http, server, location
```
设置用来读图像的缓冲区的最大值。若图像超过这个大小，服务器会返回 415 (Unsupported Media Type).

- image_filter_jpeg_quality <br />
```
语法: image_filter_jpeg_quality quality;
默认值:    image_filter_jpeg_quality 75;
上下文:    http, server, location
```
设置变换后的JPEG图像的 质量 。可配置值：1 ~ 100 。更小的值意味着更差的图像质量以及更少需要传输的数据。推荐的最大值是95. 参数的值可以包含变量。

- image_filter_sharpen<br />
```
语法:    image_filter_sharpen percent;
默认值:    image_filter_sharpen 0;
上下文:    http, server, location
```
增加最终图像的锐度。锐度百分比可以超过100. 0为关闭锐化。参数的值可以包含变量。

- image_filter_transparency <br />
```
语法:    image_filter_transparency on|off;
默认值:    image_filter_transparency on;
上下文:    http, server, location
```
定义当对PNG，或者GIF图像进行颜色变换时是否需要保留透明度。损失透明度有可能可以获得更高的图像质量。PNG图像中的alpha通道的透明度默认会一直被保留。
```nginx
image_filter off;  
#关闭模块  
image_filter test;  
#确保图片是jpeg gif png否则返415错误  
image_filter size;  
#输出有关图像的json格式：如下显示{ "img" : { "width": 100, "height": 100, "type": "gif" } } 出错显示：{}  
image_filter rotate 90|180|270;  
#旋转指定度数的图像，参数可以包括变量，单独或一起与resize crop一起使用。  
image_filter resize width height;  
#按比例减少图像到指定大小，如果减少一个可以另一个用"-"来表示,出错415，参数值可包含变量，可以与rotate一起使用，则两个一起生效。  
image_filter crop width height;  
#按比例减少图像比较大的侧面积和另一侧多余的裁剪边缘，其它和rotate一样。没太理解  
image_filter_buffer 10M;  
#设置读取图像缓冲的最大大小，超过则415错误。  
image_filter_interlace on;  
#如果启用，最终的图像将被交错。对于JPEG，最终的图像将在“渐进式JPEG”格式。  
image_filter_jpeg_quality 95;  
#设置变换的JPEG图像的期望质量。可接受的值是从1到100的范围内。较小的值通常意味着既降低图像质量，减少传输数据，推荐的最大值为95。参数值可以包含变量。  
image_filter_sharpen 100;  
#增加了最终图像的清晰度。锐度百分比可以超过100。零值将禁用锐化。参数值可以包含变量。  
image_filter_transparency on;  
#定义是否应该透明转换的GIF图像或PNG图像与调色板中指定的颜色时，可以保留。透明度的损失将导致更好的图像质量。在PNG的Alpha通道总是保留透明度。
```
<a name="n8Cks"></a>
### 3、实战http_image_filter_module
在Nginx配置文件nginx.conf添加如下配置
```nginx
location ~* \.(jpg|gif|png)$ {
  image_filter test;
  root html;
  # 图片默认宽度
  set $width -;  
  # 图片默认高度
  set $height -;  
  if ($arg_width != "") {
    set $width $arg_width;
  }
  if ($arg_height != "") {
    set $height $arg_height;
  }
  # 设置图片宽高
  image_filter resize $width $height;  
  # 设置nginx读取图片最大buffer
  image_filter_buffer 10M;  
  # 是否开启图片隔行扫描            
  image_filter_interlace on;   
  error_page 404 = 400.html;
}
```
配置完成后，在html目录新建image目录<br />访问可以查看结果
```
http://127.0.0.1/image/3.jpg?height=100
http://127.0.0.1/image/3.jpg?width=130
http://127.0.0.1/image/3.jpg?width=100&height=200
```
<a name="3S8gO"></a>
### 4、附录
几个可能有用的规则
<a name="6b218fb2"></a>
#### 匹配全站所有的结尾图片 
```nginx
location ~* \.(jpg|gif|png)$ {  
  image_filter resize 500 500;  
}
```
<a name="PCNmj"></a>
#### 匹配某个目录所有图片
```
location ~* /image/.*\.(jpg|gif|png)$ {  
            image_filter resize 500 500;  
        }
```
<a name="jgkrH"></a>
#### 使用url指定大小
```nginx
location ~* (.*\.(jpg|gif|png))!(.*)!(.*)$ {  
  set $width      $3;  
  set $height     $4;  
  rewrite "(.*\.(jpg|gif|png))(.*)$" $1;  
}  
location ~* /image/.*\.(jpg|gif|png)$ {  
  image_filter resize $width $height;  
}
```
[http://127.0.0.1/image/girl.jpg!300!200](http://127.0.0.1/image/girl.jpg!300!200)  <br />自动将原图缩放为300*200的尺寸
