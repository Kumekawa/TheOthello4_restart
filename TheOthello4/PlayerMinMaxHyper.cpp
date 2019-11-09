#include "PlayerMinMaxHyper.h"
#include "PlayerNextMax.h"

bool PlayerMinMaxHyper::SetPosition() {
	//���z�������A�����̎�Ԃ��ő�ɂȂ�悤�ɑI��
	eFieldColor _turnPlayer = myColor;
	Field _field(_turnPlayer);
	int max = -1;

	_field.SetFieldStone(field->GetFieldStone());
	for (int i = 0; i < _field.GetNextStones().size(); ++i) {
		auto ft = _field;
		_turnPlayer = myColor;

		int tx = ft.GetNextStones()[i].x;
		int ty = ft.GetNextStones()[i].y;
		//�����ŏ���̐Δz�u
		ft.SetStone(tx, ty);
		PlayerNextMax Player2(&ft, GetChangeFieldColor(myColor), false);
		PlayerNextMax Player1(&ft, myColor, false);

		//�I������܂ŉ��荇��
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
