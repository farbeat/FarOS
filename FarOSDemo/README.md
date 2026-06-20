# FarBeatDemo

### 版本
-------------------------------------
Farbeat Demo v1.0.0
适用系统
Android8.0 及以上
iOS 10及以上

适用智能设备
  华为
    HUAWEI WATCH GT2
    HUAWEI WATCH GT2 Pro
-------------------------------------
Farbeat Demo v1.0.1
适用系统
Android8.0 及以上
iOS 10及以上

适用智能设备
  华为
    HUAWEI WATCH GT2
    HUAWEI WATCH GT2 Pro
  东方泰华
    单道心电记录仪（TKECG-H01）存储版
-------------------------------------
Farbeat Demo v1.0.2
适用系统
Android8.0 及以上
iOS 10及以上

适用智能设备
  华为
    HUAWEI WATCH GT2
    HUAWEI WATCH GT2 Pro
  东方泰华
    单道心电记录仪（TKECG-H01）存储版
  宜准
    C023
-------------------------------------

### 概述
Farbeat 是针对基于运动健康设备所产生的海量的杂乱的健康行为数据进行加工处理，提取高价值信息供研究和健康干预所用。该项目可以看做是一个健康大数据的提炼工厂。在国家相关管理机构的指导和帮助下，通过标准化的输入和输出，提高健康大数据行业的标准化、规范化，推动行业发展。
目前主要包括：
- 运行在网关终端的 Farbeat SDK 中间件软件
- 运行在运动健康设备等终端 IC 芯片中的 Farbeat AIR 算法模型库
  ![](/webfile/upload/2022/01-24/18-13-010423-1813350723.jpg)
- Farbeat SDK 中间件。该中间件的目的是为了实现运动健康领域不同设备之间数据交换和质量标准统一，减少服务系统的开发者的工作量，增强运动健康设备数据加工能力，提高其价值，促进可穿戴行业的可持续发展。Farbeat 是基于相关标准开发的第三方软件，为运动健康设备厂商和服务提供商提供一个全栈式的解决方案。主要分为：设备管理、协议适配和计算标准化三大类。

  包括：

  - farbeat.Manger //管理类，配合服务提供商对算法模型库数据计算进行校验的管理
  - farbeat.Adaptor //适配类，适配不同运动健康设备厂商的各设备终端协议的数据接口
  - farbeat.Open //开放类，对外提供统一标准数据的接口
  - farbeat.DQE //数据质量增强。提供数据清洗、滤波、信号增强等计算
  - farbeat.HCE //计算引擎类，负责提供统一的计算模型
  - farbeat.AIM //AI 模型管理类，负责管理 AI 训练模型

- Farbeat AIR 算法库。包括大量运动行为识别算法，负责人体运动行为和健康态智能识别等功能（仅限特定用户）。

#### 主要功能

1.  统一的设备管理
2.  统一的数据采集
3.  标准的数据计算


#### 接口说明

### farbeat.Manager.oauthToken

获取accessToken:

```java
Manager.oauthToken(TokenCallBack callBack);
```

参数说明：

```java
callBack：获取acceccToken接口请求回调
```

示例：

```java
Manager.oauthToken(new TokenCallBack() {
    @Override
    public void onSuccess(String accessToken) {
        // TODO：获取成功
    }
});
```

>注：accessToken有效期30分钟

### farbeat.Manager.generatePid

获取Pid:

```java
Manager.generatePid(String accessToken, PidCallBack callBack);
```

参数说明：

```java
acceccToken：获取accessToken接口返回
callBack：获取pid接口请求回调
```

示例：

```java
Manager.generatePid(accessToken, new PidCallBack() {
    @Override
    public void onSuccess(String pid) {
        // TODO：获取成功
    }
});
```

>pid永久唯一有效，获取过无需再次获取

## farbeat.Adaptor.ApplicationInitialize

SDK初始化

#### 实现接口

```java
interface ApplicationInitialize {
        void init(Context context, boolean isDebug);
    }
```

#### 参数说明：

```java
context：上下文
isDebug：是否打印日志
```

#### 示例

```java
public class ApplicationInitialize implements Adaptor.ApplicationInitialize {

    @Override
    public void init(Context context, boolean isDebug) {
        // TODO 实现自己的初始化方法
    }
}
```

## farbeat.Adaptor.DeviceManager

设备管理

#### 实现接口

```java
interface DeviceManager {
        void scanSourceDevice(OnScanDeviceListener listener);
        void registerStateMonitor(FarBeatDevice device, OnConnectDeviceListener listener);
        void connectSourceDevice(FarBeatDevice device);
        void removeSourceDevice(FarBeatDevice device, OnSensorResponseListener listener);
    }
```

#### 方法说明：

void scanSourceDevice(OnScanDeviceListener listener); // 扫描设备

```java
listener：扫描结果监听
```

void registerStateMonitor(FarBeatDevice device, OnConnectDeviceListener listener); // 设备连接监听

```java
device：统一扫描设备对象
listener：设备连接结果监听
```

void connectSourceDevice(FarBeatDevice device); // 设备连接

```java
device：统一扫描设备对象
```

void removeSourceDevice(FarBeatDevice device, OnSensorResponseListener listener); // 移除设备

```java
device：统一扫描设备对象
listener：移除设备结果监听
```

#### 示例

```java
public class DeviceManager implements Adaptor.DeviceManager {

    @Override
    public void scanSourceDevice(final OnScanDeviceListener listener) {
        // TODO 实现自己的设备扫描方法，将结果传入OnScanDeviceListener监听中
    }

    @Override
    public void registerStateMonitor(final FarBeatDevice device, final OnConnectDeviceListener listener) {
        // TODO 首先，根据统一扫描设备对象FarBeatDevice实现自己的设备监听方法，将结果传入OnConnectDeviceListener监听中
		// TODO 其次，建议延时1秒启动设备连接方法
    }

    @Override
    public void connectSourceDevice(FarBeatDevice device) {
        // TODO 根据统一扫描设备对象FarBeatDevice实现自己的设备连接方法
    }

    @Override
    public void removeSourceDevice(FarBeatDevice device, final OnSensorResponseListener listener) {
        // TODO 根据统一扫描设备对象FarBeatDevice实现自己的设备断连方法，将结果传入OnSensorResponseListener监听中
    }
}
```

## farbeat.Adaptor.RealTimeData

实时数据

#### 实现接口

```java
interface RealTimeData {
        void registerRealTimeMeasure(OnHealthDataListener<HealthDataReport> listener);
        void startRealTimeMeasure(int duration);
        void stopRealTimeMeasure(OnSensorResponseListener listener);
    }
```

#### 方法说明：

void registerRealTimeMeasure(OnHealthDataListener< HealthDataReport> listener); // 实时数据监听

```java
listener：实时数据结果监听
```

void startRealTimeMeasure(int duration); // 开启实时数据

```java
duration：实时数据采集时长，单位：秒
```

void stopRealTimeMeasure(OnSensorResponseListener listener); // 停止实时数据

```java
listener：停止实时数据结果监听
```

#### 示例

```java
public class RealTimeData implements Adaptor.RealTimeData {

    @Override
    public void registerRealTimeMeasure(final OnHealthDataListener<HealthDataReport> listener) {
        // TODO 根据标准规范整合采集到的数据，传入OnHealthDataListener统一输出
    }

    @Override
    public void startRealTimeMeasure(int duration) {
        // TODO 启动指定时长的数据采集，duration：单位：秒
    }

    @Override
    public void stopRealTimeMeasure(OnSensorResponseListener listener) {
        // TODO 停止实时数据采集，并将响应结果传入OnSensorResponseListener
    }
}
```

### SDK初始化

下载管理平台配置文件放入工程assets目录下，文件：farbeat-sdk-device.json。

```java
void init(Context context, boolean isDebug);
void setIdentification(String appId, String appSecret, long dpSdkSn);
```

#### 参数说明

```java
context：上下文
isDebug：是否打印日志
appId：管理平台生成ID
appSecret：管理平台生成秘钥
dpSdkSn：管理平台生成SDK唯一标识码
```

#### 示例

```java
FarBeat farBeat = FarBeat.getInstance();
farBeat.init(mContext, true);
farBeat.setIdentification(appId, appSecret, dpSdkSn);
```

### farbeat.Open.getHealthDataSourceConfigList

获取数据源配置信息

```java
Open.getHealthDataSourceConfigList(HealthDataSourceConfigCallback<HealthDataSourceConfigList> callback);
```

参数说明：

```java
callBack：获取数据源配置信息请求回调
```

示例：

```java
Open.getHealthDataSourceConfigList(new HealthDataSourceConfigCallback<HealthDataSourceConfigList>() {
    @Override
    public void onResponse(int errorCode, HealthDataSourceConfigList configList) {
        // TODO errorCode：错误码；configList：数据源配置信息
    }
});
```

##### 数据源配置信息返回参数说明

| 字段名       | 数据类型                   | 说明                                                         |
| ------------ | -------------------------- | ------------------------------------------------------------ |
| errorCode    | int                        | 返回错误码。<br> - 取值为0：获取数据成功。所获得的数据在HealthDataSourceConfigList之中。<br> - 取值为10001：获取数据成功，但无数据(HealthDataSourceConfigList不存在)<br> - 取值为10002：获取数据失败。 |
| returnObject | HealthDataSourceConfigList | 返回数据为一个列表，包含着该网关设备已配置的所有数据源的配置信息。 |

#### HealthDataSourceConfigList

| 字段名             | 数据类型 | 说明 |
| ------------------ | -------- | ---- |
| configList | List< HealthDataSourceConfig>   | 所有数据源配置信息列表 |

##### HealthDataSourceConfig

| 字段名                      | 数据类型 | 说明                                                         |
| --------------------------- | -------- | ------------------------------------------------------------ |
| dataSourceTemplateId | long     | 数据处理SDK为每一个已配置的数据源赋予一个内部的唯一编号，以达到标识的目的。 |
| deviceSourceTemplateId | long     | 某种类型设备的唯一ID，可对数据进行以设备类型归类计算。 |
| schemeOfDataSourceType      | String   | 数据源的类别的描述方式。固定长度，20个字节，以ASCII编码的文本。 可能值包括： “Farbeat”“11073”“ICD”“LOINC”…… |
| countOfCompound             | int      | 表示该数据源所产生的数据内部所包含的数据元素的个数。本字段的值必须是大于或等于1的整数。例如：<br>1）采集体温的传感器所形成的数据源，其内部的数据元素只有1个。 <br>2）同时采集x、y、z轴的加速度传感器所形成的数据源，其内部的数据元素就有3个。 |
| dataSourceTypeList          | long[]  | 依次表示该数据源内部各数据元素的类别。数据源的类别的允许值，见[附录A](http://www.farbeat.org.cn/guide.html?id=184 "附录A") |
| unitCodeList                | long[]  | 依次表示该数据源内部各数据元素的数值单位。数值单位的允许值，见[附录B](http://www.farbeat.org.cn/guide.html?id=185 "附录B")。如某个数据元素的类别为枚举型，则本字段中对应的数值单位的取值为0。 |
| dataTypeList                | int[]   | 依次表示该数据源内部各数据元素的数据类型。 类型的取值如下：<br> - 取值为0：无类型（如枚举数据）；<br> - 取值为1：short；<br> - 取值为2：int；<br> - 取值为3：float；<br> - 取值为4：String；<br>其它取值：保留 |
| dataValueTypeList          | int[]   | 依次表示该数据源内部各数据元素的数值类型。 类型的取值如下：<br> - 取值为0：数值型；<br> - 取值为1：枚举型；<br>其它取值：保留 |
| metricSpecList              | int[]   | 依次表示该数据源内部各数据元素的度量属性类型。 类型的取值如下：<br> - 取值为1：直接测量值；<br> - 取值为2：人工输入值；<br> - 取值为3：测度值(经过计算而得到的值，例如BMI值)；<br>其它取值：保留 |
| samplingFrequency           | int      | 固定采样频率。单位为赫兹(hz)。 如果本数据源是以固定采样频率产生测量值，则samplingFrequency的值表示该数据源所产生的每两个相邻数据之间的采样间隔时间；如果本数据源不是以固定采样频率产生测量值，则samplingFrequency的值为0 |
| measurePeriodExist          | int      | 表示该数据源的输出值后面是否伴随着测量时间窗的存在。<br> - 取值为0：不存在；<br> - 取值为1：存在；<br>其它取值：保留。<br>只有HealthDataSetSingle中才有可能包含“测量时间窗”。对于HealthDataSetArray所对应的数据源，其measurePeriodExist的取值始终为0。 注：测量时间窗的值是一个代表时间长度的整数，其单位为秒(s)。以数据源输出值的时间戳为终点，往前覆盖的相应时间长度即为测量时间窗。如果输出值伴随有测量时间窗，则该输出值是基于时间窗内的测量活动而产生的。例如：数据源输出60秒之内的平均心率值，其对应的测量时间窗为60秒。 |

#### 配置信息Json示例

```json
{
  "configList":[
    {
      "countOfCompound":1,
      "dataSourceTemplateId":394219517,
      "deviceSourceTemplateId":10955454746,
      "dataSourceTypeList":[
        8454276
      ],
      "dataTypeList":[
        1
      ],
      "dataValueTypeList":[
        0
      ],
      "measurePeriodExist":0,
      "metricSpecList":[
        1
      ],
      "samplingFrequency":250,
      "schemeOfDataSourceType":"11073",
      "unitCodeList":[
        264864
      ]
    }
  ]
}
```

### farbeat.Open.getSourceDeviceConnected

启动设备扫描，扫描过程中会在弹窗中显示扫描到的所有设备，点击其中的某个设备即可执行连接功能。

```java
Open.getSourceDeviceConnected(FragmentActivity fragmentActivity);
```

参数说明

```java
fragmentActivity：上下文，用于搜索界面弹窗
```

示例

```java
Open.getSourceDeviceConnected(mActivity);
```

### farbeat.Open.setOnConnectDeviceListener

设备扫描连接全局监听

```java
Open.setOnConnectDeviceListener(OnConnectDeviceListener listener);
```

参数说明：

```java
onConnectDeviceListener：设备连接结果监听
```

示例：

```java
Open.setOnConnectDeviceListener(new OnConnectDeviceListener() {
    @Override
    public void onStartConnect(FarBeatDevice device) {
        // TODO 启动连接（所有设备连接的起点）
    }

    @Override
    public void onConnecting(FarBeatDevice device) {
        // TODO 正在连接中...（可选重写）
    }

    @Override
    public void onConnected(FarBeatDevice device) {
        // TODO 已连接
    }

    @Override
    public void onDisconnected(FarBeatDevice device) {
        // TODO 已断开
    }

    @Override
    public void onConnectFailed(FarBeatDevice device) {
        // TODO 连接失败（可选重写）
    }

    @Override
    public void onMeasuring(FarBeatDevice device) {
        // TODO 正在测量中（可选重写）
    }
});
```

>为适配多设备的连接规则，部分方法可选择性重写

### FarBeatDevice

| 字段名      | 数据类型 | 说明                   |
| ----------- | -------- | ---------------------- |
| name        | String   | 设备名称               |
| mac         | String   | 设备蓝牙地址           |
| basePackage | String   | 厂商基础包名           |
| device      | Object   | 各厂商设备扫描时的对象 |

### farbeat.Open.removeSourceDevice

移除设备连接

```java
Open.removeSourceDevice(FarBeatDevice device, OnSensorResponseListener listener);
```

参数说明：

```java
device：设备交互过程中的对象
listener：移除设备请求结果监听
```

示例：

```java
Open.removeSourceDevice(farBeatDevice, new OnSensorResponseListener() {
    @Override
    public void onResponse(boolean isSuccess, String msg) {
        // TODO isSuccess：请求结果是否成功；msg：结果返回的说明，可能为空
    }
});
```

### farbeat.Open.startRealTimeMeasure

开启设备实时数据采集

```java
Open.startRealTimeMeasure(FarBeatDevice device, int duration);
```

参数说明：

```java
device：设备交互过程中的对象
duration：采集时长，单位：秒
```

示例（10分钟采集为例）：

```java
Open.getRealTimeDataProvider().startRealTimeMeasure(farBeatDevice, 600);
```

### farbeat.Open.setOnHealthDataListener

实时数据采集全局监听

```java
Open.setOnHealthDataListener(OnHealthDataListener<HealthDataReport> onHealthDataListener);
```

参数说明：

```java
onHealthDataListener：实时数据采集的结果返回
```

示例：

```java
Open.setOnHealthDataListener(new OnHealthDataListener<HealthDataReport>() {
    @Override
    public void onHealthData(int errorCode, HealthDataReport report) {
    }
});
```

## 统一标准健康数据返回参数说明

| 字段名       | 数据类型         | 说明                                                         |
| ------------ | ---------------- | ------------------------------------------------------------ |
| errorCode    | int              | 返回错误码。取值为0：获取数据成功。所获得的数据在HealthData之中。取值为10001：获取数据成功，但无新增数据(returnObject不存在)取值为10002：获取数据失败。 |
| returnObject | HealthDataReport | 返回数据                                                     |

#### HealthDataReport

| 字段名              | 数据类型      | 说明                                                         |
| ------------------- | ------------- | ------------------------------------------------------------ |
| numOfHealthDataSet  | int           | 当前HealthDataReport中所包含的HealthDataSet数量              |
| arrayOfDataSourceTemplateID | long[]        | 一个整数数组，长度为numOfHealthData (共有n个数)，每一个数依次对应着后面的每一个healthDataSet，代表着该healthDataSet的数据来源编号(DataSourceTemplateID)。 |
| arrayOfDataCount    | int[]        | 一个整数数组，长度为numOfHealthData (共有n个数)，每一个数依次对应着后面的每一个healthDataSet，代表着该healthDataSet所包含的healthData的个数。 |
| arrayOfDataSetType  | int[]        | 一个整数数组，长度为numOfHealthData (共有n个数)，每一个数依次对应着后面的每一个healthDataSet，代表着该healthDataSet的结构类型。<br> - 取值为1：结构类型为HealthDataSetSingle；<br> - 取值为2：结构类型为HealthDataSetArray；<br>其它取值：保留 |
| healthDataSet 1     | HealthDataSet | 其结构类型是HealthDataSetSingle或HealthDataArray。           |
| healthDataSet 2     | HealthDataSet | 其结构类型是HealthDataSingle或HealthDataArray。              |
| ……                  | ……            | ……                                                           |
| healthDataSet n     | HealthDataSet | 其结构类型是HealthDataSingle或HealthDataArray。              |
| sourceDeviceId        | DeviceID    | 包含本数据源所源自的设备的标识               |

HealthDataSet的结构类型可以是HealthDataSingle和HealthDataArray之中的一个。

##### HealthDataSetSingle

结构类型为HealthDataSetSingle的数据为配有时间戳(毫秒)的单个测量值, 其内部结构为一个二元或三元的数组：

| 第1个healthData | 时间戳 | 测量时间窗 |
| --------------- | ------ | ---------- |
| 第2个healthData | 时间戳 | 测量时间窗 |
| ……              | ……     | 测量时间窗 |
| 第m个healthData | 时间戳 | 测量时间窗 |

measurePeriodExist的取值决定了“测量时间窗”的值是否出现在HealthDataSetSingle的结构中。

##### HealthDataSetArray

结构类型为HealthDataSetArray的数据为配有起始时间戳的一组测量值, 其内部结构包含一个代表着测量值起始时间的时间戳，以及一个一元的数组：

| 字段名          | 数据类型   | 说明                                       |
| --------------- | ---------- | ------------------------------------------ |
| startTime       | 毫秒时间戳     | 本HealthDataSetArray的测量值起始时间       |
| healthDataArray | healthData | 一元的数组，依时间顺序排列的若干healthData |

healthDataArray的内部结构为：

| 第1个healthData |
| --------------- |
| 第2个healthData |
| … …             |
| 第m个healthData |

###### HealthData

每一个healthData都是数据源所输出的一个值，其内部结构为：

| 第1个数据元素 | 第2个数据元素 | … …  | 第k个数据元素 |
| ------------- | ------------- | ---- | ------------- |

其内部的数据元素的个数与对应的countOfCompound字段值一致。

| 字段名         | 数据类型 | 说明   |
| -------------- | -------- | ------ |
| value | Object | 值（根据配置文件中dataType的参数值进行转换） |
| timestamp | long | 毫秒时间戳  |
| timeWindows | int | 时间窗值（单位：秒） |

##### DeviceID

| 字段名         | 数据类型 | 说明                                                         |
| -------------- | -------- | ------------------------------------------------------------ |
| nameOfIDScheme | string   | 这个设备所采用的某一种设备标识方式，固定长度为20字节的字符串, 数据内容为ASCII编码的文本。 可能值包括：“EUI-64”、“MAC Address”、“UDI”、“BT Address”、“FDI”…… |
| lengthOfID     | int      | ID的字节长度                                                 |
| id             | string   | nameOfIDScheme所采用的设备标识方式的值                       |

#### 统一标准健康数据Json示例

```json
{
    "arrayOfDataCount":[
        1,
        3
    ],
    "arrayOfDataSetType":[
        1,
        2
    ],
    "arrayOfDataSourceTemplateID":[
        177389973,
        169082411
    ],
    "healthDataSet":[
        {
            "healthDataList":[
                {
                    "timestamp":1645093832373,
                    "value":98
                }
            ]
        },
        {
            "timestamp":1645093832000,
            "healthDataList":[
                {
                    "value":608
                },
                {
                    "value":762
                },
                {
                    "value":697
                }
            ]
        }
    ],
    "sourceDeviceId":{
        "id":"E1:D4:67:68:6F:29",
        "lengthOfID":17,
        "nameOfIDScheme":"BT Address"
    },
    "numOfHealthDataSet":2
}
```

### farbeat.Open.stopRealTimeMeasure

停止实时数据采集

```java
Open.stopRealTimeMeasure(FarBeatDevice device, OnSensorResponseListener listener);
```

参数说明：

```java
device：设备交互过程中的对象
listener：停止实时数据采集请求结果监听
```

示例：

```java
Open.stopRealTimeMeasure(farBeatDevice, new OnSensorResponseListener() {
    @Override
    public void onResponse(boolean isSuccess, String msg) {
        // TODO isSuccess：请求结果是否成功；msg：结果返回的说明，可能为空
    }
});
```

### farbeat.HCE.getHrv

RRI数据计算，获取Hrv结果。

```java
HCE.getHrv(String accessToken, long deviceSourceTemplateId, HrvSource source, HrvCallBack callBack);
```

#### 参数说明

```java
accessToken：获取accessToken接口返回
deviceSourceTemplateId：根据统一标准数据中的配置信息ID从配置信息中获取，deviceSourceTemplateId相同的为一组数据
source：
    String pid; // 获取pid接口返回
    List<Integer> rriList; // rri集合
    List<Long> rriTimeList; // 时间戳集合，单位：毫秒
callBack：计算RRI接口请求回调
```
>注：rriList与timeList尺寸必须相同

#### 示例

```java
HCE.getHrv(accessToken, deviceSourceTemplateId, source, new HrvCallBack() {
@Override
public void onSuccess(HrvBean.HrvData hrv) {
        // TODO：获取成功
        }
        });
```

## HRV返回参数说明

### HrvData

| 字段名         | 数据类型 | 说明   |
| -------------- | -------- | ------ |
| FreqDomainBean | Object   | 频域   |
| NonlinearBean  | Object   | 非线性 |
| PoincareBean   | Object   |        |
| TimeDomainBean | Object   | 时域   |

#### FreqDomainBean

| 字段名             | 数据类型 | 说明 |
| ------------------ | -------- | ---- |
| AR | Object   |      |
| WelcPSDBean        | Object   |      |

##### AR

| 字段名         | 数据类型 | 说明   |
| -------------- | -------- | ------ |
| HF（HfBean）   | Object   | 高频   |
| LF（LfBean）   | Object   | 低频   |
| VLF（VlfBean） | Object   | 极低频 |

###### HfBean

| 字段名 | 数据类型 | 说明                                           |
| ------ | -------- | ---------------------------------------------- |
| Peek   | double   | 高频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 高频频带范围内的功率谱密度的面积               |
| Power2 | double   | 高频能量的占比                                 |
| Power3 | double   |                                                |

###### LfBean

| 字段名 | 数据类型 | 说明                                           |
| ------ | -------- | ---------------------------------------------- |
| Peek   | double   | 低频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 低频频带范围内的功率谱密度的面积               |
| Power2 | double   | 低频能量的占比                                 |
| Power3 | double   |                                                |

###### VlfBean

| 字段名 | 数据类型 | 说明                                             |
| ------ | -------- | ------------------------------------------------ |
| Peek   | double   | 极低频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 极低频频带范围内的功率谱密度的面积               |
| Power2 | double   | 极低频能量的占比                                 |
| Power3 | double   |                                                  |

##### WelcPSDBean

| 字段名          | 数据类型 | 说明   |
| --------------- | -------- | ------ |
| HF（HfBeanX）   | Object   | 高频   |
| LF（LfBeanX）   | Object   | 低频   |
| VLF（VlfBeanX） | Object   | 极低频 |

###### HfBeanX

| 字段名 | 数据类型 | 说明                                           |
| ------ | -------- | ---------------------------------------------- |
| Peek   | double   | 高频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 高频频带范围内的功率谱密度的面积               |
| Power2 | double   | 高频能量的占比                                 |
| Power3 | double   |                                                |

###### LfBeanX

| 字段名 | 数据类型 | 说明                                           |
| ------ | -------- | ---------------------------------------------- |
| Peek   | double   | 低频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 低频频带范围内的功率谱密度的面积               |
| Power2 | double   | 低频能量的占比                                 |
| Power3 | double   |                                                |

###### VlfBeanX

| 字段名 | 数据类型 | 说明                                             |
| ------ | -------- | ------------------------------------------------ |
| Peek   | double   | 极低频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 极低频频带范围内的功率谱密度的面积               |
| Power2 | double   | 极低频能量的占比                                 |
| Power3 | double   |                                                  |

#### NonlinearBean

| 字段名 | 数据类型 | 说明               |
| ------ | -------- | ------------------ |
| IE     | double   |                    |
| SE     | double   | 样本熵             |
| BE     | double   | 基本尺度熵         |
| GE     | double   | 新的子序列产生概率 |

#### PoincareBean

| 字段名 | 数据类型 | 说明               |
| ------ | -------- | ------------------ |
| SD1    | double   | 散点图短半轴的长度 |
| SD2    | double   | 散点图长半轴的长度 |

#### TimeDomainBean

| 字段名 | 数据类型 | 说明                                                         |
| ------ | -------- | ------------------------------------------------------------ |
| MAX    | int      | 记录时间范围内RR间期的最大值                                 |
| MIN    | int      | 记录时间范围内RR间期的最小值                                 |
| MEAN   | double   | 记录时间范围内RR间期的平均值                                 |
| SDNN   | double   | 心跳间期标准偏差                                             |
| RMSSD  | double   | 相邻R-R间期的均方根                                          |
| SDSD   | double   | 相邻RR间期差值的标准差                                       |
| NN50   | int      | 相邻R-R间期差值大于50毫秒的个数                              |
| PNN50  | double   | 相邻R-R间期差值大于50毫秒的个数与总心跳间期的个数的比值的百分比 |

### farbeat.HCE.getHrv5m

5分钟RRI数据计算，获取Hrv结果。

```java
HCE.getHrv5m(String accessToken, long deviceSourceTemplateId, HrvSource source, HrvCallBack callBack);
```

#### 参数说明

```java
accessToken：获取accessToken接口返回
deviceSourceTemplateId：根据统一标准数据中的配置信息ID从配置信息中获取，deviceSourceTemplateId相同的为一组数据
source：
    String pid; // 获取pid接口返回
    List<Integer> rriList; // rri集合
    List<Long> rriTimeList; // 时间戳集合，单位：毫秒
callBack：计算RRI接口请求回调
```
>注：rriList与timeList尺寸必须相同

#### 示例

```java
HCE.getHrv5m(accessToken, deviceSourceTemplateId, source, new HrvCallBack() {
@Override
public void onSuccess(HrvBean.HrvData hrv) {
        // TODO：获取成功
        }
        });
```

## HRV返回参数说明

### HrvData

| 字段名         | 数据类型 | 说明   |
| -------------- | -------- | ------ |
| FreqDomainBean | Object   | 频域   |
| NonlinearBean  | Object   | 非线性 |
| PoincareBean   | Object   |        |
| TimeDomainBean | Object   | 时域   |

#### FreqDomainBean

| 字段名             | 数据类型 | 说明 |
| ------------------ | -------- | ---- |
| AR | Object   |      |
| WelcPSDBean        | Object   |      |

##### AR

| 字段名         | 数据类型 | 说明   |
| -------------- | -------- | ------ |
| HF（HfBean）   | Object   | 高频   |
| LF（LfBean）   | Object   | 低频   |
| VLF（VlfBean） | Object   | 极低频 |

###### HfBean

| 字段名 | 数据类型 | 说明                                           |
| ------ | -------- | ---------------------------------------------- |
| Peek   | double   | 高频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 高频频带范围内的功率谱密度的面积               |
| Power2 | double   | 高频能量的占比                                 |
| Power3 | double   |                                                |

###### LfBean

| 字段名 | 数据类型 | 说明                                           |
| ------ | -------- | ---------------------------------------------- |
| Peek   | double   | 低频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 低频频带范围内的功率谱密度的面积               |
| Power2 | double   | 低频能量的占比                                 |
| Power3 | double   |                                                |

###### VlfBean

| 字段名 | 数据类型 | 说明                                             |
| ------ | -------- | ------------------------------------------------ |
| Peek   | double   | 极低频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 极低频频带范围内的功率谱密度的面积               |
| Power2 | double   | 极低频能量的占比                                 |
| Power3 | double   |                                                  |

##### WelcPSDBean

| 字段名          | 数据类型 | 说明   |
| --------------- | -------- | ------ |
| HF（HfBeanX）   | Object   | 高频   |
| LF（LfBeanX）   | Object   | 低频   |
| VLF（VlfBeanX） | Object   | 极低频 |

###### HfBeanX

| 字段名 | 数据类型 | 说明                                           |
| ------ | -------- | ---------------------------------------------- |
| Peek   | double   | 高频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 高频频带范围内的功率谱密度的面积               |
| Power2 | double   | 高频能量的占比                                 |
| Power3 | double   |                                                |

###### LfBeanX

| 字段名 | 数据类型 | 说明                                           |
| ------ | -------- | ---------------------------------------------- |
| Peek   | double   | 低频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 低频频带范围内的功率谱密度的面积               |
| Power2 | double   | 低频能量的占比                                 |
| Power3 | double   |                                                |

###### VlfBeanX

| 字段名 | 数据类型 | 说明                                             |
| ------ | -------- | ------------------------------------------------ |
| Peek   | double   | 极低频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 极低频频带范围内的功率谱密度的面积               |
| Power2 | double   | 极低频能量的占比                                 |
| Power3 | double   |                                                  |

#### NonlinearBean

| 字段名 | 数据类型 | 说明               |
| ------ | -------- | ------------------ |
| IE     | double   |                    |
| SE     | double   | 样本熵             |
| BE     | double   | 基本尺度熵         |
| GE     | double   | 新的子序列产生概率 |

#### PoincareBean

| 字段名 | 数据类型 | 说明               |
| ------ | -------- | ------------------ |
| SD1    | double   | 散点图短半轴的长度 |
| SD2    | double   | 散点图长半轴的长度 |

#### TimeDomainBean

| 字段名 | 数据类型 | 说明                                                         |
| ------ | -------- | ------------------------------------------------------------ |
| MAX    | int      | 记录时间范围内RR间期的最大值                                 |
| MIN    | int      | 记录时间范围内RR间期的最小值                                 |
| MEAN   | double   | 记录时间范围内RR间期的平均值                                 |
| SDNN   | double   | 心跳间期标准偏差                                             |
| RMSSD  | double   | 相邻R-R间期的均方根                                          |
| SDSD   | double   | 相邻RR间期差值的标准差                                       |
| NN50   | int      | 相邻R-R间期差值大于50毫秒的个数                              |
| PNN50  | double   | 相邻R-R间期差值大于50毫秒的个数与总心跳间期的个数的比值的百分比 |

### farbeat.HCE.getHrv24h

24小时RRI数据计算，获取Hrv结果。

```java
HCE.getHrv24h(String accessToken, long deviceSourceTemplateId, HrvSource source, HrvCallBack callBack);
```

#### 参数说明

```java
accessToken：获取accessToken接口返回
deviceSourceTemplateId：根据统一标准数据中的配置信息ID从配置信息中获取，deviceSourceTemplateId相同的为一组数据
source：
    String pid; // 获取pid接口返回
    List<Integer> rriList; // rri集合
    List<Long> rriTimeList; // 时间戳集合，单位：毫秒
callBack：计算RRI接口请求回调
```
>注：rriList与timeList尺寸必须相同

#### 示例

```java
HCE.getHrv24h(accessToken, deviceSourceTemplateId, source, new HrvCallBack() {
@Override
public void onSuccess(HrvBean.HrvData hrv) {
        // TODO：获取成功
        }
        });
```

## HRV返回参数说明

### HrvData

| 字段名         | 数据类型 | 说明   |
| -------------- | -------- | ------ |
| FreqDomainBean | Object   | 频域   |
| NonlinearBean  | Object   | 非线性 |
| PoincareBean   | Object   |        |
| TimeDomainBean | Object   | 时域   |

#### FreqDomainBean

| 字段名             | 数据类型 | 说明 |
| ------------------ | -------- | ---- |
| AR | Object   |      |
| WelcPSDBean        | Object   |      |

##### AR

| 字段名         | 数据类型 | 说明   |
| -------------- | -------- | ------ |
| HF（HfBean）   | Object   | 高频   |
| LF（LfBean）   | Object   | 低频   |
| VLF（VlfBean） | Object   | 极低频 |

###### HfBean

| 字段名 | 数据类型 | 说明                                           |
| ------ | -------- | ---------------------------------------------- |
| Peek   | double   | 高频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 高频频带范围内的功率谱密度的面积               |
| Power2 | double   | 高频能量的占比                                 |
| Power3 | double   |                                                |

###### LfBean

| 字段名 | 数据类型 | 说明                                           |
| ------ | -------- | ---------------------------------------------- |
| Peek   | double   | 低频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 低频频带范围内的功率谱密度的面积               |
| Power2 | double   | 低频能量的占比                                 |
| Power3 | double   |                                                |

###### VlfBean

| 字段名 | 数据类型 | 说明                                             |
| ------ | -------- | ------------------------------------------------ |
| Peek   | double   | 极低频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 极低频频带范围内的功率谱密度的面积               |
| Power2 | double   | 极低频能量的占比                                 |
| Power3 | double   |                                                  |

##### WelcPSDBean

| 字段名          | 数据类型 | 说明   |
| --------------- | -------- | ------ |
| HF（HfBeanX）   | Object   | 高频   |
| LF（LfBeanX）   | Object   | 低频   |
| VLF（VlfBeanX） | Object   | 极低频 |

###### HfBeanX

| 字段名 | 数据类型 | 说明                                           |
| ------ | -------- | ---------------------------------------------- |
| Peek   | double   | 高频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 高频频带范围内的功率谱密度的面积               |
| Power2 | double   | 高频能量的占比                                 |
| Power3 | double   |                                                |

###### LfBeanX

| 字段名 | 数据类型 | 说明                                           |
| ------ | -------- | ---------------------------------------------- |
| Peek   | double   | 低频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 低频频带范围内的功率谱密度的面积               |
| Power2 | double   | 低频能量的占比                                 |
| Power3 | double   |                                                |

###### VlfBeanX

| 字段名 | 数据类型 | 说明                                             |
| ------ | -------- | ------------------------------------------------ |
| Peek   | double   | 极低频频带范围内的功率谱密度的极大值所在的频率值 |
| Power1 | double   | 极低频频带范围内的功率谱密度的面积               |
| Power2 | double   | 极低频能量的占比                                 |
| Power3 | double   |                                                  |

#### NonlinearBean

| 字段名 | 数据类型 | 说明               |
| ------ | -------- | ------------------ |
| IE     | double   |                    |
| SE     | double   | 样本熵             |
| BE     | double   | 基本尺度熵         |
| GE     | double   | 新的子序列产生概率 |

#### PoincareBean

| 字段名 | 数据类型 | 说明               |
| ------ | -------- | ------------------ |
| SD1    | double   | 散点图短半轴的长度 |
| SD2    | double   | 散点图长半轴的长度 |

#### TimeDomainBean

| 字段名 | 数据类型 | 说明                                                         |
| ------ | -------- | ------------------------------------------------------------ |
| MAX    | int      | 记录时间范围内RR间期的最大值                                 |
| MIN    | int      | 记录时间范围内RR间期的最小值                                 |
| MEAN   | double   | 记录时间范围内RR间期的平均值                                 |
| SDNN   | double   | 心跳间期标准偏差                                             |
| RMSSD  | double   | 相邻R-R间期的均方根                                          |
| SDSD   | double   | 相邻RR间期差值的标准差                                       |
| NN50   | int      | 相邻R-R间期差值大于50毫秒的个数                              |
| PNN50  | double   | 相邻R-R间期差值大于50毫秒的个数与总心跳间期的个数的比值的百分比 |

### farbeat.HCE.getLvc

RRI数据计算，获取LVD、LVV。

```java
HCE.getLvc(String accessToken, long deviceSourceTemplateId, LvcSource source, LvcCallBack callBack);
```

#### 参数说明

```java
accessToken：获取accessToken接口返回
deviceSourceTemplateId：根据统一标准数据中的配置信息ID从配置信息中获取，deviceSourceTemplateId相同的为一组数据
source：
    String pid; // 获取pid接口返回
    List<Integer> rriList; // rri集合
    long functDate; // 时间戳，单位：毫秒
callBack：计算RRI接口请求回调
```

#### 示例

```java
HCE.getLvc(accessToken, deviceSourceTemplateId, source, new LvcCallBack() {
@Override
public void onSuccess(int lvd, double lvv) {
        // TODO lvd：活力度；lvv：活力值
        }
        });
```

## LVC返回参数说明

### HrvData

| 字段名 | 数据类型 | 说明   |
| ------ | -------- | ------ |
| lvd    | int      | 活力度 |
| lvv    | double   | 活力值 |

### 附录A：数据源类别表

当数据源的类别的描述方式（schemeOfDataSourceType）为”11073”时所对应的数据源的类别（dataSourceType）的取值。

| 取值    | 数据源的类别           | 数据值的类型 | 备注                                                         |
| ------- | ---------------------- | ------------ | ------------------------------------------------------------ |
| 149568  | 脉搏波P-P间期          | 数值型       | 相邻两个脉搏波的波峰之间的时间间隔，多见于基于容积脉搏波测量心率的运动健康设备。 |
| 188736  | 体重                   | 数值型       |                        -                                      |
| 188740  | 身高                   | 数值型       |                       -                                       |
| 8454257 | 最大建议心率           | 数值型       | 出于运动安全的考虑而事先设定的心率值，用户在运动过程中不应使自己的心率达到或超过此最大建议心率值。最大建议心率通常由用户（或医生）手动输入，也可以通过相关公式计算得出。 |
| 8454263 | 能量消耗               | 数值型       | 一段时间内的累计能量消耗值。                                   |
| 8454270 | 年龄                   | 数值型       | 被测量的用户的年龄。                                           |
| 8454274 | 用户运动达标与健康状态 | 枚举型       |  - 取值为2200：用户的心率低于目标心率范围的下限；<br> - 取值为2203：用户的心率高于目标心率范围的上限；<br> - 取值为2207：用户速度高于目标速度下限；<br> - 取值为2211：用户能量消耗已超过目标能量消耗下限值；<br>其它取值：保留。 |
| 8454275 | 光学心率传感器状态     | 枚举型       |  - 取值为1：从设备报告传感器没有工作或者发生故障；<br> - 取值为2：从设备报告传感器没有被用户正确佩戴或者已经移位，因此当前测量不准确；<br> - 取值为3：从设备报告传感器未连接到用户；<br> - 取值为4：从设备报告存在环境光或者电现象的干扰；<br> - 取值为5：从设备报告目前正在进行信号分析，稍后才能提供测量值；<br> - 取值为6：从设备检测到一个有问题的脉搏波；<br> - 取值为7：从设备报告信号不稳定或者不合理；<br> - 取值为8：从设备报告存在一个信号差，可能会影响测量的准确性；<br> - 取值为9：从设备报告无法分析输入信号或者不足以产生一个有意义的结果；<br> - 取值为10：从设备报告已经确定在处理信号时检测到一些不规则性；<br>其它取值：保留。 |
| 8454276 | 基于PPG的动态心率      | 数值型       | 基于容积脉搏波(PPG)测得的运动时的瞬时心率值。                  |
| 8454288 | 距离                   | 数值型       |        -                                                      |
| 8454312 | 速度                   | 数值型       |        -                                                      |
| 8454320 | 坡度                   | 数值型       | 运动时的坡度，常用于跑步机。 |
| 8454324 | 动态心率               | 数值型       | 运动时测得的瞬时心率值，但具体测量方法不详。                   |
| 8456245 | 目标心率范围的下限     | 数值型       | 事先设定的目标心率范围的下限。在一段运动训练开始之前，根据训练需求，教练或用户自己会给用户设定一个心率范围作为目标，用户在这段运动中会争取将自己的心率维持在该范围中。 |
| 8456246 | 目标心率范围的上限     | 数值型       | 事先设定的目标心率范围的上限。在一段运动训练开始之前，根据训练需求，教练或用户自己会给用户设定一个心率范围作为目标，用户在这段运动中会争取将自己的心率维持在该范围中。 |
| 8456253 | 目标能量消耗下限       | 数值型       | 事先设定的能量消耗的下限值。在一段运动训练开始之前，根据训练需求，教练或用户自己会给用户设定一个能量消耗下限值作为目标，用户在这段运动中会争取将自己的累计能量消耗超过该下限值。 |
| 147240  | R-R间期(RRI)           | 数值型       | 心电波形中，相邻两个R波的波峰之间的时间间隔。                  |
| 8465664 | 累计步数                   | 数值型       |        -                                                      |

### 附录B：数据源单位表

当数据源的类别的描述方式（schemeOfDataSourceType）为”11073”时所对应的数据源的数值单位的取值。

| 取值   | 数值单位  | 备注                 |
| ------ | --------- | -------------------- |
| 262656 | -      | 无单位数值表达           |
| 264338 | 毫秒      |          -            |
| 264512 | 年        |       -               |
| 266112 | 焦耳      |       -               |
| 266176 | 瓦特      |      -                |
| 270496 | 卡路里    |      -                |
| 264864 | 心跳/分钟 | 适用于心率值         |
| 262880 | 度        | 适用于角度的表达           |
| 262688 | 百分数(%) |     -                 |
| 263422 | 米        |          -            |
| 263424 | 厘米      |        -              |
| 263875 | 公斤      |       -               |
| 264320 | 秒        |          -            |
| 264352 | 分钟      |        -              |
| 264960 | 米/秒     |         -             |
| 274083 | 公里/小时 |       -               |
| 273664 | 步        | 适用于走路或跑步           |
| 273760 | 步/分钟   | 适用于表示走路或跑步的速度 |