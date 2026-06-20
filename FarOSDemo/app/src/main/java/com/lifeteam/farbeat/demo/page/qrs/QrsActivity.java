package com.lifeteam.farbeat.demo.page.qrs;

import android.content.Context;
import android.content.Intent;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.lifeteam.farbeat.FarPulse;
import com.lifeteam.farbeat.Manager;
import com.lifeteam.farbeat.base.BaseActivity;
import com.lifeteam.farbeat.demo.R;
import com.lifeteam.farbeat.demo.app.NingApp;
import com.lifeteam.farbeat.demo.page.other.CaptureActivity;
import com.lifeteam.farbeat.engin.qrs.QrsDiscern;
import com.lifeteam.farbeat.engin.qrs.QrsParam;
import com.lifeteam.farbeat.listener.callback.TokenCallBack;
import com.lifeteam.farbeat.util.data.DataChange;
import com.lifeteam.farbeat.util.data.DataUtil;
import com.lifeteam.farbeat.util.log.LogUtil;
import com.lifeteam.farbeat.util.system.SystemUtil;
import com.lifeteam.farbeat.util.ui.ToastUtil;

import java.util.List;

public class QrsActivity extends BaseActivity {

    private TextView qrsId;
    private TextView qrsSecret;
    private TextView qrsAccessToken;
    private TextView qrsCalculateResult;
    private EditText qrsRaw;

    public static void start(Context context) {
        Intent intent = new Intent(context, QrsActivity.class);
        context.startActivity(intent);
    }

    @Override
    public int getContentResId() {
        return R.layout.activity_qrs;
    }

    @Override
    public void init() {
        findViewById(R.id.top_bar_back).setVisibility(View.VISIBLE);
        TextView topBarTitle = findViewById(R.id.top_bar_title);
        topBarTitle.setText(R.string.app_qrs);
        qrsId = findViewById(R.id.qrs_id);
        qrsSecret = findViewById(R.id.qrs_secret);
        qrsAccessToken = findViewById(R.id.qrs_access_token);
        qrsCalculateResult = findViewById(R.id.qrs_calculate_result);
        qrsRaw = findViewById(R.id.qrs_raw);

        setAppId();
        setAppSecret();
    }

    private void setAppId() {
        String appId = NingApp.appId;
        if (DataUtil.isEmpty(appId)) {
            appId = mSession.getAppId();
        }
        if (!DataUtil.isEmpty(appId)) {
            NingApp.appId = appId;
            qrsId.setText(appId);
        }
    }

    private void setAppSecret() {
        String appSecret = NingApp.appSecret;
        if (DataUtil.isEmpty(appSecret)) {
            appSecret = mSession.getAppSecret();
        }
        if (!DataUtil.isEmpty(appSecret)) {
            NingApp.appSecret = appSecret;
            qrsSecret.setText(appSecret);
        }
    }

    public void onClick(@NonNull View v) {
        int id = v.getId();
        if (id == R.id.top_bar_back) {
            finish();
            return;
        }
        if (id == R.id.qrs_id_scan) {
            CaptureActivity.startForResult(mActivity, CaptureActivity.APP_ID);
            return;
        }
        if (id == R.id.qrs_secret_scan) {
            CaptureActivity.startForResult(mActivity, CaptureActivity.APP_SECRET);
            return;
        }
        if (id == R.id.qrs_token) {
            if (DataUtil.isEmpty(NingApp.appId)) {
                ToastUtil.show(mActivity, "请扫码获取 AppId！");
                return;
            }
            if (DataUtil.isEmpty(NingApp.appSecret)) {
                ToastUtil.show(mActivity, "请扫码获取 AppSecret！");
                return;
            }
            ToastUtil.showProgressDialog(mActivity, "正在获取 AccessToken！");
            Manager.oauthToken(new TokenCallBack() {
                @Override
                public void onSuccess(String accessToken) {
                    ToastUtil.show(mActivity, "获取成功！");
                    qrsAccessToken.setText(accessToken);
                    qrsAccessToken.setVisibility(View.VISIBLE);
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
        if (id == R.id.qrs_access_token) {
            String accessToken = qrsAccessToken.getText().toString();
            if (DataUtil.isEmpty(accessToken)) {
                ToastUtil.show(mActivity, "请刷新 AccessToken！");
                return;
            }
            SystemUtil.copy2Clipboard(mActivity, accessToken);
            ToastUtil.show(mActivity, "AccessToken 已复制到剪贴板！");
            return;
        }
        if (id == R.id.qrs_calculate) {
            String accessToken = qrsAccessToken.getText().toString();
            if (DataUtil.isEmpty(accessToken)) {
                ToastUtil.show(mActivity, "请刷新 AccessToken！");
                return;
            }
            String qr = qrsRaw.getText().toString().trim();
            if (DataUtil.isEmpty(qr)) {
                ToastUtil.show(mActivity, "原始数据不存在！");
                return;
            }
            float[] raw = getRaw(qr);
            if (raw == null || raw.length == 0) {
                ToastUtil.show(mActivity, "原始数据不存在！");
                return;
            }
            QrsDiscern qrs = FarPulse.getQRSDiscernByDefaultParam(accessToken, raw);
            if (qrs == null) {
                ToastUtil.show(mActivity, "QRS计算结果不存在！");
                return;
            }
            String qv = new Gson().toJson(qrs);
            if (DataUtil.isEmpty(qv)) {
                ToastUtil.show(mActivity, "QRS计算结果不存在！");
                return;
            }
            qrsCalculateResult.setText(qv);
            return;
        }
        if (id == R.id.qrs_calculate_result) {
            String qrsResult = qrsCalculateResult.getText().toString();
            if (DataUtil.isEmpty(qrsResult)) {
                ToastUtil.show(mActivity, "QRS计算结果不存在！");
                return;
            }
            SystemUtil.copy2Clipboard(mActivity, qrsResult);
            ToastUtil.show(mActivity, "QRS计算结果已复制到剪贴板！");
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
            }
        }
    }

    private float[] getRaw(String qrsRaw) {
        List<Integer> rawList = new Gson().fromJson(qrsRaw, new TypeToken<List<Integer>>() {}.getType());
        return DataChange.integerList2FloatArray(rawList);
    }
}