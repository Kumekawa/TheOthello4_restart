#pragma once
#include "BasePlayer.h"

//次に相手が取れる手数が最小になるように選ぶ
class PlayerNextPointMin :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerNextPointMin(Field* field, eFieldColor myColor, bool saveF = true);
};
