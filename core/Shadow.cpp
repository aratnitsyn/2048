//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Shadow.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace ShadowWindow;
// ================================================================ TShadowPanel =============================================================
//---------------------------------------------------------------------------
__fastcall TShadowPanel::TShadowPanel(TForm *form) : TPanel(form), m_pMainForm(form), m_pPanelBitMap(NULL)
{
	DoubleBuffered = true;
	Align = alClient;
	m_pAlpha = 160;

	m_pShadowBitMap = new Graphics::TBitmap();
	m_pShadowBitMap->PixelFormat = pf24bit;
	m_pShadowBitMap->Width = 1;
	m_pShadowBitMap->Height = 1;
	m_pShadowBitMap->Canvas->Brush->Color = SHADOW_COLOR;
	m_pShadowBitMap->Canvas->FillRect(m_pShadowBitMap->Canvas->ClipRect);
}
//---------------------------------------------------------------------------
__fastcall TShadowPanel::~TShadowPanel()
{
	delete m_pShadowBitMap;
	delete m_pPanelBitMap;
}
//---------------------------------------------------------------------------
void __fastcall TShadowPanel::Paint(void)
{
	Canvas->CopyRect(Canvas->ClipRect, m_pPanelBitMap->Canvas, Canvas->ClipRect);
}
//---------------------------------------------------------------------------
void __fastcall TShadowPanel::Resize(void)
{
	Visible = false;

	m_pMainForm->Refresh();

	if(m_pPanelBitMap)
	{
		delete m_pPanelBitMap;
	}

	m_pPanelBitMap = new Graphics::TBitmap();
	m_pPanelBitMap->PixelFormat = pf24bit;
	m_pPanelBitMap->Width = m_pMainForm->ClientWidth;
	m_pPanelBitMap->Height = m_pMainForm->ClientHeight;
	m_pPanelBitMap->Canvas->CopyRect(m_pPanelBitMap->Canvas->ClipRect, m_pMainForm->Canvas, m_pPanelBitMap->Canvas->ClipRect);

	BLENDFUNCTION bf;

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = m_pAlpha;
	bf.AlphaFormat = 0;
	AlphaBlend(m_pPanelBitMap->Canvas->Handle, 0, 0, Width, Height, m_pShadowBitMap->Canvas->Handle, 0, 0, 1, 1, bf);

	Visible = true;

	TPanel::Resize();
}
//---------------------------------------------------------------------------
// ================================================================ CShadowClass =============================================================
//---------------------------------------------------------------------------
CShadowWindow::CShadowWindow(TForm *form)
{
	m_pShadowPanel = new TShadowPanel(form);
	m_pShadowPanel->Parent = form;

	m_pGameOverLabel = new TLabel(m_pShadowPanel);
	m_pGameOverLabel->Parent = m_pShadowPanel;
	m_pGameOverLabel->Align = alCustom;
	m_pGameOverLabel->Alignment = taCenter;
	m_pGameOverLabel->Layout = tlBottom;
	m_pGameOverLabel->Font->Color = StringToColor("$00515260");
	m_pGameOverLabel->Font->Height = 48;
	m_pGameOverLabel->Font->Name = "Consolas";
	m_pGameOverLabel->Caption = "ВЫ ПРОИГРАЛИ";
	m_pGameOverLabel->Width = form->ClientWidth;
	m_pGameOverLabel->Height = form->ClientHeight / 2;

	m_pQuestionGameLabel = new TLabel(m_pShadowPanel);
	m_pQuestionGameLabel->Parent = m_pShadowPanel;
	m_pQuestionGameLabel->Align = alCustom;
	m_pQuestionGameLabel->Alignment = taCenter;
	m_pQuestionGameLabel->Layout = tlBottom;
	m_pQuestionGameLabel->Font->Color = clWhite;
	m_pQuestionGameLabel->Font->Height = 24;
	m_pQuestionGameLabel->Font->Name = "Consolas";
	m_pQuestionGameLabel->Caption = "Хотите сыграть ещё раз?";
	m_pQuestionGameLabel->Width = form->ClientWidth;
	m_pQuestionGameLabel->Height = form->ClientHeight / 2 + 15;

	m_pRestartGameLabel = new TLabel(m_pShadowPanel);
	m_pRestartGameLabel->Parent = m_pShadowPanel;
	m_pRestartGameLabel->Align = alCustom;
	m_pRestartGameLabel->Alignment = taRightJustify;
	m_pRestartGameLabel->Layout = tlBottom;
	m_pRestartGameLabel->Font->Color = clWhite;
	m_pRestartGameLabel->Font->Height = 24;
	m_pRestartGameLabel->Font->Name = "Consolas";
	m_pRestartGameLabel->Caption = "Да";
	m_pRestartGameLabel->Left = form->ClientWidth / 2 - (m_pRestartGameLabel->Width) - 25;
	m_pRestartGameLabel->Top = form->ClientHeight / 2 + 25;
	m_pRestartGameLabel->OnClick = OnClickButton_RestartGame;

	m_pExitGameLabel = new TLabel(m_pShadowPanel);
	m_pExitGameLabel->Parent = m_pShadowPanel;
	m_pExitGameLabel->Align = alCustom;
	m_pExitGameLabel->Alignment = taLeftJustify;
	m_pExitGameLabel->Layout = tlBottom;
	m_pExitGameLabel->Font->Color = clWhite;
	m_pExitGameLabel->Font->Height = 24;
	m_pExitGameLabel->Font->Name = "Consolas";
	m_pExitGameLabel->Caption = "Нет";
	m_pExitGameLabel->Left = form->ClientWidth / 2 + 25;
	m_pExitGameLabel->Top = form->ClientHeight / 2 + 25;
	m_pExitGameLabel->OnClick = OnClickButton_ExitGame;
}
//---------------------------------------------------------------------------
CShadowWindow::~CShadowWindow()
{
	if(m_pShadowPanel)
	{
		delete m_pShadowPanel;

		m_pShadowPanel = NULL;
		m_pGameOverLabel = NULL;
		m_pExitGameLabel = NULL;
		m_pRestartGameLabel = NULL;
		m_pQuestionGameLabel = NULL;
	}
}
//---------------------------------------------------------------------------
