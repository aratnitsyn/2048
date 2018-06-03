//---------------------------------------------------------------------------
#ifndef GameBoardH
#define GameBoardH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
#include <pngimage.hpp>
//---------------------------------------------------------------------------
#include <Map>

#include "FAQ.h"
#include "Shadow.h"

using namespace ShadowWindow;
//---------------------------------------------------------------------------
#define HEIGHT_TILE_NUMBER	12

const int tilesPositions[16][2] =
{
	// [Fisrt row]
	{20, 120},
	{85, 120},
	{150, 120},
	{215, 120},

	// [Second row]
	{20, 185},
	{85, 185},
	{150, 185},
	{215, 185},

	// [Third  row]
	{20, 250},
	{85, 250},
	{150, 250},
	{215, 250},

	// [Fourth row]
	{20, 315},
	{85, 315},
	{150, 315},
	{215, 315}
};

const int tilesNumbersPositions[16][2] =
{
	// [Fisrt row]
	{20, 136},
	{85, 136},
	{150, 136},
	{215, 136},

	// [Second row]
	{20, 201},
	{85, 201},
	{150, 201},
	{215, 201},

	// [Third  row]
	{20, 266},
	{85, 266},
	{150, 266},
	{215, 266},

	// [Fourth row]
	{20, 331},
	{85, 331},
	{150, 331},
	{215, 331}
};

typedef enum _DIRECTION
{
	DIRECTION_LEFT = 37,
	DIRECTION_UP,
	DIRECTION_RIGHT,
	DIRECTION_DOWN
} DIRECTION;
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TShape *ScoreShape;
	TLabel *ScoreLabel;
	TLabel *ScoreNumberLabel;

	TShape *RecordShape;
	TLabel *RecordLabel;
	TLabel *RecordNumberLabel;

	TLabel *GameNameLabel;
	TLabel *MeaningGameLabel;

	TShape *RetryShape;
	TImage *RetryIcon;
	TShape *QuestionShape;
	TImage *QuestionIcon;
	TTimer *AnimationShape;

	void __fastcall OnKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall OnKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RetryIconClick(TObject *Sender);
	void __fastcall QustionIconClick(TObject *Sender);
	void __fastcall AnimationShapeTimer(TObject *Sender);

private:
    CShadowWindow *m_pShadowWindow;

	TShape *m_pTile[16];
	TLabel *m_pTilesNumbers[16];
	std::map<int, System::UnicodeString> m_TilesColors;

	int m_iScore;
	int m_iRecord;

	int m_iTileAnimation;
    bool m_bTileAnimation[16];

	int m_iPressKey;
	bool m_bPressKey;

    bool m_bGameOver;

private:
	bool shiftOnDirection(WORD &);
	void shiftOnRow(int &, bool &, bool []);

	int generatePositionTile();

public:
	__fastcall TMainForm(TComponent* Owner);

	void init();
	void gameOver();
	void gameBegun();
	void gameRestart();

	int *getLine(const int);
	int *getColumn(const int);

	void setScore(const int);
	int getScore() const;

	void setRecord(const int);
	int getRecord() const;

	bool addTile(const int, const int, const bool);
	bool addRandomTile(const int);

	bool isFreePosition() const;
	bool isPossibleShift() const;
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
