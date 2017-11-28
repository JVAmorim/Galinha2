#include "stdafx.h"
#include "Galinha2.h"
#include "ChildView.h"
#include "Raposa.h"
#include <mmsystem.h> //som

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView()
{
	m_bJogoOn = false;
	m_bDificuldadeAndar = false;
}

CChildView::~CChildView()
{
	OnJogarTerminar();
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_JOGAR_INICIO, &CChildView::OnJogarInicio) ////////
	//ON_COMMAND(ID_APP_ABOUT, &CChildView::OnAbout) /////////
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_JOGAR_TERMINAR, &CChildView::OnJogarTerminar)
END_MESSAGE_MAP()

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	//cs.style &= ~WS_BORDER;
	//cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
	//	::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint()
{
	CDC* pDC = GetDC();

	pDC->FillSolidRect(0, 0, m_Limites.x, TAMANHO_DESENHO, RGB(185, 122, 87));
	pDC->FillSolidRect(0, (((m_Limites.y / TAMANHO_DESENHO) - 1) * TAMANHO_DESENHO), m_Limites.x, m_Limites.y, RGB(185, 122, 87));

	CPaintDC dc(this); // device context for painting
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	m_Limites.x = cx;
	m_Limites.y = cy;
}

void CChildView::VerificaColisao()
{
#ifdef _DEBUG
	if (!m_bJogoOn)
	{
		return;
	}
#endif
	if (m_posGalinha.y == 0)
	{
		//AfxMessageBox(_T("GANHOU")); 
		++m_uiPontuacao;
		QTD_RAPOSAS = QTD_RAPOSAS + 5;
		QTD_POCAS = QTD_POCAS + 1;
		AtualizaPontuacao();
		m_bJogoOn = false;
		OnJogarInicio();
	}
	else if (temPoca(&m_posGalinha))
	{
		m_bDificuldadeAndar = true;
		PlaySound(_T("res\\water2.wav"), NULL, SND_ASYNC);
	}
	else {
		for (USHORT i = 0; i < m_aRaposas.GetCount(); ++i)
		{
			CRaposa *pRaposa = m_aRaposas[i];
			if (m_posGalinha.y == pRaposa->m_Posicao.y)
			{
				if (pRaposa->m_bPraDireita)
				{
					if (pRaposa->m_Posicao.x == m_posGalinha.x)
						Perdeu();
				}
				else if (pRaposa->m_Posicao.x == m_posGalinha.x - TAMANHO_DESENHO)
					Perdeu();
			}
		}
	}
}

void CChildView::Pinta(unsigned short idIcon, CPoint Posicao, bool bDireita)
{
	HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(idIcon)) ;
	CDC* pDC = GetDC();
	DrawIconEx(pDC->m_hDC, Posicao.x, Posicao.y, hIcon, bDireita ? -TAMANHO_DESENHO : TAMANHO_DESENHO, TAMANHO_DESENHO, NULL, NULL, DI_NORMAL);

	ReleaseDC(pDC);
}

void CChildView::Apaga(CPoint Posicao, bool bDireita)
{
	HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_BRANCO));
	CDC* pDC = GetDC();
	
	DrawIconEx(pDC->m_hDC, Posicao.x, Posicao.y, hIcon, bDireita ? -TAMANHO_DESENHO : TAMANHO_DESENHO, TAMANHO_DESENHO, NULL, NULL, DI_NORMAL);
	
	ReleaseDC(pDC);
}

void CChildView::CriaGalinha()
{
	m_posGalinha.x = rand() % ((m_Limites.x/* - TAMANHO_DESENHO*/) / TAMANHO_DESENHO) * TAMANHO_DESENHO;
	m_posGalinha.y = (((m_Limites.y / TAMANHO_DESENHO) - 1) * TAMANHO_DESENHO);
	Pinta(IDI_ICON_GALINHA, m_posGalinha, true);
	AtualizaPontuacao();
}

void CChildView::CriaRaposa()
{
	CRaposa* pRaposa = static_cast<CRaposa*>(AfxBeginThread(RUNTIME_CLASS(CRaposa), THREAD_PRIORITY_NORMAL, 1, CREATE_SUSPENDED, NULL));

	pRaposa->m_pChildView = this;

	m_aRaposas.Add(pRaposa);
	Sleep(20);
	pRaposa->ResumeThread();
}

void CChildView::CriaPoca()
{
	CPoint pontoInicial;
	bool bCriou = false;

	while (bCriou == false)
	{
		bCriou = true;
		srand(GetTickCount());
		Sleep(3);

		INT_PTR iColunaNascer = rand() % ((m_Limites.x / TAMANHO_DESENHO) - 2);
		pontoInicial.x = TAMANHO_DESENHO + (iColunaNascer * TAMANHO_DESENHO);

		INT_PTR iLinhaNascer = rand() % ((m_Limites.y / TAMANHO_DESENHO) - 2); // -2 para nao nascer na linha da galinha
		pontoInicial.y = TAMANHO_DESENHO + (iLinhaNascer * TAMANHO_DESENHO);

		for (unsigned char i = 0; i < m_aPocas.GetCount(); ++i)
		{
			if (m_aPocas.GetAt(i).x == pontoInicial.x && m_aPocas.GetAt(i).y == pontoInicial.y)
			{
				bCriou = false;
				break;
			}
		}
	}
	m_aPocas.Add(pontoInicial);
	Pinta(IDI_ICON_POCA, pontoInicial);
}

//void CChildView::OnAbout()
//{
//	CAboutDlg aboutDlg;
//	aboutDlg.DoModal();
//}

void CChildView::OnJogarInicio()
{
	if (!m_bJogoOn)
	{
		//CDC* pDC = GetDC();
		//pDC->FillSolidRect(0, 0, m_Limites.x, m_Limites.y, RGB(255, 255, 255));
		//pDC->FillSolidRect(0, (((m_Limites.y / TAMANHO_DESENHO) - 1) * TAMANHO_DESENHO), m_Limites.x, m_Limites.y, RGB(185, 122, 87));

		for (unsigned char i = m_aRaposas.GetCount(); i < QTD_RAPOSAS; ++i)
			CriaRaposa();

		for (unsigned char i = m_aPocas.GetCount(); i < QTD_POCAS; ++i)
			CriaPoca();

		CriaGalinha();
	}
	m_bJogoOn = true;
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_bJogoOn)
	{
		if (m_bDificuldadeAndar)
		{
			srand(GetTickCount());
			if (rand() % 3 == 0)
			{
				PlaySound(_T("res\\water.wav"), NULL, SND_ASYNC); 
				return;
			}
		}
		
		m_bDificuldadeAndar = false;
		//PlaySound(_T("res\\water.wav"), NULL, SND_ASYNC);

		if (m_posGalinha.y == (((m_Limites.y / TAMANHO_DESENHO) - 1) * TAMANHO_DESENHO))
			GetDC()->FillSolidRect(0, (((m_Limites.y / TAMANHO_DESENHO) - 1) * TAMANHO_DESENHO), m_Limites.x, m_Limites.y, RGB(185, 122, 87));
		else
			Apaga(m_posGalinha, true);

		for (INT_PTR i = 0; i < m_aPocas.GetCount(); ++i)
		{
			if (m_aPocas[i].y == m_posGalinha.y && m_aPocas[i].x == m_posGalinha.x)
			{
				Pinta(IDI_ICON_POCA, m_aPocas[i]);
			}

		}

		switch (nChar)
		{
		case 37:
		case 'a':
		case 'A':
			if (m_posGalinha.x <= TAMANHO_DESENHO)
				break;
			m_posGalinha.x -= TAMANHO_DESENHO;
			break;

		case 38:
		case 'w':
		case 'W':
			if (m_posGalinha.y <= 0)
				break;
			m_posGalinha.y -= TAMANHO_DESENHO;
			break;

		case 39:
		case 'd':
		case 'D':
			if (m_posGalinha.x >= m_Limites.x - TAMANHO_DESENHO)
				break;
			m_posGalinha.x += TAMANHO_DESENHO;
			break;

		case 40:
		case 's':
		case 'S':
			if (m_posGalinha.y >= m_Limites.y - 2 * TAMANHO_DESENHO)
				break;
			m_posGalinha.y += TAMANHO_DESENHO;
			break;

		}
		Pinta(IDI_ICON_GALINHA, m_posGalinha);

		VerificaColisao();
	}
}

void CChildView::Perdeu()
{
	if (m_bJogoOn)
	{
		m_bJogoOn = false;
		PlaySound(_T("res\\wolf.wav"), NULL, SND_ASYNC);
		AfxMessageBox(_T("PERDEU")); // desenhar na tela ao inves de gerar um popup
		
		//HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(333)); //IDB_BITMAP1s
		//CDC* pDC = GetDC();
		
		
		OnJogarTerminar();
		//DrawIconEx(pDC->m_hDC, m_Limites.x / 2, m_Limites.y / 2, hIcon, TAMANHO_DESENHO, TAMANHO_DESENHO, NULL, NULL, DI_NORMAL);
	}
}

bool CChildView::temPoca(CPoint * position, bool direita)
{
	for (INT_PTR i = 0; i < m_aPocas.GetCount(); ++i)
	{
		if (position->y == m_aPocas[i].y)
		{
			if (direita) 
			{
				if(m_aPocas[i].x == position->x)
				return true;
			}
			else if ((position->x + TAMANHO_DESENHO) == m_aPocas[i].x)
			{
				return true;
			}
		}
	}
	return false;
}

void CChildView::OnJogarTerminar()
{
	m_bJogoOn = false;
	for (INT_PTR i = 0; i < m_aRaposas.GetCount(); ++i)
		m_aRaposas[i]->Kill();

	m_aRaposas.RemoveAll();
	m_aPocas.RemoveAll();
	
	QTD_RAPOSAS = 10;
	QTD_POCAS = 5;
	m_uiPontuacao = 0;

	Invalidate();

	CDC* pDC = GetDC();
	pDC->FillSolidRect(0, 0, m_Limites.x, m_Limites.y, RGB(255, 255, 255));
	pDC->FillSolidRect(0, (((m_Limites.y / TAMANHO_DESENHO) - 1) * TAMANHO_DESENHO), m_Limites.x, m_Limites.y, RGB(185, 122, 87));
}

void CChildView::AtualizaPontuacao()
{
	unsigned char dezena  = m_uiPontuacao / 10;
	unsigned char unidade = m_uiPontuacao % 10;
	
	INT_PTR idi_icon_dezena  = IDI_ICON_0 + dezena;
	INT_PTR idi_icon_unidade = IDI_ICON_0 + unidade;
	
	CDC* pDC = GetDC();
	pDC->FillSolidRect(0, 0, m_Limites.x, TAMANHO_DESENHO, RGB(185, 122, 87));

	Pinta(idi_icon_dezena, CPoint(32, 0), false);
	Pinta(idi_icon_unidade, CPoint(96, 0), false);
}