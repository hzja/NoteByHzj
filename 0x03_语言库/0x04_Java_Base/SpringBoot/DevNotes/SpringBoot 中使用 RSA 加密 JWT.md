JavaSpringBootRSAJWT
<a name="S0bZG"></a>
## 1、引入 nimbus-jose-jwt
```xml
<dependency>
  <groupId>com.nimbusds</groupId>
  <artifactId>nimbus-jose-jwt</artifactId>
  <version>9.23</version>
</dependency>
```
该包可以使用rsa算法进行jwt加密
<a name="I7V3j"></a>
## 2、使用openssl生成密钥
生成RSA加密私钥
```bash
openssl genrsa -aes256 -passout pass:123456 -out rsa_aes_private.key 1024
```
**使用RSA私钥生成公钥**
```bash
openssl rsa -in rsa_aes_private.key -pubout -out rsa_public.key
```
因为使用 openssl 生成的密钥是 pkcs1格式的密钥，java默认只能使用 pkcs8 格式的密钥，所以需要进行pkcs1到pkcs8转换的转换
```bash
openssl pkcs8 -topk8 -in rsa_aes_private.key -inform pem -out pkcs8_rsa_private.key -outform pem -nocrypt
```
<a name="bFM86"></a>
## 3、在application.yml增加配置
```yaml
certificate:
  useKid: k1
  certificates:
    - kid: k1
      privateKey: |
        -----BEGIN ENCRYPTED PRIVATE KEY-----
        MIIC3TBXBgkqhkiG9w0BBQ0wSjApBgkqhkiG9w0BBQwwHAQImUIM57O4TH4CAggA
        MAwGCCqGSIb3DQIJBQAwHQYJYIZIAWUDBAEqBBDJkNNh8w3fTcQjKP3A6oVHBIIC
        gC7Nuk2xzW2+CHycQ5InCB76u/C1L6jTKC8M7XgAhacM7WfQHHfJFjMsN9J94vwd
        8rDlTPE+nNHmLw386fBFtwDTLC8cuALmcvzH+qxYVXD5ygYGRrclUulOiRwiZ5f4
        TjdmHApP15SbglG/B4tV5ERa2nudccXDdg7fAJsqlaZsqLGnPxYBhbUwE428DFjn
        MkyA2N06AQzyU7aFYeuKGSS5D04HRAyZ/SBVUg4lBXI34TAZGG447LhHxXuorBgH
        N/JJpHGgQyURmH43HI4bpiPnXHbHTRNYUehQGUI/oNWAZugFLFrXnYl120+wkca+
        U8zQu/23uhy+4iCuy5SnNxdOKvSNpBTIh2BEbEm8nmHvbcfg5pcgExb/g7rnFWPP
        ryNdR42Vm5Wp4xrzFT71WwWSUVkC1N037QH0K09BTcJi/XV6qxxOtLSfq2uzTJQ2
        vIs9VGgKy9IAlIa6aur1Th/cpbQ+dz9ld1ZYWHgBxw8hFxZkbu+qZUeAo6c1pHZI
        rwPvYj06BK5R2xkrMYcJaEasJz0PrvxMzk9+0qSJNdT+y9nzaxLN+/ypldm3DarH
        ZiyG5QC/TJTWkckM0AIdZujLIs8j3IQc4Sp21zrjFeMBzVd3CJBGgaFAV3o6CaAp
        9OJYytj/cNAy1jEfTl7AbaRAbteBbSFQdAsSGqgC0u+JpyncH1r3YoM720HIB7Xa
        pLyOCA3zWcbKPwHTBlH1x7+ppXy/zvdAwmUlTydD1aaa2i4bv2+ctdjWhMW77Nxp
        TE3y5Kim8CSW885PgIRxKocU0DgeOEtPmuOxxMjbouSF18mSmZP8NmoiVMpf/cS9
        9c4FlRjxWiPoRY+EMWk81cU=
        -----END ENCRYPTED PRIVATE KEY-----  
      publicKey: |
        -----BEGIN PUBLIC KEY-----
        MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC1iRRYvvbs7cPGfpJuND0ArkzE
        MTzeDG0OuTxcdl+A6yOB6jRRe/9fqRRZJSZOcrvr8I2YKs9n+DGo2nGUrmcRlav5
        s77g7jGJmOW6ZvNBBU4g+w9gt4tQPCBcyjSym4HUWz04FT03cfa6Yn7f2xHwjw7j
        3DXGZnp+qx3fox5ezwIDAQAB
        -----END PUBLIC KEY-----
    - kid: k2
      privateKey: |
        -----BEGIN ENCRYPTED PRIVATE KEY-----
        MIIC3TBXBgkqhkiG9w0BBQ0wSjApBgkqhkiG9w0BBQwwHAQI1wWdC35fHiMCAggA
        MAwGCCqGSIb3DQIJBQAwHQYJYIZIAWUDBAEqBBAd8fAg67XxjRww/6zZOYiUBIIC
        gFELh2xUblnGRTGQQ7VaRayAMHd1hNNc2P6Pe0ujXxzf1qF/iE6Rhpk2Ag9I0ky+
        siZL8lbh3QzOyQUch+BBYBuUeToSN8jQ+5rTR6Vr8eaC6jRKIZCBdxbbN4r3hSkJ
        nG+BZzjArO8U0T+QTQ3cp3rpvOsRj5xR09nku3HnaY9vNqWAWd5mjJ+b8lRnh64X
        qAAmnBUcB+5xuU/BMGpF8k2X/qNVRM/YZzP1/mWO31kw1VMjSP6zdrRVQuoUv2X8
        bUlv6kBBaaTPK4qkR6y3I7QMiGxA1UL8qTPtxpOlaWR+3ofIBPk1N35k1sFLRgc/
        UhK7q0KphZ75BZGu1PpOu8T0p2fx0BnIQUZKg7+g5oqzNKKo9RlVx87wrJO1Urur
        VJS+FzK1HoGqLtsco2lzeLqmXqCbC+MXygenJJxOZOKW9/LWQxMP3e72/N0LzdqW
        0lbu9f4w86OL7Qwk4zVxCFp9bwDAvf5ZIvfnLI1yl8q7cfny6QKGU+nwENOWn3Px
        I7Dv7vNIs9K6f6Is8XPnEnBIRi2eUwVHVqeu46DLIIzS+YIvsDfkHp3h7fBh7hYW
        Iytia/QfKmpyeZp5GCJZM0pLP0qDLspXdm0oBI+WdnbF2i9YUADGdQw1CpgskYnf
        wBNDdGmkUR4aTDdwvdzPacRDF3ZZg/AaiysuWRIjEsEabwmpi4CmmLiwxwnO5uDn
        4iiLC16PUPK+sIBskYd9UgOMyC+qKbzajCZVyRZDpNPZF+jZE+ND3TOtaWHimP9M
        B3dHj6F+/rHHko9kWsc/V/RaXm//14g8SBn3Hc+vR/IAz9SajMJDRVEmQnWhkfz9
        IdLgYdHew8l+HuvjDCXtmBM=
        -----END ENCRYPTED PRIVATE KEY-----
      publicKey: |
        -----BEGIN PUBLIC KEY-----
        MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC6pyUTOpppX5JOtMbT6CjS4/U8
        ZV6Tw9kViTwrILr/AwgUFaaxptaCx+waiQgBJfTE2VVXwXipXpxfYjtgKqVqx4yR
        mVAxGNZjKIkSzAxjo7er2vP084WG/Sh958MXW8A/K7pDkSALusP8YTveEgtTKfln
        feBZh04XQmRYhPPCuQIDAQAB
        -----END PUBLIC KEY-----
```
以上配置的私钥是进行了转换后的密钥
<a name="q97CA"></a>
## 4、解析配置文件
定义配置对象
```java
package com.olive.jwt;

import java.io.Serializable;

public class CertVO implements Serializable{

    private String kid;

    private String privateKey;

    private String publicKey;

    //省略getter setter

}
```
对应配置对象
```java
package com.olive.jwt;

import java.util.List;

import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.stereotype.Component;

@Component
@ConfigurationProperties(prefix="certificate")
public class CertificateConfig {

    private String useKid;

    private List<CertVO> certificates;

    //省略getter  setter
}
```
<a name="Huczs"></a>
## 5、定义 jwt payload 对象
```java
package com.olive.jwt;

import java.io.Serializable;

public class JwtPayloadVO implements Serializable{

    private String jti; //jwt token id

    private String tid; //companyId

    private String cid; //appId

    private String iss; //token使用方

    private String sub; //token主题 格式 tid:cid:uid

    private Long exp; //过期时间 毫秒

    private Long iat; //创建时间 毫秒

    private String uid; //user id

    //省略getter setter
}
```
<a name="fmmzL"></a>
## 6、进行jwt 生成与验证
```java
package com.olive.jwt;

import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.interfaces.RSAPrivateKey;
import java.security.interfaces.RSAPublicKey;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.text.ParseException;
import java.util.Base64;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javax.annotation.PostConstruct;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;
import com.nimbusds.jose.*;

@Component
public class CertificateProvider {

    @Autowired
    private CertificateConfig certificateConfig;

    private RSAPrivateKey rsaPrivateKey = null;

    private Map<String, RSASSAVerifier> verifiers = new HashMap<>();

    @PostConstruct
    public void init() {
        rsaPrivateKey = this.getPrivateKey();
    }

    public RSAPrivateKey getPrivateKey() {
        if (rsaPrivateKey != null) {
            return rsaPrivateKey;
        }
        String use = certificateConfig.getUseKid();
        if (StringUtils.isEmpty(use)) {
            System.out.println("certificate kid is required");
            return null;
        }
        List<CertVO> certVOs = certificateConfig.getCertificates();
        if (certVOs==null && certVOs.size()==0) {
            System.out.println("certificate is required");
            return null;
        }
        try {
            for (CertVO certVO : certVOs) {
                if (use.equals(certVO.getKid())) {
                    // 加载私钥
                    rsaPrivateKey = this.loadRSARSAPrivateKey(certVO.getPrivateKey());
                }
                RSAPublicKey publicKey = loadRSAPublicKey(certVO.getPublicKey());
                verifiers.put(certVO.getKid(), new RSASSAVerifier(publicKey));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        if (rsaPrivateKey != null) {
            return rsaPrivateKey;
        } else {
            System.out.println("getPrivateKey certificate kid is required，certificate kid is required");
            return null;
        }
    }

    /**
    * 加载公钥
    *
    * @param keyStr 公钥字符串
    * @return 公钥实体
    * @throws NoSuchAlgorithmException  KeyFactory中无该算法实现
    * @throws InvalidKeySpecException 密钥无法识别
    */
    private RSAPublicKey loadRSAPublicKey(String keyStr) throws NoSuchAlgorithmException, InvalidKeySpecException {
        byte[] clear = publicKeyStrToBytes(keyStr);
        X509EncodedKeySpec keySpec = new X509EncodedKeySpec(clear);
        KeyFactory fact = KeyFactory.getInstance("RSA");
        return (RSAPublicKey) fact.generatePublic(keySpec);
    }

    private RSAPrivateKey loadRSARSAPrivateKey(String keyStr) throws Exception {
        String begin = "-----BEGIN PRIVATE KEY-----";
        String end = "-----END PRIVATE KEY-----";
        String key = keyStr.replace(begin, "").replace(end, "").replaceAll("\\s", "");
        PKCS8EncodedKeySpec spec = new PKCS8EncodedKeySpec(Base64.getDecoder().decode(key));
        KeyFactory kf = KeyFactory.getInstance("RSA");
        return (RSAPrivateKey) kf.generatePrivate(spec);
    }
    /**
    * 公钥 字符串转换成二进制
    * @param keyStr 密钥字符串
    * @return 密钥/公钥 二进制
    */
    private byte[] publicKeyStrToBytes(String keyStr) {
        String begin = "-----BEGIN PUBLIC KEY-----";
        String end = "-----END PUBLIC KEY-----";
        String key = keyStr.replace(begin, "").replace(end, "").replaceAll("\\s", "");
        return Base64.getDecoder().decode(key);
    }

    public String generateAccessToken(JwtPayloadVO jwtPayloadVO) {
        Map<String, Object> playloadMap = new HashMap<>();
        playloadMap.put("jti", jwtPayloadVO.getJti());
        playloadMap.put("tid", jwtPayloadVO.getTid());
        playloadMap.put("cid", jwtPayloadVO.getCid());
        playloadMap.put("iss", jwtPayloadVO.getIss());
        playloadMap.put("sub", jwtPayloadVO.getSub());
        playloadMap.put("exp", jwtPayloadVO.getExp());
        playloadMap.put("iat", jwtPayloadVO.getIat());
        if (StringUtils.hasLength(jwtPayloadVO.getUid())) {
            playloadMap.put("uid", jwtPayloadVO.getUid());
        }
        try {
            // 创建JWS头，设置签名算法和类型
            JWSHeader header = new JWSHeader.Builder(JWSAlgorithm.RS256).keyID(certificateConfig.getUseKid())
                .type(JOSEObjectType.JWT).build();
            JWTClaimsSet claimsSet = JWTClaimsSet.parse(playloadMap);
            // 创建RSA签名器
            JWSSigner signer = new RSASSASigner(rsaPrivateKey, true);
            SignedJWT signedJWT = new SignedJWT(header, claimsSet);
            signedJWT.sign(signer);
            return signedJWT.serialize();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    public JWTClaimsSet verify(String token) throws ParseException, JOSEException {
        SignedJWT jwt = SignedJWT.parse(token);
        JWSHeader jwtHeader = jwt.getHeader();
        String keyID = jwtHeader.getKeyID();
        RSASSAVerifier verifier = verifiers.get(keyID);
        if (verifier == null) {
            System.out.println("jwt verify error: kid " + keyID + " mismatch RSASSAVerifier");
            return null;
        }
        boolean verify = jwt.verify(verifier);
        if (!verify) {
            System.out.println("jwt verify fail, invalid token: " + token);
            return null;
        }
        return jwt.getJWTClaimsSet();
    }

}
```
<a name="brypN"></a>
## 7、加载带密码的 PCKS8 PEM 私钥
上面介绍了使用 openssl 生成 PCKS1 格式的 RSA 密钥，然后再转换成 PCKS8 格式的密码。但是转换后去除了秘钥的密码。那如果没有去除密码，如何加载带有密码的密钥呢？Java 自带的 API 没有找到，如果需要实现加载带密码的 RSA 需要用到 bouncycastle 库。
<a name="CYtoJ"></a>
### pom.xml 引入
```xml
<dependency>
  <groupId>org.bouncycastle</groupId>
  <artifactId>bcprov-jdk15on</artifactId>
  <version>1.68</version>
</dependency>
<dependency>
  <groupId>org.bouncycastle</groupId>
  <artifactId>bcpkix-jdk15on</artifactId>
  <version>1.68</version>
</dependency>
```
bcprov-jdk15on和bcpkix-jdk15on是 bouncycastle 提供的两个加解密包
<a name="rhwXs"></a>
### 加载 PCKS8 密钥
代码中有三个条件判断分支

- `PKCS8EncryptedPrivateKeyInfo`：PCKS8 格式加密密钥
- `PEMEncryptedKeyPair`：PCKS1 格式加密密钥
- `PEMKeyPair`：非加密密钥
```java
import org.bouncycastle.asn1.pkcs.PrivateKeyInfo;
import org.bouncycastle.jce.provider.BouncyCastleProvider;
import org.bouncycastle.openssl.PEMEncryptedKeyPair;
import org.bouncycastle.openssl.PEMKeyPair;
import org.bouncycastle.openssl.PEMParser;
import org.bouncycastle.openssl.bc.BcPEMDecryptorProvider;
import org.bouncycastle.openssl.jcajce.JcaPEMKeyConverter;
import org.bouncycastle.operator.InputDecryptorProvider;
import org.bouncycastle.pkcs.PKCS8EncryptedPrivateKeyInfo;
import org.bouncycastle.pkcs.PKCSException;
import org.bouncycastle.pkcs.jcajce.JcePKCSPBEInputDecryptorProviderBuilder;

import java.io.IOException;
import java.io.StringReader;
import java.security.PrivateKey;
import java.security.Security;
import java.security.cert.CertificateException;

public class PKCS8Main {
    public static void main(String[] args) throws IOException, PKCSException, CertificateException {
        // https://stackoverflow.com/questions/66286457/load-an-encrypted-pcks8-pem-private-key-in-java
        System.out.println("Load an Encrypted PCKS8 PEM Private Key In Java");
        // you need 2 bouncy castle libraries, I'm using the actual ones version 1.68:
        // bcprov-jdk15on-1.68.jar   bcpkix-jdk15on-1.68.jar

        Security.addProvider(new BouncyCastleProvider());
        char[] password = "aWCTJPET9fL7UBTp97hX99gdofeWKUf5tuxSuJeST2sEkyvkyinrfrj6EiSUTErF".toCharArray();
        String privKeyStrBase64Encoded =
            "-----BEGIN ENCRYPTED PRIVATE KEY-----\n" +
            "MIIJrTBXBgkqhkiG9w0BBQ0wSjApBgkqhkiG9w0BBQwwHAQIRZpyCAQQR8oCAggA\n" +
            "MAwGCCqGSIb3DQIJBQAwHQYJYIZIAWUDBAEqBBC8h4heO8aR5NaJq5o89myBBIIJ\n" +
            "UDo2+MIuXH+y+hB9fx1kVe7yIogh0DEeYTnTLAraeg2yudSoNLq4xF2Ftid7Ax9Q\n" +
            "6BwdrY6YB8/Pk9vn7gWBoTdrfUpJZZu6DtLu7URXPTN5mgRNmJzC4uAv9y+T8CRv\n" +
            "sLXjwYOSbF4Vsbm8JlgJPuKPKkH1lTi3t1jxik+nNrDyAXRJOYfowkW9xnGqNSxq\n" +
            "GEFXN1S16msrEq+chTblSpn8liY6o2Yk405qJ2G+dz/Lv50vOjoc1GTh8vNw23ER\n" +
            "HckRiRS8TY/sjyVtI6tZp+30azbMcXgLEHFoMDIm/bu366/aYyRI2fI76LWJUuHA\n" +
            "ZmU8GYU8j0+IewLPgtAXkjkTMuvrucHgCH3/ZRDDqFI+YTj3q19r4xmPGRWoxp/c\n" +
            "2Oex3HfUCutLonof+89I5OGfvlnaESaQcSzLJSzPV6qXGMMTFVZ6G2ZW1gfaJGC1\n" +
            "VwyL2k7Ejg8ncnOPKsf27oCFOflDLvfVQc/iW/XXZfFkGpO3xgoBE1CYq9y1w4VZ\n" +
            "gzbi88AwIV2oJTqUQkNFkDMYwPnCj/D4BAT1ipysMA3Q9gs4+EnfD9nMevtef9dR\n" +
            "wYaPNcnC/dy3Soacqm6VPOQ9RP77BIMNAQKlE95lsB++DXhPs3uDyxavar0pnJn2\n" +
            "nBfPxRGbJeVc9hxyxzQtosgkrVIIt/0nEUigTukYCESpvJAd1vsupbi6v/FGXvzg\n" +
            "4aU2aGTYUWcPbsi4kZUQH8ysmaPAC4QmHBPumzmeA+k4LxUaGJaWwYxCqD+WBCye\n" +
            "D1w1utaFTjG+HSwIqGvVTxMXZswm3Tw8+bAu5UpPeIpXejWa06duCxDkvncpOW04\n" +
            "En3hJuPTBEMxzCVIlYjk3Z7bNtjOayJRqxrHlY0OkHoy8BXRSN6f9O84go8S2qie\n" +
            "MmchFTarLfG7dir9SyDJdH2bTephRdGV3nmPLX/xza3ES4JmyHEOmYxbgqnSXsSG\n" +
            "8RZQ7Cg5tQVDHf1ydgyfqJ/lcG3SaehKHVuUR/Ulv8u5WpAYoxr1aGWafUtFYSoC\n" +
            "g/RF6E9gEbrcl/KnnPEcG5jI+86BeEsfVkpjqsh10lHG008oyeI92nXzYvZv76+d\n" +
            "9bshKT6qERGlA+2HYvZkNMtuwh0eUlq0sHCKQW3D7PTerfok3EP5ohiHwIZ0cME6\n" +
            "Jq8Dz4ORGFFAdYi86NhCkW7s4nXtP1utppaZBeZLF7nxk0XYIb3NP+a/Ll9eQQSe\n" +
            "WNDdv/387J++PzpygviGmZfF3rBl253cbPX/nhhNAOiPajdN2q3qDTpAnZpE3G+v\n" +
            "t9OnNFtgmYZ0SEOMxo8T3kMVnmUhP3OsVoWcwcqIOjwPXmwCL8c4Dju8sxfZyCQh\n" +
            "rRGKsbJyON+UTXA07rpH1JzqmWSQDiir75JBsLlX56yKIzKe86CWDlohbyykVkPs\n" +
            "vT0qTgpSvkU8N3jErjZUJsIk29uMFVcVjvR1GmMiCOIG7jZ+KefjXx43JFl6Av4w\n" +
            "fjdfaMb2J2/jd9suBhGDpakftZl57Qhz/FN8yDONmetJMcum5+dwxnbB9hCP7fpJ\n" +
            "T5tnE/w0KNFJEtXeylOHlU225czr4+gdsj+ncKiTEwIm0htulrDDgKsN74EXPcor\n" +
            "ywEpc3oZZMFF8084g6j0rjAnLO/fgtf0nXfMGDGUfIGo9AJFqoYKKMu5u5vQ+XY0\n" +
            "cc93QAB1lJ1a8yyRwfUoLHNbq2AJ9NMw1sNvkRV26dpk7ecZ8LgoUdizbztz5vb2\n" +
            "6VfArHvT1pZEdjgPwsQnegu4i4/ELXZWTZu2hfIM/aTgP9avAQmQDiKEnOnOQZ/3\n" +
            "QyFEFo5qFfH4wUKbLoQXVWslIyz7bRd+F26GoJTLPOHkPAZZ3UCCzVUHwCzc78eO\n" +
            "o9V4wgfVNFNkdyXi81X97v0bKKbkxfanz0+kBSDmeOUOKTDmyFOmhbC5SKLBF3k/\n" +
            "gNHR6BzCET7ReGZ+qIVF10Oy6SzP7M/Fmt1TU2y58CoiM3pKPDUzDYX47NCoUU6c\n" +
            "S8iberxh1O4mMxDNfwQmFzXe8cst8JGBxWX2O+Oqvl9EpGojWGXY9ydut//nDRPv\n" +
            "sQyMngcK9KLAG8/JL4hp8plKee+JtNzelCHzbjObE18waF3cveKns8WaumqpgzyT\n" +
            "2nX0vct0UxBeynIefaMEwT4WbsFXu/iMrRQpsrQmxlq3LLiet5lj3UdE9EiBvQox\n" +
            "FftQdOR8t36zNzCDnHmPzmrxggiKEjDw6BFN4sV+jm6SZWzAlypplzBHGfexYS8t\n" +
            "4taG0Z6lXev9xYgAcTFZrNJRVQ18c8/8W2V+LMB86LNTG7IKa8Cbo4FMPFGQhlAZ\n" +
            "n3URL2jaV7Cf2tTuHq0IT4Sv4/dx/Cttx8qdFjGbTt3ILCpUsh9KjxTEjteA6Ydu\n" +
            "e8lYsU8C5E3mdldojkie8iZHFSjrwRuk4EyUGXRoMe900CDHXmNQ3g6Cb2cE3AgM\n" +
            "RQvCLTQgDpQe6WJ4/HFMRXtCE3dX6P46E1968MYlgn4RAmYel1HPIh/8oWaLmwxX\n" +
            "IPEO/kxjybWkrvRDw8tQxVbR8D3sdurmYuMid2EpzI1OFLPp08JcpHn+9LyvBEw9\n" +
            "9w8ngP260HSt0rckOCyRm+JWmFRmql6LRwdWl0ht1yTASDn1+/BkQm9JfOyjMxlE\n" +
            "mXFdCHL8EK0+xcYn1IMypP0oG7TA0o1BK+vsmDoEO1pT3Qh4pTA2lFAoshWR9YBP\n" +
            "ZMW2Pyscia5+wkRuL06yAujyJP5OOmHnLp9uni8tpo4OtSqt4DRCYLM9hsB2zL6Y\n" +
            "3WvgavGznflve604cZ1jkJFkzg74WgwdXXn9XrI9OoEJm84avdQIdK2WlOiA5md5\n" +
            "lMfyMVZtCZLh/6KpC8jB4t0FlOGdoQxubslWDXcJwhPFO0KvdNv+6TeWjt8ZBECI\n" +
            "zRMq+jAR6yLN0gld3y4YI37cll6kr6wNYBd0NoL7Bzl/WtPn8MJTrwcRpohNqQkJ\n" +
            "8MOrqL14yRDvPtQ2Rijzztnd3Vb9EL0Zgkrwh9uD1ZTVDyHWHnmwW7LeBOi0/vD3\n" +
            "k/bi3qVGEqAc5YkCZbydMfzw0W506WsEMlysfbAhTEx+w/IV31mfD4VwpZ+ueSyE\n" +
            "crFUOhrrE+9a34z2mSDke3Hte1pvjhIIH6B30mjFyQh3sOoouwl9PkvbuUo8Q7v2\n" +
            "ojIAhmdgVMd19lfA+ihPnmalOtR7hCwdrjoHR3N5A+Ng\n" +
            "-----END ENCRYPTED PRIVATE KEY-----";
        PrivateKey rsaPrivateKey = stringToPrivateKey(privKeyStrBase64Encoded, password);
        System.out.println("privateKey: " + rsaPrivateKey);
    }

    static public PrivateKey stringToPrivateKey(String s, char[] password)
    throws IOException, PKCSException {
        PrivateKeyInfo pki;
        try (PEMParser pemParser = new PEMParser(new StringReader(s))) {
            Object o = pemParser.readObject();
            if (o instanceof PKCS8EncryptedPrivateKeyInfo) { // encrypted private key in pkcs8-format
                System.out.println("key in pkcs8 encoding");
                PKCS8EncryptedPrivateKeyInfo epki = (PKCS8EncryptedPrivateKeyInfo) o;
                System.out.println("encryption algorithm: " + epki.getEncryptionAlgorithm().getAlgorithm());
                JcePKCSPBEInputDecryptorProviderBuilder builder =
                    new JcePKCSPBEInputDecryptorProviderBuilder().setProvider("BC");
                InputDecryptorProvider idp = builder.build(password);
                pki = epki.decryptPrivateKeyInfo(idp);
            } else if (o instanceof PEMEncryptedKeyPair) { // encrypted private key in pkcs1-format
                System.out.println("key in pkcs1 encoding");
                PEMEncryptedKeyPair epki = (PEMEncryptedKeyPair) o;
                PEMKeyPair pkp = epki.decryptKeyPair(new BcPEMDecryptorProvider(password));
                pki = pkp.getPrivateKeyInfo();
            } else if (o instanceof PEMKeyPair) { // unencrypted private key
                System.out.println("key unencrypted");
                PEMKeyPair pkp = (PEMKeyPair) o;
                pki = pkp.getPrivateKeyInfo();
            } else {
                throw new PKCSException("Invalid encrypted private key class: " + o.getClass().getName());
            }
            JcaPEMKeyConverter converter = new JcaPEMKeyConverter().setProvider("BC");
            return converter.getPrivateKey(pki);
        }
    }
}
```
