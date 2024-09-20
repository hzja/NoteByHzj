SpringBoot Flowable<br />Flowable是一个用java语言写的轻量级工作流引擎。
<a name="xas0R"></a>
# 1、相关依赖
```xml
<!-- https://mvnrepository.com/artifact/io.springfox/springfox-swagger2 -->
<dependency>
  <groupId>io.springfox</groupId>
  <artifactId>springfox-swagger2</artifactId>
  <version>2.9.2</version>
</dependency>

<!-- https://mvnrepository.com/artifact/io.springfox/springfox-swagger-ui -->
<dependency>
  <groupId>io.springfox</groupId>
  <artifactId>springfox-swagger-ui</artifactId>
  <version>2.9.2</version>
</dependency>

<!-- https://mvnrepository.com/artifact/com.github.xiaoymin/swagger-bootstrap-ui -->
<dependency>
  <groupId>com.github.xiaoymin</groupId>
  <artifactId>swagger-bootstrap-ui</artifactId>
  <version>1.8.5</version>
</dependency>

<dependency>
  <groupId>com.alibaba</groupId>
  <artifactId>druid-spring-boot-starter</artifactId>
  <version>1.1.13</version>
</dependency>

<dependency>
  <groupId>mysql</groupId>
  <artifactId>mysql-connector-java</artifactId>
  <version>8.0.19</version>
</dependency>

<dependency>
  <groupId>org.flowable</groupId>
  <artifactId>flowable-spring-boot-starter</artifactId>
  <version>6.5.0</version>
</dependency>
```
<a name="JK8vy"></a>
# 2、flowable以及数据库的配置
```yaml
mysql:
  url: jdbc:mysql:///test?serverTimezone=UTC
  username: root
  password: 123456
  driverClassName: com.mysql.cj.jdbc.Driver
spring:
  application:
    name: ServiceActiviti
  datasource:
    type: com.alibaba.druid.pool.DruidDataSource
    url: ${mysql.url}
    driver-class-name: ${mysql.driverClassName}
    username: ${mysql.username}
    password: ${mysql.password}
    druid: # #
      url: ${mysql.url}
      username: ${mysql.username}
      password: ${mysql.password}
      driver-class-name: ${mysql.driverClassName}
      initial-size: 10
      max-active: 200
      min-idle: 10
      max-wait: 60000
      pool-prepared-statements: false
      validation-query: SELECT 1 FROM DUAL
      test-on-borrow: false
      test-on-return: false
      test-while-idle: true
      time-between-eviction-runs-millis: 60000
      min-evictable-idle-time-millis: 30000
      max-pool-prepared-statement-per-connection-size: 20
      aop-patterns: com.msyyt.crm.activity.*
      filter: # 状态监控
        stat:
          enabled: true
          db-type: mysql
          log-slow-sql: true
          slow-sql-millis: 2000
      web-stat-filter: # 监控过滤器
        enabled: true #是否启用 默认true
        exclusions:
          - '*.js'
          - '*.gif'
          - '*.jpg'
          - '*.png'
          - '*.css'
          - '*.ico'
          - /druid/*
      stat-view-servlet: # druid 监控页面
        enabled: true
        url-pattern: /druid/*
        reset-enable: false
        allow:  # 白名单
        deny:   # 黑名单
        login-username: admin
        login-password:  admin
# 工作流
flowable:
  #关闭定时任务JOB
  async-executor-activate: false
  #  将databaseSchemaUpdate设置为true。当Flowable发现库与数据库表结构不一致时，会自动将数据库表结构升级至新版本。
  database-schema-update: true
```
<a name="nsEW4"></a>
# 3、运行项目生成数据库表
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596368198643-595fca44-a32d-4656-9802-349887bbb568.png#align=left&display=inline&height=614&originHeight=1842&originWidth=1265&size=295615&status=done&style=shadow&width=421.6666666666667)
<a name="wHldq"></a>
# 4、定义标准的BPMN流程文件ExpenseProcess.bpmn20.xml放至resource目录下新建processes目录下
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596382921779-56954b4b-1834-41ae-a71a-970696d01dba.png#align=left&display=inline&height=120&originHeight=359&originWidth=691&size=24340&status=done&style=shadow&width=230.33333333333334)
```xml
<?xml version="1.0" encoding="UTF-8"?>
<definitions xmlns="http://www.omg.org/spec/BPMN/20100524/MODEL" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
             xmlns:flowable="http://flowable.org/bpmn" xmlns:bpmndi="http://www.omg.org/spec/BPMN/20100524/DI"
             xmlns:omgdc="http://www.omg.org/spec/DD/20100524/DC" xmlns:omgdi="http://www.omg.org/spec/DD/20100524/DI"
             typeLanguage="http://www.w3.org/2001/XMLSchema" expressionLanguage="http://www.w3.org/1999/XPath"
             targetNamespace="http://www.flowable.org/processdef">
    <process id="Expense" name="ExpenseProcess" isExecutable="true">
        <documentation>报销流程</documentation>
        <startEvent id="start" name="开始"></startEvent>
        <userTask id="fillTask" name="出差报销" flowable:assignee="${taskUser}">
            <extensionElements>
                <modeler:initiator-can-complete xmlns:modeler="http://flowable.org/modeler">
                    <![CDATA[false]]></modeler:initiator-can-complete>
            </extensionElements>
        </userTask>
        <exclusiveGateway id="judgeTask"></exclusiveGateway>
        <userTask id="directorTak" name="经理审批">
            <extensionElements>
                <flowable:taskListener event="create"
                                       class="com.fcant.service_flowable.handler.ManagerTaskHandler"></flowable:taskListener>
            </extensionElements>
        </userTask>
        <userTask id="bossTask" name="老板审批">
            <extensionElements>
                <flowable:taskListener event="create"
                                       class="com.fcant.service_flowable.handler.BossTaskHandler"></flowable:taskListener>
            </extensionElements>
        </userTask>
        <endEvent id="end" name="结束"></endEvent>
        <sequenceFlow id="directorNotPassFlow" name="驳回" sourceRef="directorTak" targetRef="fillTask">
            <conditionExpression xsi:type="tFormalExpression"><![CDATA[${outcome=='驳回'}]]></conditionExpression>
        </sequenceFlow>
        <sequenceFlow id="bossNotPassFlow" name="驳回" sourceRef="bossTask" targetRef="fillTask">
            <conditionExpression xsi:type="tFormalExpression"><![CDATA[${outcome=='驳回'}]]></conditionExpression>
        </sequenceFlow>
        <sequenceFlow id="flow1" sourceRef="start" targetRef="fillTask"></sequenceFlow>
        <sequenceFlow id="flow2" sourceRef="fillTask" targetRef="judgeTask"></sequenceFlow>
        <sequenceFlow id="judgeMore" name="大于500元" sourceRef="judgeTask" targetRef="bossTask">
            <conditionExpression xsi:type="tFormalExpression"><![CDATA[${money > 500}]]></conditionExpression>
        </sequenceFlow>
        <sequenceFlow id="bossPassFlow" name="通过" sourceRef="bossTask" targetRef="end">
            <conditionExpression xsi:type="tFormalExpression"><![CDATA[${outcome=='通过'}]]></conditionExpression>
        </sequenceFlow>
        <sequenceFlow id="directorPassFlow" name="通过" sourceRef="directorTak" targetRef="end">
            <conditionExpression xsi:type="tFormalExpression"><![CDATA[${outcome=='通过'}]]></conditionExpression>
        </sequenceFlow>
        <sequenceFlow id="judgeLess" name="小于500元" sourceRef="judgeTask" targetRef="directorTak">
            <conditionExpression xsi:type="tFormalExpression"><![CDATA[${money <= 500}]]></conditionExpression>
        </sequenceFlow>
    </process>
    <bpmndi:BPMNDiagram id="BPMNDiagram_Expense">
        <bpmndi:BPMNPlane bpmnElement="Expense" id="BPMNPlane_Expense">
            <bpmndi:BPMNShape bpmnElement="start" id="BPMNShape_start">
                <omgdc:Bounds height="30.0" width="30.0" x="285.0" y="135.0"></omgdc:Bounds>
            </bpmndi:BPMNShape>
            <bpmndi:BPMNShape bpmnElement="fillTask" id="BPMNShape_fillTask">
                <omgdc:Bounds height="80.0" width="100.0" x="405.0" y="110.0"></omgdc:Bounds>
            </bpmndi:BPMNShape>
            <bpmndi:BPMNShape bpmnElement="judgeTask" id="BPMNShape_judgeTask">
                <omgdc:Bounds height="40.0" width="40.0" x="585.0" y="130.0"></omgdc:Bounds>
            </bpmndi:BPMNShape>
            <bpmndi:BPMNShape bpmnElement="directorTak" id="BPMNShape_directorTak">
                <omgdc:Bounds height="80.0" width="100.0" x="735.0" y="110.0"></omgdc:Bounds>
            </bpmndi:BPMNShape>
            <bpmndi:BPMNShape bpmnElement="bossTask" id="BPMNShape_bossTask">
                <omgdc:Bounds height="80.0" width="100.0" x="555.0" y="255.0"></omgdc:Bounds>
            </bpmndi:BPMNShape>
            <bpmndi:BPMNShape bpmnElement="end" id="BPMNShape_end">
                <omgdc:Bounds height="28.0" width="28.0" x="771.0" y="281.0"></omgdc:Bounds>
            </bpmndi:BPMNShape>
            <bpmndi:BPMNEdge bpmnElement="flow1" id="BPMNEdge_flow1">
                <omgdi:waypoint x="315.0" y="150.0"></omgdi:waypoint>
                <omgdi:waypoint x="405.0" y="150.0"></omgdi:waypoint>
            </bpmndi:BPMNEdge>
            <bpmndi:BPMNEdge bpmnElement="flow2" id="BPMNEdge_flow2">
                <omgdi:waypoint x="505.0" y="150.16611295681062"></omgdi:waypoint>
                <omgdi:waypoint x="585.4333333333333" y="150.43333333333334"></omgdi:waypoint>
            </bpmndi:BPMNEdge>
            <bpmndi:BPMNEdge bpmnElement="judgeLess" id="BPMNEdge_judgeLess">
                <omgdi:waypoint x="624.5530726256983" y="150.44692737430168"></omgdi:waypoint>
                <omgdi:waypoint x="735.0" y="150.1392757660167"></omgdi:waypoint>
            </bpmndi:BPMNEdge>
            <bpmndi:BPMNEdge bpmnElement="directorNotPassFlow" id="BPMNEdge_directorNotPassFlow">
                <omgdi:waypoint x="785.0" y="110.0"></omgdi:waypoint>
                <omgdi:waypoint x="785.0" y="37.0"></omgdi:waypoint>
                <omgdi:waypoint x="455.0" y="37.0"></omgdi:waypoint>
                <omgdi:waypoint x="455.0" y="110.0"></omgdi:waypoint>
            </bpmndi:BPMNEdge>
            <bpmndi:BPMNEdge bpmnElement="bossPassFlow" id="BPMNEdge_bossPassFlow">
                <omgdi:waypoint x="655.0" y="295.0"></omgdi:waypoint>
                <omgdi:waypoint x="771.0" y="295.0"></omgdi:waypoint>
            </bpmndi:BPMNEdge>
            <bpmndi:BPMNEdge bpmnElement="judgeMore" id="BPMNEdge_judgeMore">
                <omgdi:waypoint x="605.4340277777778" y="169.56597222222223"></omgdi:waypoint>
                <omgdi:waypoint x="605.1384083044983" y="255.0"></omgdi:waypoint>
            </bpmndi:BPMNEdge>
            <bpmndi:BPMNEdge bpmnElement="directorPassFlow" id="BPMNEdge_directorPassFlow">
                <omgdi:waypoint x="785.0" y="190.0"></omgdi:waypoint>
                <omgdi:waypoint x="785.0" y="281.0"></omgdi:waypoint>
            </bpmndi:BPMNEdge>
            <bpmndi:BPMNEdge bpmnElement="bossNotPassFlow" id="BPMNEdge_bossNotPassFlow">
                <omgdi:waypoint x="555.0" y="295.0"></omgdi:waypoint>
                <omgdi:waypoint x="455.0" y="295.0"></omgdi:waypoint>
                <omgdi:waypoint x="455.0" y="190.0"></omgdi:waypoint>
            </bpmndi:BPMNEdge>
        </bpmndi:BPMNPlane>
    </bpmndi:BPMNDiagram>
</definitions>
```
<a name="coGkR"></a>
# 5、添加代理类
<a name="t75Cu"></a>
## A.ManagerTaskHandler.java
```java
package com.fcant.service_flowable.handler;

import org.flowable.engine.delegate.TaskListener;
import org.flowable.task.service.delegate.DelegateTask;

/**
 * ManagerTaskHandler
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 19:42 2020/8/2/0002
 */
public class ManagerTaskHandler implements TaskListener {
    @Override
    public void notify(DelegateTask delegateTask) {
        delegateTask.setAssignee("经理");
    }

}
```
<a name="VlEUS"></a>
## B.BossTaskHandler.java
```java
package com.fcant.service_flowable.handler;

import org.flowable.engine.delegate.TaskListener;
import org.flowable.task.service.delegate.DelegateTask;

/**
 * BossTaskHandler
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 19:43 2020/8/2/0002
 */
public class BossTaskHandler implements TaskListener {

    @Override
    public void notify(DelegateTask delegateTask) {
        delegateTask.setAssignee("老板");
    }
}
```
<a name="Fo1nx"></a>
## C.以上代理类可以使用标签进行替换
```xml
<userTask id="holidayApprovedTask" name="Holiday approved" flowable:assignee="${employee}"/>
```
<a name="eL9Yl"></a>
## D.代理类完成后需要在BPMN进行类指定
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596419583496-87e67105-13eb-439f-b9bd-92896e1ce26d.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3840&size=1219712&status=done&style=shadow&width=1280)
<a name="bTAO5"></a>
# 6、业务逻辑层以及测试接口
<a name="wGbCM"></a>
## A.ExpenseService.java
```java
package com.fcant.service_flowable.service;

import javax.servlet.http.HttpServletResponse;

/**
 * ExpenseService
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 20:10 2020/8/2/0002
 */
public interface ExpenseService {

    /**
     * 添加报销
     *
     * @param userId 用户Id
     * @param money 报销金额
     * @param descption 描述
     * @return String
     * @author Fcant 下午 20:02 2020/8/2/0002
     */
    public String addExpense(String userId, Integer money, String descption);

    /**
     * 获取审批管理列表
     *
     * @param userId 对应用户的审批列表
     * @return Object
     * @author Fcant 下午 20:01 2020/8/2/0002
     */
    public Object list(String userId);

    /**
     * 批准
     *
     * @param taskId 批准的工作流ID
     * @return
     * @author Fcant 下午 20:01 2020/8/2/0002
     */
    public String apply(String taskId);

    /**
     * 拒绝
     *
     * @param taskId 拒绝的工作流ID
     * @return String
     * @author Fcant 下午 20:00 2020/8/2/0002
     */
    public String reject(String taskId);

    /**
     * 生成流程图
     *
     * @param processId 任务ID
     * @author Fcant 下午 19:59 2020/8/2/0002
     */
    public void genProcessDiagram(HttpServletResponse httpServletResponse, String processId) throws Exception;
}
```
<a name="oBAeB"></a>
## B.ExpenseServiceImpl.java
```java
package com.fcant.service_flowable.service.impl;

import com.fcant.service_flowable.service.ExpenseService;
import org.flowable.bpmn.model.BpmnModel;
import org.flowable.engine.*;
import org.flowable.engine.runtime.Execution;
import org.flowable.engine.runtime.ProcessInstance;
import org.flowable.image.ProcessDiagramGenerator;
import org.flowable.task.api.Task;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import javax.servlet.http.HttpServletResponse;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

/**
 * ExpenseServiceImpl
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 20:10 2020/8/2/0002
 */
@Service
public class ExpenseServiceImpl implements ExpenseService {

    @Autowired
    private RuntimeService runtimeService;

    @Autowired
    private TaskService taskService;

    @Autowired
    private RepositoryService repositoryService;

    @Autowired
    private ProcessEngine processEngine;

    /**
     * 添加报销
     *
     * @param userId 用户Id
     * @param money 报销金额
     * @param descption 描述
     * @return String
     * @author Fcant 下午 20:02 2020/8/2/0002
     */
    @Override
    public String addExpense(String userId, Integer money, String descption) {
        //启动流程
        HashMap<String, Object> map = new HashMap<>();
        map.put("taskUser", userId);
        map.put("money", money);
        ProcessInstance processInstance = runtimeService.startProcessInstanceByKey("Expense", map);
        return "提交成功.流程Id为：" + processInstance.getId();
    }

    /**
     * 获取审批管理列表
     *
     * @param userId 对应用户的审批列表
     * @return Object
     * @author Fcant 下午 20:01 2020/8/2/0002
     */
    @Override
    public Object list(String userId) {
        List<Task> tasks = taskService.createTaskQuery().taskAssignee(userId).orderByTaskCreateTime().desc().list();
        for (Task task : tasks) {
            System.out.println(task.toString());
        }
        return tasks.toArray().toString();
    }

    /**
     * 批准
     *
     * @param taskId 批准的工作流ID
     * @return
     * @author Fcant 下午 20:01 2020/8/2/0002
     */
    @Override
    public String apply(String taskId) {
        Task task = taskService.createTaskQuery().taskId(taskId).singleResult();
        if (task == null) {
            throw new RuntimeException("流程不存在");
        }
        //通过审核
        HashMap<String, Object> map = new HashMap<>();
        map.put("outcome", "通过");
        taskService.complete(taskId, map);
        return "processed ok!";
    }

    /**
     * 拒绝
     *
     * @param taskId 拒绝的工作流ID
     * @return String
     * @author Fcant 下午 20:00 2020/8/2/0002
     */
    @Override
    public String reject(String taskId) {
        HashMap<String, Object> map = new HashMap<>();
        map.put("outcome", "驳回");
        taskService.complete(taskId, map);
        return "reject";
    }

    /**
     * 生成流程图
     *
     * @param processId 任务ID
     * @author Fcant 下午 19:59 2020/8/2/0002
     */
    @Override
    public void genProcessDiagram(HttpServletResponse httpServletResponse, String processId) throws Exception {
        ProcessInstance pi = runtimeService.createProcessInstanceQuery().processInstanceId(processId).singleResult();
        //流程走完的不显示图
        if (pi == null) {
            return;
        }
        Task task = taskService.createTaskQuery().processInstanceId(pi.getId()).singleResult();
        //使用流程实例ID，查询正在执行的执行对象表，返回流程实例对象
        String InstanceId = task.getProcessInstanceId();
        List<Execution> executions = runtimeService
                .createExecutionQuery()
                .processInstanceId(InstanceId)
                .list();

        //得到正在执行的Activity的Id
        List<String> activityIds = new ArrayList<>();
        List<String> flows = new ArrayList<>();
        for (Execution exe : executions) {
            List<String> ids = runtimeService.getActiveActivityIds(exe.getId());
            activityIds.addAll(ids);
        }

        //获取流程图
        BpmnModel bpmnModel = repositoryService.getBpmnModel(pi.getProcessDefinitionId());
        ProcessEngineConfiguration engconf = processEngine.getProcessEngineConfiguration();
        ProcessDiagramGenerator diagramGenerator = engconf.getProcessDiagramGenerator();
        InputStream in = diagramGenerator.generateDiagram(bpmnModel,
                "png", activityIds, flows, engconf.getActivityFontName(),
                engconf.getLabelFontName(), engconf.getAnnotationFontName(),
                engconf.getClassLoader(), 1.0, true);
        OutputStream out = null;
        byte[] buf = new byte[1024];
        int legth = 0;
        try {
            out = httpServletResponse.getOutputStream();
            while ((legth = in.read(buf)) != -1) {
                out.write(buf, 0, legth);
            }
        } finally {
            if (in != null) {
                in.close();
            }
            if (out != null) {
                out.close();
            }
        }
    }
}
```
<a name="sjfZz"></a>
## C.ExpenseController.java
```java
package com.fcant.service_flowable.controller;

import com.fcant.service_flowable.service.ExpenseService;
import io.swagger.annotations.Api;
import io.swagger.annotations.ApiOperation;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletResponse;

/**
 * ExpenseController
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 19:46 2020/8/2/0002
 */
@Api(tags = "expense",value = "工作流控制器")
@Controller
@RequestMapping("/expense")
public class ExpenseController {
    @Autowired
    ExpenseService expenseService;

    /**
     * 添加报销
     *
     * @param userId 用户Id
     * @param money 报销金额
     * @param descption 描述
     * @return String
     * @author Fcant 下午 20:02 2020/8/2/0002
     */
    @ApiOperation(value="添加报销")
    @RequestMapping(value = "/add")
    @ResponseBody
    public String addExpense(String userId, Integer money, String descption) {
        return expenseService.addExpense(userId, money, descption);
    }

    /**
     * 获取审批管理列表
     *
     * @param userId 对应用户的审批列表
     * @return Object
     * @author Fcant 下午 20:01 2020/8/2/0002
     */
    @ApiOperation(value="获取审批管理列表")
    @RequestMapping(value = "/list")
    @ResponseBody
    public Object list(String userId) {
        return expenseService.list(userId);
    }

    /**
     * 批准
     *
     * @param taskId 批准的工作流ID
     * @return
     * @author Fcant 下午 20:01 2020/8/2/0002
     */
    @ApiOperation(value="批准")
    @RequestMapping(value = "/apply")
    @ResponseBody
    public String apply(String taskId) {
        expenseService.apply(taskId);
        return "processed ok!";
    }

    /**
     * 拒绝
     *
     * @param taskId 拒绝的工作流ID
     * @return String
     * @author Fcant 下午 20:00 2020/8/2/0002
     */
    @ApiOperation(value="拒绝")
    @ResponseBody
    @RequestMapping(value = "/reject")
    public String reject(String taskId) {
        expenseService.reject(taskId);
        return "reject";
    }

    /**
     * 生成流程图
     *
     * @param processId 任务ID
     * @author Fcant 下午 19:59 2020/8/2/0002
     */
    @ApiOperation(value="生成流程图")
    @RequestMapping(value = "/processDiagram")
    public void genProcessDiagram(HttpServletResponse httpServletResponse, String processId) throws Exception {
        expenseService.genProcessDiagram(httpServletResponse, processId);
    }
}
```
<a name="O1Lsf"></a>
# 7、配置生成的流图的字体（防止/解决生成流程图片的乱码问题）
```java
package com.fcant.service_flowable.config;

import org.flowable.spring.SpringProcessEngineConfiguration;
import org.flowable.spring.boot.EngineConfigurationConfigurer;
import org.springframework.context.annotation.Configuration;

/**
 * FlowableConfig
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 19:51 2020/8/2/0002
 */
@Configuration
public class FlowableConfig implements EngineConfigurationConfigurer<SpringProcessEngineConfiguration> {


    @Override
    public void configure(SpringProcessEngineConfiguration engineConfiguration) {
        engineConfiguration.setActivityFontName("宋体");
        engineConfiguration.setLabelFontName("宋体");
        engineConfiguration.setAnnotationFontName("宋体");
    }
}
```
<a name="RQeqV"></a>
# 8、配置Swagger
```java
package com.fcant.service_flowable.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.ResourceHandlerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;
import springfox.documentation.builders.ApiInfoBuilder;
import springfox.documentation.builders.PathSelectors;
import springfox.documentation.builders.RequestHandlerSelectors;
import springfox.documentation.service.ApiInfo;
import springfox.documentation.service.Contact;
import springfox.documentation.spi.DocumentationType;
import springfox.documentation.spring.web.plugins.Docket;
import springfox.documentation.swagger2.annotations.EnableSwagger2;

/**
 * SwaggerConfig
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 23:41 2020/8/1/0001
 */
@Configuration
@EnableSwagger2
public class SwaggerConfig implements WebMvcConfigurer {

    @Override
    public void addResourceHandlers(ResourceHandlerRegistry registry) {
        registry.addResourceHandler("swagger-ui.html")
                .addResourceLocations("classpath:/META-INF/resources/");
        registry.addResourceHandler("/webjars/**")
                .addResourceLocations("classpath:/META-INF/resources/webjars/");
    }

    /**
     * @author Fcant 13:44 2019/12/5
     */
    @Bean
    public Docket petApi() {
        return new Docket(DocumentationType.SWAGGER_2)
                .apiInfo(apiInfo())
                .select()
                .apis(RequestHandlerSelectors.basePackage("com.fcant.service_flowable.controller"))
                .paths(PathSelectors.any())
                .build();
    }

    /**
     * 该套 API 说明，包含作者、简介、版本、host、服务URL
     * @return
     */
    private ApiInfo apiInfo() {
        return new ApiInfoBuilder()
                .title("Flowable Service API")
                .contact(new Contact("fcant","null","fcscanf@outlook.com"))
                .version("0.1")
                .termsOfServiceUrl("www.yuque.com/fcant")
                .description("Flowable Service API")
                .build();
    }
}
```
<a name="dbne3"></a>
# 9、启动项目，访问Swagger页面进行接口测试
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596371951255-4aeab6c1-8836-422e-9b1a-6bceb4fc52ca.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3798&size=443458&status=done&style=shadow&width=1266)<br />![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596383076207-e2d4b7e0-0270-4c38-b1ed-0edbe589c3d1.png#align=left&display=inline&height=687&originHeight=2060&originWidth=3798&size=531352&status=done&style=shadow&width=1266)
