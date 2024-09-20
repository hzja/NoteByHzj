JavaSpringBootRedis<br />在各个项目中，都可能需要用到签到和 统计功能。签到后会给用户一些礼品以此来吸引用户持续在该平台进行活跃。<br />签到功能，使用 Redis 中的 BitMap 功能来实现，就是一个非常不错的选择。
<a name="DpMZe"></a>
## 一、Redis BitMap 基本用
<a name="D5fwz"></a>
### BitMap 基本语法、指令
签到功能可以使用MySQL来完成，比如下表：<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999648839-2de6a384-214f-4349-bf80-72caad1fea8a.png#averageHue=%23fdfcfb&clientId=ucc317508-66fc-4&from=paste&id=u308c494c&originHeight=219&originWidth=675&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u10d18b1d-37c9-4ec0-93dd-5da7d7742c0&title=)<br />用户一次签到，就是一条记录，假如有1000万用户，平均每人每年签到次数为10次，则这张表一年的数据量为 1亿条<br />每签到一次需要使用（8 + 8 + 1 + 1 + 3 + 1）共22 字节的内存，一个月则最多需要600多字节<br />这样的坏处，占用内存太大了，极大的消耗内存空间！<br />可以根据 Redis中 提供的 BitMap 位图功能来实现，每次签到与未签到用0 或1 来标识 ，一次存31个数字，只用了2字节 这样就用极小的空间实现了签到功能<br />**BitMap 的操作指令：**

- **SETBIT**：向指定位置（offset）存入一个0或1
- **GETBIT**：获取指定位置（offset）的bit值
- **BITCOUNT**：统计BitMap中值为1的bit位的数量
- **BITFIELD**：操作（查询、修改、自增）BitMap中bit数组中的指定位置（offset）的值
- **BITFIELD_RO**：获取BitMap中bit数组，并以十进制形式返回
- **BITOP**：将多个BitMap的结果做位运算（与 、或、异或）
- **BITPOS**：查找bit数组中指定范围内第一个0或1出现的位置
<a name="Kzl0n"></a>
### 使用 BitMap 完成功能实现
服务器Redis版本采用 6.2<br />进入redis查询 SETBIT 命令<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999648846-51f6dd74-97b8-4f98-9be2-32d4a280898f.png#averageHue=%23040302&clientId=ucc317508-66fc-4&from=paste&id=u0722e389&originHeight=132&originWidth=728&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u207ce486-fd3e-4526-99c1-244422152cf&title=)<br />新增key 进行存储<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999648800-dc8b43fd-8bac-4b17-9d2a-57b4f396f193.png#averageHue=%23070503&clientId=ucc317508-66fc-4&from=paste&id=u8bdc62d3&originHeight=199&originWidth=454&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u65c9628f-da76-43d0-9426-63ca0f58af1&title=)<br />查询 GETBIT命令<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999648793-05e793e2-08ed-4958-831f-45f0b21d4c12.png#averageHue=%23040302&clientId=ucc317508-66fc-4&from=paste&id=ucd353200&originHeight=132&originWidth=775&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8ad679f1-24f2-43c6-9dc3-0b829d97473&title=)<br />查看指定坐标的签到状态<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999648830-f4978d0b-f5e1-439a-9537-9601e26cae14.png#averageHue=%23060403&clientId=ucc317508-66fc-4&from=paste&id=u75a31111&originHeight=60&originWidth=318&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u6a5cf7a5-415a-4c08-a1e1-dad09326d83&title=)<br />查询 BITFIELD<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999649219-01c74b22-8bce-4785-9193-b30df2dbbb77.png#averageHue=%23040302&clientId=ucc317508-66fc-4&from=paste&id=ucb0f2c17&originHeight=126&originWidth=1049&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uad1cd225-79e0-4eee-b716-c05d9a625e4&title=)<br />无符号查询<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999649234-ad62e2e8-67ec-4454-ac01-f9186e94f8d3.png#averageHue=%23090705&clientId=ucc317508-66fc-4&from=paste&id=u298cfeda&originHeight=126&originWidth=468&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9889add2-46dc-4f89-ba18-dff7c342631&title=)<br />BITPOS 查询1 和 0 第一次出现的坐标<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999649290-aefc0caf-06a0-45cb-a7bc-a53ca54fb67f.png#averageHue=%230b0806&clientId=ucc317508-66fc-4&from=paste&id=ubb5f79bf&originHeight=78&originWidth=372&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u27290d3e-3f5e-4c48-8ea8-74bc55d11d7&title=)
<a name="gZwPH"></a>
## 二、SpringBoot 整合Redis实现签到功能
<a name="Hop2B"></a>
### 需求介绍
采用BitMap实现签到功能

- 实现签到接口，将当前用户当天签到信息保存到Redis中

**思路分析：**<br />可以把 年和月 作为BitMap的key，然后保存到一个BitMap中，每次签到就到对应的位上把数字从0 变为1，只要是1，就代表是这一天签到了，反之就是没有签到。<br />实现签到接口，将当前用户当天签到信息保存至Redis中<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999649273-1c9a0b3f-ec06-4ae0-9610-76360b8ca87d.png#averageHue=%23f9f9f9&clientId=ucc317508-66fc-4&from=paste&id=u3a121440&originHeight=213&originWidth=981&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u93452a8b-1bde-4665-9ce2-08a85fa93a8&title=)<br />提示：因为BitMap 底层是基于String数据结构，因此其操作都封装在字符串操作中了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999649310-86853473-1a2e-49f5-aced-67aa6a556cc6.png#averageHue=%23d8d8c5&clientId=ucc317508-66fc-4&from=paste&id=uc483fb72&originHeight=205&originWidth=941&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc3a55918-7456-44d1-ad73-5273f93df99&title=)
<a name="KPidL"></a>
### 核心源码
UserController
```java
@PostMapping("sign")
public Result sign() {
    return userService.sign();
}
```
UserServiceImpl
```java
public Result sign() {
    //1. 获取登录用户
    Long userId = UserHolder.getUser().getId();
    //2. 获取日期
    LocalDateTime now = LocalDateTime.now();
    //3. 拼接key
    String keySuffix = now.format(DateTimeFormatter.ofPattern(":yyyyMM"));
    String key = RedisConstants.USER_SIGN_KEY + userId + keySuffix;
    //4. 获取今天是本月的第几天
    int dayOfMonth = now.getDayOfMonth();
    //5. 写入redis setbit key offset 1
    stringRedisTemplate.opsForValue().setBit(key, dayOfMonth -1, true);
    return Result.ok();
}
```
**接口进行测试**<br />ApiFox进行测试<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999649572-deccb9dc-7676-4c7a-bf5b-5f05e4954f45.png#averageHue=%23fdfdfb&clientId=ucc317508-66fc-4&from=paste&id=u6462bbdb&originHeight=688&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u1f1d7af0-acf5-4dc7-b349-3451c89564f&title=)<br />查看Redis 数据。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999649610-649b52e8-d3f2-4687-9008-4c2ec3438398.png#averageHue=%23f9f9f9&clientId=ucc317508-66fc-4&from=paste&id=uca250501&originHeight=267&originWidth=617&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ued841715-09ce-4bf7-8596-b163c30fed3&title=)
<a name="HWpbi"></a>
## 三、SpringBoot 整合Redis 实现 签到统计功能
<a name="OR448"></a>
### 问题一：什么叫做连续签到天数？
从最后一次签到开始向前统计，直到遇到第一次未签到为止，计算总的签到次数，就是连续签到天数。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999649624-85ff8e80-32b8-4b07-845f-7ea77e9c6213.png#averageHue=%23fafafa&clientId=ucc317508-66fc-4&from=paste&id=uea2a95bd&originHeight=107&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5c988fac-fc81-4f27-95a1-576fb669907&title=)<br />逻辑分析：<br />获得当前这个月的最后一次签到数据，定义一个计数器，然后不停的向前统计，直到获得第一个非0的数字即可，每得到一个非0的数字计数器+1，直到遍历完所有的数据，就可以获得当前月的签到总天数了
<a name="HIZRG"></a>
### 问题二：如何得到本月到今天为止的所有签到数据？
```
BITFIELD key GET u[dayOfMonth] 0
```
假设今天是7号，那么就可以从当前月的第一天开始，获得到当前这一天的位数，是7号，那么就是7位，去拿这段时间的数据，就能拿到所有的数据了，那么这7天里边签到了多少次呢？统计有多少个1即可。
<a name="lPrpF"></a>
### 问题三：如何从后向前遍历每个Bit位？
注意：bitMap返回的数据是10进制，哪假如说返回一个数字8，那么怎么知道到底哪些是0，哪些是1呢？<br />只需要让得到的10进制数字和1做与运算就可以了，因为1只有遇见1 才是1，其他数字都是0 ，把签到结果和1进行与操作，每与一次，就把签到结果向右移动一位，依次内推，就能完成逐个遍历的效果了。<br />需求：实现以下接口，统计当前截至当前时间在本月的连续天数<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999649785-01a4a76c-9d26-4220-ba9e-a36234467c88.png#averageHue=%23f9f9f8&clientId=ucc317508-66fc-4&from=paste&id=u88360071&originHeight=216&originWidth=984&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uda370da7-aa82-4eef-a8ca-a172947a1fa&title=)<br />有用户有时间就可以组织出对应的key，此时就能找到这个用户截止这天的所有签到记录，再根据这套算法，就能统计出来他连续签到的次数了<br />**核心源码**<br />UserController
```java
@GetMapping("/signCount")
public Result signCount() {
    return userService.signCount();
}
```
UserServiceImpl
```java
public Result signCount() {
    //1. 获取登录用户
    Long userId = UserHolder.getUser().getId();
    //2. 获取日期
    LocalDateTime now = LocalDateTime.now();
    //3. 拼接key
    String keySuffix = now.format(DateTimeFormatter.ofPattern(":yyyyMM"));
    String key = RedisConstants.USER_SIGN_KEY + userId + keySuffix;
    //4. 获取今天是本月的第几天
    int dayOfMonth = now.getDayOfMonth();
    //5. 获取本月截至今天为止的所有的签到记录，返回的是一个十进制的数字 BITFIELD sign:5:202301 GET u3 0
    List<Long> result = stringRedisTemplate.opsForValue().bitField(
        key,
        BitFieldSubCommands.create()
        .get(BitFieldSubCommands.BitFieldType.unsigned(dayOfMonth)).valueAt(0));
    //没有任务签到结果
    if (result == null || result.isEmpty()) {
        return Result.ok(0);
    }
    Long num = result.get(0);
    if (num == null || num == 0) {
        return Result.ok(0);
    }
    //6. 循环遍历
    int count = 0;
    while (true) {
        //6.1 让这个数字与1 做与运算，得到数字的最后一个bit位 判断这个数字是否为0
        if ((num & 1) == 0) {
            //如果为0，签到结束
            break;
        } else {
            count ++;
        }
        num >>>= 1;
    }
    return Result.ok(count);
}
```
**进行测试**<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999649762-6d87d1d9-a1f3-4e70-9ab9-f2c4f00fad44.png#averageHue=%23fdfdfb&clientId=ucc317508-66fc-4&from=paste&id=u0f148e34&originHeight=581&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc34a4014-6604-4e94-849f-5b8e52ed638&title=)<br />查看 Redis 变量<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999649979-60b858a1-65d4-4a67-9737-34b03878cc56.png#averageHue=%23f8f7f7&clientId=ucc317508-66fc-4&from=paste&id=uce33ebb8&originHeight=214&originWidth=564&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u9bf0f435-f8d1-4f70-811d-68aa2e0cdd8&title=)<br />从今天开始，往前查询 连续签到的天数，结果为2 测试无误！
<a name="FstIh"></a>
## 四、关于使用bitmap来解决缓存穿透的方案
回顾缓存穿透：发起了一个数据库不存在的，redis里边也不存在的数据，通常可以把他看成一个攻击。<br />解决方案：

- 判断id<0
- 数据库为空的话，向redis里边把这个空数据缓存起来

第一种解决方案：遇到的问题是如果用户访问的是id不存在的数据，则此时就无法生效<br />第二种解决方案：遇到的问题是：如果是不同的id那就可以防止下次过来直击数据<br />所以如何解决呢？<br />可以将数据库的数据，所对应的id写入到一个list集合中，当用户过来访问的时候，直接去判断list中是否包含当前的要查询的数据，如果说用户要查询的id数据并不在list集合中，则直接返回，如果list中包含对应查询的id数据，则说明不是一次缓存穿透数据，则直接放行。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999649994-0ff27ce8-d2ad-432f-8ecd-55efc67482dd.png#averageHue=%23f8f8f8&clientId=ucc317508-66fc-4&from=paste&id=uc7f4dde0&originHeight=274&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud036c9fd-5752-45c3-8e18-29762bfb7b2&title=)<br />现在的问题是这个主键其实并没有那么短，而是很长的一个 主键<br />哪怕单独去提取这个主键，但是在 11年左右，淘宝的商品总量就已经超过10亿个<br />所以如果采用以上方案，这个list也会很大，所以可以使用bitmap来减少list的存储空间<br />可以把list数据抽象成一个非常大的bitmap，不再使用list，而是将db中的id数据利用哈希思想，比如：<br />id 求余bitmap长度 ：id % bitmap.size = 算出当前这个id对应应该落在bitmap的哪个索引上，然后将这个值从0变成1，然后当用户来查询数据时，此时已经没有了list，让用户用他查询的id去用相同的哈希算法， 算出来当前这个id应当落在bitmap的哪一位，然后判断这一位是0，还是1，如果是0则表明这一位上的数据一定不存在，采用这种方式来处理，需要重点考虑一个事情，就是误差率，所谓的误差率就是指当发生哈希冲突的时候，产生的误差。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1696999650102-f50bc497-528c-4149-80aa-b7b9c819c2e4.png#averageHue=%23f8f8f8&clientId=ucc317508-66fc-4&from=paste&id=u1e7f8b55&originHeight=455&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uae3ee436-9011-4e01-a621-c882cc0e12d&title=)
<a name="Nbw2c"></a>
## 小结
以上就是对 微服务 Spring Boot 整合 Redis BitMap 实现 签到与统计 的简单介绍，签到功能是很常用的，在项目中，是一个不错的亮点，统计功能也是各大系统中比较重要的功能，签到完成后，去统计本月的连续 签到记录，来给予奖励，可大大增加用户对系统的活跃度！！！
