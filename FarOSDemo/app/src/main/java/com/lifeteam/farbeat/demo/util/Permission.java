package com.lifeteam.farbeat.demo.util;

import android.Manifest;
import android.os.Build;

public class Permission {

    public static final int R30 = 30;

    public static final String[] PERMS = {
            Manifest.permission.READ_PHONE_STATE,

            Manifest.permission.CAMERA,
            Manifest.permission.VIBRATE,

            Build.VERSION.SDK_INT >= Build.VERSION_CODES.S ? Manifest.permission.BLUETOOTH_SCAN : Manifest.permission.BLUETOOTH,
            Build.VERSION.SDK_INT >= Build.VERSION_CODES.S ? Manifest.permission.BLUETOOTH_CONNECT : Manifest.permission.BLUETOOTH,

            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE,

            Manifest.permission.ACCESS_COARSE_LOCATION,
            Manifest.permission.ACCESS_FINE_LOCATION,
    };
}