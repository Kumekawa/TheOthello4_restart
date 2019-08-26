#include "PlayerRandomHyper.h"
#include "PlayerRandom.h"

bool PlayerRandomHyper::SetPosition() {
	//仮想環境を作り、自分の手番が最大になるように選ぶ
	eFieldColor _turnPlayer = myColor;
	Field _field(&_turnPlayer);
	int max = -1;

	_field.SetFieldStone(field->GetFieldStone());
	for (int i = 0; i < _field.GetNextStones().size(); ++i) {
		//終了するまで殴り合い
		int t = 0;
		auto ft = _field;
		_turnPlayer = myColor;

		int tx = ft.GetNextStones()[i].x;
		int ty = ft.GetNextStones()[i].y;

		for (int i = 0; i < 100; ++i) {
			ft = _field;
			_turnPlayer = myColor;

			//ここで初回の石配置
			ft.SetStone(tx, ty);
			PlayerRandom Player2(&ft, &_turnPlayer, GetChangeFieldColor(myColor), false);
			PlayerRandom Player1(&ft, &_turnPlayer, myColor, false);

			while (ft.GetEndF() == 0) {
				ft.Update();
				Player2.Update();
				Player1.Update();
			}

		}
		if (max < t) {
			max = t;
			fx = tx;
			fy = ty;
		}
	}
	return true;
}

PlayerRandomHyper::PlayerRandomHyper(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF) :BasePlayer(field, turnPlayer, myColor, saveF) {

}
