Java
<a name="l1wHw"></a>
### 简介
维吉尼亚密码是一种经典的多表置换密码加密算法，创建于19世纪初，并在20世纪得到了广泛应用。该算法通过将明文切成若干份，并使用不同的密表加密，以达到保护数据的目的。本文将对维吉尼亚密码的算法原理进行介绍，
<a name="ILzmx"></a>
### 简单步骤
下面是维吉尼亚密码的简单步骤：

1. 创建密钥 - 选择一个关键字，例如 "LEMON"，并将其重复直至其长度与明文相等，例如 "LEMONLEMON"。
2. 将密钥转换为数字 - 将关键字转换为数字，例如 "LEMON" 每个字母在字母表中的位置是 12，5，13，15，14，将 "LEMONLEMON" 转换为 12,5,13,15,14,12,5,13,15,14。
3. 编写明文 - 编写要加密的消息，例如 "HELLO".
4. 将明文转换为数字 - 将明文中的每个字母转换为它在字母表中的位置，例如 "HELLO"会被转换为 8,5,12,12,15。
5. 加密 - 对每个明文字母使用密钥中的对应数字进行加密。例如，H 是第 8 个字母，L 的加密方式是密钥中的第 4 个数字，因此 H 要用密钥中的第 4 个数字加密。具体操作是每个明文字母加上对应关键字字母的位置数，如果超过了 26 就从头开始，如：
```
H + L = 8 + 12 = 20 -> T

E + E = 5 + 5 = 10 -> K

L + M = 12 + 13 = 25 -> Y

L + O = 12 + 15 = 1 -> B

O + N = 15 + 14 = 3 -> D
```
因此，该明文 "HELLO" 加密后为 "TKYBD"。

6. 解密 - 解密方法与加密方法相同，只需将密文中的每个数字减去其对应关键字字母的位置数，如：

这样就可以恢复原始的明文了。
```
T - L = 20 - 12 = 8 -> H

K - E = 10 - 5 = 5 -> E

Y - M = 25 - 13 = 12 -> L

B - O = 1 - 15 = 12 -> L

D - N = 3 - 14 = 15 -> O
```
<a name="u44zi"></a>
### Java语言实现
下面是Java语言实现维吉尼亚密码的示例代码：
```java
public class VigenereCipher {
    private String key;

    public VigenereCipher(String key) {
        this.key = key;
    }

    public String encrypt(String plainText) {
        String upperCasePlainText = plainText.toUpperCase();
        StringBuilder sb = new StringBuilder();
        int j = 0; // 用于记录关键词中字母的位置
        for (int i = 0; i < upperCasePlainText.length(); i++) {
            char c = upperCasePlainText.charAt(i);
            if (c < 'A' || c > 'Z') {
                continue; // 非字母字符跳过
            }
            // 关键词转换为数字，例如 A -> 0, B -> 1, C -> 2, ...
            int keyNum = (int) (this.key.charAt(j % this.key.length()) - 'A');
            // 明文字母转换为数字，例如 A -> 0, B -> 1, C -> 2, ...
            int plainNum = (int) (c - 'A');
            // 求加密后的数字，取模操作可以将超过26的数映射为0~25之间的数
            int cipherNum = (plainNum + keyNum) % 26;
            // 加密后的数字转换为密文字母，例如 0 -> A, 1 -> B, 2 -> C, ...
            char cipherChar = (char) (cipherNum + 'A');
            sb.append(cipherChar);
            j++; // 关键词中字母的位置加1
        }
        return sb.toString();
    }

    public String decrypt(String cipherText) {
        StringBuilder sb = new StringBuilder();
        int j = 0;
        for (int i = 0; i < cipherText.length(); i++) {
            char c = cipherText.charAt(i);
            if (c < 'A' || c > 'Z') {
                continue; // 非字母字符跳过
            }
            int keyNum = (int) (this.key.charAt(j % this.key.length()) - 'A');
            int cipherNum = (int) (c - 'A');
            int plainNum = (cipherNum - keyNum + 26) % 26; // 解密操作
            char plainChar = (char) (plainNum + 'A');
            sb.append(plainChar);
            j++;
        }
        return sb.toString();
    }

    public static void main(String[] args) {
        VigenereCipher cipher = new VigenereCipher("LEMON");
        String plainText = "HELLO";
        String cipherText = cipher.encrypt(plainText); // 加密
        System.out.println(cipherText);
        String decryptedText = cipher.decrypt(cipherText); // 解密
        System.out.println(decryptedText);
    }
}
```
上述代码中，VigenereCipher类实现了维吉尼亚密码的加密和解密方法。加密和解密的流程与前面的解释相同，只不过用Java代码实现了。在main方法中，定义了一个密钥为"LEMON"的维吉尼亚密码实例，然后对明文"HELLO"进行加密和解密，并输出结果。<br />注意，在实现维吉尼亚密码时，关键词对应的数字是从0开始的，而不是从1开始的。此外，加密和解密时只对字母进行操作，忽略其他字符。此处示例代码仅供参考，实际使用中可能需要进行更严谨的处理。
