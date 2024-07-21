DES
<a name="zzAwJ"></a>
###  DES加密算法
DES 加密，是对称加密，对称加密，顾名思义，加密和解密的运算全都是使用的同样的秘钥。<br />DES加密算法原始思想可以参照二战德国的恩格玛机，其基本思想大致相同。传统的密码加密都是由古代的循环移位思想而来，恩格玛机在这个基础之上进行了扩散模糊。但是本质原理都是一样的。现代DES在二进制级别做着同样的事：替代模糊，增加分析的难度。<br />![](https://cdn.nlark.com/yuque/0/2022/jpeg/396745/1666917579137-02fe6dd6-877c-4446-8f5e-a647d3164535.jpeg)<br />DES概述图
<a name="E83kK"></a>
### DES加密原理
DES 使用一个 56 位的密钥以及附加的 8 位奇偶校验位，产生最大 64 位的分组大小。<br />这是一个迭代的分组密码，使用称为 Feistel 的技术，其中将加密的文本块分成两半。<br />使用子密钥对其中一半应用循环功能，然后将输出与另一半进行“异或”运算；接着交换这两半，这一过程会继续下去，但最后一个循环不交换。<br />DES 使用 16 个循环，使用异或，置换，代换，移位操作四种基本运算。<br />虽然现在 DES 加密已经被破解，但是如果保密级别不是很高的话，依然是可以使用的。<br />既然已经知道DES 加密的过程是从明文64位开始，然后到初始置换IP，之后生成子秘钥，然后在秘钥控制下进行16轮加密转换，再做一次交换左右32比特，最后进行逆初始置换IP，最后返回密文的64位。<br />就像下面的图：<br />![](https://cdn.nlark.com/yuque/0/2022/png/396745/1666916820109-6587db5c-55a9-4a66-b98a-a2ab818b3a64.png#clientId=u8d5f9718-6c4d-4&from=paste&id=u9cdf9a64&originHeight=396&originWidth=423&originalType=url&ratio=1&rotation=0&showTitle=false&status=done&style=none&taskId=u6af46a20-56f1-4129-b394-6ba3311110e&title=)<br />具体的算法暂时不说，直接开始 Java 代码实现。
<a name="qZKFe"></a>
### DES 加密算法Java实现
```java
public class DESUtil {
 
    /**
     * 偏移变量，固定占8位字节
     */
    private final static String IV_PARAMETER = "12345678";
    /**
     * 密钥算法
     */
    private static final String ALGORITHM = "DES";
    /**
     * 加密/解密算法-工作模式-填充模式
     */
    private static final String CIPHER_ALGORITHM = "DES/CBC/PKCS5Padding";
    /**
     * 默认编码
     */
    private static final String CHARSET = "utf-8";
 
    /**
     * 生成key
     *
     * @param password
     * @return
     * @throws Exception
     */
    private static Key generateKey(String password) throws Exception {
        DESKeySpec dks = new DESKeySpec(password.getBytes(CHARSET));
        SecretKeyFactory keyFactory = SecretKeyFactory.getInstance(ALGORITHM);
        return keyFactory.generateSecret(dks);
    }
 
 
    /**
     * DES加密字符串
     *
     * @param password 加密密码，长度不能够小于8位
     * @param data 待加密字符串
     * @return 加密后内容
     */
    public static String encrypt(String password, String data) {
        if (password== null || password.length() < 8) {
            throw new RuntimeException("加密失败，key不能小于8位");
        }
        if (data == null)
            return null;
        try {
            Key secretKey = generateKey(password);
            Cipher cipher = Cipher.getInstance(CIPHER_ALGORITHM);
            IvParameterSpec iv = new IvParameterSpec(IV_PARAMETER.getBytes(CHARSET));
            cipher.init(Cipher.ENCRYPT_MODE, secretKey, iv);
            byte[] bytes = cipher.doFinal(data.getBytes(CHARSET));
 
            //JDK1.8及以上可直接使用Base64，JDK1.7及以下可以使用BASE64Encoder
            //Android平台可以使用android.util.Base64
            return new String(Base64.getEncoder().encode(bytes));
 
        } catch (Exception e) {
            e.printStackTrace();
            return data;
        }
    }
 
    /**
     * DES解密字符串
     *
     * @param password 解密密码，长度不能够小于8位
     * @param data 待解密字符串
     * @return 解密后内容
     */
    public static String decrypt(String password, String data) {
        if (password== null || password.length() < 8) {
            throw new RuntimeException("加密失败，key不能小于8位");
        }
        if (data == null)
            return null;
        try {
            Key secretKey = generateKey(password);
            Cipher cipher = Cipher.getInstance(CIPHER_ALGORITHM);
            IvParameterSpec iv = new IvParameterSpec(IV_PARAMETER.getBytes(CHARSET));
            cipher.init(Cipher.DECRYPT_MODE, secretKey, iv);
            return new String(cipher.doFinal(Base64.getDecoder().decode(data.getBytes(CHARSET))), CHARSET);
        } catch (Exception e) {
            e.printStackTrace();
            return data;
        }
    }
 
    /**
     * DES加密文件
     *
     * @param srcFile  待加密的文件
     * @param destFile 加密后存放的文件路径
     * @return 加密后的文件路径
     */
    public static String encryptFile(String password, String srcFile, String destFile) {
 
        if (password== null || password.length() < 8) {
            throw new RuntimeException("加密失败，key不能小于8位");
        }
        try {
            IvParameterSpec iv = new IvParameterSpec(IV_PARAMETER.getBytes(CHARSET));
            Cipher cipher = Cipher.getInstance(CIPHER_ALGORITHM);
            cipher.init(Cipher.ENCRYPT_MODE, generateKey(key), iv);
            InputStream is = new FileInputStream(srcFile);
            OutputStream out = new FileOutputStream(destFile);
            CipherInputStream cis = new CipherInputStream(is, cipher);
            byte[] buffer = new byte[1024];
            int r;
            while ((r = cis.read(buffer)) > 0) {
                out.write(buffer, 0, r);
            }
            cis.close();
            is.close();
            out.close();
            return destFile;
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        return null;
    }
 
    /**
     * DES解密文件
     *
     * @param srcFile  已加密的文件
     * @param destFile 解密后存放的文件路径
     * @return 解密后的文件路径
     */
    public static String decryptFile(String password, String srcFile, String destFile) {
        if (password== null || password.length() < 8) {
            throw new RuntimeException("加密失败，key不能小于8位");
        }
        try {
            File file = new File(destFile);
            if (!file.exists()) {
                file.getParentFile().mkdirs();
                file.createNewFile();
            }
            IvParameterSpec iv = new IvParameterSpec(IV_PARAMETER.getBytes(CHARSET));
            Cipher cipher = Cipher.getInstance(CIPHER_ALGORITHM);
            cipher.init(Cipher.DECRYPT_MODE, generateKey(key), iv);
            InputStream is = new FileInputStream(srcFile);
            OutputStream out = new FileOutputStream(destFile);
            CipherOutputStream cos = new CipherOutputStream(out, cipher);
            byte[] buffer = new byte[1024];
            int r;
            while ((r = is.read(buffer)) >= 0) {
                cos.write(buffer, 0, r);
            }
            cos.close();
            is.close();
            out.close();
            return destFile;
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        return null;
    }
}
```
其实 DES 加密过程如果要是简化出来的话，无非就是那么几步。<br />第一步：明文根据IP置换，变成新的明文，得到一个乱序的64 bit 明文组。<br />将新得到的加密明文分成两个部分，Lo和Ro。<br />第二步：子秘钥生成，DES加密过程有16轮循环函数，其中需要用到16个密钥，所以要将这56 bit密钥扩展生成16个48 bit 的子密钥。<br />第三步：得到16个子密钥K<br />第四步：S盒代换数据<br />第五步：P盒代换，P为固定置换，将经过S盒变换得到的32 bit进行一个置换操作。至此，得到F函数的最终输出。<br />第六步：循环16次<br />第七步：IP的逆置换<br />最后输出64位的比特密文。
