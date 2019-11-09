#include "PlayerRoler.h"

bool PlayerRoler::SetPosition(){
	auto ft = field->GetNextStones();
	int min = MFS_XSIZE * MFS_YSIZE;
	for (int i = 0; i < ft.size(); ++i) {
		int tx = ft[i].x;
		int ty = ft[i].y;
		int m = tx + ty * MFS_XSIZE;
		if (min > m) {
			min = m;
			fx = tx;
			fy = ty;
		}
	}
	return true;
}

PlayerRoler::PlayerRoler(Field* field, eFieldColor myColor, bool saveF) :BasePlayer(field, myColor, saveF) 
{

}

