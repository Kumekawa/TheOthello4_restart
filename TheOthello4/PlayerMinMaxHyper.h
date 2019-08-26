#pragma once
#include "BasePlayer.h"

//minmax方に基づいて配置する。お互いが最大ずつ取り合ったとき、最後に自身が最大のものを選ぶ
class PlayerMinMaxHyper :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerMinMaxHyper(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF = true);
};
