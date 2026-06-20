package com.lifeteam.farbeat.demo.adapter;

import android.content.Context;
import android.view.View;

import com.lifeteam.farbeat.demo.R;
import com.lifeteam.farbeat.util.recycler.OnClickListener;
import com.lifeteam.farbeat.util.recycler.RecyclerAdapter;
import com.lifeteam.farbeat.util.recycler.RecyclerHolder;

import org.jetbrains.annotations.NotNull;

import java.util.List;

public class InterLogcatAdapter extends RecyclerAdapter<String> implements OnClickListener {

    public InterLogcatAdapter(Context context, List<String> data) {
        super(context, data, R.layout.item_inter_logcat);
    }

    @Override
    public void convert(@NotNull RecyclerHolder holder, @NotNull String item) {
        holder.setTvText(R.id.inter_logcat, item);

        holder.setOnClickListener(this, R.id.inter_logcat);
    }

    @Override
    public void onClick(View v, int position, RecyclerHolder holder) {

    }
}