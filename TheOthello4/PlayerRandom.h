#pragma once
#include "BasePlayer.h"

//Š®‘Sƒ‰ƒ“ƒ_ƒ€
class PlayerRandom :public BasePlayer {
	vector<fieldstone> nextStones;
	bool SetPosition() override;
public:
	PlayerRandom(Field* field, eFieldColor myColor, bool saveF = true);
};