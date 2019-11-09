#include "PlayerRandom.h"
#include "DxLib.h"

bool PlayerRandom::SetPosition(){
	nextStones = field->GetNextStones();
	if (nextStones.size() > 0) {
		int t = GetRand(nextStones.size() - 1);
		fx = nextStones[t].x;
		fy = nextStones[t].y;
		return true;
	}
	return false;
}

PlayerRandom::PlayerRandom(Field* field, eFieldColor myColor, bool saveF) :BasePlayer(field, myColor, saveF) {

}
