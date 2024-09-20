Java Redis SCAN KEYS
<a name="XbZyX"></a>
## 使用SCAN代替KEYS
```java
/**
 * redis扩展工具
 *
 * @author Fcant
 * @since 2020/2/21 23:35
 */
public abstract class RedisHelper {
    private static Logger logger = LoggerFactory.getLogger(RedisHelper.class);
    /**
     * scan 实现
     *
     * @param redisTemplate redisTemplate
     * @param pattern       表达式，如：abc*，找出所有以abc开始的键
     */
    public static Set<String> scan(RedisTemplate<String, Object> redisTemplate, String pattern) {
        return redisTemplate.execute((RedisCallback<Set<String>>) connection -> {
            Set<String> keysTmp = new HashSet<>();
            try (Cursor<byte[]> cursor = connection.scan(new ScanOptions.ScanOptionsBuilder()
                    .match(pattern)
                    .count(10000).build())) {
                while (cursor.hasNext()) {
                    keysTmp.add(new String(cursor.next(), "Utf-8"));
                }
            } catch (Exception e) {
                logger.error(e.getMessage(), e);
                throw new RuntimeException(e);
            }
            return keysTmp;
        });
    }
}
```
<a name="6css6"></a>
## 源码分析
很多文章说这种实现方式`cursor`只会被执行一次，其实这是错误的，使用这种方式`cursor`会将所有的符合条件的key都返回回来，他只是将游标的移动给封装了起来而已，真正执行查询的语句起始在`cursor.hasNext()`里面，源码如下：
```java
/*
 * (non-Javadoc)
 * @see java.util.Iterator#hasNext()
 */
@Override
public boolean hasNext() {
    assertCursorIsOpen();
 // 存放结果集的容器没有值，并且游标状态是未完成的时候进行Scan动作
    while (!delegate.hasNext() && !CursorState.FINISHED.equals(state)) {
        scan(cursorId);
    }
 // 如果结果容器还有值直接返回true,进行循环
    if (delegate.hasNext()) {
        return true;
    }
 
 // 如果结果容器没有值，但是游标不为0则表示还有值，需要进行下一次循环
    if (cursorId > 0) {
        return true;
    }
    return false;
}
private void scan(long cursorId) {
 // 进行scan操作
    ScanIteration<T> result = doScan(cursorId, this.scanOptions);
 // 结果集处理
    processScanResult(result);
}
private void processScanResult(ScanIteration<T> result) {
    if (result == null) {
  // 重置结果集容器
        resetDelegate();
  // 设置游标状态为完成
        state = CursorState.FINISHED;
        return;
    }
 // 获取当前游标位置
    cursorId = Long.valueOf(result.getCursorId());
    if (isFinished(cursorId)) {
  // 游标返回0，设置游标状态为完成
        state = CursorState.FINISHED;
    }
    if (!CollectionUtils.isEmpty(result.getItems())) {
  // 将查询结果放到容器中
        delegate = result.iterator();
    } else {
        resetDelegate();
    }
}
```
由上面源码可以看到游标的移动是在`processScanResult()`方法中完成。通过`state`来记录当前游标状态，大致过程为：

1. 当存放结果集的容器没有值，并且游标状态是未完成的时候进行Scan动作
2. 如果结果容器还有值直接返回true,进行循环
3. 如果结果容器没有值，但是游标不为0则表示还有值，需要进行下一次循环
