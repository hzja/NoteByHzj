Java <br />一张图片能包含很多敏感信息，其中就包括图片拍摄的 GPS 信息。GPS 对多数网民来说都不太注重，然而对于懂技术的程序员来说，根据 GPS 信息，能准确的定位到他们。<br />根据图片上的 GSP 信息进行定位。下面就利用 Java 来读取照片的拍摄时间、GPS 定位以及手机型号等等信息。<br />代码很简单，几行就能搞定。首先，先从某某美女群里下载美女所发的照片信息。<br />![2021-07-26-22-35-02-376645.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627310156403-b3b9fa81-b1ab-4239-9351-750b28859caa.png#clientId=uec78fba0-4c3e-4&from=ui&id=ucd98db2a&originHeight=483&originWidth=814&originalType=binary&ratio=1&size=1181961&status=done&style=none&taskId=u378d195b-2bdd-406a-bc91-5f40ff47fb3)<br />然后编写 Java 代码获取图片 GSP 等信息，核心代码如下所示：
```java
public static void getGpsDirectory(String filePath){
    File picFile = getPicFile(filePath);
    getGpsDirectory(picFile);
}

public static void getGpsDirectory(File picFile){
    Metadata metadata = getPicMetadata(picFile);
    GpsDirectory gpsDirectory = metadata.getFirstDirectoryOfType(GpsDirectory.class);
    if (Objects.nonNull(gpsDirectory)) {
        GeoLocation geoLocation = gpsDirectory.getGeoLocation();
        System.out.println(geoLocation.getLongitude());
        System.out.println(geoLocation.getLatitude());
    }
}

public static Metadata getPicMetadata(File picFile){
    Metadata metadata = null;
    try(InputStream inputStream = new FileInputStream(picFile);
        BufferedInputStream bufferedInputStream = new BufferedInputStream(inputStream)) {
        FileType fileType = FileTypeDetector.detectFileType(bufferedInputStream);

        switch (fileType) {
            case Png:
                metadata = PngMetadataReader.readMetadata(picFile);
                break;
            case Jpeg:
                metadata = JpegMetadataReader.readMetadata(picFile);
                break;
            default:
                return metadata;
        }
    } catch (Exception e){
        e.printStackTrace();
    }

    return metadata;
}

public static void main(String[] args) {
    getGpsDirectory("/Users/xtt/Downloads/美女照骗.jpeg");
}
```
根据上面的代码，运行后就会获得图片的 GPS 信息。
```java
24.41046111111111
103.41424722222223
```
然后访问百度地图 api（[https://api.map.baidu.com/lbsapi/getpoint/](https://api.map.baidu.com/lbsapi/getpoint/)），去根据经纬度信息获得照片的实际拍摄地址。<br />如果需要批量操作，或者批量获取地址信息，可以根据百度地图或高德地图提供的 api 去用代码调用获取。
```java
//接口调用方法如下：
//api_key：是你注册的key值
//coords：是你的经纬度坐标
 
http://api.map.baidu.com/reverse_geocoding/v3/?ak="+api_key+"&output=json&coordtype=wgs84ll&location="+coords
```
实际上根据一张未经处理过的照片，能获得想到对的信息。
```java
{海拔=0 metres, 手机=Xiaomi, 纬度=24.41046111111111, 型号=MI MAX, 经度=103.41424722222223, 拍摄时间=2018:12:01 16:37:32, 
拍摄地点=中国  云南省 红河哈尼族彝族自治州 弥勒市 XXX路 XXX号 温泉XXXX酒店}
经纬度：24.41046111111111,103.41424722222223
拍摄时间：2018:12:01 16:37:32
手机型号：Xiaomi MI MAX
拍摄地点：中国  云南省 红河哈尼族彝族自治州 弥勒市 XXX路 XXX号 温泉XXX酒店
{"status":0,"result":{"location":{"lng":103.42283328917563,"lat":24.413805252378915},
"formatted_address":"云南省红河哈尼族彝族自治州弥勒市XXX路","business":"","addressComponent":
{"country":"中国","country_code":0,"country_code_iso":"CHN","country_code_iso2":"CN",
"province":"云南省","city":"红河哈尼族彝族自治州","city_level":2,"district":"弥勒市","town":"","town_code":"","adcode":"532504","street":"温泉路","street_number":"","direction":"","distance":""},"cityCode":107}}
```
上面还只是列举的部分信息，这里面包含美女使用的手机型号，拍摄时间，拍摄地址。<br />![2021-07-26-22-35-03-032624.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1627310199551-cfdba8db-647a-4eae-a8c2-dfbd2117fc9c.png#clientId=uec78fba0-4c3e-4&from=ui&id=u80689f1f&originHeight=417&originWidth=514&originalType=binary&ratio=1&size=644546&status=done&style=shadow&taskId=u8e1f056e-5d12-41b9-84b4-617dc35a2df)<br />这些信息都是借助图片的 Exif 获得的。EXIF（Exchangeable Image File format）是“可交换图像文件”的缩写，当中包含了专门为数码相机的照片而定制的元数据，可以记录数码照片的拍摄参数、缩略图及其他属性信息。<br />EXIF 所记录的元数据信息非常丰富，主要包含了以下几种信息：

- 拍摄日期
- 拍摄器材（机身、镜头、闪光灯等）
- 拍摄参数（快门速度、光圈F值、ISO速度、焦距、测光模式等）
- 图像处理参数（锐化、对比度、饱和度、白平衡等）
- 图像描述及版权信息
- GPS定位数据
- 缩略图
```java
[JPEG] Compression Type - Baseline
[JPEG] Data Precision - 8 bits
[JPEG] Image Height - 2400 pixels
[JPEG] Image Width - 3200 pixels
[JPEG] Number of Components - 3
[JPEG] Component 1 - Y component: Quantization table 0, Sampling factors 2 horiz/2 vert
[JPEG] Component 2 - Cb component: Quantization table 1, Sampling factors 1 horiz/1 vert
[JPEG] Component 3 - Cr component: Quantization table 1, Sampling factors 1 horiz/1 vert
[Exif Thumbnail] Thumbnail Compression - JPEG (old-style)
[Exif Thumbnail] X Resolution - 72 dots per inch
[Exif Thumbnail] Y Resolution - 72 dots per inch
[Exif Thumbnail] Resolution Unit - Inch
[Exif Thumbnail] Thumbnail Offset - 4860 bytes
[Exif Thumbnail] Thumbnail Length - 26551 bytes
[Exif SubIFD] F-Number - f/2.4
[Exif SubIFD] Exif Version - 2.20
[Exif SubIFD] Date/Time Original - 2012:03:30 13:37:36
[Exif SubIFD] Components Configuration - YCbCr
[Exif SubIFD] Aperture Value - f/2.4
[Exif SubIFD] Focal Length - 4.31 mm
[Exif SubIFD] Makernote - 1
[Exif SubIFD] FlashPix Version - 1.00
[Exif SubIFD] Color Space - sRGB
[Exif SubIFD] Exif Image Width - 3200 pixels
[Exif SubIFD] Exif Image Height - 2400 pixels
[Exif SubIFD] Padding - [2060 bytes]
[Exif SubIFD] Unknown tag (0xea1d) - 0
[Xmp] XMP Value Count - 3
[Xmp] Rating - 4.0
[GPS] GPS Latitude Ref - N
[GPS] GPS Latitude - 37° 51' 42.54"
[GPS] GPS Longitude Ref - E
[GPS] GPS Longitude - 112° 34' 5.73"
[GPS] GPS Altitude Ref - Sea level
[GPS] GPS Altitude - 0 metres
[GPS] GPS Time-Stamp - 05:37:33.00 UTC
[GPS] GPS Processing Method - ASCII
[GPS] GPS Date Stamp - 2012:03:30
[Interoperability] Interoperability Version - 1.00
[Exif IFD0] Make - Xiaomi
[Exif IFD0] Model - mione_plus
[Exif IFD0] X Resolution - 72 dots per inch
[Exif IFD0] Y Resolution - 72 dots per inch
[Exif IFD0] Resolution Unit - Inch
[Exif IFD0] YCbCr Positioning - Center of pixel array
[Exif IFD0] Rating - 4
[Exif IFD0] Unknown tag (0x4749) - 75
[Exif IFD0] Padding - [2060 bytes]
[File] File Name - IMG_20120330_133736.jpg
[File] File Size - 4374893 bytes
[File] File Modified Date - Fri Mar 30 16:22:08 CST 2012
```
在一些不知名的 APP 上，一定不要发送“原图（原始图片）信息”。最好是能够进行压缩，转码，擦除照片图片中的 Exif GPS 等隐私信息。但是也不要怕，对于微信、微博、淘宝等大厂来说，他们常用的 APP 都进行了“隐私保护”。除非通过一些非常规手段，发送原图，压缩包等方式泄露个人信息。
