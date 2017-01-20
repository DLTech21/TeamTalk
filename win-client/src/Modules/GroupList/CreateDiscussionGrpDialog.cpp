/******************************************************************************* 
 *  @file      MakeGroupWnd.cpp 2014\7\24 17:42:25 $
 *  @author    ���<dafo@mogujie.com>
 *  @brief   
 ******************************************************************************/

#include "stdafx.h"
#include "CreateDiscussionGrpDialog.h"
#include "UI/UIGroups.hpp"
#include "UI/CUISessionList.h"


DUI_BEGIN_MESSAGE_MAP(CreateDiscussionGrpDialog, WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_WINDOWINIT, OnPrepare)
DUI_END_MESSAGE_MAP()

/******************************************************************************/

// -----------------------------------------------------------------------------
//  MakeGroupWnd: Public, Constructor

CreateDiscussionGrpDialog::CreateDiscussionGrpDialog()
{

}

// -----------------------------------------------------------------------------
//  MakeGroupWnd: Public, Destructor

CreateDiscussionGrpDialog::~CreateDiscussionGrpDialog()
{

}

LPCTSTR CreateDiscussionGrpDialog::GetWindowClassName() const
{
	return _T("CreateDiscussionGrpDialog");
}

DuiLib::CDuiString CreateDiscussionGrpDialog::GetSkinFile()
{
	return  _T("CreateDiscussionGrpDialog\\skin.xml");
}

DuiLib::CDuiString CreateDiscussionGrpDialog::GetSkinFolder()
{
	return _T("");
}

CControlUI* CreateDiscussionGrpDialog::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("GroupList")) == 0)
	{
		return new CGroupsUI(m_PaintManager);
	}
	else if (_tcsicmp(pstrClass,_T("UserList")) == 0)
	{
		return new UIUserList(m_PaintManager);
	}
	return NULL;

}
void CreateDiscussionGrpDialog::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}


LRESULT CreateDiscussionGrpDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_NCLBUTTONDBLCLK != uMsg)//����˫�����������
	{
		return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
	}
	return 0;
}

void CreateDiscussionGrpDialog::OnPrepare(TNotifyUI& msg)
{
	CGroupsUI* pGroupsList = static_cast<CGroupsUI*>(m_PaintManager.FindControl(_T("AllUsersList")));
	if (pGroupsList != NULL)
	{
		if (pGroupsList->GetCount() > 0)
			pGroupsList->RemoveAll();

		GroupsListItemInfo item;

		item.folder = true;
		item.empty = false;
		item.nick_name = _T("SWAT");

		Node* root_parent = pGroupsList->AddNode(item, NULL);

		item.folder = false;
		item.logo = _T("duilib.png");
		item.nick_name = _T("���");
		item.description = _T("153787916");
		pGroupsList->AddNode(item, root_parent);

		item.folder = false;
		item.logo = _T("groups.png");
		item.nick_name = _T("�쵶");
		item.description = _T("79145400");
		pGroupsList->AddNode(item, root_parent);


		item.id = _T("3");
		item.folder = true;
		item.empty = false;
		item.nick_name = _T("�ͷ�");
		Node* root_parent2 = pGroupsList->AddNode(item, NULL);

		item.folder = false;
		item.logo = _T("groups.png");
		item.nick_name = _T("��Ī");
		item.description = _T("79145400");
		pGroupsList->AddNode(item, root_parent2);

		for (int n = 0; n < 40; ++n)
		{
			item.folder = false;
			item.logo = _T("groups.png");
			item.nick_name = _T("����");
			item.description = _T("79145400");
			pGroupsList->AddNode(item, root_parent2);
		}

	}

	UIUserList* pRecentlyList = static_cast<UIUserList*>(m_PaintManager.FindControl(_T("AddedUsersList")));
	if (pRecentlyList != NULL)
	{
		if (pRecentlyList->GetCount() > 0)
			pRecentlyList->RemoveAll();

		UserListItemInfo item;

		srand((UINT)time(0));
		item.Time = rand() % 200;
		item.folder = false;
		item.empty = false;
		item.logo = _T("duilib.png");
		item.nick_name = _T("����");
		item.description.Format(_T("%d"), item.Time);

		pRecentlyList->AddNode(item, NULL);


		item.Time = rand() % 200;
		item.logo = _T("duilib.png");
		item.nick_name = _T("���");
		//item.description = _T("��˧��");
		item.description.Format(_T("%d"), item.Time);
		pRecentlyList->AddNode(item, NULL);


		item.Time = rand() % 200;
		item.logo = _T("duilib.png");
		item.nick_name = _T("��Ҷ");
		//item.description = _T("��ɧ������");
		item.description.Format(_T("%d"), item.Time);
		pRecentlyList->AddNode(item, NULL, 2);

		item.Time = rand() % 200;
		item.logo = _T("duilib.png");
		item.nick_name = _T("����");
		//item.description = _T("��һ�������");
		item.description.Format(_T("%d"), item.Time);
		pRecentlyList->AddNode(item, NULL);

		item.Time = rand() % 200;
		item.logo = _T("duilib.png");
		item.nick_name = _T("��Ҷ");
		//item.description = _T("��ɧ������");
		item.description.Format(_T("%d"), item.Time);
		pRecentlyList->AddNode(item, NULL);

		item.Time = rand() % 200;
		item.logo = _T("duilib.png");
		item.nick_name = _T("ǧ��");
		//item.description = _T("��è�ĺ�ֽ");
		item.description.Format(_T("%d"), item.Time);
		pRecentlyList->AddNode(item, NULL, 3);

		pRecentlyList->sort();
	}
}

/******************************************************************************/