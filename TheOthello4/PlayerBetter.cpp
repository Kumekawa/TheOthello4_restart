#include "PlayerBetter.h"

bool PlayerBetter::SetPosition() {
	int turn = field->GetElapsedTurn();
	if (turn < term[0]) {
		//Ž©•ª‚ÌƒRƒ}‚ª­‚È‚­‚È‚éˆÊ’u‚É‘Å‚Â
		//http://bassy84.net/syosin-itihoukougaesi.html
		int min = -1;
		auto fs = field->GetNextStones();
		for (int i = 0; i < fs.size(); ++i) {
			if (min == -1 || min > fs[i].amount[myColor]) {
				min = fs[i].amount[myColor];
				fx = fs[i].x;
				fy = fs[i].y;
			}
		}
	}
	return 0;
}

PlayerBetter::PlayerBetter(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF) :BasePlayer(field, turnPlayer, myColor, saveF) {

}
