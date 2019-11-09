#include "FieldContent.h"
#include "DxLib.h"
#include "MacroColor.h"

eFieldColor GetChangeFieldColor(eFieldColor turn) {
	switch (turn)
	{
	case eFC_Black:
		turn = eFC_White;
		break;
	case eFC_White:
		turn = eFC_Black;
		break;
	default:
		break;
	}
	return turn;
}

void ChangeFieldColor(eFieldColor* turn) {
	*turn = GetChangeFieldColor(*turn);
}

//どれが一番多いか
eFieldColor fieldstone::GetMaxColor() {
	SetAmount();
	int t = amount[eFC_Black] - amount[eFC_White];
	if (t > 0) {
		return eFC_Black;
	}
	else if (t < 0) {
		return eFC_White;
	}
	else {
		return eFC_None;
	}
}

void fieldstone::SetAmount() {
	for (int i = 0; i < 3; ++i) {
		amount[i] = 0;
	}
	for (int i = 0; i < MFS_XSIZE; ++i) {
		for (int j = 0; j < MFS_YSIZE; ++j) {
			amount[stone[i][j]]++;
		}
	}
}

//盤面を文字列で表し、最後にx,yを付与する
string fieldstone::GetString() {
	stringstream ss;
	for (int i = 0; i < MFS_XSIZE; ++i) {
		for (int j = 0; j < MFS_YSIZE; ++j) {
			ss << stone[i][j];
		}
	}
	ss << "," << x << "," << y;
	return ss.str();
}

bool fieldstone::GetEquals(fieldstone fieldStone) {
	for (int i = 0; i < MFS_XSIZE; ++i) {
		for (int j = 0; j < MFS_YSIZE; ++j) {
			if (fieldStone.stone[i][j] != stone[i][j]) {
				return false;
			}
		}
	}
	return true;
}

//指定された位置に現在のフィールドのみを描画する
void fieldstone::DrawField(int x, int y, int size) {
	double r = (double)size / MWS_YMAX;
	double unitSize = MFS_UNIT * r;
	DrawBox(x, y, x + size, y + size, MC_GREEN, 1);
	for (int i = 0; i <= MFS_XSIZE; ++i) {
		//DrawLineVertical(i * MFS_UNIT, MC_BLACK);
		DrawLine(i * unitSize + x, y, i * unitSize + x, y + size, MC_BLACK);
	}
	for (int j = 0; j <= MFS_YSIZE; ++j) {
		//DrawLineHorizontal(j * MFS_UNIT, MC_BLACK);
		DrawLine(x, j * unitSize + y, x + size, j * unitSize + y, MC_BLACK);
	}
	for (int i = 0; i < MFS_XSIZE; ++i) {
		for (int j = 0; j < MFS_YSIZE; ++j) {
			//DrawStone(i, j, fieldStone);
			int a = i * unitSize + unitSize / 2 + x;
			int b = j * unitSize + unitSize / 2 + y;
			int c = MC_BLACK;
			if (stone[i][j] != eFC_None) {
				if (stone[i][j] == eFC_White) {
					c = MC_WHITE;
				}
				DrawCircle(a, b, unitSize / 3, c, 1);
			}
		}
	}

}
