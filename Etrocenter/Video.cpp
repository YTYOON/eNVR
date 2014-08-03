// Video.cpp : implementation file
//

#include "stdafx.h"
#include "Video.h"
#include "Vfw.h"
#include "NVR.h"

#include "MainFrm.h"
#include <Gdiplus.h>
#include "DlgSnapshot.h"
#include "DlgSnapshotDummy.h"
#include "./TextDesigner/OutlineText.h"

//#include "imgLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//#define TEXTCOLOR RGB(255,255,255)
//#define BKGNDCOLOR RGB(54,54,54)

#define VIDEOWAITTIME 10000
#define nOSDHeight 26

/////////////////////////////////////////////////////////////////////////////
// CVideo
CSendAudio CVideo::sendAudio;
CVideo* CVideo::m_pVideoAudio(NULL);
#ifdef PANORAMA
WNDPROC CVideo::DisplayWndProc(NULL); //implement
#endif

IMPLEMENT_DYNAMIC(CVideo, CStatic)
CVideo::CVideo():
m_bUpsideDown(false),m_bFlip(false),m_picture_buf(NULL), m_pImage(NULL),
m_hVideoThread(NULL),m_hAVReceiveThread(NULL),m_playfps(0),m_pVideoInfo(NULL),m_pAudioInfo(NULL),m_pNode(NULL),
m_hSendAudioThread(NULL),m_nSuspendMode(NULL),m_hSendAudioStopEvent(NULL),m_bRecording(false),m_rectVideo(0,0,0,0),m_rectDraw(0,0,0,0)
,m_bReCalcRect(false),m_bFixAspectRatio(true),m_nPlayBreak(20),m_bPtzFreelance(FALSE),bMouseDown(false)
,m_bAviFileChangeFlag(true)
,m_bOriginalRect(true),m_nRectIdx(0)
,m_hDecodEvent(NULL)
,m_zDelta(0),m_bDrag(false),m_nMsgIdx(NULL)
,m_hStopEvent(NULL),m_bAudioStart(false),m_nVideoIdx(0),m_bAutoPlay(false),m_nFPS(0),m_bSetAudio(false)
,m_bih(),m_tzMsg(),m_nClickMode(0),m_fxScale(0.),m_fyScale(0.),m_bClosePIP(true),m_pOSD(NULL)
#ifdef PANORAMA
,m_pPanorama(NULL), m_bPanorama(false),m_pPanoramaBuff(NULL),nPreWidth(0),nPreHeight(0)
#endif
,m_pStreamStatus(NULL)
,m_pszGreyDib(NULL),m_bStopRecvStream(false)
{
	m_pbk = CNVRBk::GetBkObject();
	m_PickPoint.x = 0;
	m_PickPoint.y = 0;
	_tcscpy(m_tzMsg[0], _T("disconnected"));
	_tcscpy(m_tzMsg[1], _T("disconnected"));
	_tcscpy(m_tzMsg[2], _T("reconnecting"));
	_tcscpy(m_tzMsg[3], _T(""));
}

CVideo::~CVideo()
{
    DELETEARRAY(m_pszGreyDib);
    if (m_pVideoAudio == this)
        m_pVideoAudio = NULL;
#ifdef PANORAMA
   // AfxMessageBox(_T("TEST1"))
    //m_pPanorama->FreeDisplay();
    //DELETEOBJ(m_pPanorama);
    //AfxMessageBox(_T("TEST2"));
#endif
}


BEGIN_MESSAGE_MAP(CVideo, CStatic)
	//{{AFX_MSG_MAP(CVideo)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_COMMAND(IDC_SET_FIX_RATIO, OnSetFixRatio)
	ON_COMMAND(IDC_SET_VIDEOSET_UPDISEDOWN, OnSetVideosetUpdisedown)
	ON_COMMAND(IDC_SET_VIDEOSET_FLIP, OnSetVideosetFlip)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_COMMAND(ID_MENU_SNAPSHOT, &CVideo::OnSnapshot)
	ON_COMMAND(ID_MENU_EXITFULLSCREEN, OnMenuExitfullscreen)
#ifdef PANORAMA
	ON_COMMAND(IDM__MENU_PANORAMA180, &CVideo::OnSetParama180)
	ON_COMMAND(IDM__MENU_PANORAMA360, &CVideo::OnSetParama360)    
#endif
	ON_COMMAND(IDM_MENU_OSD, &CVideo::OnMenuOSD)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideo message handlers
BOOL CVideo::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (message)
	{
		if (m_bPtzFreelance)
		{
			if (bMouseDown)
			{
				::SetCursor(((CEtroExplorerApp*)AfxGetApp())->m_hCursor[1]);
			}else
			{
				::SetCursor(((CEtroExplorerApp*)AfxGetApp())->m_hCursor[0]);
			}
		}
		else if(m_bOriginalRect == false)
		{
			::SetCursor(LoadCursor(NULL,IDC_SIZEALL));
		}
		else if(m_bDrag)
		{	
				//vBeginDrag();
				::SetCursor(((CEtroExplorerApp*)AfxGetApp())->m_hCursor[3]);
		}
	
		else
		{
			::SetCursor(LoadCursor(NULL,IDC_ARROW));
		}

		return true;
	}
	::SetCursor(LoadCursor(NULL,IDC_ARROW));
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}
void CVideo::OnPtzFreelance() 
{
	m_bPtzFreelance ^=1;
}

int CVideo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	GetClientRect(&m_rectVideo);
	m_hbr   =   CreateSolidBrush(OSDBKGNDCOLOR); 
    CDC* pDc = GetDC();
    if (!pDc) return 0;
    m_dcBuf.CreateCompatibleDC(pDc);
    CPen* pPen = (CPen*)m_dcBuf.SelectStockObject(WHITE_PEN);
    CPen* pOldPen = m_dcBuf.SelectObject(pPen);
    ReleaseDC(pDc);
	return 0;
}
//
//AVFrame* CVideo::alloc_avpicture(PixelFormat pix_fmt, int width, int height)
//{
//	AVFrame *picture = NULL;
//	int size;
//
//	picture = avcodec_alloc_frame();
//	if ( ! picture )
//		return NULL;
//	size = avpicture_get_size(pix_fmt,width,height);
//	m_picture_buf = (unsigned char *)av_malloc(size);
//	if ( ! m_picture_buf )
//	{
//		av_free(picture);
//		return NULL;
//	}
//	avpicture_fill((AVPicture *)picture, m_picture_buf, pix_fmt, width, height);
//	return picture;
//}
//


void CVideo::vTextOutLine(CDC* pdc, TCHAR* tzCaption, WORD nFontSize, CRect* pRc)
{
#define OUTLINE1 Color(50,50,50)
#define OUTLINE2 Color(255, 128, 128, 128)
#define OUTLINSHADOW Gdiplus::Color(128,30,30,30)


	using namespace Gdiplus;
	using namespace TextDesigner;

	Graphics graphics(pdc->GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	OutlineText m_OutlineText;
	m_OutlineText.TextOutline(
		OUTLINE1, 
		OUTLINE2, 
		5);

	m_OutlineText.EnableShadow(true);
	//Rem to SetNullShadow() to release memory if a previous shadow has been set.
	m_OutlineText.SetNullShadow();
	m_OutlineText.Shadow(
		OUTLINSHADOW, 5, 
		Gdiplus::Point(4,4));

	FontFamily fontFamily(L"Arial");


    WORD nPosX = (pRc->Width() - nFontSize/2 * _tcslen(tzCaption))/2;
    WORD nPosY = (pRc->Height() - nFontSize )/2;

	StringFormat strFormat;
	m_OutlineText.DrawString(&graphics,&fontFamily, 
		FontStyleBold, nFontSize, tzCaption, 
		Gdiplus::Point(nPosX, nPosY), &strFormat);

}

void CVideo::DrawText(HDC hDC)
{
	CDC *pDC = CDC::FromHandle(hDC);
	int fontSize = m_rectVideo.right/12;	
	vTextOutLine(pDC, m_tzMsg[m_nMsgIdx], fontSize, &m_rectVideo);
}

void CVideo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if ( m_nMsgIdx != MSG_NORMAL)
	{
        if (m_pszGreyDib)
        {  
            DrawPicture2(dc.m_hDC, m_pszGreyDib +sizeof(BITMAPINFOHEADER));
        }
		DrawText(dc.m_hDC);
	}
	else
    {
		DrawPicture2(dc.m_hDC, m_picture_buf);

    }
    if(m_bDisplayOSD)
    {
       m_pOSD->Invalidate(false);
       m_pOSD->UpdateWindow();  
	}
}
void CVideo::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
{
	CWnd::MoveWindow(lpRect, bRepaint);
	CRect rectOld(m_rectVideo);
	InvalidateRect(rectOld,true);

 	GetClientRect(m_rectVideo);
	//m_rectVideo.DeflateRect(2, 2);
	//m_rectVideo.right = m_rectVideo.right -2;
	//m_rectVideo.bottom = m_rectVideo.bottom -2;
	m_bReCalcRect = true;
	bResetDrawRect();
    if(m_bDisplayOSD)
    {
	    vRearrangeOSD();
    }
#ifdef PANORAMA
    if(m_pPanorama && m_bPanorama)
	{
		CRect rectPano;
		rectPano = m_rectVideo;
		rectPano.top += OSDHEIGHT;
	    m_pPanorama->MoveWindow(rectPano.Width(),rectPano.Height());
     }
#endif
}

void CVideo::vStopRecv()
{
	int nRetry = 0;
	SetStopRecv(true);
	SetStopRecvStream(true);
	if (m_hAVReceiveThread)
	{
		DWORD dwResult;
		dwResult = ::WaitForSingleObject(m_hAVReceiveThread, VIDEOWAITTIME);

		if (dwResult != WAIT_TIMEOUT) 
		{
			CloseHandle( m_hAVReceiveThread );
		}
		else
		{
			::TerminateThread(m_hAVReceiveThread, 0);
		}
		m_hAVReceiveThread = NULL;
	}
}

void CVideo::vStopPlay()
{
	SetStopPalay(true);
	SetStopRecv(true);
	SetStopRecvStream(true);
	if (m_hStopEvent)
	{
		SetEvent(m_hStopEvent);
	}
	DWORD dwResult;
	if (m_hVideoThread)
	{
		dwResult = ::WaitForSingleObject(m_hVideoThread, VIDEOWAITTIME);
		if (dwResult != WAIT_TIMEOUT) 
		{
			CloseHandle( m_hVideoThread );
		}
		else
		{
			::TerminateThread(m_hVideoThread, 0);
		}
		m_hVideoThread = NULL;
	}
}
void CVideo::LogException(LPEXCEPTION_POINTERS pExceptPtrs)
{
    // save the data from the current exception information
    EXCEPTION_RECORD er = *(pExceptPtrs->ExceptionRecord);
//    CONTEXT ctx = *(pExceptPtrs->ContextRecord);

    // show the exception code
    switch(er.ExceptionCode)
    {
    case EXCEPTION_ACCESS_VIOLATION: 
        break;
    default:
        break;
    }
}

LONG CVideo::AppExceptionPolicy(LPEXCEPTION_POINTERS pExceptPtrs)
{
    // log the exception
    CVideo::LogException(pExceptPtrs);

    // the policy might do something interesting based on the
    //  exception code
    LONG nFilterCode(0);

    // grab the record and decide on an action
    EXCEPTION_RECORD er = *(pExceptPtrs->ExceptionRecord);
    switch(er.ExceptionCode)
    {
	case 0:
    default: nFilterCode = EXCEPTION_EXECUTE_HANDLER;        
        break;
    }
    TCHAR tz[255];
    _stprintf(tz, _T("er.ExceptionCode :%x\r\n"), er.ExceptionCode); 
    OutputDebugString(tz);
    return(nFilterCode);
}
void CVideo::vReleaseAVObject()
{
	////	
	//__try{
	//	if( m_picture )	av_free(m_picture);
	//	m_picture = NULL; 

	//	if ( m_rgb32 )	av_free(m_rgb32);
	//	m_rgb32 = NULL;

	//	if( m_c )		avcodec_close(m_c);
	//	m_c = NULL;	
	//	m_pImage = NULL;
	//}__except(AppExceptionPolicy(GetExceptionInformation()))
	//{
	//}
}
UINT __stdcall CVideo::VideoPlayThread(LPVOID lpParam)
{
	CVideo* pThis = ((CVideo*)lpParam);
	pThis->SetStopRecvStream(false);
  
    pThis->m_hStopEvent = ::CreateEvent(NULL, true, false, NULL);
	while (pThis->GetStopRecvStream() == false)
	{
		pThis->VideoPlayThreadRunPack();
        Sleep(100);
	}
    pThis->m_nMsgIdx = MSG_LOGO;
	pThis->vReleaseAVObject();
	CLOSEHANDLE(pThis->m_hStopEvent);
    return 0;
}

void CVideo::vHintInfo()
{

	CDC* pDC = GetDC();
	CString cs;
//	cs.Format(_T("fps:  %02d retrytime: %d ReadError: %d VSMP Error: %d DataSize Error: %d StartTime:%02d:%02d:%02d Duration:%02d:%02d:%02d"),m_playfps, m_RetryTime,m_RetryRead, m_RetryVSMP, m_RetryDataSize, 
//		startTime.wHour, startTime.wMinute, startTime.wSecond, TimeDuration.wHour, TimeDuration.wMinute, TimeDuration.wSecond);
	cs.Format(_T("FPS: %02d"),m_playfps);
	pDC->TextOut(0, 0, cs);
//	TRACE(_T("playfps = %d; m_RetryTime =%d\r\n"), m_playfps, m_RetryTime);
//	m_playfps =playfps;
//	fpsTime = (LONG)tempNode->vFrame->sec;
//	playfps = 0;
	ReleaseDC(pDC);

}
/*
bool CVideo::VideoPlayThreadRunInit()
{
//	FUNCTRACE(_T("VideoPlayThreadRunInit"));
    AVCodec *codec = NULL;
	CodecID	codecID;
	if( m_pVideoInfo == NULL ) return false;

	// Convert FOURCC to CodecID.
	switch( m_pVideoInfo->encodingType ) {
		case FCC_MJPG :	codecID = CODEC_ID_MJPEG; break;
		case FCC_MP4S :	codecID = CODEC_ID_MPEG4; break;
		case FCC_DX50 :	codecID = CODEC_ID_MPEG4; break;
		case FCC_MPG2:	codecID = CODEC_ID_MPEG2VIDEO; break;
		case FCC_MPG1:	codecID = CODEC_ID_MPEG1VIDEO; break;
		case FCC_H263 :	codecID = CODEC_ID_H263; break;
		case FCC_H264 : codecID = CODEC_ID_H264; break;
		default:	return false;
	}

	avcodec_init();
// 	av_set_memory(malloc,free,realloc);
	avcodec_register_all();
	codec = avcodec_find_decoder(codecID);
	if (!codec)  return false;

	m_c = avcodec_alloc_context();
	if (!m_c)
	{
		return false;
	}
	m_picture = avcodec_alloc_frame();
//	m_c->width =  m_pVideoInfo->videoWidth;
//	m_c->height = m_pVideoInfo->videoHeight;
	m_c->workaround_bugs = 1;
	m_c->error_concealment	= 3;
	//m_c->error_resilience = 1;

	if (avcodec_open(m_c, codec) < 0) 
	{
		av_free( codec );
		m_c = NULL;
		return false;
	}
	if (m_rgb32)
	{
		av_free(m_rgb32);
		m_rgb32 = NULL;
	}
	return true;
}

*/
void CVideo::DrawPicture(HDC hDC, BYTE *image)
{

	int height = m_bih.biHeight;
	height = m_bUpsideDown ? height*-1 : height;

	::SetDIBitsToDevice(hDC,                    // hDC
      m_rectDraw.left,//(m_rectVideo.Width() - m_bih.biWidth)/2,             // DestX
      m_rectDraw.top ,//(m_rectVideo.Height() + m_bih.biHeight)/2,              // DestY
      m_bih.biWidth,        // nDestWidth
      m_bih.biHeight*-1,       // nDestHeight
      0,            // SrcX
      0,            // SrcY
      0,                          // nStartScan
      -1*height,  // nNumScans
      image,                  // lpBits
      (LPBITMAPINFO)&m_bih,     // lpBitsInfo
      DIB_RGB_COLORS);            // wUsage

}
//void Blur2(DWORD* source, DWORD* dest, DWORD width, DWORD height, int radius) 
//{
//    int x, y, kx, ky, total;
//    for (y = 0; y < height; ++y) 
//    {
//        for (x = 0; x < width; ++x) 
//        {
//            total = 0;
//            for (ky = -radius; ky <= radius; ++ky)
//            {
//                for (kx = -radius; kx <= radius; ++kx)
//                    total += source[x + kx, y + ky];
//            }
//            dest[x, y] = total / (radius * 2 + 1) ^ 2;  
//        }
//    }
//};
#define CLAMP(x, a, b) ((x < a) ? a : (x > b) ? b : x)

void Blur(DWORD* in, DWORD* out, int width, int height, int radius)
{
    int widthMinus1 = width-1;
    int tableSize = 2*radius+1;
    int* divide = new int[256*tableSize];

    // the value scope will be 0 to 255, and number of 0 is table size
    // will get means from index not calculate result again since 
    // color value must be  between 0 and 255.
    for ( int i = 0; i < 256*tableSize; i++ )
        divide[i] = i/tableSize; 

    int inIndex = 0;
    
    // 
    for ( int y = 0; y < height; y++ ) {
        int outIndex = y;
        int ta = 0, tr = 0, tg = 0, tb = 0; // ARGB -> prepare for the alpha, red, green, blue color value.
        for ( int i = -radius; i <= radius; i++ ) {
            int rgb = in[inIndex + CLAMP(i, 0, width-1)]; // read input pixel data here. table size data.
            ta += (rgb >> 24) & 0xff;
            tr += (rgb >> 16) & 0xff;
            tg += (rgb >> 8) & 0xff;
            tb += rgb & 0xff;
        }

        for ( int x = 0; x < width; x++ ) { // get output pixel data.
            out[ outIndex ] = (divide[ta] << 24) | (divide[tr] << 16) | (divide[tg] << 8) | divide[tb]; // calculate the output data.

            int i1 = x+radius+1;
            if ( i1 > widthMinus1 )
                i1 = widthMinus1;
            int i2 = x-radius;
            if ( i2 < 0 )
                i2 = 0;
            int rgb1 = in[inIndex+i1];
            int rgb2 = in[inIndex+i2];
            
            ta += ((rgb1 >> 24) & 0xff)-((rgb2 >> 24) & 0xff);
            tr += ((rgb1 & 0xff0000)-(rgb2 & 0xff0000)) >> 16;
            tg += ((rgb1 & 0xff00)-(rgb2 & 0xff00)) >> 8;
            tb += (rgb1 & 0xff)-(rgb2 & 0xff);
            outIndex += height; // per column or per row as cycle...
        }
        inIndex += width; // next (i+ column number * n, n=1....n-1)
    }
    delete [] divide;
}
void CVideo::GreyPicture(BITMAPINFOHEADER* pbih, BYTE *image)
{
	DWORD nWidth = abs(pbih->biWidth);
	DWORD nHeight = abs(pbih->biHeight);
	BYTE* pInx = image;
	for (int ci = 0; ci < nHeight; ci++)
	{
		pInx = image + ci * WIDTHBYTES(nWidth * pbih->biBitCount);
		for (int cj = 0; cj < nWidth; cj++)
		{
            DWORD nDepth =  (pbih->biBitCount /8);
			DWORD nPixel = cj *  nDepth;
			*(pInx +nPixel) = *(pInx +nPixel +3) = *(pInx +nPixel +2)  = *(pInx +nPixel +1);
            (nDepth > 3) ? (*(pInx +nPixel +3) = *(pInx +nPixel +1)) : NULL;
		}
	}
/////////////////////////////////////////////////
    if (m_pszGreyDib)
    {
        DELETEARRAY(m_pszGreyDib);
    }
    int size = pbih->biSizeImage;
    size = sizeof(BITMAPINFOHEADER) +WIDTHBYTES(pbih->biWidth * pbih->biBitCount)*abs(pbih->biHeight);
    m_pszGreyDib = new BYTE[size];
    memcpy(m_pszGreyDib, pbih, sizeof(BITMAPINFOHEADER));
    memcpy(m_pszGreyDib +sizeof(BITMAPINFOHEADER), image, WIDTHBYTES(pbih->biWidth * pbih->biBitCount)*abs(pbih->biHeight));

    //Blur((DWORD*)image, (DWORD*)(m_pszGreyDib +sizeof(BITMAPINFOHEADER)), pbih->biWidth, abs(pbih->biHeight), 3);
}




#pragma optimize( "", off )
void CVideo::PanoramaUpdateNewFrame( BYTE *image)
{
    __try
    {
        //TCHAR tz[256];
        //_stprintf(tz, _T("%x %x\r\n"), m_pPanoramaBuff, image);
        //OutputDebugString(tz);
        memcpy(m_pPanoramaBuff, image, WIDTHBYTES( m_bih.biWidth * m_bih.biBitCount) * abs(m_bih.biHeight));//m_bih.biBitCount 
        //OutputDebugString(_T("memcpy\r\n"));
        BOOL bRet = m_pPanorama->UpdateNewFrame();
        //OutputDebugString(_T("UpdateNewFrame\r\n"));
    }__except(AppExceptionPolicy(GetExceptionInformation()))
    {
        OutputDebugString(_T("PanoramaUpdateNewFrame exception\r\n"));
    }
}
#pragma optimize( "", on )
void CVideo::DrawPicture2(HDC hDC, BYTE *image)
{
    if (image)
    { 
#ifdef PANORAMA
        if(m_bPanorama && m_pPanorama)
		{
            // method 2
            //ETROLOG(_T("Enter UpdateNewFrame"));
            //memcpy(m_pPanoramaBuff, image, WIDTHBYTES( m_bih.biWidth * m_bih.biBitCount) * abs(m_bih.biHeight));//m_bih.biBitCount 
            //BOOL bRet = m_pPanorama->UpdateNewFrame();
            PanoramaUpdateNewFrame(image);			
		}
        else 
#endif
        {          
            if (m_bOriginalRect)
            {
                SetStretchBltMode(hDC, COLORONCOLOR);
                if (GDI_ERROR == StretchDIBits( hDC, 
                    m_rectDraw.left, m_rectDraw.top, m_rectDraw.right, m_rectDraw.bottom, 
                    m_rcImg.left, m_rcImg.top, m_rcImg.Width(), m_rcImg.Height(),
                    (LPVOID)image, (BITMAPINFO*)&m_bih,
                    DIB_RGB_COLORS, SRCCOPY ))
                {
                    SDKERRMSG(TRUE);
                    TRACE(_T("ERROR"));
                }
            }
            else
            {
                bool bRet(true);
                CRect rect(0, 0, m_rectVideo.Width(), m_rectVideo.Height());
                HANDLE m_hBmp = CreateCompatibleBitmap(hDC, m_rectVideo.Width(), m_rectVideo.Height());
                HBITMAP hOldBmp = (HBITMAP)m_dcBuf.SelectObject(m_hBmp);
                DeleteObject(hOldBmp);   
                CBrush bkColor(BKCOLOR);
	            m_dcBuf.FillRect(&rect,&bkColor);	
                
                SetStretchBltMode(hDC, COLORONCOLOR);	
                SetStretchBltMode(m_dcBuf, COLORONCOLOR);	
                if (GDI_ERROR == StretchDIBits( m_dcBuf, 
	                m_rectDraw.left, m_rectDraw.top, m_rectDraw.right, m_rectDraw.bottom, 
	                m_rcImg.left, m_rcImg.top, m_rcImg.Width(), m_rcImg.Height(),
	                (LPVOID)image, (BITMAPINFO*)&m_bih,
	                DIB_RGB_COLORS, SRCCOPY ))
                {
	                SDKERRMSG(TRUE);
	                TRACE(_T("ERROR"));
                }else
                {
                    if (!m_bClosePIP)
                    {
                        CRect rectTmp = m_rcThumbnail;
                        rectTmp.right = rectTmp.left + rectTmp.right ;
                        rectTmp.bottom = rectTmp.top + rectTmp.bottom;
                        int nOffset = abs(m_rcThumbnail.bottom)/20;
                        rectTmp.OffsetRect(nOffset, nOffset);
                        CBrush brush(RGB(30,30,30));
                        m_dcBuf.FillRect(rectTmp, &brush);	
                        StretchDIBits( m_dcBuf, 
                                        m_rcThumbnail.left, m_rcThumbnail.top, m_rcThumbnail.right, m_rcThumbnail.bottom,
	                                    //m_rectVideo.right-320, m_rectVideo.bottom-180, 320, 180, 
	                                    0, 0,  m_bih.biWidth, -m_bih.biHeight,
	                                    (LPVOID)image, (BITMAPINFO*)&m_bih,
	                                    DIB_RGB_COLORS, SRCCOPY );
                        CPen PenLine( PS_SOLID, 1, RGB(250,0,0));
                        CPen PenOuter( PS_SOLID, 1, RGB(250,250,250));
		                CPen *pOldPen =	m_dcBuf.SelectObject( &PenOuter);
                        m_dcBuf.MoveTo(m_rcThumbnail.left, m_rcThumbnail.top);
                        m_dcBuf.LineTo(m_rcThumbnail.left+m_rcThumbnail.right, m_rcThumbnail.top);
                        m_dcBuf.LineTo(m_rcThumbnail.left+m_rcThumbnail.right, m_rcThumbnail.top+m_rcThumbnail.bottom);
                        m_dcBuf.LineTo(m_rcThumbnail.left, m_rcThumbnail.top+m_rcThumbnail.bottom);
                        m_dcBuf.LineTo(m_rcThumbnail.left, m_rcThumbnail.top);
                        m_dcBuf.SelectObject(pOldPen);
		                pOldPen = m_dcBuf.SelectObject( &PenLine);
                        m_dcBuf.MoveTo(m_rcCrop.left, m_rcCrop.top);
                        m_dcBuf.LineTo(m_rcCrop.right, m_rcCrop.top);
                        m_dcBuf.LineTo(m_rcCrop.right, m_rcCrop.bottom);
                        m_dcBuf.LineTo(m_rcCrop.left, m_rcCrop.bottom);
                        m_dcBuf.LineTo(m_rcCrop.left, m_rcCrop.top);
                        m_dcBuf.SelectObject(pOldPen);
                    }else
                    {
                        m_bOriginalRect = true;
                    }
                    ::BitBlt(hDC, m_rectVideo.left, m_rectVideo.top + nOSDHeight,
                            m_rectVideo.Width(), m_rectVideo.Height(), 
                            m_dcBuf, m_rectVideo.left,  m_rectVideo.top + nOSDHeight, SRCCOPY);
                }
            }
        }
    }
}
void CVideo::DrawPicture3(HDC hDC, BYTE *image)
{
    bool bRet;
    if (image)
    {
        static bool bFistInit = false;
        static HDRAWDIB m_hDD;
        if (bFistInit == false)
        {
            bFistInit = true;
            m_hDD = DrawDibOpen();
            m_bih.biHeight = abs(m_bih.biHeight);
            bRet = DrawDibBegin(m_hDD, NULL, m_rectDraw.right, m_rectDraw.bottom, 
				     &m_bih, abs(m_bih.biWidth), abs(m_bih.biHeight), 0);	
	       // bRet = DrawDibRealize(m_hDD, hDC, FALSE);
        }
        bRet = DrawDibDraw(m_hDD, hDC,	m_rectDraw.left, m_rectDraw.top, m_rectDraw.right, m_rectDraw.bottom, 
                    &m_bih,(LPVOID)image,
		            m_rcImg.left, m_rcImg.top, abs(m_rcImg.Width()), (m_rcImg.Height()), DDF_SAME_DRAW);
    }
	

}
/*
LONG CVideo::VideoPlayThreadRunPack()
{
	if (m_c == NULL)
	{
		if (!VideoPlayThreadRunInit())
			return -1;

	}
	if (m_pVideoReceiveQue == NULL)
		return -1;
	LONG  size(0), len(0), got_picture(0), playfps(0), fpsTime(0);
	HDC hDC = ::GetDC(m_hWnd);
	bool bFirstTime = true;
	while ( m_bStopPlay == false)
	{
		pQuePoolNode tempNode = m_pVideoReceiveQue->deque();
		if( !tempNode ||  !tempNode->vFrame)  {
			Sleep(5); 
			continue; 
		}

		len = avcodec_decode_video(m_c, m_picture, (PINT)&got_picture, (LPBYTE)tempNode->vFrame->data, tempNode->vFrame->datasize);
		TRACE(_T("Width: %d\tHeight: %d\r\n"), m_c->width, m_c->height);
		if (got_picture)
		{
			// the picture is allocated by the decoder. no need to free it
			// some options....
			if (m_rgb32 == NULL)
			{
				#ifdef OLD_CODEC
				m_bih.biWidth = m_c->width;	m_bih.biHeight = -1 * m_c->height;
				m_rgb32 = alloc_avpicture(PIX_FMT_RGB32, m_c->width, m_c->height);//
				#else
				m_rgb32 = alloc_avpicture(PIX_FMT_RGB32, m_c->width, m_c->height);//
				//				m_rgb32 = alloc_avpicture(PIX_FMT_RGB32, m_rectDraw.Width(),  m_rectDraw.Height());//m_c->width, m_c->height);//
				#endif									
			}
	
			if (m_bReCalcRect || (m_rgb32 == NULL))
			{
				bResetDrawRect();

				m_bih.biBitCount = 32;
				m_pVideoInfo->videoWidth = m_c->width;
				m_pVideoInfo->videoHeight =  m_c->height;
				m_bih.biWidth = m_pVideoInfo->videoWidth;//m_rectDraw.Width();// 
				m_bih.biHeight = -1 *  m_pVideoInfo->videoHeight;//m_rectDraw.Height();//
				m_bih.biSize = sizeof(BITMAPINFOHEADER);
				m_bih.biPlanes = 1;
				m_bih.biCompression = BI_RGB;
			
			}
			// now ColorspaceConverting  YV12 -> RGB32 
			if (m_rgb32 == 0) break;
			#ifdef OLD_CODEC //old code:
				m_size = size =  img_convert((AVPicture *)m_rgb32, PIX_FMT_RGB32,
					(AVPicture *)m_picture, PIX_FMT_YUV420P,
					 m_c->width, m_c->height);
			#else //new code:
				 const int sws_flags = SWS_FAST_BILINEAR;//SWS_POINT | SWS_ACCURATE_RND;//SWS_POINT SWS_BICUBIC;//
				 SwsContext *img_convert_ctx;
//				 int outlinesize[4] = {m_rectDraw.Width(), m_rectDraw.Width()/2, m_rectDraw.Width()/2, 0}; 
				   img_convert_ctx = sws_getContext(m_c->width, m_c->height, PIX_FMT_YUV420P,
													m_c->width, m_c->height,PIX_FMT_RGB32,
													//m_rectDraw.Width(), m_rectDraw.Height(), PIX_FMT_RGB32,
													sws_flags, NULL, NULL, NULL);
				   sws_scale( img_convert_ctx, m_picture->data, m_picture->linesize,
					          0, m_c->height, m_rgb32->data, m_rgb32->linesize);
				   sws_freeContext(img_convert_ctx);

			#endif
			
			m_pImage = m_rgb32->data[0];


			DrawPicture2( hDC, (unsigned char *)m_rgb32->data[0]);

			playfps ++;
			if( tempNode->vFrame && fpsTime != (LONG)tempNode->vFrame->sec )
			{
				fpsTime = (LONG)tempNode->vFrame->sec;
				playfps = 0;
			}		
		}
		if( tempNode ) m_pVideoReceiveQue->freePoolNode(tempNode);
	}
	if( m_pVideoReceiveQue ) m_pVideoReceiveQue->clearQue();
	TRACE(_T("EXIT VideoPlayThreadRunPack\r\n"));
    return 0;
}
*/

void CVideo::OnDestroy() 
{ 
	vStopPlay();
	vStopRecv();
	if(m_pOSD)
		m_pOSD->DestroyWindow();
    DELETEOBJ(m_pOSD);  	
	DeleteObject(m_hbr);
#ifdef PANORAMA
	if (m_pPanorama)
	{
		HWND hHandle=m_pPanorama->GetHWND();
		if (hHandle != INVALID_HANDLE_VALUE)
		{
			 ::DestroyWindow(hHandle);
		}	
	}
#endif
	CStatic::OnDestroy();
}

void CVideo::OnLButtonDown(UINT nFlags, CPoint point) 
{
    SetFocus();
    ScreenToClient(&point);	
		
    if (m_rcPosThumbnail.PtInRect(point))	
    {
        m_nClickMode = MOVE_THUMBNAIL;

    }else
    {
        m_nClickMode = CLICK_NONE;
    }
	if (m_bPtzFreelance)
	{
		m_CursorPos = point;
		bMouseDown = true;
	}
	m_PickPoint = point;
	
	if(m_bDisplayOSD)
	{
		if(m_rectSendAudio.PtInRect(point))
		{
			ASSERT(FALSE);
		}

	}
	m_bLButtonUp = false;
	TRACE(_T("ptX:%d,ptY:%d\r\n"),point.x,point.y);
	TRACE(_T("m_rcThumbnail:%d %d %d %d\r\n"),m_rcThumbnail.left,m_rcThumbnail.top,m_rcThumbnail.Width(),m_rcThumbnail.Height());
	TRACE(_T("m_rcPosThumbnail:%d %d %d %d\r\n"),m_rcPosThumbnail.left,m_rcPosThumbnail.top,m_rcPosThumbnail.Width(),m_rcPosThumbnail.Height());

	CStatic::OnLButtonDown(nFlags, point);
}

void CVideo::OnSetVideosetUpdisedown() 
{ 
	m_bUpsideDown = m_bUpsideDown ^ 1;
	m_pNode->bUpsideDown = m_bUpsideDown;
	m_bReCalcRect = true;
}

void CVideo::OnSetVideosetFlip() 
{
	m_bFlip = m_bFlip ^1;	
	m_pNode->bFlip = m_bFlip;
	m_bReCalcRect = true;
}


void CVideo::OnSetFixRatio() 
{
	m_bReCalcRect = true;
	m_bFixAspectRatio ^=1;
	::PostMessage(GetParent()->m_hWnd, WM_LBUTTONDOWN,NULL,NULL);
}

void CVideo::OnClicked() 
{
	//if (!m_pNode) return;
 //	((CIPCamTreeView*)((CMainFrame*)AfxGetMainWnd())->GetTreeView())->GetTreeCtrl()->SelectItem(m_pNode->hTreeItem);
}

BOOL CVideo::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	::SetCursor(LoadCursor(NULL,IDC_SIZEALL));
	zDelta = zDelta/10;
	int nDeltaTmp = m_zDelta;
	m_zDelta += zDelta;
	if (m_zDelta <= 0) 
	{
		m_zDelta = 0;
		m_rcImg.left = m_rcImg.top = 0;
		m_rcImg.right = m_bih.biWidth;
		m_rcImg.bottom = m_bih.biHeight*-1;
        m_bClosePIP = true; //for reflush and erase background
		return true;
	}
    m_bClosePIP = false;
	m_bOriginalRect = false;
	CRect rcTmp = m_rcImg;
	float fScale = (float)m_bih.biWidth / (float)abs(m_bih.biHeight);
	int nDeflateX = (int)((float)(zDelta)*fScale);
	int nDeflateY = zDelta;
	rcTmp.DeflateRect(nDeflateX, nDeflateY);
	int nWidth = rcTmp.Width();
	int nHeight = rcTmp.Height();
	if ((nWidth < 2) || (nHeight < 2))
	{
		m_zDelta = nDeltaTmp;
		return false;
	}
	if (rcTmp.left < 0)// || (rcTmp.top < 0))
	{
		rcTmp.left =0;
		rcTmp.right = nWidth;
	}
	if (rcTmp.top <0)
	{
		rcTmp.top = 0;
		rcTmp.bottom = nHeight;
	}
	if (rcTmp.right >  m_bih.biWidth)
	{
		rcTmp.right = m_bih.biWidth ;
		rcTmp.left = rcTmp.right - nWidth;
	}
	if (rcTmp.bottom >  abs(m_bih.biHeight))
	{
		rcTmp.bottom = abs(m_bih.biHeight);
		rcTmp.top = rcTmp.bottom - nHeight;
	}
	if (rcTmp.right <=  rcTmp.left)
	{
		swap(rcTmp.right, rcTmp.left);

	}
	if (rcTmp.bottom <=  rcTmp.top )
	{
		swap(rcTmp.top, rcTmp.bottom);
	}
	m_rcImg = rcTmp; 
    m_fxScale = (float)m_rcThumbnail.right/(float)m_rcImg.right
              * (float)m_rcImg.right/ (float)m_bih.biWidth;
    m_fyScale = (float)m_rcThumbnail.bottom/(float)m_rcImg.bottom
              * (float)m_rcImg.bottom/ (float)abs(m_bih.biHeight);
/*
    int nleft, ntop, nright, nbottom;
    nleft = m_rcThumbnail.left +(float)m_rcImg.left*m_fxScale;
    ntop = m_rcThumbnail.top +(float)m_rcImg.top*m_fyScale;
    nright =nleft+ (float)(m_rcImg.right-m_rcImg.left)* m_fxScale;
    nbottom =ntop+ (float)(m_rcImg.bottom-m_rcImg.top)* m_fyScale;
    m_rcCrop =CRect(nleft, ntop, nright, nbottom);*/
    int nleft, ntop, nright, nbottom;
    nleft = m_rcThumbnail.left +(float)m_rcImg.left*m_fxScale;
    ntop =  (m_rcThumbnail.top +  m_rcThumbnail.bottom) - ((float)m_rcImg.top*m_fyScale);
    nright =nleft+ (float)(m_rcImg.right-m_rcImg.left)* m_fxScale;
    nbottom =ntop- (float)(m_rcImg.bottom-m_rcImg.top)* m_fyScale;
    m_rcCrop =CRect(nleft, ntop, nright, nbottom); 
    Invalidate(false);
	//return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}

void CVideo::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bPtzFreelance & nFlags & MK_LBUTTON)
//	if (nFlags & MK_LBUTTON )
	{
          ScreenToClient(&point);
        if (m_nClickMode == MOVE_THUMBNAIL)
        {
            CSize size = point - m_PickPoint;
            m_PickPoint = point;
            m_rcThumbnail.left = m_rcThumbnail.left +size.cx;
            m_rcThumbnail.top = m_rcThumbnail.top +size.cy; 
            
            m_rcPosThumbnail = CRect(m_rcPosThumbnail.left+size.cx, m_rcPosThumbnail.top +size.cy, m_rcPosThumbnail.right +size.cx, m_rcPosThumbnail.bottom +size.cy);            
            int nleft, ntop, nright, nbottom;

            nleft = m_rcThumbnail.left +(float)m_rcImg.left*m_fxScale;
            ntop =  (m_rcThumbnail.top +  m_rcThumbnail.bottom) - ((float)m_rcImg.top*m_fyScale);
            nright =nleft+ (float)(m_rcImg.right-m_rcImg.left)* m_fxScale;
            nbottom =ntop- (float)(m_rcImg.bottom-m_rcImg.top)* m_fyScale;
            m_rcCrop =CRect(nleft, ntop, nright, nbottom);        
        }else
        {
		    int dx = point.x - m_PickPoint.x;
		    int dy = point.y - m_PickPoint.y;
		    if (m_bUpsideDown) dy *= -1;
		    if (m_bFlip) dx *= -1;
            int nWidth = m_rcImg.Width();
            int nHeight = m_rcImg.Height();
		    m_rcImg.left = m_rcImg.left - dx;
		    m_rcImg.top = m_rcImg.top + dy;
		    m_rcImg.right = m_rcImg.left + nWidth;
		    m_rcImg.bottom = m_rcImg.top + nHeight;
		    if (m_rcImg.left < 0)
		    {
			    m_rcImg.left =0;
			    m_rcImg.right = nWidth;
		    }
		    if (m_rcImg.top < 0)
		    {
			    m_rcImg.top = 0;
			    m_rcImg.bottom = nHeight;
		    }
		    if (m_rcImg.right >  m_bih.biWidth)
		    {
			    m_rcImg.right = m_bih.biWidth ;
			    m_rcImg.left = m_rcImg.right -nWidth;
		    }
		    if (m_rcImg.bottom >  abs(m_bih.biHeight))
		    {
			    m_rcImg.bottom = abs(m_bih.biHeight);
			    m_rcImg.top = m_rcImg.bottom - nHeight;
		    }
            //m_rcThumbnail=CRect(2*m_rectVideo.right/3-10, 2*m_rectVideo.bottom/3, m_rectDraw.right/3, m_rectDraw.bottom/3);
            int nleft, ntop, nright, nbottom;
            nleft = m_rcThumbnail.left +(float)m_rcImg.left*m_fxScale;
            ntop =  (m_rcThumbnail.top +  m_rcThumbnail.bottom) - ((float)m_rcImg.top*m_fyScale);
            nright =nleft+ (float)(m_rcImg.right-m_rcImg.left)* m_fxScale;
            nbottom =ntop- (float)(m_rcImg.bottom-m_rcImg.top)* m_fyScale;
            m_rcCrop =CRect(nleft, ntop, nright, nbottom);        
        }
          
	    ::PostMessage(m_hWnd, WM_SETCURSOR, (WPARAM)m_hWnd, MAKELONG(512, 1));

	}
	//return CStatic::OnMouseMove(nFlags, point);
}

void CVideo::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (!m_pNode) return;
	m_pbk->SendMessage(MsgUpdateView,UM_SET_TREENODE_FOCUS,(LPARAM)m_pNode);
	m_bLButtonUp = true;
	CStatic::OnLButtonUp(nFlags, point);
}

void CVideo::OnSnapshot()
{
	if(!m_picture_buf) return;
#ifdef PANORAMA
    if (m_pPanorama)
    {
        m_pPanorama->Snapshot();
    }
    else
#endif
    {
	    typedef DWORD (IMGLIBPROC)(uint8_t*,BITMAPINFOHEADER);
	    IMGLIBPROC* pFunc;
	    HMODULE hRes = ::LoadLibrary(_T("imgLib.DLL"));
        if (hRes)
        {       
            pFunc = (IMGLIBPROC*)::GetProcAddress(hRes, "IMGLIB_OnSnapShot");
            if(pFunc)
            {
                int nRet = (*pFunc)(m_picture_buf, m_bih); 
            }
            FreeLibrary(hRes);

        }
    }

	//CFile file;
	//BITMAPFILEHEADER bmfHdr ={0};
	//BITMAPINFOHEADER bmpHead = m_bih; 
	//bmpHead.biSize = sizeof(BITMAPINFOHEADER);
	//bmpHead.biHeight = (m_bih.biHeight);
	//bmpHead.biBitCount = 32;
	//DWORD nLineWidth = WIDTHBYTES( 32 * bmpHead.biWidth);
	//bmpHead.biSizeImage =  nLineWidth * abs(bmpHead.biHeight);
	//bmpHead.biPlanes = 1;
	//bmpHead.biCompression = BI_RGB;
	//bmpHead.biClrImportant = 0;
	//bmpHead.biClrUsed = 0;
	////DWORD nbits = PIX_FMT_RGB32;

	//bmfHdr.bfType = DIB_HEADER_MARKER;
	//bmfHdr.bfSize = bmpHead.biSizeImage  + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	//bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + bmpHead.biSize + 0; // + PaletteSize((LPSTR)lpBI);
	//CString csFilename;// = CGS::csGetCacheFolder();
	//SYSTEMTIME hosttime;
	//GetLocalTime(&hosttime);
	//CGS gs;
	//csFilename.Format(_T("%s\\%04d%02d%02d%02d%02d%02d.bmp") ,gs.csGetCacheFolder().LockBuffer(), hosttime.wYear, hosttime.wMonth, hosttime.wDay, hosttime.wHour, hosttime.wMinute, hosttime.wSecond);
	//
	//file.Open( csFilename.LockBuffer(), CFile::modeCreate | CFile::modeWrite); 
	//TRY
	//{
	//	BYTE *LPOFFSET;
	//	DWORD dwDIBSaveCnt;
	//	// Write the file header
	//	file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
	//	file.Write((LPSTR)&bmpHead, sizeof(BITMAPINFOHEADER));
	//	file.Write((unsigned char *)m_picture_buf,  bmpHead.biSizeImage);
	//	file.Close();
	//	//HINSTANCE  nResult = ShellExecute(NULL, _T("open"), csFilename.LockBuffer(), _T(""), _T(""), SW_SHOW);

	//	CDlgSnapshotDummy dlg2;
	//	dlg2.Create(IDD_DLGSNAPSHOTDUMMY);
	//	
	//	CDlgSnapshot dlg;	
	//	dlg.vGetBMP(m_picture_buf,m_bih,bmpHead.biSizeImage,csFilename);		
	//	int ret = dlg.DoModal();
	//	if (ret == 0 || IDCANCEL == ret )
	//	{
	//		dlg2.DestroyWindow();	
	//	}
	//	
	//}
	//CATCH (CFileException, e)
	//{
	//	THROW_LAST();
	//}
	//END_CATCH
	return;
}
void CVideo::OnMenuExitfullscreen()
{
	// TODO: Add your command handler code here
   
	m_pbk->GetMainFrame()->m_bOnFullScreen ^= 1;
	m_pbk->GetMainFrame()->OnFullScreen(m_pbk->GetMainFrame()->m_bOnFullScreen);
}
void CVideo::OnMenuOSD()
{
	m_bDisplayOSD ^= 1;
	::PostMessage(GetParent()->m_hWnd,ID_SET_OSD,(WPARAM) m_bDisplayOSD, NULL);

	
}
bool CVideo::bResetOSD(bool bOSD)
{

	if (m_pNode == NULL) return false;
	m_bDisplayOSD = bOSD;

	if (bOSD)
	{
		bCreateOSD();

	}else
	{
		DELETEOBJ(m_pOSD);
	
	}
	m_bReCalcRect = true;
	GetParent()->Invalidate(true);
	return true;
}
bool CVideo::bCreateOSD()
{	
	bool bRet = true;
	
	CRect rectOSD;
	CRect rectParent;
	GetClientRect(&rectParent);
	rectOSD.left = rectParent.left;
	rectOSD.right = rectParent.right;
	rectOSD.bottom = rectParent.top + OSDHEIGHT;
	rectOSD.top = rectParent.top;
    if (!m_pOSD)
    {
	    m_pOSD = new COSD();
 		m_pOSD->Create(NULL, WS_CHILD | WS_VISIBLE | SS_REALSIZEIMAGE, rectOSD, this, IDC_OSD); 
		
    }
	m_pOSD->SetWindowPos(NULL, rectOSD.left,rectOSD.right,rectOSD.Width(),rectOSD.Height(),SWP_SHOWWINDOW |SWP_NOMOVE | SWP_NOZORDER );
	m_pOSD->vCreateAndRearrangeControl(OSDTYPE::OSDLIVEVIEW,m_csCameraName);
	
	
    return bRet;
}
void  CVideo::vRearrangeOSD()
{
	//if (m_pNode == NULL) return ;
    bCreateOSD();    
}


HBRUSH CVideo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CStatic::OnCtlColor(pDC, pWnd, nCtlColor);
	

	// TODO:  Change any attributes of the DC here
	if(nCtlColor == CTLCOLOR_STATIC) //
	{
	
	
		pDC->SetTextColor(OSDTEXTCOLOR);
		pDC->SetBkColor(OSDBKGNDCOLOR);
	    //hbr   =   CreateSolidBrush(OSDBKGNDCOLOR); 
	}
	//if(pWnd->GetDlgCtrlID() ==IDC_CAPTIONBAR)
	//{
	//	
	//	pDC->SetTextColor(TEXTCOLOR);
	//	pDC->SetBkColor(BKGNDCOLOR);

	//}

	// TODO:  Return a different brush if the default is not desired
	return m_hbr;
}



void CVideo::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);    
}

BOOL CVideo::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_bDisplayOSD)
	{
	    if(pMsg->message ==	WM_LBUTTONDOWN)
	    {
		    CPoint pt;
		    pt.x = GET_X_LPARAM(pMsg->lParam); 
		    pt.y = GET_Y_LPARAM(pMsg->lParam); 
		    ClientToScreen(&pt);
		    if(m_bDisplayOSD)
		    {
			    if(m_rectSendAudio.PtInRect(pt))
			    {
				    ASSERT(FALSE);
			    }

		    }
	    }
	}
	return CStatic::PreTranslateMessage(pMsg);
}

bool CVideo::bResetDrawRect(void)
{
	CRect rectVoid(0,0,0,0);
	CRect rcTmp = m_rectVideo;
	if(m_bDisplayOSD)
	{
		rcTmp.top += nOSDHeight;
		rcTmp.bottom -= nOSDHeight;
	}
	if (m_bFixAspectRatio)
	{
		int nXDest(0), nYDest(0), nWidth, nHeight;
		float fWscale, fHscale;
		fWscale = (float)abs(rcTmp.right) / (float)abs(m_bih.biWidth);
		fHscale = (float)abs(rcTmp.bottom) / (float)abs(m_bih.biHeight);
		if ( fWscale > fHscale )
		{
			float fScale = (float)abs(m_bih.biWidth)/(float)abs(m_bih.biHeight);
			nWidth = rcTmp.bottom* fScale ;//fHscale * m_rectVideo.Width();
			nHeight = rcTmp.bottom;// * -1;
			nXDest = rcTmp.left + (rcTmp.right - nWidth)/2;
			nYDest = rcTmp.top;
		}else
		{
			float fScale = float((float)abs(m_bih.biHeight) / (float)abs(m_bih.biWidth));
			nWidth =  rcTmp.right;
			nHeight = ((float)rcTmp.right) * fScale ;//* -1;
			nXDest = rcTmp.left;
			nYDest =  rcTmp.top + (rcTmp.bottom - nHeight)/2;
		}
		m_rectDraw = CRect(	nXDest, nYDest, nWidth, nHeight);
	}else
	{
		m_rectDraw = CRect(rcTmp);
	}
	if(m_rectVideo == rectVoid)// for hiding matrix, set rectangles to (0,0,0,0)
	{
		 m_rcPosThumbnail = m_rcThumbnail = m_rectDraw = rectVoid;
	}
	else
    {
        m_rcThumbnail=CRect(2*m_rectVideo.right/3-10, 2*m_rectVideo.bottom/3, m_rectDraw.right/3, m_rectDraw.bottom/3);
        m_rcPosThumbnail = CRect(m_rcThumbnail.left, m_rcThumbnail.top- nOSDHeight, m_rcThumbnail.left+ m_rcThumbnail.right, m_rcThumbnail.top+ m_rcThumbnail.bottom);
		TRACE(_T("m_rectDraw: %d\t%d\t%d\t%d\t@bResetDrawRect\r\n"), m_rectDraw.left, m_rectDraw.top, m_rectDraw.right, m_rectDraw.bottom);
		TRACE(_T("m_rectVideo: %d\t%d\t%d\t%d\t@bResetDrawRect\r\n"), m_rectVideo.left, m_rectVideo.top, m_rectVideo.right, m_rectVideo.bottom);
		TRACE(_T("m_rcThumbnail:%d\t%d\t%d\t%d\t@bResetDrawRect\r\n"),m_rcThumbnail.left,m_rcThumbnail.top,m_rcThumbnail.Width(),m_rcThumbnail.Height());
        TRACE(_T("m_rcPosThumbnail:%d\t%d\t%d\t%d\t@bResetDrawRect\r\n"),m_rcPosThumbnail.left,m_rcPosThumbnail.top,m_rcPosThumbnail.Width(),m_rcPosThumbnail.Height());
		if (m_bFlip | m_bUpsideDown)
        {
		    int width = m_rcThumbnail.right, height = m_rcThumbnail.bottom;
		    int nOrgX = m_rcThumbnail.left;
		    int nOrgY = m_rcThumbnail.top;

            nOrgX = m_bFlip ? (width + nOrgX): nOrgX;
		    width =	m_bFlip ? width*-1 : width ;
		    nOrgY = m_bUpsideDown ? (height + nOrgY) : nOrgY;
		    height = m_bUpsideDown ? height*-1 : height;
		    m_rcThumbnail = CRect(nOrgX, nOrgY, width, height);
        }

        m_fxScale = (float)m_rcThumbnail.right/(float)m_rcImg.right
                  * (float)m_rcImg.right/ (float)m_bih.biWidth;
        m_fyScale = (float)m_rcThumbnail.bottom/(float)m_rcImg.bottom
                  * (float)m_rcImg.bottom/ (float)abs(m_bih.biHeight);
        int nleft, ntop, nright, nbottom;
        nleft = m_rcThumbnail.left +(float)m_rcImg.left*m_fxScale;
        ntop =  (m_rcThumbnail.top +  m_rcThumbnail.bottom) - ((float)m_rcImg.top*m_fyScale);
        nright =nleft+ (float)(m_rcImg.right-m_rcImg.left)* m_fxScale;
        nbottom =ntop- (float)(m_rcImg.bottom-m_rcImg.top)* m_fyScale;
        m_rcCrop =CRect(nleft, ntop, nright, nbottom);   
   
		int width = m_rectDraw.right, height = m_rectDraw.bottom;
		int nOrgX = m_rectDraw.left;
		int nOrgY = m_rectDraw.top;

		nOrgX = m_bFlip ? (width + nOrgX): nOrgX;
		width =	m_bFlip ? width*-1 : width ;
		nOrgY = m_bUpsideDown ? (height + nOrgY) : nOrgY;
		height = m_bUpsideDown ? height*-1 : height;
		m_rectDraw = CRect(nOrgX, nOrgY, width, height);
		if (m_bFlip)
		{
			TRACE(_T("m_rectDraw: %d\t%d\t%d\t%d\r\n"), m_rectDraw.left, m_rectDraw.top, m_rectDraw.right, m_rectDraw.bottom);
		}
	}
	CRect rectTmp;
	GetWindowRect(&rectTmp);
	GetParent()->ScreenToClient(&rectTmp);
	GetParent()->InvalidateRect(&rectTmp, true);
	TRACE(_T("m_rectDraw: %d\t%d\t%d\t%d\r\n"), m_rectDraw.left, m_rectDraw.top, m_rectDraw.right, m_rectDraw.bottom);
	m_bReCalcRect = false;
	return false;
}

void CVideo::OnSendAudio()
{
	static bool m_bPreSetAudio;

	if(sendAudio.bDetectWaveIn())
	{		
		m_bAudioStart ^=1;
		
		if (m_bAudioStart)
		{						
			bool bRet = sendAudio.bStartSendAudio(m_pNode);		

			m_bPreSetAudio = m_bSetAudio;	//original Receive audio status		
			if(m_bSetAudio)
			{					
				vSetRecvAudio();	// turn off receive audio				
			}
			//else
			//{
			//	m_bPreSetAudio = false;// original Receive audio status
			//}

			if(bRet)
			{
				::PostMessage(GetParent()->m_hWnd, ID_CHANGE_SEND_AUDIO_BMP,(WPARAM) m_nVideoIdx, NULL);	
			}
		
		}
		else
		{	
			if(m_pOSD->m_pSendAudio)
			{
				m_pOSD->m_pSendAudio->SetBitmaps(IDB_BITMAP_OSD_MIC,GLOBALBKCOLOR);				
			}
		
			sendAudio.vStopSendAudio();

			if(m_bPreSetAudio)
			{
				vSetRecvAudio();
			}
		
		}
	}
	else
	{
		CString csNoMic;
		csNoMic.LoadStringW(CProductDefine::g_hResStr, IDS_OSD_NO_MIC);
		AfxMessageBox(csNoMic);
	}
	
}

BOOL CVideo::OnEraseBkgnd(CDC* pDC1)
{
    CDC* pDC = GetDC();
	pDC->SetBkColor(GLOBALFONTCOLOR);//BKCOLOR
	CRect rect1;
	GetClientRect(rect1);
	CBrush bkColor(GLOBALFONTCOLOR);
	pDC->FillRect(&rect1,&bkColor);
    ReleaseDC(pDC);
    return true;
/*
    //return CStatic::OnEraseBkgnd(pDC1);
const COLORREF GRADIENTCOLOR1 RGB(0,0,0);
const COLORREF GRADIENTCOLOR2 RGB( 50, 50, 50);	
    //CDC* pDC = GetDC();
	pDC->SetBkColor(BKCOLOR);//BKCOLOR
    return CStatic::OnEraseBkgnd(pDC);
    bool bRet(true);
    CRect rect;
    GetClientRect(&rect);
    HANDLE m_hBmp = CreateCompatibleBitmap(pDC->m_hDC, rect.Width(), rect.Height());
	HBITMAP hOldBmp = (HBITMAP)pDC->SelectObject(m_hBmp);
    DeleteObject(hOldBmp);   
    CGS::GradientFillRect(pDC, rect, GRADIENTCOLOR1, GRADIENTCOLOR2, GRADIENT_FILL_RECT_V);//, GRADIENT_FILL_RECT_V);   
    //HBITMAP m_hBmpBmp = CreateCompatibleBitmap(pDC->m_hDC,rect.Width(), rect.Height());
    ReleaseDC(pDC);
    return bRet;

	//CRect rect;
	//GetClientRect(rect);
	//CBrush bkColor(BKGNDCOLOR);
	//pDC->FillRect(&rect,&bkColor);
*/
}
#ifdef PANORAMA
void CVideo::OnSetParama360()
{
    if (m_pPanorama)
    {
        m_pPanorama->SetLensType(m_lensType = LENS_TYPE_FISHEYE360);
    }
}
void CVideo::OnSetParama180()
{
    if (m_pPanorama)
    {
        m_pPanorama->SetLensType(m_lensType= LENS_TYPE_FISHEYE180);
    }
}


LRESULT CALLBACK CVideo::HookProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
   if ( message == WM_COMMAND ) {
      switch ( LOWORD(wParam) ) {
          case IDM__MENU_PANORAMA360 :
          case IDM__MENU_PANORAMA180 :
          case ID_MENU_EXITFULLSCREEN:
            ::PostMessage(::GetParent(hWnd), message, wParam, lParam);
          return NULL;
      }
   }
   return CallWindowProc(DisplayWndProc, hWnd, message, wParam, lParam);
}
VOID CVideo::PanormaTrackMenuCallback(PanoramicDisplay *Display, HMENU hMenu )
{
   AppendMenu(hMenu, MF_SEPARATOR, 0, L"");
   AppendMenu(hMenu, 0, IDM__MENU_PANORAMA360, L"360 Panorama");
   AppendMenu(hMenu, 0, IDM__MENU_PANORAMA180, L"180 Panorama");
   AppendMenu(hMenu, 0, ID_MENU_EXITFULLSCREEN, L"FullScreen");
}																									    
bool CVideo::bFishEyeModal(NODEITEM* pNode)
{
	bool bRet = false;
    char* szFishEyeModal[] = {"N53U-FL","N53F-F","N50U-FL","N51U-FL","N50F-F","N51F-F","XTS-PAN5MP", "XTS-PAN5MPVP"};
    panoramic_type nLensType[] =  {LENS_TYPE_FISHEYE180, LENS_TYPE_FISHEYE360,LENS_TYPE_FISHEYE180,LENS_TYPE_FISHEYE180, LENS_TYPE_FISHEYE360, LENS_TYPE_FISHEYE360, LENS_TYPE_FISHEYE360, LENS_TYPE_FISHEYE360};
    ASSERT((sizeof(szFishEyeModal)/sizeof(char*)) == (sizeof(nLensType)/sizeof(panoramic_type)));
    
	if ( pNode->bFishEye )
    {
		if (pNode->byPanoromaType == PANORAMA_180)
		{
			m_lensType = LENS_TYPE_FISHEYE180;
		} 
		else
		{
			m_lensType = LENS_TYPE_FISHEYE360;
		}
        //m_lensType = LENS_TYPE_FISHEYE360; //set default lens type 360
        bRet = true;
    }
    return bRet;
}
void CVideo::vCreatePanoramaDisplay()
{
	// new a panorama display here
	USES_CONVERSION;
	int nWidth(0),nHeight(0);
	CRect rectPano;
	rectPano = m_rectVideo;	
	rectPano.top += nOSDHeight;
	
	if(!m_pPanorama && m_bPanorama) // if there is no m_pPanorama instance, new one
	{		
		CRect rectPano;
	    rectPano = m_rectVideo;
	    rectPano.top += nOSDHeight;        
        m_pPanorama = new PanoramicDisplay(this->m_hWnd, rectPano.left,rectPano.top, rectPano.Width(), rectPano.Height(), IDC_PANORAMA);
        m_pPanorama->SetLensType(m_lensType); 
		DisplayWndProc  = (WNDPROC)SetWindowLongPtr(m_pPanorama->GetHWND(), GWLP_WNDPROC, (LONG_PTR)HookProc);    
        m_pPanorama->SetTrackPopupMenuCallback(PanormaTrackMenuCallback);		
	}
}
#endif

void CVideo::UpdateDevice(int camid)
{
	ec_Camera camobj;
	m_pbk->GetCamera(camid,camobj);
	m_pOSD->vCreateAndRearrangeControl(OSDLIVEVIEW,camobj.cameraname);
	ReconnectStream();
}