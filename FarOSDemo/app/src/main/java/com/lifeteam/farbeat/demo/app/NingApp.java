package com.lifeteam.farbeat.demo.app;

import android.app.Application;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Build;

import androidx.multidex.MultiDex;

import com.lifeteam.farbeat.FarBeat;
import com.lifeteam.farbeat.engin.FarBeatDevice;
import com.lifeteam.farbeat.listener.callback.OnSensorResponseListener;
import com.lifeteam.farbeat.util.constant.Config;
import com.lifeteam.farbeat.util.data.DataUtil;
import com.lifeteam.farbeat.util.log.LogUtil;
import com.tencent.smtt.export.external.TbsCoreSettings;
import com.tencent.smtt.sdk.QbSdk;
import com.tencent.smtt.sdk.TbsListener;

import org.jetbrains.annotations.Contract;
import org.jetbrains.annotations.NotNull;

import java.util.HashMap;
import java.util.Map;

public class NingApp extends Application {

    protected static NingApp mContext;
    public static String appId = "gw_e615c1cf3f9cb254";
    public static String appSecret = "a3fe50bc3fe44a918152d2caa37f60a3";
    public static long dpSdkSn = 99211048L;
    public static FarBeatDevice currentFarBeatDevice = null;
    public static String currentDeviceName = "";
    public static String currentDeviceMac = "";
    public static String versionName = "";
    public static String phoneInfo = "";

    @Override
    public void onCreate() {
        super.onCreate();
        mContext = this;

        String pkg = mContext.getPackageName();
        String packageName = DataUtil.isEmpty(pkg) ? "com.lifeteam.farbeat.demo" : pkg;
        versionName = DataUtil.null2Empty(getVersionName(mContext, packageName));
        phoneInfo = DataUtil.null2Empty(getPhoneInfo());

        // 初始化SDK
        FarBeat farBeat = FarBeat.getInstance();
        farBeat.init(mContext, true, new OnSensorResponseListener() {
            @Override
            public void onResponse(boolean isSuccess, String msg) {
                LogUtil.E("初始化SDK：" + isSuccess, msg);
            }
        });
        farBeat.setIdentification(appId, appSecret, dpSdkSn);

        initTBS();
    }

    private void initTBS() {
        Map<String, Object> map = new HashMap<>();
        map.put(TbsCoreSettings.TBS_SETTINGS_USE_SPEEDY_CLASSLOADER, true);
        map.put(TbsCoreSettings.TBS_SETTINGS_USE_DEXLOADER_SERVICE, true);
        QbSdk.initTbsSettings(map);

        QbSdk.setDownloadWithoutWifi(true);
        QbSdk.initX5Environment(mContext, new QbSdk.PreInitCallback() {
            @Override
            public void onCoreInitFinished() {
                // 内核初始化完成，可能为系统内核，也可能为系统内核
                LogUtil.E("TBS", "onCoreInitFinished");
            }

            /**
             * 预初始化结束
             * 由于X5内核体积较大，需要依赖网络动态下发，所以当内核不存在的时候，默认会回调false，此时将会使用系统内核代替
             * @param isX5 是否使用X5内核
             */
            @Override
            public void onViewInitFinished(boolean isX5) {
                LogUtil.E("TBS", "onViewInitFinished：" + isX5);
            }
        });

        QbSdk.setTbsListener(new TbsListener() {
            @Override
            public void onDownloadFinish(int i) {
                LogUtil.E("TBS", "下载完成 progress = " + i);
            }

            @Override
            public void onInstallFinish(int i) {
                LogUtil.E("TBS", "正在安装内核 progress = " + i);
            }

            @Override
            public void onDownloadProgress(int i) {
//                LogUtil.E("TBS", "已经下载 progress = " + i);
            }
        });
    }

    public static NingApp getAppContext() {
        return mContext;
    }

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
        MultiDex.install(mContext);
    }

    /**
     * 获取系统版本名称
     */
    public String getVersionName(@NotNull Context context, String packageName) {
        String versionName = Config.EMPTY;
        try {
            PackageManager pm = context.getPackageManager();
            PackageInfo pi = pm.getPackageInfo(packageName, 0);
            versionName = pi.versionName;
        } catch (PackageManager.NameNotFoundException e) {
            LogUtil.E("获取系统版本名称异常", e.toString());
        }
        return versionName;
    }

    /**
     * 获取设备基本信息
     */
    @NotNull
    @Contract(pure = true)
    public String getPhoneInfo() {
        return Build.BRAND + " " + Build.MODEL;
    }
}