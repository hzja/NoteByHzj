SpringBoot Activiti6
<a name="uBJlv"></a>
## 1、Activiti6简介
对于流程审批类型的项目，用Activiti解决再合适不过了。<br />Activiti是一种轻量级，可嵌入的BPM引擎，而且还设计适用于可扩展的云架构。可以和SpringBoot完美结合。是一个工作流处理框架。<br />Activiti6的7 个服务接口和28张表：

| **服务接口** | **说明** |
| --- | --- |
| RepositoryService | 仓库服务，用于管理仓库，比如部署或删除流程定义、读取流程资源等。 |
| IdentifyService | 身份服务，管理用户、组以及它们之间的关系。 |
| RuntimeService | 运行时服务，管理所有正在运行的流程实例、任务等对象。 |
| TaskService | 任务服务，管理任务。 |
| FormService | 表单服务，管理和流程、任务相关的表单。 |
| HistroyService | 历史服务，管理历史数据。 |
| ManagementService | 引擎管理服务，比如管理引擎的配置、数据库和作业等核心对象 |
| **表** | **说明** |
| ACT_RE_* | RE’表示repository。 这个前缀的表包含了流程定义和流程静态资源 （图片，规则，等等） |
| ACT_RU_* | RU’表示runtime。这些运行时的表，包含流程实例，任务，变量，异步任务，等运行中的数据。 Activiti只在流程实例执行过程中保存这些数据，在流程结束时就会删除这些记录。 这样运行时表可以一直很小速度很快。 |
| ACT_ID_* | ‘ID’表示identity。 这些表包含身份信息，比如用户，组等等。 |
| ACT_HI_* | ‘HI’表示history。 这些表包含历史数据，比如历史流程实例， 变量，任务等等。 |
| ACT_GE_* | 通用数据， 用于不同场景下，如存放资源文件。 |

<a name="gnc9W"></a>
## 2、引入依赖
```xml
<!--fastjson-->
<dependency>
  <artifactId>fastjson</artifactId>
  <groupId>com.alibaba</groupId>
  <version>1.2.67</version>
</dependency>

<dependency>
  <groupId>org.apache.commons</groupId>
  <artifactId>commons-lang3</artifactId>
</dependency>

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
  <groupId>org.activiti</groupId>
  <artifactId>activiti-spring-boot-starter-basic</artifactId>
  <version>6.0.0</version>
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
  <groupId>org.springframework.boot</groupId>
  <artifactId>spring-boot-starter-web</artifactId>
</dependency>

<dependency>
  <groupId>javax.servlet</groupId>
  <artifactId>servlet-api</artifactId>
  <version>2.5</version>
</dependency>
```
<a name="WGHpP"></a>
## 3、在SpringBoot配置文件配置数据源与Activiti
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
activiti:
  # 自动部署验证设置:
  # true（默认）自动部署流程
  # false 不自动部署，需要手动部署发布流程
  check-process-definitions: true
  # 可选值为: false,true,create-drop,drop-create
  # 默认为true。为true表示activiti会对数据库中的表进行更新操作，如果不存在，则进行创建。
  database-schema-update: true
```
<a name="As5rj"></a>
## 4、配置启动类（排除互斥的安全依赖以及包扫描）
```java
package com.fcant.service_acti;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.annotation.ComponentScan;

@SpringBootApplication(exclude={
        org.activiti.spring.boot.SecurityAutoConfiguration.class
})
@ComponentScan("com.fcant")
public class ServiceActiApplication {

    public static void main(String[] args) {
        SpringApplication.run(ServiceActiApplication.class, args);
    }

}

```
<a name="d5vcj"></a>
## 5、项目接口文档的配置
```java
package com.fcant.service_acti.config;

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
                .apis(RequestHandlerSelectors.basePackage("com.fcant.service_acti.controller"))
                .paths(PathSelectors.any())
                .build();
    }

    /**
     * 该套 API 说明，包含作者、简介、版本、host、服务URL
     * @return
     */
    private ApiInfo apiInfo() {
        return new ApiInfoBuilder()
                .title("Activiti Service API")
                .contact(new Contact("fcant","null","fcscanf@outlook.com"))
                .version("0.1")
                .termsOfServiceUrl("www.yuque.com/fcant")
                .description("Activiti Service API")
                .build();
    }

}
```
<a name="JFvZQ"></a>
## 7、在resources下面添加xml类型的工作流文件
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596332070186-e809136b-911e-4082-9337-37f66a5aa7fc.png#averageHue=%23f5f3ec&height=687&id=EHSUE&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1179842&status=done&style=shadow&title=&width=1280)
```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<definitions xmlns="http://www.omg.org/spec/BPMN/20100524/MODEL" xmlns:activiti="http://activiti.org/bpmn"
             xmlns:bpmndi="http://www.omg.org/spec/BPMN/20100524/DI"
             xmlns:omgdc="http://www.omg.org/spec/DD/20100524/DC" xmlns:omgdi="http://www.omg.org/spec/DD/20100524/DI"
             xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" expressionLanguage="http://www.w3.org/1999/XPath"
             id="m1572942222337" name="" targetNamespace="http://www.activiti.org/test"
             typeLanguage="http://www.w3.org/2001/XMLSchema">
    <process id="QjFlow" isClosed="false" isExecutable="true" processType="None">
        <startEvent id="startevent1" name="开始">
            <documentation id="startevent1_D_1"/>
        </startEvent>
        <userTask activiti:candidateUsers="${approveUserIds}" activiti:exclusive="true" id="usertask2" name="用户审批"/>
        <exclusiveGateway gatewayDirection="Unspecified" id="exclusivegateway1" name="审核网关"/>
        <userTask activiti:assignee="${applyUserId}" activiti:exclusive="true" id="usertask1" name="提交申请"/>
        <endEvent id="endevent1" name="结束"/>
        <sequenceFlow id="flow3" sourceRef="usertask2" targetRef="exclusivegateway1"/>
        <sequenceFlow id="flow6" sourceRef="startevent1" targetRef="usertask1"/>
        <sequenceFlow id="flow7" sourceRef="usertask1" targetRef="usertask2"/>
        <userTask activiti:assignee="${applyUserId}" activiti:exclusive="true" id="_2" name="重新提交"/>
        <exclusiveGateway gatewayDirection="Unspecified" id="_5" name="审核网关"/>
        <sequenceFlow id="_6" sourceRef="_2" targetRef="_5"/>
        <sequenceFlow id="_7" sourceRef="_5" targetRef="usertask2">
            <conditionExpression xsi:type="tFormalExpression"><![CDATA[${result==1}]]></conditionExpression>
        </sequenceFlow>
        <sequenceFlow id="_8" sourceRef="_5" targetRef="endevent1">
            <conditionExpression xsi:type="tFormalExpression"><![CDATA[${result==0}]]></conditionExpression>
        </sequenceFlow>
        <sequenceFlow id="_9" sourceRef="exclusivegateway1" targetRef="endevent1">
            <conditionExpression xsi:type="tFormalExpression"><![CDATA[${result==1 || result==2}]]></conditionExpression>
        </sequenceFlow>
        <sequenceFlow id="_10" sourceRef="exclusivegateway1" targetRef="_2">
            <conditionExpression xsi:type="tFormalExpression"><![CDATA[${result==0}]]></conditionExpression>
        </sequenceFlow>
    </process>
    <bpmndi:BPMNDiagram documentation="background=#3C3F41;count=1;horizontalcount=1;orientation=0;width=842.4;height=1195.2;imageableWidth=832.4;imageableHeight=1185.2;imageableX=5.0;imageableY=5.0" id="Diagram-_1" name="New Diagram">
        <bpmndi:BPMNPlane bpmnElement="QjFlow">
            <bpmndi:BPMNShape bpmnElement="startevent1" id="Shape-startevent1">
                <omgdc:Bounds height="32.0" width="32.0" x="75.0" y="140.0"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="32.0" width="32.0" x="0.0" y="0.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNShape>
            <bpmndi:BPMNShape bpmnElement="usertask2" id="Shape-usertask2">
                <omgdc:Bounds height="55.0" width="105.0" x="320.0" y="130.0"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="55.0" width="105.0" x="0.0" y="0.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNShape>
            <bpmndi:BPMNShape bpmnElement="exclusivegateway1" id="Shape-exclusivegateway1" isMarkerVisible="false">
                <omgdc:Bounds height="32.0" width="32.0" x="480.0" y="140.0"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="32.0" width="32.0" x="0.0" y="0.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNShape>
            <bpmndi:BPMNShape bpmnElement="usertask1" id="Shape-usertask1">
                <omgdc:Bounds height="55.0" width="105.0" x="165.0" y="130.0"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="55.0" width="105.0" x="0.0" y="0.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNShape>
            <bpmndi:BPMNShape bpmnElement="endevent1" id="Shape-endevent1">
                <omgdc:Bounds height="32.0" width="32.0" x="685.0" y="140.0"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="32.0" width="32.0" x="0.0" y="0.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNShape>
            <bpmndi:BPMNShape bpmnElement="_2" id="Shape-_2">
                <omgdc:Bounds height="55.0" width="85.0" x="455.0" y="260.0"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="55.0" width="85.0" x="0.0" y="0.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNShape>
            <bpmndi:BPMNShape bpmnElement="_5" id="Shape-_5" isMarkerVisible="false">
                <omgdc:Bounds height="32.0" width="32.0" x="340.0" y="365.0"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="32.0" width="32.0" x="0.0" y="0.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNShape>
            <bpmndi:BPMNEdge bpmnElement="flow3" id="BPMNEdge_flow3" sourceElement="usertask2" targetElement="exclusivegateway1">
                <omgdi:waypoint x="425.0" y="157.5"/>
                <omgdi:waypoint x="480.0" y="156.0"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="-1.0" width="-1.0" x="-1.0" y="-1.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNEdge>
            <bpmndi:BPMNEdge bpmnElement="_6" id="BPMNEdge__6" sourceElement="_2" targetElement="_5">
                <omgdi:waypoint x="455.0" y="287.5"/>
                <omgdi:waypoint x="372.0" y="381.0"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="0.0" width="0.0" x="0.0" y="0.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNEdge>
            <bpmndi:BPMNEdge bpmnElement="flow6" id="BPMNEdge_flow6" sourceElement="startevent1" targetElement="usertask1">
                <omgdi:waypoint x="107.0" y="156.0"/>
                <omgdi:waypoint x="165.0" y="157.5"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="-1.0" width="-1.0" x="-1.0" y="-1.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNEdge>
            <bpmndi:BPMNEdge bpmnElement="_7" id="BPMNEdge__7" sourceElement="_5" targetElement="usertask2">
                <omgdi:waypoint x="340.0" y="381.0"/>
                <omgdi:waypoint x="290.0" y="315.0"/>
                <omgdi:waypoint x="320.0" y="157.5"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="0.0" width="0.0" x="0.0" y="0.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNEdge>
            <bpmndi:BPMNEdge bpmnElement="flow7" id="BPMNEdge_flow7" sourceElement="usertask1" targetElement="usertask2">
                <omgdi:waypoint x="270.0" y="157.5"/>
                <omgdi:waypoint x="320.0" y="157.5"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="-1.0" width="-1.0" x="-1.0" y="-1.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNEdge>
            <bpmndi:BPMNEdge bpmnElement="_8" id="BPMNEdge__8" sourceElement="_5" targetElement="endevent1">
                <omgdi:waypoint x="371.0" y="380.0"/>
                <omgdi:waypoint x="600.0" y="380.0"/>
                <omgdi:waypoint x="685.0" y="156.0"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="0.0" width="0.0" x="0.0" y="0.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNEdge>
            <bpmndi:BPMNEdge bpmnElement="_9" id="BPMNEdge__9" sourceElement="exclusivegateway1" targetElement="endevent1">
                <omgdi:waypoint x="512.0" y="156.0"/>
                <omgdi:waypoint x="685.0" y="156.0"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="0.0" width="0.0" x="0.0" y="0.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNEdge>
            <bpmndi:BPMNEdge bpmnElement="_10" id="BPMNEdge__10" sourceElement="exclusivegateway1" targetElement="_2">
                <omgdi:waypoint x="496.0" y="172.0"/>
                <omgdi:waypoint x="496.0" y="260.0"/>
                <bpmndi:BPMNLabel>
                    <omgdc:Bounds height="0.0" width="0.0" x="0.0" y="0.0"/>
                </bpmndi:BPMNLabel>
            </bpmndi:BPMNEdge>
        </bpmndi:BPMNPlane>
    </bpmndi:BPMNDiagram>
</definitions>
```
<a name="EQNhp"></a>
## 8、WorkFlow业务逻辑以及接口设计
<a name="Qcqs3"></a>
### WorkFlowService.java
```java
package com.fcant.service_acti.service;

import javax.servlet.http.HttpServletResponse;
import java.util.List;
import java.util.Map;

/**
 * WorkFlowService
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 22:21 2020/8/1/0001
 */
public interface WorkFlowService {

    /**
     * 启动流程
     * @param pdKey
     * @param businessKey
     * @param variables
     * @return
     */
    String startWorkflow(String pdKey, String businessKey, Map<String,Object> variables);

    /**
     * 继续流程
     * @param taskId
     * @param variables
     */
    void continueWorkflow(String taskId, Map variables);

    /**
     * 委托流程
     * @param taskId
     * @param variables
     */
    void delegateWorkflow(String taskId, Map variables);

    /**
     * 结束流程(一般不使用，让流程正常结束)
     * @param pProcessInstanceId
     */
    void endWorkflow(String pProcessInstanceId,String deleteReason);

    /**
     * 获取当前的任务节点
     * @param pProcessInstanceId
     */
    String getCurrentTask(String pProcessInstanceId);


    /**
     * 查询用户待办流程实例ID集合
     * @param userId
     * @param pdKey
     * @return
     */
    List<String> findUserProcessIds(String userId, String pdKey, Integer pageNo, Integer pageSize);

    /**
     * 获取流程图像，已执行节点和流程线高亮显示
     */
    void getProcessImage(String pProcessInstanceId, HttpServletResponse response);
}
```
<a name="R9ktt"></a>
### WorkFlowServiceImpl.java
```java
package com.fcant.service_acti.service.impl;

import com.fcant.service_acti.service.WorkFlowService;
import lombok.extern.slf4j.Slf4j;
import org.activiti.bpmn.model.BpmnModel;
import org.activiti.bpmn.model.FlowNode;
import org.activiti.bpmn.model.SequenceFlow;
import org.activiti.engine.*;
import org.activiti.engine.history.HistoricActivityInstance;
import org.activiti.engine.history.HistoricProcessInstance;
import org.activiti.engine.repository.ProcessDefinition;
import org.activiti.engine.runtime.ProcessInstance;
import org.activiti.engine.task.DelegationState;
import org.activiti.engine.task.Task;
import org.activiti.image.ProcessDiagramGenerator;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

/**
 * WorkFlowServiceImpl
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 22:04 2020/8/1/0001
 */
@Service
@Slf4j
@Transactional(rollbackFor = Exception.class)
public class WorkFlowServiceImpl implements WorkFlowService {
    @Autowired
    private RepositoryService repositoryService;

    @Autowired
    private RuntimeService runtimeService;

    @Autowired
    private TaskService taskService;

    @Autowired
    private HistoryService historyService;

    @Autowired
    private ProcessEngine processEngine;

    public static final String DEAL_USER_ID_KEY = "dealUserId";

    public static final String DELEGATE_STATE = "PENDING";

    /**
     * 启动工作流
     *
     * @param pdKey
     * @param businessKey
     * @param variables
     * @return
     */
    @Override
    public String startWorkflow(String pdKey, String businessKey, Map<String,Object> variables) {
        ProcessDefinition processDef = getLatestProcDef(pdKey);
        if (processDef == null) {
            // 部署流程
            processEngine.getRepositoryService()
                    .createDeployment()//创建部署对象
                    .name(pdKey)
                    .addClasspathResource("processes/"+pdKey+".bpmn")
                    .deploy();
            processDef = getLatestProcDef(pdKey);
        }
        ProcessInstance process  = runtimeService.startProcessInstanceById(processDef.getId(), businessKey, variables);
        return process.getId();
    }

    /**
     * 继续流程
     *
     * @param taskId
     * @param variables
     */
    @Override
    public void continueWorkflow(String taskId, Map variables){
        //根据taskId提取任务
        Task task = taskService.createTaskQuery().taskId(taskId).singleResult();
        DelegationState delegationState = task.getDelegationState();
        if(delegationState != null && DELEGATE_STATE.equals(delegationState.toString())){
            // 委托任务，先需要被委派人处理完成任务
            taskService.resolveTask(taskId,variables);
        }else {
            // 当前受理人
            String dealUserId =variables.get(DEAL_USER_ID_KEY).toString();
            // 签收
            taskService.claim(taskId, dealUserId);
        }
        // 设置参数
        taskService.setVariables(taskId, variables);
        // 完成
        taskService.complete(taskId);
    }

    /**
     * 委托流程
     * @param taskId
     * @param variables
     */
    @Override
    public void delegateWorkflow(String taskId, Map variables){
        // 受委托人
        String dealUserId =variables.get(DEAL_USER_ID_KEY).toString();
        // 委托
        taskService.delegateTask(taskId, dealUserId);
    }

    /**
     * 结束流程
     * @param pProcessInstanceId
     */
    @Override
    public void endWorkflow(String pProcessInstanceId,String deleteReason){
        // 结束流程
        runtimeService.deleteProcessInstance(pProcessInstanceId, deleteReason);
    }

    /**
     * 获取当前的任务节点
     * @param pProcessInstanceId
     */
    @Override
    public String getCurrentTask(String pProcessInstanceId){
        Task task = taskService.createTaskQuery().processInstanceId(pProcessInstanceId).active().singleResult();
        return task.getId();
    }

    /**
     *
     * 根据用户id查询待办流程实例ID集合
     *
     */
    @Override
    public List<String> findUserProcessIds(String userId, String pdKey, Integer pageNo, Integer pageSize) {
        List<Task> resultTask;
        if(pageSize == 0 ){
            // 不分页
            resultTask = taskService.createTaskQuery().processDefinitionKey(pdKey)
                    .taskCandidateOrAssigned(userId).list();
        }else {
            resultTask = taskService.createTaskQuery().processDefinitionKey(pdKey)
                    .taskCandidateOrAssigned(userId).listPage(pageNo-1,pageSize);
        }
        //根据流程实例ID集合
        List<String> processInstanceIds = resultTask.stream()
                .map(task -> task.getProcessInstanceId())
                .collect(Collectors.toList());
        return processInstanceIds == null ? new ArrayList<>() : processInstanceIds;
    }

    /**
     * 获取流程图像，已执行节点和流程线高亮显示
     */
    @Override
    public void getProcessImage(String pProcessInstanceId, HttpServletResponse response) {
        log.info("[开始]-获取流程图图像");
        // 设置页面不缓存
        response.setHeader("Pragma", "No-cache");
        response.setHeader("Cache-Control", "no-cache");
        response.setDateHeader("Expires", 0);
        response.setContentType("image/png");
        InputStream imageStream = null;
        try (OutputStream os = response.getOutputStream()){
            //  获取历史流程实例
            HistoricProcessInstance historicProcessInstance = historyService.createHistoricProcessInstanceQuery()
                    .processInstanceId(pProcessInstanceId).singleResult();

            if (historicProcessInstance == null) {
                System.out.println("获取流程实例ID[" + pProcessInstanceId + "]对应的历史流程实例失败！");
            } else {
                // 获取流程历史中已执行节点，并按照节点在流程中执行先后顺序排序
                List<HistoricActivityInstance> historicActivityInstanceList = historyService.createHistoricActivityInstanceQuery()
                        .processInstanceId(pProcessInstanceId).orderByHistoricActivityInstanceId().asc().list();

                // 已执行的节点ID集合
                List<String> executedActivityIdList = new ArrayList<String>();
                int index = 1;
                log.info("获取已经执行的节点ID");
                for (HistoricActivityInstance activityInstance : historicActivityInstanceList) {
                    executedActivityIdList.add(activityInstance.getActivityId());

                    log.info("第[" + index + "]个已执行节点=" + activityInstance.getActivityId() + " : " +activityInstance.getActivityName());
                    index++;
                }
                // 获取流程定义
                BpmnModel bpmnModel = repositoryService.getBpmnModel(historicProcessInstance.getProcessDefinitionId());

                // 已执行的线集合
                List<String> flowIds = getHighLightedFlows(bpmnModel, historicActivityInstanceList);

                // 流程图生成器
                ProcessDiagramGenerator pec = processEngine.getProcessEngineConfiguration().getProcessDiagramGenerator();
                // 获取流程图图像字符流（png/jpg）
                imageStream = pec.generateDiagram(bpmnModel, "jpg", executedActivityIdList, flowIds, "宋体", "微软雅黑", "黑体", null, 2.0);

                int bytesRead = 0;
                byte[] buffer = new byte[8192];
                while ((bytesRead = imageStream.read(buffer, 0, 8192)) != -1) {
                    os.write(buffer, 0, bytesRead);
                }

            }
            log.info("[完成]-获取流程图图像");
        } catch (Exception e) {
            log.error("【异常】-获取流程图失败！",e);
        }finally {
            if(imageStream != null){
                try {
                    imageStream.close();
                } catch (IOException e) {
                    log.error("关闭流异常：",e);
                }
            }
        }
    }

    public List<String> getHighLightedFlows(BpmnModel bpmnModel, List<HistoricActivityInstance> historicActivityInstances) {
        // 24小时制
        SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        // 用以保存高亮的线flowId
        List<String> highFlows = new ArrayList<String>();

        for (int i = 0; i < historicActivityInstances.size() - 1; i++) {
            // 对历史流程节点进行遍历
            // 得到节点定义的详细信息
            FlowNode activityImpl = (FlowNode) bpmnModel.getMainProcess().getFlowElement(historicActivityInstances.get(i).getActivityId());

            // 用以保存后续开始时间相同的节点
            List<FlowNode> sameStartTimeNodes = new ArrayList<FlowNode>();
            FlowNode sameActivityImpl1 = null;
            // 第一个节点
            HistoricActivityInstance activityImpl_ = historicActivityInstances.get(i);
            HistoricActivityInstance activityImp2_;

            for (int k = i + 1; k <= historicActivityInstances.size() - 1; k++) {
                // 后续第1个节点
                activityImp2_ = historicActivityInstances.get(k);

                if (activityImpl_.getActivityType().equals("userTask") && activityImp2_.getActivityType().equals("userTask") &&
                        df.format(activityImpl_.getStartTime()).equals(df.format(activityImp2_.getStartTime()))) {
                    // 都是usertask，且主节点与后续节点的开始时间相同，说明不是真实的后继节点
                } else {
                    //找到紧跟在后面的一个节点
                    sameActivityImpl1 = (FlowNode) bpmnModel.getMainProcess().getFlowElement(historicActivityInstances.get(k).getActivityId());
                    break;
                }

            }
            // 将后面第一个节点放在时间相同节点的集合里
            sameStartTimeNodes.add(sameActivityImpl1);
            for (int j = i + 1; j < historicActivityInstances.size() - 1; j++) {
                // 后续第一个节点
                HistoricActivityInstance activityImpl1 = historicActivityInstances.get(j);
                // 后续第二个节点
                HistoricActivityInstance activityImpl2 = historicActivityInstances.get(j + 1);

                if (df.format(activityImpl1.getStartTime()).equals(df.format(activityImpl2.getStartTime()))) {
                    // 如果第一个节点和第二个节点开始时间相同保存
                    FlowNode sameActivityImpl2 = (FlowNode) bpmnModel.getMainProcess().getFlowElement(activityImpl2.getActivityId());
                    sameStartTimeNodes.add(sameActivityImpl2);
                } else {// 有不相同跳出循环
                    break;
                }
            }
            // 取出节点的所有出去的线
            List<SequenceFlow> pvmTransitions = activityImpl.getOutgoingFlows();
            // 对所有的线进行遍历
            for (SequenceFlow pvmTransition : pvmTransitions) {
                // 如果取出的线的目标节点存在时间相同的节点里，保存该线的id，进行高亮显示
                FlowNode pvmActivityImpl = (FlowNode) bpmnModel.getMainProcess().getFlowElement(pvmTransition.getTargetRef());
                if (sameStartTimeNodes.contains(pvmActivityImpl)) {
                    highFlows.add(pvmTransition.getId());
                }
            }

        }
        return highFlows;

    }

    /**
     * 获取最新版本流程
     *
     * @param modelName
     * @return
     */
    private ProcessDefinition getLatestProcDef(String modelName) {
        return repositoryService.createProcessDefinitionQuery().processDefinitionKey(modelName).
                latestVersion().singleResult();
    }

}
```
<a name="V9FoW"></a>
### ActivitiController.java
```java
package com.fcant.service_acti.controller;

import com.fcant.service_acti.result.Result;
import com.fcant.service_acti.service.WorkFlowService;
import io.swagger.annotations.Api;
import io.swagger.annotations.ApiOperation;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import javax.servlet.http.HttpServletResponse;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

/**
 * ActivitiController
 * <p>
 * encoding:UTF-8
 *
 * @author Fcant 下午 22:16 2020/8/1/0001
 */
@RestController
@Api(tags = "activiti",value = "工作流控制器")
@RequestMapping("/activiti")
@Slf4j
public class ActivitiController {

    @Autowired
    private WorkFlowService workFlowService;


    @PostMapping("/apply")
    @ApiOperation(value="启动请假流程")
    public Result startWorkflow(@RequestParam(required = false) String pdKey){
        Map param = new HashMap(4){{
            put("applyUserId","001");
            put("approveUserIds", Arrays.asList("001","002","003"));
        }};

        if(StringUtils.isBlank(pdKey)){
            pdKey="QjFlow";
        }
        // 启动流程
        String pdId = workFlowService.startWorkflow(pdKey, "QJ001", param);
        // 获取请假申请任务节点
        String Id = workFlowService.getCurrentTask(pdId);
        // 完成请假申请任务节点
        Map continueParam = new HashMap(2){{
            put("dealUserId",param.get("applyUserId"));
        }};
        workFlowService.continueWorkflow(Id,continueParam);
        return Result.success().reSetMsg("请假已提交");
    }

    @PostMapping("/approve")
    @ApiOperation(value="审批请假流程")
    public Result continueWorkflow(@RequestParam String pId,@RequestParam String result){
        Map param = new HashMap(2){{
            put("dealUserId","001");
            put("result",result);
        }};

        // 获取请假审批任务节点
        String Id = workFlowService.getCurrentTask(pId);
        // 完成请假审批任务节点
        workFlowService.continueWorkflow(Id,param);
        return Result.success().reSetMsg("审批成功");
    }

    @PostMapping("/delegate")
    @ApiOperation(value="委托请假流程")
    public Result delegateWorkflow(@RequestParam String pId,@RequestParam String userId){
        Map param = new HashMap(2){{
            put("dealUserId",userId);
        }};
        // 获取请假审批任务节点
        String Id = workFlowService.getCurrentTask(pId);
        // 完成请假审批任务节点
        workFlowService.delegateWorkflow(Id,param);
        return Result.success().reSetMsg("委托成功");
    }

    /**
     *  查询用户待办流程实例
     * @param userId
     * @param pdKey
     */
    @GetMapping("/user-process")
    @ApiOperation(value="查询用户待办流程实例")
    public Result findUserProcessIds(@RequestParam String userId, @RequestParam(required = false) String pdKey) {
        if(StringUtils.isBlank(pdKey)){
            pdKey="QjFlow";
        }
        // 获取流程图
        return Result.success().addData("workflow", workFlowService.findUserProcessIds(userId,pdKey,1,0));
    }

    /**
     * 读取流程资源
     * @param pId 流程实例id
     */
    @GetMapping("/read-resource")
    @ApiOperation(value="读取流程资源")
    public void readResource(@RequestParam String pId, HttpServletResponse response) {
        // 获取流程图
        workFlowService.getProcessImage(pId, response);
    }

}
```
<a name="ORROi"></a>
## 9、启动项目，生成28张acti表
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596332108782-6824abe5-8079-4bde-bef7-a625e15721e4.png#averageHue=%23f5f4f0&height=687&id=z7t2M&originHeight=2060&originWidth=3840&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1138186&status=done&style=shadow&title=&width=1280)
<a name="X4vVy"></a>
## 10、访问接口进行测试
![image.png](https://cdn.nlark.com/yuque/0/2020/png/396745/1596362370938-71e2d266-6804-4a6c-b561-33686597ff4e.png#averageHue=%238cbd0d&height=686&id=pcJD5&originHeight=2058&originWidth=3798&originalType=binary&ratio=1&rotation=0&showTitle=false&size=451867&status=done&style=shadow&title=&width=1266)

