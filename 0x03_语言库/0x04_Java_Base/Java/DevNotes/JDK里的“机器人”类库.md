Java<br />Java中Robot类位于java.awt.Robot，该类用于为测试自动化，自运行演示程序和其他需要控制鼠标和键盘的应用程序生成本机系统输入事件，Robot类的主要目的是便于Java平台实现自动测试。<br />Robot可以模拟鼠标和键盘的输入，相当于Java版的按键精灵。
<a name="gIaKp"></a>
## 类介绍
![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624204047529-7bd7485a-d6f7-497e-8b87-60759547b284.webp#clientId=ua42b0bf6-54e3-4&from=paste&id=u6d9f5cde&originHeight=456&originWidth=1067&originalType=url&ratio=3&status=done&style=shadow&taskId=u4c7d1cba-db4d-4d09-9400-c4003e305cd)<br />构造方法摘要和方法摘要<br />![](https://cdn.nlark.com/yuque/0/2021/webp/396745/1624204047625-3a0e8af5-0b34-49ad-9984-25fbc154403e.webp#clientId=ua42b0bf6-54e3-4&from=paste&id=ud25ebf33&originHeight=848&originWidth=816&originalType=url&ratio=3&status=done&style=shadow&taskId=uccea7bc0-d0d4-4216-be4d-ee0fea88df4)<br />键盘上的每一个键，都可以通过`KeyEvent`的常量来标识：
<a name="Jj9sS"></a>
## 实现功能
用Robot模拟双击打开一个指定位置的文本文件，<br />然后文本里输入一段文字，<br />最后模拟用户保存文件。
<a name="JP3My"></a>
### 实例代码
```java
import java.awt.*;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;

public class Test {

    private final static int x=38; // x坐标

    private final static int y=38; // y坐标

    private final static int []keys={ // 输入字母
        KeyEvent.VK_J,KeyEvent.VK_A,KeyEvent.VK_V,KeyEvent.VK_A,
        KeyEvent.VK_1,KeyEvent.VK_2,KeyEvent.VK_3,KeyEvent.VK_4,
        KeyEvent.VK_SPACE,
        KeyEvent.VK_N,KeyEvent.VK_I,KeyEvent.VK_U,KeyEvent.VK_B,
        KeyEvent.VK_I,KeyEvent.VK_L,KeyEvent.VK_I,KeyEvent.VK_L,
        KeyEvent.VK_T,KeyEvent.VK_Y
    };


    public static void main(String[] args) throws Exception{
        Robot robot = new Robot(); // new一个机器人

        // 模拟双击打开一个指定位置的文本文件
        robot.mouseMove(38,38); // 模拟移动鼠标到指定位置
        robot.delay(100); // 休息0.1秒
        robot.mousePress(InputEvent.BUTTON1_MASK); // 模拟双击打开文本文件
        robot.mouseRelease(InputEvent.BUTTON1_MASK);
        robot.mousePress(InputEvent.BUTTON1_MASK);
        robot.mouseRelease(InputEvent.BUTTON1_MASK);
        robot.delay(1000); // 休息0.1秒

        // 模拟输入文字
        for(int i=0;i<keys.length;i++){
            robot.keyPress(keys[i]);
            robot.keyRelease(keys[i]);
            robot.delay(100);
        }

        // 模拟保存
        robot.delay(1000); // 休息0.1秒
        robot.keyPress(KeyEvent.VK_CONTROL); // 模拟 ctrl+s
        robot.keyPress(KeyEvent.VK_S);
        robot.keyRelease(KeyEvent.VK_S);
        robot.keyRelease(KeyEvent.VK_CONTROL); // 模拟释放 ctrl+s
    }
}
```
