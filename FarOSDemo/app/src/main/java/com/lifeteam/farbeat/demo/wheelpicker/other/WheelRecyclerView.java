package com.lifeteam.farbeat.demo.wheelpicker.other;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Shader;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.widget.LinearLayout;

import androidx.annotation.Nullable;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.lifeteam.farbeat.util.system.ScreenUtil;

public class WheelRecyclerView extends RecyclerView {

    private Paint paint;
    private int firstLineY;
    private int secondLineY;

    private ScrollListener scrollListener;
    private int itemHeight = 36;
    private int distance;
    private int visibleCount = 3;
    private int wheelWidthWeight = 1;
    private LinearLayout parent;
    private Rect mRect;
    private Paint paint1;

    public WheelRecyclerView(Context context) {
        this(context, null);
    }

    public WheelRecyclerView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init(attrs);
    }

    private void init(AttributeSet attrs) {
        paint = new Paint();
        paint1 = new Paint();
        paint.setColor(Color.BLACK);
        paint.setStrokeWidth(1);// 线宽 1px

        setClipToPadding(false);// 滚动不裁剪，不显示顶部的padding
        setClipChildren(false);

        setPadding(0, itemHeight * (visibleCount / 2), 0, itemHeight * (visibleCount / 2));
        firstLineY = (visibleCount / 2) * itemHeight;
        secondLineY = (visibleCount / 2 + 1) * itemHeight;

        mRect = new Rect();
    }

    @Override
    public void onScrollStateChanged(int state) {
        if (state == RecyclerView.SCROLL_STATE_IDLE) {
            smoothScrollToView(findViewAtCenter());
            if (scrollListener != null && distance == 0) {// 解决微调时，连续动2次的问题，避免影响index的定位
                scrollListener.scrollChanged(state);
            }
        }
    }

    @Override
    public void draw(Canvas canvas) {
        super.draw(canvas);

        mRect.set(0, 0, getWidth(), getHeight());
        LinearGradient lg = new LinearGradient(0, 0, 0, getHeight() / 2, Color.parseColor("#ddffffff"), Color.parseColor("#00ffffff"), Shader.TileMode.MIRROR);

        paint1.setShader(lg);
        canvas.drawRect(mRect, paint1);
    }

    @Override
    protected void onMeasure(int widthSpec, int heightSpec) {
        super.onMeasure(widthSpec, heightSpec);

        // 将宽度 按照 宽度的权重进行设置
        try {
            parent = (LinearLayout) getParent();
        } catch (Exception e) {
            Log.e("error", e.toString());
        }
        float count;
        if (parent != null) {
            count = parent.getChildCount();
        } else {
            count = 1;
        }
        int screenW = ScreenUtil.getScreenW(getContext());

        // 将高度按照 item 的高度定制
        int resultWidth = (int) (wheelWidthWeight / count * screenW);
        int resultHeight = itemHeight * visibleCount;
        setMeasuredDimension(resultWidth, resultHeight);
    }

    /**
     * 位置微调，把 item 放在2条线内
     *
     * @param v item
     */
    public void smoothScrollToView(View v) {
        if (v == null) {
            return;
        }
        int y = v.getTop() + itemHeight / 2;
        int halfHeight = getHeight() / 2;
        distance = (y - halfHeight);
        smoothScrollBy(0, distance);
    }

    /**
     * 寻找中间位置的 item
     */
    public View findViewAt(int y) {
        final int count = getChildCount();
        for (int i = 0; i < count; ++i) {
            final View v = getChildAt(i);
            final int y0 = v.getTop();
            final int y1 = v.getHeight() + y0;
            if (y >= y0 && y <= y1) {
                return v;
            }
        }
        return null;
    }

    public View findViewAtCenter() {
        return findViewAt(getHeight() / 2);
    }

    public interface ScrollListener {
        void scrollChanged(int state);
    }

    public void setScrollListener(ScrollListener listener) {
        this.scrollListener = listener;
    }

    public void moveToPosition(int position) {
        LinearLayoutManager lm = (LinearLayoutManager) getLayoutManager();
        if (lm != null) {
            lm.scrollToPosition(position);
        }
    }
}
