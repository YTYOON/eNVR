#pragma once
#include "afx.h"


// #define DFP_SEND_DRIVE_COMMAND 0x0007c084 
#define   DFP_SEND_DRIVE_COMMAND CTL_CODE(IOCTL_DISK_BASE, 0x0021 , METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
 // #define DFP_RECEIVE_DRIVE_DATA 0x0007c088 
#define   DFP_RECEIVE_DRIVE_DATA CTL_CODE(IOCTL_DISK_BASE, 0x0022 , METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
 #define   FILE_DEVICE_SCSI            0x0000001B 
#define   IOCTL_SCSI_MINIPORT_IDENTIFY ((FILE_DEVICE_SCSI << 16 ) + 0x0501 )
 #define   IOCTL_SCSI_MINIPORT         0x0004D008           // see NTDDSCSI.H for definition
  
// ATA/ATAPI���O
#define   IDE_ATA_IDENTIFY            0xEC      // ATA��ID���O(IDENTIFY DEVICE)
  
// IDE�R�O�H�s��
//typedefstruct _IDEREGS
//{
//    BYTE bFeaturesReg;        //�S�x�H�s��(�Ω�SMART�R�O) 
//    BYTE bSectorCountReg;     //���ϼƥرH�s�� 
//    BYTE bSectorNumberReg;    //�}�l���ϱH�s�� 
//    BYTE bCylLowReg;          //�}�l�W���C�r�`�H�s�� 
//    BYTE bCylHighReg;         //�}�l�W�����r�`�H�s�� 
//    BYTE bDriveHeadReg;       //�X�ʾ�/���Y�H�s�� 
//    BYTE bCommandReg;         //���O�H�s�� 
//    BYTE bReserved;           //�O�d
//} IDEREGS, *PIDEREGS, *LPIDEREGS;
//  
//�q�X�ʵ{�Ǫ�^�����A
//typedef  struct _DRIVERSTATUS
//{
//    BYTE bDriverError;       //���~�X 
//    BYTE bIDEStatus;         // IDE���A�H�s�� 
//    BYTE bReserved[ 2 ];       //�O�d 
//    DWORD dwReserved[ 2 ];     //�O�d
//} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;
//  
// IDE�]��IOCTL��J�ƾڵ��c
//typedef  struct _SENDCMDINPARAMS
//{
//    DWORD cBufferSize;       //�w�İϦr�`�� 
//    IDEREGS irDriveRegs;     // IDE�H�s���� 
//    BYTE bDriveNumber;       //�X�ʾ��� 
//    BYTE bReserved[ 3 ];       //�O�d 
//    DWORD dwReserved[ 4 ];     //�O�d 
//    BYTE bBuffer[ 1 ];         //��J�w�İ�(���B�H�x�ʦa�]�t1�r�`)
//} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;
//  
// IDE�]��IOCTL��X�ƾڵ��c
//typedef  struct _SENDCMDOUTPARAMS
//{
//    DWORD cBufferSize;           //�w�İϦr�`�� 
//    DRIVERSTATUS DriverStatus;   //�X�ʵ{�Ǫ�^���A 
//    BYTE bBuffer[ 1 ];             //��J�w�İ�(���B�H�x�ʦa�]�t1�r�`)
//} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;
//  
// IDE��ID�R�O��^���ƾ�
//�@512�r�`(256��WORD)�A�o�̶ȩw�q�F�@�ǷP���쪺��(�򥻤W�̾�ATA/ATAPI-4) 
typedef  struct _IDINFO
{
    USHORT wGenConfig;                  // WORD 0:�򥻫H���r 
    USHORT wNumCyls;                    // WORD 1:�W���� 
    USHORT wReserved2;                  // WORD 2:�O�d 
    USHORT wNumHeads;                   // WORD 3:���Y�� 
    USHORT wReserved4;                  // WORD 4:�O�d 
    USHORT wReserved5;                  // WORD 5:�O�d 
    USHORT wNumSectorsPerTrack;         // WORD 6:�C�ϹD���ϼ� 
    USHORT wVendorUnique[ 3 ];            // WORD 7-9:�t�a�]�w�� 
    CHAR sSerialNumber[ 20 ];           // WORD 10- 19:�ǦC�� 
    USHORT wBufferType;                 // WORD 20:�w������ 
    USHORT wBufferSize;                 // WORD 21:�w�Ĥj�p 
    USHORT wECCSize;                    // WORD 22: ECC����j�p 
    CHAR sFirmwareRev[ 8 ];             // WORD 23-26:�T�󪩥� 
    CHAR sModelNumber[ 40 ];            // WORD 27-46:�������� 
    USHORT wMoreVendorUnique;           // WORD 47:�t�a�]�w�� 
    USHORT wReserved48;                 // WORD 48:�O�d
    struct {
        USHORT reserved1: 8 ;
        USHORT DMA: 1 ;                   // 1=���DMA 
        USHORT LBA: 1 ;                   // 1=���LBA 
        USHORT DisIORDY: 1 ;              // 1=�i���ϥ�IORDY 
        USHORT IORDY: 1 ;                 // 1=���IORDY 
        USHORT SoftReset: 1 ;             // 1=�ݭnATA�n�Ұ� 
        USHORT Overlap: 1 ;               // 1=������|�ާ@ 
        USHORT Queue: 1 ;                 // 1=����R�O���C 
        USHORT InlDMA: 1 ;                // 1=�����e�s��DMA 
    } wCapabilities;                     // WORD 49:�@���O 
    USHORT wReserved1;                  // WORD 50:�O�d 
    USHORT wPIOTiming;                  // WORD 51: PIO�ɧ� 
    USHORT wDMATiming;                  // WORD 52: DMA�ɧ�
    struct {
        USHORT CHSNumber: 1 ;             // 1=WORD 54-58���� 
        USHORT CycleNumber: 1 ;           // 1=WORD 64-70���� 
        USHORT UnltraDMA: 1 ;             // 1=WORD 88���� 
        USHORT reserved: 13 ;
    } wFieldValidity;                    // WORD 53:����r�q���ĩʼлx 
    USHORT wNumCurCyls;                 // WORD 54: CHS�i�M�}���W���� 
    USHORT wNumCurHeads;                // WORD 55: CHS�i�M�}�����Y�� 
    USHORT wNumCurSectorsPerTrack;      // WORD 56: CHS�i�M�}�C�ϹD���ϼ� 
    USHORT wCurSectorsLow;              // WORD 57: CHS�i�M�}�����ϼƧC��r 
    USHORT wCurSectorsHigh;             // WORD 58: CHS�i�M�}�����ϼư���r
    struct {
        USHORT CurNumber: 8 ;             //��e�@���ʥiŪ�g���ϼ� 
        USHORT Multi: 1 ;                 // 1=�w��ܦh����Ū�g 
        USHORT reserved1: 7 ;
    } wMultSectorStuff;                  // WORD 59:�h����Ū�g�]�w 
    ULONG dwTotalSectors;               // WORD 60-61: LBA�i�M�}�����ϼ� 
    USHORT wSingleWordDMA;              // WORD 62:��r�`DMA�����O
    struct {
        USHORT Mode0: 1 ;                 // 1=����Ҧ�0 (4.17Mb/s) 
        USHORT Mode1: 1 ;                 // 1=����Ҧ�1 (13.3Mb/s) 
        USHORT Mode2: 1 ;                 // 1=����Ҧ�2 (16.7 Mb/s) 
        USHORT Reserved1: 5 ;
        USHORT Mode0Sel: 1 ;              // 1=�w��ܼҦ�0 
        USHORT Mode1Sel: 1 ;              // 1=�w��ܼҦ�1 
        USHORT Mode2Sel: 1 ;              // 1=�w��ܼҦ�2 
        USHORT Reserved2: 5 ;
    } wMultiWordDMA;                     // WORD 63:�h�r�`DMA�����O
    struct {
        USHORT AdvPOIModes: 8 ;           //�������POI�Ҧ��� 
        USHORT reserved: 8 ;
    } wPIOCapacity;                      // WORD 64:����PIO�����O 
    USHORT wMinMultiWordDMACycle;       // WORD 65:�h�r�`DMA�ǿ�g�����̤p�� 
    USHORT wRecMultiWordDMACycle;       // WORD 66:�h�r�`DMA�ǿ�g������ĳ�� 
    USHORT wMinPIONoFlowCycle;          // WORD 67:�L�y�����PIO�ǿ�g�����̤p�� 
    USHORT wMinPOIFlowCycle;            // WORD 68:���y�����PIO�ǿ�g�����̤p�� 
    USHORT wReserved69[ 11 ];             // WORD 69-79:�O�d
    struct {
        USHORT Reserved1: 1 ;
        USHORT ATA1: 1 ;                  // 1=���ATA-1 
        USHORT ATA2: 1 ;                  // 1=���ATA-2 
        USHORT ATA3: 1 ;                  // 1=���ATA-3 
        USHORT ATA4: 1 ;                  // 1=���ATA /ATAPI-4 
        USHORT ATA5: 1 ;                  // 1=���ATA/ATAPI-5 
        USHORT ATA6: 1 ;                  // 1=���ATA/ATAPI-6 
        USHORT ATA7: 1 ;                  // 1=���ATA/ATAPI-7 
        USHORT ATA8: 1 ;                  // 1=���ATA/ATAPI-8 
        USHORT ATA9: 1 ;                  // 1=���ATA/ATAPI-9 
        USHORT ATA10: 1 ;                 // 1=���ATA/ATAPI-10 
        USHORT ATA11: 1 ;                 // 1=���ATA/ATAPI-11 
        USHORT ATA12: 1 ;                 // 1=���ATA/ATAPI-12 
        USHORT ATA13: 1 ;                 // 1=���ATA/ATAPI-13 
        USHORT ATA14: 1 ;                 // 1=���ATA /ATAPI-14 
        USHORT Reserved2: 1 ;
    } wMajorVersion;                     // WORD 80:�D���� 
    USHORT wMinorVersion;               // WORD 81:�ƪ��� 
    USHORT wReserved82[ 6 ];              // WORD 82-87:�O�d
    struct {
        USHORT Mode0: 1 ;                 // 1=����Ҧ�0 (16.7Mb/s) 
        USHORT Mode1: 1 ;                 // 1=����Ҧ�1 (25Mb/s) 
        USHORT Mode2: 1 ;                 // 1=����Ҧ�2 (33Mb/ s) 
        USHORT Mode3: 1 ;                 // 1=����Ҧ�3 (44Mb/s) 
        USHORT Mode4: 1 ;                 // 1=����Ҧ�4 (66Mb/s) 
        USHORT Mode5: 1 ;                 // 1=����Ҧ�5 (100Mb /s) 
        USHORT Mode6: 1 ;                 // 1=����Ҧ�6 (133Mb/s) 
        USHORT Mode7: 1 ;                 // 1=����Ҧ�7 (166Mb/s) ??? 
        USHORT Mode0Sel: 1 ;              // 1=�w��ܼҦ�0 
        USHORT Mode1Sel: 1 ;              // 1=�w��ܼҦ�1 
        USHORT Mode2Sel: 1 ;              // 1=�w��ܼҦ�2 
        USHORT Mode3Sel: 1 ;              // 1=�w��ܼҦ�3 
        USHORT Mode4Sel: 1 ;              // 1 =�w��ܼҦ�4 
        USHORT Mode5Sel: 1 ;              // 1=�w��ܼҦ�5 
        USHORT Mode6Sel: 1 ;              // 1=�w��ܼҦ�6 
        USHORT Mode7Sel: 1 ;              // 1=�w��ܼҦ�7 
    } wUltraDMA;                         // WORD 88: Ultra DMA�����O 
    USHORT wReserved89[ 167 ];          // WORD 89-255
} IDINFO, *PIDINFO;
  
// SCSI�X�ʩһݪ���J��X�@�Ϊ����c
typedef  struct _SRB_IO_CONTROL
{
   ULONG HeaderLength;         //�Y���� 
   UCHAR Signature[ 8 ];         //�S�x�W�� 
   ULONG Timeout;              //�W�ɮɶ� 
   ULONG ControlCode;          //����X 
   ULONG ReturnCode;           //��^�X 
   ULONG Length;               //�w�İϪ���
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
