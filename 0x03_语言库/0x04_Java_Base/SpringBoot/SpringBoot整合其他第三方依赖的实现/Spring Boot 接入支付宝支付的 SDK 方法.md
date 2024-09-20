Java SpringBoot<br />支付宝去年推出了新的转账接口`alipay.fund.trans.uni.transfer`(升级后安全性更高，功能更加强大) ，老转账接口`alipay.fund.trans.toaccount.transfer`将不再维护，新老接口的一个区别就是新接口采用的证书验签方式。使用新接口要将sdk版本升级到最新版本，博主升级时最新版本是4.10.97。接下来看集成步骤：
<a name="Wjxry"></a>
### 1、将支付宝开放平台里下载的3个证书放在resources下面
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1635384030157-49122cef-57af-4f50-a617-038b538103b2.webp#clientId=udd5a4e09-cc4c-4&from=paste&id=u575d840b&originHeight=178&originWidth=578&originalType=url&ratio=1&status=done&style=none&taskId=u29aafc70-5e13-4643-b37d-8f12be69110)
<a name="O2Uyy"></a>
### 2、写支付宝支付的配置文件
alipay.properties：
```
alipay.appId=你的应用id
alipay.serverUrl=https://openapi.alipay.com/gateway.do
alipay.privateKey=你的应用私钥
alipay.format=json
alipay.charset=UTF-8
alipay.signType=RSA2
alipay.appCertPath=/cert/appCertPublicKey_2021001164652941.crt
alipay.alipayCertPath=/cert/alipayCertPublicKey_RSA2.crt
alipay.alipayRootCertPath=/cert/alipayRootCert.crt
```
<a name="iXQdV"></a>
### 3、引入pom依赖
```xml
<dependency>
  <groupId>com.alipay.sdk</groupId>
  <artifactId>alipay-sdk-java</artifactId>
  <version>4.10.97.ALL</version>
</dependency>
```
<a name="PTCJ3"></a>
### 4、将配置信息注入AliPayBean
```java
import lombok.Data;
import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.context.annotation.PropertySource;
import org.springframework.stereotype.Component;

@Component
@PropertySource("classpath:/production/alipay.properties")
@ConfigurationProperties(prefix = "alipay")
@Data
public class AliPayBean {
    private String appId;
    private String privateKey;
    private String publicKey;
    private String serverUrl;
    private String domain;
    private String format;
    private String charset;
    private String signType;
    private String appCertPath;
    private String alipayCertPath;
    private String alipayRootCertPath;

}
```
<a name="kWG3E"></a>
### 5、写配置类
```java
import com.alipay.api.AlipayClient;
import com.alipay.api.CertAlipayRequest;
import com.alipay.api.DefaultAlipayClient;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.util.FileCopyUtils;

import java.io.InputStream;


@Configuration
public class AliConfig {

    @Value("${custom.http.proxyHost}")
    private String proxyHost;
    @Value("${custom.http.proxyPort}")
    private int proxyPort;
    @Value("${spring.profiles.active}")
    private String activeEnv;

    @Autowired
    private AliPayBean aliPayBean;

    @Bean(name = {"alipayClient"})
    public AlipayClient alipayClientService() throws Exception{
        CertAlipayRequest certAlipayRequest = new CertAlipayRequest();
        //设置网关地址
        certAlipayRequest.setServerUrl(aliPayBean.getServerUrl());
        //设置应用Id
        certAlipayRequest.setAppId(aliPayBean.getAppId());
        //设置应用私钥
        certAlipayRequest.setPrivateKey(aliPayBean.getPrivateKey());
        //设置请求格式，固定值json
        certAlipayRequest.setFormat(aliPayBean.getFormat());
        //设置字符集
        certAlipayRequest.setCharset(aliPayBean.getCharset());
        //设置签名类型
        certAlipayRequest.setSignType(aliPayBean.getSignType());
        //如果是生产环境或者预演环境，则使用代理模式
        if ("prod".equals(activeEnv) || "stage".equals(activeEnv) || "test".equals(activeEnv)) {
            //设置应用公钥证书路径
            certAlipayRequest.setCertContent(getCertContentByPath(aliPayBean.getAppCertPath()));
            //设置支付宝公钥证书路径
            certAlipayRequest.setAlipayPublicCertContent(getCertContentByPath(aliPayBean.getAlipayCertPath()));
            //设置支付宝根证书路径
            certAlipayRequest.setRootCertContent(getCertContentByPath(aliPayBean.getAlipayRootCertPath()));
            certAlipayRequest.setProxyHost(proxyHost);
            certAlipayRequest.setProxyPort(proxyPort);

        }else {
            //local
            String serverPath = this.getClass().getResource("/").getPath();
            //设置应用公钥证书路径
            certAlipayRequest.setCertPath(serverPath+aliPayBean.getAppCertPath());
            //设置支付宝公钥证书路径
            certAlipayRequest.setAlipayPublicCertPath(serverPath+aliPayBean.getAlipayCertPath());
            //设置支付宝根证书路径
            certAlipayRequest.setRootCertPath(serverPath+aliPayBean.getAlipayRootCertPath());
        }
        return new DefaultAlipayClient(certAlipayRequest);
    }
    public String getCertContentByPath(String name){
        InputStream inputStream = null;
        String content = null;
        try{
            inputStream = this.getClass().getClassLoader().getResourceAsStream(name);
            content = new String(FileCopyUtils.copyToByteArray(inputStream));
        }catch (Exception e){
            e.printStackTrace();
        }
        return content;
    }

}
```
<a name="I6khl"></a>
### 6、写支付工具类
```java
import com.alipay.api.AlipayApiException;
import com.alipay.api.AlipayClient;
import com.alipay.api.domain.AlipayTradeAppPayModel;
import com.alipay.api.domain.AlipayTradeQueryModel;
import com.alipay.api.request.AlipayTradeAppPayRequest;
import com.alipay.api.request.AlipayTradeQueryRequest;
import com.alipay.api.response.AlipayTradeAppPayResponse;
import com.alipay.api.response.AlipayTradeQueryResponse;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Service;
/**
 * @description:支付宝工具类
 * @Date:2020-08-26
 */
@Slf4j
@Service
public class AliPayUtils {
    @Autowired
    @Qualifier("alipayClient")
    private AlipayClient alipayClient;

    /**
     * 交易查询接口
     * @param request
     * @return
     * @throws Exception
     */
    public boolean isTradeQuery(AlipayTradeQueryModel model) throws AlipayApiException {
        AlipayTradeQueryRequest request = new AlipayTradeQueryRequest();
        request.setBizModel(model);
        AlipayTradeQueryResponse alipayTradeQueryResponse = alipayClient.certificateExecute(request);
        if(alipayTradeQueryResponse.isSuccess()){
            return true;
        } else {
            return false;
        }
    }

    /**
     * app支付
     * @param model
     * @param notifyUrl
     * @return
     * @throws AlipayApiException
     */
    public String startAppPay(AlipayTradeAppPayModel model, String notifyUrl)  throws AlipayApiException {
        AlipayTradeAppPayRequest aliPayRequest = new AlipayTradeAppPayRequest();
        model.setProductCode("QUICK_MSECURITY_PAY");
        aliPayRequest.setNotifyUrl(notifyUrl);
        aliPayRequest.setBizModel(model);
        // 这里和普通的接口调用不同，使用的是sdkExecute
        AlipayTradeAppPayResponse aliResponse = alipayClient.sdkExecute(aliPayRequest);
        return aliResponse.getBody();
    }
    /**
     * 转账接口
     *
     * @param transferParams
     * @return AlipayFundTransToaccountTransferResponse
     */
     public AlipayFundTransUniTransferResponse doTransferNew(TransferParams transferParams) throws Exception {

        String title = (StringUtils.isNotBlank(transferParams.getRemark()) ? transferParams
                .getRemark() : "转账");
        //转账请求入参
        AlipayFundTransUniTransferRequest request = new AlipayFundTransUniTransferRequest();
        //转账参数
        BizContentForUniTransfer bizContent = new BizContentForUniTransfer();
        bizContent.setOut_biz_no(transferParams.getOutBizNo());
        bizContent.setTrans_amount(MathUtil.changeF2Y(Math.abs(Integer.parseInt(transferParams.getAmount()))));
        bizContent.setProduct_code("TRANS_ACCOUNT_NO_PWD");
        bizContent.setBiz_scene("DIRECT_TRANSFER");
        bizContent.setOrder_title(title);
        Participant participant = new Participant();
        participant.setIdentity(transferParams.getPayeeAccount());
        participant.setIdentity_type(transferParams.getPayeeType());
        participant.setName((StringUtils.isNotBlank(transferParams.getPayeeRealName()) ? transferParams
                .getPayeeRealName() : StringUtils.EMPTY));
        bizContent.setPayee_info(participant);
        bizContent.setRemark(title);

        request.setBizContent(JSON.toJSONString(bizContent));

        //转账请求返回
        AlipayFundTransUniTransferResponse response = null;
        try {
            response = alipayClient.certificateExecute(request);
        } catch (Exception e) {

            log.info("doTransfer exception，异常信息：{}", e.toString());

            log.info("doTransfer exception，支付宝返回信息：{}", JSONObject.toJSONString(response));

        }

        log.info("doTransfer,AlipayFundTransUniTransferResponse:{}", JSONObject.toJSONString(response));

        return response;
    }
}
```
Tips：转账用到的类
```java
@Data
public class TransferParams {

    /**
     * 应用编号
     */
    private Long appId;

    /**
     * 创建人id
     */
    private Long createdBy;

    /**
     * 转账业务订单号
     */
    private String outBizNo;

    /**
     * 收款方识别方式
     */
    private String payeeType;

    /**
     * 收款方账号，可以是支付宝userId或者支付宝loginId
     */
    private String payeeAccount;

    /**
     * 转账金额，单位分
     */
    private String amount;

    /**
     * 付款方名称
     */
    private String payerShowName;

    /**
     * 收款方名称
     */
    private String payeeRealName;

    /**
     * 备注
     */
    private String remark;

    /**
     * 支付宝转账流水号
     */
    private String orderId;
}

import lombok.Data;

import java.math.BigDecimal;

/**
 * 支付宝转账参数
 */
@Data
public class BizContentForUniTransfer {
    /**
     * 业务订单号
     */
    private String out_biz_no;

    /**
     * 订单总金额，单位为元，精确到小数点后两位，
     */
    private BigDecimal trans_amount;

    /**
     * 业务产品码，
     * 单笔无密转账到支付宝账户固定为:TRANS_ACCOUNT_NO_PWD；
     * 单笔无密转账到银行卡固定为:TRANS_BANKCARD_NO_PWD;
     * 收发现金红包固定为:STD_RED_PACKET；
     */
    private String product_code;

    /**
     * 描述特定的业务场景，可传的参数如下：
     * DIRECT_TRANSFER：单笔无密转账到支付宝/银行卡, B2C现金红包;
     * PERSONAL_COLLECTION：C2C现金红包-领红包
     */
    private String biz_scene;

    /**
     * 转账业务的标题，用于在支付宝用户的账单里显示
     */
    private String order_title;

    /**
     * 原支付宝业务单号。C2C现金红包-红包领取时，传红包支付时返回的支付宝单号；
     * B2C现金红包、单笔无密转账到支付宝/银行卡不需要该参数。
     */
    private String original_order_id;

    /**
     * 业务备注
     */
    private String remark;

    /**
     * 转账业务请求的扩展参数，支持传入的扩展参数如下：
     * 1、sub_biz_scene 子业务场景，红包业务必传，取值REDPACKET，C2C现金红包、B2C现金红包均需传入；
     * 2、withdraw_timeliness为转账到银行卡的预期到账时间，可选（不传入则默认为T1），
     * 取值T0表示预期T+0到账，取值T1表示预期T+1到账，因到账时效受银行机构处理影响，支付宝无法保证一定是T0或者T1到账；
     */
    private String business_params;

    /**
     * 支付收款对象
     */
    private Participant payee_info;
}

@Data
public class Participant {
    /**
     * 参与方的唯一标识
     */
    private String identity;

    /**
     * 参与方的标识类型，目前支持如下类型：
     * 1、ALIPAY_USER_ID 支付宝的会员ID
     * 2、ALIPAY_LOGON_ID：支付宝登录号，支持邮箱和手机号格式
     */
    private String identity_type;

    /**
     * 参与方真实姓名，如果非空，将校验收款支付宝账号姓名一致性。
     * 当identity_type=ALIPAY_LOGON_ID时，本字段必填。
     */
    private String name;
}
```
