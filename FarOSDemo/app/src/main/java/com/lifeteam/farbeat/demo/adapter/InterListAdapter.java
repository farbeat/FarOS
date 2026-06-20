package com.lifeteam.farbeat.demo.adapter;

import android.annotation.SuppressLint;
import android.content.Context;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import com.lifeteam.farbeat.demo.R;
import com.lifeteam.farbeat.demo.bean.InterExcel;
import com.lifeteam.farbeat.demo.page.interoperability.InterActivity;
import com.lifeteam.farbeat.util.data.Res;
import com.lifeteam.farbeat.util.recycler.OnClickListener;
import com.lifeteam.farbeat.util.recycler.RecyclerAdapter;
import com.lifeteam.farbeat.util.recycler.RecyclerHolder;

import org.jetbrains.annotations.NotNull;

import java.util.List;

public class InterListAdapter extends RecyclerAdapter<InterExcel> implements OnClickListener {

    public InterListAdapter(Context context, List<InterExcel> data) {
        super(context, data, R.layout.item_inter_list);
    }

    @SuppressLint("SetTextI18n")
    @Override
    public void convert(@NotNull RecyclerHolder holder, @NotNull InterExcel item) {
        TextView interListTitle = holder.findViewById(R.id.inter_list_title);
        ImageView interListResult = holder.findViewById(R.id.inter_list_result);

        String title = item.getNumber() + "、" + item.getTitle();
        interListTitle.setText(title);
        interListTitle.setTextColor(item.isTest() ?
                Res.color(mContext, R.color.black) :
                Res.color(mContext, R.color.gray9)
        );

        String result = item.getResult();
        switch (result) {
            case InterActivity.PASS:
                interListResult.setImageResource(R.mipmap.duihao);
                break;
            case InterActivity.FAIL:
                interListResult.setImageResource(R.mipmap.chahao);
                break;
            default:
                interListResult.setImageResource(R.mipmap.henggang);
                break;
        }
        holder.setOnClickListener(this, R.id.inter_list_root);
    }

    @Override
    public void onClick(View v, int position, RecyclerHolder holder) {

    }
}