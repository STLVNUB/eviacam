/////////////////////////////////////////////////////////////////////////////
// Name:        crvcamera_cv.cpp
// Purpose:  
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Modified by: 
// Created:     30/05/2008
// Copyright:   (C) 2008 Cesar Mauri Loba - CREA Software Systems
// 
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////////
#include "crvcamera_cv.h"
#include <sys/timeb.h>
#include <sys/types.h>
#include <stdio.h>
#include <cassert>

bool CCameraCV::g_cvInitialized= false;
int CCameraCV::g_numDevices= 0;
char CCameraCV::g_deviceNames[MAX_CV_DEVICES][50];

#if defined(WIN32)
#include <windows.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

// Workround to enable capture at 30fps for some camera models
// Should be called with administrative rights
// Return 0 if ok, -1 if permission denied
int VfwCamFpsWorkaround ()
{
	HKEY hKey;    
	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys=0;               // number of subkeys 
    DWORD    cbMaxSubKey;              // longest subkey size 
    DWORD    cchMaxClass;              // longest class string 
    DWORD    cValues;              // number of values for key 
    DWORD    cchMaxValue;          // longest value name 
    DWORD    cbMaxValueData;       // longest value data 
    DWORD    cbSecurityDescriptor; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 
	DWORD i, retCode;   
    DWORD cchValue = MAX_VALUE_NAME; 

	// Open key
	retCode= RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\VfWWDM Mapper"), 
							0, KEY_ALL_ACCESS, &hKey);
	if (retCode!= ERROR_SUCCESS) return -1;

	// Get the class name and the value count. 
    retCode = RegQueryInfoKey(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 
 
	// Enumerate the subkeys
	if (cSubKeys)
	{
		for (i=0; i<cSubKeys; i++) 
		{ 
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,	achKey, &cbName, NULL, 
									NULL, NULL, &ftLastWriteTime); 
			if (retCode == ERROR_SUCCESS) 
			{
				HKEY hKeyValue;

				retCode= RegOpenKeyEx(hKey, achKey, 0, KEY_WRITE, &hKeyValue);
				if (retCode== ERROR_SUCCESS)
				{
					DWORD value= 333330;
					retCode= RegSetValueEx(hKeyValue, TEXT("AvgTimePerFrame"), 0, REG_DWORD, 
											(const BYTE *) &value, sizeof(DWORD));
				}
				RegCloseKey(hKeyValue);
			}
		}
	} 

	// Close key
	RegCloseKey(hKey);

	return 0;
}

#endif

CCameraCV::CCameraCV(int cameraId, int width, int height, float fr)
{
	m_Id= cameraId;
	m_Width= width;
	m_Height= height;
	m_FrameRate= fr;
	m_pCvCapture= NULL;

#if defined(WIN32)
	VfwCamFpsWorkaround ();
#endif

	if (!g_cvInitialized) { cvInitSystem (0, NULL); g_cvInitialized= true; }
}

CCameraCV::~CCameraCV(void)
{
	Close ();	
}

bool CCameraCV::Open ()
{
	if (m_pCvCapture!= NULL) return true;	// Already opened
	m_pCvCapture= cvCaptureFromCAM (m_Id);
	if (m_pCvCapture== NULL) return false;
	
	m_lastTimeStamp= GetTime();
	cvSetCaptureProperty( m_pCvCapture, CV_CAP_PROP_FRAME_WIDTH, (double) m_Width );
	cvSetCaptureProperty( m_pCvCapture, CV_CAP_PROP_FRAME_HEIGHT, (double) m_Height );
	// The following line does nothing under MS Windows
	cvSetCaptureProperty( m_pCvCapture, CV_CAP_PROP_FPS, (double) m_FrameRate );
/*
/// TO SET UP FRAME RATE
// Set the preview rate in case we want to support previews in the future.
	
	capPreviewRate(m_hwndPreview, 33);

	// Attempt to get the capture parameters.
	capDriverGetCaps(m_hwndPreview, &m_capdrivercaps, sizeof(m_capdrivercaps));

	// Default values.
	m_captureparms.dwRequestMicroSecPerFrame = 33333;
	m_captureparms.fMakeUserHitOKToCapture = FALSE;
	m_captureparms.wPercentDropForError = 100;
	m_captureparms.fYield = TRUE;
	m_captureparms.dwIndexSize = 0;
	m_captureparms.wChunkGranularity = 0;
	m_captureparms.fUsingDOSMemory = FALSE;
	m_captureparms.wNumVideoRequested = 3;
	m_captureparms.fCaptureAudio = FALSE;
	m_captureparms.wNumAudioRequested = 0;
	m_captureparms.vKeyAbort = 0;
	m_captureparms.fAbortLeftMouse = FALSE;
	m_captureparms.fAbortRightMouse = FALSE;
	m_captureparms.fLimitEnabled = FALSE;
	m_captureparms.wTimeLimit = 0;
	m_captureparms.fMCIControl = FALSE;
	m_captureparms.fStepMCIDevice = FALSE;
	m_captureparms.dwMCIStartTime = 0;
	m_captureparms.dwMCIStopTime = 0;
	m_captureparms.fStepCaptureAt2x = FALSE;
	m_captureparms.wStepCaptureAverageFrames = 5;
	m_captureparms.dwAudioBufferSize = 0;
	m_captureparms.fDisableWriteCache = FALSE;
	m_captureparms.AVStreamMaster = 0;

	// Attempt to set the capture parameters.
	capCaptureSetSetup(m_hwndPreview, &m_captureparms, sizeof(m_captureparms));

	// Make sure that the values we have are correct.
	capCaptureGetSetup(m_hwndPreview, &m_captureparms, sizeof(m_captureparms));


	capSetVideoFormat ?????

	capSetUserData ???
	*/

	return true;
}

void CCameraCV::Close ()
{
	if (m_pCvCapture== NULL) return;	// Already closed

	CloseLive ();
	cvReleaseCapture (&m_pCvCapture);

	m_pCvCapture= NULL;
}

IplImage *CCameraCV::QueryFrame()
{
	assert (m_pCvCapture);
	if (m_pCvCapture== NULL) return NULL;

	IplImage *pImage= cvQueryFrame( m_pCvCapture );
	//assert (pImage);
	if (pImage== NULL) return NULL;

	// Flip image when needed
	if ( pImage->origin == 1 && m_horizontalFlip)
	{
		//cvConvertImage ( pImage, pImage, CV_CVTIMG_FLIP );
		cvFlip (pImage, NULL, -1);
		pImage->origin= 0;
	}
	else if ( pImage->origin == 1 && !m_horizontalFlip)
	{
		cvFlip (pImage, NULL, 0);
		pImage->origin= 0;
	}
	else if ( pImage->origin == 0 && m_horizontalFlip)
		cvFlip (pImage, NULL, 1);			


#if defined(linux)
	// TODO: it seems that under Linux no proper channelSeq is reported
	// Tested with Logitech Quickcam pro 4000 
	pImage->channelSeq[0]= 'B';
	pImage->channelSeq[2]= 'R';
#endif

	OnQueryFrame (pImage);
	
	return pImage;
}

int CCameraCV::GetNumDevices()
{
	int i;
	CvCapture* tmpCapture;

	if (!g_cvInitialized) { cvInitSystem (0, NULL); g_cvInitialized= true; }

	// Detect number of connected devices
	for (i= 0; i< MAX_CV_DEVICES; i++)
	{
		 tmpCapture= cvCreateCameraCapture (i);
		 if (tmpCapture== NULL) break;

		 cvReleaseCapture (&tmpCapture);

		 // Generate device name
		 sprintf (g_deviceNames[i], "Camera (Id:%d)", i);
	}
	
	g_numDevices= i;

	return g_numDevices;
}

char* CCameraCV::GetDeviceName (int id)
{
	if (id>= g_numDevices) return NULL;
	
	return g_deviceNames[id];
}