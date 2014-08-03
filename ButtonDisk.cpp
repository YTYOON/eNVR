// ButtonDisk.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "ButtonDisk.h"
#include <math.h> 

#define WM_BUTTONDISK_PRESS		WM_USER + 9428
#define pi 3.1415926535897932384626433832795028

IMPLEMENT_DYNAMIC(CButtonDisk, CStatic)

CButtonDisk::CButtonDisk()
:m_nStatus(NORMAL),m_nPosStatus(POS_CENTER)
{

}

CButtonDisk::~CButtonDisk()
{
}


BEGIN_MESSAGE_MAP(CButtonDisk, CStatic)
    ON_WM_MOUSEHOVER()
    ON_WM_LBUTTONUP()
    ON_WM_MBUTTONDOWN()
    ON_WM_CREATE()
    ON_WM_KILLFOCUS()
    ON_WM_ACTIVATE()
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



BOOL bPtOnCircle(CPoint& ptTest, int dwRad)
{
    float radiusTest = sqrt(float((abs(ptTest.x) ^ 2) + (abs(ptTest.y)  ^ 2)));
    float radiusCircle = sqrt(float(abs(dwRad) ^ 2));
    if ( radiusTest <=  radiusCircle)
        TRACE(_T(" radiusTest <=  radiusCircle"));
    else
        TRACE(_T(" radiusTest >  radiusCircle"));
    return  radiusTest <=  radiusCircle;
}
//BOOL bPtOnArc(CPoint& ptTest, CPoint ptRad1, CPoint ptRad2)
//{
//    float radiusTest = sqrt(float(ptTest.x ^ 2 + ptTest.y ^ 2));
//    float radiusCircle = sqrt(float(ptRad1.x ^ 2 + ptRad1.y ^ 2));
//    float Angle = atan(float(ptTest.y/ptTest.x));
//    ASSERT(false);  // Javan Pending
//    return  radiusTest <=  radiusCircle;
//    
//}
#define SIGN(A)  ( (A) >= 0 ? 1: 0)
bool bPtOnArc(CPoint& ptTest, float fRadius, double fAngle1, double fAngle2)
{
    bool bRet(false);
    float radiusTest = sqrt(float((abs(ptTest.x) ^ 2) + (abs(ptTest.y)  ^ 2)));
    float radiusCircle = sqrt(abs(fRadius)* abs(fRadius));  //+ (1-SIGN(ptTest.y))*pi
    if ( radiusTest <=  radiusCircle)
    {
        float fAngleTest;
        if (ptTest.x == 0)
            fAngleTest = pi/2 +  (1- SIGN(ptTest.y)) *pi;
        else
        {
            fAngleTest = abs(atan(float((float)ptTest.y/(float)ptTest.x))); 
            if (ptTest.x < 0 && ptTest.y > 0) fAngleTest = pi - fAngleTest;
            if (ptTest.x < 0 && ptTest.y < 0) fAngleTest = pi + fAngleTest;
            if (ptTest.x > 0 && ptTest.y < 0) fAngleTest = 2*pi - fAngleTest;
        }
        if (fAngleTest >= fAngle1 &&  fAngleTest <= fAngle2) 
            bRet = true;
    }
    return bRet;
    /*const float a_min = 0.5 * (rect.right - rect.left) - tolerance - 1;
    const float a_min = 0.5 * (rect.right - rect.left) - tolerance - 1;
    const float a_min = 0.5 * (rect.right - rect.left) - tolerance - 1;
    const float a_min = 0.5 * (rect.right - rect.left) - tolerance - 1;
    const CPoint ptCenter(a_max +rect.left - tolerance, b_max + rect.top - tolerance);
    CPoint ptEnd1;
    ::IntersectRadialAndEllipse*/
    return true;

}



void CButtonDisk::OnMouseHover(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CStatic::OnMouseHover(nFlags, point);
}

void CButtonDisk::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_nStatus = MOUSE_HOVER;

    ::PostMessage(this->m_hWnd, WM_PAINT, 0, 0);
    //Invalidate(false);
   ::PostMessage(GetParent()->GetSafeHwnd(), WM_BUTTONDISK_RELEASE, m_nPosStatus, 0 );
    CStatic::OnLButtonUp(nFlags, point);
}

void CButtonDisk::OnMButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CStatic::OnMButtonDown(nFlags, point);
}
int CButtonDisk::nAreaHitTest(CPoint& point)
{
    enum  {BULL_EYE = 0, LEFT, TOP, RIGHT, BOTTOM};
    int nArea(-1);
    CPoint pointOffset;
    float fRadius = 108/2;
    
    pointOffset.x = point.x - fRadius;
    pointOffset.y = fRadius - point.y;
    bool bInCircle = bPtOnCircle(pointOffset, fRadius);
    if (bInCircle == false) return nArea;
    else
    {
        int nCenterRadius(20);
        if ( bPtOnCircle(pointOffset, nCenterRadius) ) nArea = POS_CENTER ;
        else if (bPtOnArc(pointOffset, fRadius, pi/4, (3*pi)/4))  nArea = POS_UP;  
        else if (bPtOnArc(pointOffset, fRadius, 0, pi/4))  nArea = POS_RIGHT;
        else if (bPtOnArc(pointOffset, fRadius, pi*7/4, 2*pi))  nArea = POS_RIGHT;
        else if (bPtOnArc(pointOffset, fRadius, pi*5/4, pi*7/4))  nArea = POS_DOWN;
        else if (bPtOnArc(pointOffset, fRadius, pi*3/4, pi*5/4))  nArea = POS_LEFT;
             
        //if (bPtOnArc(pointOffset,
    }

    return nArea;

}
int CButtonDisk::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CStatic::OnCreate(lpCreateStruct) == -1)
        return -1;
    m_bmpNormal.LoadBitmap(IDB_BITMAP_DISK);

    m_bmpEvent[0][0].LoadBitmap(IDB_BITMAP_DISK_CENTER_FOCUS);
    m_bmpEvent[0][1].LoadBitmap(IDB_BITMAP_DISK_UP_FOCUS);
    m_bmpEvent[0][2].LoadBitmap(IDB_BITMAP_DISK_RIGHT_FOCUS);
    m_bmpEvent[0][3].LoadBitmap(IDB_BITMAP_DISK_DOWN_FOCUS);
    m_bmpEvent[0][4].LoadBitmap(IDB_BITMAP_DISK_LEFT_FOCUS);

    m_bmpEvent[1][0].LoadBitmap(IDB_BITMAP_DISK_CENTER_PRESS);
    m_bmpEvent[1][1].LoadBitmap(IDB_BITMAP_DISK_UP_PRESS);
    m_bmpEvent[1][2].LoadBitmap(IDB_BITMAP_DISK_RIGHT_PRESS);
    m_bmpEvent[1][3].LoadBitmap(IDB_BITMAP_DISK_DOWN_PRESS);
    m_bmpEvent[1][4].LoadBitmap(IDB_BITMAP_DISK_LEFT_PRESS);

    CRect rect;
    GetClientRect(rect);
    CDC* pDc = GetDC();
    int nWidth = rect.Width();
    int nHeight = rect.Height();
    for (int ci = 0; ci < 2; ci++)
    {
        for (int cj = 0; cj < 5; cj++)
        {
            m_dcEvent[ci][cj].CreateCompatibleDC(pDc);
            HBITMAP hbmp = CreateCompatibleBitmap(m_dcEvent[ci][cj].m_hDC, nWidth, nHeight);
            HBITMAP hOldbitmap = (HBITMAP)m_dcEvent[ci][cj].SelectObject(m_bmpEvent[ci][cj]);
            DeleteObject(hOldbitmap);
        }
    }
    m_dcNormal.CreateCompatibleDC(pDc);
    HBITMAP hOld = (HBITMAP)m_dcNormal.SelectObject(m_bmpNormal);
    DeleteObject(hOld);
    return 0;
}

void CButtonDisk::OnKillFocus(CWnd* pNewWnd)
{
    CStatic::OnKillFocus(pNewWnd);

    // TODO: Add your message handler code here
}

void CButtonDisk::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    CStatic::OnActivate(nState, pWndOther, bMinimized);
    if (nState == WA_INACTIVE)
    {
        m_nStatus == NORMAL;
        ::PostMessage(this->m_hWnd, WM_PAINT, 0, 0);
    }
}

void CButtonDisk::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_nStatus = MOUSE_PRESS;
    ::PostMessage(this->m_hWnd, WM_PAINT, 0, 0);
    ::PostMessage(GetParent()->GetSafeHwnd(), WM_BUTTONDISK_PRESS, m_nPosStatus, 0 );
    CStatic::OnLButtonDown(nFlags, point);
}

void CButtonDisk::OnPaint()
{
    CPaintDC dcPaint(this); // device context for painting
    CDC& dc = *GetDC();
    BITMAP bm; 
    if (m_nStatus == NORMAL)
    {
        m_bmpNormal.GetObject(sizeof(bm), &bm);
        ::BitBlt(dc.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, m_dcNormal.m_hDC, 0, 0, SRCCOPY);
    }
    else
    {
        m_bmpEvent[m_nStatus][m_nPosStatus].GetObject(sizeof(bm), &bm);
        ::BitBlt(dc.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, m_dcEvent[m_nStatus][m_nPosStatus].m_hDC, 0, 0, SRCCOPY);
    }
    ReleaseDC(&dc);
}

void CButtonDisk::OnMouseMove(UINT nFlags, CPoint point)
{

    CRect rect;
    GetClientRect(rect);
    int nTest = nAreaHitTest(point);
    if (nTest != -1)
    {
        if  (m_nStatus != MOUSE_PRESS)
        {
            m_nStatus = MOUSE_HOVER;
        }
        m_nPosStatus = nTest;
        ::PostMessage(this->m_hWnd, WM_PAINT, 0, 0);
        this->SetCapture();
    }
    else
    {
        ReleaseCapture();
        m_nStatus = NORMAL;
        ::PostMessage(this->m_hWnd, WM_PAINT, 0, 0);

    }
    CStatic::OnMouseMove(nFlags, point);
}
