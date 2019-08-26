#include "PlayerMyalgorithm.h"
#include "DxLib.h"

bool PlayerMyAlgorithm::SetPosition() {
	//自分なりのアルゴリズムを作ってみる
	auto fs = field->GetNextStones();
	//5%の確率でランダム
	if (GetRand(99) < 5) {
		auto r = GetRand(fs.size() - 1);
		fx = fs[r].x;
		fy = fs[r].y;
		return true;
	}

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

	//次の盤面時、自分のものが最大になるように選ぶ
	int max = -1;
	for (int i = 0; i < fs.size(); ++i) {
		if (max < fs[i].amount[myColor]) {
			fx = fs[i].x;
			fy = fs[i].y;
		}
	}
	return true;
}

PlayerMyAlgorithm::PlayerMyAlgorithm(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF) :BasePlayer(field, turnPlayer, myColor, saveF) {

}
