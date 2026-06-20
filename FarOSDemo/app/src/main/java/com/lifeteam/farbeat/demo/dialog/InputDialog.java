package com.lifeteam.farbeat.demo.dialog;

import android.app.Dialog;
import android.content.Context;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import androidx.annotation.NonNull;

import com.lifeteam.farbeat.demo.R;
import com.lifeteam.farbeat.demo.bean.InfoBean;
import com.lifeteam.farbeat.demo.listener.OnInfoListener;
import com.lifeteam.farbeat.util.cache.AppSession;
import com.lifeteam.farbeat.util.data.DataUtil;
import com.lifeteam.farbeat.util.data.GSON;
import com.lifeteam.farbeat.util.ui.ToastUtil;

public class InputDialog implements View.OnClickListener {

    public static final int TEST_CONCLUSION = 10001;
    private final Context context;
    private final AppSession session;
    private final int function;
    private Dialog dialog;
    private InfoBean info;
    private final EditText inputContent;
    private OnInfoListener onInfoListener;

    public void setOnInfoListener(OnInfoListener onInfoListener) {
        this.onInfoListener = onInfoListener;
    }

    public InputDialog(Context context, AppSession session, int function) {
        this.context = context;
        this.session = session;
        this.function = function;
        if (dialog != null && dialog.isShowing()) {
            dialog.dismiss();
            dialog = null;
        }
        dialog = new Dialog(context, R.style.ProgressDialogHasBg);
        dialog.setContentView(R.layout.dialog_input);
        dialog.setCancelable(false);
        dialog.show();
        TextView inputTitle = dialog.findViewById(R.id.input_title);
        inputContent = dialog.findViewById(R.id.input_content);
        if (function == TEST_CONCLUSION) {
            inputTitle.setText("检测结论");
            inputContent.setHint("请输入检测结论");
            String param = session.getParam();
            if (!DataUtil.isEmpty(param)) {
                info = GSON.toBean(param, InfoBean.class);
                if (info != null) {
                    String conclusion = info.getConclusion();
                    if (!DataUtil.isEmpty(param)) {
                        inputContent.setText(conclusion);
                    }
                }
            }
        }
        TextView buttonCancel = dialog.findViewById(R.id.button_cancel);
        TextView buttonConfirm = dialog.findViewById(R.id.button_confirm);
        buttonCancel.setVisibility(View.GONE);
        buttonConfirm.setOnClickListener(this);
    }

    @Override
    public void onClick(@NonNull View v) {
        if (v.getId() == R.id.button_confirm) {
            String content = inputContent.getText().toString().trim();
            if (function == TEST_CONCLUSION) {
                if (DataUtil.isEmpty(content)) {
                    ToastUtil.show(context, "请输入检测结论");
                    return;
                }
                if (info == null) {
                    info = new InfoBean();
                }
                info.setConclusion(content);
                session.setParam(GSON.toJson(info));
            }
            if (onInfoListener != null && info != null) {
                onInfoListener.onInfo(info);
            }
        }
        dismissProgressDialog();
    }

    private void dismissProgressDialog() {
        if (null != dialog && dialog.isShowing()) {
            dialog.dismiss();
            dialog = null;
        }
    }
}