#pragma once
#include "BasePlayer.h"

//minmax方に基づいて配置する。とりあえず2段階
class PlayerMinMax :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerMinMax(Field* field, eFieldColor myColor, bool saveF = true);
};
