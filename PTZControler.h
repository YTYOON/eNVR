#pragma once
#include "mycurl.h"
#include "DlgPtzSetting.h"


// CPTZControler command target
enum  PTZAction {P_L=0, P_R, T_U, T_D, Z_I, Z_O, PL_TU, PL_TD, PR_TU, PR_TD,STOP,F_I,F_O} ;
#define PTZ_THRESHOLD  2

class CPTZControler : public CObject
{
public:
	CPTZControler(HWND hwnd);
	virtual ~CPTZControler();
private:
    HWND m_hWnd	;
public: // variables
	NODEITEM *m_pNode;	
	PTZAuthInfo m_AuthInfo;    
	bool m_bCGISent;	
	bool m_bJoystickZMove;	
	bool m_bPatrolEnable;
	HANDLE m_hPatrolThread;
	vector <int> m_vctnPatrolPoints;	
	vector< map <int,int> > m_vctmapPatrolSeqDurSets;	
	Curlplus mycurl;
	int m_nPTZSpeed; 
	int m_x, m_y, m_z;
	int m_nPreInput;
	int m_nPatrolSeqDurIdx;
	CDatabase* pDB;
// functions

    BOOL bGetJoyStickInfo();
	LRESULT OnJoyStickMove(WPARAM wParam, LPARAM lParam);
	LRESULT OnJoyStickZMove(WPARAM wParam, LPARAM lParam);
	void vPTZMoveCGI(PTZAction ptzaction, int nPTZSpeed);
};


