#pragma once
#include "BasePlayer.h"

//Ÿ‚É‘Šè‚ªæ‚ê‚éè”‚ªÅ¬‚É‚È‚é‚æ‚¤‚É‘I‚Ô
class PlayerNextPointMin :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerNextPointMin(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF = true);
};
