介绍一款工具Squaretest，它是一款自动生成单元测试的插件，为什么会用到它？<br />主要因为最近公司上了代码质量管控的指标，会考评各个项目的单元测试覆盖率，以及sonar扫描出来的各种问题，很多老项目老代码，或者着急交付的项目，单元测试严重缺失，覆盖率只有5%不到。<br />所以几个小伙伴这几天就在疯狂的堆单元测试，3个人堆了2天才堆到30%，然后根据逻辑写各种Mock，感觉是有迹可循的东西，所以就查了下，发现果然有插件来干这个事情，那么接下来就来看看。<br />使用的是idea，先来下载一下插件，File——>Settings——>Plugins，搜索Squaretest，然后install就好了，插件安装完成后需要重启一下<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683988869150-9d116284-8aae-45b7-929b-389cc22f857f.png#averageHue=%23b8a590&clientId=u86f3e714-a6ba-4&from=paste&id=u601c1996&originHeight=774&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u875c8067-0150-44ff-a180-9daf18e999a&title=)<br />重启之后，菜单栏就多了一项Squaretest，下面来讲下怎么用，大家也可以通过看这个菜单的最后一项：Generate Test Methods(Help)来看它的一个演示，但演示不太全，下面截图给大家看下怎么用的，以及一些使用心得。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683988869384-cfdb54e2-f623-4721-b44f-aefad1d44309.png#averageHue=%23ede6e4&clientId=u86f3e714-a6ba-4&from=paste&id=u5dd55ce1&originHeight=143&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uffb47d16-1a37-40d8-992c-7e78be4c206&title=)<br />首先打开一个类，这个类就是即将要作为实验的类，这个类有7个`public`方法，因为`Squaretest`生成的单元测试方法都是只能生成`public`的，当然这也是合理的！毕竟`private`的肯定被`public`调用了。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683988869187-749c09e8-1d48-46e6-865b-5cf5e9db79f7.png#averageHue=%23f9f8f6&clientId=u86f3e714-a6ba-4&from=paste&id=ucc0dcff8&originHeight=919&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ud212b911-5d02-4ab6-8676-d6c31808185&title=)<br />如果来手写这个类的单元测试，光看都要一会，下面看操作，打开你的类，光标定位到代码里，右击鼠标选择Generate…<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683988869340-faf7c327-36ca-466a-a051-b15f75f22c71.png#averageHue=%23f4f2f0&clientId=u86f3e714-a6ba-4&from=paste&id=u138e982b&originHeight=1103&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ucfb70d48-1bf2-4c19-8295-13bbca5a1a6&title=)<br />然后就会看到这里有两个熟悉的图标，第一次的话选择第二个选项，它会让你选择一下单元测试的模板，因为已经选择过了，所以现在演示不会再弹出，但后面会告诉你怎么更改模板。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683989180543-8ce169e9-27b5-41fd-8475-5aa6e2eebb16.png#averageHue=%23faf7f5&clientId=u86f3e714-a6ba-4&from=paste&id=u195f6970&originHeight=932&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=ue35c54d9-db03-48e1-9cd7-b9221094519&title=)<br />选择第二项后就会弹出一个框看下面这里它自动会识别出当前类需要[Mock](http://mp.weixin.qq.com/s?__biz=MzI4Njc5NjM1NQ==&mid=2247516757&idx=1&sn=f0eaa5f84c8a062db5c022916c77b6d9&chksm=ebd5bd79dca2346f8081f4e4412896f95f694570e83b4026b7de5b36fe873ac1705b2e5efe19&scene=21#wechat_redirect)的成员变量，直接点ok<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683988869721-b147d252-5f2f-4c73-bb6b-c7a6a9fc6aa1.png#averageHue=%23faf6f4&clientId=u86f3e714-a6ba-4&from=paste&id=u7bb93e8e&originHeight=808&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc47f50df-6a8d-4eb9-a9f5-5dba4ee9036&title=)<br />自动会使用类的真实目录层次在test文件夹中创建出来一个单元测试类，类名就是原类名后加Test<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683988869940-4b38dce1-4581-451a-99fd-b15b5924bf9f.png#averageHue=%23fcfbfb&clientId=u86f3e714-a6ba-4&from=paste&id=u6ee49c23&originHeight=654&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u80dd3bd6-e931-4ac4-a78b-a739aa0a912&title=)<br />把代码贴出来给大家看看它生成出来的是什么样的，7个单元测试方法，秒秒钟就出来了，然后执行一把试试！
```java
public class CrawlerScreenShotServiceImplTest {

    @Mock
    private CrawerScreenShotTaskMapper mockCrawerScreenShotTaskMapper;
    @Mock
    private CrawerScreenShotTaskLogMapper mockCrawerScreenShotTaskLogMapper;

    @InjectMocks
    private CrawlerScreenShotServiceImpl crawlerScreenShotServiceImplUnderTest;

    @Before
    public void setUp() {
        initMocks(this);
    }

    @Test
    public void testReceiveData() {
        // Setup
        final CrawlerScreenShotVO vo = new CrawlerScreenShotVO();
        vo.setUrl("url");
        vo.setPcFlag(false);
        vo.setMembergroup("membergroup");
        vo.setTaskType(0);
        vo.setUrlType(0);

        when(mockCrawerScreenShotTaskLogMapper.saveSelective(any(CrawerScreenShotTaskLog.class))).thenReturn(0);
        when(mockCrawerScreenShotTaskMapper.saveBatch(Arrays.asList(new CrawlerScreenShotTask(0L, "url", "imageOssUrl", false, false, "memberGroup", 0, 0, "fileName", new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime(), new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime(), false, "skuCode", "state", "operater")))).thenReturn(0);

        // Run the test
        final Result<String> result = crawlerScreenShotServiceImplUnderTest.receiveData(vo);

        // Verify the results
    }

    @Test
    public void testListJobScreenShotTask() {
        // Setup

        // Configure CrawerScreenShotTaskMapper.listJobScreenShotTask(...).
        final CrawlerScreenShotTaskDto crawlerScreenShotTaskDto = new CrawlerScreenShotTaskDto();
        crawlerScreenShotTaskDto.setId(0L);
        crawlerScreenShotTaskDto.setUrl("url");
        crawlerScreenShotTaskDto.setSkuCode("skuCode");
        crawlerScreenShotTaskDto.setPcFlag(false);
        crawlerScreenShotTaskDto.setMemberGroup("memberGroup");
        crawlerScreenShotTaskDto.setUrlType(0);
        crawlerScreenShotTaskDto.setFileName("fileName");
        crawlerScreenShotTaskDto.setTaskType(0);
        crawlerScreenShotTaskDto.setState("state");
        final List<CrawlerScreenShotTaskDto> crawlerScreenShotTaskDtos = Arrays.asList(crawlerScreenShotTaskDto);
        when(mockCrawerScreenShotTaskMapper.listJobScreenShotTask(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime())).thenReturn(crawlerScreenShotTaskDtos);

        // Run the test
        final List<CrawlerScreenShotTaskDto> result = crawlerScreenShotServiceImplUnderTest.listJobScreenShotTask();

        // Verify the results
    }

    @Test
    public void testQuery() {
        // Setup
        final NikeScreenShotListRequestVo requestVo = new NikeScreenShotListRequestVo();
        requestVo.setUrl("url");
        requestVo.setUrlType(0);
        requestVo.setStartTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());
        requestVo.setEndTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());
        requestVo.setStatus(0);
        requestVo.setPcFlag(0);
        requestVo.setPageNum(0);
        requestVo.setPageSize(0);

        // Configure CrawerScreenShotTaskMapper.query(...).
        final PimScreenShotVo pimScreenShotVo = new PimScreenShotVo();
        pimScreenShotVo.setId(0L);
        pimScreenShotVo.setUrl("url");
        pimScreenShotVo.setImageOssUrl("imageOssUrl");
        pimScreenShotVo.setStatus(0);
        pimScreenShotVo.setPcFlag(false);
        pimScreenShotVo.setCreateTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());
        pimScreenShotVo.setUrlType(0);
        pimScreenShotVo.setMsg("msg");
        final List<PimScreenShotVo> pimScreenShotVos = Arrays.asList(pimScreenShotVo);
        when(mockCrawerScreenShotTaskMapper.query(any(NikeScreenShotListRequestVo.class))).thenReturn(pimScreenShotVos);

        // Run the test
        final PageInfo<PimScreenShotVo> result = crawlerScreenShotServiceImplUnderTest.query(requestVo);

        // Verify the results
    }

    @Test
    public void testQuerySelectBoxData() {
        // Setup

        // Configure CrawerScreenShotTaskMapper.query(...).
        final PimScreenShotVo pimScreenShotVo = new PimScreenShotVo();
        pimScreenShotVo.setId(0L);
        pimScreenShotVo.setUrl("url");
        pimScreenShotVo.setImageOssUrl("imageOssUrl");
        pimScreenShotVo.setStatus(0);
        pimScreenShotVo.setPcFlag(false);
        pimScreenShotVo.setCreateTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());
        pimScreenShotVo.setUrlType(0);
        pimScreenShotVo.setMsg("msg");
        final List<PimScreenShotVo> pimScreenShotVos = Arrays.asList(pimScreenShotVo);
        when(mockCrawerScreenShotTaskMapper.query(any(NikeScreenShotListRequestVo.class))).thenReturn(pimScreenShotVos);

        // Run the test
        final PimScreenShotTaskParamsDto result = crawlerScreenShotServiceImplUnderTest.querySelectBoxData();

        // Verify the results
    }

    @Test
    public void testFindExecutionScreenShotTaskCount() {
        // Setup
        when(mockCrawerScreenShotTaskMapper.findExecutionScreenShotTaskCount()).thenReturn(0);

        // Run the test
        final Integer result = crawlerScreenShotServiceImplUnderTest.findExecutionScreenShotTaskCount();

        // Verify the results
        assertEquals(0, result);
    }

    @Test
    public void testFindCrawerScreenshotTaskByCreateTime() {
        // Setup
        final CrawlerScreenShotTaskSyncDto crawlerScreenShotTaskSyncDto = new CrawlerScreenShotTaskSyncDto();
        crawlerScreenShotTaskSyncDto.setId(0L);
        crawlerScreenShotTaskSyncDto.setUrl("url");
        crawlerScreenShotTaskSyncDto.setSkuCode("skuCode");
        crawlerScreenShotTaskSyncDto.setTaskType(0);
        crawlerScreenShotTaskSyncDto.setStatus(0);
        crawlerScreenShotTaskSyncDto.setLastModifyTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());
        crawlerScreenShotTaskSyncDto.setOperater("operater");
        crawlerScreenShotTaskSyncDto.setMsg("msg");
        final List<CrawlerScreenShotTaskSyncDto> expectedResult = Arrays.asList(crawlerScreenShotTaskSyncDto);

        // Configure CrawerScreenShotTaskMapper.findCrawerScreenshotTaskByCreateTime(...).
        final CrawlerScreenShotTaskSyncDto crawlerScreenShotTaskSyncDto1 = new CrawlerScreenShotTaskSyncDto();
        crawlerScreenShotTaskSyncDto1.setId(0L);
        crawlerScreenShotTaskSyncDto1.setUrl("url");
        crawlerScreenShotTaskSyncDto1.setSkuCode("skuCode");
        crawlerScreenShotTaskSyncDto1.setTaskType(0);
        crawlerScreenShotTaskSyncDto1.setStatus(0);
        crawlerScreenShotTaskSyncDto1.setLastModifyTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());
        crawlerScreenShotTaskSyncDto1.setOperater("operater");
        crawlerScreenShotTaskSyncDto1.setMsg("msg");
        final List<CrawlerScreenShotTaskSyncDto> crawlerScreenShotTaskSyncDtos = Arrays.asList(crawlerScreenShotTaskSyncDto1);
        when(mockCrawerScreenShotTaskMapper.findCrawerScreenshotTaskByCreateTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime())).thenReturn(crawlerScreenShotTaskSyncDtos);

        // Run the test
        final List<CrawlerScreenShotTaskSyncDto> result = crawlerScreenShotServiceImplUnderTest.findCrawerScreenshotTaskByCreateTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());

        // Verify the results
        assertEquals(expectedResult, result);
    }

    @Test
    public void testQueryCrawlerDashboard() {
        // Setup
        when(mockCrawerScreenShotTaskMapper.queryCrawlerDashboard(0, 0, 0, new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime(), new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime())).thenReturn(0);

        // Run the test
        final Integer result = crawlerScreenShotServiceImplUnderTest.queryCrawlerDashboard(0, 0, 0, new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime(), new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());

        // Verify the results
        assertEquals(0, result);
    }
}
```
报错了呢，不要慌，这个断言是为了检查单元测试跑出来的结果是否符合预期的，如果不想检查只想完成覆盖率，直接干掉就可以了（手动狗头）。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683988870143-ff704019-0da2-455d-b18b-cea1ee80249b.png#averageHue=%23f8f4f3&clientId=u86f3e714-a6ba-4&from=paste&id=u157800d3&originHeight=675&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u8844156d-a9ac-4c5f-bb0a-dddccbbd84c&title=)<br />怎么样！刺不刺激，爽不爽，秒秒钟90多行的代码覆盖率就到了90%以上。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683988870081-7192fdc5-9d6c-4eee-81a4-2aeaaf31443c.png#averageHue=%23d4d3c0&clientId=u86f3e714-a6ba-4&from=paste&id=uc3e250b9&originHeight=412&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=uc0920718-9e18-43aa-a214-eb9b429a89b&title=)<br />上面说过第一次进来会让你选择单元测试的模板，如果要切换的话可以在单元测试类中按快捷键，Alt+M，或者通过Squaretest的菜单倒数第二个，下面这个就是按快捷键的效果，选择的是下图这个模板，也可以借鉴。<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683988870114-cd72f207-d36b-4a83-aaeb-25cffad0bb58.png#averageHue=%23f8f5f4&clientId=u86f3e714-a6ba-4&from=paste&id=ucfae67d6&originHeight=729&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2173ef3a-4fb0-487e-a71c-e1e7852e136&title=)<br />以上Squaretest部分就结束了，当然拉也不能高兴的太早，这个类算是比较成功的情况，很多时候还是要自己小修小改的，毕竟它生成出来的测试数据可能完全匹配不上if else数据对吧，但这都很好改，这样就从自己分析if else变成了，debug程序了呀，哪里报错，debug过去，看看是不是生成的数据有问题，改个数据，就通过了，反正本人用的是很舒畅的，妥妥的节省70%的工作量。<br />解决了上面一个问题之后，又发现另一个问题，这个工具VO，DTO，Entity，Command，Model这种实体类来讲，一般这种实体类都用lombok的注解get，set，还有constract构造器等注解，但是这个工具只能生成这些实体类的构造器的单元测试，无法生成get set方法的单元测试，所以写了个base方法，实体类继承一下，简单的写两行带就好了，看下面代码：
```java
public class CrawlerScreenShotServiceImplTest {

    @Mock
    private CrawerScreenShotTaskMapper mockCrawerScreenShotTaskMapper;
    @Mock
    private CrawerScreenShotTaskLogMapper mockCrawerScreenShotTaskLogMapper;

    @InjectMocks
    private CrawlerScreenShotServiceImpl crawlerScreenShotServiceImplUnderTest;

    @Before
    public void setUp() {
        initMocks(this);
    }

    @Test
    public void testReceiveData() {
        // Setup
        final CrawlerScreenShotVO vo = new CrawlerScreenShotVO();
        vo.setUrl("url");
        vo.setPcFlag(false);
        vo.setMembergroup("membergroup");
        vo.setTaskType(0);
        vo.setUrlType(0);

        when(mockCrawerScreenShotTaskLogMapper.saveSelective(any(CrawerScreenShotTaskLog.class))).thenReturn(0);
        when(mockCrawerScreenShotTaskMapper.saveBatch(Arrays.asList(new CrawlerScreenShotTask(0L, "url", "imageOssUrl", false, false, "memberGroup", 0, 0, "fileName", new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime(), new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime(), false, "skuCode", "state", "operater")))).thenReturn(0);

        // Run the test
        final Result<String> result = crawlerScreenShotServiceImplUnderTest.receiveData(vo);

        // Verify the results
    }

    @Test
    public void testListJobScreenShotTask() {
        // Setup

        // Configure CrawerScreenShotTaskMapper.listJobScreenShotTask(...).
        final CrawlerScreenShotTaskDto crawlerScreenShotTaskDto = new CrawlerScreenShotTaskDto();
        crawlerScreenShotTaskDto.setId(0L);
        crawlerScreenShotTaskDto.setUrl("url");
        crawlerScreenShotTaskDto.setSkuCode("skuCode");
        crawlerScreenShotTaskDto.setPcFlag(false);
        crawlerScreenShotTaskDto.setMemberGroup("memberGroup");
        crawlerScreenShotTaskDto.setUrlType(0);
        crawlerScreenShotTaskDto.setFileName("fileName");
        crawlerScreenShotTaskDto.setTaskType(0);
        crawlerScreenShotTaskDto.setState("state");
        final List<CrawlerScreenShotTaskDto> crawlerScreenShotTaskDtos = Arrays.asList(crawlerScreenShotTaskDto);
        when(mockCrawerScreenShotTaskMapper.listJobScreenShotTask(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime())).thenReturn(crawlerScreenShotTaskDtos);

        // Run the test
        final List<CrawlerScreenShotTaskDto> result = crawlerScreenShotServiceImplUnderTest.listJobScreenShotTask();

        // Verify the results
    }

    @Test
    public void testQuery() {
        // Setup
        final NikeScreenShotListRequestVo requestVo = new NikeScreenShotListRequestVo();
        requestVo.setUrl("url");
        requestVo.setUrlType(0);
        requestVo.setStartTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());
        requestVo.setEndTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());
        requestVo.setStatus(0);
        requestVo.setPcFlag(0);
        requestVo.setPageNum(0);
        requestVo.setPageSize(0);

        // Configure CrawerScreenShotTaskMapper.query(...).
        final PimScreenShotVo pimScreenShotVo = new PimScreenShotVo();
        pimScreenShotVo.setId(0L);
        pimScreenShotVo.setUrl("url");
        pimScreenShotVo.setImageOssUrl("imageOssUrl");
        pimScreenShotVo.setStatus(0);
        pimScreenShotVo.setPcFlag(false);
        pimScreenShotVo.setCreateTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());
        pimScreenShotVo.setUrlType(0);
        pimScreenShotVo.setMsg("msg");
        final List<PimScreenShotVo> pimScreenShotVos = Arrays.asList(pimScreenShotVo);
        when(mockCrawerScreenShotTaskMapper.query(any(NikeScreenShotListRequestVo.class))).thenReturn(pimScreenShotVos);

        // Run the test
        final PageInfo<PimScreenShotVo> result = crawlerScreenShotServiceImplUnderTest.query(requestVo);

        // Verify the results
    }

    @Test
    public void testQuerySelectBoxData() {
        // Setup

        // Configure CrawerScreenShotTaskMapper.query(...).
        final PimScreenShotVo pimScreenShotVo = new PimScreenShotVo();
        pimScreenShotVo.setId(0L);
        pimScreenShotVo.setUrl("url");
        pimScreenShotVo.setImageOssUrl("imageOssUrl");
        pimScreenShotVo.setStatus(0);
        pimScreenShotVo.setPcFlag(false);
        pimScreenShotVo.setCreateTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());
        pimScreenShotVo.setUrlType(0);
        pimScreenShotVo.setMsg("msg");
        final List<PimScreenShotVo> pimScreenShotVos = Arrays.asList(pimScreenShotVo);
        when(mockCrawerScreenShotTaskMapper.query(any(NikeScreenShotListRequestVo.class))).thenReturn(pimScreenShotVos);

        // Run the test
        final PimScreenShotTaskParamsDto result = crawlerScreenShotServiceImplUnderTest.querySelectBoxData();

        // Verify the results
    }

    @Test
    public void testFindExecutionScreenShotTaskCount() {
        // Setup
        when(mockCrawerScreenShotTaskMapper.findExecutionScreenShotTaskCount()).thenReturn(0);

        // Run the test
        final Integer result = crawlerScreenShotServiceImplUnderTest.findExecutionScreenShotTaskCount();

        // Verify the results
        assertEquals(0, result);
    }

    @Test
    public void testFindCrawerScreenshotTaskByCreateTime() {
        // Setup
        final CrawlerScreenShotTaskSyncDto crawlerScreenShotTaskSyncDto = new CrawlerScreenShotTaskSyncDto();
        crawlerScreenShotTaskSyncDto.setId(0L);
        crawlerScreenShotTaskSyncDto.setUrl("url");
        crawlerScreenShotTaskSyncDto.setSkuCode("skuCode");
        crawlerScreenShotTaskSyncDto.setTaskType(0);
        crawlerScreenShotTaskSyncDto.setStatus(0);
        crawlerScreenShotTaskSyncDto.setLastModifyTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());
        crawlerScreenShotTaskSyncDto.setOperater("operater");
        crawlerScreenShotTaskSyncDto.setMsg("msg");
        final List<CrawlerScreenShotTaskSyncDto> expectedResult = Arrays.asList(crawlerScreenShotTaskSyncDto);

        // Configure CrawerScreenShotTaskMapper.findCrawerScreenshotTaskByCreateTime(...).
        final CrawlerScreenShotTaskSyncDto crawlerScreenShotTaskSyncDto1 = new CrawlerScreenShotTaskSyncDto();
        crawlerScreenShotTaskSyncDto1.setId(0L);
        crawlerScreenShotTaskSyncDto1.setUrl("url");
        crawlerScreenShotTaskSyncDto1.setSkuCode("skuCode");
        crawlerScreenShotTaskSyncDto1.setTaskType(0);
        crawlerScreenShotTaskSyncDto1.setStatus(0);
        crawlerScreenShotTaskSyncDto1.setLastModifyTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());
        crawlerScreenShotTaskSyncDto1.setOperater("operater");
        crawlerScreenShotTaskSyncDto1.setMsg("msg");
        final List<CrawlerScreenShotTaskSyncDto> crawlerScreenShotTaskSyncDtos = Arrays.asList(crawlerScreenShotTaskSyncDto1);
        when(mockCrawerScreenShotTaskMapper.findCrawerScreenshotTaskByCreateTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime())).thenReturn(crawlerScreenShotTaskSyncDtos);

        // Run the test
        final List<CrawlerScreenShotTaskSyncDto> result = crawlerScreenShotServiceImplUnderTest.findCrawerScreenshotTaskByCreateTime(new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());

        // Verify the results
        assertEquals(expectedResult, result);
    }

    @Test
    public void testQueryCrawlerDashboard() {
        // Setup
        when(mockCrawerScreenShotTaskMapper.queryCrawlerDashboard(0, 0, 0, new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime(), new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime())).thenReturn(0);

        // Run the test
        final Integer result = crawlerScreenShotServiceImplUnderTest.queryCrawlerDashboard(0, 0, 0, new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime(), new GregorianCalendar(2019, Calendar.JANUARY, 1).getTime());

        // Verify the results
        assertEquals(0, result);
    }
}
```
同样的方式在实体类上通过Squaretest生成单元测试，然后继承上面写的那个base类，vo的单元测试代码稍加改动，如下<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683988870339-dccbd437-1f45-42c9-a218-c6fd8162e620.png#averageHue=%23fcf1ef&clientId=u86f3e714-a6ba-4&from=paste&id=u122aaf32&originHeight=266&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u5979f527-c640-4270-8976-624da634500&title=)<br />看run完之后，覆盖率100%，妥妥的，通过这两个解决方案，一天之内就把覆盖率搞到了60%以上，不要太刺激，大家可以用用试试，当然这个也不是纯为了应付差事写的单元测试，后续开发的时候，也可以用这个工具来生成，然后自测自己的代码，这样也是提升工作效率的！<br />![](https://cdn.nlark.com/yuque/0/2023/png/396745/1683988870520-f469c436-274f-4a6a-92e4-a68567c3371e.png#averageHue=%23e8e9d5&clientId=u86f3e714-a6ba-4&from=paste&id=ub53ace19&originHeight=191&originWidth=1080&originalType=url&ratio=2.5&rotation=0&showTitle=false&status=done&style=none&taskId=u2453b98d-c570-4973-8fd5-c59a8481656&title=)
