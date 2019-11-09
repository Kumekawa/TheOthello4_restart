#include "PlayerNextPointMin.h"

bool PlayerNextPointMin::SetPosition() {
	//仮想環境を作り、相手の手数が最小になるように選ぶ
	eFieldColor _turnPlayer = myColor;
	Field _field(_turnPlayer);
	int min = -1;

	_field.SetFieldStone(field->GetFieldStone());
	for (int i = 0; i < _field.GetNextStones().size(); ++i) {
		auto ft = _field;
		_turnPlayer = myColor;

		int tx = ft.GetNextStones()[i].x;
		int ty = ft.GetNextStones()[i].y;
		//ここで初回の石配置
		ft.SetStone(tx, ty);

		ft.Update();

		if (min == -1 || min > ft.GetNextStones().size()) {
			min = ft.GetNextStones().size();
			fx = tx;
			fy = ty;
		}
	}
	return true;
}

PlayerNextPointMin::PlayerNextPointMin(Field* field, eFieldColor myColor, bool saveF) :BasePlayer(field, myColor, saveF) {

}
