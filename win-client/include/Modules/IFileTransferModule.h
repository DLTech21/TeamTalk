/*******************************************************************************
 *  @file      IFileTransfer.h 2014\8\26 11:51:55 $
 *  @author    �쵶<kuaidao@mogujie.com>
 *  @brief     
 ******************************************************************************/

#ifndef IFILETRANSFER_425BDB8D_221E_4952_93C5_4362AF5217CB_H__
#define IFILETRANSFER_425BDB8D_221E_4952_93C5_4362AF5217CB_H__

#include "GlobalDefine.h"
#include "Modules/IModuleInterface.h"
#include "Modules/ModuleDll.h"
#include "Modules/IMiscModule.h"
#include "Modules/ModuleBase.h"
#include "utility/utilCommonAPI.h"
#include "utility/utilStrCodingAPI.h"
/******************************************************************************/
NAMESPACE_BEGIN(module)

const std::string FILETRANSFER_IP = "122.225.68.125";
const UInt16 FILETRANSFER_PORT = 29800;
const std::string MODULE_FILETRAS_PREFIX = "FileTransfer";

//KEYID
const std::string KEY_FILETRANSFER_SENDFILE		= MODULE_FILETRAS_PREFIX + "SendFile";	//�����ļ�
const std::string KEY_FILETRANSFER_REQUEST		= MODULE_FILETRAS_PREFIX + "Request";   //�����ļ�����-���շ�
const std::string KEY_FILETRANSFER_RESPONSE		= MODULE_FILETRAS_PREFIX + "Response";  //�����ļ����󷵻�-���ͷ�
const std::string KEY_FILESEVER_UPLOAD_OFFLINE_FINISH	= MODULE_FILETRAS_PREFIX + "UploadOfflineFinish";	//�����ļ����䵽�ļ����������
const std::string KEY_FILESEVER_UPDATA_PROGRESSBAR		= MODULE_FILETRAS_PREFIX + "UploadProgressBar";		//�����ļ����������
const std::string KEY_FILESEVER_PROGRESSBAR_FINISHED	= MODULE_FILETRAS_PREFIX + "ProgressBarFinished";	//�������
const std::string KEY_FILESEVER_UPLOAD_FAILED	= MODULE_FILETRAS_PREFIX + "UploadFailed";		//����ʧ��
const std::string KEY_FILESEVER_UPLOAD_CANCEL	= MODULE_FILETRAS_PREFIX + "UploadCancel";		//����ȡ��
const std::string KEY_FILESEVER_UPLOAD_REJECT	= MODULE_FILETRAS_PREFIX + "UploadReject";		//�ܾ�����

/**
 * The class <code>IFileTransfer</code> 
 *
 */
class IFileTransferModule : public module::ModuleBase
						   ,public module::IPduPacketParse
{
public:
	virtual BOOL sendFile(IN const CString& sFilePath, IN const std::string& sSendToSID, IN BOOL bOnlineMode) = 0;
	virtual BOOL acceptFileTransfer(IN const std::string& sTaskId, IN const BOOL bAccept = TRUE) = 0;
	virtual BOOL doCancel(IN const std::string& sFileID) = 0;
	virtual void showFileTransferDialog() = 0;
};

MODULE_API IFileTransferModule* getFileTransferModule();

NAMESPACE_END(module)
/******************************************************************************/
#endif// IFILETRANSFER_425BDB8D_221E_4952_93C5_4362AF5217CB_H__
