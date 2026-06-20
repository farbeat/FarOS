package com.lifeteam.farbeat.demo.wheelpicker.feel;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.lifeteam.farbeat.util.system.DensityUtil;

import org.jetbrains.annotations.NotNull;

public class FeelRecyclerView extends RecyclerView {
    private Runnable mSmoothScrollTask;
    private Paint mBgPaint;
    private int mItemHeight;
    private int mItemWidth;
    private int mInitialY;
    private int mFirstLineY;
    private int mSecondLineY;
    private boolean mFirstAmend;
    private int distance;

    public FeelRecyclerView(@NonNull Context context) {
        this(context, null);
    }

    public FeelRecyclerView(@NonNull Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public FeelRecyclerView(@NonNull Context context, @Nullable AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        initTask();
    }

    public void doDraw(Canvas canvas) {
        if (mItemHeight > 0) {
            int screenX = getWidth();
            int startX = screenX / 2 - mItemWidth / 2 - DensityUtil.dp2px(getContext(), 5);
            int stopX = mItemWidth + startX + DensityUtil.dp2px(getContext(), 5);

            canvas.drawLine(startX, mFirstLineY, stopX, mFirstLineY, mBgPaint);
            canvas.drawLine(startX, mSecondLineY, stopX, mSecondLineY, mBgPaint);
        }
    }

    @Override
    public void onDraw(Canvas c) {
        super.onDraw(c);
        doDraw(c);
        if (!mFirstAmend) {
            mFirstAmend = true;
            LinearLayoutManager layoutManager = (LinearLayoutManager) getLayoutManager();
            if (layoutManager != null) {
                layoutManager.scrollToPositionWithOffset(getItemSelectedOffset(), 0);
            }
        }
    }

    @Override
    public void onScrolled(int dx, int dy) {
        super.onScrolled(dx, dy);
//        for (int i = 0; i < getChildCount(); i++) {
//            int h = mItemHeight / 2;
//            float itemViewY = getChildAt(i).getTop() + h;
//            if (mFirstLineY < itemViewY && itemViewY < mSecondLineY) {
//                SoundPools.getInstance(getContext()).play(SoundPools.MUSIC_SCROLL_PICKER);
//            }
//        }
    }

    /*---------------------------------------------------------------------------------*/
    @Override
    public void onScrollStateChanged(int state) {
        if (state == RecyclerView.SCROLL_STATE_IDLE) {
            smoothScrollToView(findViewAtCenter());
            if (distance == 0) {// 解决微调时，连续动2次的问题，避免影响index的定位
                freshItemView();
            }
        }
    }

    public View findViewAtCenter() {
        return findViewAt(getHeight() / 2);
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

    /**
     * 位置微调，把 item 放在2条线内
     *
     * @param v item
     */
    public void smoothScrollToView(View v) {
        if (v == null) {
            return;
        }
        int y = v.getTop() + mItemHeight / 2;
        int halfHeight = getHeight() / 2;
        distance = (y - halfHeight);
        smoothScrollBy(0, distance);
    }

    /*---------------------------------------------------------------------------------*/

    private void initPaint() {
        if (mBgPaint == null) {
            mBgPaint = new Paint();
            mBgPaint.setColor(0xFFFFFFFF);
            mBgPaint.setStrokeWidth(DensityUtil.dp2px(getContext(), 1));
        }
    }

    private int getScrollYDistance() {
        LinearLayoutManager layoutManager = (LinearLayoutManager) this.getLayoutManager();
        if (layoutManager == null) {
            return 0;
        }
        int position = layoutManager.findFirstVisibleItemPosition();
        View firstVisibleChildView = layoutManager.findViewByPosition(position);
        if (firstVisibleChildView == null) {
            return 0;
        }
        int itemHeight = firstVisibleChildView.getHeight();
        return (position) * itemHeight - firstVisibleChildView.getTop();
    }

    @SuppressLint("ClickableViewAccessibility")
    @Override
    public boolean onTouchEvent(@NotNull MotionEvent e) {
        if (e.getAction() == MotionEvent.ACTION_UP) {
            processItemOffset();
        }
        return super.onTouchEvent(e);
    }


    @Override
    protected void onMeasure(int widthSpec, int heightSpec) {

        widthSpec = MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED);
        heightSpec = MeasureSpec.makeMeasureSpec(Integer.MAX_VALUE >> 2, MeasureSpec.AT_MOST);

        super.onMeasure(widthSpec, heightSpec);

        measureSize();

        setMeasuredDimension(mItemWidth, mItemHeight * getVisibleItemNumber());
    }

    private void measureSize() {
        if (getChildCount() > 0) {
            if (mItemHeight == 0) {
                mItemHeight = getChildAt(0).getMeasuredHeight();
            }
            if (mItemWidth == 0) {
                mItemWidth = getChildAt(0).getMeasuredWidth();
            }

            if (mFirstLineY == 0 || mSecondLineY == 0) {
                mFirstLineY = mItemHeight * getItemSelectedOffset();
                mSecondLineY = mItemHeight * (getItemSelectedOffset() + 1);
            }
        }
    }

    private void processItemOffset() {
        mInitialY = getScrollYDistance();
        postDelayed(mSmoothScrollTask, 30);
    }

    private void initTask() {
        mSmoothScrollTask = new Runnable() {
            @Override
            public void run() {
                int newY = getScrollYDistance();
                if (mInitialY != newY) {
                    mInitialY = getScrollYDistance();
                    postDelayed(mSmoothScrollTask, 30);
                } else if (mItemHeight > 0) {
                    final int offset = mInitialY % mItemHeight;//离选中区域中心的偏移量
                    if (offset == 0) {
                        return;
                    }
                    if (offset >= mItemHeight / 2) {//滚动区域超过了item高度的1/2，调整position的值
                        smoothScrollBy(0, mItemHeight - offset);
                    } else if (offset < mItemHeight / 2) {
                        smoothScrollBy(0, -offset);
                    }
                }
            }
        };
    }


    private int getVisibleItemNumber() {
        IFeelOperation operation = (IFeelOperation) getAdapter();
        if (operation != null) {
            return operation.getVisibleItemNumber();
        }
        return 3;
    }

    private int getItemSelectedOffset() {
        IFeelOperation operation = (IFeelOperation) getAdapter();
        if (operation != null) {
            return operation.getSelectedItemOffset();
        }
        return 1;
    }

    private void freshItemView() {
        for (int i = 0; i < getChildCount(); i++) {
            int h = mItemHeight / 2;
            float itemViewY = getChildAt(i).getTop() + h;
            updateView(getChildAt(i), mFirstLineY < itemViewY && itemViewY < mSecondLineY);
        }
    }

    private void updateView(View itemView, boolean isSelected) {
        IFeelOperation operation = (IFeelOperation) getAdapter();
        if (operation != null) {
            operation.updateView(itemView, isSelected);
        }
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        initPaint();
    }
}