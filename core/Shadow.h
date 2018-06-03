//---------------------------------------------------------------------------
#ifndef CShadowH
#define CShadowH
//---------------------------------------------------------------------------
#include <vcl.h>
//---------------------------------------------------------------------------
#define SHADOW_COLOR	clGray
#define SHADOW_ALPHA	200
//---------------------------------------------------------------------------
namespace ShadowWindow
{
	class TShadowPanel : public TPanel
	{
	public:
		__fastcall TShadowPanel(TForm *form);
		__fastcall ~TShadowPanel();
	protected:
		virtual void __fastcall Paint(void);
		DYNAMIC void __fastcall Resize(void);

		TForm *m_pMainForm;
		Graphics::TBitmap *m_pPanelBitMap;
		Graphics::TBitmap *m_pShadowBitMap;
		byte m_pAlpha;
	};

	class CShadowWindow
	{
	public:
		CShadowWindow(TForm *form);
		~CShadowWindow();

		void __fastcall OnClickButton_ExitGame(TObject *Sender);
		void __fastcall OnClickButton_RestartGame(TObject *Sender);
	private:
		TShadowPanel *m_pShadowPanel;
		TLabel *m_pGameOverLabel;
		TLabel *m_pExitGameLabel;
		TLabel *m_pRestartGameLabel;
		TLabel *m_pQuestionGameLabel;
	};
};
#endif
