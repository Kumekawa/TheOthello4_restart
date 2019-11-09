#pragma once
#include "BaseClass.h"
#include "FieldContent.h"
#include <vector>;

class Field :public BaseClass {
	fieldstone fieldStone;
	//���̔Ֆʂ̌����i�[���Ă���
	vector<fieldstone> nextStones;
	//�����܂ł̔Ֆʂ��L�����Ă���
	vector<fieldstone> history;
	eFieldColor turnPlayer;
	int endCounter;
	bool endF;
	int elapsedTurn;

	void DrawStone(int x, int y, fieldstone efc);

	//�w��_�ɒu���邩���ׁA�u����Ȃ�u���B�u������true��Ԃ�
	bool SetNextStonePoint(fieldstone* s, int x, int y);
	//2�A���u���Ȃ������ꍇ�Afalse��Ԃ�
	bool SetNextStone();

public:
	Field(eFieldColor turnPlayer);

	void Initialize()override;

	void Update()override;

	void Draw()override;

	//�Ăяo���ꂽ�Ƃ��A�u���ꂽ�ʒu�Ɏ��̔Ֆʌ�₪���邩���ׂ�
	//��������΂�����Ăяo���Ď��̃^�[����
	void SetStone(int x, int y);

	vector<fieldstone> GetNextStones();
	void SetFieldStone(fieldstone fieldStone);
	fieldstone GetFieldStone();
	int GetElapsedTurn();
	int GetEndCounter();
	bool GetEndF();
	vector<fieldstone> GetHistory();
	eFieldColor GetTurnPlayer();
};
