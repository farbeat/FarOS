package com.lifeteam.farbeat.demo.page.calculate;

import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.KeyEvent;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.lifeteam.farbeat.Open;
import com.lifeteam.farbeat.base.BaseActivity;
import com.lifeteam.farbeat.demo.R;
import com.lifeteam.farbeat.demo.adapter.DeviceAdapter;
import com.lifeteam.farbeat.engin.FarBeatDevice;
import com.lifeteam.farbeat.engin.config.HealthDataSourceConfigList;
import com.lifeteam.farbeat.listener.device.OnConnectDeviceListener;
import com.lifeteam.farbeat.util.constant.Config;
import com.lifeteam.farbeat.util.data.DataUtil;
import com.lifeteam.farbeat.util.ui.ToastUtil;

import org.jetbrains.annotations.NotNull;

import java.util.List;

public class DeviceActivity extends BaseActivity {

    private RecyclerView deviceRecycler;
    private DeviceAdapter deviceAdapter;
    private final int onStartConnect = 90001;
    private final int onConnected = 90002;
    private final int onDisconnected = 90003;
    private final int onConnectFailed = 90004;

    public static void start(Context context, HealthDataSourceConfigList sourceConfigList) {
        Intent intent = new Intent(context, DeviceActivity.class);
        intent.putExtra(Config.PARAM, sourceConfigList);
        context.startActivity(intent);
    }

    @Override
    public int getContentResId() {
        return R.layout.activity_device;
    }

    @Override
    public void init() {
        TextView topBarTitle = findViewById(R.id.top_bar_title);
        TextView topBarOther = findViewById(R.id.top_bar_other);
        topBarTitle.setText("设备列表");
        topBarOther.setText("添加设备");
        topBarOther.setVisibility(View.VISIBLE);
        deviceRecycler = findViewById(R.id.device_recycler);

        HealthDataSourceConfigList sourceConfigList = (HealthDataSourceConfigList) getIntent().getSerializableExtra(Config.PARAM);

        deviceRecycler.setLayoutManager(new LinearLayoutManager(mActivity));
        deviceAdapter = new DeviceAdapter(mActivity, null);
        deviceAdapter.setSourceConfigList(sourceConfigList);
        deviceRecycler.setAdapter(deviceAdapter);

        Open.setOnConnectDeviceListener(onConnectDeviceListener);
    }

    public void onClick(@NotNull View v) {
        int id = v.getId();
        if (id == R.id.top_bar_other) {
            Open.getSourceDeviceConnected(mActivity);
        }
    }

    private final OnConnectDeviceListener onConnectDeviceListener = new OnConnectDeviceListener() {
        @Override
        public void onStartConnect(FarBeatDevice device) {
            sendMessage(onStartConnect, device);
        }

        @Override
        public void onConnected(FarBeatDevice device) {
            sendMessage(onConnected, device);
        }

        @Override
        public void onDisconnected(FarBeatDevice device) {
            sendMessage(onDisconnected, device);
        }

        @Override
        public void onConnectFailed(FarBeatDevice device) {
            sendMessage(onConnectFailed, device);
        }
    };

    private void sendMessage(int what, FarBeatDevice device) {
        Message msg = new Message();
        msg.what = what;
        msg.obj = device;
        handler.sendMessage(msg);
    }

    private final Handler handler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(@NonNull Message msg) {
            switch(msg.what) {
                case onStartConnect:
                    FarBeatDevice startConnectDevice = (FarBeatDevice) msg.obj;
                    if (startConnectDevice != null) {
                        ToastUtil.showProgressDialog(mActivity, "设备【" + startConnectDevice.getName() + "】开始连接...");
                    }
                    break;
                case onConnected:
                    FarBeatDevice connectedDevice = (FarBeatDevice) msg.obj;
                    if (connectedDevice != null) {
                        String connectedMac = connectedDevice.getMac();
                        if (!DataUtil.isEmpty(connectedMac)) {
                            boolean isAdd = true;
                            List<FarBeatDevice> data = deviceAdapter.getData();
                            for (FarBeatDevice device : data){
                                if (connectedMac.equals(device.getMac())) {
                                    isAdd = false;
                                }
                            }
                            if (isAdd) {
                                deviceAdapter.addItem(connectedDevice);
                                deviceRecycler.setVisibility(View.VISIBLE);
                                ToastUtil.show(mActivity, "设备【" + connectedDevice.getName() + "】已连接！");
                            }
                        }
                    }
                    ToastUtil.dismissProgressDialog();
                    break;
                case onDisconnected:
                    FarBeatDevice disConnectDevice = (FarBeatDevice) msg.obj;
                    if (disConnectDevice != null) {
                        String disconnectedMac = disConnectDevice.getMac();
                        if (!DataUtil.isEmpty(disconnectedMac)) {
                            List<FarBeatDevice> data = deviceAdapter.getData();
                            for (FarBeatDevice device : data){
                                if (disconnectedMac.equals(device.getMac())) {
                                    deviceAdapter.removeItem(device);
                                    ToastUtil.show(mActivity, "设备【" + disConnectDevice.getName() + "】已断开！");
                                }
                            }
                        }
                    }
                    if (deviceAdapter.getData().size() == 0) {
                        deviceRecycler.setVisibility(View.GONE);
                    }
                    ToastUtil.dismissProgressDialog();
                    break;
                case onConnectFailed:
                    FarBeatDevice connectFailedDevice = (FarBeatDevice) msg.obj;
                    if (connectFailedDevice != null) {
                        ToastUtil.show(mActivity, "设备【" + connectFailedDevice.getName() + "】连接失败！");
                    }
                    ToastUtil.dismissProgressDialog();
                    break;
            }
        }
    };

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_DOWN) {
            Intent intent = new Intent(Intent.ACTION_MAIN);
            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            intent.addCategory(Intent.CATEGORY_HOME);
            startActivity(intent);
            return true;
        } else return keyCode == KeyEvent.KEYCODE_DEL && event.getAction() == KeyEvent.ACTION_DOWN;
    }
}