Java
<a name="L1iOy"></a>
## 背景
最近有一个需求，用一句话来说就是：展示关注人发布的动态，这个涉及到 feed 流系统的设计。本文主要介绍一个一般企业可用的 Feed 流解决方案。
<a name="tWduZ"></a>
## 相关概念
下面先介绍一下关于 Feed 流的简单概念。
<a name="O6Rqr"></a>
### 什么是 feed 流

- **Feed**：Feed 流中的每一条状态或者消息都是 Feed，比如微博中的一条微博就是一个 Feed。
- **Feed流**：持续更新并呈现给用户内容的信息流。每个人的朋友圈，微博关注页等等都是一个 Feed 流。
<a name="qmIhn"></a>
### feed 流分类
Feed 流常见的分类有两种：

- **Timeline**：按发布的时间顺序排序，产品如果选择 Timeline 类型，那么就是认为 Feed 流中的 Feed 不多，但是每个 Feed 都很重要，都需要用户看到。类似于微信朋友圈，微博等。
- **Rank**：按某个非时间的因子排序，一般是按照用户的喜好度排序，一般用于新闻推荐类、商品推荐等。
<a name="VvaOG"></a>
## 设计
设计一个 Feed 流系统，两个关键步骤，一个是 Feed 流的 初始化，一个是 推送。关于 Feed 流的存储其实也是一个核心的点，但是这里持久化使用的还是 MySQL，后续可以考虑优化。
<a name="KnLuL"></a>
### Feed 流初始化
Feed 流【关注页 Feed 流】的初始化指的是，当用户的 Feed 流还不存在的时候，为该用户创建一个属于他自己的关注页 Feed 流，具体怎么做呢？其实很简单，遍历一遍关注列表，取出所有关注用户的 feed，将 feedId 存放到 redis 的 sortSet 中即可。这里面有几个关键点：

- **初始化数据**：初始化的数据需要从数据库中加载出来。
- **key 值**：sortSet 的 key 值需要使用当前用户的 id 做标识。
- **score 值**：如果是 Timeline 类型，直接取 feed 创建的时间戳即可。如果是 rank 类型，则把你的业务对应的权重值设进去。
<a name="ZDECC"></a>
### 推送
经过上面的初始化，已经把 feed 流放在了 redis 缓存中了。接下来就是需要更新 feed 流了，在下面四种情况需要进行更新：

- 关注的用户发布新的 feed：
- 关注的用户删除 feed。
- 用户新增关注。
- 用户取消关注。
<a name="x7WvY"></a>
#### 发布/删除 Feed 流程
上面四步具体怎么操作，会在下面的实现步骤中详细描述，在这里先重点讨论一下第一、二种情况。因为在处理 大V 【千万级别粉丝】的时候，是需要对 大V 的所有粉丝的 feed 流进行处理的，这时候涉及到的量就会非常巨大，需要多加斟酌。关于推送，一般有两种 推/拉。

- 推：A用户发布新的动态时，要往 A用户所有的粉丝 feed 流中推。
- 拉：A用户发布新的动态时，先不进行推送，而是等 粉丝进来的时候，才主动到 A用户的个人页TimeLine 拉取最新的 feed，然后进行一个 merge。如果关注了多个大V，可以并发的向多个大V 个人页TimeLine 中拉取。
<a name="WiS3p"></a>
#### 推拉结合模式
当用户发布一条新的 Feed 时，处理流程如下：

1. 先从关注列表中读取到自己的粉丝列表，以及判断自己是否是大V。
2. 将自己的Feed消息写入个人页Timeline。如果是大V，写入流程到此就结束了。
3. 如果是普通用户，还需要将自己的Feed消息写给自己的粉丝，如果有100个粉丝，那么就要写给100个用户。

当刷新自己的Feed流的时候，处理流程如下：

1. 先去读取自己关注的大V列表
2. 去读取自己的 Feed 流。
3. 如果有关注的大V，则再次并发读取每一个大V的个人页Timeline，如果关注了10个大V，那么则需要10次访问。
4. 合并2和3步的结果，然后按时间排序，返回给用户。

至此，使用推拉结合方式的发布，读取Feed流的流程都结束了。
<a name="DKmv6"></a>
#### 推模式
如果只是用推模式了，则会变的比较简单：<br />**「发布Feed：」**<br />不用区分是否大V，所有用户的流程都一样，都是三步。<br />**「读取Feed流：」**<br />不需要第一步，也不需要第三步，只需要第二步即可，将之前的2 + N(N是关注的大V个数) 次网络开销减少为 1 次网络开销。读取延时大幅降级。
<a name="zDr50"></a>
#### 两种模式总结：
推拉结合存在一个弊端，就是刷新自己的Feed流时，大V的个人页Timeline 的读压力会很大。<br />如何解决：

- 不使用大V/普通用户的优化方式，使用对活跃粉丝采用推模式，非活跃粉丝采用拉模式。
- 完全使用推模式就可以彻底解决这个问题，但是会带来存储量增大，大V Feed 发送总时间增大，从发给第一个粉丝到发给最后一个粉丝可能要几分钟时间。
<a name="PZtXo"></a>
## 实现
这里主要采用纯推模式实现了一个普通企业基本可用的 Feed 流系统，下面介绍一下具体的实现代码，主要包括3大个部分：

- 初始化 Feed 流。
- 关注的用户发布/删除 feed，该用户的粉丝更新自己的Feed流。
- 用户新增/取消关注，更新自己的Feed流。
<a name="tYEE1"></a>
### 初始化 Feed 流
当用户第一进来刷新Feed 流，且 Feed 流还不存在时，需要进行初始化，初始化的具体代码如下：核心思想就是从数据库中load出 feed 信息，塞到 zSet 中，然后分页返回。
```java
/**
 * 获取关注的人的信息流
 */
public List<FeedDto> listFocusFeed(Long userId, Integer page, Integer size) {
    String focusFeedKey = "focusFeedKey" + userId;

    // 如果 zset 为空，先初始化
    if (!zSetRedisTemplate.exists(focusFeedKey)) {
        initFocusIdeaSet(userId);
    }

    // 如果 zset 存在，但是存在 0 值
    Double zscore = zSetRedisTemplate.zscore(focusFeedKey, "0");
    if (zscore != null && zscore > 0) {
        return null;
    }

    //分页
    int offset = (page - 1) * size;

    long score = System.currentTimeMillis();
    // 按 score 值从大到小从 zSet 中取出 FeedId 集合
    List<String> list = zSetRedisTemplate.zrevrangeByScore(focusFeedKey, score, 0, offset, size);

    List<FeedDto> result = new ArrayList<>();
    if (QlchatUtil.isNotEmpty(list)) {
        for (String s : list) {
            // 根据 feedId 从缓存中 load 出 feed
            FeedDto feedDto = this.loadCache(Long.valueOf(s));
            if (feedDto != null) {
                result.add(feedDto);
            }
        }
    }
    return result;
}

/**
 * 初始化关注的人的信息流 zSet
 */
private void initFocusFeedSet( Long userId) {
    String focusFeedKey = "focusFeedKey" + userId;
    zSetRedisTemplate.del(focusIdeaKey);
    
    // 从数据库中加载当前用户关注的人发布过的 Feed
    List<Feed> list = this.feedMapper.listFocusFeed(userId);
    
    if (QlchatUtil.isEmpty(list)) {
        //保存0，避免空数据频繁查库
        zSetRedisTemplate.zadd(focusFeedKey, 1, "0");
        zSetRedisTemplate.expire(focusFeedKey, RedisKeyConstants.ONE_MINUTE * 5);
        return;
    }
    
    // 遍历 FeedList，把 FeedId 存到 zSet 中
    for (Feed feed : list) {
        zSetRedisTemplate.zadd(focusFeedKey, feed.getCreateTime().getTime(), feed.getId().toString());
    }
    
    zSetRedisTemplate.expire(focusFeedKey, 60 * 60 * 60);
}
```
<a name="PZhCx"></a>
### 关注的用户发布/删除新的 feed
每当用户发布/删除新的 feed，需要更新该用户所有的粉丝的 Feed流，该步骤一般比较耗时，所以建议异步处理，为了避免一次性load出太多的粉丝数据，这里采用循环分页查询。为了避免粉丝的 Feed流过大，会限制 Feed 流的长度为1000，当Feed流长度超过1000时，会移除最旧的 Feed。
```java
/**
 * 新增/删除 feed时，处理粉丝 feed 流
 *
 * @param userId 新增/删除 feed的用户id
 * @param feedId 新增/删除 的feedId
 * @param type   feed_add = 新增feed feed_sub = 删除feed
 */
public void handleFeed(Long userId, Long feedId, String type) {

    Integer currentPage = 1;
    Integer size = 1000;
    List<FansDto> fansDtos;

    while (true) {
        Page page = new Page();
        page.setSize(size);
        page.setPage(currentPage);
        fansDtos = this.fansService.listFans(userId, page);

        for (FansDto fansDto : fansDtos) {
            String focusFeedKey = "focusFeedKey" + userId;

            // 如果粉丝 zSet 不存在，退出
            if (!this.zSetRedisTemplate.exists(focusFeedKey)) {
                continue;
            }

            // 新增Feed
            if ("feed_add".equals(type)) {
                this.removeOldestZset(focusFeedKey);
                zSetRedisTemplate.zadd(focusFeedKey, System.currentTimeMillis(), feedId);
            }
                // 删除Feed
            else if ("feed_sub".equals(type)) {
                zSetRedisTemplate.zrem(focusFeedKey, feedId);
            }

        }

        if (fansDtos.size() < size) {
            break;
        }
        currentPage++;
    }

}

/**
 * 删除 zSet 中最旧的数据
 */
private void removeOldestZset(String focusFeedKey){
    // 如果当前 zSet 大于1000，删除最旧的数据
    if (this.zSetRedisTemplate.zcard(focusFeedKey) >= 1000) {
        // 获取当前 zSet 中 score 值最小的
        List<String> zrevrange = this.zSetRedisTemplate.zrevrange(focusFeedKey, -1, -1, String.class);
        if (QlchatUtil.isNotEmpty(zrevrange)) {
            this.zSetRedisTemplate.zrem(focusFeedKey, zrevrange.get(0));
        }
    }
}
```
<a name="Om8CA"></a>
### 用户新增关注/取消关注
这里比较简单，新增/取消关注，把新关注的 Feed 往自己的 Feed流中增加/删除 即可，但是同样需要异步处理。
```java
/**
 * 关注/取关 时，处理followerId的zSet
 *
 * @param followId   被关注的人
 * @param followerId 当前用户
 * @param type       focus = 关注 unfocus = 取关
 */
public void handleFocus( Long followId, Long followerId, String type) {

    String focusFeedKey = "focusFeedKey" + userId;

    // 如果粉丝 zSet 不存在，退出
    if (!this.zSetRedisTemplate.exists(focusFeedKey)) {
        return;
    }
    List<FeedDto> FeedDtos = this.listFeedByFollowId(source, followId);
    for (FeedDto feedDto : FeedDtos) {

        // 关注
        if ("focus".equals(type)) {
            this.removeOldestZset(focusFeedKey);
            this.zSetRedisTemplate.zadd(focusFeedKey, feedDto.getCreateTime().getTime(), feedDto.getId());
        }
            // 取关
        else if ("unfocus".equals(type)) {
            this.zSetRedisTemplate.zrem(focusFeedKey, feedDto.getId());
        }


    }
}
```
上面展示的是核心代码，仅仅是为大家提供一个实现思路，并不是直接可运行的代码，毕竟真正实现还会涉及到很多其他的无关要紧的类。
