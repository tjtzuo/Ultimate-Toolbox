// ==========================================================================
// 					Class Specification : COXSound
// ==========================================================================
//
// Version: 9.3
//
// This software along with its related components, documentation and files ("The Libraries")
// is © 1994-2007 The Code Project (1612916 Ontario Limited) and use of The Libraries is
// governed by a software license agreement ("Agreement").  Copies of the Agreement are
// available at The Code Project (www.codeproject.com), as part of the package you downloaded
// to obtain this file, or directly from our office.  For a copy of the license governing
// this software, you may contact us at legalaffairs@codeproject.com, or by calling 416-849-8900.                      
//
// //////////////////////////////////////////////////////////////////////////
//	Properties:
//
//	NO		Abstract class (does not have any objects)
//	YES		Derived from CObject
//	NO		Is a CWnd
//	NO		Two stage creation (constructor & Create())
//	NO		Has a message map
//	NO		Needs a resource template
//
//	NO		Persistant objects
//	NO		Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Description :
//	The COXSound class is an easy way to have an application play WAVE sounds.  
//	 WAVES can be loaded from WAVE files (*.wav), WAVE resources or 
//	 streamed in from a CFile object.  
//	Once a wave is loaded it may be played either synchronously or asynchronously.
//	In the case of the latter, the sound may be looped.

// Remark :

// Prerequisites (necessary conditions):

/////////////////////////////////////////////////////////////////////////////
#ifndef __OXSOUND_H__
#define __OXSOUND_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "OXDllExt.h"

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

#ifndef __OXMFCPRIV_H__
#include "..\include\afxpriv.h"
#define __OXMFCPRIV_H__
#endif

#ifndef __OXMFCIMPL_H__
#if _MFC_VER < 0x0700
	#include <..\src\afximpl.h>
#else
	#include <..\src\mfc\afximpl.h>
#endif
#define __OXMFCIMPL_H__
#endif

#pragma warning(disable : 4201)

#include <mmsystem.h>
#include "OXSoundWnd.h"
#include "OXMainRes.h"
#include "UTB64Bit.h"

// ... Make sure linker uses the the multi media library
#pragma comment(lib, "winmm.lib ")

#undef GlobalAllocPtr
#undef GlobalFreePtr

// all possible sound sources 
//
enum SoundSource
{
	SNDSRC_FILE=0,
	SNDSRC_CFILE=1,
	SNDSRC_INTRESOURCE=2,
	SNDSRC_UNKNOWN=3
};
//
// Following are defined:
//
//	SNDSRC_FILE				-	name of the file
//	SNDSRC_CFILE			-	pointer to opened CFile
//	SNDSRC_INTRESOURCE		-	resource ID
//	SNDSRC_UNKNOWN			-	unknown source, used to initialize variables
//
////////////////////////////////////////////////////////////////////////////


// Following structure was defined specifically to save information about
// the sound source
//
typedef struct _tagSOUNDSOURCEINFO
{
	enum SoundSource source;
	HINSTANCE hInstance;
	UINT nResourceID;
	CString sFileName;

	// constructor
	_tagSOUNDSOURCEINFO()
	{
		Reset();
	}

	// destructor
	void Reset()
	{
		source=SNDSRC_UNKNOWN;
		hInstance=NULL;
		sFileName=_T("");
		nResourceID=0;
	}

	// serialization
	void Serialize(CArchive& ar)
	{
	    // Only CObject-derived objects and six data-type
		// primitives are serializable. However, you
		// can cast any data type to a serializable data type,
		// and then you can serialize your data. The serializable
		// data types are

		// BYTE:    8 bits unsigned
		// WORD:    16 bits unsigned
		// LONG:    32 bits unsigned
		// DWORD:   32 bits unsigned
		// float    32 bits
		// double   64 bits, IEEE standard
    
		if (ar.IsStoring())
		{
			ar << (DWORD)source;
			ar << (DWORD)PtrToUlong(hInstance);
			ar << (DWORD)nResourceID;
			ar << sFileName;
	    }
		else
		{
			DWORD dwDummy;
			ar >> dwDummy;
			if(dwDummy!=(DWORD)SNDSRC_CFILE)
			{
				source=(enum SoundSource)dwDummy;
				ar >> dwDummy;
				hInstance=(HINSTANCE)(INT_PTR)dwDummy;
				ar >> dwDummy;
				nResourceID=(UINT)dwDummy;
				ar >> sFileName;
			}
	    }                 
	}

    // assignment operator
    _tagSOUNDSOURCEINFO& operator=( const _tagSOUNDSOURCEINFO& ssi )
	{
		if(this==&ssi)
			return *this;
		
		source=ssi.source;
		hInstance=ssi.hInstance;
		nResourceID=ssi.nResourceID;
		sFileName=ssi.sFileName;
		return *this;
	}

} SOUNDSOURCEINFO, * LPSOUNDSOURCEINFO;
//
// Below you will find the description of all structure's elements:
//
//	source			-	type of sound source. Refere to the description of
//						enum SoundSource for details
//	hInstance		-	handle of the instance that contains the nResourceID 
//						resource if source==SNDSRC_INTRESOURCE, otherwise
//						it is not used
//	nResourceID		-	resource ID that is used as sound source; valid only 
//						if source==SNDSRC_INTRESOURCE, otherwise it is not used
//	sFileName		-	name of the file that is used as sound source; valid only 
//						if source==SNDSRC_FILE, otherwise it is not used
//
////////////////////////////////////////////////////////////////////////////


class OX_CLASS_DECL COXSound : public CObject
{
friend class COXSoundWnd;
DECLARE_SERIAL(COXSound);

//	Data members --------------------------------------------
public:
protected:
	static CMap<COXSound*, COXSound*, DWORD, DWORD> m_allSoundObjects;

	HWND			m_hCallbackWnd;

	UINT			m_nRes;
	CString			m_sFilename;

	HGLOBAL			m_hWave;
	LPVOID			m_lpWave;

	BOOL			m_bIsPlaying;
	BOOL			m_bIsLoaded;
	BOOL			m_bLooping;

	MMRESULT		m_mmLastResult;

	LPVOID			m_lpWaveData;
	WAVEHDR			m_waveHdr;
	LPWAVEHDR		m_lpWaveHdr;
	HWAVEOUT		m_hWaveOut;
	WAVEFORMATEX	m_waveFormatEx;

	SOUNDSOURCEINFO m_SoundSourceInfo;

private:


//	Member functions --------------------------------------------
public:
	//	--- In:
	//	--- Out:
	//	--- Returns:	
	//	---	Effect: Constructor
	COXSound();
	
	//	--- In:			pCallbackWnd : CWnd which notification messages are sent
	//	--- Out:
	//	--- Returns:	
	//	---	Effect:		Stores the callback CWnd for notification messages
	void SetCallbackWnd(CWnd* pCallbackWnd);


	//	--- In:
	//	--- Out:
	//	--- Returns:	BOOL : TRUE if wave playback device exists
	//	---	Effect:		Determines if any wave playback devices exist
	static BOOL CanPlay();

	//	--- In:			pszFilename : complete path and filename for a WAVE file
	//	--- Out:
	//	--- Returns:	BOOL : TRUE if the file is opened successfully
	//	---	Effect:		Opens a WAVE file and loads data
	BOOL Open(LPCTSTR pszFileName);

	//	--- In:		nSoundResource : Resource ID for a WAVE resource
	//				hResInstance : Instance handle for resource (defaults to current executable)
	//	--- Out:
	//	--- Returns:	BOOL - TRUE if the wave resource is opened
	//	---	Effect:		Opens a WAVE resource and loads its data
	BOOL Open(UINT nSoundResource, HINSTANCE hResInstance = AfxGetResourceHandle());

	//	--- In:			pOpenedFile : CFile object from which to read WAVE data
	//	--- Out:
	//	--- Returns:	BOOL : TRUE if the file is opened successfully
	//	---	Effect:		Streams in WAVE data from an existing CFile object
	BOOL Open(CFile* pOpenedFile);

	//	--- In:			bLoop : Boolean flag to specify if the wave should loop (only works on async playback)
	//					bAsync : Boolean flag to specify if the wave should be played asynchronously
	//	--- Out:
	//	--- Returns:	BOOL : TRUE if the WAVE is played successfully
	//	---	Effect:		Plays a loaded WAVE
	BOOL Play(BOOL bLoop = FALSE, BOOL bAsync = TRUE);

	//	--- In:			
	//	--- Out:
	//	--- Returns:	
	//	---	Effect:		Stops a playing wave
	void Stop();

	//	--- In:
	//	--- Out:
	//	--- Returns:	BOOL : TRUE if a wave is loaded
	//	---	Effect:		Determines if any wave data is loaded
	BOOL IsWaveLoaded() const;

	//	--- In:
	//	--- Out:
	//	--- Returns:	BOOL : TRUE if a wave is playing
	//	---	Effect:		Determines if any wave is being played
	BOOL IsPlaying() const;

	//	--- In:			
	//	--- Out:		waveFormatEx : WAVEFORMATEX structure to receive data
	//	--- Returns:	BOOL : TRUE if the WAVEFORMATEX structure is valid
	//	---	Effect:		Gets format information about the loaded WAVE
	BOOL GetWaveFormat(WAVEFORMATEX* waveFormatEx);

	//	--- In:			
	//	--- Out:		
	//	--- Returns: The last multi media error that has occured
	//	---	Effect:		
	MMRESULT GetLastMMError() const;

	//	--- In:			hResult : MMRESULT for which to get the error description
	//	--- Out:		szDesc : Return buffer for text message
	//	--- Returns:	
	//	---	Effect:		Utility function which gets a text message for a passed MMRESULT
	static void GetErrorText(MMRESULT hResult, CString& sDesc);

	//	--- In:			
	//	--- Out:		pMMTime : MMTIME structure which returns the current position of the playing WAVE
	//	--- Returns:	
	//	---	Effect:		Gets the current position of the playing WAVE
	BOOL GetCurrentPosition(MMTIME* pMMTime) const;

	//	--- In:
	//	--- Out:
	//	--- Returns:	
	//	---	Effect:		Handles callback processing for COXSound - called from the callback window.
	void HandleCallback();

	//	--- In:
	//	--- Out:
	//	--- Returns:	pointer to the sound source info structure
	//	---	Effect:		
	inline LPSOUNDSOURCEINFO GetSoundSourceInfo() { return &m_SoundSourceInfo; }

	//	--- In:
	//	--- Out:
	//	--- Returns:	
	//	---	Effect: Destructor
	virtual ~COXSound();	

protected:
	void FreeMem();
	void FreeBuffer();

	static void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uiMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

	BOOL PrepareWaveHeader();
	BOOL PlayWithCallback();
	BOOL ParseWaveData(void* pvRes, WAVEFORMATEX** ppWaveHeader, BYTE** ppbWaveData, DWORD* pcbWaveSize);

	void CloseWaveOutDevice();

	LPVOID GlobalAllocPtr(UINT uiFlags, DWORD dwBytes) ;
	void GlobalFreePtr(LPVOID pPtr) ;

	BOOL CanPlayLoadedData();

private:

public:  
    virtual void Serialize(CArchive& ar);
};

#endif	// __OXSOUND_H__
