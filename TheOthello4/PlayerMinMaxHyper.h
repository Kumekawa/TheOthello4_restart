#pragma once
#include "BasePlayer.h"

//minmax���Ɋ�Â��Ĕz�u����B���݂����ő傸��荇�����Ƃ��A�Ō�Ɏ��g���ő�̂��̂�I��
class PlayerMinMaxHyper :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerMinMaxHyper(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF = true);
};
