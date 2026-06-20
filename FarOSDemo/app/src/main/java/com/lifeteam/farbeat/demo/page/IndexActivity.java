package com.lifeteam.farbeat.demo.page;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.view.KeyEvent;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.lifeteam.farbeat.FarBeat;
import com.lifeteam.farbeat.base.BaseActivity;
import com.lifeteam.farbeat.demo.R;
import com.lifeteam.farbeat.demo.app.NingApp;
import com.lifeteam.farbeat.demo.page.calculate.SplashActivity;
import com.lifeteam.farbeat.demo.page.interoperability.InfoActivity;
import com.lifeteam.farbeat.demo.page.interoperability.InterActivity;
import com.lifeteam.farbeat.demo.page.other.CaptureActivity;
import com.lifeteam.farbeat.demo.page.qrs.QrsActivity;
import com.lifeteam.farbeat.demo.util.Permission;
import com.lifeteam.farbeat.dialog.AskDialog;
import com.lifeteam.farbeat.engin.config.DeviceSdkSourceConfigure;
import com.lifeteam.farbeat.listener.callback.DeviceSdkSourceConfigureCallback;
import com.lifeteam.farbeat.listener.common.OnPositiveListener;
import com.lifeteam.farbeat.util.constant.Config;
import com.lifeteam.farbeat.util.constant.ErrorCode;
import com.lifeteam.farbeat.util.data.DataChange;
import com.lifeteam.farbeat.util.data.DataUtil;
import com.lifeteam.farbeat.util.data.GSON;
import com.lifeteam.farbeat.util.system.SystemUtil;
import com.lifeteam.farbeat.util.ui.ToastUtil;
import com.zlylib.fileselectorlib.utils.FileUtils;

import java.util.ArrayList;
import java.util.List;

import pub.devrel.easypermissions.AfterPermissionGranted;
import pub.devrel.easypermissions.EasyPermissions;
import pub.devrel.easypermissions.helper.PermissionHelper;

public class IndexActivity extends BaseActivity implements EasyPermissions.PermissionCallbacks {

    private long firstTime = 0;

    @Override
    public int getContentResId() {
        return R.layout.activity_index;
    }

    @Override
    public void init() {
        methodRequires2Permission();
    }

    public void onClick(@NonNull View v) {
        if (SystemUtil.isNetworkAvailable(mActivity)) {
            int id = v.getId();
            if (id == R.id.index_interoperability) {
                String content = "请前往管理平台扫码获取SDK序列号【DpSdkSn】！";
                String cancel = "取消";
                String confirm = "立即扫码";
                AskDialog askDialog = new AskDialog(mActivity, content, cancel, confirm);
                askDialog.setOnPositiveListener(new OnPositiveListener() {
                    @Override
                    public void onPositive() {
                        CaptureActivity.startForResult(mActivity, CaptureActivity.APP_SN);
                    }
                });
                return;
            }
            if (id == R.id.index_calculate) {
                SplashActivity.start(mActivity);
                return;
            }
            if (id == R.id.index_qrs) {
                QrsActivity.start(mActivity);
                return;
            }
            if (id == R.id.index_clear) {
                new AskDialog(mActivity, "清理本地缓存信息吗？").setOnPositiveListener(new OnPositiveListener() {
                    @Override
                    public void onPositive() {
                        NingApp.appId = "";
                        NingApp.appSecret = "";
                        NingApp.dpSdkSn = 0L;
                        mSession.removeAll();
                        FileUtils.delete(InterActivity.filePath);
                        ToastUtil.show(mActivity, "本地缓存信息清理成功！");
                    }
                });
            }
        } else {
            ToastUtil.show(mActivity, "网络不可用，请检查网络！");
        }
    }

    @SuppressLint("RestrictedApi")
    @AfterPermissionGranted(Permission.R30)
    private void methodRequires2Permission() {
        if (!EasyPermissions.hasPermissions(mActivity, Permission.PERMS)) {
            PermissionHelper.newInstance(mActivity).directRequestPermissions(Permission.R30, Permission.PERMS);
        }
    }

    @Override
    public void onPermissionsGranted(int requestCode, @NonNull List<String> perms) {

    }

    @Override
    public void onPermissionsDenied(int requestCode, @NonNull List<String> perms) {

    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        EasyPermissions.onRequestPermissionsResult(requestCode, permissions, grantResults, this);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getAction() == KeyEvent.ACTION_DOWN) {
            if (System.currentTimeMillis() - firstTime > Config.DAILY_2_SECOND) {
                ToastUtil.show(mActivity, "再按一次退出程序");
                firstTime = System.currentTimeMillis();
            } else {
                finishAffinity();
                android.os.Process.killProcess(android.os.Process.myPid());
                System.exit(0);
            }
            return true;
        } else return keyCode == KeyEvent.KEYCODE_DEL && event.getAction() == KeyEvent.ACTION_DOWN;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK && data != null) {
            String content = data.getStringExtra(CaptureActivity.CONTENT);
            if (requestCode == CaptureActivity.APP_SN) {
                long sn = DataChange.StringToLong(content);
                if (sn != 0) {
                    ToastUtil.showProgressDialog(mActivity);

                    final List<String> alreadyInitPkg = new ArrayList<>();
                    DeviceSdkSourceConfigure alreadyInitDevice;
                    String sdkDeviceJson = mSession.getSdkDeviceJson();
                    if (DataUtil.isEmpty(sdkDeviceJson)) {
                        alreadyInitDevice = FarBeat.getInstance().getSdkDevice();
                    } else {
                        alreadyInitDevice = GSON.toBean(sdkDeviceJson, DeviceSdkSourceConfigure.class);
                    }
                    if (alreadyInitDevice != null) {
                        List<String> bPkg = alreadyInitDevice.getBasePackage();
                        if (bPkg != null && bPkg.size() > 0) {
                            alreadyInitPkg.addAll(bPkg);
                        }
                    }

                    FarBeat.getInstance().getCustomProvider().getDeviceSdkSourceConfigure(
                            new DeviceSdkSourceConfigureCallback<DeviceSdkSourceConfigure>() {
                        @Override
                        public void onResponse(int errorCode, DeviceSdkSourceConfigure sdkDevice) {
                            if (errorCode == ErrorCode.DEVICE_DATA_SUCCESS) {
                                List<String> pkGs = sdkDevice.getBasePackage();
                                if (pkGs != null && pkGs.size() > 0) {
                                    for (int i = 0; i < pkGs.size(); i++) {
                                        String pkg = pkGs.get(i);
                                        if (alreadyInitPkg.contains(pkg)) {
                                            pkGs.remove(pkg);
                                            i--;
                                        }
                                    }
                                    sdkDevice.setBasePackage(pkGs);

                                    NingApp.dpSdkSn = sn;
                                    mSession.setDpSdkSn(content);
                                    FarBeat.getInstance().setDpSdkSn(sn);

                                    InfoActivity.start(mActivity, pkGs.size() > 0 ? sdkDevice : null);
                                    ToastUtil.show(mActivity, "获取成功！");
                                } else {
                                    ToastUtil.show(mActivity, "没有要测试的设备适配SDK！");
                                }
                            }
                            ToastUtil.dismissProgressDialog();
                        }
                    }, sn);
                }
            }
        }
    }
}