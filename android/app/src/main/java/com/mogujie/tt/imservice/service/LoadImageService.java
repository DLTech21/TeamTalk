package com.mogujie.tt.imservice.service;

import android.app.IntentService;
import android.content.Intent;
import android.graphics.Bitmap;
import android.text.TextUtils;

import com.mogujie.tt.DB.sp.SystemConfigSp;
import com.mogujie.tt.config.SysConstant;
import com.mogujie.tt.config.UrlConstant;
import com.mogujie.tt.imservice.entity.ImageMessage;
import com.mogujie.tt.imservice.event.MessageEvent;
import com.mogujie.tt.ui.helper.PhotoHelper;
import com.mogujie.tt.utils.FileUtil;
import com.mogujie.tt.utils.MoGuHttpClient;
import com.mogujie.tt.utils.Logger;

import java.io.File;
import java.io.IOException;

import de.greenrobot.event.EventBus;

/**
 * @author : yingmu on 15-1-12.
 * @email : yingmu@mogujie.com.
 *
 */
public class LoadImageService extends IntentService {

    private static Logger logger = Logger.getLogger(LoadImageService.class);

    public LoadImageService(){
        super("LoadImageService");
    }

    public LoadImageService(String name) {
        super(name);
    }

    /**
     * This method is invoked on the worker thread with a request to process.
     * Only one Intent is processed at a time, but the processing happens on a
     * worker thread that runs independently from other application logic.
     * So, if this code takes a long time, it will hold up other requests to
     * the same IntentService, but it will not hold up anything else.
     * When all requests have been handled, the IntentService stops itself,
     * so you should not call {@link #stopSelf}.
     *
     * @param intent The value passed to {@link
     *               android.content.Context#startService(android.content.Intent)}.
     */
    @Override
    protected void onHandleIntent(Intent intent) {
        ImageMessage messageInfo = (ImageMessage)intent.getSerializableExtra(SysConstant.UPLOAD_IMAGE_INTENT_PARAMS);
            String result = null;
            Bitmap bitmap;
            try {
                File file= new File(messageInfo.getPath());
                if(file.exists() && FileUtil.getExtensionName(messageInfo.getPath()).toLowerCase().equals(".gif"))
                {
                    MoGuHttpClient httpClient = new MoGuHttpClient();
                    SystemConfigSp.instance().init(getApplicationContext());
                    result = httpClient.uploadImage3(SystemConfigSp.instance().getStrConfig(SystemConfigSp.SysCfgDimension.MSFSSERVER), FileUtil.File2byte(messageInfo.getPath()), messageInfo.getPath());
                }
                else
                {
                    bitmap = PhotoHelper.revitionImage(messageInfo.getPath());
                    if (null != bitmap) {
                        MoGuHttpClient httpClient = new MoGuHttpClient();
                        byte[] bytes = PhotoHelper.getBytes(bitmap);
                        result = httpClient.uploadImage3(SystemConfigSp.instance().getStrConfig(SystemConfigSp.SysCfgDimension.MSFSSERVER), bytes, messageInfo.getPath());
                    }
                }

                if (TextUtils.isEmpty(result)) {
                    logger.i("upload image faild,cause by result is empty/null");
                    EventBus.getDefault().post(new MessageEvent(MessageEvent.Event.IMAGE_UPLOAD_FAILD
                    ,messageInfo));
                } else {
                    logger.i("upload image succcess,imageUrl is %s",result);
                    String imageUrl = result;
                    messageInfo.setUrl(imageUrl);
                    EventBus.getDefault().post(new MessageEvent(
                            MessageEvent.Event.IMAGE_UPLOAD_SUCCESS
                            ,messageInfo));
                }
            } catch (IOException e) {
                logger.e(e.getMessage());
            }
    }
}
