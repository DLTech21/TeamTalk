/******************************************************************************* 
 *  @file      UIUserList.cpp 2014\7\17 13:07:19 $
 *  @author    ���<dafo@mogujie.com>
 *  @brief   
 ******************************************************************************/

#include "stdafx.h"
#include "Modules/UI/UIRecentSessionList.h"
#include "Modules/IMiscModule.h"
#include "Modules/IUserListModule.h"
#include "Modules/IGroupListModule.h"
#include "Modules/IMessageModule.h"
#include "Modules/ISysConfigModule.h"
#include "utility/utilStrCodingAPI.h"
#include "utility/Multilingual.h"
#include "../SessionManager.h"
#include "../../Message/ReceiveMsgManage.h"


/******************************************************************************/

// -----------------------------------------------------------------------------
//  UIUserList: Public, Constructor
const int kIMListItemNormalHeight = 60;

const TCHAR* const kLogoButtonControlName = _T("logo");
const TCHAR* const kLogoContainerControlName = _T("logo_container");
const TCHAR* const kNickNameControlName = _T("nickname");
const TCHAR* const lastContentTimeControlName = _T("lastContentTime");
const TCHAR* const klastmsgControlName = _T("lastmsg");
const TCHAR* const kUnreadcntControlName = _T("unreadcnt");


static bool OnLogoButtonEvent(void* event) {
	if (((TEventUI*)event)->Type == UIEVENT_BUTTONDOWN) {
		CControlUI* pButton = ((TEventUI*)event)->pSender;
		if (pButton != NULL) {
			CListContainerElementUI* pListElement = (CListContainerElementUI*)(pButton->GetTag());
			if (pListElement != NULL)
			{
				pListElement->DoEvent(*(TEventUI*)event);
				std::string sid = util::cStringToString(CString(pListElement->GetUserData()));
				if (!sid.empty())
				{
					//show the detail window.
					module::getSysConfigModule()->asynNotifyObserver(module::KEY_SYSCONFIG_SHOW_USERDETAILDIALOG, sid);
				}
			}
		}
	}
	return true;
}

CUIRecentSessionList::CUIRecentSessionList(CPaintManagerUI& paint_manager)
: UIIMList(paint_manager)
{

}

Node* CUIRecentSessionList::AddNode(const SessionListItemInfo& item, Node* parent /*= NULL*/, int index /*= 0*/)
{
	if (!parent)
		parent = root_node_;

	TCHAR szBuf[MAX_PATH] = { 0 };

	CListContainerElementUI* pListElement = NULL;
	if (!m_dlgBuilder.GetMarkup()->IsValid()) {
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("MainDialog\\recentlyListItem.xml"), (UINT)0, NULL, &paint_manager_));
	}
	else {
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create((UINT)0, &paint_manager_));
	}
	if (pListElement == NULL)
		return NULL;

	Node* node = new Node;

	node->data().level_ = parent->data().level_ + 1;
	if (item.folder)
		node->data().has_child_ = !item.empty;
	else
		node->data().has_child_ = false;

	node->data().folder_ = item.folder;
	node->data().child_visible_ = (node->data().level_ == 0);
	node->data().sId = item.id;
	node->data().text_ = item.nickName;
	node->data().list_elment_ = pListElement;

	if (!parent->data().child_visible_)
		pListElement->SetVisible(false);

	if (parent != root_node_ && !parent->data().list_elment_->IsVisible())
		pListElement->SetVisible(false);

	CDuiRect rcPadding = text_padding_;
	for (int i = 0; i < node->data().level_; ++i)
	{
		rcPadding.left += level_text_start_pos_;
	}
	pListElement->SetPadding(rcPadding);

	CControlUI* log_button = static_cast<CControlUI*>(paint_manager_.FindSubControlByName(pListElement, kLogoButtonControlName));
	if (log_button != NULL)
	{
		if (!item.folder && !item.avatarPath.IsEmpty())
		{
			_stprintf_s(szBuf, MAX_PATH - 1, _T("%s"), item.avatarPath);
			log_button->SetBkImage(szBuf);
		}
		else
		{
			CContainerUI* logo_container = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pListElement, kLogoContainerControlName));
			if (logo_container != NULL)
				logo_container->SetVisible(false);
		}
		log_button->SetTag((UINT_PTR)pListElement);
		//log_button->OnEvent += MakeDelegate(&OnLogoButtonEvent);//�Ҽ�����˸ù��ܣ����ð�
	}

	CDuiString html_text;
	if (node->data().has_child_)
	{
		if (node->data().child_visible_)
			html_text += level_expand_image_;
		else
			html_text += level_collapse_image_;

		_stprintf_s(szBuf, MAX_PATH - 1, _T("<x %d>"), level_text_start_pos_);
		html_text += szBuf;
	}

	if (item.folder)
	{
		html_text += node->data().text_;
	}
	else
	{
		_stprintf_s(szBuf, MAX_PATH - 1, _T("%s"), item.nickName);
		html_text += szBuf;
	}

	CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kNickNameControlName));
	if (nick_name != NULL)
	{
		if (item.folder)
			nick_name->SetFixedWidth(0);

		nick_name->SetShowHtml(true);
		nick_name->SetText(html_text);
	}

	CLabelUI* lastTime = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, lastContentTimeControlName));
	if (lastTime != NULL)
	{
		lastTime->SetShowHtml(true);
		CString strTime = module::getMiscModule()->makeShortTimeDescription(item.Time);//timeData.Format(_T("-%Y%m%d-%H-%M-%S-"));
		lastTime->SetText(strTime);
	}

	CLabelUI* plastMsgUI = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, klastmsgControlName));
	if (plastMsgUI)
	{
		CString strContent = item.description;
		ReceiveMsgManage::getInstance()->parseContent(strContent, TRUE, 400);//��Ҫת���ɱ��صĸ�ʽ
		plastMsgUI->SetText(strContent);
	}

	pListElement->SetFixedHeight(kIMListItemNormalHeight);
	pListElement->SetTag((UINT_PTR)node);
	pListElement->SetUserData(item.id);
	if (0 == index)
	{
		if (parent->has_children())
		{
			Node* prev = parent->get_last_child();
			index = prev->data().list_elment_->GetIndex() + 1;
		}
		else
		{
			if (parent == root_node_)
				index = 0;
			else
				index = parent->data().list_elment_->GetIndex() + 1;
		}
	}

	if (!CListUI::AddAt(pListElement, index))
	{
		delete pListElement;
		delete node;
		node = NULL;
	}

	parent->add_child(node);
	return node;
}

BOOL CUIRecentSessionList::AddNode(const std::string& sId)
{
	if (UIIMList::IsExistSId(sId))
	{
		LOG__(DEBG, _T("already exist!"));
		return FALSE;
	}
	SessionListItemInfo item;

	module::SessionEntity* pSessionInfo = SessionEntityManager::getInstance()->getSessionEntityBySId(sId);
	if (!pSessionInfo)
	{
		LOG__(DEBG, _T("Can't get the SessionEntity"));
		return FALSE;
	}
	item.folder = false;
	item.empty = false;
	item.description = util::stringToCString(pSessionInfo->latestMsgContent);
	item.Time = pSessionInfo->updatedTime;
	item.id = util::stringToCString(sId);
	if (pSessionInfo->sessionType == module::SESSION_USERTYPE)
	{
		module::UserInfoEntity userInfo;
		if (module::getUserListModule()->getUserInfoBySId(sId, userInfo))
		{
			item.avatarPath = util::stringToCString(userInfo.getAvatarPath());
			item.nickName = userInfo.getRealName();
		}
		else
		{
			item.nickName = util::stringToCString(sId);
			item.avatarPath = module::getUserListModule()->getDefaultAvatartPath();
		}
	}
	else if (module::SESSION_GROUPTYPE == pSessionInfo->sessionType)
	{
		module::GroupInfoEntity groupInfo;
		if (module::getGroupListModule()->getGroupInfoBySId(sId, groupInfo))
		{
			item.avatarPath = util::stringToCString(groupInfo.getAvatarPath());
			item.nickName = groupInfo.csName;
		}
		else
		{
			item.nickName = util::stringToCString(sId);
			item.avatarPath = module::getGroupListModule()->getDefaultAvatartPath();
		}
	}

	AddNode(item, NULL);
	return TRUE;
}

int CALLBACK IMListItemCompareFunc(UINT_PTR pa, UINT_PTR pb, UINT_PTR pUser)
{
	CListContainerElementUI* pListElement1 = (CListContainerElementUI*)pa;
	CListContainerElementUI* pListElement2 = (CListContainerElementUI*)pb;
	Node* node1 = (Node*)pListElement1->GetTag();
	Node* node2 = (Node*)pListElement2->GetTag();

	//��ȡ�Ự�ķ�����ʱ��

	CString s1 = node1->data().sId;
	CString s2 = node2->data().sId;
	if (s1.IsEmpty() || s2.IsEmpty())
	{
		LOG__(ERR, _T("sid is empty"));
		return 0;
	}
	std::string sid1 = util::cStringToString(s1);
	std::string sid2 = util::cStringToString(s2);
	module::SessionEntity* psEntity1 = SessionEntityManager::getInstance()->getSessionEntityBySId(sid1);
	module::SessionEntity* psEntity2 = SessionEntityManager::getInstance()->getSessionEntityBySId(sid2);
	
	if (!psEntity1 || !psEntity2)
	{
		LOG__(ERR, _T("Can't get SessionEntity"));
		return 0;
	}
	time_t m_updatedTime1 = psEntity1->updatedTime;
	time_t m_updatedTime2 = psEntity2->updatedTime;

	if (m_updatedTime1 == m_updatedTime2)
	{
		return 0;
	}

	int  nRes = psEntity1->updatedTime > psEntity2->updatedTime ? -1 : 1;
	return nRes;
}

void CUIRecentSessionList::sort()
{
	SortItems(IMListItemCompareFunc, 0);
}

BOOL CUIRecentSessionList::UpdateItemConentBySId(IN const std::string& sId)
{
	Node* pNode = GetItemBySId(sId);
	PTR_FALSE(pNode);
	CControlUI* pListElement = pNode->data().list_elment_;
	PTR_FALSE(pListElement);

	CLabelUI* plastMsgTimeUI = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, lastContentTimeControlName));
	if (!plastMsgTimeUI)
	{
		return FALSE;
	}
	CLabelUI* plastMsgUI = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, klastmsgControlName));
	if (!plastMsgUI)
	{
		return FALSE;
	}
	CLabelUI* Unreadcnt_button = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kUnreadcntControlName));
	if (!Unreadcnt_button)
	{
		return FALSE;
	}
	
	//���»Ự�����һ����Ϣ
	module::SessionEntity*  pSessionEntity = SessionEntityManager::getInstance()->getSessionEntityBySId(sId);
	if (!pSessionEntity)
	{
		LOG__(ERR, _T("Can't find the SessionEntity"));
		return FALSE;
	}
	std::string msgDecrptyCnt;
	DECRYPT_MSG(pSessionEntity->latestMsgContent, msgDecrptyCnt);
	CString strContent = util::stringToCString(msgDecrptyCnt);
	ReceiveMsgManage::getInstance()->parseContent(strContent, TRUE, 400);//��Ҫת���ɱ��صĸ�ʽ

	module::UserInfoEntity userInfo;
	CString strMsgTalkName;
	if (module::SESSION_GROUPTYPE == pSessionEntity->sessionType &&//ֻ��Ⱥ��Ҫչʾ ��Ϣ�ķ�����
		module::getUserListModule()->getUserInfoBySId(pSessionEntity->latestMsgFromId, userInfo))
	{
		strMsgTalkName = userInfo.getRealName();
		strMsgTalkName += CString(_T("��"));
	}
	strContent = strMsgTalkName + strContent;
	plastMsgUI->SetText(strContent);

	if (!SessionDialogManager::getInstance()->findSessionDialogBySId(sId))//���ڲ����ڵ�ʱ����¼���
	{
		//����δ������
		UInt32 nCnt = ReceiveMsgManage::getInstance()->getUnReadMsgCountBySId(sId);
        SetTextUICount(Unreadcnt_button, nCnt);
	}

	//������Ϣ��ʱ��
	CString strTime = module::getMiscModule()->makeShortTimeDescription(pSessionEntity->updatedTime);
	plastMsgTimeUI->SetText(strTime);
	
	sort();
	return TRUE;
}
BOOL CUIRecentSessionList::UpdateItemInfo(IN const SessionListItemInfo& item)
{
	std::string sid = util::cStringToString(CString(item.id));
	Node* pNode = GetItemBySId(sid);
	if (nullptr == pNode)
	{
		return FALSE;
	}
	CControlUI* pListElement = pNode->data().list_elment_;
	PTR_FALSE(pListElement);
	CContainerUI* logo_Button = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pListElement, kLogoButtonControlName));
	if (logo_Button != NULL)
		logo_Button->SetBkImage(item.avatarPath);

	CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kNickNameControlName));
	if (nick_name != NULL)
		nick_name->SetText(item.nickName);
	return TRUE;
}

void CUIRecentSessionList::ClearItemMsgCount(IN const std::string& sId)
{
	Node* pNode = GetItemBySId(sId);
	if (!pNode)
		return;
	CControlUI* pListElement = pNode->data().list_elment_;
	PTR_VOID(pListElement);
	//���δ������
	CLabelUI* Unreadcnt_button = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kUnreadcntControlName));
	if (!Unreadcnt_button)
	{
		LOG__(DEBG, _T("can't find the Unreadcnt_button"));
		return;
	}
	Unreadcnt_button->SetText(_T(""));
	Unreadcnt_button->SetVisible(false);

	return;
}



/******************************************************************************/


