JavaSpringBootVue<br />在order模块添加依赖
```xml
<dependency>
  <groupId>com.github.wxpay</groupId>
  <artifactId>wxpay-sdk</artifactId>
  <version>0.0.3</version>
</dependency>
```
在配置类添加申请的商家号信息
```
#关联的公众号appid
weixin.pay.appid=wxXXXXXXX
#商户号
weixin.pay.partner=XXXXXXXXX
#商户key
weixin.pay.partnerkey=XXXXXXXXXX
```
添加微信生成二维码service
```java
@Service
public class WeiXinServiceImpl implements WeiXinService {
    @Autowired
    private PaymentInfoService paymentInfoService;
    @Autowired
    private OrderInfoService orderInfoService;
    @Autowired
    private WeiXinService weiXinService;
    //生成支付的二维码
    @Override
    public Map createNative(Long orderId) {
        //支付记录表添加数据
        //根据单号查询订单相关信息
        OrderInfo orderInfo = orderInfoService.getById(orderId);
        if (orderInfo == null){
            throw new OrderException(20001,"订单不存在");
        }
        //添加订单状态
        paymentInfoService.savePaymentInfo(orderInfo,PaymentTypeEnum.WEIXIN.getStatus());
        //调用微信接口返回二维码
        try {
            //2 调用微信接口，得到二维码地址等信息
            //封装传递微信地址参数
            Map paramMap = new HashMap();
            paramMap.put("appid", ConstantPropertiesUtils.APPID); //公众号id
            paramMap.put("mch_id", ConstantPropertiesUtils.PARTNER); //商户号
            paramMap.put("nonce_str", WXPayUtil.generateNonceStr()); //随机字符串，调用工具类
 
            Date reserveDate = orderInfo.getReserveDate();
            String reserveDateString = new DateTime(reserveDate).toString("yyyy/MM/dd");
            String body = reserveDateString + "就诊"+ orderInfo.getDepname();
            paramMap.put("body", body);//扫码后手机显示内容
 
            paramMap.put("out_trade_no", orderInfo.getOutTradeNo()); //订单流水号
            //paramMap.put("total_fee", order.getAmount().multiply(new BigDecimal("100")).longValue()+"");
            paramMap.put("total_fee", "1");//TODO 为了测试  支付金额
            paramMap.put("spbill_create_ip", "127.0.0.1");  //终端ip
            paramMap.put("notify_url", "http://xxxxxxxxx");//回调地址
            paramMap.put("trade_type", "NATIVE"); //二维码类型
            //请求微信生成二维码接口
            HttpClient client = new HttpClient("https://api.mch.weixin.qq.com/pay/unifiedorder");
            //设置post请求相关参数
            //微信支付要求传递参数xml格式
            //把封装map集合变成xml，加密处理，传输
            String xml = WXPayUtil.generateSignedXml(paramMap, ConstantPropertiesUtils.PARTNERKEY);
            client.setXmlParam(xml);
            //支持https协议
            client.setHttps(true);
            //发送
            client.post();
 
            //调用微信接口，返回数据,xml格式的数据
            String resultXml = client.getContent();
            System.out.println("微信二维码："+resultXml);
            //把xml格式数据转换map
            Map<String, String> resultMap = WXPayUtil.xmlToMap(resultXml);
 
            Map map = new HashMap<>();
            map.put("orderId", orderId);
            map.put("totalFee", orderInfo.getAmount());
            map.put("resultCode", resultMap.get("result_code"));
            map.put("codeUrl", resultMap.get("code_url"));  //微信二维码地址
 
            return map;
        } catch (Exception e) {
            e.printStackTrace();
            throw new orderException(20001,"生成二维码失败");
        }
    }
}
```
控制层
```java
@RestController
@RequestMapping("/api/order/weixin")
public class WeixinController {
    @Autowired
    private WeiXinService weixinPayService;
    /**
     * 下单 生成二维码
     */
    @GetMapping("/createNative/{orderId}")
    public R createNative(
            @ApiParam(name = "orderId", value = "订单id", required = true)
            @PathVariable("orderId") Long orderId) {
        Map map = weixinPayService.createNative(orderId);
        return R.ok().data(map);
    }
}
```
前端微信支付二维码，wx.js定义方法
```javascript
createNative(orderId) {
  return request({
    url: `/api/order/weixin/createNative/${orderId}`,
    method: 'get'
  })
}
```
<a name="cHg1V"></a>
## 显示二维码需要前端安装插件
安装`npm install vue-qriously`<br />订单详情页，修改order/show.vue组件
```vue
import weixinApi from '@/api/yygh/wx'
 
<!-- 微信支付弹出框 -->
    <el-dialog :visible.sync="dialogPayVisible" style="text-align: left" :append-to-body="true" width="500px" @close="closeDialog">
      <div class="container">
        <div class="operate-view" style="height: 350px;">
          <div class="wrapper wechat">
            <div>
              <qriously :value="payObj.codeUrl" :size="220"/>
              <div style="text-align: center;line-height: 25px;margin-bottom: 40px;">
                请使用微信扫一扫<br/>
                扫描二维码支付
              </div>
            </div>
          </div>
        </div>
      </div>
    </el-dialog>
    
 //生成二维码
    pay() {
      //弹框
      this.dialogPayVisible = true
      //调用接口
      weixinApi.createNative(this.orderId).then(response => {
        this.payObj = response.data
        if(this.payObj.codeUrl == '') {
          this.dialogPayVisible = false
          this.$message.error("支付错误")
        } else {
          //每隔3秒查询一次支付状态
          this.timer = setInterval(()=>{
            this.queryPayStatus(this.orderId)
          },3000)
        }
      })
    },
```
查询订单支付状态，添加定时器方法，每隔3秒去查询一次支付状态，api
```javascript
queryPayStatus(orderId) {
  return request({
    url: `/api/order/weixin/queryPayStatus/${orderId}`,
    method: 'get'
  })
},
```
后端，weixinservice封装信息请求微信提供的接口，判断是否支付成功，因为微信返回的是xml文件，所以需要转换<br />支付成功后，更新状态
```java
//调用微信接口查询支付状态
@Override
public Map queryPayStatus(Long orderId, String paymentType) {
    //1 根据orderId查询订单信息
    OrderInfo orderInfo = orderInfoService.getById(orderId);
    if(orderInfo == null) {
        throw new orderException(20001,"订单不存在");
    }
    try {
        //2 封装微信接口需要数据
        Map paramMap = new HashMap<>();
        paramMap.put("appid", ConstantPropertiesUtils.APPID);
        paramMap.put("mch_id", ConstantPropertiesUtils.PARTNER);
        paramMap.put("out_trade_no", orderInfo.getOutTradeNo());
        paramMap.put("nonce_str", WXPayUtil.generateNonceStr());

        //3 调用微信接口，传递数据，设置参数
        HttpClient client = new HttpClient("https://api.mch.weixin.qq.com/pay/orderquery");
        client.setXmlParam(WXPayUtil.generateSignedXml(paramMap,ConstantPropertiesUtils.PARTNERKEY));
        client.setHttps(true);
        client.post();

        //4 获取微信接口返回数据
        String xml = client.getContent();
        System.out.println("支付状态返回xml: "+xml);
        Map<String, String> resultMap = WXPayUtil.xmlToMap(xml);

        return resultMap;

    } catch (Exception e) {
        e.printStackTrace();
        throw new orderException(20001,"查询失败");
    }
}
```
控制层，查询状态
```java
@ApiOperation(value = "查询支付状态")
@GetMapping("/queryPayStatus/{orderId}")
public Result queryPayStatus(
        @ApiParam(name = "orderId", value = "订单id", required = true)
        @PathVariable("orderId") Long orderId) {
    //调用查询接口
    Map<String, String> resultMap = weixinPayService.queryPayStatus(orderId, PaymentTypeEnum.WEIXIN.name());
    if (resultMap == null) {//出错
        return Result.fail().message("支付出错");
    }
    if ("SUCCESS".equals(resultMap.get("trade_state"))) {//如果成功
        //更改订单状态，处理支付结果
        String out_trade_no = resultMap.get("out_trade_no");
        paymentInfoService.paySuccess(out_trade_no, PaymentTypeEnum.WEIXIN.getStatus(), resultMap);
        return Result.ok().message("支付成功");
    }
    return Result.ok().message("支付中");
}
```
<a name="erCyQ"></a>
## 退款
退款与支付唯一不同的是需要在下载微信提供的退款证书，下载好后通过配置文件加载退款证书路径
```
weixin.cert=C:\\apiclient_cert.p12
```
weixinservice中
```java
//退款
@Override
public Boolean refund(Long orderId) {
    //1 根据订单号查询订单支付记录信息
    QueryWrapper<PaymentInfo> wrapper = new QueryWrapper<>();
    wrapper.eq("order_id",orderId);
    PaymentInfo paymentInfo = paymentInfoService.getOne(wrapper);

    //2 TODO 添加退款信息到退款表

    try {
        //3 调用微信退款接口
        //封装微信接口需要数据
        Map<String,String> paramMap = new HashMap<>(8);
        paramMap.put("appid",ConstantPropertiesUtils.APPID);       //公众账号ID
        paramMap.put("mch_id",ConstantPropertiesUtils.PARTNER);   //商户编号
        paramMap.put("nonce_str",WXPayUtil.generateNonceStr());
        paramMap.put("transaction_id",paymentInfo.getTradeNo()); //微信订单号
        paramMap.put("out_trade_no",paymentInfo.getOutTradeNo()); //商户订单编号
        paramMap.put("out_refund_no","tk"+paymentInfo.getOutTradeNo()); //商户退款单号
        //       paramMap.put("total_fee",paymentInfoQuery.getTotalAmount().multiply(new BigDecimal("100")).longValue()+"");
        //       paramMap.put("refund_fee",paymentInfoQuery.getTotalAmount().multiply(new BigDecimal("100")).longValue()+"");
        paramMap.put("total_fee","1");
        paramMap.put("refund_fee","1");

        //设置接口和参数
        HttpClient client = new HttpClient("https://api.mch.weixin.qq.com/secapi/pay/refund");
        client.setXmlParam(WXPayUtil.generateSignedXml(paramMap,ConstantPropertiesUtils.PARTNERKEY));
        client.setHttps(true);
        client.setCert(true);//退款证书
        client.setCertPassword(ConstantPropertiesUtils.PARTNER);//证书密码 商户key
        //发送post请求
        client.post();

        //4、返回退款数据
        String xml = client.getContent();
        Map<String, String> resultMap = WXPayUtil.xmlToMap(xml);


    } catch (Exception e) {
        e.printStackTrace();
    }

    return null;
}
```
