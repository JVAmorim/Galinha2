#include "stdafx.h"
#include "Raposa.h"
#include "ChildView.h"

IMPLEMENT_DYNCREATE(CRaposa, CWinThread);

BEGIN_MESSAGE_MAP(CRaposa, CWinThread)
	ON_THREAD_MESSAGE(WM_TIMER, &OnTimer)
END_MESSAGE_MAP()

CRaposa::CRaposa()
{
}

CRaposa::~CRaposa()
{
}

BOOL CRaposa::InitInstance()
{
	srand(GetTickCount());
	INT_PTR iVelocidade = rand() % 1000 + 1;
	m_uiTimer = SetTimer(NULL, NULL, iVelocidade, NULL);
	srand(GetTickCount());
	m_bPraDireita = rand() % 2;

	CPoint pontoInicial;

	LONG lXRaposaEsquerda = m_pChildView->m_Limites.x / TAMANHO_DESENHO;
	m_bPraDireita ? pontoInicial.x = 0 : pontoInicial.x = TAMANHO_DESENHO * lXRaposaEsquerda;

	INT_PTR iLinhaNascer = rand() % ((m_pChildView->m_Limites.y / TAMANHO_DESENHO) - 2); // -2 para nao nascer na linha da galinha
	pontoInicial.y = TAMANHO_DESENHO + (iLinhaNascer * TAMANHO_DESENHO);

	m_Posicao = pontoInicial;

	return TRUE;
}

int CRaposa::ExitInstance()
{
	Kill();
	return CWinThread::ExitInstance();
}

void CRaposa::Kill()
{
	KillTimer(NULL, m_uiTimer);
	PostThreadMessage(WM_QUIT, NULL, NULL);
}

void CRaposa::OnTimer(WPARAM, LPARAM)
{
	if (m_pChildView->m_bJogoOn == false)
	{
		return;
	}
	m_pChildView->Apaga(m_Posicao, m_bPraDireita);

	m_Posicao.x += m_bPraDireita ? TAMANHO_DESENHO : -TAMANHO_DESENHO;

	
	while (m_pChildView->temPoca(&m_Posicao, m_bPraDireita))
		m_Posicao.x += m_bPraDireita ? TAMANHO_DESENHO : -TAMANHO_DESENHO;
	
	if (m_Posicao.x >= m_pChildView->m_Limites.x || m_Posicao.x <= 0)
	{
		KillTimer(NULL, m_uiTimer);
		InitInstance();
	}

	m_pChildView->Pinta(313, m_Posicao, m_bPraDireita);
	//if (m_pChildView->m_bJogoOn == false)
	//{
	//	return;
	//}
	if (m_pChildView->m_posGalinha.y == m_Posicao.y)
	{
		if (m_bPraDireita)
		{
			if (m_Posicao.x == m_pChildView->m_posGalinha.x)
				m_pChildView->Perdeu();
		}
		else if (m_Posicao.x == m_pChildView->m_posGalinha.x - TAMANHO_DESENHO)
			m_pChildView->Perdeu();
	}
}