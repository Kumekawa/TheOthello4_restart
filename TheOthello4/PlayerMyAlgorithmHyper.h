#pragma once
#include "BasePlayer.h"


//minmax���Ɋ�Â��Ĕz�u����B���݂����ő傸��荇�����Ƃ��A�Ō�Ɏ��g���ő�̂��̂�I��
class PlayerMyAlgorithmHyper :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerMyAlgorithmHyper(Field* field, eFieldColor myColor, bool saveF = true);
};
