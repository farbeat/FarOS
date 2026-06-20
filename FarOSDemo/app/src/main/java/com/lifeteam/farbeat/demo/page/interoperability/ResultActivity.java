package com.lifeteam.farbeat.demo.page.interoperability;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.lifeteam.farbeat.base.BaseActivity;
import com.lifeteam.farbeat.demo.R;
import com.lifeteam.farbeat.demo.bean.InfoBean;
import com.lifeteam.farbeat.demo.page.other.ReaderActivity;
import com.lifeteam.farbeat.demo.util.EmailUtil;
import com.lifeteam.farbeat.demo.util.FilesProvider;
import com.lifeteam.farbeat.util.constant.Config;
import com.lifeteam.farbeat.util.data.DataUtil;
import com.lifeteam.farbeat.util.data.GSON;
import com.lifeteam.farbeat.util.ui.ToastUtil;
import com.zlylib.fileselectorlib.FileSelector;
import com.zlylib.fileselectorlib.SelectOptions;
import com.zlylib.fileselectorlib.bean.EssFile;
import com.zlylib.fileselectorlib.utils.Const;

import java.io.File;
import java.util.ArrayList;

public class ResultActivity extends BaseActivity {

    private boolean isShowEmailInfo = false;
    private String exportExcelPath;
    private InfoBean info;
    private EditText resultHost;
    private EditText resultPort;
    private EditText resultFromEmail;
    private EditText resultFromPwd;
    private EditText resultToEmail;
    private LinearLayout resultInfoLl;
    private Button resultSendEmail;

    public static void start(Context context, String path) {
        Intent intent = new Intent(context, ResultActivity.class);
        intent.putExtra(Config.PARAM, path);
        context.startActivity(intent);
    }

    @Override
    public int getContentResId() {
        return R.layout.activity_result;
    }

    @SuppressLint("SetTextI18n")
    @Override
    public void init() {
        findViewById(R.id.top_bar_back).setVisibility(View.VISIBLE);
        TextView topBarTitle = findViewById(R.id.top_bar_title);
        TextView resultPath = findViewById(R.id.result_path);
        resultHost = findViewById(R.id.result_host);
        resultPort = findViewById(R.id.result_port);
        resultFromEmail = findViewById(R.id.result_from_email);
        resultFromPwd = findViewById(R.id.result_from_pwd);
        resultToEmail = findViewById(R.id.result_to_email);
        resultInfoLl = findViewById(R.id.result_info_ll);
        resultSendEmail = findViewById(R.id.result_send_email);

        topBarTitle.setText("测试结果");
        exportExcelPath = getIntent().getStringExtra(Config.PARAM);
        if (!DataUtil.isEmpty(exportExcelPath)) {
            resultPath.setText(InterActivity.sheetName + "文件路径：" + exportExcelPath);
        }

        String param = mSession.getParam();
        if (!DataUtil.isEmpty(param)) {
            info = GSON.toBean(param, InfoBean.class);
            if (info != null) {
                String host = info.getHost();
                if (!DataUtil.isEmpty(host)) {
                    resultHost.setText(host);
                }
                String port = info.getPort();
                if (!DataUtil.isEmpty(port)) {
                    resultPort.setText(port);
                }
                String fromMail = info.getFromMail();
                if (!DataUtil.isEmpty(fromMail)) {
                    resultFromEmail.setText(fromMail);
                }
                String fromPwd = info.getFromPwd();
                if (!DataUtil.isEmpty(fromPwd)) {
                    resultFromPwd.setText(fromPwd);
                }
                String toMail = info.getToMail();
                if (!DataUtil.isEmpty(toMail)) {
                    resultToEmail.setText(toMail);
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
        if (id == R.id.result_look) {
            FileSelector.from(mActivity)
                    .isSingle()
                    .setTargetPath(InterActivity.filePath)
                    .setFileTypes(InterActivity.suffix)
                    .requestCode(Config.OK)
                    .start();
            return;
        }
        if (id == R.id.result_send_email) {
            if (!isShowEmailInfo) {
                isShowEmailInfo = true;
                resultInfoLl.setVisibility(View.VISIBLE);
                return;
            }
            if (!new File(exportExcelPath).exists()) {
                ToastUtil.show(mActivity, "【" + InterActivity.sheetName + "】文件不存在！");
                return;
            }
            String host = resultHost.getText().toString().trim();
            if (DataUtil.isEmpty(host)) {
                ToastUtil.show(mActivity, "请输入邮箱主机(HOST)！");
                return;
            }
            String port = resultPort.getText().toString().trim();
            if (DataUtil.isEmpty(port)) {
                ToastUtil.show(mActivity, "请输入邮箱端口(PORT)！");
                return;
            }
            String fromEmail = resultFromEmail.getText().toString().trim();
            if (DataUtil.isEmpty(fromEmail)) {
                ToastUtil.show(mActivity, "请输入发件人邮箱！");
                return;
            }
            String fromPwd = resultFromPwd.getText().toString().trim();
            if (DataUtil.isEmpty(fromPwd)) {
                ToastUtil.show(mActivity, "请输入发件人邮箱密码！");
                return;
            }
            String toEmail = resultToEmail.getText().toString().trim();
            if (DataUtil.isEmpty(toEmail)) {
                ToastUtil.show(mActivity, "请输入收件人邮箱！");
                return;
            }
            if (info == null) {
                info = new InfoBean();
            }
            info.setHost(host);
            info.setPort(port);
            info.setFromMail(fromEmail);
            info.setFromPwd(fromPwd);
            info.setToMail(toEmail);
            mSession.setParam(GSON.toJson(info));
            String body = InterActivity.sheetName + "数据，请下载附件查看！\n邮件由系统自动发送，无需回复！";
            EmailUtil.sendEmail(fromEmail, fromPwd, toEmail, host, port, InterActivity.sheetName, body, exportExcelPath);
            ToastUtil.show(mActivity, "邮件已发送，请前往邮箱查看！");
            resultSendEmail.setVisibility(View.GONE);
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == Config.OK) {
            if (data != null && SelectOptions.getInstance().isSingle) {
                ArrayList<EssFile> essFileList = data.getParcelableArrayListExtra(Const.EXTRA_RESULT_SELECTION);
                if (essFileList != null && essFileList.size() > 0) {
                    EssFile essFile = essFileList.get(0);
                    if (essFile.isFile() && essFile.isExits()) {
                        ReaderActivity.start(mActivity, InterActivity.sheetName, essFile.getAbsolutePath());
                    }
                }
            }
        }
    }

    private void openDir() {
        File file = new File(InterActivity.filePath);
        if (file.exists()) {
            Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
            intent.setType("*/*");
            intent.addCategory(Intent.CATEGORY_OPENABLE);
            String authority = mActivity.getPackageName() + ".provider";
            Uri uriForFile = FilesProvider.getUriForFile(mActivity, authority, file);
            intent.setDataAndType(uriForFile, "file/*");
            mActivity.startActivity(intent);
        }
    }
}