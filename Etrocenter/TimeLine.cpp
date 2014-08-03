// TimeLine.cpp : implementation file
//

#include "stdafx.h"
#include "TimeLine.h"
#include "NVR.h"
#define nRows 7
#define nColumns 24
#define SCHEDULECOLOR RGB(20,20,200)
#define EVENTCOLOR RGB(200,20,20)
#define BKCOLOR RGB(255,255,255)

// CTimeLine

IMPLEMENT_DYNAMIC(CTimeLine, CStatic)

CTimeLine::CTimeLine():m_bDrag(false),m_nStartIdx(0),m_nEndIdx(-1),m_RectBrush(SCHEDULECOLOR),m_RectBrushEvent(EVENTCOLOR),m_bChangeColor(true),m_nPaintedIdx(-1),m_bEnable(false)
{

}

CTimeLine::~CTimeLine()
{
	m_rects.clear();
	m_bColored.clear();
	m_bColoredEvent.clear();
	m_RectBrush.DeleteObject();
	m_RectBrushEvent.DeleteObject();
}


BEGIN_MESSAGE_MAP(CTimeLine, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()



// CTimeLine message handlers



void CTimeLine::OnPaint()
{
		CPaintDC dc(this);
		CDC* pDC = &dc;
		CPen Pen( PS_SOLID, 2, RGB(192,192,192) );
		CRect rect;
		GetClientRect(&rect);
		CBrush bkColor(GLOBALBKCOLOR);
		for (int ci=0; ci < m_rects.size(); ci++)
		{
			CRect rect = m_rects[ci];
			CPen* pPen;
			pPen =  &Pen;
			CPen *pOldPen =	pDC->SelectObject( pPen );
			rect.DeflateRect(-2,-2);
			pDC->MoveTo(rect.TopLeft());
			pDC->LineTo(rect.right, rect.top);
			pDC->LineTo(rect.BottomRight());
			pDC->LineTo(rect.left, rect.bottom);
			pDC->LineTo(rect.TopLeft());
			pDC->SelectObject( &pOldPen );		
		}	
		
		

		for(int iTemp = 0; iTemp < m_ChartStatus.size(); iTemp++)
		{
			if(m_iArrayFirstByte == STATUS::CONTINUOUS)
			{
				dc.FillRect(m_rects[iTemp],&m_RectBrush);
			}
			else
			{
				switch(m_ChartStatus[iTemp])
				{
				case CLEAR:
					dc.FillRect(m_rects[iTemp],&bkColor);
					break;
				case CONTINUOUS:
					dc.FillRect(m_rects[iTemp],&m_RectBrush);
					break;
				case EVENT:
					dc.FillRect(m_rects[iTemp],&m_RectBrushEvent);
					break;
				default:
					ASSERT(false);
					break;
				}
			}
		}

		ReleaseDC(pDC);
}

void CTimeLine::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!m_bEnable)	return;

	CClientDC dc(this);
	CBrush bkColor(GLOBALBKCOLOR);
	for(int i=0;i<m_rects.size();i++)
	{
		if(m_rects[i].PtInRect(point))
		{
			if(m_bChangeColor)
			{
				::SetCursor(((CEtroExplorerApp*)AfxGetApp())->m_hCursor[4]);
			}
			else
			{
				::SetCursor(((CEtroExplorerApp*)AfxGetApp())->m_hCursor[5]);
			}
			if (m_nPaintedIdx == i)
				return;
			else 
				m_nPaintedIdx = i;
		
			if(m_bChangeColor)
			{
				switch(m_ChartStatus[i])
				{
					case CLEAR:
					{
						dc.FillRect(m_rects[i],&m_RectBrush);			
						m_ChartStatus[i] = CONTINUOUS;
						break;
					}
					case CONTINUOUS:
					{
						dc.FillRect(m_rects[i],&bkColor);			
						m_ChartStatus[i] = CLEAR;
						break;
					}
					case EVENT:
					{	
						dc.FillRect(m_rects[i],&m_RectBrush);			
						m_ChartStatus[i] = CONTINUOUS;
						break;
					}
				}
			}
			else
			{
				switch(m_ChartStatus[i])
				{
					case CLEAR:
					{
						dc.FillRect(m_rects[i],&m_RectBrushEvent);
						m_ChartStatus[i] = EVENT;
						break;
					}
					case CONTINUOUS:
					{
						dc.FillRect(m_rects[i],&m_RectBrushEvent);
						m_ChartStatus[i] = EVENT;
						break;
					}
					case EVENT:
					{
						dc.FillRect(m_rects[i],&bkColor);			
						m_ChartStatus[i] = CLEAR;
						break;
					}
				}				
			}
	
			m_nStartIdx = i;
		}
	}
	
}

void CTimeLine::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_nPaintedIdx = -1;
	//if(m_bDrag)
	//{
	//	CClientDC dc(this);
	//	
	//	for(int i=0;i<m_rects.size();i++)
	//	{	
	//		if(m_rects[i].PtInRect(point))
	//		{
	//			m_nEndIdx = i;
	//			break;
	//		}

	//	}

	//	if(m_nEndIdx == -1)
	//		return;

	//	if(m_nEndIdx >= m_nStartIdx)
	//	{
	//		for(int i = m_nStartIdx;i <= m_nEndIdx;i++)
	//		{
	//			if(m_bChangeColor)
	//			{
	//				dc.FillRect(m_rects[i],&m_RectBrush);
	//				m_bColoredEvent[i] = false;
	//				m_bColored[i] = true;
	//			}
	//			else
	//			{
	//				dc.FillRect(m_rects[i],&m_RectBrushEvent);
	//				m_bColored[i] = false;
	//				m_bColoredEvent[i] = true;
	//			}
	//		}
	//		
	//	}
	//	else
	//	{
	//		for(int i = m_nEndIdx;i <= m_nStartIdx;i++)
	//		{
	//			if(m_bChangeColor)
	//			{
	//				dc.FillRect(m_rects[i],&m_RectBrush);
	//				m_bColoredEvent[i] = false;
	//				m_bColored[i] = true;
	//			}
	//			else
	//			{
	//				dc.FillRect(m_rects[i],&m_RectBrushEvent);
	//				m_bColored[i] = false;
	//				m_bColoredEvent[i] = true;
	//			}
	//		}
	//		
	//	}

	//	m_nStartIdx =  0;
	//	m_bDrag = false;
	//	m_nEndIdx = -1;
	//}
	//else
	//{
	//	m_nStartIdx =  0;
	//	m_nEndIdx = -1;
	//}
}

void CTimeLine::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//CClientDC dc(this);
	//CBrush bkColor(GLOBALBKCOLOR);
	//for(int i=0;i<m_rects.size();i++)
	//{	
	//	if(m_rects[i].PtInRect(point))
	//	{
	//		//if(m_bColored[i] == true )
	//		//{
	//			dc.FillRect(m_rects[i],&bkColor);
	//			m_ChartStatus[i] = CLEAR;
	//		//}
	//	}
	//}
	//
	//bkColor.DeleteObject();
}

void CTimeLine::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bEnable)	return;

	CClientDC dc(this);
	CBrush  bkColor(GLOBALBKCOLOR);

	for(int iTemp = 0; iTemp < m_rects.size(); iTemp++)
	{
		if(m_rects[iTemp].PtInRect(point) && nFlags == MK_LBUTTON)
		{	
			if(m_bChangeColor)
			{
				::SetCursor(((CEtroExplorerApp*)AfxGetApp())->m_hCursor[4]);
			}
			else
			{
				::SetCursor(((CEtroExplorerApp*)AfxGetApp())->m_hCursor[5]);
			}

			if (m_nPaintedIdx == iTemp)
				return;
			else 
				m_nPaintedIdx = iTemp;
		
			if(m_bChangeColor) // 0 event; 1 continuous
			{
				switch(m_ChartStatus[iTemp])
				{
					case CLEAR:
					{
						dc.FillRect(m_rects[iTemp],&m_RectBrush);			
						m_ChartStatus[iTemp] = CONTINUOUS;
						
						break;
					}
					case CONTINUOUS:
					{
						
						dc.FillRect(m_rects[iTemp],& bkColor);
						m_ChartStatus[iTemp] = CLEAR;
						break;
						
					}
					case EVENT:
					{	
						dc.FillRect(m_rects[iTemp],&m_RectBrush);			
						m_ChartStatus[iTemp] = CONTINUOUS;
						
						break;
					}
				}
			}
			else
			{
				switch(m_ChartStatus[iTemp])
				{
					case CLEAR:
					{

						dc.FillRect(m_rects[iTemp],&m_RectBrushEvent);
						m_ChartStatus[iTemp] = EVENT;
						break;
					}
					case CONTINUOUS:
					{
						dc.FillRect(m_rects[iTemp],&m_RectBrushEvent);
						m_ChartStatus[iTemp] = EVENT;
						break;
					}
					case EVENT:
					{
						dc.FillRect(m_rects[iTemp],& bkColor);
						m_ChartStatus[iTemp] = CLEAR;
						break;
					}
				}
			}		
		}
	}
}

void CTimeLine::vSetGridRect()
{
	CRect rect;
	GetClientRect(rect);

	m_rects.clear();
	m_ChartStatus.clear();
	
	int nGapx = rect.Width() /  nColumns ; 
	int nGapy = rect.Height() / nRows;

	for (int cj=0; cj < (nRows); cj++) 
	{
		for (int ci=0; ci <(nColumns); ci++) 
		{
			RECT rcTmp={nGapx * ci, nGapy * cj, nGapx * (ci +1), nGapy * (cj+1)};
			((CRect*)&rcTmp)->DeflateRect(1, 1);
			m_rects.push_back(rcTmp);
			m_ChartStatus.push_back(CLEAR);
		}
	}

	return;
}

void CTimeLine::OnSize(UINT nType, int cx, int cy)
{
	
	vSetGridRect();
	CStatic::OnSize(nType, cx, cy);
}

DateTime CTimeLine::RectToDate(int nRectIdx)
{
	DateTime date;
	//int hour[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
	
	int nDivider = nRectIdx / nColumns;
	int nMod     =  nRectIdx % nColumns;

	switch(nDivider)
	{
		case 0:
			_tcscpy(date.Date,_T("SUN"));
			break;
		case 1:
			_tcscpy(date.Date,_T("MON"));
			break;
		case 2:
			_tcscpy(date.Date,_T("TUE"));
			break;
		case 3:
			_tcscpy(date.Date,_T("WED"));
			break;
		case 4:
			_tcscpy(date.Date,_T("THU"));
			break;
		case 5:
			_tcscpy(date.Date,_T("FRI"));
			break;
		case 6:
		{
			
			_tcscpy(date.Date,_T("SAT"));
			break; 
		}

	}
	
	date.hour = nMod;


	return date;
}

void CTimeLine::vReset()
{
	CClientDC dc(this);
	CBrush bkColor(GLOBALBKCOLOR);
	for(int i=0;i<m_rects.size();i++)
	{	
		dc.FillRect(m_rects[i],&bkColor);
		m_ChartStatus[i] = CLEAR;
	}

	bkColor.DeleteObject();


}

void CTimeLine::vInitial()
{
	CClientDC dc(this);
	CBrush bkColor(GLOBALBKCOLOR);
	for(int i=0;i<m_rects.size();i++)
	{	
		if(m_bColored[i] == false)
		{
			dc.FillRect(m_rects[i],&bkColor);
		}
	}
	
	bkColor.DeleteObject();
}

void CTimeLine::vFillDayRect(int iDay, bool bFill)
{
	if(!m_bEnable)	return;

	CClientDC dc(this);
	CBrush bkColor(GLOBALBKCOLOR);
	bool bType = m_bChangeColor;

	for(int iTemp = (iDay-1)*24; iTemp < iDay*24; iTemp++)
	{
		if(bFill)
		{
			//true -> m_RectBrush, false -> m_RectBrushEvent
			if(bType)
			{
				dc.FillRect(m_rects[iTemp],&m_RectBrush);
				m_ChartStatus[iTemp] = CONTINUOUS;
			}
			else
			{				
				dc.FillRect(m_rects[iTemp],&m_RectBrushEvent);
				m_ChartStatus[iTemp] = EVENT;	
			}
		}
		else
		{
			/*if(bType)
			{
				if(m_bColoredEvent[iTemp])	
				{
				m_ChartStatus[iTemp] = EVENT;	
				continue;
				}
				m_bColored[iTemp] = false;
				dc.FillRect(m_rects[iTemp],&cbBackgroundColor);
			}
			else
			{
				if(m_bColored[iTemp])
				{
					m_ChartStatus[iTemp] = CONTINUOUS;
					continue;
				}
				m_bColoredEvent[iTemp] = false;
				dc.FillRect(m_rects[iTemp],&cbBackgroundColor);
			}*/
			m_ChartStatus[iTemp] = CLEAR;
			dc.FillRect(m_rects[iTemp],&bkColor);
		}
	}
}

void CTimeLine::vSetChangeColorFlag(bool bFlag)
{
	m_bChangeColor = bFlag;
}

void CTimeLine::vGetEventByte(BYTE* btSrc)
{
	CClientDC dc(this);
	for(int iTemp = 0; iTemp < m_rects.size(); iTemp++)
	{
		switch(m_ChartStatus[iTemp])
		{
		case CLEAR:
			btSrc[iTemp] = '0';
			break;
		case CONTINUOUS:
			btSrc[iTemp] = '1';
			break;
		case EVENT:
			btSrc[iTemp] = '2';
			break;
		default:
			ASSERT(false);
			break;
		}
	}
}

void CTimeLine::vFillAllRect()
{
	CClientDC dc(this);
	for(int iTemp = 0; iTemp < m_rects.size(); iTemp++)
	{
		dc.FillRect(m_rects[iTemp],&m_RectBrush);
		//m_ChartStatus[iTemp] = CONTINUOUS;
	}
}

void CTimeLine::vFillScheduleAndEventRect(BYTE *btSrc)
{
	CClientDC dc(this);
	CBrush bkColor(GLOBALBKCOLOR);

	for(int iBytes = 1; iBytes < 22; iBytes++)
	{
		for(int iBits = 7; iBits > -1; iBits--)
		{
			unsigned int bitmask = 1 << iBits;
			if(btSrc[iBytes] & bitmask)
			{
				dc.FillRect(m_rects[((iBytes-1)*8+(7-iBits))],&m_RectBrush);
				m_ChartStatus[((iBytes-1)*8+(7-iBits))] = CONTINUOUS;
			}
			else
			{
				dc.FillRect(m_rects[((iBytes-1)*8+(7-iBits))],&bkColor);
				m_ChartStatus[((iBytes-1)*8+(7-iBits))] = CLEAR;
			}
		}
	}

	for(int iBytes = 22; iBytes < 43; iBytes++)
	{
		for(int iBits = 7; iBits > -1; iBits--)
		{
			unsigned int bitmask = 1 << iBits;
			if(btSrc[iBytes] & bitmask)
			{
				dc.FillRect(m_rects[((iBytes-1)*8+(7-iBits))-168],&m_RectBrushEvent);
				m_ChartStatus[((iBytes-1)*8+(7-iBits))-168] = EVENT;
			}
			else
			{
				//m_ChartStatus[((iBytes-1)*8+(7-iBits))-168] = CLEAR;
			}
		}
	}

	return;
}

void CTimeLine::vDisable()
{
	m_bEnable = false;

	return;
}

void CTimeLine::vEnable()
{
	m_bEnable = true;

	return;
}
BOOL CTimeLine::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(m_bEnable == true)
	{
		if(m_bChangeColor)
		{
			::SetCursor(((CEtroExplorerApp*)AfxGetApp())->m_hCursor[4]);
			
		}
		else
		{
			::SetCursor(((CEtroExplorerApp*)AfxGetApp())->m_hCursor[5]);
			
		}
	}
	//return CStatic::OnSetCursor(pWnd, nHitTest, message);
	return true;
}
