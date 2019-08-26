#include "PlayerMaxValue.h"

double PlayerMaxValue::GetValue(fieldstone fieldStone, int turn) {
	double sum = 0;
	for (int i = 0; i < MFS_XSIZE; ++i) {
		for (int j = 0; j < MFS_YSIZE; ++j) {
			int t = 0;
			auto f = fieldStone.stone[i][j];
			if (f == myColor) {
				t = 1;
			}
			else if (f == GetChangeFieldColor(myColor)) {
				t = -1;
			}
			else {
				t = 0;
			}
			sum += t * w[turn][i][j];
		}
	}
	return sum;
}

bool PlayerMaxValue::SetPosition() {
	if (endF) {
		return false;
	}
	double max;
	bool f = true;
	auto tf = field->GetNextStones();
	for (int i = 0; i < tf.size(); ++i) {
		double tv = GetValue(tf[i], field->GetElapsedTurn() + 1);
		if (f || max < tv) {
			max = tv;
			fx = tf[i].x;
			fy = tf[i].y;
		}
	}
	return true;
}

PlayerMaxValue::PlayerMaxValue(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, double weight[MFS_AMOUNT - 4][MFS_XSIZE][MFS_YSIZE], bool saveF) :BasePlayer(field, turnPlayer, myColor, saveF) {
	for (int i = 0; i < MFS_AMOUNT - 4; ++i) {
		for (int x = 0; x < MFS_XSIZE; ++x) {
			for (int y = 0; y < MFS_YSIZE; ++y) {
				w[i][x][y] = weight[i][x][y];
			}
		}
	}
}