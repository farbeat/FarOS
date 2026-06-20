package com.lifeteam.farbeat.demo.adapter;

import android.annotation.SuppressLint;
import android.content.Context;
import android.view.View;
import android.widget.TextView;

import com.lifeteam.farbeat.Open;
import com.lifeteam.farbeat.demo.R;
import com.lifeteam.farbeat.demo.app.NingApp;
import com.lifeteam.farbeat.demo.page.calculate.MainActivity;
import com.lifeteam.farbeat.dialog.AskDialog;
import com.lifeteam.farbeat.engin.FarBeatDevice;
import com.lifeteam.farbeat.engin.config.HealthDataSourceConfigList;
import com.lifeteam.farbeat.listener.callback.OnSensorResponseListener;
import com.lifeteam.farbeat.listener.common.OnPositiveListener;
import com.lifeteam.farbeat.util.data.DataUtil;
import com.lifeteam.farbeat.util.recycler.OnClickListener;
import com.lifeteam.farbeat.util.recycler.OnLongClickListener;
import com.lifeteam.farbeat.util.recycler.RecyclerAdapter;
import com.lifeteam.farbeat.util.recycler.RecyclerHolder;
import com.lifeteam.farbeat.util.system.SystemUtil;
import com.lifeteam.farbeat.util.ui.ToastUtil;

import org.jetbrains.annotations.NotNull;

import java.util.List;

public class DeviceAdapter extends RecyclerAdapter<FarBeatDevice> implements OnClickListener, OnLongClickListener {

    private HealthDataSourceConfigList sourceConfigList;

    public void setSourceConfigList(HealthDataSourceConfigList sourceConfigList) {
        this.sourceConfigList = sourceConfigList;
    }

    public DeviceAdapter(Context context, List<FarBeatDevice> data) {
        super(context, data, R.layout.item_device);
    }

    @SuppressLint("SetTextI18n")
    @Override
    public void convert(@NotNull RecyclerHolder holder, @NotNull FarBeatDevice item) {
        TextView deviceName = holder.findViewById(R.id.device_name);
        TextView deviceMac = holder.findViewById(R.id.device_mac);
        String name = item.getName();
        if (!DataUtil.isEmpty(name)) {
            deviceName.setText(name);
        }
        String mac = item.getMac();
        if (!DataUtil.isEmpty(mac)) {
            deviceMac.setText("MAC：" + mac);
        }
        holder.setOnClickListener(this, R.id.device_root);
        holder.setOnLongClickListener(this, R.id.device_root);
    }

    @Override
    public void onClick(View v, int position, RecyclerHolder holder) {
        FarBeatDevice item = getItem(position);
        if (item != null) {
            NingApp.currentFarBeatDevice = item;
            MainActivity.start(mContext, sourceConfigList);
        }
    }

    @Override
    public void onLongClick(View v, final int position, RecyclerHolder holder) {
        FarBeatDevice item = getItem(position);
        if (item != null) {
            AskDialog askDialog = new AskDialog(mContext, "您确定要移除设备【" + item.getName() + "】吗？");
            askDialog.setOnPositiveListener(new OnPositiveListener() {
                @Override
                public void onPositive() {
                    Open.removeSourceDevice(item, new OnSensorResponseListener() {
                        @Override
                        public void onResponse(boolean isSuccess, String msg) {
                            if (isSuccess) {
                                String mac = item.getMac();
                                if (!DataUtil.isEmpty(mac)) {
                                    SystemUtil.unpairDevice(mac);
                                }
                                removeItem(position);
                                ToastUtil.show(mContext, "移除成功");
                            } else {
                                ToastUtil.show(mContext, DataUtil.isEmpty(msg) ? "移除失败" : msg);
                            }
                        }
                    });
                }
            });
        }
    }
}