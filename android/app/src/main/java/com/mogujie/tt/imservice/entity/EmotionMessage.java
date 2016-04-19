package com.mogujie.tt.imservice.entity;

import com.mogujie.tt.DB.entity.MessageEntity;
import com.mogujie.tt.DB.entity.PeerEntity;
import com.mogujie.tt.DB.entity.UserEntity;
import com.mogujie.tt.config.DBConstant;
import com.mogujie.tt.config.MessageConstant;
import com.mogujie.tt.imservice.support.SequenceNumberMaker;

import java.io.Serializable;
import java.io.UnsupportedEncodingException;

/**
 * Created by Donal on 16/3/30.
 */
public class EmotionMessage extends MessageEntity implements Serializable {
    public EmotionMessage(){
        msgId = SequenceNumberMaker.getInstance().makelocalUniqueMsgId();
    }

    private EmotionMessage(MessageEntity entity){
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

    public static EmotionMessage parseFromNet(MessageEntity entity){
        EmotionMessage emoMessage = new EmotionMessage(entity);
        emoMessage.setStatus(MessageConstant.MSG_SUCCESS);
        emoMessage.setDisplayType(DBConstant.SHOW_GIF_TYPE);
        return emoMessage;
    }

    public static EmotionMessage parseFromDB(MessageEntity entity){
        if(entity.getDisplayType()!=DBConstant.SHOW_GIF_TYPE){
            throw new RuntimeException("#TextMessage# parseFromDB,not SHOW_GIF_TYPE");
        }
        EmotionMessage emoMessage = new EmotionMessage(entity);
        return emoMessage;
    }

    public static EmotionMessage buildForSend(String content,UserEntity fromUser,PeerEntity peerEntity){
        EmotionMessage emoMessage = new EmotionMessage();
        int nowTime = (int) (System.currentTimeMillis() / 1000);
        emoMessage.setFromId(fromUser.getPeerId());
        emoMessage.setToId(peerEntity.getPeerId());
        emoMessage.setUpdated(nowTime);
        emoMessage.setCreated(nowTime);
        emoMessage.setDisplayType(DBConstant.SHOW_GIF_TYPE);
        emoMessage.setGIfEmo(true);
        int peerType = peerEntity.getType();
        int msgType = peerType == DBConstant.SESSION_TYPE_GROUP ? DBConstant.MSG_TYPE_GROUP_EMOTION
                : DBConstant.MSG_TYPE_SINGLE_EMOTION;
        emoMessage.setMsgType(msgType);
        emoMessage.setStatus(MessageConstant.MSG_SENDING);
        // 内容的设定
        emoMessage.setContent(content);
        emoMessage.buildSessionKey(true);
        return emoMessage;
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
