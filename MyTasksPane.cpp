// MyTasksPane.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "MyTasksPane.h"
#include "MainFrm.h"


// CMyTasksPane

IMPLEMENT_DYNAMIC(CMyTasksPane, CMFCTasksPane)

CMyTasksPane::CMyTasksPane(CMainFrame* pMainFrame):m_pMainFrame(pMainFrame)
,m_pFullGroupCaption(NULL)
{
    EnableAnimation(false);
}

CMyTasksPane::~CMyTasksPane()
{
}


BEGIN_MESSAGE_MAP(CMyTasksPane, CMFCTasksPane)
	ON_WM_LBUTTONUP()
    ON_WM_RBUTTONUP()
    //ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CMyTasksPane message handlers
void CMyTasksPane::OnLButtonUp(UINT nFlags, CPoint point)
{
    CMFCTasksPane::OnLButtonUp(nFlags, point);
    vRestoreStatus();
    return;
}
void CMyTasksPane::OnRButtonUp(UINT nFlags, CPoint point)
{
    CMFCTasksPaneTaskGroup* pGroupPickup = GroupCaptionHitTest(point);
    //CMFCTasksPaneTask*  pTask= TaskHitTest(point);
    if (pGroupPickup)
    {
        if (pGroupPickup == m_pFullGroupCaption) 
        {
             vRestoreStatus();
        }else
        {
            vRestoreStatus();
            m_pFullGroupCaption = pGroupPickup;
            CMFCTasksPaneTask* pTask = (CMFCTasksPaneTask*)m_pFullGroupCaption->m_lstTasks.GetHead();//.GetAt(0);
            CRect rectClient1;
	        GetClientRect(&rectClient1);
            SetWindowHeight(pTask->m_hwndTask, rectClient1.Height());
            for (POSITION pos = m_lstTaskGroups.GetHeadPosition(); pos != NULL;)
            {
                CMFCTasksPaneTaskGroup* pGroupIt = (CMFCTasksPaneTaskGroup*) m_lstTaskGroups.GetNext(pos);
                m_mapCollapse[pGroupIt] = pGroupIt->m_bIsCollapsed; 
                CollapseGroup(pGroupIt, pGroupIt != m_pFullGroupCaption);
            }
            //CollapseGroup(pGroupPickup, false);
        }
    }
    //CMFCTasksPane::OnRButtonDown(nFlags, point);
}

int CMyTasksPane::AddWindow(int nGroup, HWND hwndTask, int nWndHeight, BOOL bAutoDestroyWindow, DWORD dwUserData)
{
    m_mapGroupHeight[nGroup] = nWndHeight;
    m_mapTaskHwnd[nGroup] = hwndTask;
    int nIdx = CMFCTasksPane::AddWindow( nGroup,  hwndTask,  nWndHeight,  bAutoDestroyWindow,  dwUserData);
    return nIdx;
}

BOOL CMyTasksPane::OnShowControlBarMenu(CPoint point)
{
    return true;

}
//void CMyTasksPane::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
//{
//    ReleaseCapture();
//    SetCursor(::LoadCursor(NULL, IDC_ARROW));
//    return;
//    // TODO: Add your message handler code here
//}

void CMyTasksPane::vRestoreStatus(void)
{
    if (m_pFullGroupCaption != NULL)
    {    
        int nGroup(0);
        if (GetGroupLocation(m_pFullGroupCaption, nGroup))
        {
            CMFCTasksPaneTask* pTask = (CMFCTasksPaneTask*)m_pFullGroupCaption->m_lstTasks.GetHead();
            SetWindowHeight(pTask->m_hwndTask, m_mapGroupHeight[nGroup]);
        }
        for (POSITION pos = m_lstTaskGroups.GetHeadPosition(); pos != NULL;)
        {
            CMFCTasksPaneTaskGroup* pGroup = (CMFCTasksPaneTaskGroup*) m_lstTaskGroups.GetNext(pos);
            //if (m_pFullGroupCaption == pGroup)
            {
                CollapseGroup(pGroup, m_mapCollapse[pGroup]);
            }
        }    
        m_pFullGroupCaption = NULL;
    }
    for (POSITION pos = m_lstTaskGroups.GetHeadPosition(); pos != NULL;)
    {            
        CMFCTasksPaneTaskGroup* pGroup = (CMFCTasksPaneTaskGroup*) m_lstTaskGroups.GetNext(pos);
        int nGroup(0);
        if (GetGroupLocation(pGroup, nGroup))
        {
            ::InvalidateRect(m_mapTaskHwnd[nGroup], NULL, false);
        }
    }
}
