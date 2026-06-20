# FarOS
计算SDK初始化
FarBeat.getInstance().init(context);
通用SDK初始化
FarBeatManager fbMgr = FarBeatManager.getInstance();
fbMgr.setDebugIsEnable(true);
fbMgr.init(mContext, SDKDevice.devices());
fbMgr.setGSdkSn(sdkSn);

public class SDKDevice {
    public static List<Integer> devices() {
        List<Integer> devices = new ArrayList<>();
        devices.add(SdkDevice.GT2);
        devices.add(SdkDevice.TKH01);
        return devices;
    }
}

1、获取accessToken

Manager.oauthToken(String appId, String appSecret, TokenCallBack callBack);
参数说明：

appId：
    控制台创建应用获取
appSecret：
    控制台创建应用获取
callBack：
    获取acceccToken接口请求回调
示例：

Manager.oauthToken(appId, appSecret, new TokenCallBack() {
    @Override
    public void onSuccess(String accessToken) {
        // TODO：获取成功
    }
});

2、获取Pid:

Manager.generatePid(String accessToken, String appId, PidCallBack callBack);
参数说明：

acceccToken：
    获取accessToken接口返回
appId：
    控制台创建应用获取
callBack：
    获取pid接口请求回调
示例：

Manager.generatePid(accessToken, appId, new PidCallBack() {
    @Override
    public void onSuccess(String pid) {
        // TODO：获取成功
    }
});
pid永久唯一有效，获取过无需再次获取

3、RRI数据计算，获取Hrv结果

HCE.getHrv(String accessToken, HrvSource source, HrvCallBack callBack);
参数说明：

accessToken：
    获取accessToken接口返回
source：
    String pid; // 获取pid接口返回
    List<Integer> rriList; // rri集合
    List<Long> rriTimeList; // 时间戳集合，单位：毫秒
callBack：
    计算RRI接口请求回调
注：rriList与timeList尺寸必须相同

示例：

HCE.getHrv(accessToken, source, new HrvCallBack() {
    @Override
    public void onSuccess(HrvBean.HrvData hrv) {
        // TODO：获取成功
    }
});

4、5分钟RRI数据计算，获取Hrv结果

HCE.getHrv5m(String accessToken, HrvSource source, HrvCallBack callBack);
参数说明：

accessToken：
    获取accessToken接口返回
source：
    String pid; // 获取pid接口返回
    List<Integer> rriList; // rri集合
    List<Long> rriTimeList; // 时间戳集合，单位：毫秒
callBack：
    计算RRI接口请求回调
注：rriList与timeList尺寸必须相同

示例：

HCE.getHrv5m(accessToken, source, new HrvCallBack() {
    @Override
    public void onSuccess(HrvBean.HrvData hrv) {
        // TODO：获取成功
    }
});

5、24小时RRI数据计算，获取Hrv结果

HCE.getHrv24h(String accessToken, HrvSource source, HrvCallBack callBack);
参数说明：

accessToken：
    获取accessToken接口返回
source：
    String pid; // 获取pid接口返回
    List<Integer> rriList; // rri集合
    List<Long> rriTimeList; // 时间戳集合，单位：毫秒
callBack：
    计算RRI接口请求回调
注：rriList与timeList尺寸必须相同

示例：

HCE.getHrv24h(accessToken, source, new HrvCallBack() {
    @Override
    public void onSuccess(HrvBean.HrvData hrv) {
        // TODO：获取成功
    }
});

6、RRI数据计算，获取LVD、LVV

HCE.getLvc(String accessToken, LvcSource source, LvcCallBack callBack);
参数说明：

accessToken：
    获取accessToken接口返回
source：
    String pid; // 获取pid接口返回
    List<Integer> rriList; // rri集合
    long functDate; // 时间戳，单位：毫秒
callBack：
    计算RRI接口请求回调
示例：

HCE.getLvc(accessToken, source, new LvcCallBack() {
    @Override
    public void onSuccess(int lvd, double lvv) {
        // TODO lvd：活力度；lvv：活力值
    }
});

7、获取数据源配置信息

Adaptor.getHealthDataSourceConfigList(HealthDataSourceConfigCallback<HealthDataSourceConfigList> callback);
参数说明：

callBack：
    获取数据源配置信息请求回调
示例：

Adaptor.getHealthDataSourceConfigList(new HealthDataSourceConfigCallback<HealthDataSourceConfigList>() {
    @Override
    public void onResponse(int errorCode, HealthDataSourceConfigList configList) {
        // TODO errorCode：错误码；configList：数据源配置信息
    }
});

8、启动设备扫描连接

Adaptor.getSourceDeviceConnected(FragmentActivity fragmentActivity, List<Integer> devices);
参数说明：

fragmentActivity：
    上下文，用于搜索界面弹窗
devices：
    同初始化中的，可搜索设备的集合
示例：

public class SDKDevice {
    public static List<Integer> devices() {
        List<Integer> devices = new ArrayList<>();
        devices.add(SdkDevice.GT2);
        devices.add(SdkDevice.TKH01);
        return devices;
    }
}
Adaptor.getSourceDeviceConnected(this, SDKDevice.devices());

9、设备扫描连接全局监听

Adaptor.setOnConnectDeviceListener(OnConnectDeviceListener onConnectDeviceListener);
参数说明：

onConnectDeviceListener：
    设备连接结果监听
示例：

Adaptor.setOnConnectDeviceListener(new OnConnectDeviceListener() {
    @Override
    public void onStartConnect(ScanDevice device) {
        // TODO 启动连接（可选重写）
    }
    @Override
    public void onConnecting(String name, String mac) {
        // TODO 正在连接中（可选重写）
    }
    @Override
    public void onConnected(String name, String mac) {
        // TODO 连接成功
    }
    @Override
    public void onDisconnected(String name, String mac) {
        // TODO 断开连接
    }
    @Override
    public void onConnectFailed(String name, String mac) {
        // TODO 连接失败（可选重写）
    }
    @Override
    public void onMeasuring(String name, String mac) {
        // TODO 正在测量中（可选重写）
    }
});
为适配多设备的连接规则，部分方法可选择性重写

10、移除设备连接

Adaptor.removeSourceDevice(int sdkDevice, SensorResponseCallback callback);
参数说明：

sdkDevice：
    设备类型标识
callback：
    请求结果返回
示例（以华为手表为例）：

Adaptor.removeSourceDevice(SdkDevice.GT2, new SensorResponseCallback() {
    @Override
    public void onResponse(boolean isSuccess, String msg) {
        // TODO isSuccess：请求结果是否成功；msg：结果返回的说明，可能为空
    }
});

11、开启设备实时数据采集

Adaptor.startRealTimeMeasure(int sdkDevice, int duration);
参数说明：

sdkDevice：
    要移除的设备类型标识
duration：
    采集时长，单位：秒
示例（以华为手表，10分钟采集为例）：

Adaptor.getRealTimeDataProvider().startRealTimeMeasure(SdkDevice.GT2, 10 * 60);

12、开启设备实时数据采集结果全局监听

Adaptor.setOnStartRealTimeMeasureListener(SensorResponseCallback callback);
参数说明：

callback：
    设备实时数据采集结果全局监听结果
示例：

Adaptor.setOnStartRealTimeMeasureListener(new SensorResponseCallback() {
    @Override
    public void onResponse(boolean isSuccess, String msg) {
        // TODO isSuccess：请求结果是否成功；msg：结果返回的说明，可能为空
    }
});

13、实时数据采集全局监听

Adaptor.setOnHealthDataListener(OnHealthDataListener<HealthDataReport> onHealthDataListener);
参数说明：

onHealthDataListener：
    实时数据采集的结果返回
示例：

Adaptor.setOnHealthDataListener(new OnHealthDataListener<HealthDataReport>() {
    @Override
    public void onHealthData(int errorCode, HealthDataReport report) {
    }
});

14、停止实时数据采集

Adaptor.stopRealTimeMeasure(int sdkDevice, SensorResponseCallback callback);
参数说明：

sdkDevice：
    设备类型标识
callback：
    请求结果返回
示例（以华为手表为例）：

Adaptor.stopRealTimeMeasure(SdkDevice.GT2, new SensorResponseCallback() {
    @Override
    public void onResponse(boolean isSuccess, String msg) {
        // TODO isSuccess：请求结果是否成功；msg：结果返回的说明，可能为空
    }
});
