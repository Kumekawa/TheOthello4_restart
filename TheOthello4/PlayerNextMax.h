#pragma once
#include "BasePlayer.h"

//Ÿ‚ªÅ‘å‚Ì‚à‚Ì‚ğ’T‚·
class PlayerNextMax :public BasePlayer {
	vector<fieldstone> nextStones;
	bool SetPosition() override;
public:
	PlayerNextMax(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF = true);
};
