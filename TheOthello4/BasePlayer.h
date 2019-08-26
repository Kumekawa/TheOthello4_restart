#pragma once
#include "BaseClass.h"
#include "Field.h"
#include <vector>

class BasePlayer :public BaseClass {
protected:
	Field* field;
	eFieldColor* turnPlayer;
	eFieldColor myColor;
	int myDrawColor;
	int fx, fy;

	bool startF;
	bool endF;

	FILE* fp;
	string fname;
	bool saveF;
	//�Ֆʂ�ۑ����Ă���
	vector<fieldstone> saveField;
	vector<vector<string>> saveData;

	//�z�u�ꏊ�����߂邽�߂̊֐��B
	//fx��fy������������B
	//���ۂɒu���^�C�~���O�ł͕Ԃ�l��true�ɂ���B
	virtual bool SetPosition() = 0;
public:
	BasePlayer(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF);
	void Initialize() override;
	void Update()override;
	void Draw() override;
	bool GetEndF();
};
