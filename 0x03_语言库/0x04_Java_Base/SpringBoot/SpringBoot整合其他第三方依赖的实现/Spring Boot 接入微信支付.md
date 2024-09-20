Java SpringBoot
<a name="GGqTB"></a>
### 1、引入微信SDK
```xml
<dependency>
  <groupId>com.github.wxpay</groupId>
  <artifactId>wxpay-sdk</artifactId>
  <version>0.0.3</version>
</dependency>
```
<a name="QFI6Z"></a>
### 2、微信支付基本配置类
```java
public static final String APP_ID = "你的appid";
public static final String KEY = "你的api秘钥，不是appSecret";
public static final String MCH_ID = "你的商户id";
String certPath = ClassUtils.getDefaultClassLoader().getResource("").getPath()+"/weixin/apiclient_cert.p12";//从微信商户平台下载的安全证书存放的路径。
```
<a name="GuDFx"></a>
### 3、编写配置类
```java
package com.foochane.awpay.test.config;

import com.github.binarywang.wxpay.config.WxPayConfig;
import com.github.binarywang.wxpay.service.WxPayService;
import com.github.binarywang.wxpay.service.impl.WxPayServiceImpl;
import org.springframework.boot.autoconfigure.condition.ConditionalOnClass;
import org.springframework.boot.autoconfigure.condition.ConditionalOnMissingBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;

@Configuration
@ConditionalOnClass(WxPayService.class)
public class MyWxPayConfig {
    @Bean
    @ConditionalOnMissingBean
    public WxPayService wxService() {
        WxPayConfig payConfig = new WxPayConfig();
        payConfig.setAppId("xxxxxx");
        payConfig.setMchId("xxxxx");
        payConfig.setMchKey("xxxxxxxxxx");
        payConfig.setKeyPath("D:\\xx\\xx\\xxxx\\apiclient_cert.p12");
        payConfig.setUseSandboxEnv(false); //不使用沙箱环境
        WxPayService wxPayService = new WxPayServiceImpl();
        wxPayService.setConfig(payConfig);
        return wxPayService;
    }
}
```
<a name="rtNwr"></a>
### 4、支付代码
```java
package com.foochane.awpay.test.controller;
import com.github.binarywang.wxpay.bean.notify.WxPayNotifyResponse;
import com.github.binarywang.wxpay.bean.notify.WxPayOrderNotifyResult;
import com.github.binarywang.wxpay.bean.request.WxPayRefundRequest;
import com.github.binarywang.wxpay.bean.request.WxPayUnifiedOrderRequest;
import com.github.binarywang.wxpay.bean.result.*;
import com.github.binarywang.wxpay.exception.WxPayException;
import com.github.binarywang.wxpay.service.WxPayService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

@RestController
public class WxPayController {

    @Autowired
    private WxPayService wxPayService;
    /**
     * 添加支付订单
     * @param request 请求参数
     *                至少要包含如下参数，请求示例（扫描支付）：
     *                  {
     *                      "tradeType": "NATIVE",
     *                      "body": "商品购买",
     *                      "outTradeNo": "P22321112130097578901",
     *                      "productId": "12342424242323233",
     *                      "totalFee": 1,
     *                      "spbillCreateIp": "12.3.44.4",
     *                      "notifyUrl":"http://www.xxxx.com:/wx/order/notify"
     *                  }
     * @return 返回支付信息
     * @throws  WxPayException
     */
    @ResponseBody
    @RequestMapping(value = "wx/pay/order/create",method = RequestMethod.POST)
    public WxPayUnifiedOrderResult unifiedOrder(@RequestBody WxPayUnifiedOrderRequest request) throws WxPayException {
        return this.wxPayService.unifiedOrder(request);
    }
    /**
     * 支付回调通知处理
     * @param xmlData
     * @return
     * @throws WxPayException
     */
    @PostMapping("wx/order/notify")
    public String parseOrderNotifyResult(@RequestBody String xmlData) throws WxPayException {
        final WxPayOrderNotifyResult notifyResult = this.wxPayService.parseOrderNotifyResult(xmlData);
        // TODO 根据自己业务场景需要构造返回对象
        return WxPayNotifyResponse.success("成功");
    }
    /**
     * 查询订单
     * @param transactionId 微信订单号
     * @param outTradeNo    商户系统内部的订单号，当没提供transactionId时需要传这个,两个参数二选一即可
     */
    @GetMapping("/wx/par/order/query")
    public WxPayOrderQueryResult queryOrder(@RequestParam(required = false) String transactionId,
                                            @RequestParam(required = false) String outTradeNo)
            throws WxPayException {
        return this.wxPayService.queryOrder(transactionId, outTradeNo);
    }
    /**
     * 申请退款
     * @param request 请求对象
     *                请求示例(至少包含如下参数）：
     *                {
     *                  "outRefundNo": "rxx34343121",
     *                  "outTradeNo": "p22321213009757890",
     *                  "refundAccount": "REFUND_SOURCE_UNSETTLED_FUNDS",
     *                  "refundDesc": "退款",
     *                  "refundFee": 1,
     *                  "totalFee": 1,
     *                  "notifyUrl": "http://www.xxxx.com/wx/notify
     *               }
     * @return 退款操作结果
     */
    @PostMapping("/wx/refund/order/create")
    public WxPayRefundResult refund(@RequestBody WxPayRefundRequest request) throws WxPayException {
        return this.wxPayService.refund(request);
    }
    /**
     * 微信支付-查询退款
     * 以下四个参数四选一
     *
     * @param transactionId 微信订单号
     * @param outTradeNo    商户订单号
     * @param outRefundNo   商户退款单号
     * @param refundId      微信退款单号
     * @return 退款信息
     */
    @GetMapping("/wx/refund/order/query")
    public WxPayRefundQueryResult refundQuery(@RequestParam(required = false) String transactionId,
                                              @RequestParam(required = false) String outTradeNo,
                                              @RequestParam(required = false) String outRefundNo,
                                              @RequestParam(required = false) String refundId)
            throws WxPayException {
        return this.wxPayService.refundQuery(transactionId, outTradeNo, outRefundNo, refundId);
    }
    /**
     * 关闭订单
     * @param outTradeNo 商户系统内部的订单号
     */
    @GetMapping("/wx/order/close{outTradeNo}")
    public WxPayOrderCloseResult closeOrder(@PathVariable String outTradeNo) throws WxPayException {
        return this.wxPayService.closeOrder(outTradeNo);
    }
}
```
<a name="pvHJm"></a>
### 5、支付实现结果
只需要修改配置文件中自己的appid等信息就可以使用了。文中没有Java后台的业务逻辑，可以自己添加，这里主要实现支付功能。（为了隐私，截图中屏蔽了主体信息。）<br />![2021-05-30-10-00-49-968373.png](https://cdn.nlark.com/yuque/0/2021/png/396745/1622340091000-e4f5892d-7243-48e0-8d0a-9e16b486dfa7.png#clientId=u4d17c87d-0b87-4&from=ui&id=u5f4f78d8&originHeight=844&originWidth=502&originalType=binary&size=1274047&status=done&style=shadow&taskId=ue781bb66-335c-4de2-b4b9-2bc7e6be16e)
