#pragma once
#include "BasePlayer.h"

//置ける場所に置いたとき、お互いがランダムに置きあうのを100回繰り返し、勝率が高いところを選ぶ
class PlayerRandomHyper :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerRandomHyper(Field* field, eFieldColor myColor, bool saveF = true);
};
