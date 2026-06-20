package com.lifeteam.farbeat.demo.cookies;

import android.content.Context;
import android.content.SharedPreferences;
import android.text.TextUtils;

import org.jetbrains.annotations.NotNull;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.Objects;
import java.util.concurrent.ConcurrentHashMap;

import okhttp3.Cookie;
import okhttp3.HttpUrl;

public class PersistentCookieStore {

    private final Map<String, ConcurrentHashMap<String, Cookie>> cookies;
    private final SharedPreferences cookiePrefs;

    public PersistentCookieStore(Context context) {
        cookiePrefs = context.getSharedPreferences("Cookies_Prefs", 0);
        cookies = new HashMap<>();

        //将持久化的cookies缓存到内存中 即map cookies
        Map<String, ?> prefsMap = cookiePrefs.getAll();
        for (Map.Entry<String, ?> entry : prefsMap.entrySet()) {
            String[] cookieNames = TextUtils.split((String) entry.getValue(), ",");
            for (String name : cookieNames) {
                String encodedCookie = cookiePrefs.getString(name, null);
                if (encodedCookie != null) {
                    Cookie decodedCookie = decodeCookie(encodedCookie);
                    if (decodedCookie != null) {
                        if (!cookies.containsKey(entry.getKey())) {
                            cookies.put(entry.getKey(), new ConcurrentHashMap<String, Cookie>());
                        }
                        ConcurrentHashMap<String, Cookie> cookieMap = cookies.get(entry.getKey());
                        if (cookieMap != null) {
                            cookieMap.put(name, decodedCookie);
                        }
                    }
                }
            }
        }
    }

    protected String getCookieToken(@NotNull Cookie cookie) {
        return cookie.name() + "@" + cookie.domain();
    }

    public void add(HttpUrl url, Cookie cookie) {
        String name = getCookieToken(cookie);

        //将cookies缓存到内存中 如果缓存过期 就重置此cookie
        if (!cookie.persistent()) {
            if (!cookies.containsKey(url.host())) {
                cookies.put(url.host(), new ConcurrentHashMap<String, Cookie>());
            }
            ConcurrentHashMap<String, Cookie> cookieMap = cookies.get(url.host());
            if (cookieMap != null) {
                cookieMap.put(name, cookie);
            }
        } else {
            if (cookies.containsKey(url.host())) {
                ConcurrentHashMap<String, Cookie> cookieMap = cookies.get(url.host());
                if (cookieMap != null) {
                    cookieMap.remove(name);
                }
            }
        }

        //将cookies持久化到本地
        SharedPreferences.Editor prefsWriter = cookiePrefs.edit();
        ConcurrentHashMap<String, Cookie> cookieMap = cookies.get(url.host());
        String hostValue = "";
        if (cookieMap != null) {
            hostValue = TextUtils.join(",", cookieMap.keySet());
        }
        prefsWriter.putString(url.host(), hostValue);
        prefsWriter.putString(name, encodeCookie(new OkHttpCookies(cookie)));
        prefsWriter.apply();
    }

    public List<Cookie> get(@NotNull HttpUrl url) {
        ArrayList<Cookie> ret = new ArrayList<>();
        if (cookies.containsKey(url.host())) {
            ConcurrentHashMap<String, Cookie> cookieMap = cookies.get(url.host());
            if (cookieMap != null) {
                ret.addAll(cookieMap.values());
            }
        }
        return ret;
    }

    public boolean removeAll() {
        SharedPreferences.Editor prefsWriter = cookiePrefs.edit();
        prefsWriter.clear();
        prefsWriter.apply();
        cookies.clear();
        return true;
    }

    public boolean remove(@NotNull HttpUrl url, Cookie cookie) {
        String name = getCookieToken(cookie);

        if (cookies.containsKey(url.host()) && Objects.requireNonNull(cookies.get(url.host())).containsKey(name)) {
            Objects.requireNonNull(cookies.get(url.host())).remove(name);

            SharedPreferences.Editor prefsWriter = cookiePrefs.edit();
            if (cookiePrefs.contains(name)) {
                prefsWriter.remove(name);
            }
            prefsWriter.putString(url.host(), TextUtils.join(",", Objects.requireNonNull(cookies.get(url.host())).keySet()));
            prefsWriter.apply();

            return true;
        } else {
            return false;
        }
    }

    public List<Cookie> getCookies() {
        ArrayList<Cookie> ret = new ArrayList<>();
        for (String key : cookies.keySet()) {
            ConcurrentHashMap<String, Cookie> cookieMap = cookies.get(key);
            if (cookieMap != null) {
                ret.addAll(cookieMap.values());
            }
        }
        return ret;
    }

    /**
     * cookies 序列化成 string
     *
     * @param cookie 要序列化的cookie
     * @return 序列化之后的string
     */
    protected String encodeCookie(OkHttpCookies cookie) {
        if (cookie == null)
            return null;
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        try {
            ObjectOutputStream outputStream = new ObjectOutputStream(os);
            outputStream.writeObject(cookie);
        } catch (IOException e) {
            // IOException in encodeCookie
            return null;
        }

        return byteArrayToHexString(os.toByteArray());
    }

    /**
     * 将字符串反序列化成cookies
     *
     * @param cookieString cookies string
     * @return cookie object
     */
    protected Cookie decodeCookie(String cookieString) {
        byte[] bytes = hexStringToByteArray(cookieString);
        ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(bytes);
        Cookie cookie = null;
        try {
            ObjectInputStream objectInputStream = new ObjectInputStream(byteArrayInputStream);
            cookie = ((OkHttpCookies) objectInputStream.readObject()).getCookies();
        } catch (IOException e) {
            // IOException in decodeCookie
        } catch (ClassNotFoundException e) {
            // ClassNotFoundException in decodeCookie
        }

        return cookie;
    }

    /**
     * 二进制数组转十六进制字符串
     *
     * @param bytes byte array to be converted
     * @return string containing hex values
     */
    protected String byteArrayToHexString(@NotNull byte[] bytes) {
        StringBuilder sb = new StringBuilder(bytes.length * 2);
        for (byte element : bytes) {
            int v = element & 0xff;
            if (v < 16) {
                sb.append('0');
            }
            sb.append(Integer.toHexString(v));
        }
        return sb.toString().toUpperCase(Locale.US);
    }

    /**
     * 十六进制字符串转二进制数组
     *
     * @param hexString string of hex-encoded values
     * @return decoded byte array
     */
    protected byte[] hexStringToByteArray(@NotNull String hexString) {
        int len = hexString.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(hexString.charAt(i), 16) << 4) + Character.digit(hexString.charAt(i + 1), 16));
        }
        return data;
    }
}
