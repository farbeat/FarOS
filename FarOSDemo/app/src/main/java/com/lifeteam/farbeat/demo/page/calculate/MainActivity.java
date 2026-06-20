package com.lifeteam.farbeat.demo.page.calculate;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;

import com.lifeteam.farbeat.HCE;
import com.lifeteam.farbeat.Manager;
import com.lifeteam.farbeat.Open;
import com.lifeteam.farbeat.base.BaseActivity;
import com.lifeteam.farbeat.demo.R;
import com.lifeteam.farbeat.demo.app.NingApp;
import com.lifeteam.farbeat.demo.dialog.MinuteDialog;
import com.lifeteam.farbeat.demo.listener.OnChooseMinuteListener;
import com.lifeteam.farbeat.dialog.AskDialog;
import com.lifeteam.farbeat.engin.FarBeatDevice;
import com.lifeteam.farbeat.engin.config.HealthDataSourceConfigList;
import com.lifeteam.farbeat.engin.data.HealthData2;
import com.lifeteam.farbeat.engin.data.HealthDataReport;
import com.lifeteam.farbeat.engin.data.HealthDataSetSingle;
import com.lifeteam.farbeat.engin.service.HrvBean;
import com.lifeteam.farbeat.engin.service.HrvSource;
import com.lifeteam.farbeat.engin.service.LvcSource;
import com.lifeteam.farbeat.listener.callback.HrvCallBack;
import com.lifeteam.farbeat.listener.callback.LvcCallBack;
import com.lifeteam.farbeat.listener.callback.OnSensorResponseListener;
import com.lifeteam.farbeat.listener.callback.PidCallBack;
import com.lifeteam.farbeat.listener.callback.TokenCallBack;
import com.lifeteam.farbeat.listener.common.OnPositiveListener;
import com.lifeteam.farbeat.listener.device.OnHealthDataListener;
import com.lifeteam.farbeat.util.constant.Config;
import com.lifeteam.farbeat.util.constant.DataSetType;
import com.lifeteam.farbeat.util.constant.DataType;
import com.lifeteam.farbeat.util.constant.ModeType;
import com.lifeteam.farbeat.util.constant.PeriodType;
import com.lifeteam.farbeat.util.constant.SourceType;
import com.lifeteam.farbeat.util.constant.ValueType;
import com.lifeteam.farbeat.util.data.DataUtil;
import com.lifeteam.farbeat.util.log.LogUtil;
import com.lifeteam.farbeat.util.system.SystemUtil;
import com.lifeteam.farbeat.util.time.TimeFormat;
import com.lifeteam.farbeat.util.ui.ToastUtil;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends BaseActivity implements OnHealthDataListener<HealthDataReport> {

    private String accessToken;
    private ImageButton topBarShare;
    private TextView topBarOther;
    private TextView mainMeasureState;
    private Button mainStartMeasure;
    private Button mainPid;
    private TextView mainAccessToken;
    private TextView mainText;
    private LinearLayout mainHrv;
    private TextView mainLomHfPeek;
    private TextView mainLomHfPower1;
    private TextView mainLomHfPower2;
    private TextView mainLomHfPower3;
    private TextView mainLomLfPeek;
    private TextView mainLomLfPower1;
    private TextView mainLomLfPower2;
    private TextView mainLomLfPower3;
    private TextView mainLomVlfPeek;
    private TextView mainLomVlfPower1;
    private TextView mainLomVlfPower2;
    private TextView mainLomVlfPower3;
    private TextView mainWelHfPeek;
    private TextView mainWelHfPower1;
    private TextView mainWelHfPower2;
    private TextView mainWelHfPower3;
    private TextView mainWelLfPeek;
    private TextView mainWelLfPower1;
    private TextView mainWelLfPower2;
    private TextView mainWelLfPower3;
    private TextView mainWelVlfPeek;
    private TextView mainWelVlfPower1;
    private TextView mainWelVlfPower2;
    private TextView mainWelVlfPower3;
    private TextView mainBe;
    private TextView mainGe;
    private TextView mainIe;
    private TextView mainSe;
    private TextView mainSd1;
    private TextView mainSd2;
    private TextView mainNn50;
    private TextView mainPnn50;
    private TextView mainMax;
    private TextView mainMean;
    private TextView mainMin;
    private TextView mainRmssd;
    private TextView mainSdnn;
    private TextView mainSdsd;
    private List<HealthDataSourceConfigList.HealthDataSourceConfig> configList;
    private FarBeatDevice currentFarBeatDevice;
    private String pId;
    private int currentMinute = 0;
    private long startTimestamp = 0L;
    private long endTimestamp = 0L;
    private final List<Integer> rriList = new ArrayList<>();
    private final List<Long> timestampList = new ArrayList<>();
    private PeterTimeCountRefresh peterTimeCountRefresh;
    private String startTime;
    private long surplusMillisUntilFinished = 0L;
    private long currentMillisInFuture = 0L;
    private boolean isHasData = false;
    private long mDeviceSourceTemplateId = 0;

    public static void start(Context context, HealthDataSourceConfigList sourceConfigList) {
        Intent intent = new Intent(context, MainActivity.class);
        intent.putExtra(Config.PARAM, sourceConfigList);
        context.startActivity(intent);
    }

    @Override
    public int getContentResId() {
        return R.layout.activity_main;
    }

    @SuppressLint("SetTextI18n")
    @Override
    public void init() {
        findViewById(R.id.top_bar_back).setVisibility(View.VISIBLE);
        TextView topBarTitle = findViewById(R.id.top_bar_title);
        topBarShare = findViewById(R.id.top_bar_share);
        topBarOther = findViewById(R.id.top_bar_other);
        mainMeasureState = findViewById(R.id.main_measure_state);
        mainStartMeasure = findViewById(R.id.main_start_measure);
        mainPid = findViewById(R.id.main_pid);
        mainAccessToken = findViewById(R.id.main_access_token);
        mainText = findViewById(R.id.main_text);
        mainLomHfPeek = findViewById(R.id.main_lom_hf_peek);
        mainLomHfPower1 = findViewById(R.id.main_lom_hf_power1);
        mainLomHfPower2 = findViewById(R.id.main_lom_hf_power2);
        mainLomHfPower3 = findViewById(R.id.main_lom_hf_power3);
        mainLomLfPeek = findViewById(R.id.main_lom_lf_peek);
        mainLomLfPower1 = findViewById(R.id.main_lom_lf_power1);
        mainLomLfPower2 = findViewById(R.id.main_lom_lf_power2);
        mainLomLfPower3 = findViewById(R.id.main_lom_lf_power3);
        mainLomVlfPeek = findViewById(R.id.main_lom_vlf_peek);
        mainLomVlfPower1 = findViewById(R.id.main_lom_vlf_power1);
        mainLomVlfPower2 = findViewById(R.id.main_lom_vlf_power2);
        mainLomVlfPower3 = findViewById(R.id.main_lom_vlf_power3);
        mainWelHfPeek = findViewById(R.id.main_wel_hf_peek);
        mainWelHfPower1 = findViewById(R.id.main_wel_hf_power1);
        mainWelHfPower2 = findViewById(R.id.main_wel_hf_power2);
        mainWelHfPower3 = findViewById(R.id.main_wel_hf_power3);
        mainWelLfPeek = findViewById(R.id.main_wel_lf_peek);
        mainWelLfPower1 = findViewById(R.id.main_wel_lf_power1);
        mainWelLfPower2 = findViewById(R.id.main_wel_lf_power2);
        mainWelLfPower3 = findViewById(R.id.main_wel_lf_power3);
        mainWelVlfPeek = findViewById(R.id.main_wel_vlf_peek);
        mainWelVlfPower1 = findViewById(R.id.main_wel_vlf_power1);
        mainWelVlfPower2 = findViewById(R.id.main_wel_vlf_power2);
        mainWelVlfPower3 = findViewById(R.id.main_wel_vlf_power3);
        mainBe = findViewById(R.id.main_be);
        mainGe = findViewById(R.id.main_ge);
        mainIe = findViewById(R.id.main_ie);
        mainSe = findViewById(R.id.main_se);
        mainSd1 = findViewById(R.id.main_sd1);
        mainSd2 = findViewById(R.id.main_sd2);
        mainNn50 = findViewById(R.id.main_nn50);
        mainPnn50 = findViewById(R.id.main_pnn50);
        mainMax = findViewById(R.id.main_max);
        mainMean = findViewById(R.id.main_mean);
        mainMin = findViewById(R.id.main_min);
        mainRmssd = findViewById(R.id.main_rmssd);
        mainSdnn = findViewById(R.id.main_sdnn);
        mainSdsd = findViewById(R.id.main_sdsd);
        mainHrv = findViewById(R.id.main_hrv);
        topBarTitle.setText(R.string.app_calculate);
        topBarShare.setVisibility(View.VISIBLE);
        currentFarBeatDevice = NingApp.currentFarBeatDevice;
        pId = mSession.getPid();
        if (!DataUtil.isEmpty(pId)) {
            mainPid.setText("Pid已获取");
        }
        Open.setOnHealthDataListener(this);

        HealthDataSourceConfigList sourceConfigList = (HealthDataSourceConfigList) getIntent().getSerializableExtra(Config.PARAM);
        if (sourceConfigList != null) {
            configList = sourceConfigList.getConfigList();
        }
    }

    public void onClick(@NonNull View v) {
        int id = v.getId();
        if (id == R.id.top_bar_back) {
            onBackPressed();
            return;
        }
        if (id == R.id.top_bar_share || id == R.id.top_bar_other) {
            int minute = currentMinute == 0 ? 5 : currentMinute;
            MinuteDialog minuteDialog = MinuteDialog.newInstance(minute);
            minuteDialog.setOnChooseMinuteListener(new OnChooseMinuteListener() {
                @SuppressLint("SetTextI18n")
                @Override
                public void onChooseMinute(int minute) {
                    currentMinute = minute == 0 ? minute + 1 : minute;
                    topBarShare.setVisibility(View.INVISIBLE);
                    topBarOther.setText(currentMinute + "分钟");
                    topBarOther.setVisibility(View.VISIBLE);
                }
            });
            minuteDialog.show(getSupportFragmentManager(), MinuteDialog.TAG);
            return;
        }
        if (id == R.id.main_start_measure) {
            if (currentMinute <= 0) {
                ToastUtil.show(mActivity, "请选择测量时长");
                return;
            }
            if (currentFarBeatDevice != null) {
                ToastUtil.showProgressDialog(mActivity, "正在开启实时测量！");
                Open.startRealTimeMeasure(currentFarBeatDevice, currentMinute * 60);
            }
            return;
        }
        if (id == R.id.main_stop_measure) {
            handler.sendEmptyMessage(Config.HUNDRED);
            interruptCountDownTimer();
            return;
        }
        if (id == R.id.main_clear) {
            startTimestamp = 0L;
            endTimestamp = 0L;
            rriList.clear();
            timestampList.clear();
            mainMeasureState.setText(Config.EMPTY);
            mainMeasureState.setVisibility(View.GONE);
            surplusMillisUntilFinished = 0L;
            currentMillisInFuture = 0L;
            return;
        }
        if (id == R.id.main_access_token) {
            SystemUtil.copy2Clipboard(mActivity, mainAccessToken.getText());
            ToastUtil.show(mActivity, "AccessToken 已复制到剪贴板！");
            return;
        }
        if (id == R.id.main_token) {
            ToastUtil.showProgressDialog(mActivity, "正在获取 AccessToken！");
            Manager.oauthToken(new TokenCallBack() {
                @Override
                public void onSuccess(String accessToken) {
                    MainActivity.this.accessToken = accessToken;
                    ToastUtil.show(mActivity, "获取成功！");
                    mainAccessToken.setText(accessToken);
                    mainAccessToken.setVisibility(View.VISIBLE);
                    ToastUtil.dismissProgressDialog();
                }

                @Override
                public void onFailed(int errorCode, String msg) {
                    super.onFailed(errorCode, msg);
                    ToastUtil.show(mActivity, "获取失败：" + errorCode + " ==> " + msg);
                    ToastUtil.dismissProgressDialog();
                }

                @Override
                public void onError(Throwable throwable) {
                    super.onError(throwable);
                    ToastUtil.show(mActivity, "获取报错：" + throwable.toString());
                    ToastUtil.dismissProgressDialog();
                }
            });
            return;
        }
        if (id == R.id.main_pid) {
            if (!DataUtil.isEmpty(pId)) {
                ToastUtil.show(mActivity, "Pid已拥有，无需再次获取！");
                return;
            }
            ToastUtil.showProgressDialog(mActivity, "正在获取 Pid！");
            Manager.generatePid(accessToken, new PidCallBack() {
                @SuppressLint("SetTextI18n")
                @Override
                public void onSuccess(String pid) {
                    if (!DataUtil.isEmpty(pid)) {
                        pId = pid;
                        mSession.setPid(pId);
                        mainPid.setText("Pid已获取");
                        ToastUtil.show(mActivity, "Pid获取成功！");
                    }
                    ToastUtil.dismissProgressDialog();
                }
            });
            return;
        }
        if (id == R.id.main_calculate_hrv) {
            if (DataUtil.isEmpty(accessToken)) {
                ToastUtil.show(mActivity, "AccessToken不存在，请刷新！");
                return;
            }
            if (DataUtil.isEmpty(pId)) {
                ToastUtil.show(mActivity, "Pid不存在，请先获取Pid！");
                return;
            }
            if (mDeviceSourceTemplateId == 0L) {
                ToastUtil.show(mActivity, "设备源ID不存在，请检查重试！");
                return;
            }
            ToastUtil.showProgressDialog(mActivity, "正在计算 HRV！");
            HrvSource hs = new HrvSource();
            hs.setPid(pId);
            hs.setRriList(rriList);
            hs.setRriTimeList(timestampList);
            HCE.getHrv(accessToken, mDeviceSourceTemplateId, hs, new HrvCallBack() {
                @SuppressLint("SetTextI18n")
                @Override
                public void onSuccess(HrvBean.HrvData hrv) {
                    if (hrv != null) {
                        showHrvResult(hrv);
                        ToastUtil.show(mActivity, "HCE.getHrv计算成功！");
                        mainText.setText("HCE.getHrv计算成功！");
                    }
                    ToastUtil.dismissProgressDialog();
                }

                @Override
                public void onFailed(int errorCode, String msg) {
                    super.onFailed(errorCode, msg);
                    ToastUtil.show(mActivity, "HCE.getHrv计算失败：" + errorCode + " ==> " + msg);
                    ToastUtil.dismissProgressDialog();
                }

                @Override
                public void onError(Throwable throwable) {
                    super.onError(throwable);
                    ToastUtil.show(mActivity, "HCE.getHrv计算报错：" + throwable.toString());
                    ToastUtil.dismissProgressDialog();
                }
            });
            return;
        }
        if (id == R.id.main_calculate_hrv_5m) {
            if (DataUtil.isEmpty(accessToken)) {
                ToastUtil.show(mActivity, "AccessToken不存在，请刷新！");
                return;
            }
            if (DataUtil.isEmpty(pId)) {
                ToastUtil.show(mActivity, "Pid不存在，请先获取Pid！");
                return;
            }
            if (mDeviceSourceTemplateId == 0L) {
                ToastUtil.show(mActivity, "设备源ID不存在，请检查重试！");
                return;
            }
            ToastUtil.showProgressDialog(mActivity, "正在计算5分钟 HRV！");
            HrvSource hs = new HrvSource();
            hs.setPid(pId);
            hs.setRriList(rriList);
            hs.setRriTimeList(timestampList);
            HCE.getHrv5m(accessToken, mDeviceSourceTemplateId, hs, new HrvCallBack() {
                @SuppressLint("SetTextI18n")
                @Override
                public void onSuccess(HrvBean.HrvData hrv) {
                    if (hrv != null) {
                        showHrvResult(hrv);
                        ToastUtil.show(mActivity, "HCE.getHrv5m计算成功！");
                        mainText.setText("HCE.getHrv5m计算成功！");
                    }
                    ToastUtil.dismissProgressDialog();
                }

                @Override
                public void onFailed(int errorCode, String msg) {
                    super.onFailed(errorCode, msg);
                    ToastUtil.show(mActivity, "HCE.getHrv5m计算失败：" + errorCode + " ==> " + msg);
                    ToastUtil.dismissProgressDialog();
                }

                @Override
                public void onError(Throwable throwable) {
                    super.onError(throwable);
                    ToastUtil.show(mActivity, "HCE.getHrv5m计算报错：" + throwable.toString());
                    ToastUtil.dismissProgressDialog();
                }
            });
            return;
        }
        if (id == R.id.main_calculate_hrv_24h) {
            if (DataUtil.isEmpty(accessToken)) {
                ToastUtil.show(mActivity, "AccessToken不存在，请刷新！");
                return;
            }
            if (DataUtil.isEmpty(pId)) {
                ToastUtil.show(mActivity, "Pid不存在，请先获取Pid！");
                return;
            }
            if (mDeviceSourceTemplateId == 0L) {
                ToastUtil.show(mActivity, "设备源ID不存在，请检查重试！");
                return;
            }
            ToastUtil.showProgressDialog(mActivity, "正在计算24小时 HRV！");
            HrvSource hs = new HrvSource();
            hs.setPid(pId);
            hs.setRriList(rriList);
            hs.setRriTimeList(timestampList);
            HCE.getHrv24h(accessToken, mDeviceSourceTemplateId, hs, new HrvCallBack() {
                @SuppressLint("SetTextI18n")
                @Override
                public void onSuccess(HrvBean.HrvData hrv) {
                    if (hrv != null) {
                        showHrvResult(hrv);
                        ToastUtil.show(mActivity, "HCE.getHrv24h计算成功！");
                        mainText.setText("HCE.getHrv24h计算成功！");
                    }
                    ToastUtil.dismissProgressDialog();
                }

                @Override
                public void onFailed(int errorCode, String msg) {
                    super.onFailed(errorCode, msg);
                    ToastUtil.show(mActivity, "HCE.getHrv24h计算失败：" + errorCode + " ==> " + msg);
                    ToastUtil.dismissProgressDialog();
                }

                @Override
                public void onError(Throwable throwable) {
                    super.onError(throwable);
                    ToastUtil.show(mActivity, "HCE.getHrv24h计算报错：" + throwable.toString());
                    ToastUtil.dismissProgressDialog();
                }
            });
            return;
        }
        if (id == R.id.main_calculate_lvc) {
            if (DataUtil.isEmpty(accessToken)) {
                ToastUtil.show(mActivity, "AccessToken不存在，请刷新！");
                return;
            }
            if (DataUtil.isEmpty(pId)) {
                ToastUtil.show(mActivity, "Pid不存在，请先获取Pid！");
                return;
            }
            if (mDeviceSourceTemplateId == 0L) {
                ToastUtil.show(mActivity, "设备源ID不存在，请检查重试！");
                return;
            }
            ToastUtil.showProgressDialog(mActivity, "正在计算 LVC！");
            LvcSource ls = new LvcSource();
            ls.setPid(pId);
            ls.setFunctDate(startTimestamp);
            ls.setRriList(rriList);
            HCE.getLvc(accessToken, mDeviceSourceTemplateId, ls, new LvcCallBack() {
                @SuppressLint("SetTextI18n")
                @Override
                public void onSuccess(int lvd, double lvv) {
                    ToastUtil.show(mActivity, "HCE.getLvc计算成功！");
                    mainText.setText("【LVC】活力度：" + lvd + "；活力值：" + lvv);
                    ToastUtil.dismissProgressDialog();
                }

                @Override
                public void onFailed(int errorCode, String msg) {
                    super.onFailed(errorCode, msg);
                    ToastUtil.show(mActivity, "HCE.getLvc计算失败：" + errorCode + " ==> " + msg);
                    ToastUtil.dismissProgressDialog();
                }

                @Override
                public void onError(Throwable throwable) {
                    super.onError(throwable);
                    ToastUtil.show(mActivity, "HCE.getLvc计算报错：" + throwable.toString());
                    ToastUtil.dismissProgressDialog();
                }
            });
        }
    }

    private void showHrvResult(HrvBean.HrvData hrv) {
        HrvBean.HrvData.FreqDomainBean freqDomain = hrv.getFreqDomain();
        if (freqDomain != null) {
            HrvBean.HrvData.FreqDomainBean.LombScarglePSDBean lombScarglePSD = freqDomain.getLombScarglePSD();
            if (lombScarglePSD != null) {
                HrvBean.HrvData.FreqDomainBean.LombScarglePSDBean.HfBean hf = lombScarglePSD.getHf();
                if (hf != null) {
                    mainLomHfPeek.setText(String.valueOf(hf.getPeek()));
                    mainLomHfPower1.setText(String.valueOf(hf.getPower1()));
                    mainLomHfPower2.setText(String.valueOf(hf.getPower2()));
                    mainLomHfPower3.setText(String.valueOf(hf.getPower3()));
                }
                HrvBean.HrvData.FreqDomainBean.LombScarglePSDBean.LfBean lf = lombScarglePSD.getLf();
                if (lf != null) {
                    mainLomLfPeek.setText(String.valueOf(lf.getPeek()));
                    mainLomLfPower1.setText(String.valueOf(lf.getPower1()));
                    mainLomLfPower2.setText(String.valueOf(lf.getPower2()));
                    mainLomLfPower3.setText(String.valueOf(lf.getPower3()));
                }
                HrvBean.HrvData.FreqDomainBean.LombScarglePSDBean.VlfBean vlf = lombScarglePSD.getVlf();
                if (vlf != null) {
                    mainLomVlfPeek.setText(String.valueOf(vlf.getPeek()));
                    mainLomVlfPower1.setText(String.valueOf(vlf.getPower1()));
                    mainLomVlfPower2.setText(String.valueOf(vlf.getPower2()));
                    mainLomVlfPower3.setText(String.valueOf(vlf.getPower3()));
                }
            }
            HrvBean.HrvData.FreqDomainBean.WelcPSDBean welcPSD = freqDomain.getWelcPSD();
            if (welcPSD != null) {
                HrvBean.HrvData.FreqDomainBean.WelcPSDBean.HfBeanX hf = welcPSD.getHf();
                if (hf != null) {
                    mainWelHfPeek.setText(String.valueOf(hf.getPeek()));
                    mainWelHfPower1.setText(String.valueOf(hf.getPower1()));
                    mainWelHfPower2.setText(String.valueOf(hf.getPower2()));
                    mainWelHfPower3.setText(String.valueOf(hf.getPower3()));
                }
                HrvBean.HrvData.FreqDomainBean.WelcPSDBean.LfBeanX lf = welcPSD.getLf();
                if (lf != null) {
                    mainWelLfPeek.setText(String.valueOf(lf.getPeek()));
                    mainWelLfPower1.setText(String.valueOf(lf.getPower1()));
                    mainWelLfPower2.setText(String.valueOf(lf.getPower2()));
                    mainWelLfPower3.setText(String.valueOf(lf.getPower3()));
                }
                HrvBean.HrvData.FreqDomainBean.WelcPSDBean.VlfBeanX vlf = welcPSD.getVlf();
                if (vlf != null) {
                    mainWelVlfPeek.setText(String.valueOf(vlf.getPeek()));
                    mainWelVlfPower1.setText(String.valueOf(vlf.getPower1()));
                    mainWelVlfPower2.setText(String.valueOf(vlf.getPower2()));
                    mainWelVlfPower3.setText(String.valueOf(vlf.getPower3()));
                }
            }
        }
        HrvBean.HrvData.NonlinearBean nonlinear = hrv.getNonlinear();
        if (nonlinear != null) {
            mainBe.setText(String.valueOf(nonlinear.getBe()));
            mainGe.setText(String.valueOf(nonlinear.getGe()));
            mainIe.setText(String.valueOf(nonlinear.getIe()));
            mainSe.setText(String.valueOf(nonlinear.getSe()));
        }
        HrvBean.HrvData.PoincareBean poincare = hrv.getPoincare();
        if (nonlinear != null) {
            mainSd1.setText(String.valueOf(poincare.getSD1()));
            mainSd2.setText(String.valueOf(poincare.getSD2()));
        }
        HrvBean.HrvData.TimeDomainBean timeDomain = hrv.getTimeDomain();
        if (timeDomain != null) {
            mainNn50.setText(String.valueOf(timeDomain.getNN50()));
            mainPnn50.setText(String.valueOf(timeDomain.getPNN50()));
            mainMax.setText(String.valueOf(timeDomain.getMax()));
            mainMean.setText(String.valueOf(timeDomain.getMean()));
            mainMin.setText(String.valueOf(timeDomain.getMin()));
            mainRmssd.setText(String.valueOf(timeDomain.getRmssd()));
            mainSdnn.setText(String.valueOf(timeDomain.getSdnn()));
            mainSdsd.setText(String.valueOf(timeDomain.getSdsd()));
        }
        mainHrv.setVisibility(View.VISIBLE);
    }

    @Override
    public void onHealthData(int errorCode, HealthDataReport report) {
        LogUtil.E("标准数据", report);
        if (!isHasData) {
            isHasData = true;
            Message message = new Message();
            message.what = Config.ZERO;
            message.obj = "实时测量开启成功，正在采集数据中...";
            handler.handleMessage(message);
        }
        if (report != null && configList != null && configList.size() > 0) {
            List<Long> sourceIds = report.getArrayOfDataSourceTemplateID();
            List<Integer> dataSetTypes = report.getArrayOfDataSetType();
            List<Object> healthDataSet = report.getHealthDataSet();
            if (sourceIds.size() == healthDataSet.size()) {
                for (int i = 0; i < sourceIds.size(); i++) {
                    long sId = sourceIds.get(i);
                    String schemeOfDataSourceType = null;
                    long dataSourceType = 0L;
                    int dataType = 0;
                    int dataValueType = 0;
                    int measurePeriodExist = 0;
                    long deviceSourceTemplateId = 0L;
                    for (HealthDataSourceConfigList.HealthDataSourceConfig config : configList) {
                        if (sId == config.getDataSourceTemplateId()) {
                            schemeOfDataSourceType = config.getSchemeOfDataSourceType();
                            dataSourceType = config.getDataSourceTypeList().get(0);
                            dataType = config.getDataTypeList().get(0);
                            dataValueType = config.getDataValueTypeList().get(0);
                            measurePeriodExist = config.getMeasurePeriodExist();
                            // 多种类型设备归类，如果只连接了一个或同种多个的设备只获取一次就可以
                            deviceSourceTemplateId = config.getDeviceSourceTemplateId();
                        }
                    }
                    if (mDeviceSourceTemplateId == 0 || mDeviceSourceTemplateId == deviceSourceTemplateId) {
                        if (mDeviceSourceTemplateId != deviceSourceTemplateId) {
                            mDeviceSourceTemplateId = deviceSourceTemplateId;
                        }
                        if (!DataUtil.isEmpty(schemeOfDataSourceType) && schemeOfDataSourceType.equals(ModeType.X73)) {
                            if (dataSourceType == SourceType.RRI) {
                                if (dataSetTypes.get(i) == DataSetType.SINGLE) {
                                    HealthDataSetSingle setSingle = (HealthDataSetSingle) healthDataSet.get(i);
                                    if (setSingle != null) {
                                        List<Object> healthDataList = setSingle.getHealthDataList();
                                        if (healthDataList != null) {
                                            int size = healthDataList.size();
                                            if (size > 0) {
                                                for (int j = 0; j < size; j++) {
                                                    switch (measurePeriodExist) {
                                                        case PeriodType.NO_EXIST:
                                                            HealthData2 health = (HealthData2) healthDataList.get(j);
                                                            if (health != null) {
                                                                endTimestamp = health.getTimestamp();
                                                                if (startTimestamp == 0 && currentMinute != 0) {
                                                                    startTimestamp = endTimestamp;
                                                                    startTime = TimeFormat.convertTimestamp(TimeFormat.FEN_FULL_MS, startTimestamp);
                                                                    // 开始等待倒计时
                                                                    currentMillisInFuture = currentMinute * 60 * 1000L;
                                                                    peterTimeCountRefresh = new PeterTimeCountRefresh(currentMillisInFuture);
                                                                    peterTimeCountRefresh.start();
                                                                }
                                                                Object value = health.getValue();
                                                                if (dataValueType == ValueType.NUMERIC) {
                                                                    switch (dataType) {
                                                                        case DataType.SHORT:
                                                                            Number n = (Number) value;
                                                                            rriList.add(n.intValue());
                                                                            break;
                                                                        case DataType.INT:
                                                                            rriList.add((int) value);
                                                                            break;
                                                                    }
                                                                }
                                                                timestampList.add(endTimestamp);
                                                                Message msg = new Message();
                                                                msg.what = Config.OK;
                                                                msg.obj = value;
                                                                handler.sendMessage(msg);
                                                            }
                                                            break;
                                                        case PeriodType.IS_EXIST:
                                                            break;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    private final Handler handler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(@NonNull Message msg) {
            switch (msg.what) {
                case Config.ZERO:
                    ToastUtil.show(mActivity, (String) msg.obj);
                    ToastUtil.dismissProgressDialog();
                    break;
                case Config.OK:
                    String ok = "采集首条时间：" + startTime + "\n" +
                            "采集到RRI数据：" + rriList.size() + " 条；\n" +
                            "RRI对应时间戳：" + timestampList.size() + " 条；\n" +
                            "当前最新RRI值：" + msg.obj + " ms。";
                    mainMeasureState.setText(ok);
                    mainMeasureState.setVisibility(View.VISIBLE);
                    break;
                case Config.TEN:
                    long millis = currentMillisInFuture - surplusMillisUntilFinished + 1000;
                    String ten = "采集首条时间：" + startTime + "\n" +
                            "采集末条时间：" + TimeFormat.convertTimestamp(TimeFormat.FEN_FULL_MS, endTimestamp) + "\n" +
                            "采集到RRI数据：" + rriList.size() + " 条；\n" +
                            "RRI对应时间戳：" + timestampList.size() + " 条；\n" +
                            "采集时长大约：" + TimeFormat.getCnTimerTime(millis) + "。";
                    mainMeasureState.setText(ten);
                    mainMeasureState.setVisibility(View.VISIBLE);

                    String obj = (String) msg.obj;
                    if (!DataUtil.isEmpty(obj)) {
                        ToastUtil.show(mActivity, obj);
                    }
                    break;
                case Config.NN:
                    mainStartMeasure.setText((String) msg.obj);
                    break;
                case Config.HUNDRED:
                    mainStartMeasure.setText("开启实时测量");
                    stopRealTimeMeasure(true);
                    break;
            }
        }
    };

    private void stopRealTimeMeasure(boolean isToast) {
        if (currentFarBeatDevice != null) {
            Open.stopRealTimeMeasure(currentFarBeatDevice, new OnSensorResponseListener() {
                @Override
                public void onResponse(boolean isSuccess, String msg) {
                    Message message = new Message();
                    message.what = Config.TEN;
                    message.obj = isToast ? ("停止实时测量：" + (isSuccess ? "成功" : "失败") + "；" + msg) : "";
                    handler.handleMessage(message);
                }
            });
        }
    }

    class PeterTimeCountRefresh extends CountDownTimer {

        public PeterTimeCountRefresh(long millisInFuture) {
            super(millisInFuture, 1000);
        }

        @Override
        public void onTick(long millisUntilFinished) {
            surplusMillisUntilFinished = millisUntilFinished;
            String timerTime = TimeFormat.getTimerTime(surplusMillisUntilFinished);
            Message msg = new Message();
            msg.what = Config.NN;
            msg.obj = timerTime;
            handler.handleMessage(msg);
        }

        @Override
        public void onFinish() {
            handler.sendEmptyMessage(Config.HUNDRED);
        }
    }

    public void interruptCountDownTimer() {
        if (peterTimeCountRefresh != null) {
            peterTimeCountRefresh.cancel();
            peterTimeCountRefresh = null;
        }
    }

    @Override
    protected void onDestroy() {
        interruptCountDownTimer();
        stopRealTimeMeasure(false);
        super.onDestroy();
    }

    @Override
    public void onBackPressed() {
        AskDialog askDialog = new AskDialog(mActivity, "确定要退出当前页面吗？\n退出后将清除数据且终止当前测试！");
        askDialog.setOnPositiveListener(new OnPositiveListener() {
            @Override
            public void onPositive() {
                finish();
            }
        });
    }
}