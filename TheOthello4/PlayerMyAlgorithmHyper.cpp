#include "PlayerMyAlgorithmHyper.h"
#include "PlayerNextMax.h"

bool PlayerMyAlgorithmHyper::SetPosition() {
	//自分なりのアルゴリズムを作ってみる
	auto fs = field->GetNextStones();
	////5%の確率でランダム
	//if (GetRand(99) < 5) {
	//	auto r = GetRand(fs.size() - 1);
	//	fx = fs[r].x;
	//	fy = fs[r].y;
	//	return true;
	//}

	//四隅が取れれば取る
	for (int i = 0; i < fs.size(); ++i) {
		int tx = fs[i].x;
		int ty = fs[i].y;
		for (int a = 0; a < MFS_XSIZE; a += MFS_XSIZE - 1) {
			for (int b = 0; b < MFS_YSIZE; b += MFS_YSIZE - 1) {
				if (tx == a && ty == b) {
					fx = tx;
					fy = ty;
					return true;
				}
			}
		}
	}

	//仮想環境を作り、自分の手番が最大になるように選ぶ
	eFieldColor _turnPlayer = myColor;
	Field _field(_turnPlayer);
	double max = -1;

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
		while (ft.GetEndF() == 0) {
			ft.Update();
			Player2.Update();
			Player1.Update();
		}

		//もしtxまたはtyが端ならボーナスを付ける
		double bonus = 1.0;
		if (tx == 0 || ty == 0 || tx == MFS_XSIZE - 1 || ty == MFS_YSIZE - 1) {
			bonus = 1.25;
		}

		//もし四隅の隣をとらなければならない場合、得点を下げる
		double minus = 1.0;
		int x_s[] = {
			1,MFS_XSIZE - 2,
			0,1,MFS_XSIZE - 2, MFS_XSIZE - 1,
			0,1,MFS_XSIZE - 2, MFS_XSIZE - 1,
			1,MFS_XSIZE - 2,
		};
		int y_s[] = {
			0,0,
			1,1,1,1,
			MFS_YSIZE - 2,MFS_YSIZE - 2,MFS_YSIZE - 2,MFS_YSIZE - 2,
			MFS_YSIZE - 1,MFS_YSIZE - 1,
		};
		for (int i = 0; i < 12; ++i) {
			if (tx == x_s[i] && ty == y_s[i]) {
				minus = 0.5;
			}
		}

		if (max < ft.GetFieldStone().amount[myColor] * bonus * minus) {
			max = ft.GetFieldStone().amount[myColor] * bonus * minus;
			fx = tx;
			fy = ty;
		}
	}
	return true;
}

PlayerMyAlgorithmHyper::PlayerMyAlgorithmHyper(Field* field, eFieldColor myColor, bool saveF) :BasePlayer(field, myColor, saveF) {

}
