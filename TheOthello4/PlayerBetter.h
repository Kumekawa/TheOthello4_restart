#pragma once
#include "BasePlayer.h"

//�悳���Ȃ�ɏ]���č���Ă݂�
//�Q�l:https://bassy84.net/othello-syosin.html
class PlayerBetter :public BasePlayer {
	//�Ƃ肠����4�ɕ������Ă݂�
	int term[3] = { 10, 20, 30};
	
	bool SetPosition() override;
public:
	PlayerBetter(Field* field, eFieldColor myColor, bool saveF = true);
};
