package com.lifeteam.farbeat.demo.dialog;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.lifeteam.farbeat.base.BaseDialogFragment;
import com.lifeteam.farbeat.demo.R;
import com.lifeteam.farbeat.demo.listener.OnChooseMinuteListener;
import com.lifeteam.farbeat.demo.wheelpicker.ex.WheelView;

import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.List;

public class MinuteDialog extends BaseDialogFragment implements View.OnClickListener {

    public static final String TAG = MinuteDialog.class.getSimpleName();
    private OnChooseMinuteListener onChooseMinuteListener;
    private int currentMinute;
    private WheelView<String> minuteWheel;
    private static final String CURRENT_MINUTE = "currentMinute";

    public void setOnChooseMinuteListener(OnChooseMinuteListener onChooseMinuteListener) {
        this.onChooseMinuteListener = onChooseMinuteListener;
    }

    @NotNull
    public static MinuteDialog newInstance(int currentMinute) {
        MinuteDialog dialog = new MinuteDialog();
        Bundle bundle = new Bundle();
        bundle.putInt(CURRENT_MINUTE, currentMinute);
        dialog.setArguments(bundle);
        return dialog;
    }

    @Override
    public int getContentResId() {
        return R.layout.dialog_minute;
    }

    @Override
    public int showHeight() {
        return 326;
    }

    @SuppressLint("SetTextI18n")
    @Override
    public void init() {

        TextView topDialogBarTitle = findViewById(R.id.top_dialog_bar_title);
        findViewById(R.id.top_dialog_bar_close).setVisibility(View.GONE);

        Bundle bundle = getArguments();
        if (bundle != null) {
            currentMinute = bundle.getInt(CURRENT_MINUTE);
            topDialogBarTitle.setText("选择时长（分钟）");
        }

        minuteWheel = findViewById(R.id.minute_wheel);

        initMinuteWheel();

        findViewById(R.id.button_cancel).setOnClickListener(this);
        findViewById(R.id.button_confirm).setOnClickListener(this);
    }

    private void initMinuteWheel() {
        List<String> minutes = new ArrayList<>();
        for (int i = 1; i <= 1440; i++) {
            minutes.add(i + "分钟");
        }
        minuteWheel.setData(minutes);
        minuteWheel.setSoundEffectResource(R.raw.snap_finger);
        minuteWheel.setSoundEffect(true);
        minuteWheel.setOnItemSelectedListener(new WheelView.OnItemSelectedListener<String>() {
            @Override
            public void onItemSelected(WheelView<String> view, String data, int position) {
                currentMinute = position + 1;
            }
        });
        minuteWheel.setSelectedItemPosition(Math.max(0, currentMinute - 1));
    }

    @Override
    public void onClick(@NotNull View v) {
        int id = v.getId();
        if (id == R.id.button_confirm && onChooseMinuteListener != null) {
            onChooseMinuteListener.onChooseMinute(currentMinute);
        }
        myDismiss();
    }
}