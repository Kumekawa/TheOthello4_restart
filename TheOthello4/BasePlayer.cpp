#include "BasePlayer.h"
#include "MacroColor.h"
#include <fstream>

BasePlayer::BasePlayer(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF) {
	this->field = field;
	this->turnPlayer = turnPlayer;
	this->myColor = myColor;
	if (myColor == eFC_Black) {
		myDrawColor = MC_BLACK;
	}
	else {
		myDrawColor = MC_WHITE;
	}
	this->saveF = saveF;
	Initialize();

	if (myColor == eFC_Black) {
		fname = "black.txt";
	}
	else {
		fname = "white.txt";
	}

	if (saveF) {
		ifstream ifs(fname);
		if (!ifs.fail()) {
			string str;
			while (getline(ifs, str))
			{
				stringstream ss{ str };
				string buf;
				vector<string> v;
				while (getline(ss, buf, '/')) {
					v.push_back(buf);
				}
				saveData.push_back(v);
			}
			ifs.close();
		}
	}
}

void BasePlayer::Initialize(){
	fx = 0;
	fy = 0;
	saveField.clear();
	startF = true;
	endF = false;
}
void BasePlayer::Update(){
	if (field->GetEndF()) {
		endF = true;
	}
	if (SetPosition()) {
		if (*turnPlayer == myColor) {
			field->SetStone(fx, fy);
			if (saveF) {
				saveField.push_back(field->GetFieldStone());
			}
		}
	}

	if (saveF) {
		if (field->GetEndCounter() == 3) {
			int r = 1;
			auto fs = field->GetFieldStone().amount;
			int me = fs[myColor];
			int you = fs[GetChangeFieldColor(myColor)];
			if (me > you) {
				r = 1;
			}
			else if (me < you) {
				r = 2;
			}
			else {
				r = 3;
			}

			for (int j = 0; j < saveField.size(); ++j) {
				bool f = true;
				for (int i = 0; i < saveData.size(); ++i) {
					if (saveData[i][0] == saveField[j].GetString()) {

						//ˆê’v‚·‚é‚à‚Ì‚ª‚ ‚ê‚ÎŒ‹‰Ê‚É‰ž‚¶‚Ä‰ÁŽZ‚·‚é
						saveData[i][r] = to_string(stoi(saveData[i][r]) + 1);
						f = false;
					}
				}
				//ˆê’v‚·‚é‚à‚Ì‚ª–³‚©‚Á‚½‚Ì‚ÅsaveData‚É’Ç‰Á‚·‚é
				if (f) {
					vector<string> ts = { saveField[j].GetString() };
					for (int n = 0; n < 3; ++n) {
						if (n + 1 == r) {
							ts.push_back("1");
						}
						else {
							ts.push_back("0");
						}
					}
					saveData.push_back(ts);
				}
			}

			ofstream ofs(fname);
			for (int i = 0; i < saveData.size(); ++i) {
				ofs << saveData[i][0];
				for (int j = 1; j < 4; ++j) {
					ofs << '/' << saveData[i][j];
				}
				ofs << endl;
			}
			ofs.close();
		}
	}
}
void BasePlayer::Draw(){
	if (*turnPlayer == myColor && field->GetNextStones().size() > 0) {
		int tx = fx * MFS_UNIT + MFS_UNIT / 2;
		int ty = fy * MFS_UNIT + MFS_UNIT / 2;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DrawCircle(tx, ty, MFS_UNIT / 4, myDrawColor, 1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
		DrawCircle(tx, ty, MFS_UNIT / 4, myDrawColor, 0);
	}
}
bool BasePlayer::GetEndF() {
	return endF;
}
