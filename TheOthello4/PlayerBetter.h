#pragma once
#include "BasePlayer.h"

//よさげなやつに従って作ってみる
//参考:https://bassy84.net/othello-syosin.html
class PlayerBetter :public BasePlayer {
	//とりあえず4つに分割してみる
	int term[3] = { 10, 20, 30};
	
	bool SetPosition() override;
public:
	PlayerBetter(Field* field, eFieldColor myColor, bool saveF = true);
};
