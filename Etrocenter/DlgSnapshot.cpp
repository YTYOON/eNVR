// DlgSnapshot.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSnapshot.h"
#include "MainFrm.h"
#define IDC_SCREEN                      10009
#define BTNSIZE		24
#define IDB_SNAPSHOT_CLOSE 1000040

// CDlgSnapshot dialog

IMPLEMENT_DYNAMIC(CDlgSnapshot, CDialog)

CDlgSnapshot::CDlgSnapshot(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSnapshot::IDD, pParent),m_picture_buf(NULL),m_rectDraw(0,0,0,0),m_rectDrawOrg(0,0,0,0),m_rcImg(0,0,0,0),m_rectDlg(0,0,0,0),m_zDelta(0)
	
{
	memset(&m_bih, 0, sizeof(BITMAPINFOHEADER));
	m_PickPoint.x = 0;
	m_PickPoint.y = 0;
}

CDlgSnapshot::~CDlgSnapshot()
{
	if(m_picture_buf)
	{
		delete[] m_picture_buf;
	}
	m_picture_buf = NULL;


	DELETEOBJ(m_ToolTip);

	m_MemBitmap.DeleteObject();
	m_MemDC.DeleteDC();
}

void CDlgSnapshot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSnapshot, CDialog)
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
//	ON_BN_CLICKED(IDB_BTN_SNAPSHOT_CLOSE, &CDlgSnapshot::OnBnClickedClose)
//	ON_BN_CLICKED(IDB_BTN_SNAPSHOT_OPEN, &CDlgSnapshot::OnBnClickedOpenDirectory)
//	ON_CONTROL_RANGE(BN_CLICKED,IDC_BUTTON1,IDC_BUTTON2, &CDlgSnapshot::OnBnClickedOpenDirectory)
	ON_WM_LBUTTONUP()	
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgSnapshot::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgSnapshot::OnBnClickedButton2)
//	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CDlgSnapshot message handlers
void CDlgSnapshot::vGetBMP(uint8_t* picture_buf, BITMAPINFOHEADER bih,DWORD biSizeImage, CString csFileName)
{
	m_biSizeImage = biSizeImage;
	m_picture_buf =  new BYTE [biSizeImage];
	memcpy(m_picture_buf,picture_buf,biSizeImage);
	memcpy(&m_bih,&bih,sizeof(bih));
	m_rcImg.left = m_rcImg.top = 0;
	m_rcImg.right = m_bih.biWidth;
	m_rcImg.bottom = m_bih.biHeight*(-1);
	m_csFileName = csFileName;
}
int CDlgSnapshot::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;	
		
	GetDesktopWindow()->GetWindowRect(&m_rectDlg);  
	this->SetWindowPos(NULL,m_rectDlg.left,m_rectDlg.top,m_rectDlg.Width(),m_rectDlg.Height(),SWP_SHOWWINDOW);
	ShowWindow(SW_SHOW);
	vGetBackGround();
	
	
	CRect rectBtn;
	rectBtn = m_rectDlg;
	DWORD dwStyleBtn = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW;
		
	rectBtn.left = rectBtn.right - BTNSIZE  ;
	rectBtn.bottom = BTNSIZE ;
	m_pBtnClose.Create(NULL, dwStyleBtn, rectBtn, this,  IDC_BUTTON2);
	m_pBtnClose.LoadBitmaps(IDB_CAMGROUP_DEL);

    rectBtn.right = rectBtn.left ;
	rectBtn.left = rectBtn.right - BTNSIZE ;
	
	m_pBtnOpenDirectory.Create(NULL, dwStyleBtn, rectBtn, this, IDC_BUTTON1 );//
	m_pBtnOpenDirectory.LoadBitmaps(IDB_CAMGROUP_EDIT);
	m_vctRectBtn.push_back(rectBtn);

	m_ToolTip= new CToolTipCtrl();
	m_ToolTip->Create(this);
	m_ToolTip->AddTool(&m_pBtnOpenDirectory, _T("Open Directory"));	

	m_rectDraw = m_rectDlg;
	m_rectDraw.left += m_rectDlg.Width()/8;
	m_rectDraw.top += m_rectDlg.Height()/8;
	m_rectDraw.right = m_rectDlg.Width()/2;
	m_rectDraw.bottom = m_rectDlg.Height()/2;
	
	// calculate rectangle according to the orignal video ratio
	int nXDest(0), nYDest(0), nWidth, nHeight;
	float fWscale, fHscale;
	fWscale = (float)abs(m_rectDraw.right) / (float)abs(m_bih.biWidth);
	fHscale = (float)abs(m_rectDraw.bottom) / (float)abs(m_bih.biHeight);
	if ( fWscale > fHscale )
	{
		float fScale = (float)abs(m_bih.biWidth)/(float)abs(m_bih.biHeight);
		nWidth = m_rectDraw.bottom* fScale ;//fHscale * m_rectVideo.Width();
		nHeight = m_rectDraw.bottom;// * -1;
		nXDest = m_rectDraw.left + (m_rectDraw.right - nWidth)/2;
		nYDest = m_rectDraw.top;
	}else
	{
		float fScale = float((float)abs(m_bih.biHeight) / (float)abs(m_bih.biWidth));
		nWidth =  m_rectDraw.right;
		nHeight = ((float)m_rectDraw.right) * fScale ;//* -1;
		nXDest = m_rectDraw.left;
		nYDest =  m_rectDraw.top + (m_rectDraw.bottom - nHeight)/2;
	}
	m_rectDraw = CRect(	nXDest, nYDest, nWidth, nHeight);
	m_rectDrawOrg = m_rectDraw;	
	
	
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
	m_snapshotViewer = new CSnapshotViewer;
	m_snapshotViewer->Create(_T(" "), dwStyle, m_rectDraw , this ,IDC_SCREEN);
	if(m_picture_buf)
	m_snapshotViewer->vGetData(m_picture_buf, m_bih, m_biSizeImage,  m_rectDraw);
	m_snapshotViewer->SetWindowPos(NULL,m_rectDraw.left,m_rectDraw.top,m_rectDraw.Width(),m_rectDraw.Height(),SWP_SHOWWINDOW);			
	
	
	return 0;
}
void CDlgSnapshot::vGetBackGround()
{	
	
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetDesktopWindow()->GetWindowRect(rect);
	//CDC MemDC;   
	//CBitmap m_MemBitmap; 
	m_MemDC.CreateCompatibleDC(&dc);
	//CBrush brush(RGB(100,100,100));
	//m_MemDC.FillRect(rect,&brush);
	m_MemBitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	CGdiObject *pOldBit = m_MemDC.SelectObject(&m_MemBitmap);
	BitBlt(m_MemDC.m_hDC,0,0,rect.Width(),rect.Height(),dc.m_hDC,0,0,SRCCOPY);
	//for(int i = 0; i<rect.Height();i++) // height
	//{
	//	for(int j = 0 ; j < rect.Width(); j++) // width
	//	{
	//		m_MemDC.SetPixelV(j,i,dc.GetPixel(j,i)); 
	//	}
	//}
	
	//BLENDFUNCTION bf; 
 //   bf.BlendOp = AC_SRC_OVER;
 //   bf.BlendFlags = 0;
 //   bf.SourceConstantAlpha = 0x7f;  // half of 0xff = 50% transparency (0x7f) 
 //   bf.AlphaFormat = 0;             // ignore source alpha channel 	
	//
	//bool ret = AlphaBlend( m_MemDC.m_hDC, rect.Width(), rect.Height(), 
	//						rect.Width(), rect.Height(), 
	//						m_MemDC.m_hDC, 0, 0, rect.Width(), rect.Height(), bf);

	//dc.BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&m_MemDC,0,0,SRCCOPY);
	//m_MemBitmap.DeleteObject();
	//m_MemDC.DeleteDC();	
}
void  CDlgSnapshot::DrawAlphaBlend (HWND hWnd, HDC hdcwnd)
{
    HDC hdc;               // handle of the DC we will create  
    BLENDFUNCTION bf;      // structure for alpha blending 
    HBITMAP hbitmap;       // bitmap handle 
    BITMAPINFO bmi;        // bitmap header 
    VOID *pvBits;          // pointer to DIB section 
    ULONG   ulWindowWidth, ulWindowHeight;      // window width/height 
    ULONG   ulBitmapWidth, ulBitmapHeight;      // bitmap width/height 
    RECT    rt;            // used for getting window dimensions 
    UINT32   x,y;         
		// stepping variables 
    UCHAR ubAlpha;         // used for doing transparent gradient 
    UCHAR ubRed;        
    UCHAR ubGreen;
    UCHAR ubBlue;
    float fAlphaFactor;    // used to do premultiply 
            
    // get window dimensions 
    GetClientRect(&rt);
    
    // calculate window width/height 
    ulWindowWidth = rt.right - rt.left;  
    ulWindowHeight = rt.bottom - rt.top;  
	ulBitmapWidth =  ulWindowWidth;
	ulBitmapHeight = ulWindowHeight;

    // make sure we have at least some window size 
    if ((!ulWindowWidth) || (!ulWindowHeight))
        return;

    // divide the window into 3 horizontal areas 
    //ulWindowHeight = ulWindowHeight / 3;

    // create a DC for our bitmap -- the source DC for AlphaBlend  
    hdc = CreateCompatibleDC(hdcwnd);
    
    // zero the memory for the bitmap info 
    ZeroMemory(&bmi, sizeof(BITMAPINFO));

    // setup bitmap info  
    // set the bitmap width and height to 60% of the width and height of each of the three horizontal areas. Later on, the blending will occur in the center of each of the three areas. 
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = ulWindowWidth;
    bmi.bmiHeader.biHeight = ulWindowHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;         // four 8-bit components 
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = ulBitmapWidth * ulBitmapHeight * 4;

    // create our DIB section and select the bitmap into the dc 
   hbitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0x0);
   SelectObject(hdc, hbitmap);
	
  	
    // in top window area, constant alpha = 50%, but no source alpha 
    // the color format for each pixel is 0xaarrggbb    	

    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = 0x7f;  // half of 0xff = 50% transparency (0x7f) 
    bf.AlphaFormat = 0;             // ignore source alpha channel 

	bool ret;
    ret = AlphaBlend(hdcwnd, ulWindowWidth, ulWindowHeight, 
                    ulBitmapWidth, ulBitmapHeight, 
                    hdc, 0, 0, ulBitmapWidth, ulBitmapHeight, bf);

       // return;        // alpha blend failed
	   if(ret == false)
		   return;
 
    // do cleanup 
    DeleteObject(hbitmap);
    DeleteDC(hdc);    
}

BOOL CDlgSnapshot::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	::SetCursor(LoadCursor(NULL,IDC_SIZEALL));

	if(m_snapshotViewer)
	{
		m_snapshotViewer->OnMouseWheel(nFlags, zDelta, pt);
	}
	return true;
}

void CDlgSnapshot::OnMouseMove(UINT nFlags, CPoint point)
{		
	if(m_snapshotViewer)
	{
		m_snapshotViewer->OnMouseMove(nFlags,point);
	}
	return CDialog::OnMouseMove(nFlags, point);
	
}

void CDlgSnapshot::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetDesktopWindow()->GetWindowRect(rect);
	/*CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	CGdiObject *pOldBit = memdc.SelectObject(&m_MemBitmap);*/
	//dc.SelectObject(m_MemBitmap);	
	dc.BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&m_MemDC,0,0,SRCCOPY);
}

void CDlgSnapshot::OnLButtonDown(UINT nFlags, CPoint point)
{	
	
	if(m_snapshotViewer)
	{
		m_snapshotViewer->OnLButtonDown(nFlags,point);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgSnapshot::OnBnClickedClose()
{

}
void CDlgSnapshot::OnBnClickedOpenDirectory(UINT nCmdID)
{
	EndDialog(0);

}

void CDlgSnapshot::OnLButtonUp(UINT nFlags, CPoint point)
{	

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CDlgSnapshot::PreTranslateMessage(MSG* pMsg)
{
	if(m_ToolTip != NULL)
		m_ToolTip->RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgSnapshot::OnBnClickedButton1()
{
	TCHAR szParm[MAX_PATH];
    _stprintf(szParm, _T("explorer /Select, %s"), m_csFileName.GetBuffer());
    STARTUPINFO ssi;
    PROCESS_INFORMATION ppi;
    ZeroMemory( &ssi, sizeof(ssi) );
    ssi.cb = sizeof(ssi);
	CreateProcess(NULL, szParm,NULL, NULL, FALSE, 0, NULL, NULL, &ssi, &ppi ) ;

}

void CDlgSnapshot::OnBnClickedButton2()
{
	
	EndDialog(0);
}

BOOL CDlgSnapshot::OnEraseBkgnd(CDC* pDC)
{
//return CDialog::OnEraseBkgnd(pDC);
	//pDC->SetBkMode(TRANSPARENT);
//	CRect rect;
	//GetDesktopWindow()->GetWindowRect(rect);
	//pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&m_MemDC,0,0,SRCCOPY);
	
	return true;
}

HBRUSH CDlgSnapshot::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode(TRANSPARENT);
	if(CTLCOLOR_DLG == nCtlColor)
    {
       
        return (HBRUSH)GetStockObject(NULL_BRUSH);
    }
	return hbr;
}
