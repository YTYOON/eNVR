#pragma once
#include "afx.h"
#include "IPSSendaudio.h"
#include <Mmsystem.h>


#pragma comment( lib, "Winmm.lib" )
class CSendAudio :
	public CObject
{
public:
	CSendAudio(void);
	~CSendAudio(void);

	NODEITEM *m_pNode;

	
	bool bStartSendAudio(NODEITEM* pNode);
	void vStopSendAudio();
	bool m_bAudioStart;
	bool bDetectWaveIn();
};
