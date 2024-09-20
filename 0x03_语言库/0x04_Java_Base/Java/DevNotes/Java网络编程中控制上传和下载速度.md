Java<br />常见的FTP，HTTP，BT等协议都是TCP的，但是现在流行的utorrent却基于UDP实现了自己UTP协议(UDP+拥塞控制)，不管使用什么协议，站在I/O的角度来说，限速的控制思路都是一样的。<br />思路很简单，如下：<br />1.假设下载或者上传速度上限是m (KB/s)，那么发送一个固定的字节数据(假设是n字节)的时间花费是：n/m；<br />2.假设现在要发送n字节的数据，那么理论所需的时间应该是n/m，而在实际情况下，发送n字节的数据只花费了t秒，那么发送该发送线程就应该睡眠n/m-t秒，这样就基本实现了速度的控制。<br />代码以TCP为例
<a name="f1IM1"></a>
## 速度控制
```java
public class BandwidthLimiter {

    /* KB */
    private static Long KB = 1024l;

    /* The smallest count chunk length in bytes */
    private static Long CHUNK_LENGTH = 1024l;

    /* How many bytes will be sent or receive */
    private int bytesWillBeSentOrReceive = 0;

    /* When the last piece was sent or receive */
    private long lastPieceSentOrReceiveTick = System.nanoTime();

    /* Default rate is 1024KB/s */
    private int maxRate = 1024;

    /* Time cost for sending CHUNK_LENGTH bytes in nanoseconds */
    private long timeCostPerChunk = (1000000000l * CHUNK_LENGTH)
            / (this.maxRate * KB);

    /**
     * Initialize a BandwidthLimiter object with a certain rate.
     * 
     * @param maxRate
     *            the download or upload speed in KBytes
     */
    public BandwidthLimiter(int maxRate) {
        this.setMaxRate(maxRate);
    }

    /**
     * Set the max upload or download rate in KB/s. maxRate must be grater than
     * 0. If maxRate is zero, it means there is no bandwidth limit.
     * 
     * @param maxRate
     *            If maxRate is zero, it means there is no bandwidth limit.
     * @throws IllegalArgumentException
     */
    public synchronized void setMaxRate(int maxRate)
            throws IllegalArgumentException {
        if (maxRate < 0) {
            throw new IllegalArgumentException("maxRate can not less than 0");
        }
        this.maxRate = maxRate < 0 ? 0 : maxRate;
        if (maxRate == 0)
            this.timeCostPerChunk = 0;
        else
            this.timeCostPerChunk = (1000000000l * CHUNK_LENGTH)
                    / (this.maxRate * KB);
    }

    /**
     * Next 1 byte should do bandwidth limit.
     */
    public synchronized void limitNextBytes() {
        this.limitNextBytes(1);
    }

    /**
     * Next len bytes should do bandwidth limit
     * 
     * @param len
     */
    public synchronized void limitNextBytes(int len) {
        this.bytesWillBeSentOrReceive += len;

        /* We have sent CHUNK_LENGTH bytes */
        while (this.bytesWillBeSentOrReceive > CHUNK_LENGTH) {
            long nowTick = System.nanoTime();
            long missedTime = this.timeCostPerChunk
                    - (nowTick - this.lastPieceSentOrReceiveTick);
            if (missedTime > 0) {
                try {
                    Thread.sleep(missedTime / 1000000,
                            (int) (missedTime % 1000000));
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            this.bytesWillBeSentOrReceive -= CHUNK_LENGTH;
            this.lastPieceSentOrReceiveTick = nowTick
                    + (missedTime > 0 ? missedTime : 0);
        }
    }
}
```
<a name="pjreK"></a>
### 下载控制
```java
import java.io.IOException;
import java.io.InputStream;

public class DownloadLimiter extends InputStream {
    private InputStream is = null;
    private BandwidthLimiter bandwidthLimiter = null;
    
    public DownloadLimiter(InputStream is, BandwidthLimiter bandwidthLimiter)
    {
        this.is = is;
        this.bandwidthLimiter = bandwidthLimiter;
    }
    @Override
    public int read() throws IOException {
        if(this.bandwidthLimiter != null)
            this.bandwidthLimiter.limitNextBytes();
        return this.is.read();
    }

    public int read(byte b[], int off, int len) throws IOException
    {
        if (bandwidthLimiter != null)
            bandwidthLimiter.limitNextBytes(len);
        return this.is.read(b, off, len);
    }
}
```
<a name="nrAED"></a>
### 上传控制
```java
import java.io.IOException;
import java.io.OutputStream;

public class UploadLimiter extends OutputStream {
    private OutputStream os = null;
    private BandwidthLimiter bandwidthLimiter = null;
    
    public UploadLimiter(OutputStream os, BandwidthLimiter bandwidthLimiter)
    {
        this.os = os;
        this.bandwidthLimiter = bandwidthLimiter;
    }
    
    @Override
    public void write(int b) throws IOException {
        if (bandwidthLimiter != null)
            bandwidthLimiter.limitNextBytes();
        this.os.write(b);
    }
    
    public void write(byte[] b, int off, int len) throws IOException {
        if (bandwidthLimiter != null)
            bandwidthLimiter.limitNextBytes(len);
        this.os.write(b, off, len);
    }

}
```
对于一个TCP socket
```java
ServerSocket socket = new ServerSocket();
//其它初始化略

//从socket中以一定的速率读数据
DownloadLimiter dl = new DownloadLimiter(socket.getInputStream(), new BandwidthLimiter(6250));
is = new DataInputStream(dl);

//读数据
int len = is.readInt();
ByteBuffer buffer = ByteBuffer.allocate(4 + len);
buffer.putInt(len);
is.readFully(buffer.array(), 4, buffer.remaining());


//以一定的速率写数据到socket
UploadLimiter ul = new UploadLimiter(socket.getOutputStream(), new BandwidthLimiter(6250));
ul.write();
```
在多线程环境下也可以使用上述的方法。最后附图是任务管理器的网络利用率图6250KB/s（也就是50000kb/s,附图中网络利用率也在5%左右，所以应该这个做法还算准确）<br />![](https://cdn.nlark.com/yuque/0/2021/png/396745/1623333339114-fe11d441-e4f8-4474-992f-546bf2c64576.png#clientId=ub96cc184-fe9a-4&from=paste&id=u1c5145a7&originHeight=435&originWidth=681&originalType=url&ratio=3&status=done&style=shadow&taskId=u90493ecf-3b0e-488c-b972-ad57b797191)
