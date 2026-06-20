package com.lifeteam.farbeat.demo.page.calculate;

import android.content.Context;
import android.content.Intent;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.lifeteam.farbeat.FarBeat;
import com.lifeteam.farbeat.Open;
import com.lifeteam.farbeat.base.BaseActivity;
import com.lifeteam.farbeat.demo.R;
import com.lifeteam.farbeat.demo.app.NingApp;
import com.lifeteam.farbeat.demo.page.other.CaptureActivity;
import com.lifeteam.farbeat.engin.config.DeviceSdkSourceConfigure;
import com.lifeteam.farbeat.engin.config.HealthDataSourceConfigList;
import com.lifeteam.farbeat.listener.callback.HealthDataSourceConfigCallback;
import com.lifeteam.farbeat.listener.callback.OnSensorResponseListener;
import com.lifeteam.farbeat.util.data.DataChange;
import com.lifeteam.farbeat.util.data.DataUtil;
import com.lifeteam.farbeat.util.data.GSON;
import com.lifeteam.farbeat.util.log.LogUtil;
import com.lifeteam.farbeat.util.system.SystemUtil;
import com.lifeteam.farbeat.util.ui.ToastUtil;

import java.util.ArrayList;
import java.util.List;

public class SplashActivity extends BaseActivity {

    private TextView splashId;
    private TextView splashSecret;
    private TextView splashSn;
    private EditText splashSdkDevice;
    private final List<String> alreadyInitDevice = new ArrayList<>();

    public static void start(Context context) {
        Intent intent = new Intent(context, SplashActivity.class);
        context.startActivity(intent);
    }

    @Override
    public int getContentResId() {
        return R.layout.activity_splash;
    }

    @Override
    public void init() {
        findViewById(R.id.top_bar_back).setVisibility(View.VISIBLE);
        TextView topBarTitle = findViewById(R.id.top_bar_title);
        topBarTitle.setText("配置信息");
        splashId = findViewById(R.id.splash_id);
        splashSecret = findViewById(R.id.splash_secret);
        splashSn = findViewById(R.id.splash_sn);
        splashSdkDevice = findViewById(R.id.splash_sdk_device);

        setAppId();
        setAppSecret();
        setDpSdkSn();
        setSdkDeviceJson();
    }

    private void setAppId() {
        String appId = NingApp.appId;
        if (DataUtil.isEmpty(appId)) {
            appId = mSession.getAppId();
        }
        if (!DataUtil.isEmpty(appId)) {
            NingApp.appId = appId;
            splashId.setText(appId);
        }
    }

    private void setAppSecret() {
        String appSecret = NingApp.appSecret;
        if (DataUtil.isEmpty(appSecret)) {
            appSecret = mSession.getAppSecret();
        }
        if (!DataUtil.isEmpty(appSecret)) {
            NingApp.appSecret = appSecret;
            splashSecret.setText(appSecret);
        }
    }

    private void setDpSdkSn() {
        long dpSdkSn = NingApp.dpSdkSn;
        if (dpSdkSn == 0L) {
            dpSdkSn = mSession.getLongDpSdkSn();
        }
        if (dpSdkSn != 0L) {
            NingApp.dpSdkSn = dpSdkSn;
            splashSn.setText(String.valueOf(dpSdkSn));
        }
    }

    private void setSdkDeviceJson() {
        String sdkDeviceJson = mSession.getSdkDeviceJson();
        if (DataUtil.isEmpty(sdkDeviceJson)) {
            DeviceSdkSourceConfigure sdkDevice = FarBeat.getInstance().getSdkDevice();
            if (sdkDevice != null) {
                sdkDeviceJson = GSON.toJson(sdkDevice);
            }
        }
        if (!DataUtil.isEmpty(sdkDeviceJson)) {
            DeviceSdkSourceConfigure sdkDevice = GSON.toBean(sdkDeviceJson, DeviceSdkSourceConfigure.class);
            if (sdkDevice != null) {
                alreadyInitDevice.addAll(sdkDevice.getBasePackage());
                splashSdkDevice.setText(DataChange.formatJson(sdkDeviceJson));
            }
        }
    }

    public void onClick(@NonNull View v) {
        int id = v.getId();
        if (id == R.id.top_bar_back) {
            finish();
            return;
        }
        if (id == R.id.splash_id_scan) {
            CaptureActivity.startForResult(mActivity, CaptureActivity.APP_ID);
            return;
        }
        if (id == R.id.splash_secret_scan) {
            CaptureActivity.startForResult(mActivity, CaptureActivity.APP_SECRET);
            return;
        }
        if (id == R.id.splash_sn_scan) {
            CaptureActivity.startForResult(mActivity, CaptureActivity.APP_SN);
            return;
        }
        if (id == R.id.splash_id) {
            if (DataUtil.isEmpty(NingApp.appId)) {
                ToastUtil.show(mActivity, "请扫码获取 AppId！");
                return;
            }
            SystemUtil.copy2Clipboard(mActivity, splashId.getText());
            ToastUtil.show(mActivity, "AppId 已复制到剪贴板！");
            return;
        }
        if (id == R.id.splash_secret) {
            if (DataUtil.isEmpty(NingApp.appSecret)) {
                ToastUtil.show(mActivity, "请扫码获取 AppSecret！");
                return;
            }
            SystemUtil.copy2Clipboard(mActivity, splashSecret.getText());
            ToastUtil.show(mActivity, "AppSecret 已复制到剪贴板！");
            return;
        }
        if (id == R.id.splash_sn) {
            if (NingApp.dpSdkSn == 0L) {
                ToastUtil.show(mActivity, "请扫码获取 DpSdkSn！");
                return;
            }
            SystemUtil.copy2Clipboard(mActivity, splashSn.getText());
            ToastUtil.show(mActivity, "DpSdkSn 已复制到剪贴板！");
            return;
        }
        if (id == R.id.splash_device_title) {
            String deviceJson = splashSdkDevice.getText().toString().trim();
            if (!DataUtil.isEmpty(deviceJson)) {
                splashSdkDevice.setText(DataChange.formatJson(deviceJson));
            }
            return;
        }
        if (id == R.id.splash_config) {
            if (DataUtil.isEmpty(NingApp.appId)) {
                ToastUtil.show(mActivity, "请扫码获取 AppId！");
                return;
            }
            if (DataUtil.isEmpty(NingApp.appSecret)) {
                ToastUtil.show(mActivity, "请扫码获取 AppSecret！");
                return;
            }
            if (NingApp.dpSdkSn == 0L) {
                ToastUtil.show(mActivity, "请扫码获取 DpSdkSn！");
                return;
            }
            FarBeat farBeat = FarBeat.getInstance();
            // 初始化配置文件
            String deviceJson = splashSdkDevice.getText().toString().trim();
            if (!DataUtil.isEmpty(deviceJson)) {
                try {
                    DeviceSdkSourceConfigure sdkDevice = GSON.toBean(deviceJson, DeviceSdkSourceConfigure.class);
                    if (sdkDevice != null) {
                        List<String> pkGs = sdkDevice.getBasePackage();
                        if (pkGs == null || pkGs.size() <= 0) {
                            ToastUtil.show(mActivity, "至少包含一条设备适配SDK包名！");
                            return;
                        }
                        for (int i = 0; i < pkGs.size(); i++) {
                            String pkg = pkGs.get(i);
                            if (alreadyInitDevice.contains(pkg)) {
                                pkGs.remove(pkg);
                                i--;
                            } else {
                                alreadyInitDevice.add(pkg);
                            }
                        }
                        if (pkGs.size() > 0) {
                            mSession.setSdkDeviceJson(deviceJson);
                            sdkDevice.setBasePackage(pkGs);
                            farBeat.setSdkDevice(sdkDevice, new OnSensorResponseListener() {
                                @Override
                                public void onResponse(boolean isSuccess, String msg) {
                                    LogUtil.E("初始化SDK：" + isSuccess, msg);
                                }
                            });
                        }
                    }
                } catch (Exception e) {
                    ToastUtil.show(mActivity, "farbeat-sdk-device.json数据格式错误！");
                    return;
                }
            } else {
                ToastUtil.show(mActivity, "farbeat-sdk-device.json数据不存在！");
                return;
            }
            // 初始化SDK标识
            farBeat.setIdentification(NingApp.appId, NingApp.appSecret, NingApp.dpSdkSn);
            // 初始化配置信息
            Open.getHealthDataSourceConfigList(new HealthDataSourceConfigCallback<HealthDataSourceConfigList>() {
                @Override
                public void onResponse(int errorCode, HealthDataSourceConfigList sourceConfigList) {
                    LogUtil.E("配置文件", sourceConfigList);
                    if (sourceConfigList != null) {
                        ToastUtil.show(mActivity, "初始化配置信息成功！");
                        DeviceActivity.start(mActivity, sourceConfigList);
                    } else {
                        ToastUtil.show(mActivity, "错误码：" + errorCode);
                    }
                }
            });
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK && data != null) {
            String content = data.getStringExtra(CaptureActivity.CONTENT);
            switch (requestCode) {
                case CaptureActivity.APP_ID:
                    NingApp.appId = content;
                    mSession.setAppId(content);
                    setAppId();
                    break;
                case CaptureActivity.APP_SECRET:
                    NingApp.appSecret = content;
                    mSession.setAppSecret(content);
                    setAppSecret();
                    break;
                case CaptureActivity.APP_SN:
                    NingApp.dpSdkSn = DataChange.StringToLong(content);
                    mSession.setDpSdkSn(content);
                    setDpSdkSn();
                    break;
            }
        }
    }
}