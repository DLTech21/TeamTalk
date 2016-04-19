package com.mogujie.tt.imservice.entity;

import com.google.gson.Gson;
import com.mogujie.tt.DB.entity.MessageEntity;
import com.mogujie.tt.DB.entity.PeerEntity;
import com.mogujie.tt.DB.entity.UserEntity;
import com.mogujie.tt.config.DBConstant;
import com.mogujie.tt.config.MessageConstant;
import com.mogujie.tt.imservice.support.SequenceNumberMaker;

import java.io.Serializable;
import java.io.UnsupportedEncodingException;

/**
 * Created by Donal on 16/3/31.
 */
public class LocationMessage extends MessageEntity implements Serializable {
    public LocationMessage(){
        msgId = SequenceNumberMaker.getInstance().makelocalUniqueMsgId();
    }

    private LocationMessage(MessageEntity entity){
        /**父类的id*/
        id =  entity.getId();
        msgId  = entity.getMsgId();
        fromId = entity.getFromId();
        toId   = entity.getToId();
        sessionKey = entity.getSessionKey();
        content=entity.getContent();
        msgType=entity.getMsgType();
        displayType=entity.getDisplayType();
        status = entity.getStatus();
        created = entity.getCreated();
        updated = entity.getUpdated();
    }

    public static LocationMessage parseFromNet(MessageEntity entity){
        LocationMessage locationMessage = new LocationMessage(entity);
        locationMessage.setStatus(MessageConstant.MSG_SUCCESS);
        locationMessage.setDisplayType(DBConstant.SHOW_LOCATION_TYPE);
        return locationMessage;
    }

    public static LocationMessage parseFromDB(MessageEntity entity){
        if(entity.getDisplayType()!=DBConstant.SHOW_LOCATION_TYPE){
            throw new RuntimeException("#TextMessage# parseFromDB,not SHOW_LOCATION_TYPE");
        }
        LocationMessage locationMessage = new LocationMessage(entity);
        return locationMessage;
    }

    public static LocationMessage buildForSend(String address, double lat, double lng, UserEntity fromUser,PeerEntity peerEntity){
        LocationMessage locationMessage = new LocationMessage();
        int nowTime = (int) (System.currentTimeMillis() / 1000);
        locationMessage.setFromId(fromUser.getPeerId());
        locationMessage.setToId(peerEntity.getPeerId());
        locationMessage.setUpdated(nowTime);
        locationMessage.setCreated(nowTime);
        locationMessage.setDisplayType(DBConstant.SHOW_LOCATION_TYPE);
        locationMessage.setGIfEmo(true);
        int peerType = peerEntity.getType();
        int msgType = peerType == DBConstant.SESSION_TYPE_GROUP ? DBConstant.MSG_TYPE_GROUP_LOCATION
                : DBConstant.MSG_TYPE_SINGLE_LOCATION;
        locationMessage.setMsgType(msgType);
        locationMessage.setStatus(MessageConstant.MSG_SENDING);
        // 内容的设定
        LocationEntity locationEntity = new LocationEntity();
        locationEntity.setAddress(address);
        locationEntity.setLat(lat + "");
        locationEntity.setLng(lng+"");
        locationMessage.setContent(new Gson().toJson(locationEntity));
        locationMessage.buildSessionKey(true);
        return locationMessage;
    }


    /**
     * Not-null value.
     * DB的时候需要
     */
    @Override
    public String getContent() {
        return content;
    }

    @Override
    public byte[] getSendContent() {
        try {
            /** 加密*/
            String sendContent =new String(com.mogujie.tt.Security.getInstance().EncryptMsg(content));
            return sendContent.getBytes("utf-8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        return null;
    }
}
