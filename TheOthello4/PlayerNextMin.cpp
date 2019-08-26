#include "PlayerNextMin.h"

PlayerNextMin::PlayerNextMin(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF):BasePlayer(field,turnPlayer,myColor,saveF){

}

bool PlayerNextMin::SetPosition() {
	nextStones = field->GetNextStones();
	if (nextStones.size() > 0) {
		int m = -1;
		for (int i = 0; i < nextStones.size(); ++i) {
			fieldstone t = nextStones[i];
			if (m == -1 || m > t.amount[myColor]) {
				m = t.amount[myColor];
				fx = t.x;
				fy = t.y;
			}
		}
		return true;
	}
	return false;
}