package com.mogujie.tt.ui.activity;

import android.os.Bundle;
import android.text.TextUtils;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.mogujie.tt.R;
import com.mogujie.tt.app.IMApplication;
import com.mogujie.tt.http.base.BaseClient;
import com.mogujie.tt.http.base.BaseResponse;
import com.mogujie.tt.http.register.RegisterClient;
import com.mogujie.tt.ui.base.TTBaseActivity;
import com.mogujie.tt.utils.Logger;
import com.mogujie.tt.utils.ThemeUtils;
import com.mogujie.tt.utils.ViewUtils;

/**
 * Created by Donal on 16/4/19.
 */
public class RegisterActivity extends TTBaseActivity implements View.OnClickListener{
    private EditText mNameView;
    private EditText mNicknameView;
    private EditText mPasswordView;
    private View mLoginStatusView;
    private View loginPage;
    private InputMethodManager intputManager;
    private Logger logger = Logger.getLogger(RegisterActivity.class);
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LayoutInflater.from(this).inflate(R.layout.tt_activity_register, topContentView);
        intputManager = (InputMethodManager) getSystemService(this.INPUT_METHOD_SERVICE);
        initView();
    }

    private void initView() {
        setLeftButton(R.drawable.tt_top_back);
        setLeftText(getResources().getString(R.string.top_left_back));
        setTitle("注册");
        topLeftBtn.setOnClickListener(this);
        letTitleTxt.setOnClickListener(this);
        topRightBtn.setOnClickListener(this);
        mNameView = (EditText) findViewById(R.id.name);
        mNicknameView = (EditText) findViewById(R.id.nickname);
        mPasswordView = (EditText) findViewById(R.id.password);
        mPasswordView.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {

                if (id == R.id.login || id == EditorInfo.IME_NULL) {
                    doRegist();
                    return true;
                }
                return false;
            }
        });
        mLoginStatusView = findViewById(R.id.register_status);
        loginPage = findViewById(R.id.login_page);
        loginPage.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View v, MotionEvent event) {

                if (mPasswordView != null) {
                    intputManager.hideSoftInputFromWindow(mPasswordView.getWindowToken(), 0);
                }

                if (mNameView != null) {
                    intputManager.hideSoftInputFromWindow(mNameView.getWindowToken(), 0);
                }

                if (mNicknameView != null) {
                    intputManager.hideSoftInputFromWindow(mNicknameView.getWindowToken(), 0);
                }

                return false;
            }
        });
    }

    @Override
    public void onClick(View v) {
        final int id = v.getId();
        switch (id) {
            case R.id.left_btn:
            case R.id.left_txt:
                RegisterActivity.this.finish();
                break;
            case R.id.sign_in_button:
                doRegist();
                break;
            default:
                break;
        }
    }

    private void showProgress(boolean show) {
        if (show) {
            mLoginStatusView.setVisibility(View.VISIBLE);
        } else {
            mLoginStatusView.setVisibility(View.GONE);
        }
    }

    private void doRegist() {
        String loginName = mNameView.getText().toString();
        String mPassword = mPasswordView.getText().toString();
        String mNickname = mNicknameView.getText().toString();
        if (TextUtils.isEmpty(mPassword)) {
            Toast.makeText(this, getString(R.string.error_pwd_required), Toast.LENGTH_SHORT).show();
            return;
        }

        if (TextUtils.isEmpty(loginName)) {
            Toast.makeText(this, getString(R.string.error_name_required), Toast.LENGTH_SHORT).show();
            return;
        }

        if (TextUtils.isEmpty(mNickname)) {
            Toast.makeText(this, getString(R.string.error_name_required), Toast.LENGTH_SHORT).show();
            return;
        }
        RegisterClient.registerUser(loginName, mPassword, mNickname, "", new BaseClient.ClientCallback() {
            @Override
            public void onPreConnection() {
                ViewUtils.createProgressDialog(getRunningActivity(), "正在注册...", ThemeUtils.getThemeColor()).show();
            }

            @Override
            public void onCloseConnection() {
                ViewUtils.dismissProgressDialog();
            }

            @Override
            public void onSuccess(Object data) {
                BaseResponse res = (BaseResponse)data;
                if (res.getStatus() == 0) {
                    ViewUtils.showMessage("注册成功");
                    RegisterActivity.this.finish();
                }
                else {
                    Toast.makeText(RegisterActivity.this, res.getMsg(), Toast.LENGTH_SHORT).show();
                }
            }

            @Override
            public void onFailure(String message) {
                Toast.makeText(RegisterActivity.this, message, Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onException(Exception e) {
                Toast.makeText(RegisterActivity.this, e.getMessage(), Toast.LENGTH_SHORT).show();
            }
        });
    }
}
