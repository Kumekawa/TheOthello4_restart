#pragma once
#include "BaseClass.h"
#include "FieldContent.h"
#include <vector>;

class Field :public BaseClass {
	fieldstone fieldStone;
	//次の盤面の候補を格納しておく
	vector<fieldstone> nextStones;
	//ここまでの盤面を記憶しておく
	vector<fieldstone> history;
	eFieldColor turnPlayer;
	int endCounter;
	bool endF;
	int elapsedTurn;

	void DrawStone(int x, int y, fieldstone efc);

	//指定点に置けるか調べ、置けるなら置く。置けたらtrueを返す
	bool SetNextStonePoint(fieldstone* s, int x, int y);
	//2連続置けなかった場合、falseを返す
	bool SetNextStone();

public:
	Field(eFieldColor turnPlayer);

	void Initialize()override;

	void Update()override;

	void Draw()override;

	//呼び出されたとき、置かれた位置に次の盤面候補があるか調べる
	//もしあればそれを呼び出して次のターンへ
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
