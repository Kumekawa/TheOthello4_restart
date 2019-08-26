#include "PlayerMinMax.h"
#include "PlayerNextMax.h"

bool PlayerMinMax::SetPosition() {
	//‰¼‘zŠÂ‹«‚ðì‚èAŽ©•ª‚ÌŽè”Ô‚ªÅ‘å‚É‚È‚é‚æ‚¤‚É‘I‚Ô
	eFieldColor _turnPlayer = myColor;
	Field _field(&_turnPlayer);
	int max = -1;

	_field.SetFieldStone(field->GetFieldStone());
	for (int i = 0; i < _field.GetNextStones().size(); ++i) {
		auto ft = _field;
		_turnPlayer = myColor;

		int tx = ft.GetNextStones()[i].x;
		int ty = ft.GetNextStones()[i].y;
		ft.SetStone(tx, ty);
		ft.Update();
		PlayerNextMax Player2(&ft, &_turnPlayer, GetChangeFieldColor(myColor), false);
		Player2.Update();

		PlayerNextMax Player1(&ft, &_turnPlayer, myColor, false);
		Player1.Update();
		if (max < ft.GetFieldStone().amount[myColor]) {
			max = ft.GetFieldStone().amount[myColor];
			fx = tx;
			fy = ty;
		}
	}
	return true;
}

PlayerMinMax::PlayerMinMax(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF) :BasePlayer(field, turnPlayer, myColor, saveF) {

}
