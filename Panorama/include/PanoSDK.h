
//----------------------------------------------------------------------------------
// File:        PanoSDK.h
// SDK Version: 1.05
// Email:       PanoGuard@gmail.com
//
// TO  THE MAXIMUM  EXTENT PERMITTED  BY APPLICABLE  LAW, THIS SOFTWARE  IS PROVIDED
// *AS IS* AND "I MAY" AND  ITS SUPPLIERS DISCLAIM  ALL WARRANTIES,  EITHER  EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED  TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL "I MAY" OR ITS SUPPLIERS
// BE  LIABLE  FOR  ANY  SPECIAL,  INCIDENTAL,  INDIRECT,  OR  CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION,  DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE  USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF "I MAY" HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
//
//
//----------------------------------------------------------------------------------

#ifndef  PANO_360_180_SDK_HINC
#define  PANO_360_180_SDK_HINC

#define  CAMERA_BUFFER_SIZE    2984

typedef HANDLE    CAMERA;
typedef VOID (*ImageReceiveCallback)(class PanoramicDisplay *Display, int uID, HANDLE hDIB, BYTE *StreamBuf, int width, int height, int BufLength);
typedef VOID (*AnnotateCallback)(class PanoramicDisplay *Display, int uID, HDC hDC);
typedef VOID (*TrackPopupMenuCallback)(class PanoramicDisplay *Display, HMENU hMenu);

/******************************************************************/
/* "PG360SDK_EXPORTS" it's used for internal developement only  . */
/* The SDK user should not define "PG360SDK_EXPORTS               */
/******************************************************************/
#ifdef PG360SDK_EXPORTS
   #define PG360SDK_API __declspec(dllexport)
#else
   #define PG360SDK_API __declspec(dllimport)
#endif

/********************************************************************************/
/********************************************************************************/
/*                    Panoramic lens (Mirror) Classifications                   */
/********************************************************************************/
/********************************************************************************/
enum panoramic_type { LENS_TYPE_GENERAL = 1 , LENS_TYPE_FISHEYE360 = 2, LENS_TYPE_CONCAVE = 3, LENS_TYPE_CONVEX = 4, LENS_TYPE_PAL = 5, LENS_TYPE_PALI = 6, LENS_TYPE_ANNULARFISH = 7, LENS_TYPE_FISHEYE180 = 8 };
// LENS_TYPe_GENERAL means non-panorama general image


#if !defined VIDE360_MODE_DECLARED
#define  VIDE360_MODE_DECLARED
/********************************************************************************/
/********************************************************************************/
/*                     360 Degree Panorama Viewing Modes                        */
/********************************************************************************/
/********************************************************************************/
enum VIDE360_MODE { SHOW360_AUTOCRUISE = 0,    SHOW360_NOCRUISE = 1,    SHOW360_STRIP  = 2,   SHOW360_QUADSOURCE = 3,    SHOW360_STRIP2  = 4,       SHOW360_SOURCEONLY   = 5,    SHOW360_CYLINDRICAL = 6 , SHOW360_SINGLESCREEN = 7 ,  SHOW360_STRIP4 = 8, SHOW360_STRIP6 = 9,  SHOW360_6PTZ = 10,  SHOW360_ALL_MODES = 11 };


/********************************************************************************/
/********************************************************************************/
/*                      180 Degree Panorama Viewing Modes                       */
/********************************************************************************/
/********************************************************************************/
enum VIDE180_MODE { SHOW180_QUADCYLINDER = 0,  SHOW180_CYLINDER = 1, SHOW180_QUAD  = 2, SHOW180_SOURCE = 3,     SHOW180_1PERSPECTIVE = 4,  SHOW180_STRIP = 5 , SHOW180_QUADSOURCE = 6};
#endif

// defines for Display->SetLeftMouseStyle(DWORD Style)
#define  LEFTMOUSE_NORMAL                    0X0000    //Using SDK Default Settings
#define  LEFTMOUSE_DISABLED                  0X0001

// defines for Display->SetRightMouseStyle(DWORD Style)
#define  RIGHTMOUSE_NORMAL                   0X0000    //Using SDK Default Settings
#define  RIGHTMOUSE_DISABLED                 0X0001
#define  RIGHTMOUSE_DISABLE_ALIGNMENT        0X0002
#define  RIGHTMOUSE_DISABLE_SNAPSHOT         0X0004
#define  RIGHTMOUSE_DISABLE_DISPLAY_MODE     0X0008


/********************************************************************************/
/********************************************************************************/
/*                      Fisheye Lens Attribute                                  */
/*                If you are using CBC Fisheye Lens                             */
/*         It's Stereographic Projection,  Please add following Code            */
/*           Display->SetLensAttribute(FISHEYE_STEREOGRAPHIC)                   */
/********************************************************************************/
/********************************************************************************/
enum LENS_ATTRIBUTE { FISHEYE_EQUIDISTANT = 0,  FISHEYE_STEREOGRAPHICR = 1 };

#define LINE_SIZE(width)   ((((width*24)+31) >> 5) << 2)   // for YUV to RGB conversion.

#ifdef __cplusplus
  extern "C" {
#endif
/********************************************************************************/
/********************************************************************************/
/*                     Panoramic Viewing Window Class                           */
/********************************************************************************/
/********************************************************************************/
class PG360SDK_API PanoramicDisplay {
public:
   //Window Mode Constructor ...
   PanoramicDisplay(HWND hWnd, int xInitPos,int yInitPos, int outWidth, int outHeight, int userID); /* New */
   PanoramicDisplay(CAMERA Camera, HWND hWndParent, int xInitPos,int yInitPos, int outWidth, int outHeight, int userID);
   PanoramicDisplay(LPSTR szMAC, HWND hWndParent, int xInitPos,int yInitPos, int outWidth, int outHeight, int userID);
   PanoramicDisplay(LPSTR szMAC, HWND hWndParent, int sourceWidth, int sourceHeight, int xInitPos,int yInitPos, int outWidth, int outHeight, int userID); /* New */
   //Windowless Mode Constructor ...
   PanoramicDisplay(int outWidth, int outHeight, int userID); /* New */
   PanoramicDisplay(CAMERA Camera, int outWidth, int outHeight, int userID);
   PanoramicDisplay(LPSTR szMAC, int outWidth, int outHeight, int userID);
   PanoramicDisplay(LPSTR szMAC, int sourceWidth, int sourceHeight, int outWidth, int outHeight, int userID); /* New */
   VOID Associate(HWND hWnd);

   //Destructor ...
   ~PanoramicDisplay();

   HWND    GetHWND();
   CAMERA  GetCamera();

   /************************************************************************************
    ************************************************************************************
    ************* C A M E R A   I M A G E   D E V I C E   M A I N T A I N O U S  *******
    ************************************************************************************
    ************************************************************************************/
   BOOL   LoadCamera(LPSTR szMAC);
   BOOL   LoadCamera(LPSTR szMAC, int sourceWidth, int sourceHeight);
   BOOL   SaveCamera();
   BOOL   ChangeCamera(CAMERA Camera);
   BOOL   ChangeCamera(int sourceWidth, int sourceHeight);
   BOOL   ChangeCamera(LPSTR szMAC, int sourceWidth, int sourceHeight);
   BOOL   SetCameraName(LPSTR name);
   LPSTR  GetCameraName();

   // Panoramic Image Maintainous ...
   BOOL   SetLensType(int imageType);
   int    GetLensType();
   BOOL   SetLensFOV(double fov);
   double GetLensFOV();
   BOOL   GetMirror();
   BOOL   SetMirror(BOOL fMirror);
   BOOL   GetFlip();
   BOOL   SetFlip(BOOL fFlip);
   BOOL   Alignment();

   // If this function used, output image size will be set as (destWidth X destHeight) not window (Windowless) Client RECT size.
   BOOL   SetOutputImageSize(int destWidth, int destHeight);
//   BOOL   AdvanceAlignment();  Removed ....

   BOOL   Get360Alignment(double *lensDOV, int *Radius, int *xCenter, int *yCenter, int *OvalToRound, BOOL *flipVertical, BOOL *mirrorHorizontal );
   BOOL   Get180Alignment(double *lensDOV, int *Radius, int *xCenter, int *yCenter, int *OvalToRound, BOOL *flipVertical, BOOL *mirrorHorizontal, double *verticaEffectivelAngle, double *verticalOffsetAngle );
   BOOL   Get360PALAlignment(double *lensDOV, int *bigRadius, int *smallRadius, int *xCenter, int *yCenter, int *OvalToRound, BOOL *flipVertical, BOOL *mirrorHorizontal, double *effectiveAngle );

   BOOL   Set360Alignment(double lensDOV, int Radius, int xCenter, int yCenter, int OvalToRound, BOOL flipVertical, BOOL mirrorHorizontal);
   BOOL   Set180Alignment(double lensDOV, int Radius, int xCenter, int yCenter, int OvalToRound, BOOL flipVertical, BOOL mirrorHorizontal, double verticaEffectivelAngle, double verticalOffsetAngle );
   BOOL   Set360PALAlignment(double lensDOV, int bigRadius, int smallRadius, int xCenter, int yCenter, int OvalToRound, BOOL flipVertical, BOOL mirrorHorizontal, double effectiveAngle );

   /************************************************************************************
    ************************************************************************************
    ************* S C R E E N   A N D   B U F F E R    M A I N T A I N O U S  **********
    ************************************************************************************
    ************************************************************************************/
   //Window Mode MoveWindow ...
   VOID   MoveWindow(int x,int y,int outputWidth,int outputHeight);
   //Windowless MoveWindow ...
   VOID   MoveWindow(int outputWidth,int outputHeight);
   BOOL   GetDIBResolution(int *outputWidth, int *outputHeight);

   HANDLE GetScreenBuffer(int *outputWidth, int *outputHeight, BYTE **outputBuffer, int *BufSize);  /// Image Format must Be BGR24 or  BGR32

   // Choose Either for
   BYTE  *GetDisplaySourceFrameBuffer(int *sourceWidth, int *sourceHeight);  // SDK will create internal buffer for receiving Video RGB Buffer
   BOOL   SetDisplaySourceFrameBuffer(HANDLE sourcehDIB);                // SDK will create internal buffer for receiving Video RGB Buffer

   BOOL   UpdateNewFrame();


   BOOL   SendJPEGFrameImage(BYTE *FrameBuffer,int length);
   BOOL   SendBufferBGR24(BYTE *sourceImage); // Avoid to use this Function, Because it will be remove for performance considerations.
   BOOL   SendBufferRGB24(BYTE *sourceImage); // Avoid to use this Function, Because it will be remove for performance considerations.
   BOOL   SendBufferRGB32(BYTE *sourceImage); // Avoid to use this Function, Because it will be remove for performance considerations.
   BOOL   SendBufferBGR32(BYTE *sourceImage); // Avoid to use this Function, Because it will be remove for performance considerations.

   VOID   SetDisplayCallback(ImageReceiveCallback MyDisplayCallback);
   VOID   SetAnnotateCallback(AnnotateCallback MyAnnotateCallback);
   BOOL   SetTrackPopupMenuCallback(TrackPopupMenuCallback MyPopupMenyCallback);

   BOOL   SetRightMouseStyle(DWORD Style);
   BOOL   SetLeftMouseStyle(DWORD Style);

   BOOL   Snapshot();

   /************************************************************************************
    ************************************************************************************
    ************* C A M E R A   V I E W I N G    C O N T R O L *************************
    ************************************************************************************
    ************************************************************************************/
   int  GetShowType();
   BOOL SetShowType(int show);

   // Viewing Control ....
   BOOL MoveRight();
   BOOL MoveLeft();
   BOOL MoveUp();
   BOOL MoveDown();
   BOOL Zoomin();
   BOOL Zoomout();
   int  GetAutoPanSpeed();
   BOOL SetAutoPanSpeed(int Degree);

   // GetPTZ and SetPTZ control the PTZ for each indexed Pano-Sub-Screen
   // Each Index is a integer between 0 ~ 4
   BOOL   GetPTZ(int index, double *pan, double *tilt, double *zoom);
   BOOL   SetPTZ(int index, double pan, double tilt, double zoom);
   BOOL   GetPTAngle(int srcX,int srcY, double *pan, double *tilt );

   /***********  The Following Function is use for Control PTZ individually    ************/
   int  GetFocusIndex();
   BOOL SetFocusIndex(int Focus);
   WNDPROC GetWndProc();   // For Windowless process PanoGuard Windows Default proc

                      //MAC in XX:XX:XX:XX:XX:XX Hex Digit Characters format
                      //License Code  in DDDDDDDDDDDDDDDDDDD 20 Continue Hex Digit characters '0' ~ '9'  'A' ~ 'Z' format
   BOOL SetLicenseCode(LPSTR szMAC, LPSTR Code, LPSTR NetIP, int port, LPSTR CameraAccount, LPSTR CameraPassword);
   BOOL GetLicenseCode(LPSTR szMAC, LPSTR Code);
   BOOL GetLicenseCode(LPSTR szMAC, LPSTR Code, LPSTR NetIP,int *port, LPSTR CameraAccount, LPSTR CameraPassword);
   BOOL IsRegistered();
   VOID SetLensAttribute(int LensAttribute); // CBC announce fisheye lens, it's Stereographic Projection
   BOOL SendCommand(int Command, WPARAM wParam, LPARAM lParam);

   //The following elements is use for PanoGuard Internel only, Not for SDK Developers.,
private:
   PanoramicDisplay  *BorlandPtr;  // This Pointer is used for Borland C++ 6 Builder Compability purpose only, Do not using this member variable.
   CAMERA   Camera;
   WNDPROC  WndProc;
   int      mode;
public:
   //The following elements is use for PanoGuard Internel only, Not for SDK Developers.,
   VOID *Reserved;
   void  FreeDisplay();

public:
   //The following elements is use for SDK Developers.,
   VOID *UserData;  // Reserved for User Development

};

class PG360SDK_API PanoramicSystem {
public:

   CAMERA  OpenSnapshot(HWND hWnd);
   CAMERA  OpenSnapshot(HWND hWnd,LPSTR FileName);

   BOOL    FindCamera(LPSTR szMAC, CAMERA Camera);
   BOOL    FindCamera(LPSTR szMAC,int width,int height, CAMERA Camera);

   BOOL    SaveCameraConfigure(CAMERA Cam);
   VOID    SetDebugMode();
   VOID    Configure(HWND hWnd);
   BOOL    MergeConfigure(LPSTR FullPath);   // New Created , 2013/01/18
   BOOL    BrowseMergeConfigure(HWND hWnd);   // New Created , 2013/01/18
//   BOOL    CreateIPCAMConfigure(HWND hWnd, CAMERA Camera); //This function has been Removed
   BOOL    CreateConfigure(int lens_type, int srcWidth,int srcHeight, LPSTR alias, CAMERA Camera);
   BOOL    CreateConfigure(HWND hWnd, CAMERA Camera);
   BOOL    EditCameraConfigure(HWND hWnd, CAMERA Camera);

   BOOL    CreateCamera(int imageType, int srcWidth, int srcHeight, double lensDOV, HANDLE hCamera);
   BOOL    CreateCamera(int imageType, int srcWidth, int srcHeight, double lensDOV, LPSTR szMAC, LPSTR szLicense, LPSTR ipAddress, LPSTR CameraAccount, LPSTR CameraPassword, int HTTPPort, LPSTR Alias, HANDLE hCamera);
   BOOL    RemoveCamera(LPSTR szMAC);     // Remove All Camera Settings with specified 'MAC ID' in PanoGuardSDK manage list
   BOOL    RemoveCamera(LPSTR szMAC,int srcWidth, int srcHeight);

#if !defined MINIMUN_SIZE_SDK
   // DIB Device Independent Bitmap Handling
   HANDLE  LoadJpegFile(LPSTR Path);
   HANDLE  DecodeJpegStream(BYTE *JpegFrameBuffer, int JpegBufLength);
#endif
   BOOL    DIBImage(HANDLE hDIB, int *width, int *height, BYTE **BGR24Buffer);
   BOOL    DestroyDIB(HANDLE hDIB);
   BOOL    SetCameraLicenseCode(LPSTR MAC, LPSTR License);
   BOOL    IsSystemRegistered();

};


PG360SDK_API VOID  WINAPI SetDisplayCallback(class PanoramicDisplay *Display, ImageReceiveCallback MyDisplayCallback);

// Syntax : RemotePackVideo("c:\\MyDVR\\MyVideo.avi", Display->GetCamera(), "c:\\UserData\\DeskTop\\MyVideoPack.exe"); , will create auto-extract auto-execute video player.
PG360SDK_API BOOL WINAPI RemotePackVideo( LPSTR szExeName, LPSTR VideoName, HANDLE CAMERA, LPSTR szWWW, LPWSTR szCaption, LPWSTR szAbout);

extern PG360SDK_API PanoramicSystem   gPanoramicSystem;
#ifdef __cplusplus
  }
#endif


#if 0    // Reference of enum YUV format
enum PixelFormat {
    PIX_FMT_NONE= -1,
    PIX_FMT_YUV420P,   ///< Planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
    PIX_FMT_YUYV422,   ///< Packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
    PIX_FMT_RGB24,     ///< Packed RGB 8:8:8, 24bpp, RGBRGB...
    PIX_FMT_BGR24,     ///< Packed RGB 8:8:8, 24bpp, BGRBGR...
    PIX_FMT_YUV422P,   ///< Planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
    PIX_FMT_YUV444P,   ///< Planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)
    PIX_FMT_RGB32,     ///< Packed RGB 8:8:8, 32bpp, (msb)8A 8R 8G 8B(lsb), in cpu endianness
    PIX_FMT_YUV410P,   ///< Planar YUV 4:1:0,  9bpp, (1 Cr & Cb sample per 4x4 Y samples)
    PIX_FMT_YUV411P,   ///< Planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples)
    PIX_FMT_RGB565,    ///< Packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), in cpu endianness
    PIX_FMT_RGB555,    ///< Packed RGB 5:5:5, 16bpp, (msb)1A 5R 5G 5B(lsb), in cpu endianness most significant bit to 0
    PIX_FMT_GRAY8,     ///<        Y        ,  8bpp
    PIX_FMT_MONOWHITE, ///<        Y        ,  1bpp, 1 is white
    PIX_FMT_MONOBLACK, ///<        Y        ,  1bpp, 0 is black
    PIX_FMT_PAL8,      ///< 8 bit with PIX_FMT_RGB32 palette
    PIX_FMT_YUVJ420P,  ///< Planar YUV 4:2:0, 12bpp, full scale (jpeg)
    PIX_FMT_YUVJ422P,  ///< Planar YUV 4:2:2, 16bpp, full scale (jpeg)
    PIX_FMT_YUVJ444P,  ///< Planar YUV 4:4:4, 24bpp, full scale (jpeg)
    PIX_FMT_XVMC_MPEG2_MC,///< XVideo Motion Acceleration via common packet passing(xvmc_render.h)
    PIX_FMT_XVMC_MPEG2_IDCT,
    PIX_FMT_UYVY422,   ///< Packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1
    PIX_FMT_UYYVYY411, ///< Packed YUV 4:1:1, 12bpp, Cb Y0 Y1 Cr Y2 Y3
    PIX_FMT_BGR32,     ///< Packed RGB 8:8:8, 32bpp, (msb)8A 8B 8G 8R(lsb), in cpu endianness
    PIX_FMT_BGR565,    ///< Packed RGB 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), in cpu endianness
    PIX_FMT_BGR555,    ///< Packed RGB 5:5:5, 16bpp, (msb)1A 5B 5G 5R(lsb), in cpu endianness most significant bit to 1
    PIX_FMT_BGR8,      ///< Packed RGB 3:3:2,  8bpp, (msb)2B 3G 3R(lsb)
    PIX_FMT_BGR4,      ///< Packed RGB 1:2:1,  4bpp, (msb)1B 2G 1R(lsb)
    PIX_FMT_BGR4_BYTE, ///< Packed RGB 1:2:1,  8bpp, (msb)1B 2G 1R(lsb)
    PIX_FMT_RGB8,      ///< Packed RGB 3:3:2,  8bpp, (msb)2R 3G 3B(lsb)
    PIX_FMT_RGB4,      ///< Packed RGB 1:2:1,  4bpp, (msb)2R 3G 3B(lsb)
    PIX_FMT_RGB4_BYTE, ///< Packed RGB 1:2:1,  8bpp, (msb)2R 3G 3B(lsb)
    PIX_FMT_NV12,      ///< Planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 for UV
    PIX_FMT_NV21,      ///< as above, but U and V bytes are swapped

    PIX_FMT_RGB32_1,   ///< Packed RGB 8:8:8, 32bpp, (msb)8R 8G 8B 8A(lsb), in cpu endianness
    PIX_FMT_BGR32_1,   ///< Packed RGB 8:8:8, 32bpp, (msb)8B 8G 8R 8A(lsb), in cpu endianness

    PIX_FMT_GRAY16BE,  ///<        Y        , 16bpp, big-endian
    PIX_FMT_GRAY16LE,  ///<        Y        , 16bpp, little-endian
    PIX_FMT_NB,        ///< number of pixel formats, DO NOT USE THIS if you want to link with shared libav* because the number of formats might differ between versions
};

#endif

#endif

