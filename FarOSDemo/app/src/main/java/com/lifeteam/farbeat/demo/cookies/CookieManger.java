package com.lifeteam.farbeat.demo.cookies;

import android.content.Context;

import org.jetbrains.annotations.NotNull;

import java.util.List;

import okhttp3.Cookie;
import okhttp3.CookieJar;
import okhttp3.HttpUrl;

public class CookieManger implements CookieJar {

    public static String APP_PLATFORM = "sdk-platform";

    private static PersistentCookieStore cookieStore;

    public CookieManger(Context context) {
        if (cookieStore == null) {
            cookieStore = new PersistentCookieStore(context);
        }
    }

    @Override
    public void saveFromResponse(@NotNull HttpUrl url, @NotNull List<Cookie> cookies) {
        if (cookies.size() > 0) {
            for (Cookie item : cookies) {
                cookieStore.add(url, item);
            }
        }
    }

    @NotNull
    @Override
    public List<Cookie> loadForRequest(@NotNull HttpUrl url) {
        return cookieStore.get(url);
    }

    static class Customer {

        private String userID;
        private String token;

        public Customer(String userID, String token) {
            this.userID = userID;
            this.token = token;
        }

        public String getUserID() {
            return userID;
        }

        public void setUserID(String userID) {
            this.userID = userID;
        }

        public String getToken() {
            return token;
        }

        public void setToken(String token) {
            this.token = token;
        }
    }
}