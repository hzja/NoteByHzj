Java<br />当前使用的是Linux系统，已经安装OpenSSL软件包。
<a name="Qt2cy"></a>
## 一、使用OpenSSL来生成私钥和公钥
<a name="H68BW"></a>
### 检查OpenSSL的安装

- **执行命令**`**openssl version -a**`** 验证机器上已经安装openssl**
```bash
$ openssl version -a
```
![image.png](https://cdn.nlark.com/yuque/0/2022/png/396745/1661085466501-20f1775b-0e13-431b-bd87-f4f55acd71f8.png#clientId=ueb60f103-c2e0-4&from=paste&height=396&id=ub6251dfb&originHeight=991&originWidth=3313&originalType=binary&ratio=1&rotation=0&showTitle=false&size=938015&status=done&style=none&taskId=u2afe4c88-0023-4823-a332-24fb016303d&title=&width=1325.2)
<a name="p6MnP"></a>
### 生成私钥
**这条命令让openssl随机生成一份私钥，加密长度是1024位。加密长度是指理论上最大允许”被加密的信息“长度的限制，也就是明文的长度限制。随着这个参数的增大（比方说2048），允许的明文长度也会增加，但同时也会造成计算复杂度的极速增长。一般推荐的长度就是2048位。**
```bash
$ openssl genrsa -out rsa_private_key.pem 2048
```
运行结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661085210968-04335b35-d9c3-4cee-a6bc-32eb422afb4d.png#clientId=ueb60f103-c2e0-4&from=paste&id=u371d7850&originHeight=143&originWidth=804&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u28b0dcac-9582-4f5c-b300-96dabee2ba6&title=)<br />生成私钥文件：C:\Users\PC\rsa_private_key.pem，内容都是标准的ASCII字符，开头一行和结尾一行有明显的标记，真正的私钥数据是中间的不规则字符。
```bash
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAyF3lqF456UvWSyvj5iP5oS4hIprE1fUqR0hHXMHN48f/4ycj
tAml3MSHHeE0j3cnutI5xh5WlZAKI1GLZwcZeg0WNDFiRP6pAdz32w==
-----END RSA PRIVATE KEY-----
```
<a name="OdXWv"></a>
### 根据私钥生成公钥

- **根据私钥生成公钥：C:\Users\PC\rsa_public_key.pem**
```bash
$ openssl rsa -in rsa_private_key.pem -pubout -out rsa_public_key.pem
```
运行结果：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1661085210885-eb4bac2c-8d9b-4ce5-a757-83786c7e8688.png#clientId=ueb60f103-c2e0-4&from=paste&id=u3859aa36&originHeight=52&originWidth=724&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u4dc50411-b8b9-4da5-ad1b-391de1d0b2a&title=)<br />公钥内容：
```bash
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAyF3lqF456UvWSyvj5iP5
grP5WGPmTggfh6vo2NLdvAngk62UgAALdvokmM4xFs/qj8bDBQ8R2Wqvl3JsX/rd
-----END PUBLIC KEY-----
```
注意：此时的私钥还不能直接被使用，需要进行PKCS#8编码：

- **PKCS#8编码：指明输入私钥文件为rsa_private_key.pem，输出私钥文件为pkcs8_rsa_private_key.pem，不采用任何二次加密（-nocrypt）**
```bash
$ openssl pkcs8 -topk8 -in rsa_private_key.pem -out pkcs8_rsa_private_key.pem -nocrypt
```
C:\Users\PC\可以看到pkcs8_rsa_private_key.pem文件。<br />至此：可用的密钥对已经生成好了，私钥使用pkcs8_rsa_private_key.pem，公钥采用rsa_public_key.pem
<a name="rEy1y"></a>
## 二、Java使用
<a name="DYaCw"></a>
### 1、读取pem文件格式
**RSAEncrypt.java**
```java
import org.apache.commons.codec.binary.Base64;
import sun.misc.BASE64Decoder;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import java.io.*;
import java.security.*;
import java.security.interfaces.RSAPrivateKey;
import java.security.interfaces.RSAPublicKey;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;

public class RSAEncrypt {
    /**
     * 字节数据转字符串专用集合
     */
    private static final char[] HEX_CHAR = {'0', '1', '2', '3', '4', '5', '6',
            '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    private static final String PRIVATE_KEY = "/pkcs8_rsa_private_key.pem";

    private static final String PUBLIC_KEY = "/rsa_public_key.pem";

    /**
     * 随机生成密钥对
     */
    public static void genKeyPair(String filePath) {
        // KeyPairGenerator类用于生成公钥和私钥对，基于RSA算法生成对象
        KeyPairGenerator keyPairGen = null;
        try {
            keyPairGen = KeyPairGenerator.getInstance("RSA");
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        // 初始化密钥对生成器，密钥大小为96-1024位
        keyPairGen.initialize(1024, new SecureRandom());
        // 生成一个密钥对，保存在keyPair中
        KeyPair keyPair = keyPairGen.generateKeyPair();
        // 得到私钥
        RSAPrivateKey privateKey = (RSAPrivateKey) keyPair.getPrivate();
        // 得到公钥
        RSAPublicKey publicKey = (RSAPublicKey) keyPair.getPublic();
        try {
            // 得到公钥字符串
            Base64 base64 = new Base64();
            String publicKeyString = new String(base64.encode(publicKey.getEncoded()));
            // 得到私钥字符串
            String privateKeyString = new String(base64.encode(privateKey.getEncoded()));
            // 将密钥对写入到文件
            FileWriter pubfw = new FileWriter(filePath + PUBLIC_KEY);
            FileWriter prifw = new FileWriter(filePath + PRIVATE_KEY);
            BufferedWriter pubbw = new BufferedWriter(pubfw);
            BufferedWriter pribw = new BufferedWriter(prifw);
            pubbw.write(publicKeyString);
            pribw.write(privateKeyString);
            pubbw.flush();
            pubbw.close();
            pubfw.close();
            pribw.flush();
            pribw.close();
            prifw.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 从文件中输入流中加载公钥
     *
     * @param path 公钥输入流
     * @throws Exception 加载公钥时产生的异常
     */
    public static String loadPublicKeyByFile(String path) throws Exception {
        try {
            BufferedReader br = new BufferedReader(new FileReader(path
                    + PUBLIC_KEY));
            String readLine = null;
            StringBuilder sb = new StringBuilder();
            while ((readLine = br.readLine()) != null) {
                if (readLine.charAt(0) == '-') {
                    continue;
                } else {
                    sb.append(readLine);
                    sb.append('\r');
                }
            }
            br.close();
            return sb.toString();
        } catch (IOException e) {
            throw new Exception("公钥数据流读取错误");
        } catch (NullPointerException e) {
            throw new Exception("公钥输入流为空");
        }
    }

    /**
     * 从字符串中加载公钥
     *
     * @param publicKeyStr 公钥数据字符串
     * @throws Exception 加载公钥时产生的异常
     */
    public static RSAPublicKey loadPublicKeyByStr(String publicKeyStr)
            throws Exception {
        try {
            BASE64Decoder base64 = new BASE64Decoder();
            byte[] buffer = base64.decodeBuffer(publicKeyStr);
            KeyFactory keyFactory = KeyFactory.getInstance("RSA");
            X509EncodedKeySpec keySpec = new X509EncodedKeySpec(buffer);
            return (RSAPublicKey) keyFactory.generatePublic(keySpec);
        } catch (NoSuchAlgorithmException e) {
            throw new Exception("无此算法");
        } catch (InvalidKeySpecException e) {
            throw new Exception("公钥非法");
        } catch (NullPointerException e) {
            throw new Exception("公钥数据为空");
        }
    }

    /**
     * 从文件中加载私钥
     *
     * @param path 私钥文件名
     * @return 是否成功
     * @throws Exception
     */
    public static String loadPrivateKeyByFile(String path) throws Exception {
        try {
            BufferedReader br = new BufferedReader(new FileReader(path
                    + PRIVATE_KEY));
            String readLine = null;
            StringBuilder sb = new StringBuilder();
            while ((readLine = br.readLine()) != null) {
                if (readLine.charAt(0) == '-') {
                    continue;
                } else {
                    sb.append(readLine);
                    sb.append('\r');
                }
            }
            br.close();
            return sb.toString();
        } catch (IOException e) {
            throw new Exception("私钥数据读取错误");
        } catch (NullPointerException e) {
            throw new Exception("私钥输入流为空");
        }
    }

    public static RSAPrivateKey loadPrivateKeyByStr(String privateKeyStr)
            throws Exception {
        try {
            BASE64Decoder base64Decoder = new BASE64Decoder();
            byte[] buffer = base64Decoder.decodeBuffer(privateKeyStr);
            PKCS8EncodedKeySpec keySpec = new PKCS8EncodedKeySpec(buffer);
            KeyFactory keyFactory = KeyFactory.getInstance("RSA");
            return (RSAPrivateKey) keyFactory.generatePrivate(keySpec);
        } catch (NoSuchAlgorithmException e) {
            throw new Exception("无此算法");
        } catch (InvalidKeySpecException e) {
            throw new Exception("私钥非法");
        } catch (NullPointerException e) {
            throw new Exception("私钥数据为空");
        }
    }

    /**
     * 公钥加密过程
     *
     * @param publicKey     公钥
     * @param plainTextData 明文数据
     * @return
     * @throws Exception 加密过程中的异常信息
     */
    public static byte[] encrypt(RSAPublicKey publicKey, byte[] plainTextData)
            throws Exception {
        if (publicKey == null) {
            throw new Exception("加密公钥为空, 请设置");
        }
        Cipher cipher = null;
        try {
            // 使用默认RSA
            cipher = Cipher.getInstance("RSA");
            // cipher= Cipher.getInstance("RSA", new BouncyCastleProvider());
            cipher.init(Cipher.ENCRYPT_MODE, publicKey);
            byte[] output = cipher.doFinal(plainTextData);
            return output;
        } catch (NoSuchAlgorithmException e) {
            throw new Exception("无此加密算法");
        } catch (NoSuchPaddingException e) {
            e.printStackTrace();
            return null;
        } catch (InvalidKeyException e) {
            throw new Exception("加密公钥非法,请检查");
        } catch (IllegalBlockSizeException e) {
            throw new Exception("明文长度非法");
        } catch (BadPaddingException e) {
            throw new Exception("明文数据已损坏");
        }
    }

    /**
     * 私钥加密过程
     *
     * @param privateKey    私钥
     * @param plainTextData 明文数据
     * @return
     * @throws Exception 加密过程中的异常信息
     */
    public static byte[] encrypt(RSAPrivateKey privateKey, byte[] plainTextData)
            throws Exception {
        if (privateKey == null) {
            throw new Exception("加密私钥为空, 请设置");
        }
        Cipher cipher = null;
        try {
            // 使用默认RSA
            cipher = Cipher.getInstance("RSA");
            cipher.init(Cipher.ENCRYPT_MODE, privateKey);
            byte[] output = cipher.doFinal(plainTextData);
            return output;
        } catch (NoSuchAlgorithmException e) {
            throw new Exception("无此加密算法");
        } catch (NoSuchPaddingException e) {
            e.printStackTrace();
            return null;
        } catch (InvalidKeyException e) {
            throw new Exception("加密私钥非法,请检查");
        } catch (IllegalBlockSizeException e) {
            throw new Exception("明文长度非法");
        } catch (BadPaddingException e) {
            throw new Exception("明文数据已损坏");
        }
    }

    /**
     * 私钥解密过程
     *
     * @param privateKey 私钥
     * @param cipherData 密文数据
     * @return 明文
     * @throws Exception 解密过程中的异常信息
     */
    public static byte[] decrypt(RSAPrivateKey privateKey, byte[] cipherData)
            throws Exception {
        if (privateKey == null) {
            throw new Exception("解密私钥为空, 请设置");
        }
        Cipher cipher = null;
        try {
            // 使用默认RSA
            cipher = Cipher.getInstance("RSA");
            // cipher= Cipher.getInstance("RSA", new BouncyCastleProvider());
            cipher.init(Cipher.DECRYPT_MODE, privateKey);
            byte[] output = cipher.doFinal(cipherData);
            return output;
        } catch (NoSuchAlgorithmException e) {
            throw new Exception("无此解密算法");
        } catch (NoSuchPaddingException e) {
            e.printStackTrace();
            return null;
        } catch (InvalidKeyException e) {
            throw new Exception("解密私钥非法,请检查");
        } catch (IllegalBlockSizeException e) {
            throw new Exception("密文长度非法");
        } catch (BadPaddingException e) {
            throw new Exception("密文数据已损坏");
        }
    }

    /**
     * 公钥解密过程
     *
     * @param publicKey  公钥
     * @param cipherData 密文数据
     * @return 明文
     * @throws Exception 解密过程中的异常信息
     */
    public static byte[] decrypt(RSAPublicKey publicKey, byte[] cipherData)
            throws Exception {
        if (publicKey == null) {
            throw new Exception("解密公钥为空, 请设置");
        }
        Cipher cipher = null;
        try {
            // 使用默认RSA
            cipher = Cipher.getInstance("RSA");
            // cipher= Cipher.getInstance("RSA", new BouncyCastleProvider());
            cipher.init(Cipher.DECRYPT_MODE, publicKey);
            byte[] output = cipher.doFinal(cipherData);
            return output;
        } catch (NoSuchAlgorithmException e) {
            throw new Exception("无此解密算法");
        } catch (NoSuchPaddingException e) {
            e.printStackTrace();
            return null;
        } catch (InvalidKeyException e) {
            throw new Exception("解密公钥非法,请检查");
        } catch (IllegalBlockSizeException e) {
            throw new Exception("密文长度非法");
        } catch (BadPaddingException e) {
            throw new Exception("密文数据已损坏");
        }
    }

    /**
     * 字节数据转十六进制字符串
     *
     * @param data 输入数据
     * @return 十六进制内容
     */
    public static String byteArrayToString(byte[] data) {
        StringBuilder stringBuilder = new StringBuilder();
        for (int i = 0; i < data.length; i++) {
            // 取出字节的高四位 作为索引得到相应的十六进制标识符 注意无符号右移
            stringBuilder.append(HEX_CHAR[(data[i] & 0xf0) >>> 4]);
            // 取出字节的低四位 作为索引得到相应的十六进制标识符
            stringBuilder.append(HEX_CHAR[(data[i] & 0x0f)]);
            if (i < data.length - 1) {
                stringBuilder.append(' ');
            }
        }
        return stringBuilder.toString();
    }
}
```
<a name="GbtkM"></a>
### 2、SHA256withRSA签名
**RSASignature.java**
```java
import java.security.PrivateKey;
import java.security.PublicKey;

public class RSASignature {

    /**
     * 签名算法
     */
    public static final String SIGN_ALGORITHMS = "hwU-gARp25KqMFVxw-9J3eCfRNXSN9QM0,ymavRvG0ByPpWEx-IJ{I0DE3w6LMP0fUwKDmSevTpFf=1Q}-h8B,14UGHs4-{flavC";

    /**
     * RSA签名
     *
     * @param content    待签名数据
     * @param privateKey 商户私钥
     * @param encode     字符集编码
     * @return 签名值
     */
    public static String sign(String content, PrivateKey privateKey, String encode) {
        try {
            java.security.Signature signature = java.security.Signature.getInstance(SIGN_ALGORITHMS);
            signature.initSign(privateKey);
            signature.update(content.getBytes(encode));
            byte[] signed = signature.sign();
            return byte2Hex(signed);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    public static String sign(String content, PrivateKey privateKey) {
        try {
            java.security.Signature signature = java.security.Signature.getInstance(SIGN_ALGORITHMS);
            signature.initSign(privateKey);
            signature.update(content.getBytes());
            byte[] signed = signature.sign();
            return byte2Hex(signed);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    /**
     * 将byte[] 转换成字符串
     */
    public static String byte2Hex(byte[] srcBytes) {
        StringBuilder hexRetSB = new StringBuilder();
        for (byte b : srcBytes) {
            String hexString = Integer.toHexString(0x00ff & b);
            hexRetSB.append(hexString.length() == 1 ? 0 : "").append(hexString);
        }
        return hexRetSB.toString();
    }

    /**
     * RSA验签名检查
     *
     * @param content   待签名数据
     * @param sign      签名值
     * @param publicKey 分配给开发商公钥
     * @param encode    字符集编码
     * @return 布尔值
     */
    public static boolean doCheck(String content, String sign, PublicKey publicKey, String encode) {
        try {
            java.security.Signature signature = java.security.Signature.getInstance(SIGN_ALGORITHMS);

            signature.initVerify(publicKey);
            signature.update(content.getBytes(encode));
            boolean bverify = signature.verify(hex2Bytes(sign));
            return bverify;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    public static boolean doCheck(String content, String sign, PublicKey publicKey) {
        try {
            java.security.Signature signature = java.security.Signature.getInstance(SIGN_ALGORITHMS);
            signature.initVerify(publicKey);
            signature.update(content.getBytes());
            boolean bverify = signature.verify(hex2Bytes(sign));
            return bverify;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }


    /**
     * 将16进制字符串转为转换成字符串
     */
    public static byte[] hex2Bytes(String source) {
        byte[] sourceBytes = new byte[source.length() / 2];
        for (int i = 0; i < sourceBytes.length; i++) {
            sourceBytes[i] = (byte) Integer.parseInt(source.substring(i * 2, i * 2 + 2), 16);
        }
        return sourceBytes;
    }
}
```
<a name="G0VrX"></a>
### 3、测试
ApplicationTests.java
```java
import org.apache.commons.codec.binary.Base64;

public class ApplicationTests {

    public static void main(String[] args) throws Exception {
        String publicPath = "C:\\Users\\PC\\Desktop\\jar"; //公匙存放位置
        String privatePath = "C:\\Users\\PC"; //私匙存放位置
        Base64 base64 = new Base64();
        System.out.println("--------------公钥加密私钥解密过程-------------------");
        String signKey = "ihep_公钥加密私钥解密";
        // 公钥加密过程
        byte[] cipherData = RSAEncrypt.encrypt(RSAEncrypt.loadPublicKeyByStr(RSAEncrypt.loadPublicKeyByFile(publicPath)),
                signKey.getBytes());
        String cipher = new String(base64.encode(cipherData));
        // 私钥解密过程
        byte[] res = RSAEncrypt.decrypt(RSAEncrypt.loadPrivateKeyByStr(RSAEncrypt.loadPrivateKeyByFile(privatePath)),
                base64.decode(cipher));
        String restr = new String(res);
        System.out.println("原文：" + signKey);
        System.out.println("加密：" + cipher);
        System.out.println("解密：" + restr);
        System.out.println();

        System.out.println("--------------私钥加密公钥解密过程-------------------");
        // 私钥加密过程
        cipherData = RSAEncrypt.encrypt(RSAEncrypt.loadPrivateKeyByStr(RSAEncrypt.loadPrivateKeyByFile(privatePath)), signKey.getBytes());
        cipher = new String(base64.encode(cipherData));
        // 公钥解密过程
        res = RSAEncrypt.decrypt(RSAEncrypt.loadPublicKeyByStr(RSAEncrypt.loadPublicKeyByFile(publicPath)), base64.decode(cipher));
        restr = new String(res);
        System.out.println("原文：" + signKey);
        System.out.println("加密：" + cipher);
        System.out.println("解密：" + restr);
        System.out.println();
    }
}
```
执行返回结果：
```java
--------------公钥加密私钥解密过程-------------------
原文：ihep_公钥加密私钥解密
加密：NoHUCeWh89dww+zcWT4+iLFlg/oJb1N7SqahbOsNCfE+F/7PZQ/tnRFaQ+TKEXpsZORQ+w0Zix40BJayWInU9jdXKc6zgafChnj/W5NHrAYNWskMGPTGzon1TBmae8V9E+EgZs7VAi2qLYSxGeczMGN8Ak08qpziGQpJp/ciStf76VkhTjSSb/4RpyxhgsJAyzTRv45traXaSj3IHP0zwYYD8gfXb3Yla/x95Ypys8/E3fggronXG6q98J+1sSF1U/4baWk/nx0s3/al60HnImWlJFCus9zz684g4iyn5PsEoRz1gpyjavXvK9cfGdPkPbitlcbMEMdQHV7cMOr1XQ==
解密：ihep_公钥加密私钥解密

--------------私钥加密公钥解密过程-------------------
原文：ihep_公钥加密私钥解密
加密：v3Y+8RDZMFTnHyMgBsBr2jqvYosqymuhY+CJESF6kPNyanYBaay0Hp++C7iHAnPs3QBTXQZVdUrd4X9dCW0TvO94t5uCM9C5hIZQa1dqhv//i5O3fK6YUfFBR8UcnGXa6OlHz3WCBdEiG/xgpKlix4e99Yk5oJVVGu+TmcsaxbrTZMSFMkKx2SQvF5AiVZWCMV4ZIqPyBe+DOtiQBDPQs8ruExVbM+L5m+3M6bWeWzlmQnwDBqxFgFdbfVqnB4hrXQwUqt4DgPTiuIlQmEpQGjTr+p+bFkxtR0itt8Bba+XBLR9uiDsQ7q4MMRZnJk0/c5e1ek15rQyAqAJKZrvdWA==
解密：ihep_公钥加密私钥解密
```
