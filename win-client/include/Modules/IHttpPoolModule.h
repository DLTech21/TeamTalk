/*******************************************************************************
 *  @file      IHttpPoolModule.h 2014\7\25 11:18:16 $
 *  @author    �쵶<kuaidao@mogujie.com>
 *  @brief   һ���򵥵�HTTP���س�
 ******************************************************************************/

#ifndef IHTTPPOOLMODULE_3ABB1077_8BD7_44DC_90B6_FEB35B03F6CF_H__
#define IHTTPPOOLMODULE_3ABB1077_8BD7_44DC_90B6_FEB35B03F6CF_H__

#include "GlobalDefine.h"
#include "Modules/ModuleDll.h"
#include "Modules/ModuleBase.h"
#include "Modules/IModuleInterface.h"
/******************************************************************************/
NAMESPACE_BEGIN(module)

struct MODULE_API IHttpOperation : public ICallbackOpertaion
{
public:
	IHttpOperation(IOperationDelegate& callback)
		:ICallbackOpertaion(callback)
	{

	}
	inline void cancel() { m_bIsCancel = TRUE; }
	inline BOOL isCanceled() const { return m_bIsCancel; }

	virtual void release() = 0;

private:
	BOOL		m_bIsCancel = FALSE;
};

const std::string MODULE_HTTPPOOL_PREFIX = "httppool";
//KEYID

const std::string KEY_HTTPPOOL_CALLBACK = MODULE_HTTPPOOL_PREFIX + "callback";

/**
 * The class <code>IHttpPoolModule</code> 
 *
 */
class MODULE_API IHttpPoolModule : public module::ModuleBase
{
public:
	/**
	* ���http�������̳߳�
	*
	* @param   IHttpOperation	 * pOperaion    ����Ԫ
	* @param   BOOL bHighPriority       ������һ���򵥵����ȼ�����ʱ����������
	* @return
	* @exception there is no any exception to throw.
	*/
	virtual void    pushHttpOperation(IHttpOperation * pOperaion, BOOL bHighPriority = FALSE) = 0;
	/**
	* �ر��̹߳�����
	*
	* @param
	* @return
	* @exception there is no any exception to throw.
	*/
	virtual void    shutdown() = 0;
};

MODULE_API IHttpPoolModule* getHttpPoolModule();

NAMESPACE_END(module)
/******************************************************************************/
#endif// IHTTPPOOLMODULE_3abb1077-8bd7-44dc-90b6-feb35b03f6cf_H__
