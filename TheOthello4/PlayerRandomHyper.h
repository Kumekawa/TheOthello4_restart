#pragma once
#include "BasePlayer.h"

//�u����ꏊ�ɒu�����Ƃ��A���݂��������_���ɒu�������̂�100��J��Ԃ��A�����������Ƃ����I��
class PlayerRandomHyper :public BasePlayer {
	bool SetPosition() override;
public:
	PlayerRandomHyper(Field* field, eFieldColor myColor, bool saveF = true);
};
