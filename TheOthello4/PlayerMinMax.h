#pragma once
#include "BasePlayer.h"

//minmax���Ɋ�Â��Ĕz�u����B�Ƃ肠����2�i�K
class PlayerMinMax :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerMinMax(Field* field, eFieldColor myColor, bool saveF = true);
};
