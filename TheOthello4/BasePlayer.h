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
	//盤面を保存していく
	vector<fieldstone> saveField;
	vector<vector<string>> saveData;

	//配置場所を決めるための関数。
	//fxとfyを書き換える。
	//実際に置くタイミングでは返り値をtrueにする。
	virtual bool SetPosition() = 0;
public:
	BasePlayer(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF);
	void Initialize() override;
	void Update()override;
	void Draw() override;
	bool GetEndF();
};
