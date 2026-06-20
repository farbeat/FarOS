package com.lifeteam.farbeat.demo.page.interoperability;

import android.content.Context;
import android.content.Intent;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import androidx.annotation.NonNull;

import com.lifeteam.farbeat.base.BaseActivity;
import com.lifeteam.farbeat.demo.R;
import com.lifeteam.farbeat.demo.bean.InfoBean;
import com.lifeteam.farbeat.engin.config.DeviceSdkSourceConfigure;
import com.lifeteam.farbeat.util.constant.Config;
import com.lifeteam.farbeat.util.data.DataUtil;
import com.lifeteam.farbeat.util.data.GSON;
import com.lifeteam.farbeat.util.ui.ToastUtil;

public class InfoActivity extends BaseActivity {

    private DeviceSdkSourceConfigure sdkDevice;
    private EditText infoTester;
    private EditText infoSubmitter;
    private EditText infoAddress;
    private InfoBean info;

    public static void start(Context context, DeviceSdkSourceConfigure sdkDevice) {
        Intent intent = new Intent(context, InfoActivity.class);
        intent.putExtra(Config.PARAM, sdkDevice);
        context.startActivity(intent);
    }

    @Override
    public int getContentResId() {
        return R.layout.activity_info;
    }

    @Override
    public void init() {
        sdkDevice = (DeviceSdkSourceConfigure) getIntent().getSerializableExtra(Config.PARAM);
        findViewById(R.id.top_bar_back).setVisibility(View.VISIBLE);
        TextView topBarTitle = findViewById(R.id.top_bar_title);
        topBarTitle.setText("基本信息");
        infoTester = findViewById(R.id.info_tester);
        infoSubmitter = findViewById(R.id.info_submitter);
        infoAddress = findViewById(R.id.info_address);

        String param = mSession.getParam();
        if (!DataUtil.isEmpty(param)) {
            info = GSON.toBean(param, InfoBean.class);
            if (info != null) {
                String tester = info.getTester();
                if (!DataUtil.isEmpty(tester)) {
                    infoTester.setText(tester);
                }
                String submitter = info.getSubmitter();
                if (!DataUtil.isEmpty(submitter)) {
                    infoSubmitter.setText(submitter);
                }
                String address = info.getAddress();
                if (!DataUtil.isEmpty(address)) {
                    infoAddress.setText(address);
                }
            }
        }
    }

    public void onClick(@NonNull View v) {
        int id = v.getId();
        if (id == R.id.top_bar_back) {
            finish();
            return;
        }
        if (id == R.id.info_go) {
            String tester = infoTester.getText().toString().trim();
            if (DataUtil.isEmpty(tester)) {
                ToastUtil.show(mActivity, "请输入测试人姓名！");
                return;
            }
            String submitter = infoSubmitter.getText().toString().trim();
            if (DataUtil.isEmpty(submitter)) {
                ToastUtil.show(mActivity, "请输入送检人姓名！");
                return;
            }
            String address = infoAddress.getText().toString().trim();
            if (DataUtil.isEmpty(address)) {
                ToastUtil.show(mActivity, "请输入测试公司名称！");
                return;
            }
            if (info == null) {
                info = new InfoBean();
            }
            info.setTester(tester);
            info.setSubmitter(submitter);
            info.setAddress(address);
            mSession.setParam(GSON.toJson(info));
            InterActivity.start(mActivity, sdkDevice);
            finish();
        }
    }
}