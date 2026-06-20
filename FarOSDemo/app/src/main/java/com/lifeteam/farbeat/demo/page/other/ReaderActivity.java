package com.lifeteam.farbeat.demo.page.other;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.RelativeLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;

import com.lifeteam.farbeat.base.BaseActivity;
import com.lifeteam.farbeat.demo.R;
import com.lifeteam.farbeat.listener.common.ClickListener;
import com.lifeteam.farbeat.util.data.DataUtil;
import com.tencent.smtt.sdk.TbsReaderView;

import java.io.File;

public class ReaderActivity extends BaseActivity implements ClickListener, TbsReaderView.ReaderCallback {

    private static final String FILE_TITLE = "file_title";
    private static final String FILE_PATH = "file_path";
    private TbsReaderView tbsReaderView;

    public static void start(Context context, String title, String path) {
        Intent intent = new Intent(context, ReaderActivity.class);
        intent.putExtra(FILE_TITLE, title);
        intent.putExtra(FILE_PATH, path);
        context.startActivity(intent);
    }

    @Override
    public int getContentResId() {
        return R.layout.activity_reader;
    }

    @SuppressLint("SetTextI18n")
    @Override
    public void init() {
        findViewById(R.id.top_bar_back).setVisibility(View.VISIBLE);
        TextView topBarTitle = findViewById(R.id.top_bar_title);
        RelativeLayout readerRelative = findViewById(R.id.reader_relative);
        TextView readerPath = findViewById(R.id.reader_path);
        String fileTitle = getIntent().getStringExtra(FILE_TITLE);
        String filePath = getIntent().getStringExtra(FILE_PATH);
        if (!DataUtil.isEmpty(fileTitle)) {
            topBarTitle.setText(fileTitle);
        }
        if (!DataUtil.isEmpty(filePath)) {
            readerPath.setText("该文件存储路径：" + filePath);
            tbsReaderView = new TbsReaderView(mActivity, this);
            File file = new File(filePath);
            if (file.exists()) {
                RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(
                        ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT
                );
                readerRelative.addView(tbsReaderView, params);
                Bundle bundle = new Bundle();
                bundle.putString(TbsReaderView.KEY_FILE_PATH, filePath);
                bundle.putString(TbsReaderView.KEY_TEMP_PATH, filePath);
                String fileType = getFileType(filePath);
                if (tbsReaderView.preOpen(getFileType(filePath), false)) {
                    tbsReaderView.openFile(bundle);
                }
            }
        }
    }

    /**
     * 获取文件后缀名（不带点）
     */
    @NonNull
    private String getFileType(String fileName) {
        String str = "";
        if (DataUtil.isEmpty(fileName)) {
            return str;
        }
        int i = fileName.lastIndexOf('.');
        if (i <= -1) {
            return str;
        }
        str = fileName.substring(i + 1);
        return str;
    }

    @Override
    public void onCallBackAction(Integer i, Object o1, Object o2) {

    }

    @Override
    public void onClick(@NonNull View v) {
        int id = v.getId();
        if (id == R.id.top_bar_back) {
            finish();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (tbsReaderView != null) {
            tbsReaderView.onStop();
            tbsReaderView.destroyDrawingCache();
        }
    }
}