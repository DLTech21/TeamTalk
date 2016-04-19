package com.mogujie.tt.http.register;

import com.alibaba.fastjson.JSON;
import com.loopj.android.http.RequestParams;
import com.loopj.android.http.TextHttpResponseHandler;
import com.mogujie.tt.http.base.BaseClient;
import com.mogujie.tt.http.base.BaseResponse;
import com.mogujie.tt.utils.Logger;

import org.apache.http.Header;

/**
 * Created by Donal on 16/4/19.
 */
public class RegisterClient {
    private static Logger logger = Logger.getLogger(RegisterClient.class);
    public static void registerUser(String account, String password, String nickname, String avatar, final BaseClient.ClientCallback callback) {
        RequestParams params = new RequestParams();
        callback.onPreConnection();
        params.put("name", account);
        params.put("pass", password);
        params.put("nickname", nickname);
        params.put("avatar", avatar);
        params.put("departId", "1");
        params.put("sex", "1");
        params.put("phone", "");
        params.put("email", "");
        params.put("domain", "0");
        callback.onPreConnection();
        BaseClient.post("/Home/User/registerUser", params, new TextHttpResponseHandler() {
            @Override
            public void onFailure(int i, Header[] headers, String s, Throwable throwable) {
                callback.onCloseConnection();
                callback.onFailure(s);
            }

            @Override
            public void onSuccess(int i, Header[] headers, String s) {
                callback.onCloseConnection();
                try {

                    BaseResponse res = JSON.parseObject(s, BaseResponse.class);
                    callback.onSuccess(res);
                } catch (Exception e) {
                    callback.onException(e);
                }
            }
        });
    }
}
