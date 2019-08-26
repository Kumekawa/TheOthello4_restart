#include "PlayerRandomHyper.h"
#include "PlayerRandom.h"

bool PlayerRandomHyper::SetPosition() {
	//���z�������A�����̎�Ԃ��ő�ɂȂ�悤�ɑI��
	eFieldColor _turnPlayer = myColor;
	Field _field(&_turnPlayer);
	int max = -1;

	_field.SetFieldStone(field->GetFieldStone());
	for (int i = 0; i < _field.GetNextStones().size(); ++i) {
		//�I������܂ŉ��荇��
		int t = 0;
		auto ft = _field;
		_turnPlayer = myColor;

		int tx = ft.GetNextStones()[i].x;
		int ty = ft.GetNextStones()[i].y;

		for (int i = 0; i < 100; ++i) {
			ft = _field;
			_turnPlayer = myColor;

			//�����ŏ���̐Δz�u
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
