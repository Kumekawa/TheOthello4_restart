#pragma once
#include "BasePlayer.h"
#include "DxLib.h"
#include "MacroColor.h"

struct drawfield {
	fieldstone fieldStone;
	vector<drawfield>nextDrawFields;
	bool bestF;
	void Add(fieldstone fieldStone) {
		drawfield drawField;
		drawField.fieldStone = fieldStone;
		drawField.nextDrawFields.clear();
		nextDrawFields.push_back(drawField);
		bestF = false;
	}
	drawfield* GetLast() {
		return &nextDrawFields[nextDrawFields.size() - 1];
	}
};

//minmax方に基づいて配置する。本を参照して実装したやつ
class PlayerMinMaxFromBooks :public BasePlayer {
	bool SetPosition() override;

	drawfield drawFields;
	vector<int> bestX;
	vector<int> bestY;
	long amount;
	int alphaMax;
	int betaMin;
	bool abMethod = true;

	int SearchBestPosition(Field _field,int x,int y,int deep,drawfield* drawField,int aMax,int bMin) {
		//drawField.push_back(_field);
		_field.SetStone(x, y);
		_field.Update();
		amount++;
		if (_field.GetEndF() || deep == 20) {
			eFieldColor c = eFieldColor::eFC_Black;
			if (myColor == c) {
				c = eFC_White;
			}
			if (int zokoF = 0) {
				return _field.GetFieldStone().amount[c];
			}
			else {
				return _field.GetFieldStone().amount[myColor];
			}
		}
		if (abMethod) {
			if (_field.GetTurnPlayer() == myColor) {
				for (int i = 0; i < _field.GetNextStones().size(); ++i) {
					fieldstone stones = _field.GetNextStones()[i];
					drawField->Add(stones);
					int t = SearchBestPosition(_field, stones.x, stones.y, deep + 1, (drawField->GetLast()), aMax, bMin);
					if (aMax < t) {
						aMax = t;
					}
					if (aMax >= bMin) {
						break;
					}
				}
				return aMax;
			}
			else {
				for (int i = 0; i < _field.GetNextStones().size(); ++i) {
					fieldstone stones = _field.GetNextStones()[i];
					drawField->Add(stones);
					int t = SearchBestPosition(_field, stones.x, stones.y, deep + 1, (drawField->GetLast()), aMax, bMin);
					if (bMin > t) {
						bMin = t;
					}
					if (aMax >= bMin) {
						break;
					}
				}
				return bMin;
			}
		}
		else {
			//drawField.push_back(_field);
			_field.SetStone(x, y);
			_field.Update();
			amount++;
			if (_field.GetEndF()) {
				return _field.GetFieldStone().amount[myColor];
			}
			
			int max = -1;
			int min = MFS_AMOUNT + 1;
			int maxX, maxY;
			int minX, minY;
			int maxBest = 0;
			int minBest = 0;
			for (int i = 0; i < _field.GetNextStones().size(); ++i) {
				fieldstone stones = _field.GetNextStones()[i];
				drawField->Add(stones);
				int t = SearchBestPosition(_field, stones.x, stones.y, deep + 1, (drawField->GetLast()),aMax,bMin);
				if (_field.GetTurnPlayer() == myColor) {
					if (max < t) {
						max = t;
						maxX = stones.x;
						maxY = stones.y;
						maxBest = i;
					}
				}
				else {
					if (min > t) {
						min = t;
						minX = stones.x;
						minY = stones.y;
						minBest = i;
					}
				}
			}
			if (deep == 0) {
				int t = 0;
			}
			if (_field.GetTurnPlayer() == myColor) {
				bestX.push_back(maxX);
				bestX.push_back(maxY);
				drawField->nextDrawFields[maxBest].bestF = true;
				return max;
			}
			else {
				bestX.push_back(minX);
				bestX.push_back(minY);
				drawField->nextDrawFields[minBest].bestF = true;
				return min;
			}
		}
	}

	//parentは直前のやつがあった場所,rightは横幅
	void DrawDrawField(drawfield drawField,int x,int y,int leftX,int rightX,int parentX,int parentY,int size) {
		if (drawField.bestF) {
			DrawLine(x + size / 2, y + size / 2, parentX + size / 2, parentY + size / 2, MC_RED);
		}
		else{
			DrawLine(x + size / 2, y + size / 2, parentX + size / 2, parentY + size / 2, MC_WHITE);
		}
		drawField.fieldStone.DrawField(x,y,size);

		if (drawField.nextDrawFields.empty()) {
			return;
		}
		
		int offset = (rightX - leftX) / drawField.nextDrawFields.size();
		for (int i = 0; i < drawField.nextDrawFields.size(); ++i) {
			DrawDrawField(drawField.nextDrawFields[i], (offset * i + offset * (i + 1)) / 2 + leftX, y + size,offset * i + leftX,offset * (i + 1) + leftX,x,y,size);
		}

	}

public:
	PlayerMinMaxFromBooks(Field* field, eFieldColor myColor, bool saveF = true);
};

