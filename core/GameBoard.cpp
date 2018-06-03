//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "GameBoard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
	init();
	gameBegun();
}
//---------------------------------------------------------------------------
void TMainForm::init()
{
	/* [Initialize tiles] */
	m_TilesColors[0] = "$0095A2B2";
	m_TilesColors[2] = "$00BCC7D5";
	m_TilesColors[4] = "$00A6C2D3";
	m_TilesColors[8] = "$005591EA";
	m_TilesColors[16] = "$003E71EB";
	m_TilesColors[32] = "$003859EC";
	m_TilesColors[64] = "$00183EEA";
	m_TilesColors[128] = "$0048AFDC";
	m_TilesColors[256] = "$0048AFDC";
	m_TilesColors[512] = "$0048AFDC";
	m_TilesColors[2048] = "$0048AFDC";

	TShape *tileShape = NULL;

	for(int i = 0; i < 16; i++)
	{
		tileShape = new TShape(this);
		tileShape->Parent = this;
		tileShape->Left = tilesPositions[i][0];
		tileShape->Top = tilesPositions[i][1];
		tileShape->Brush->Color = StringToColor(m_TilesColors[0]);
		tileShape->Brush->Style = bsSolid;
		tileShape->Pen->Style = psClear;
		tileShape->Shape = stRoundRect;
		tileShape->Height = 60;
		tileShape->Width = 60;

		m_pTile[i] = tileShape;

		tileShape = NULL;
	}

	/* [Initialize tiles number] */
    TLabel *tileLabelNumber = NULL;

	for(int i = 0; i < 16; i++)
	{
		tileLabelNumber = new TLabel(this);
		tileLabelNumber->Parent = this;
		tileLabelNumber->Left = tilesNumbersPositions[i][0];
		tileLabelNumber->Top = tilesNumbersPositions[i][1];
		tileLabelNumber->Font->Color = StringToColor("$00515260");
		tileLabelNumber->Font->Height = 24;
		tileLabelNumber->Font->Name = "Consolas";
		tileLabelNumber->Font->Size = 16;
		tileLabelNumber->Caption = "";
		tileLabelNumber->Height = HEIGHT_TILE_NUMBER;

		m_pTilesNumbers[i] = tileLabelNumber;

		tileLabelNumber = NULL;
	}

	/* [Initialize score & record] */
	setScore(0);
	setRecord(0);

	m_iPressKey = 0;
	m_bPressKey = false;

	m_iTileAnimation = 0;

	for(int i = 0; i < 16; i++)
	{
        m_bTileAnimation[i] = false;
    }

	m_bGameOver = false;
	m_pShadowWindow = NULL;
}
//---------------------------------------------------------------------------
void TMainForm::gameOver()
{
    m_bGameOver = true;

	if(!m_pShadowWindow)
	{
		m_pShadowWindow = new CShadowWindow(this);
    }
}
//---------------------------------------------------------------------------
void TMainForm::gameBegun()
{
	addRandomTile(2);
}
//---------------------------------------------------------------------------
void TMainForm::gameRestart()
{
	if(m_pShadowWindow)
	{
		delete m_pShadowWindow;

		m_pShadowWindow = NULL;
	}

	m_bGameOver = false;

	setScore(0);

	for(int i = 0; i < 16; i++)
	{
        addTile(i, 0, false);
	}

	gameBegun();
}
//---------------------------------------------------------------------------
int TMainForm::generatePositionTile()
{
	srand(time(NULL));

	int iPosition = rand() % 16;

	while(m_pTilesNumbers[iPosition]->Caption != "")
	{
		iPosition = rand() % 16;
	}

	return iPosition;
}
//---------------------------------------------------------------------------
bool TMainForm::addTile(const int iPosition, const int iNumber, bool bSummation)
{
	if(iPosition >= 0 && iPosition < 16)
	{
		TShape *tileShape = m_pTile[iPosition];
		TLabel *tileLabelNumber = m_pTilesNumbers[iPosition];

		if(iNumber)
		{
			if(iNumber >= 4096)
			{
				tileShape->Brush->Color = clBlack;
			}
			else
			{
				tileShape->Brush->Color = StringToColor(m_TilesColors[iNumber]);
			}

			if(bSummation)
			{
                m_iTileAnimation++;

				m_bTileAnimation[iPosition] = true;
			}

			tileLabelNumber->Caption = iNumber;
			tileLabelNumber->Left = tilesNumbersPositions[iPosition][0] + (60 / 2) - IntToStr(iNumber).Length() * (HEIGHT_TILE_NUMBER / 2);

			if(iNumber > 4)
			{
				tileLabelNumber->Font->Color = clWhite;
			}
			else
			{
				tileLabelNumber->Font->Color = StringToColor("$00515260");
            }
		}
		else
		{
			tileShape->Brush->Color = StringToColor(m_TilesColors[0]);

			tileLabelNumber->Caption = "";
        }

		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool TMainForm::addRandomTile(const int iCount)
{
	bool isAddTile = false;

	for(int i = 0, iPosition = 0, iProbality = 0; i < iCount; i++)
	{
		iPosition = generatePositionTile();

		iProbality = rand() % 10;

		if(iProbality > 2)
		{
			isAddTile = addTile(iPosition, 2, false);
		}
		else
		{
			isAddTile = addTile(iPosition, 4, false);
		}
	}

	return isAddTile;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::OnKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key < 37 || Key > 40 || AnimationShape->Enabled)
	{
        return;
    }

	if(!m_bPressKey && !m_bGameOver)
	{
		m_iPressKey = Key;
		m_bPressKey = true;

        m_iTileAnimation = 0;

		bool bShift = shiftOnDirection(Key);

		AnimationShape->Enabled = true;

		if(bShift)
		{
			addRandomTile(1);

			if(!isFreePosition() && !isPossibleShift())
			{
            	gameOver();
			}
        }
	}
}
//---------------------------------------------------------------------------
bool TMainForm::shiftOnDirection(WORD & Direction)
{
	bool bShift = false, bSummation[4] = {false};

	int *iNumbersTiles = NULL, iOldNumbersTiles[4] = {0};

	switch(Direction)
	{
		case DIRECTION_LEFT:
		case DIRECTION_RIGHT:
		{
			for(int iLine = 0; iLine < 4; iLine++)
			{
				iNumbersTiles = getLine(iLine);

				if(Direction == DIRECTION_RIGHT)
				{
					for(int i = 0, iIndex = 0, iTemp = 0; i < 2; i++)
					{
                        iIndex = 4 - i - 1;

						iTemp = iNumbersTiles[i];

						iNumbersTiles[i] = iNumbersTiles[iIndex];
						iNumbersTiles[iIndex] = iTemp;
					}
				}

				shiftOnRow(*iNumbersTiles, bShift, bSummation);

				if(Direction == DIRECTION_RIGHT)
				{
					for(int i = 0, iIndex = 0, iTemp = 0; i < 2; i++)
					{
                        iIndex = 4 - i - 1;

						/* */
						iTemp = iNumbersTiles[i];

						iNumbersTiles[i] = iNumbersTiles[iIndex];
						iNumbersTiles[iIndex] = iTemp;

						/* */
						iTemp = bSummation[i];

						bSummation[i] = bSummation[iIndex];
						bSummation[iIndex] = iTemp;
					}
				}

				for(int iStart = iLine * 4, iEnd = iLine * 4 + 4, iIndex = 0; iStart < iEnd; iStart++, iIndex++)
				{
					addTile(iStart, iNumbersTiles[iIndex], bSummation[iIndex]);
				}

				for(int i = 0; i < 4; i++)
				{
                    bSummation[i] = 0;
                }
			}

			break;
		}
		case DIRECTION_UP:
		case DIRECTION_DOWN:
		{
			for(int iColumn = 0; iColumn < 4; iColumn++)
			{
				iNumbersTiles = getColumn(iColumn);

				for(int i = 0; i < 4; i++)
				{
                    iOldNumbersTiles[i] = iNumbersTiles[i];
                }

				if(Direction == DIRECTION_DOWN)
				{
					for(int i = 0, iIndex = 0, iTemp = 0; i < 2; i++)
					{
                        iIndex = 4 - i - 1;

						iTemp = iNumbersTiles[i];

						iNumbersTiles[i] = iNumbersTiles[iIndex];
						iNumbersTiles[iIndex] = iTemp;
					}
				}

				shiftOnRow(*iNumbersTiles, bShift, bSummation);

				if(Direction == DIRECTION_DOWN)
				{
					for(int i = 0, iIndex = 0, iTemp = 0; i < 2; i++)
					{
						iIndex = 4 - i - 1;

						/* */
						iTemp = iNumbersTiles[i];

						iNumbersTiles[i] = iNumbersTiles[iIndex];
						iNumbersTiles[iIndex] = iTemp;

						/* */
						iTemp = bSummation[i];

						bSummation[i] = bSummation[iIndex];
						bSummation[iIndex] = iTemp;
					}
				}

				for(int iStart = iColumn, iEnd = iColumn + 13, iIndex = 0; iStart < iEnd; iStart += 4, iIndex++)
				{
					addTile(iStart, iNumbersTiles[iIndex], bSummation[iIndex]);
				}

				for(int i = 0; i < 4; i++)
				{
                    bSummation[i] = 0;
                }
			}

			break;
		}
	}

	return bShift;
}
//---------------------------------------------------------------------------
void TMainForm::shiftOnRow(int &iTilesNumbers, bool &bShift, bool bSummation[])
{
	int *pTilesNumbers = &iTilesNumbers;

	int iRealTilesNumbers[4] = {0};

	for(int iStart = 0, iIndex = 0; iStart < 4; iStart++)
	{
		if(pTilesNumbers[iStart] != 0)
		{
			if(iIndex != iStart)
			{
				bShift = true;
			}

			iRealTilesNumbers[iIndex] = pTilesNumbers[iStart];

			iIndex++;
		}
	}

	for(int i = 0; i < 4; i++)
	{
		pTilesNumbers[i] = 0;
    }

	for(int iStart = 0, iIndex = 0; iStart < 4; iStart++, iIndex++)
	{
		if((iStart + 1) < 4 && (iRealTilesNumbers[iStart] == iRealTilesNumbers[iStart + 1]) && iRealTilesNumbers[iStart] != 0)
		{
			bShift = true;
			bSummation[iIndex] = true;

			pTilesNumbers[iIndex] = iRealTilesNumbers[iStart] * 2;

			setScore(getScore() + pTilesNumbers[iIndex]);

			if(getScore() > getRecord())
			{
                setRecord(getScore());
            }

			iStart++;
		}
		else
		{
			pTilesNumbers[iIndex] = iRealTilesNumbers[iStart];
		}
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::OnKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(m_iPressKey == Key)
	{
		m_bPressKey = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall CShadowWindow::OnClickButton_ExitGame(TObject *Sender)
{
	MainForm->Close();
}
//---------------------------------------------------------------------------
void __fastcall CShadowWindow::OnClickButton_RestartGame(TObject *Sender)
{
	MainForm->gameRestart();
}
//---------------------------------------------------------------------------
int *TMainForm::getLine(const int iLine)
{
	int *iTilesNumbers = new int[4];

	for(int iStart = iLine * 4, iEnd = iLine * 4 + 4, iIndex = 0; iStart < iEnd; iStart++, iIndex++)
	{
		if(m_pTilesNumbers[iStart]->Caption == "")
		{
			iTilesNumbers[iIndex] = 0;
		}
		else
		{
			try
			{
				iTilesNumbers[iIndex] = StrToInt(m_pTilesNumbers[iStart]->Caption);
			}
			catch(EConvertError&)
			{
                ShowMessage("2048: [Error] #2");
            }
        }
	}

	return iTilesNumbers;
}
//---------------------------------------------------------------------------
int *TMainForm::getColumn(const int iColumn)
{
	int *iTilesNumbers = new int[4];

	for(int iStart = iColumn, iEnd = iColumn + 13, iIndex = 0; iStart < iEnd; iStart += 4, iIndex++)
	{
		if(m_pTilesNumbers[iStart]->Caption == "")
		{
			iTilesNumbers[iIndex] = 0;
		}
		else
		{
			try
			{
				iTilesNumbers[iIndex] = StrToInt(m_pTilesNumbers[iStart]->Caption);
			}
			catch(EConvertError&)
			{
                ShowMessage("2048: [Error] #2");
            }
        }
	}

	return iTilesNumbers;
}
//---------------------------------------------------------------------------
void TMainForm::setScore(const int iScore)
{
	m_iScore = iScore;

	ScoreNumberLabel->Caption = iScore;
}
//---------------------------------------------------------------------------
int TMainForm::getScore() const
{
	return m_iScore;
}
//---------------------------------------------------------------------------
void TMainForm::setRecord(const int iRecord)
{
	m_iRecord = iRecord;

	RecordNumberLabel->Caption = iRecord;
}
//---------------------------------------------------------------------------
int TMainForm::getRecord() const
{
	return m_iRecord;
}
//---------------------------------------------------------------------------
bool TMainForm::isFreePosition() const
{
	for(int iStart = 0; iStart < 16; iStart++)
	{
		if(m_pTilesNumbers[iStart]->Caption == "")
		{
			return true;
        }
    }

	return false;
}
//---------------------------------------------------------------------------
bool TMainForm::isPossibleShift() const
{
	if(!isFreePosition())
	{
		int iNumberFirst = 0, iNumberSecond = 0;

		for(int iLine = 0; iLine < 4; iLine++)
		{
			for(int iStart = iLine * 4, iEnd = iLine * 4 + 4; iStart < iEnd; iStart++)
			{
				if((iStart + 1) < iEnd)
				{
					iNumberFirst = StrToInt(m_pTilesNumbers[iStart]->Caption);
					iNumberSecond = StrToInt(m_pTilesNumbers[iStart + 1]->Caption);

					if(iNumberFirst == iNumberSecond)
					{
						return true;
					}
                }
            }
		}

		for(int iColumn = 0; iColumn < 4; iColumn++)
		{
			for(int iStart = iColumn, iEnd = iColumn + 13; iStart < iEnd; iStart += 4)
			{
				if((iStart + 4) < iEnd)
				{
					iNumberFirst = StrToInt(m_pTilesNumbers[iStart]->Caption);
					iNumberSecond = StrToInt(m_pTilesNumbers[iStart + 4]->Caption);

					if(iNumberFirst == iNumberSecond)
					{
						return true;
					}
                }
            }
		}

		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RetryIconClick(TObject *Sender)
{
	gameRestart();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::QustionIconClick(TObject *Sender)
{
	FAQForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::AnimationShapeTimer(TObject *Sender)
{
	for(int i = 0; i < 16; i++)
	{
		if(m_bTileAnimation[i])
		{
			TShape *tile = m_pTile[i];

			if (tile->Tag == 0)	{
				tile->Width += 2;
				tile->Height += 2;

				tile->Top -= 1;
				tile->Left -= 1;

				if (tile->Width >= 68 || tile->Height >= 68)  {
					tile->Tag = 1;
				}
			} else	{
				tile->Width -= 2;
				tile->Height -= 2;

				tile->Top += 1;
				tile->Left += 1;

				if (tile->Width <= 60 || tile->Height <= 60) {
					tile->Tag = 0;

					tile->Width = 60;
					tile->Height = 60;

                    m_iTileAnimation--;

					m_bTileAnimation[i] = false;
				}
			}
		}
	}

	if(!m_iTileAnimation)
	{
        AnimationShape->Enabled = false;
    }
}
//---------------------------------------------------------------------------

