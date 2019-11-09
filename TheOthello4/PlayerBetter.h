#pragma once
#include "BasePlayer.h"

//‚æ‚³‚°‚È‚â‚Â‚É]‚Á‚Äì‚Á‚Ä‚İ‚é
//Ql:https://bassy84.net/othello-syosin.html
class PlayerBetter :public BasePlayer {
	//‚Æ‚è‚ ‚¦‚¸4‚Â‚É•ªŠ„‚µ‚Ä‚İ‚é
	int term[3] = { 10, 20, 30};
	
	bool SetPosition() override;
public:
	PlayerBetter(Field* field, eFieldColor myColor, bool saveF = true);
};
