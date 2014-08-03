#pragma once
//#include "nvrbk.h"
#include <setupapi.h> // Device setup APIs
#include "DEVGUID.h"
#pragma comment(lib, "Setupapi.lib")
class VK
{
public:
    VK()
    {
        //if (CNVRBk::GetBkObject()->isNVRType())
        {            
            if (GetKeyboardPresent() == false)
            {
                HWND hwnd = ::FindWindow(_T("OSKMainClass"), NULL);
                if (hwnd)
                {
                    ::ShowWindow(hwnd, SW_SHOWNOACTIVATE);
                }else
                {
                    TCHAR szParm[MAX_PATH]={};
                    _stprintf(szParm, _T("osk.exe"));
                    STARTUPINFO ssi={};
                    ssi.wShowWindow = SW_SHOWNOACTIVATE;
                    PROCESS_INFORMATION ppi={};
                    CreateProcess(NULL, szParm, NULL, NULL, FALSE, 0, NULL, NULL, &ssi, &ppi );
                }
            }
        }
    };
    ~VK()
    {
        //if (CNVRBk::GetBkObject()->isNVRType())
        {
            HWND hwnd = ::FindWindow(_T("OSKMainClass"), NULL);
            if (hwnd)
            {
                ::CloseWindow(hwnd);
            }
        }
    };
    bool GetKeyboardPresent()
    {
       HDEVINFO hDevInfo;
       SP_DEVINFO_DATA DeviceInfoData;
       hDevInfo = SetupDiGetClassDevs((LPGUID) &GUID_DEVCLASS_KEYBOARD,
           NULL, // Enumerator
           0,
           DIGCF_PRESENT);// | DIGCF_ALLCLASSES );
       
       if (hDevInfo == INVALID_HANDLE_VALUE)
       {
           return false;
       }
       
       // Enumerate through all devices in Set.
       bool bKeyPresent(false);
       DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
       //if (SetupDiEnumDeviceInfo(hDevInfo, i++, &DeviceInfoData)) 
       for (DWORD i=0;SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData);i++)
       {
           if (0== memcmp(&DeviceInfoData.ClassGuid, &GUID_DEVCLASS_KEYBOARD, sizeof(GUID)))
           {
               bKeyPresent = true;
               break;
           }
       }
       //  Cleanup
       SetupDiDestroyDeviceInfoList(hDevInfo);
       return bKeyPresent;
    }
};
/*
int test()
{
	GUID     DevClass;    // CLSID holder for the WEIUSB device class
	HDEVINFO hDevInfoSet; // Handle to device information set
	DWORD    dwIndex;     // Index of current device info set record
	DWORD    dwRequired;  // Required buffer length
	DWORD    dwError;     // Error value from GetLastError()
	BOOL     bResult;     // Boolean return result value

	SP_DEVICE_INTERFACE_DATA         DevNode;
	PSP_DEVICE_INTERFACE_DETAIL_DATA DevDetails;

	// Convert the registry-formatted CLSID for the WEIUSB device
	// class to a GUID structure.
	CLSIDFromString(CLSID_STR_WEIUSB, &DevClass);

	// Generate the device information set.  This is the actual
	// enumeration process.  We are specifying the class GUID of
	// the device type we want to enumerate, in this case only
	// WEIUSB devices.  The second argument would allow us to
	// constrain the enumeration to those devices attached to a
	// specified enumerator, or bus.  We could, for example,
	// enumerate only those devices attached via USB.
	hDevInfoSet = SetupDiGetClassDevs(
		&DevClass, // Only get WEIUSB devices
		NULL,      // Not specific to any bus
		NULL,      // Not associated with any window
		DIGCF_DEVICEINTERFACE + DIGCF_PRESENT);

	// Make sure enumeration completed without errors.
	if (hDevInfoSet == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr,
			"Unable to create device information set (Error 0x%08X)\n",
			GetLastError());
		return 1;
	}
	printf("Successfully created device information set.\n");

	// Iterate through the device info set.
	for (dwIndex = 0; ; dwIndex++)
	{
		// Retrieve the data from the next node index within the
		// device information set.
		DevNode.cbSize = sizeof(DevNode);
		bResult = SetupDiEnumDeviceInterfaces(
			hDevInfoSet,   // Handle to device info set
			NULL,          // Do not apply advanced filtering
			&DevClass,     // Class of device to retrieve
			dwIndex,       // List index of requested record
			&DevNode);     // Pointer to structure to receive data

		// If the previous call failed, do not continue trying
		// to enumerate devices.
		if (!bResult)
		{
			dwError = GetLastError();
			if (dwError != ERROR_NO_MORE_ITEMS)
			{
				fprintf(stderr,
					"Error enumerating devices (0x%08X).\n",
					dwError);
			}
			break;
		}

		// The device information data represents some device
		// flags and the class GUID associated with the device
		// instance.  The device details must be retrieved to
		// get the device path that can be used to open the
		// device.  This is a two-step process.  First the API
		// must be called with the pointer to a buffer set to
		// NULL, and the size of the buffer specified as zero.
		// This will cause the API to fail, but the API will
		// provide a value in the dwRequired argument indicating
		// how much memory is needed to store the file path.
		// The memory can then be allocated and the API called a
		// second time, specifying the pointer to the buffer and
		// the buffer size, to receive the actual device path.
		SetupDiGetDeviceInterfaceDetail(
			hDevInfoSet,  // Handle to device information set
			&DevNode,     // Specify a pointer to the current node
			NULL,         // Pointer to structure to receive path data
			0,            // Currently no space is allocated
			&dwRequired,  // Pointer to var to receive required buffer size
			NULL);        // Pointer to var to receive additional device info

		// Allocate memory required.
		DevDetails = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(dwRequired);
		if (DevDetails == NULL)
		{
			fprintf(stderr,
				"Unable to allocate memory for buffer.  Stopping.\n");
			break;
		}

		// Initialize the structure before using it.
		memset(DevDetails, 0, dwRequired);
		DevDetails->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		// Call the API a second time to retrieve the actual
		// device path string.
		bResult = SetupDiGetDeviceInterfaceDetail(
			hDevInfoSet,  // Handle to device information set
			&DevNode,     // Pointer to current node in devinfo set
			DevDetails,   // Pointer to buffer to receive device path
			dwRequired,   // Length of user-allocated buffer
			&dwRequired,  // Pointer to arg to receive required buffer length
			NULL);        // Not interested in additional data

		if (!bResult)
		{
			// Some error occurred retrieve the device path.
			fprintf(stderr,
				"ERROR: Unable to retrieve device path (0x%08X).\n",
				GetLastError());
		}
		else
		{
			// Successfully retrieved the device path.  Go ahead
			// and print it to the console.
			printf("   %s\n", DevDetails->DevicePath);
		}

		// Deallocate memory used
		free(DevDetails);

	} // for (dwIndex = 0;; dwIndex++)

	// Print output notification for user.
	printf("Enumeration listing complete.  %d device(s) detected.\n", dwIndex);

	// Deallocate resources used by Windows for enumeration.
	SetupDiDestroyDeviceInfoList(hDevInfoSet);

	return 0;
}

int test2()
{
       HDEVINFO hDevInfo;
       SP_DEVINFO_DATA DeviceInfoData;
       DWORD i;

       // Create a HDEVINFO with all present devices.
       hDevInfo = SetupDiGetClassDevs(NULL,
           0, // Enumerator
           0,
           DIGCF_PRESENT | DIGCF_ALLCLASSES );
       
       if (hDevInfo == INVALID_HANDLE_VALUE)
       {
           // Insert error handling here.
           return 1;
       }
       
       // Enumerate through all devices in Set.
       bool bKeyPresent(false);
       DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
       for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,
           &DeviceInfoData);i++)
       {
           DWORD DataT;
           LPTSTR buffer = NULL;
           DWORD buffersize = 0;
           if (0== memcmp(&DeviceInfoData.ClassGuid, &GUID_DEVCLASS_KEYBOARD, sizeof(GUID)))
           {
               //AfxMessageBox(_T("Find KEYBOARD!\r\n"));
               bKeyPresent = true;
           }
           
           //
           // Call function with null to begin with, 
           // then use the returned buffer size (doubled)
           // to Alloc the buffer. Keep calling until
           // success or an unknown failure.
           //
           //  Double the returned buffersize to correct
           //  for underlying legacy CM functions that 
           //  return an incorrect buffersize value on 
           //  DBCS/MBCS systems.
           // 

           //while (!SetupDiGetDeviceRegistryProperty(
           //    hDevInfo,
           //    &DeviceInfoData,
           //    SPDRP_DEVICEDESC,
           //    &DataT,
           //    (PBYTE)buffer,
           //    buffersize,
           //    &buffersize))
           //{
           //    if (GetLastError() == 
           //        ERROR_INSUFFICIENT_BUFFER)
           //    {
           //        // Change the buffer size.
           //        if (buffer) LocalFree(buffer);
           //        // Double the size to avoid problems on 
           //        // W2k MBCS systems per KB 888609. 
           //        buffer = (LPTSTR)LocalAlloc(LPTR,buffersize * 2);
           //    }
           //    else
           //    {
           //        // Insert error handling here.
           //        break;
           //    }
           //}
           //
           //printf("Result:[%s]\n",buffer);
           //
           //if (buffer) LocalFree(buffer);
       }
       
       if (bKeyPresent== false) 
           AfxMessageBox(_T("Keyboard not found\r\n"));
       if ( GetLastError()!=NO_ERROR &&
            GetLastError()!=ERROR_NO_MORE_ITEMS )
       {
           // Insert error handling here.
           return 1;
       }
       
       //  Cleanup
       SetupDiDestroyDeviceInfoList(hDevInfo);
       
       return 0;
}
int test3()
{
    HDEVINFO                 hDevInfo;
    SP_DEVINFO_DATA          did;
    ULONG                    i;

    //
    // Open a handle to the plug and play dev node.
    //
    hDevInfo = SetupDiGetClassDevs((LPGUID) &GUID_DEVCLASS_KEYBOARD,
                                   NULL,
                                   NULL,
                                   0
                                   );

    i = 0;
    ZeroMemory(&did, sizeof(SP_DEVINFO_DATA));
    did.cbSize = sizeof(SP_DEVINFO_DATA);

    if (SetupDiEnumDeviceInfo(hDevInfo, i++, &did)) 
    {
        //if (SetupDiEnumDeviceInterfaces(hDevInfo
        AfxMessageBox(L"keyboard present\r\n");
    }else 
    {
       AfxMessageBox(L"keyboard not present\r\n");
    }

    SetupDiDestroyDeviceInfoList (hDevInfo);
    return 0;
}
*/