package com.lifeteam.farbeat.demo.page.interoperability;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.lifeteam.farbeat.FarBeat;
import com.lifeteam.farbeat.Open;
import com.lifeteam.farbeat.base.BaseActivity;
import com.lifeteam.farbeat.demo.R;
import com.lifeteam.farbeat.demo.adapter.InterListAdapter;
import com.lifeteam.farbeat.demo.adapter.InterLogcatAdapter;
import com.lifeteam.farbeat.demo.app.NingApp;
import com.lifeteam.farbeat.demo.bean.InfoBean;
import com.lifeteam.farbeat.demo.bean.InterExcel;
import com.lifeteam.farbeat.demo.dialog.InputDialog;
import com.lifeteam.farbeat.demo.listener.OnInfoListener;
import com.lifeteam.farbeat.demo.util.ExcelUtil;
import com.lifeteam.farbeat.demo.util.What;
import com.lifeteam.farbeat.dialog.AskDialog;
import com.lifeteam.farbeat.engin.FarBeatDevice;
import com.lifeteam.farbeat.engin.config.DeviceSdkSourceConfigure;
import com.lifeteam.farbeat.engin.config.HealthDataSourceConfigList;
import com.lifeteam.farbeat.engin.data.HealthDataReport;
import com.lifeteam.farbeat.listener.callback.HealthDataSourceConfigCallback;
import com.lifeteam.farbeat.listener.callback.OnSensorResponseListener;
import com.lifeteam.farbeat.listener.common.OnPositiveListener;
import com.lifeteam.farbeat.listener.device.OnConnectDeviceListener;
import com.lifeteam.farbeat.listener.device.OnHealthDataListener;
import com.lifeteam.farbeat.util.constant.Config;
import com.lifeteam.farbeat.util.constant.ErrorCode;
import com.lifeteam.farbeat.util.data.DataChange;
import com.lifeteam.farbeat.util.data.DataUtil;
import com.lifeteam.farbeat.util.data.GSON;
import com.lifeteam.farbeat.util.data.Res;
import com.lifeteam.farbeat.util.file.FileUtil;
import com.lifeteam.farbeat.util.log.LogUtil;
import com.lifeteam.farbeat.util.recycler.DividerItemDecoration;
import com.lifeteam.farbeat.util.recycler.refresh.RecyclerUtil;
import com.lifeteam.farbeat.util.system.SystemUtil;
import com.lifeteam.farbeat.util.time.TimeFormat;
import com.lifeteam.farbeat.util.ui.ToastUtil;
import com.zlylib.fileselectorlib.utils.FileUtils;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class InterActivity extends BaseActivity implements OnHealthDataListener<HealthDataReport> {

    private RecyclerView interListRecycler;
    private RecyclerView interLogcatRecycler;
    private TextView interTime;
    private Button interStart;
    public static final String sheetName = "实时数据交换测试结果报告表";
    private final String[] header = {"序号", "测试项名称", "", "结果"};
    private final String[] titles = {
            "初始化设备适配SDK",
            "获取数据源配置信息",
            "注册设备状态监听",
            "启动设备扫描",
            "设备蓝牙连接",
            "注册设备实时数据交换监听",
            "启动设备实时数据交换（默认5分钟）",
            "停止设备实时数据交换",
            "移除设备连接"
    };
    public static final String filePath = FarBeat.getInstance().getExternalFilesPath("excel");
    public static final String suffix = ".xls";
    private final String fileName = "Report of RT data exchange test results";
    public static final String PASS = "通过";
    public static final String FAIL = "失败";
    private final String SUCCESS = "成功！";
    private final String FAILURE = "失败，中止测试！";;
    private FarBeatDevice currentFarBeatDevice;
    private InterListAdapter interListAdapter;
    private InterLogcatAdapter interLogcatAdapter;
    private final List<InterExcel> excelList = new ArrayList<>();
    private int currHealthDataState = 0;
    private PeterTimeCountRefresh peterTimeCountRefresh;
    private boolean isSendAndShowHealthData = true;
    private boolean isInterDisConnectDevice = false;
    private DeviceSdkSourceConfigure sdkDevice;
    private String exportExcelPath;

    public static void start(Context context, DeviceSdkSourceConfigure sdkDevice) {
        Intent intent = new Intent(context, InterActivity.class);
        intent.putExtra(Config.PARAM, sdkDevice);
        context.startActivity(intent);
    }

    @Override
    public int getContentResId() {
        return R.layout.activity_inter;
    }

    @SuppressLint("SetTextI18n")
    @Override
    public void init() {
        TextView topBarTitle = findViewById(R.id.top_bar_title);
        TextView interTitle = findViewById(R.id.inter_title);
        interListRecycler = findViewById(R.id.inter_list_recycler);
        interLogcatRecycler = findViewById(R.id.inter_logcat_recycler);
        interTime = findViewById(R.id.inter_time);
        interStart = findViewById(R.id.inter_start);

        topBarTitle.setText(R.string.app_interoperability);
        interTitle.setText("测试项（共 " + titles.length + " 项）");

        sdkDevice = (DeviceSdkSourceConfigure) getIntent().getSerializableExtra(Config.PARAM);

        RecyclerUtil.setVerticalLinearLayout(interListRecycler);
        interListRecycler.addItemDecoration(new DividerItemDecoration(mActivity));
        interListAdapter = new InterListAdapter(mActivity, null);
        interListRecycler.setAdapter(interListAdapter);

        RecyclerUtil.setVerticalLinearLayout(interLogcatRecycler);
        interLogcatAdapter = new InterLogcatAdapter(mActivity, null);
        interLogcatRecycler.setAdapter(interLogcatAdapter);

        setInterList(Config._1, Res.string(mActivity, R.string.def_text));
    }

    private void setInterList(int position, String result) {
        setInterList(position, result, null);
    }

    private void setInterList(int position, String result, Object obj) {
        if (position != Config._1 && position < excelList.size()) {
            InterExcel excel = excelList.get(position);
            if (excel != null) {
                excelList.remove(excel);
                excel.setResult(result);
                excel.setTest(true);
                excelList.add(position, excel);
                scrollListToPosition(position);
                if (FAIL.equals(result) && obj instanceof String) {
                    AskDialog askDialog = new AskDialog(mActivity, (String) obj, "取消", "关闭页面！");
                    askDialog.setOnPositiveListener(new OnPositiveListener() {
                        @Override
                        public void onPositive() {
                            finish();
                        }
                    });
                }
            }
        } else {
            for (int i = 0; i < titles.length; i++) {
                InterExcel excel = new InterExcel();
                excel.setNumber(i + 1);
                excel.setTitle(titles[i]);
                excel.setResult(result);
                excel.setTest(false);
                excelList.add(excel);
            }
        }
        interListAdapter.setData(excelList);
    }

    public void onClick(@NonNull View v) {
        int id = v.getId();
        if (id == R.id.inter_start) {
            if (SystemUtil.bluetoothIsEnabled()) {
                interStart.setVisibility(View.GONE);
                sendHandleMessage(What.LOG_0, "正在准备开始测试，请稍后...");
                ToastUtil.show(mActivity, interStart.getText().toString());
            } else {
                SystemUtil.openBluetooth();
            }
        }
    }

    private final Handler handler = new Handler(Looper.getMainLooper()) {

        private InfoBean currentInfo;

        @Override
        public void handleMessage(@NonNull Message msg) {
            switch (msg.what) {
                case What.LOG_0:
                    setLogcatSingle(msg.obj);
                    sendHandleMessage(What.LOG_1, "开始执行：1、" + titles[0]);
                    break;
                case What.LOG_1:
                    setLogcatSingle(msg.obj);
                    if (sdkDevice != null) {
                        FarBeat.getInstance().setSdkDevice(sdkDevice, new OnSensorResponseListener() {
                            @Override
                            public void onResponse(boolean isSuccess, String msg) {
                                LogUtil.E("初始化SDK：" + isSuccess, msg);
                                if (isSuccess) {
                                    sendHandleMessage(What.LOG_2, "【1、" + titles[0] + "】" + SUCCESS);
                                } else {
                                    sendHandleMessage(What.LOG_FILE_2, "【1、" + titles[0] + "】" + FAILURE);
                                }
                            }
                        });
                    } else {
                        sendHandleMessage(What.LOG_2, "【1、" + titles[0] + "】" + SUCCESS);
                    }
                    break;
                case What.LOG_2:
                    setLogcatSingle(msg.obj);
                    setInterList(0, PASS);
                    sendHandleMessage(What.LOG_3, "开始执行：2、" + titles[1]);
                    break;
                case What.LOG_FILE_2:
                    setLogcatSingle(msg.obj);
                    setInterList(0, FAIL, msg.obj);
                    break;
                case What.LOG_3:
                    setLogcatSingle(msg.obj);
                    Open.getHealthDataSourceConfigList(new HealthDataSourceConfigCallback<HealthDataSourceConfigList>() {
                        @Override
                        public void onResponse(int errorCode, HealthDataSourceConfigList sourceConfigList) {
                            if (errorCode == ErrorCode.DEVICE_DATA_SUCCESS) {
                                sendHandleMessage(What.LOG_4, "【2、" + titles[1] + "】" + SUCCESS);
                            } else {
                                sendHandleMessage(What.LOG_5, "【2、" + titles[1] + "】" + FAILURE);
                            }
                        }
                    });
                    break;
                case What.LOG_4:
                    setLogcatSingle(msg.obj);
                    setInterList(1, PASS);
                    sendHandleMessage(What.LOG_6, "开始执行：3、" + titles[2]);
                    break;
                case What.LOG_5:
                    setLogcatSingle(msg.obj);
                    setInterList(1, FAIL, msg.obj);
                    break;
                case What.LOG_6:
                    setLogcatSingle(msg.obj);
                    sendHandleMessage(What.LOG_7, "【3、" + titles[2] + "】" + SUCCESS);
                    Open.setOnConnectDeviceListener(onConnectDeviceListener);
                    break;
                case What.LOG_7:
                    setLogcatSingle(msg.obj);
                    setInterList(2, PASS);
                    sendHandleMessage(What.LOG_8, "开始执行：4、" + titles[3]);
                    break;
                case What.LOG_8:
                    setLogcatSingle(msg.obj);
                    if (SystemUtil.bluetoothIsEnabled()) {
                        sendHandleMessage(What.LOG_9, "【4、" + titles[3] + "】" + SUCCESS);
                        Open.getSourceDeviceConnected(mActivity);
                    } else {
                        sendHandleMessage(What.LOG_10, "【4、" + titles[3] + "】" + FAILURE);
                    }
                    break;
                case What.LOG_9:
                    setLogcatSingle(msg.obj);
                    setInterList(3, PASS);
                    sendHandleMessage(What.LOG_11, "开始执行：5、" + titles[4]);
                    break;
                case What.LOG_10:
                    setLogcatSingle(msg.obj);
                    setInterList(3, FAIL, msg.obj);
                    break;
                case What.LOG_11:
                    setLogcatSingle(msg.obj);
                    break;
                case What.LOG_12:
                    setLogcatSingle(msg.obj);
                    break;
                case What.LOG_13:
                    setLogcatSingle(msg.obj);
                    sendHandleMessage(What.LOG_16, "【5、" + titles[4] + "】" + SUCCESS);
                    break;
                case What.LOG_14:
                    setLogcatSingle(msg.obj);
                    sendHandleMessage(What.LOG_17, "【5、" + titles[4] + "】" + FAILURE);
                    break;
                case What.LOG_15:
                    setLogcatSingle(msg.obj);
                    break;
                case What.LOG_16:
                    setLogcatSingle(msg.obj);
                    setInterList(4, PASS);
                    sendHandleMessage(What.LOG_18, "开始执行：6、" + titles[5]);
                    break;
                case What.LOG_17:
                    setLogcatSingle(msg.obj);
                    setInterList(4, FAIL, msg.obj);
                    break;
                case What.LOG_18:
                    setLogcatSingle(msg.obj);
                    sendHandleMessage(What.LOG_19, "【6、" + titles[5] + "】" + SUCCESS);
                    setOnHealthDataListener();
                    break;
                case What.LOG_19:
                    setLogcatSingle(msg.obj);
                    setInterList(5, PASS);
                    sendHandleMessage(What.LOG_20, "开始执行：7、" + titles[6]);
                    break;
                case What.LOG_20:
                    setLogcatSingle(msg.obj);
                    if (currentFarBeatDevice != null) {
                        Open.startRealTimeMeasure(currentFarBeatDevice, Config.MINUTE_5);
                    } else {
                        sendHandleMessage(What.LOG_22, "【7、" + titles[6] + "】" + FAILURE);
                    }
                    break;
                case What.LOG_21:
                    setLogcatSingle(msg.obj);
                    setInterList(6, PASS);
                    sendHandleMessage(What.LOG_24, "开始5分钟实时数据采集...");
                    break;
                case What.LOG_22:
                    setLogcatSingle(msg.obj);
                    setInterList(6, FAIL, msg.obj);
                    break;
                case What.LOG_23:
                    if (isSendAndShowHealthData) {
                        setLogcatList(msg.obj);
                    }
                    break;
                case What.LOG_24:
                    setLogcatSingle(msg.obj);
                    currHealthDataState = 2;
                    peterTimeCountRefresh = new PeterTimeCountRefresh(Config.DAILY_5_MINUTE);
                    peterTimeCountRefresh.start();
                    interTime.setVisibility(View.VISIBLE);
                    setInterList(6, PASS); // 多一个控件后，重新定位一下
                    break;
                case What.LOG_25:
                    setLogcatSingle(msg.obj);
                    sendHandleMessage(What.LOG_26, "开始执行：8、" + titles[7]);
                    interTime.setVisibility(View.INVISIBLE);
                    break;
                case What.LOG_26:
                    setLogcatSingle(msg.obj);
                    if (currentFarBeatDevice != null) {
                        Open.stopRealTimeMeasure(currentFarBeatDevice, new OnSensorResponseListener() {
                            @Override
                            public void onResponse(boolean isSuccess, String msg) {
                                if (isSuccess) {
                                    sendHandleMessage(What.LOG_27, "【8、" + titles[7] + "】" + SUCCESS);
                                } else {
                                    sendHandleMessage(What.LOG_28, "【8、" + titles[7] + "】" + FAILURE);
                                }
                            }
                        });
                    } else {
                        sendHandleMessage(What.LOG_28, "【8、" + titles[7] + "】" + FAILURE);
                    }
                    break;
                case What.LOG_27:
                    isInterDisConnectDevice = true;
                    setLogcatSingle(msg.obj);
                    setInterList(7, PASS);
                    sendHandleMessage(What.LOG_29, "开始执行：9、" + titles[8]);
                    break;
                case What.LOG_28:
                    setLogcatSingle(msg.obj);
                    setInterList(7, FAIL, msg.obj);
                    break;
                case What.LOG_29:
                    setLogcatSingle(msg.obj);
                    if (currentFarBeatDevice != null) {
                        Open.removeSourceDevice(currentFarBeatDevice, new OnSensorResponseListener() {
                            @Override
                            public void onResponse(boolean isSuccess, String msg) {
                                if (isSuccess) {
                                    sendHandleMessage(What.LOG_30, "【9、" + titles[8] + "】" + SUCCESS);
                                } else {
                                    sendHandleMessage(What.LOG_31, "【9、" + titles[8] + "】" + FAILURE);
                                }
                            }
                        });
                    } else {
                        sendHandleMessage(What.LOG_31, "【9、" + titles[8] + "】" + FAILURE);
                    }
                    break;
                case What.LOG_30:
                    setLogcatSingle(msg.obj);
                    break;
                case What.LOG_31:
                    setLogcatSingle(msg.obj);
                    setInterList(8, FAIL, msg.obj);
                    break;
                case What.LOG_32:
                    setLogcatSingle(msg.obj);
                    setInterList(8, PASS);
                    sendHandleMessage(What.LOG_33, "本次互操作设备测试全部执行完毕！");
                    break;
                case What.LOG_33:
                    setLogcatSingle(msg.obj);
                    FileUtils.delete(filePath);
                    InputDialog inputDialog = new InputDialog(mActivity, mSession, InputDialog.TEST_CONCLUSION);
                    inputDialog.setOnInfoListener(new OnInfoListener() {
                        @Override
                        public void onInfo(InfoBean info) {
                            currentInfo = info;
                            sendHandleMessage(What.LOG_34, "正在生成" + sheetName + "：" + fileName + suffix);
                        }
                    });
                    break;
                case What.LOG_34:
                    setLogcatSingle(msg.obj);
                    if (currentInfo != null) {
                        FileUtil.createDir(filePath);
                        exportExcelPath = filePath + File.separator + fileName + suffix;
                        ExcelUtil.initExcel(exportExcelPath, sheetName, header, currentInfo);
                        ExcelUtil.writeObjListToExcel(excelList, exportExcelPath, currentInfo.getConclusion());
                        sendHandleMessage(What.LOG_35, sheetName + "【" + fileName + suffix + "】生成完毕！");
                    }
                    break;
                case What.LOG_35:
                    setLogcatSingle(msg.obj);
                    scrollListToPosition(interListAdapter.getItemCount() - 1);
                    sendHandleMessage(What.LOG_36, null);
                    break;
                case What.LOG_36:
                    ResultActivity.start(mActivity, exportExcelPath);
                    finish();
                    break;
            }
        }
    };

    class PeterTimeCountRefresh extends CountDownTimer {

        public PeterTimeCountRefresh(long millisInFuture) {
            super(millisInFuture, 1000);
        }

        @Override
        public void onTick(long millisUntilFinished) {
            interTime.setText(TimeFormat.getTimerTime(millisUntilFinished));
        }

        @Override
        public void onFinish() {
            isSendAndShowHealthData = false;
            sendHandleMessage(What.LOG_25, "5分钟实时数据采集完毕！");
        }
    }

    public void interruptCountDownTimer() {
        isSendAndShowHealthData = false;
        if (peterTimeCountRefresh != null) {
            peterTimeCountRefresh.cancel();
            peterTimeCountRefresh = null;
        }
        if (currentFarBeatDevice != null) {
            Open.stopRealTimeMeasure(currentFarBeatDevice, null);
            Open.removeSourceDevice(currentFarBeatDevice, new OnSensorResponseListener() {
                @Override
                public void onResponse(boolean isSuccess, String msg) {
                    String mac = currentFarBeatDevice.getMac();
                    if (!DataUtil.isEmpty(mac)) {
                        SystemUtil.unpairDevice(mac);
                    }
                }
            });
        }
    }

    @Override
    protected void onDestroy() {
        interruptCountDownTimer();
        super.onDestroy();
    }

    private final OnConnectDeviceListener onConnectDeviceListener = new OnConnectDeviceListener() {
        @Override
        public void onStartConnect(FarBeatDevice device) {
            if (!isInterDisConnectDevice) {
                String name = device != null ? device.getName() : "";
                sendHandleMessage(What.LOG_12, "【" + name + "】正在连接中...");
            }
        }

        @Override
        public void onConnected(FarBeatDevice device) {
            if (!isInterDisConnectDevice) {
                if (device != null) {
                    currentFarBeatDevice = device;
                    String name = device.getName();
                    NingApp.currentDeviceName = name;
                    NingApp.currentDeviceMac = device.getMac();
                    sendHandleMessage(What.LOG_13, "【" + name + "】已连接！");
                } else {
                    sendHandleMessage(What.LOG_17, "【5、" + titles[4] + "】" + FAILURE);
                }
            }
        }

        @Override
        public void onDisconnected(FarBeatDevice device) {
            String name = device != null ? device.getName() : "";
            if (!isInterDisConnectDevice) {
                sendHandleMessage(What.LOG_14, "【" + name + "】已断开！");
            } else {
                sendHandleMessage(What.LOG_32, "【" + name + "】已断开！");
            }
        }

        @Override
        public void onConnectFailed(FarBeatDevice device) {
            if (!isInterDisConnectDevice) {
                String name = device != null ? device.getName() : "";
                sendHandleMessage(What.LOG_15, "【" + name + "】连接失败！");
            }
        }
    };

    private void setOnHealthDataListener() {
        Open.setOnHealthDataListener(this);
    }

    @Override
    public void onHealthData(int errorCode, HealthDataReport report) {
        if (currHealthDataState == 0) {
            currHealthDataState = 1;
            sendHandleMessage(What.LOG_21, "【7、" + titles[6] + "】" + SUCCESS);
        }
        if (currHealthDataState == 2 && isSendAndShowHealthData) {
            sendHandleMessage(What.LOG_23, DataChange.formatJson2List(GSON.toJson(report)));
        }
    }

    private void sendHandleMessage(int what, Object obj) {
        Message msg = new Message();
        msg.what = what;
        msg.obj = obj;
        handler.sendMessageDelayed(msg, Config.DAILY_500_MILLI);
    }

    private void setLogcatSingle(Object obj) {
        interLogcatAdapter.addItem((String) obj);
        scrollLogcatToPosition();
    }

    private void setLogcatList(Object obj) {
        interLogcatAdapter.addData(DataUtil.castList(obj, String.class));
        scrollLogcatToPosition();
    }

    private void scrollListToPosition(int position) {
        interListRecycler.scrollToPosition(position);
    }

    private void scrollLogcatToPosition() {
        interLogcatRecycler.scrollToPosition(interLogcatAdapter.getItemCount() - 1);
    }

    @Override
    public void onBackPressed() {
        AskDialog askDialog = new AskDialog(mActivity, "确定要退出当前页面吗？\n退出后将终止当前测试！");
        askDialog.setOnPositiveListener(new OnPositiveListener() {
            @Override
            public void onPositive() {
                finish();
            }
        });
    }
}