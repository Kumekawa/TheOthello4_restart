#pragma once
#include "BasePlayer.h"

//Ÿ‚ªÅ¬‚Ì‚à‚Ì‚ğ’T‚·
class PlayerNextMin :public BasePlayer {
	vector<fieldstone> nextStones;
	bool SetPosition() override;
public:
	PlayerNextMin(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF = true);
};
