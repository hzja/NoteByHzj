Activiti6 表结构
<a name="aVLKU"></a>
## 数据库设计规则
| 表分类 | 描述 | 说明 |
| :--- | :--- | :--- |
| ACT_GE_* | 通用数据表（GE 代表 General） |   |
| ACT_RE_* | 流程定义存储表 （RE 代表 Repository） | 保存一些 ‘静态’信息，如流程定义和流程资源（如图片、规则等）； |
| ACT_ID_* | 身份信息表 (ID 代表 Identity) |   |
| ACT_RU_* | 运行时记录表 （RU 代表 Runtime） | 并且当流程结束后会立即移除这些数据，<br />这是为了保证运行时表尽量的小并运行的足够快； |
| ACT_HI_* | 历史数据表 （HI 代表 History） | **Activiti提供4中历史级别 **<br />**none**: 不保存任何历史记录，可以提高系统性能；<br />**activity**：保存所有的流程实例、任务、活动信息；<br />**audit**：也是Activiti的**默认**级别，保存所有的流程实例、任务、活动、表单属性；<br />** full**： 最完整的历史记录，除了包含**audit**级别的信息之外还能保存详细，例如：流程变量。 |

<a name="Wxzpy"></a>
## 1.通用数据表（ACT_GE_*）
<a name="cxQSy"></a>
### 1.1 ACT_EVT_LOG (act_evt_log) > 事件日志表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| LOG_NR_ | bigint | 20 | 是 | 主键自增 |
| TYPE_ | varchar | 64 | 否 | 类型 |
| PROC_DEF_ID_ | varchar | 64 | 否 | 流程定义ID |
| PROC_INST_ID_ | varchar | 64 | 否 | 流程实例ID |
| EXECUTION_ID_ | varchar | 64 | 否 | 执行ID |
| TASK_ID_ | varchar | 64 | 否 | 任务ID |
| TIME_STAMP_ | timestamp |   | 是 | 时间 |
| USER_ID_ | varchar | 255 | 否 | 用户ID |
| DATA_ | longblob |   | 否 | 内容 |
| LOCK_OWNER_ | varchar | 255 | 否 | 锁定节点 |
| LOCK_TIME_ | timestamp |   | 否 | 锁定时间 |
| IS_PROCESSED_ | tinyint | 4 | 是 | 是否正在执行, 默认 0 |

<a name="GAKNW"></a>
### 1.2 ACT_GE_PROPERTY > 属性表
属性抽象成为 key-value对，使用该表来保存这些属性。

| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| NAME_ | varchar | 64 | 是 | 属性名称 |
| VALUE_ | varchar | 300 | 否 | 属性值 |
| REV_ | int | 11 | 否 | 数据版本号 |

<a name="wIlK1"></a>
### 1.3 ACT_GE_BYTEARRAY >  资源表
注：用来保存部署文件的大文本数据。<br />保存流程定义图片和xml、Serializable(序列化)的变量,即保存所有二进制数据，特别注意类路径部署时候，不要把svn等隐藏文件或者其他与流程无关的文件也一起部署到该表中，会造成一些错误（可能导致流程定义无法删除）

| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| REV_ | int | 11 | 否 | 数据版本号 |
| NAME_ | varchar | 255 | 否 | 资源名称 |
| DEPLOYMENT_ID_ | varchar | 64 | 否 | ACT_RE_DEPLOYMENT部署表的主键。<br />一次部署可以关联多个资源 |
| BYTES_ | longblob |   | 否 | 资源内容，数据类型为longblob，最大可存储4GB的数据 |
| GENERATED_ | tinyint |   | 否 | 是否为自动产生的资源， 0-false，1-true |

<a name="5cLuL"></a>
## 2.流程定义存储表（ACT_RE_*）
<a name="T8TNm"></a>
### 2.1 ACT_RE_DEPLOYMENT >  部署数据表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| NAME_ | varchar | 255 | 否 | 部署名称 |
| CATEGORY_ | varchar | 255 | 否 | 分类 |
| KEY_ | varchar | 255 | 否 | 关键字 |
| TENANT_ID_ | varchar | 255 | 否 | 租户ID |
| DEPLOY_TIME_ | timestamp |   | 否 | 部署时间 |

<a name="tKYME"></a>
### 2.2 ACT_RE_MODEL >  模型信息表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| REV_ | int | 11 | 否 | 数据版本号 |
| NAME_ | varchar | 255 | 否 | 模型名称 |
| KEY_ | varchar | 255 | 否 | 模型关键字 (流程引擎使用) |
| CATEGORY_ | varchar | 255 | 否 | 模型分类 |
| CREATE_TIME_ | timestamp |   | 否 | 创建时间 |
| LAST_UPDATE_TIME_ | timestamp |   | 否 | 最后修改时间 |
| VERSION_ | int | 11 | 否 | 模型版本 |
| META_INFO_ | varchar | 4000 | 否 | 数据源信息(以json格式保存流程定义的信息) |
| DEPLOYMENT_ID_ | varchar | 64 | 否 | 部署ID，ACT_RE_DEPLOYMENT > ID |
| EDITOR_SOURCE_VALUE_ID_ | varchar | 64 | 否 | 编辑源值ID |
| EDITOR_SOURCE_EXTRA_VALUE_ID_ | varchar | 64 | 否 | 编辑源扩展值ID |
| TENANT_ID_ | varchar | 255 | 否 | 租户ID |

<a name="osris"></a>
### 2.3 ACT_RE_PROCDEF >  流程定义表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键（组合主键） |
| REV_ | int | 11 | 否 | 数据版本号 |
| CATEGORY_ | varchar | 255 | 否 | 流程命名空间分类 (流程定义的Namespace) |
| NAME_ | varchar | 255 | 否 | 流程定义名称 (文件process元素的name属性值) |
| KEY_ | varchar | 255 | 否 | 流程定义key (文件process元素的id属性值) |
| VERSION_ | int | 11 | 否 | 流程定义的版本 |
| DEPLOYMENT_ID_ | varchar | 64 | 否 | 关联部署表主键 |
| RESOURCE_NAME_ | varchar | 4000 | 否 | 流程定义的对应的资源名称，一般为文件相对路径 |
| DGRM_RESOURCE_NAME_ | varchar | 4000 | 否 | 流程定义的对应流程图的资源名称<br />(流程bpmn文件名称) |
| DESCRIPTION_ | varchar | 4000 | 否 | 流程定义描述 |
| HAS_START_FORM_KEY_ | tinyint | 4 | 否 | 是否存在开始表单标记<br />0-否 , 1-是 |
| HAS_GRAPHICAL_NOTATION_ | tinyint | 4 | 否 | 是否有图形信息 |
| SUSPENSION_STATE_ | int | 11 | 否 | 挂起状态 1正常 2-挂起 |
| TENANT_ID_ | varchar | 25 | 否 | 租户ID |
| ENGINE_VERSION_ | varchar | 255 | 否 | 引擎版本 |

<a name="riWhj"></a>
### 2.4 ACT_PROCDEF_INFO >  流程定义扩展表
| 字段 | 类型 | 长度 | 描述 |
| :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 主键 |
| PROC_DEF_ID_ | varchar | 64 | 流程定义ID<br />外键：ACT_RE_PROCDEF |
| REV_ | int | 11 | 版本号 |
| INFO_JSON_ID_ | varchar | 64 | 内容，资源表ID<br />外键：ACT_GE_BYTEARRAY |

<a name="2gv2L"></a>
## 3.身份数据表
<a name="iqcdO"></a>
### 3.1 ACT_ID_USER > 用户信息表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| REV_ | int | 11 | 否 | 数据版本号 |
| FIRST_ | varchar | 255 | 否 | 用户名称 |
| LAST_ | varchar | 255 | 否 | 用户姓氏 |
| EMAIL_ | varchar | 255 | 否 | 用户邮箱 |
| PWD | varchar | 255 | 否 | 用户密码 |
| PICTURE_ID_ | varchar | 64 | 否 | 用户头像图片，对应资源表的数据ID |

<a name="VfXPN"></a>
### 3.2 ACT_ID_INFO > 用户扩展信息表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| REV_ | int | 11 | 否 | 数据版本号 |
| USER_ID_ | varchar | 64 | 否 | 用户ID（用户表主键） |
| TYPE_ | varchar | 64 | 否 | 信息类型<br />account - 用户的帐号<br />userinfo - 用户信息 |
| KEY_ | varchar | 255 | 否 | 数据的键 |
| VALUE_ | varchar | 255 | 否 | 数据的值 |
| PASSWORD_ | longblob |   | 否 | 用户帐号的密码字段<br />注：当前Activiti并没有使用该字段 |
| PARENT_ID_ | varchar | 255 | 否 | 该信息的父信息ID |

<a name="0lg0X"></a>
### 3.3 ACT_ID_GROUP > 用户组表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| REV_ | int | 11 | 否 | 数据版本号 |
| NAME_ | varchar | 255 | 否 | 组名称 |
| TYPE_ | varchar | 255 | 否 | 组类型 |

<a name="GgDKu"></a>
### 3.4 ACT_ID_MEMBERSHIP > 用户与组关系表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| USER_ID_ | varchar | 64 | 是 | 用户ID（用户表主键） |
| GROUP_ID_ | varchar | 64 | 是 | 用户组ID（用户组表主键） |

<a name="SPd9k"></a>
## 4.运行时流程数据表
<a name="rB1Yj"></a>
### 4.1 ACT_RU_EXECUTION > 运行时流程实例执行实例表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| REV_ | int | 11 | 否 | 数据版本号 |
| PROC_INST_ID_ | varchar | 64 | 否 | 流程实例ID<br />一个流程实例有可能会产生多个执行流，该字段表示执行流所属的流程实例 |
| BUSINESS_KEY_ | varchar | 255 | 否 | 启动流程时指定的业务主键。(一般存业务表主键) |
| PARENT_ID_ | varchar | 64 | 否 | 父流程实例ID,<br />一个流程实例有可能会产生执行流，新的执行流数据以该字段标识其所属的流程实例。 |
| PROC_DEF_ID_ | varchar | 64 | 否 | 流程定义ID |
| SUPER_EXEC_ | varchar | 64 | 否 | 外部子流程 |
| ROOT_PROC_INST_ID_ | varchar | 64 | 否 |   |
| ACT_ID_ | varchar | 255 | 否 | 当前执行流ID, ID在流程文件中定义。 |
| IS_ACTIVE_ | tinyint | 4 | 否 | 激活状态 0非 1 是 |
| IS_CONCURRENT_ | tinyint | 4 | 否 | 并发状态 0非 1 是 |
| IS_SCOPE_ | tinyint | 4 | 否 | 是否主流程实例   0非 1 是 |
| IS_EVENT_SCOPE_ | tinyint | 4 | 否 | 是事件  0非 1 是 |
| IS_MI_ROOT_ | tinyint | 4 | 否 |   |
| SUSPENSION_STATE_ | int | 11 | 否 | 挂起状态 1-正常 2-挂起 |
| CACHED_ENT_STATE_ | int | 11 | 否 | 流程实体的缓存，取值为0~7 |
| TENANT_ID_ | varchar | 255 | 否 | 租户ID |
| NAME_ | varchar | 255 | 否 | 名称 |
| START_TIME_ | datetime |   | 否 | 启动时间 |
| START_USER_ID_ | varchar | 255 | 否 | 启动用户 |
| LOCK_TIME_ | timestamp |   | 否 | 锁定时间 |
| IS_COUNT_ENABLED_ | tinyint | 4 | 否 |   |
| EVT_SUBSCR_COUNT_ | int | 11 | 否 |   |
| TASK_COUNT_ | int | 11 | 否 |   |
| JOB_COUNT_ | int | 11 | 否 |   |
| TIMER_JOB_COUNT_ | int | 11 | 否 |   |
| SUSP_JOB_COUNT_ | int | 11 | 否 |   |
| DEADLETTER_JOB_COUNT_ | int | 11 | 否 |   |
| VAR_COUNT_ | int | 11 | 否 |   |
| ID_LINK_COUNT_ | int | 11 | 否 |   |

<a name="xRhXe"></a>
### 4.2 ACT_RU_TASK > 用户任务表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| REV_ | int | 11 | 否 | 数据版本号 |
| EXECUTION_ID_ | varchar | 64 | 否 | 执行流ID |
| PROC_INST_ID_ | varchar | 64 | 否 | 流程实例ID |
| PROC_DEF_ID_ | varchar | 64 | 否 | 流程定义ID |
| NAME_ | varchar | 64 | 否 | 任务名称 (在流程文件中定义) |
| PARENT_TASK_ID_ | varchar | 64 | 否 | 父任务ID (子任务才会设置该字段的值) |
| DESCRIPTION_ | varchar | 4000 | 否 | 任务描述 (在流程文件中配置) |
| TASK_DEF_KEY_ | varchar | 255 | 否 | 任务定义的ID值,(在流程文件中定义) |
| OWNER_ | varchar | 255 | 否 | 任务拥有人 (无外键关联) |
| ASSIGNEE_ | varchar | 255 |   | 代理人，被指派执行该任务的人 (无外键关联) |
| DELEGATION_ | varchar | 64 | 否 | 任务委托状态<br />PENDING - 委托中<br />RESOLVED - 已处理 |
| PRIORITY_ | int | 11 | 否 | 任务优先级，默认为50 |
| CREATE_TIME_ | timestamp |   | 否 | 创建时间 |
| DUE_DATE_ | datetime |   | 否 | 任务预订日期 |
| CATEGORY_ | varchar |   | 否 | 类型 |
| SUSPENSION_STATE_ | int | 11 | 否 | 暂停状态 1-正常 2-挂起 |
| TENANT_ID_ | varchar | 255 | 否 | 租户ID |
| FORM_KEY_ | varchar | 255 | 否 | 表单ID |
| CLAIM_TIME_ | datetime |   | 否 | 签收时间 |

<a name="07439c8a"></a>
### 4.3 ACT_RU_VARIABLE > 流程参数数据表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| REV_ | int | 11 | 否 | 数据版本号 |
| TYPE_ | varchar | 255 | 否 | 参数类型,不为空<br />可选值：boolean. bytes、serializable. date. double、 integer、jpa-entity、long、 null、 short 、string<br />以上字段值由Activiti 提供，也可自定义扩展类型 |
| NAME_ | varchar | 255 | 否 | 参数名称 |
| EXECUTION_ID_ | varchar | 64 | 否 | 对应的执行ID |
| PROC_INST_ID_ | varchar | 64 | 否 | 对应的流程实例ID |
| TASK_ID_ | varchar | 64 | 否 | 任务ID (若是任务参数，则需要设置) |
| BYTEARRAY_ID_ | varchar | 64 | 否 | 资源表ID<br />(若参数值是序列化对象，可将该对象作为资源保存到资源表中） |
| DOUBLE_ | double |   | 否 | 参数类型为double,则值会保存到该字段中 |
| LONG_ | bigint | 20 | 否 | 参数类型为long, 则值会保存到该字段中。 |
| TEXT_ | varchar | 4000 | 否 | 用于保存文本类型的参数值，该字段为varchar 类型，长度为4 000。 |
| TEXT2_ | varchar | 4000 | 否 | 文本类型的参数值。 |

<a name="Hs5KJ"></a>
### 4.4 ACT_RU_IDENTITYLINK > 参与者信息表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| REV_ | int | 11 | 否 | 数据版本号 |
| GROUP_ID_ | varchar | 255 | 否 | 用户组ID |
| TYPE_ | varchar | 255 | 否 | 该关系数据的类型，当前提供了3个值:<br />assignee、 candidate和owner, 表示流程数据的指派人(组)、候选人(组)和拥有人 |
| USER_ID_ | varchar | 255 | 否 | 用户ID |
| TASK_ID_ | varchar | 64 | 否 | 任务ID |
| PROC_INST_ID_ | varchar | 64 | 否 | 流程实例ID |
| PROC_DEF_ID_ | varchar | 64 | 否 | 流程定义ID |
|   |   |   |   |   |

<a name="LhaDn"></a>
### 4.5 ACT_RU_EVENT_SUBSCR > 运行时事件表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| REV_ | int | 11 | 否 | 数据版本号 |
| EVENT_TYPE_ | varchar | 255 | 否 | 事件类型，message,signal |
| EVENT_NAME_ | varchar | 255 | 否 | 事件名称 |
| EXECUTION_ID_ | varchar | 64 | 否 | 流程执行ID |
| PROC_INST_ID_ | varchar | 64 | 否 | 流程实例ID |
| ACTIVITY_ID_ | varchar | 64 | 否 | 时间活动ID |
| CONFIGURATION_ | varchar | 255 | 否 | 属性配置信息 |
| CREATED_ | timestamp |   | 否 | 创建时间 |
| PROC_DEF_ID_ | varchar | 64 | 否 | 流程定义ID |
| TENANT_ID_ | varchar | 255 | 否 | 租户ID |

<a name="8c4Ry"></a>
### 4.6 ACT_RU_JOB > 运行作业信息表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| REV_ | int | 11 | 否 | 数据版本号 |
| TYPE_ | varchar | 255 | 否 | 类型，值为message或timer |
| LOCK_EXP_TIME_ | timestamp |   | 否 | 锁定的结束时间 |
| LOCK_OWNER_ | varchar | 255 | 否 | 挂起者，锁定标识，默认为UUID |
| EXCLUSIVE_ | tinyint | 1 | 否 | 是否唯一 |
| EXECUTION_ID_ | varchar | 64 | 否 | 执行实例ID |
| PROCESS_INSTANCE_ID_ | varchar | 64 | 否 | 流程实例ID |
| PROC_DEF_ID_ | varchar | 64 | 否 | 流程定义ID |
| RETRIES_ | int | 11 | 否 | 工作的剩余执行次数，默认值为3 |
| EXCEPTION_STACK_ID_ | varchar | 64 | 否 | 异常堆栈（资源表ID） |
| EXCEPTION_MSG_ | varchar | 4000 | 否 | 异常信息 |
| DUEDATE_ | timestamp |   | 否 | 工作执行时间 |
| REPEAT_ | varchar | 255 | 否 | 工作重复执行次数 |
| HANDLER_TYPE_ | varchar | 255 | 否 | 处理器类型 |
| HANDLER_CFG_ | varchar | 4000 | 否 | 处理器配置 |
| TENANT_ID_ | varchar | 255 | 否 | 租户ID |

<a name="hFsnL"></a>
### 4.7 ACT_RU_DEADLETTER_JOB > 无法执行工作表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| REV_ | int | 11 | 否 | 数据版本号 |
| TYPE_ | varchar | 255 | 否 | 类型，值为message或timer |
| EXCLUSIVE_ | tinyint | 1 | 否 | 是否唯一 |
| EXECUTION_ID_ | varchar | 64 | 否 | 执行实例ID |
| PROCESS_INSTANCE_ID_ | varchar | 64 | 否 | 流程实例ID |
| PROC_DEF_ID_ | varchar | 64 | 否 | 流程定义ID |
| EXCEPTION_STACK_ID_ | varchar | 64 | 否 | 异常堆栈（资源表ID） |
| EXCEPTION_MSG_ | varchar | 4000 | 否 | 异常信息 |
| DUEDATE_ | timestamp |   | 否 | 工作执行时间 |
| REPEAT_ | varchar | 255 | 否 | 工作重复执行次数 |
| HANDLER_TYPE_ | varchar | 255 | 否 | 处理器类型 |
| HANDLER_CFG_ | varchar | 4000 | 否 | 处理器配置 |
| TENANT_ID_ | varchar | 255 | 否 | 租户ID |

<a name="fiVOD"></a>
### 4.8 ACT_RU_SUSPENDED_JOB > 暂停表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| REV_ | int | 11 | 否 | 数据版本号 |
| TYPE_ | varchar | 255 | 否 | 类型，值为message或timer |
| EXCLUSIVE_ | tinyint | 1 | 否 | 是否唯一 |
| EXECUTION_ID_ | varchar | 64 | 否 | 执行实例ID |
| PROCESS_INSTANCE_ID_ | varchar | 64 | 否 | 流程实例ID |
| PROC_DEF_ID_ | varchar | 64 | 否 | 流程定义ID |
| RETRIES_ | int | 11 | 否 | 工作的剩余执行次数，默认值为3 |
| EXCEPTION_STACK_ID_ | varchar | 64 | 否 | 异常堆栈（资源表ID） |
| EXCEPTION_MSG_ | varchar | 4000 | 否 | 异常信息 |
| DUEDATE_ | timestamp |   | 否 | 工作执行时间 |
| REPEAT_ | varchar | 255 | 否 | 工作重复执行次数 |
| HANDLER_TYPE_ | varchar | 255 | 否 | 处理器类型 |
| HANDLER_CFG_ | varchar | 4000 | 否 | 处理器配置 |
| TENANT_ID_ | varchar | 255 | 否 | 租户ID |

<a name="3ixLa"></a>
### 4.9 ACT_RU_TIMER_JOB > 定时器表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| REV_ | int | 11 | 否 | 数据版本号 |
| TYPE_ | varchar | 255 | 否 | 类型，值为message或timer |
| LOCK_EXP_TIME_ | timestamp |   | 否 | 锁定的结束时间 |
| LOCK_OWNER_ | varchar | 255 | 否 | 挂起者，锁定标识，默认为UUID |
| EXCLUSIVE_ | tinyint | 1 | 否 | 是否唯一 |
| EXECUTION_ID_ | varchar | 64 | 否 | 执行实例ID |
| PROCESS_INSTANCE_ID_ | varchar | 64 | 否 | 流程实例ID |
| PROC_DEF_ID_ | varchar | 64 | 否 | 流程定义ID |
| RETRIES_ | int | 11 | 否 | 工作的剩余执行次数，默认值为3 |
| EXCEPTION_STACK_ID_ | varchar | 64 | 否 | 异常堆栈（资源表ID） |
| EXCEPTION_MSG_ | varchar | 4000 | 否 | 异常信息 |
| DUEDATE_ | timestamp |   | 否 | 工作执行时间 |
| REPEAT_ | varchar | 255 | 否 | 工作重复执行次数 |
| HANDLER_TYPE_ | varchar | 255 | 否 | 处理器类型 |
| HANDLER_CFG_ | varchar | 4000 | 否 | 处理器配置 |
| TENANT_ID_ | varchar | 255 | 否 | 租户ID |

<a name="g8BiJ"></a>
## 5. 历史数据表
<a name="p5cTk"></a>
### 5.1 ACT_HI_PROCINST 历史流程实例信息表
与正在运行中的流程实例表act_ru_execution正好对应

| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| PROC_INST_ID_ | varchar | 64 | 是 | 流程实例ID |
| BUSINESS_KEY_ | varchar | 255 | 否 | 业务主键标识 |
| PROC_DEF_ID_ | varchar | 64 | 否 | 流程定义ID |
| START_TIME_ | datetime |   |   | 开始时间 |
| END_TIME_ | datetime |   |   | 结束时间 |
| DURATION_ | bigint | 20 |   | 时长 |
| START_USER_ID_ | varchar | 255 |   | 发起人ID |
| START_ACT_ID_ | varchar | 255 |   | 开始节点ID |
| END_ACT_ID_ | varchar | 255 |   | 结束节点ID |
| SUPER_PROCESS_INSTANCE_ID_ | varchar | 64 |   | 超级流程实例ID |
| DELETE_REASON_ | varchar | 4000 |   | 删除理由 |
| TENANT_ID_ | varchar | 255 |   | 租户ID |
| NAME_ | varchar | 255 |   | 名称 |

<a name="2hV2y"></a>
### 5.2 ACT_HI_DETAIL 流程明细表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| TYPE_ | varchar | 255 | 否 | 数据类型:<br />FormProperty, //表单<br />VariableUpdate //参数 |
| PROC_INST_ID_ | varchar | 64 | 否 | 流程实例ID |
| EXECUTION_ID_ | varchar | 64 | 否 | 执行流程ID |
| TASK_ID_ | varchar | 64 | 否 | 任务ID |
| ACT_INST_ID_ | varchar | 64 | 否 | 活动实例Id |
| NAME_ | varchar | 255 | 否 | 名称 |
| VAR_TYPE_ | varchar | 255 | 否 | 变量类型<br />对应流程文件标签<br />示例：startEvent、userTask |
| REV_ | int | 11 | 否 | 数据版本号 |
| TIME_ | datetime |   | 否 | 创建时间 |
| BYTEARRAY_ID_ | varchar | 64 | 否 | 字节数组ID |
| DOUBLE_ | double |   | 否 | 存储Double类型变量 |
| LONG_ | bigint | 20 | 否 | 存储long类型变量 |
| TEXT_ | varchar | 4000 | 否 |   |
| TEXT2_ | varchar | 4000 | 否 |   |
|   |   |   |   |   |

<a name="juoAb"></a>
### 5.3 ACT_HI_ACTINS 历史节点表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| PROC_DEF_ID_ | varchar | 64 | 否 | 流程定义ID |
| PROC_INST_ID_ | varchar | 64 | 否 | 流程实例ID |
| EXECUTION_ID_ | varchar | 255 | 否 | 执行流ID |
| ACT_ID_ | varchar | 255 | 否 | 流程活动的ID, 在流程文件中定义 |
| TASK_ID_ | varchar | 255 | 否 | 任务ID |
| CALL_PROC_INST_ID_ | varchar | 64 | 否 | 请求流程实例ID<br />调用外部流程的流程实例ID' |
| ACT_NAME_ | varchar | 255 | 否 | 活动的名称 |
| ACT_TYPE_ | varchar | 255 | 否 | 活动类型，<br />例如开始事件，活动名称为startEvent |
| ASSIGNEE_ | varchar | 255 | 否 | 活动指派人 |
| START_TIME_ | datetime |   | 否 | 活动开始时间，即执行流到达活动时的时间 |
| END_TIME_ | datetime |   | 否 | 活动结束时间，即执行流离开该活动时的时间 |
| DURATION_ | bigint | 20 | 否 | 耗时，毫秒值 |
| DELETE_REASON_ | varchar | 4000 | 否 | 删除原因 |
| TENANT_ID_ | varchar | 255 | 否 | 租户ID |

<a name="E1sQ0"></a>
### 5.4 ACT_HI_TASKINST 历史任务流程实例信息
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| PROC_DEF_ID_ | varchar | 64 | 否 | 流程定义ID |
| TASK_DEF_KEY_ | varchar | 255 | 否 | 节点定义ID |
| PROC_INST_ID_ | varchar | 64 | 否 | 流程实例ID |
| EXECUTION_ID_ | varchar | 64 | 否 | 执行实例ID |
| NAME_ | varchar | 255 | 否 | 名称 |
| PARENT_TASK_ID_ | varchar | 64 | 否 | 父节点实例ID |
| DESCRIPTION_ | varchar | 4000 | 否 | 描述 |
| OWNER_ | varchar | 255 | 否 | 任务拥有人 |
| ASSIGNEE_ | varchar | 255 | 否 | 代理人 |
| START_TIME_ | datetime |   | 是 | 开始时间 |
| CLAIM_TIME_ | datetime |   | 否 | 提醒时间 |
| END_TIME_ |   |   | 否 | 结束时间 |
| DURATION_ | bigint | 20 | 否 | 时长 |
| DELETE_REASON_ | varchar | 4000 | 否 | 删除理由 |
| PRIORITY_ | int | 11 | 否 | 优先级 |
| DUE_DATE_ | datetime |   | 否 | 应完成时间 |
| FORM_KEY_ | varchar | 255 | 否 | 表单key |
| CATEGORY_ | varchar | 255 | 否 | 分类 |
| TENANT_ID_ | varchar | 255 | 否 | 租户ID |

<a name="cwEIN"></a>
### 5.5 ACT_HI_IDENTITYLINK > 历史流程人员表
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| GROUP_ID_ | varchar | 64 | 否 | 用户组ID（用户组表主键） |
| TYPE_ | varchar | 255 | 否 | 用户组类型<br />主要分为：ssignee、candidate、<br />owner、starter 、participant |
| USER_ID_ | varchar | 255 | 否 | 用户ID |
| TASK_ID_ | varchar | 64 | 否 | 任务ID |
| PROC_INST_ID_ | varchar | 64 | 否 | 流程实例ID |

<a name="J1Pdw"></a>
### 5.6 ACT_HI_COMMENT > 历史审批意见表 
| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| GROUP_ID_ | varchar | 64 | 否 | 用户组ID（用户组表主键） |
| TYPE_ | varchar | 255 | 否 | 类型<br />event -事件<br />comment - 意见 |
| USER_ID_ | varchar | 255 | 否 | 用户ID |
| TASK_ID_ | varchar | 64 | 否 | 任务ID |
| PROC_INST_ID_ | varchar | 64 | 否 | 流程实例ID |
| ACTION_ | varchar | 255 | 否 | 行为类型<br />值为下列内容中的一种：<br />AddUserLink、DeleteUserLink、AddGroupLink、DeleteGroupLink、AddComment、AddAttachment、DeleteAttachment |
| MESSAGE_ | varchar | 4000 | 否 | 处理意见，例如审批意见 |
| FULL_MSG_ | longblob |   |   | 全部消息 |

<a name="WlM3u"></a>
### 5.7 ACT_HI_VARINST > 历史变量信息
与运行中的参数存储表 act_ru_variable正好对应

| 字段 | 类型 | 长度 | 是否必填 | 描述 |
| :--- | :--- | :--- | :--- | :--- |
| ID_ | varchar | 64 | 是 | 主键 |
| PROC_INST_ID_ | varchar | 64 | 否 | 流程实例ID |
| EXECUTION_ID_ | varchar | 64 | 否 | 执行ID |
| TASK_ID_ | varchar | 64 | 否 | 任务ID |
| NAME_ | varchar | 255  | 否 | 名称 |
| VAR_TYPE_ | varchar | 100   | 否 | 变量类型 |
| REV_ | int | 11 | 否 | 数据版本号 |
| BYTEARRAY_ID_ | varchar | 64 | 否 | 对应资源表ID |
| DOUBLE_ | double |   | 否 | 存储double类型参数 |
| LONG_ | bigint | 20 | 否 | 存储long类型参数 |
| TEXT_ | varchar | 4000 | 否 |   |
| TEXT2_ | varchar | 4000 | 否 |   |
| CREATE_TIME_ | datetime |   | 否 | 创建时间 |
| LAST_UPDATED_TIME_ | datetime |   | 否 | 最后修改时间 |

 
