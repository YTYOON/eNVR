#pragma once
#include "afx.h"


// #define DFP_SEND_DRIVE_COMMAND 0x0007c084 
#define   DFP_SEND_DRIVE_COMMAND CTL_CODE(IOCTL_DISK_BASE, 0x0021 , METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
 // #define DFP_RECEIVE_DRIVE_DATA 0x0007c088 
#define   DFP_RECEIVE_DRIVE_DATA CTL_CODE(IOCTL_DISK_BASE, 0x0022 , METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
 #define   FILE_DEVICE_SCSI            0x0000001B 
#define   IOCTL_SCSI_MINIPORT_IDENTIFY ((FILE_DEVICE_SCSI << 16 ) + 0x0501 )
 #define   IOCTL_SCSI_MINIPORT         0x0004D008           // see NTDDSCSI.H for definition
  
// ATA/ATAPI指令
#define   IDE_ATA_IDENTIFY            0xEC      // ATA的ID指令(IDENTIFY DEVICE)
  
// IDE命令寄存器
//typedefstruct _IDEREGS
//{
//    BYTE bFeaturesReg;        //特徵寄存器(用於SMART命令) 
//    BYTE bSectorCountReg;     //扇區數目寄存器 
//    BYTE bSectorNumberReg;    //開始扇區寄存器 
//    BYTE bCylLowReg;          //開始柱面低字節寄存器 
//    BYTE bCylHighReg;         //開始柱面高字節寄存器 
//    BYTE bDriveHeadReg;       //驅動器/磁頭寄存器 
//    BYTE bCommandReg;         //指令寄存器 
//    BYTE bReserved;           //保留
//} IDEREGS, *PIDEREGS, *LPIDEREGS;
//  
//從驅動程序返回的狀態
//typedef  struct _DRIVERSTATUS
//{
//    BYTE bDriverError;       //錯誤碼 
//    BYTE bIDEStatus;         // IDE狀態寄存器 
//    BYTE bReserved[ 2 ];       //保留 
//    DWORD dwReserved[ 2 ];     //保留
//} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;
//  
// IDE設備IOCTL輸入數據結構
//typedef  struct _SENDCMDINPARAMS
//{
//    DWORD cBufferSize;       //緩衝區字節數 
//    IDEREGS irDriveRegs;     // IDE寄存器組 
//    BYTE bDriveNumber;       //驅動器號 
//    BYTE bReserved[ 3 ];       //保留 
//    DWORD dwReserved[ 4 ];     //保留 
//    BYTE bBuffer[ 1 ];         //輸入緩衝區(此處象徵性地包含1字節)
//} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;
//  
// IDE設備IOCTL輸出數據結構
//typedef  struct _SENDCMDOUTPARAMS
//{
//    DWORD cBufferSize;           //緩衝區字節數 
//    DRIVERSTATUS DriverStatus;   //驅動程序返回狀態 
//    BYTE bBuffer[ 1 ];             //輸入緩衝區(此處象徵性地包含1字節)
//} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;
//  
// IDE的ID命令返回的數據
//共512字節(256個WORD)，這裡僅定義了一些感興趣的項(基本上依據ATA/ATAPI-4) 
typedef  struct _IDINFO
{
    USHORT wGenConfig;                  // WORD 0:基本信息字 
    USHORT wNumCyls;                    // WORD 1:柱面數 
    USHORT wReserved2;                  // WORD 2:保留 
    USHORT wNumHeads;                   // WORD 3:磁頭數 
    USHORT wReserved4;                  // WORD 4:保留 
    USHORT wReserved5;                  // WORD 5:保留 
    USHORT wNumSectorsPerTrack;         // WORD 6:每磁道扇區數 
    USHORT wVendorUnique[ 3 ];            // WORD 7-9:廠家設定值 
    CHAR sSerialNumber[ 20 ];           // WORD 10- 19:序列號 
    USHORT wBufferType;                 // WORD 20:緩衝類型 
    USHORT wBufferSize;                 // WORD 21:緩衝大小 
    USHORT wECCSize;                    // WORD 22: ECC校驗大小 
    CHAR sFirmwareRev[ 8 ];             // WORD 23-26:固件版本 
    CHAR sModelNumber[ 40 ];            // WORD 27-46:內部型號 
    USHORT wMoreVendorUnique;           // WORD 47:廠家設定值 
    USHORT wReserved48;                 // WORD 48:保留
    struct {
        USHORT reserved1: 8 ;
        USHORT DMA: 1 ;                   // 1=支持DMA 
        USHORT LBA: 1 ;                   // 1=支持LBA 
        USHORT DisIORDY: 1 ;              // 1=可不使用IORDY 
        USHORT IORDY: 1 ;                 // 1=支持IORDY 
        USHORT SoftReset: 1 ;             // 1=需要ATA軟啟動 
        USHORT Overlap: 1 ;               // 1=支持重疊操作 
        USHORT Queue: 1 ;                 // 1=支持命令隊列 
        USHORT InlDMA: 1 ;                // 1=支持交叉存取DMA 
    } wCapabilities;                     // WORD 49:一般能力 
    USHORT wReserved1;                  // WORD 50:保留 
    USHORT wPIOTiming;                  // WORD 51: PIO時序 
    USHORT wDMATiming;                  // WORD 52: DMA時序
    struct {
        USHORT CHSNumber: 1 ;             // 1=WORD 54-58有效 
        USHORT CycleNumber: 1 ;           // 1=WORD 64-70有效 
        USHORT UnltraDMA: 1 ;             // 1=WORD 88有效 
        USHORT reserved: 13 ;
    } wFieldValidity;                    // WORD 53:後續字段有效性標誌 
    USHORT wNumCurCyls;                 // WORD 54: CHS可尋址的柱面數 
    USHORT wNumCurHeads;                // WORD 55: CHS可尋址的磁頭數 
    USHORT wNumCurSectorsPerTrack;      // WORD 56: CHS可尋址每磁道扇區數 
    USHORT wCurSectorsLow;              // WORD 57: CHS可尋址的扇區數低位字 
    USHORT wCurSectorsHigh;             // WORD 58: CHS可尋址的扇區數高位字
    struct {
        USHORT CurNumber: 8 ;             //當前一次性可讀寫扇區數 
        USHORT Multi: 1 ;                 // 1=已選擇多扇區讀寫 
        USHORT reserved1: 7 ;
    } wMultSectorStuff;                  // WORD 59:多扇區讀寫設定 
    ULONG dwTotalSectors;               // WORD 60-61: LBA可尋址的扇區數 
    USHORT wSingleWordDMA;              // WORD 62:單字節DMA支持能力
    struct {
        USHORT Mode0: 1 ;                 // 1=支持模式0 (4.17Mb/s) 
        USHORT Mode1: 1 ;                 // 1=支持模式1 (13.3Mb/s) 
        USHORT Mode2: 1 ;                 // 1=支持模式2 (16.7 Mb/s) 
        USHORT Reserved1: 5 ;
        USHORT Mode0Sel: 1 ;              // 1=已選擇模式0 
        USHORT Mode1Sel: 1 ;              // 1=已選擇模式1 
        USHORT Mode2Sel: 1 ;              // 1=已選擇模式2 
        USHORT Reserved2: 5 ;
    } wMultiWordDMA;                     // WORD 63:多字節DMA支持能力
    struct {
        USHORT AdvPOIModes: 8 ;           //支持高級POI模式數 
        USHORT reserved: 8 ;
    } wPIOCapacity;                      // WORD 64:高級PIO支持能力 
    USHORT wMinMultiWordDMACycle;       // WORD 65:多字節DMA傳輸週期的最小值 
    USHORT wRecMultiWordDMACycle;       // WORD 66:多字節DMA傳輸週期的建議值 
    USHORT wMinPIONoFlowCycle;          // WORD 67:無流控制時PIO傳輸週期的最小值 
    USHORT wMinPOIFlowCycle;            // WORD 68:有流控制時PIO傳輸週期的最小值 
    USHORT wReserved69[ 11 ];             // WORD 69-79:保留
    struct {
        USHORT Reserved1: 1 ;
        USHORT ATA1: 1 ;                  // 1=支持ATA-1 
        USHORT ATA2: 1 ;                  // 1=支持ATA-2 
        USHORT ATA3: 1 ;                  // 1=支持ATA-3 
        USHORT ATA4: 1 ;                  // 1=支持ATA /ATAPI-4 
        USHORT ATA5: 1 ;                  // 1=支持ATA/ATAPI-5 
        USHORT ATA6: 1 ;                  // 1=支持ATA/ATAPI-6 
        USHORT ATA7: 1 ;                  // 1=支持ATA/ATAPI-7 
        USHORT ATA8: 1 ;                  // 1=支持ATA/ATAPI-8 
        USHORT ATA9: 1 ;                  // 1=支持ATA/ATAPI-9 
        USHORT ATA10: 1 ;                 // 1=支持ATA/ATAPI-10 
        USHORT ATA11: 1 ;                 // 1=支持ATA/ATAPI-11 
        USHORT ATA12: 1 ;                 // 1=支持ATA/ATAPI-12 
        USHORT ATA13: 1 ;                 // 1=支持ATA/ATAPI-13 
        USHORT ATA14: 1 ;                 // 1=支持ATA /ATAPI-14 
        USHORT Reserved2: 1 ;
    } wMajorVersion;                     // WORD 80:主版本 
    USHORT wMinorVersion;               // WORD 81:副版本 
    USHORT wReserved82[ 6 ];              // WORD 82-87:保留
    struct {
        USHORT Mode0: 1 ;                 // 1=支持模式0 (16.7Mb/s) 
        USHORT Mode1: 1 ;                 // 1=支持模式1 (25Mb/s) 
        USHORT Mode2: 1 ;                 // 1=支持模式2 (33Mb/ s) 
        USHORT Mode3: 1 ;                 // 1=支持模式3 (44Mb/s) 
        USHORT Mode4: 1 ;                 // 1=支持模式4 (66Mb/s) 
        USHORT Mode5: 1 ;                 // 1=支持模式5 (100Mb /s) 
        USHORT Mode6: 1 ;                 // 1=支持模式6 (133Mb/s) 
        USHORT Mode7: 1 ;                 // 1=支持模式7 (166Mb/s) ??? 
        USHORT Mode0Sel: 1 ;              // 1=已選擇模式0 
        USHORT Mode1Sel: 1 ;              // 1=已選擇模式1 
        USHORT Mode2Sel: 1 ;              // 1=已選擇模式2 
        USHORT Mode3Sel: 1 ;              // 1=已選擇模式3 
        USHORT Mode4Sel: 1 ;              // 1 =已選擇模式4 
        USHORT Mode5Sel: 1 ;              // 1=已選擇模式5 
        USHORT Mode6Sel: 1 ;              // 1=已選擇模式6 
        USHORT Mode7Sel: 1 ;              // 1=已選擇模式7 
    } wUltraDMA;                         // WORD 88: Ultra DMA支持能力 
    USHORT wReserved89[ 167 ];          // WORD 89-255
} IDINFO, *PIDINFO;
  
// SCSI驅動所需的輸入輸出共用的結構
typedef  struct _SRB_IO_CONTROL
{
   ULONG HeaderLength;         //頭長度 
   UCHAR Signature[ 8 ];         //特徵名稱 
   ULONG Timeout;              //超時時間 
   ULONG ControlCode;          //控制碼 
   ULONG ReturnCode;           //返回碼 
   ULONG Length;               //緩衝區長度
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;
class CIdeDiskInfo :
    public CObject
{
public:
    CIdeDiskInfo(void);
    ~CIdeDiskInfo(void);
private:
    HANDLE CIdeDiskInfo::OpenDevice(LPCTSTR filename);
    BOOL CIdeDiskInfo::IdentifyDevice(HANDLE hDevice, PIDINFO pIdInfo);
    BOOL CIdeDiskInfo::IdentifyDeviceAsScsi(HANDLE hDevice, int nDrive, PIDINFO pIdInfo);
    void CIdeDiskInfo::AdjustString( char * str, int len);
    BOOL CIdeDiskInfo::GetPhysicalDriveInfoInNT( int nDrive, PIDINFO pIdInfo);
    BOOL CIdeDiskInfo::GetIdeDriveAsScsiInfoInNT( int nDrive, PIDINFO pIdInfo);
    void CIdeDiskInfo::SaveValue(char* szValue, DWORD nLen);
    bool CIdeDiskInfo::bReadValue(char* szValue, DWORD* pnLen);
public:
    bool bAuthorization(void);
    bool bEnroll(DWORD nChannel);
    bool CIdeDiskInfo::bAuthorized(BYTE* nChannel);
};
