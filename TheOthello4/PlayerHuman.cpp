#include "PlayerHuman.h"
#include "MacroColor.h"
#include "Between.h"

PlayerHuman::PlayerHuman(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF) :BasePlayer(field, turnPlayer, myColor, saveF) {

}

bool PlayerHuman::SetPosition() {
	int mx, my;
	GetMousePoint(&mx, &my);
	fx = mx / MFS_UNIT;
	fy = my / MFS_UNIT;
	SetBetween(0, &fx, MFS_XSIZE);
	SetBetween(0, &fy, MFS_YSIZE);



	for (int i = 0; i < MFS_XSIZE; ++i) {
		for (int j = 0; j < MFS_YSIZE; ++j) {
			int tx = i * MFS_UNIT + MFS_UNIT / 2;
			int ty = j * MFS_UNIT + MFS_UNIT / 2;
			int c = MC_BLACK;
			auto tf = field->GetNextStones();
			for (int n = 0; n < tf.size(); ++n) {
				if (tf[n].x == fx && tf[n].y == fy) {
					if (tf[n].stone[i][j] != eFC_None) {
						if (tf[n].stone[i][j] == eFC_White) {
							c = MC_WHITE;
						}
						DrawCircle(tx, ty, MFS_UNIT * 4 / 9, c, 0);
					}
				}
			}
		}
	}



	if (GetMouseInput() & MOUSE_INPUT_LEFT) {
		auto tf = field->GetNextStones();
		for (int i = 0; i < tf.size(); ++i) {
			if (tf[i].x == fx && tf[i].y == fy) {
				return true;
			}
		}
	}
	return false;
}
