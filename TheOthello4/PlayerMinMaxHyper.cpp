#include "PlayerMinMaxHyper.h"
#include "PlayerNextMax.h"

bool PlayerMinMaxHyper::SetPosition() {
	//仮想環境を作り、自分の手番が最大になるように選ぶ
	eFieldColor _turnPlayer = myColor;
	Field _field(_turnPlayer);
	int max = -1;

	_field.SetFieldStone(field->GetFieldStone());
	for (int i = 0; i < _field.GetNextStones().size(); ++i) {
		auto ft = _field;
		_turnPlayer = myColor;

		int tx = ft.GetNextStones()[i].x;
		int ty = ft.GetNextStones()[i].y;
		//ここで初回の石配置
		ft.SetStone(tx, ty);
		PlayerNextMax Player2(&ft, GetChangeFieldColor(myColor), false);
		PlayerNextMax Player1(&ft, myColor, false);

		//終了するまで殴り合い
		while (ft.GetEndF() != true) {
			ft.Update();
			Player2.Update();
			Player1.Update();
		}
		if (max < ft.GetFieldStone().amount[myColor]) {
			max = ft.GetFieldStone().amount[myColor];
			fx = tx;
			fy = ty;
		}
	}
	return true;
}

PlayerMinMaxHyper::PlayerMinMaxHyper(Field* field, eFieldColor myColor, bool saveF) :BasePlayer(field, myColor, saveF) {

}
