#pragma once

class CChildView;

class CRaposa : public CWinThread
{
public:
	DECLARE_DYNCREATE(CRaposa, CWinThread);
	DECLARE_MESSAGE_MAP();

	CRaposa();
	~CRaposa();
public:
	CPoint m_Posicao;
	CChildView* m_pChildView;
	bool m_bPraDireita;
	
	void Kill();
private:
	UINT_PTR m_uiTimer;
	
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	afx_msg void OnTimer(WPARAM, LPARAM);
};