#include "FieldContent.h"

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

//‚Ç‚ê‚ªˆê”Ô‘½‚¢‚©
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

//”Õ–Ê‚ð•¶Žš—ñ‚Å•\‚µAÅŒã‚Éx,y‚ð•t—^‚·‚é
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
