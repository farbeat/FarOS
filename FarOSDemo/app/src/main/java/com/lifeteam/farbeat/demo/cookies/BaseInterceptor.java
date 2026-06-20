package com.lifeteam.farbeat.demo.cookies;

import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.util.Map;
import java.util.Set;

import okhttp3.Interceptor;
import okhttp3.Request;
import okhttp3.Response;

public class BaseInterceptor implements Interceptor {

    private final Map<String, String> headers;

    public BaseInterceptor(Map<String, String> map) {
        headers = map;
    }

    @Override
    public Response intercept(@NotNull Chain chain) throws IOException {
        Request.Builder builder = chain.request().newBuilder();
        if (headers != null && headers.size() > 0) {
            Set<String> keys = headers.keySet();
            for (String headerKey : keys) {
                String value = headers.get(headerKey);
                if (value != null) {
                    builder.addHeader(headerKey, value).build();
                }
            }
        }
        return chain.proceed(builder.build());
    }
}