package com.mogujie.tt.http.base;

import android.content.Context;

import com.loopj.android.http.AsyncHttpClient;
import com.loopj.android.http.AsyncHttpResponseHandler;
import com.loopj.android.http.RequestParams;
import com.loopj.android.http.SyncHttpClient;

/**
 * Created by Donal on 16/4/19.
 */
public class BaseClient {


    public final static String BASE_API = "http://115.159.211.199";

    public static AsyncHttpClient client = new AsyncHttpClient();

    public static AsyncHttpClient getInstance() {
        return client;
    }

    public static void get(String url, RequestParams params, AsyncHttpResponseHandler responseHandler) {
        client.get(getAbsoluteUrl(url), params, responseHandler);
    }

    public static void post(String url, RequestParams params, AsyncHttpResponseHandler responseHandler) {
        client.post(getAbsoluteUrl(url), params, responseHandler);
    }

    public static void post(Context context, String url, RequestParams params, AsyncHttpResponseHandler responseHandler) {
        client.post(context, getAbsoluteUrl(url), params, responseHandler);
    }

    private static String getAbsoluteUrl(String relativeUrl) {
        client.setTimeout(60*1000);
        client.setMaxConnections(8);
        if (relativeUrl.contains("http")) {
            return relativeUrl;
        }
        else {
            return BASE_API + relativeUrl;
        }
    }

    public interface ClientCallback
    {
        abstract void onPreConnection();

        abstract void onCloseConnection();

        /**
         * 返回api有效数据
         *
         * @param data
         */
        abstract void onSuccess(Object data);

        /**
         * 连接api失败
         *
         * @param message
         */
        abstract void onFailure(String message);

        /**
         * 返回解析json等异常
         *
         * @param e
         */
        abstract void onException(Exception e);
    }
}
