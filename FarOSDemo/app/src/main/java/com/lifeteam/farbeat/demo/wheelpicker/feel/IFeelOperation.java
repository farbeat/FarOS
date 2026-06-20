package com.lifeteam.farbeat.demo.wheelpicker.feel;

import android.view.View;

public interface IFeelOperation {
    int getSelectedItemOffset();

    int getVisibleItemNumber();

    void updateView(View itemView, boolean isSelected);
}
