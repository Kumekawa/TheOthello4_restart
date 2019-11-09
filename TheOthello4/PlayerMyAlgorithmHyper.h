#pragma once
#include "BasePlayer.h"


//minmax方に基づいて配置する。お互いが最大ずつ取り合ったとき、最後に自身が最大のものを選ぶ
class PlayerMyAlgorithmHyper :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerMyAlgorithmHyper(Field* field, eFieldColor myColor, bool saveF = true);
};
