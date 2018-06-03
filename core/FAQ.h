//---------------------------------------------------------------------------

#ifndef FAQH
#define FAQH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TFAQForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *HowToPlay;
	TLabel *Item1;
	TImage *Image1;
	TLabel *Item2;
	TLabel *TacticsGame;
private:	// User declarations
public:		// User declarations
	__fastcall TFAQForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFAQForm *FAQForm;
//---------------------------------------------------------------------------
#endif
