#pragma once

class CRaposa;
const unsigned char TAMANHO_DESENHO = 64;
//__declspec(selectany) unsigned char QTD_RAPOSAS = 20;

class CChildView : public CWnd
{
public:
	CChildView();
	virtual ~CChildView();
	afx_msg void OnJogarInicio();
	//afx_msg void OnAbout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnJogarTerminar();
	void Pinta(unsigned short idIcon, CPoint Posicao, bool direita = true);
	void Apaga(CPoint Posicao, bool direita = true);
	void VerificaColisao();
	void Perdeu();
	bool temPoca(CPoint* position, bool direita = true);

	CPoint m_posGalinha;
	bool m_bJogoOn;
	CPoint m_Limites;

private:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnPaint();
	void CriaGalinha();
	void CriaRaposa();
	void CriaPoca();
	void AtualizaPontuacao();

	CArray<CRaposa*> m_aRaposas;
	CArray<CPoint> m_aPocas;
	unsigned char QTD_RAPOSAS = 10;
	unsigned char QTD_POCAS = 5;
	unsigned char m_uiPontuacao = 0;
	bool m_bDificuldadeAndar;
};
