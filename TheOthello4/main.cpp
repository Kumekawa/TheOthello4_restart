#include "DxLib.h"
#include "MacroWindowSize.h"
#include "DxLibAlways.h"
#include "DxLibEndEffect.h"
#include "Between.h"
#include "DrawLineStright.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;

constexpr int  MFS_XSIZE = 8;
constexpr int  MFS_YSIZE = 8;
constexpr int  MFS_WIDTH = MWS_YMAX;
constexpr int  MFS_HEIGHT= MWS_YMAX;
constexpr int MFS_UNIT = MFS_WIDTH / MFS_XSIZE;

enum eFieldColor {
	eFC_Black,
	eFC_White,
	eFC_None
};
eFieldColor GetChangeFieldColor(eFieldColor turn) {
	switch (turn)
	{
	case eFC_Black:
		turn = eFC_White;
		break;
	case eFC_White:
		turn = eFC_Black;
		break;
	default:
		break;
	}
	return turn;
}

void ChangeFieldColor(eFieldColor *turn) {
	*turn = GetChangeFieldColor(*turn);
}

class BaseClass {
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

struct fieldstone {
	//�ǂ��ɒu���Ă��̔ՖʂɂȂ�����
	int x, y;
	//���ۂ̔Ֆ�
	eFieldColor stone[MFS_XSIZE][MFS_YSIZE];
	//���ۂɂ��������邩
	int amount[3];
	//�ǂꂪ��ԑ�����
	eFieldColor GetMaxColor() {
		int t = amount[eFC_Black] - amount[eFC_White];
		if (t > 0) {
			return eFC_Black;
		}
		else if (t < 0) {
			return eFC_White;
		}
		else {
			return eFC_None;
		}
	}

	void SetAmount() {
		for (int i = 0; i < 3; ++i) {
			amount[i] = 0;
		}
		for (int i = 0; i < MFS_XSIZE; ++i) {
			for (int j = 0; j < MFS_YSIZE; ++j) {
				amount[stone[i][j]]++;
			}
		}
	}
	//�Ֆʂ𕶎���ŕ\���A�Ō��x,y��t�^����
	string GetString() {
		stringstream ss;
		for (int i = 0; i < MFS_XSIZE; ++i) {
			for (int j = 0; j < MFS_YSIZE; ++j) {
				ss << stone[i][j];
			}
		}
		ss << "," << x << "," << y;
		return ss.str();
	}
};

class Field :public BaseClass {
	fieldstone fieldStone;
	//���̔Ֆʂ̌����i�[���Ă���
	vector<fieldstone> nextStones;
	eFieldColor *turnPlayer;
	int endF;
	int elapsedTurn;

	void DrawStone(int x, int y,fieldstone efc) {
		int i = x * MFS_UNIT + MFS_UNIT / 2;
		int j = y * MFS_UNIT + MFS_UNIT / 2;
		int c = MC_BLACK;
		if (efc.stone[x][y] != eFC_None) {
			if (efc.stone[x][y] == eFC_White) {
				c = MC_WHITE;
			}
			DrawCircle(i, j, MFS_UNIT / 3, c, 1);
		}
	}

	//�w��_�ɒu���邩���ׁA�u����Ȃ�u���B�u������true��Ԃ�
	bool SetNextStonePoint(fieldstone *s,int x,int y) {
		//�w��_�ɐ΂�����Βu���Ȃ��̂ŏI��
		if (s->stone[x][y] != eFC_None) {
			return false;
		}
		bool putF = false;
		for (int i = -1; i < 2; ++i) {
			for (int j = -1; j < 2; ++j) {
				if (i == 0 && j == 0) {
					continue;
				}
				for (int k = 1;; k++) {
					int m = x + i * k;
					int n = y + j * k;
					//��ʊO�ɂ������肵����I��
					if (CheckBetween(0, m, MFS_XSIZE) != 0) {
						break;
					}
					if (CheckBetween(0, n, MFS_YSIZE) != 0) {
						break;
					}

					//�΂�������ΏI��
					if (s->stone[m][n] == eFC_None) {
						break;
					}
					//�^�[���v���C���[�̐΂���������A�����܂łɔ�^�[���v���C���[�̐΂�����ΐF��ς��ďI��
					if (s->stone[m][n] == *turnPlayer) {
						if (k > 1) {
							for (;k >= 0;--k) {
								int a = x + i * k;
								int b = y + j * k;
								s->stone[a][b] = *turnPlayer;
							}
							putF = true;
						}
						break;
					}
				}
			}
		}
		return putF;
	}
	//2�A���u���Ȃ������ꍇ�Afalse��Ԃ�
	bool SetNextStone() {
		bool putF = false;
		//2��J��Ԃ�����u����Ƃ��낪�����̂ŃQ�[���I��
		nextStones.clear();
		int n = 0;
		do {
			if (n > 0) {
				ChangeFieldColor(turnPlayer);
			}
			for (int i = 0; i < MFS_XSIZE; ++i) {
				for (int j = 0; j < MFS_YSIZE; ++j) {
					fieldstone t = fieldStone;
					if (SetNextStonePoint(&t, i, j)) {
						t.x = i;
						t.y = j;
						nextStones.push_back(t);
						putF = true;
					}
				}
			}
			++n;
		} while (!putF && n <= 2);
		if (n > 2) {
			return false;
		}
		return true;
	}
public:
	Field(eFieldColor *turnPlayer) {
		this->turnPlayer = turnPlayer;
	}
	void Initialize()override {
		//�Ֆʃ��Z�b�g
		for (int i = 0; i < MFS_XSIZE; ++i) {
			for (int j = 0; j < MFS_YSIZE; ++j) {
				fieldStone.stone[i][j] = eFC_None;
			}
		}
		int tx = MFS_XSIZE / 2;
		int ty = MFS_YSIZE / 2;
		fieldStone.stone[tx - 1][ty - 1] = eFC_Black;
		fieldStone.stone[tx][ty] = eFC_Black;
		fieldStone.stone[tx - 1][ty] = eFC_White;
		fieldStone.stone[tx][ty - 1] = eFC_White;
		SetNextStone();
		endF = 0;
		elapsedTurn = 0;
	}
	void Update()override {
		
	}
	void Draw()override {
		DrawBox(0, 0, MFS_WIDTH, MFS_HEIGHT, MC_GREEN, 1);
		for (int i = 0; i <= MFS_XSIZE; ++i) {
			DrawLineVertical(i * MFS_UNIT, MC_BLACK);
		}
		for (int j = 0; j <= MFS_YSIZE; ++j) {
			DrawLineHorizontal(j * MFS_UNIT, MC_BLACK);
		}
		for (int i = 0; i < MFS_XSIZE; ++i) {
			for (int j = 0; j < MFS_YSIZE; ++j) {
				DrawStone(i, j, fieldStone);
			}
		}

		int b = fieldStone.amount[eFC_Black];
		int w = fieldStone.amount[eFC_White];
		DrawFormatString(MFS_WIDTH, 0, MC_WHITE, "\n\nblack:%d\nwhite%d\nturn:%d\nPlayer:%s", b, w, elapsedTurn,*turnPlayer == eFC_Black?"black":"white");
		if (endF > 0) {
			int t = b - w;
			string s;
			if (t > 0) {
				s = "���̏����I";
			}
			else if(t < 0){
				s = "���̏����I";
			}
			else {
				s = "��������";
			}
			if (endF < 5) {
				++endF;
			}
			else {
				DrawFormatString(MFS_WIDTH, 0, MC_WHITE, "Press R To Restart");
			}
			DrawFormatString(MFS_WIDTH, 0, MC_WHITE, ("\n" + s).c_str());
		}
	}

	//�Ăяo���ꂽ�Ƃ��A�u���ꂽ�ʒu�Ɏ��̔Ֆʌ�₪���邩���ׂ�
	//��������΂�����Ăяo���Ď��̃^�[����
	void SetStone(int x,int y) {
		for (int i = 0; i < nextStones.size(); ++i) {
			fieldstone f = nextStones[i];
			if (f.x == x && f.y == y) {
				//�����ɂ��Ă���Ύ��̔Ֆʂ�����
				fieldStone = f; 
				ChangeFieldColor(turnPlayer);
				if (SetNextStone() == false) {
					endF++;
				}
				fieldStone.SetAmount();
				++elapsedTurn;
				break;
			}
		}
	}

	vector<fieldstone> GetNextStones() {
		return nextStones;
	}
	void SetFieldStone(fieldstone fieldStone) {
		this->fieldStone = fieldStone;
		//�^�[���v���C���[�͏��������Ȃ�
		if (SetNextStone() == false) {
			endF++;
		}
		fieldStone.SetAmount();
	}
	fieldstone GetFieldStone() {
		return fieldStone;
	}
	int GetElapsedTurn() {
		return elapsedTurn;
	}
	int GetEndF() {
		return endF;
	}
};

class BasePlayer :public BaseClass {
protected:
	Field *field;
	eFieldColor *turnPlayer;
	eFieldColor myColor;
	int myDrawColor;
	int fx, fy;


	FILE *fp;
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
	BasePlayer(Field *field, eFieldColor *turnPlayer, eFieldColor myColor, bool saveF) {
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
	void Initialize() override {
		fx = 0;
		fy = 0;
		saveField.clear();
	}
	void Update()override {
		if (*turnPlayer == myColor && field->GetNextStones().size() > 0) {
			if (SetPosition()) {
				field->SetStone(fx, fy);
				if (saveF) {
					saveField.push_back(field->GetFieldStone());
				}
			}
		}
		
		if (saveF) {
			if (field->GetEndF() == 3) {
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

							//��v������̂�����Ό��ʂɉ����ĉ��Z����
							saveData[i][r] = to_string(stoi(saveData[i][r]) + 1);
							f = false;
						}
					}
					//��v������̂����������̂�saveData�ɒǉ�����
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
	void Draw() override {
		if (*turnPlayer == myColor && field->GetNextStones().size() > 0) {
			int tx = fx * MFS_UNIT + MFS_UNIT / 2;
			int ty = fy * MFS_UNIT + MFS_UNIT / 2;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawCircle(tx, ty, MFS_UNIT / 4, myDrawColor, 1);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 128);
			DrawCircle(tx, ty, MFS_UNIT / 4, myDrawColor, 0);
		}
	}
	
};

//�}�E�X����
class PlayerHuman :public BasePlayer {
	bool SetPosition() override {
		int mx, my;
		GetMousePoint(&mx, &my);
		fx = mx / MFS_UNIT;
		fy = my / MFS_UNIT;
		SetBetween(0, &fx, MFS_XSIZE);
		SetBetween(0, &fy, MFS_YSIZE);
		if (GetMouseInput() & MOUSE_INPUT_LEFT) {
			auto tf = field->GetNextStones();
			for (int i = 0; i < tf.size(); ++i) {
				if (tf[i].x == fx && tf[i].y == fy) {
					return true;
				}
			}
		}
		return false;
	}
public:
	PlayerHuman(Field *field, eFieldColor *turnPlayer, eFieldColor myColor, bool saveF = true) :BasePlayer(field, turnPlayer, myColor, saveF) {
	
	}
};

//���S�����_��
class PlayerRandom :public BasePlayer {
	vector<fieldstone> nextStones;
	bool SetPosition() override {
		nextStones = field->GetNextStones();
		if (nextStones.size() > 0) {
			int t = GetRand(nextStones.size() - 1);
			fx = nextStones[t].x;
			fy = nextStones[t].y;
			return true;
		}
		return false;
	}
public:
	PlayerRandom(Field *field, eFieldColor *turnPlayer, eFieldColor myColor, bool saveF = true) :BasePlayer(field, turnPlayer, myColor, saveF) {

	}
};

//���ォ�烍�[���[���
class PlayerRoler :public BasePlayer {
	int t;
	bool SetPosition() override {
		t = (t + 1) % (MFS_XSIZE * MFS_YSIZE);
		fx = t % MFS_XSIZE;
		fy = t / MFS_YSIZE;
		return true;
	}
public:
	PlayerRoler(Field *field, eFieldColor *turnPlayer, eFieldColor myColor, bool saveF = true) :BasePlayer(field, turnPlayer, myColor, saveF) {
		t = 0;
	}
};

//�����ő�̂��̂�T��
class PlayerNextMax :public BasePlayer {
	vector<fieldstone> nextStones;
	bool SetPosition() override {
		nextStones = field->GetNextStones();
		if (nextStones.size() > 0) {
			int m = -1;
			for (int i = 0; i < nextStones.size(); ++i) {
				fieldstone t = nextStones[i];
				if (m < t.amount[myColor]) {
					m = t.amount[myColor];
					fx = t.x;
					fy = t.y;
				}
			}
			return true;
		}
		return false;
	}
public:
	PlayerNextMax(Field *field, eFieldColor *turnPlayer, eFieldColor myColor, bool saveF = true) :BasePlayer(field, turnPlayer, myColor, saveF) {

	}
};

//minmax���Ɋ�Â��Ĕz�u����B�Ƃ肠����2�i�K
class PlayerMinMax :public BasePlayer {
	bool SetPosition() override {
		//���z�������A�����̎�Ԃ��ő�ɂȂ�悤�ɑI��
		eFieldColor _turnPlayer = myColor;
		Field _field(&_turnPlayer);
		int max = -1;

		_field.SetFieldStone(field->GetFieldStone());
		for (int i = 0; i < _field.GetNextStones().size(); ++i) {
			auto ft = _field;
			_turnPlayer = myColor;

			int tx = ft.GetNextStones()[i].x;
			int ty = ft.GetNextStones()[i].y;
			ft.SetStone(tx, ty);
			ft.Update();
			PlayerNextMax Player2(&ft, &_turnPlayer, GetChangeFieldColor(myColor),false);
			Player2.Update();

			PlayerNextMax Player1(&ft, &_turnPlayer, myColor, false);
			Player1.Update();
			if (max < ft.GetFieldStone().amount[myColor]) {
				max = ft.GetFieldStone().amount[myColor];
				fx = tx;
				fy = ty;
			}
		}
		return true;
	}
public:
	PlayerMinMax(Field *field, eFieldColor *turnPlayer, eFieldColor myColor, bool saveF = true) :BasePlayer(field, turnPlayer, myColor, saveF) {
	
	}
};

class PlayerMyAlgorithm :public BasePlayer {
	bool SetPosition() override {
		//�����Ȃ�̃A���S���Y��������Ă݂�
		auto fs = field->GetNextStones();
		//5%�̊m���Ń����_��
		if (GetRand(99) < 5) {
			auto r = GetRand(fs.size() - 1);
			fx = fs[r].x;
			fy = fs[r].y;
			return true;
		}

		//�l��������Ύ��
		for (int i = 0; i < fs.size(); ++i) {
			int tx = fs[i].x;
			int ty = fs[i].y;
			for (int a = 0; a < MFS_XSIZE; a += MFS_XSIZE - 1) {
				for (int b = 0; b < MFS_YSIZE; b += MFS_YSIZE - 1) {
					if (tx == a && ty == b) {
						fx = tx;
						fy = ty;
						return true;
					}
				}
			}
		}

		//���̔Ֆʎ��A�����̂��̂��ő�ɂȂ�悤�ɑI��
		int max = -1;
		for (int i = 0; i < fs.size(); ++i) {
			if (max < fs[i].amount[myColor]) {
				fx = fs[i].x;
				fy = fs[i].y;
			}
		}
		return true;
	}
public:
	PlayerMyAlgorithm(Field *field, eFieldColor *turnPlayer, eFieldColor myColor, bool saveF = true) :BasePlayer(field, turnPlayer, myColor,saveF) {

	}
};

//minmax���Ɋ�Â��Ĕz�u����B���݂����ő傸��荇�����Ƃ��A�Ō�Ɏ��g���ő�̂��̂�I��
class PlayerMinMaxHyper :public BasePlayer {
	bool SetPosition() override {
		//���z�������A�����̎�Ԃ��ő�ɂȂ�悤�ɑI��
		eFieldColor _turnPlayer = myColor;
		Field _field(&_turnPlayer);
		int max = -1;

		_field.SetFieldStone(field->GetFieldStone());
		for (int i = 0; i < _field.GetNextStones().size(); ++i) {
			auto ft = _field;
			_turnPlayer = myColor;

			int tx = ft.GetNextStones()[i].x;
			int ty = ft.GetNextStones()[i].y;
			//�����ŏ���̐Δz�u
			ft.SetStone(tx, ty);
			PlayerNextMax Player2(&ft, &_turnPlayer, GetChangeFieldColor(myColor), false);
			PlayerNextMax Player1(&ft, &_turnPlayer, myColor, false);

			//�I������܂ŉ��荇��
			while (ft.GetEndF() == 0) {
				ft.Update();
				Player2.Update();
				Player1.Update();
			}
			if (max < ft.GetFieldStone().amount[myColor]) {
				max = ft.GetFieldStone().amount[myColor];
				fx = tx;
				fy = ty;
			}
		}
		return true;
	}
public:
	PlayerMinMaxHyper(Field *field, eFieldColor *turnPlayer, eFieldColor myColor, bool saveF = true) :BasePlayer(field, turnPlayer, myColor, saveF) {

	}
};

//�u����ꏊ�ɒu�����Ƃ��A���݂��������_���ɒu�������̂�100��J��Ԃ��A�����������Ƃ����I��
class PlayerRandomHyper :public BasePlayer {
	bool SetPosition() override {
		//���z�������A�����̎�Ԃ��ő�ɂȂ�悤�ɑI��
		eFieldColor _turnPlayer = myColor;
		Field _field(&_turnPlayer);
		int max = -1;

		_field.SetFieldStone(field->GetFieldStone());
		for (int i = 0; i < _field.GetNextStones().size(); ++i) {
			//�I������܂ŉ��荇��
			int t = 0;
			auto ft = _field;
			_turnPlayer = myColor;

			int tx = ft.GetNextStones()[i].x;
			int ty = ft.GetNextStones()[i].y;

			for (int i = 0; i < 100; ++i) {
				ft = _field;
				_turnPlayer = myColor;

				//�����ŏ���̐Δz�u
				ft.SetStone(tx, ty);
				PlayerRandom Player2(&ft, &_turnPlayer, GetChangeFieldColor(myColor), false);
				PlayerRandom Player1(&ft, &_turnPlayer, myColor, false);

				while (ft.GetEndF() == 0) {
					ft.Update();
					Player2.Update();
					Player1.Update();
				}

			}
			if (max < t) {
				max = t;
				fx = tx;
				fy = ty;
			}
		}
		return true;
	}
public:
	PlayerRandomHyper(Field *field, eFieldColor *turnPlayer, eFieldColor myColor, bool saveF = true) :BasePlayer(field, turnPlayer, myColor, saveF) {

	}
};

//minmax���Ɋ�Â��Ĕz�u����B���݂����ő傸��荇�����Ƃ��A�Ō�Ɏ��g���ő�̂��̂�I��
class PlayerMyAlgorithmHyper :public BasePlayer {
	bool SetPosition() override {
		//�����Ȃ�̃A���S���Y��������Ă݂�
		auto fs = field->GetNextStones();
		////5%�̊m���Ń����_��
		//if (GetRand(99) < 5) {
		//	auto r = GetRand(fs.size() - 1);
		//	fx = fs[r].x;
		//	fy = fs[r].y;
		//	return true;
		//}

		//�l��������Ύ��
		for (int i = 0; i < fs.size(); ++i) {
			int tx = fs[i].x;
			int ty = fs[i].y;
			for (int a = 0; a < MFS_XSIZE; a += MFS_XSIZE - 1) {
				for (int b = 0; b < MFS_YSIZE; b += MFS_YSIZE - 1) {
					if (tx == a && ty == b) {
						fx = tx;
						fy = ty;
						return true;
					}
				}
			}
		}

		//���z�������A�����̎�Ԃ��ő�ɂȂ�悤�ɑI��
		eFieldColor _turnPlayer = myColor;
		Field _field(&_turnPlayer);
		double max = -1;

		_field.SetFieldStone(field->GetFieldStone());
		for (int i = 0; i < _field.GetNextStones().size(); ++i) {
			auto ft = _field;
			_turnPlayer = myColor;

			int tx = ft.GetNextStones()[i].x;
			int ty = ft.GetNextStones()[i].y;
			//�����ŏ���̐Δz�u
			ft.SetStone(tx, ty);
			PlayerNextMax Player2(&ft, &_turnPlayer, GetChangeFieldColor(myColor), false);
			PlayerNextMax Player1(&ft, &_turnPlayer, myColor, false);

			//�I������܂ŉ��荇��
			while (ft.GetEndF() == 0) {
				ft.Update();
				Player2.Update();
				Player1.Update();
			}

			//����tx�܂���ty���[�Ȃ�{�[�i�X��t����
			double bonus = 1.0;
			if (tx == 0 || ty == 0 || tx == MFS_XSIZE - 1 || ty == MFS_YSIZE - 1) {
				bonus = 1.25;
			}

			//�����l���ׂ̗��Ƃ�Ȃ���΂Ȃ�Ȃ��ꍇ�A���_��������
			double minus = 1.0;
			int x_s[] = {
				1,MFS_XSIZE - 2,
				0,1,MFS_XSIZE - 2, MFS_XSIZE - 1,
				0,1,MFS_XSIZE - 2, MFS_XSIZE - 1,
				1,MFS_XSIZE - 2,
			};
			int y_s[] = {
				0,0,
				1,1,1,1,
				MFS_YSIZE - 2,MFS_YSIZE - 2,MFS_YSIZE - 2,MFS_YSIZE - 2,
				MFS_YSIZE - 1,MFS_YSIZE - 1,
			};
			for (int i = 0; i < 12; ++i) {
				if (tx == x_s[i] && ty == y_s[i]) {
					minus = 0.5;
				}
			}

			if (max < ft.GetFieldStone().amount[myColor] * bonus * minus) {
				max = ft.GetFieldStone().amount[myColor] * bonus * minus;
				fx = tx;
				fy = ty;
			}
		}
		return true;
	}
public:
	PlayerMyAlgorithmHyper(Field *field, eFieldColor *turnPlayer, eFieldColor myColor, bool saveF = true) :BasePlayer(field, turnPlayer, myColor, saveF) {

	}
};


//���ɑ��肪����萔���ŏ��ɂȂ�悤�ɑI��
class PlayerNextPointMin :public BasePlayer {
	bool SetPosition() override {
		//���z�������A����̎萔���ŏ��ɂȂ�悤�ɑI��
		eFieldColor _turnPlayer = myColor;
		Field _field(&_turnPlayer);
		int min = -1;

		_field.SetFieldStone(field->GetFieldStone());
		for (int i = 0; i < _field.GetNextStones().size(); ++i) {
			auto ft = _field;
			_turnPlayer = myColor;

			int tx = ft.GetNextStones()[i].x;
			int ty = ft.GetNextStones()[i].y;
			//�����ŏ���̐Δz�u
			ft.SetStone(tx, ty);
	
			ft.Update();
	
			if (min == -1 || min > ft.GetNextStones().size()){
				min = ft.GetNextStones().size();
				fx = tx;
				fy = ty;
			}
		}
		return true;
	}
public:
	PlayerNextPointMin(Field *field, eFieldColor *turnPlayer, eFieldColor myColor, bool saveF = true) :BasePlayer(field, turnPlayer, myColor, saveF) {

	}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(false);//���O���o�͂��Ȃ�

	ChangeWindowMode(TRUE);
	//�W���C�p�b�h�����܂��N�����Ȃ��ꍇ�͉��̊֐����g���Ă��������B
	SetUseJoypadVibrationFlag(FALSE);

	SetWindowSizeChangeEnableFlag(FALSE, FALSE); //���[�U�[���̃E�B���h�E�T�C�Y�ύX�s��
	SetAlwaysRunFlag(TRUE); //�őO�ʂɂȂ���������
	SetGraphMode(MWS_XMAX, MWS_YMAX, 32); //�E�B���h�E�̍ő�T�C�Y�w��
	SetWindowSize(MWS_XMAX, MWS_YMAX); //�E�B���h�E�̃T�C�Y�w��

	//DxLib�̏���������
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	//�`���ʂ𗠂�
	SetDrawScreen(DX_SCREEN_BACK);

	eFieldColor turnPlayer = eFC_Black;

	vector<BaseClass*> objects;
	Field field(&turnPlayer);
	objects.push_back(&field);

	bool saveF = false;

	PlayerHuman player1(&field, &turnPlayer, eFC_Black, saveF);
	//PlayerRandom player1(&field, &turnPlayer, eFC_Black, saveF);
	//PlayerRoler player1(&field, &turnPlayer, eFC_Black, saveF);
	//PlayerNextMax player1(&field, &turnPlayer, eFC_Black, saveF);
	//PlayerMinMax player1(&field, &turnPlayer, eFC_Black, saveF);
	//PlayerMyAlgorithm player1(&field, &turnPlayer, eFC_Black, saveF);
	//PlayerMinMaxHyper player1(&field, &turnPlayer, eFC_Black, saveF);

	//PlayerHuman player2(&field, &turnPlayer, eFC_White, saveF);
	//PlayerRandom player2(&field, &turnPlayer, eFC_White, saveF);
	//PlayerRoler player2(&field, &turnPlayer, eFC_White, saveF);
	//PlayerNextMax player2(&field, &turnPlayer, eFC_White, saveF);
	//PlayerMinMax player2(&field, &turnPlayer, eFC_White, saveF);
	//PlayerMyAlgorithm player2(&field, &turnPlayer, eFC_White, saveF);
	//PlayerMinMaxHyper player2(&field, &turnPlayer, eFC_White, saveF);
	//PlayerRandomHyper player2(&field, &turnPlayer, eFC_White, saveF);
	PlayerMyAlgorithmHyper player2(&field, &turnPlayer, eFC_White, saveF);
	//PlayerNextPointMin player2(&field, &turnPlayer, eFC_White, saveF);
	
	objects.push_back(&player1);
	objects.push_back(&player2);

	

	for (int i = 0; i < objects.size(); ++i) {
		objects[i]->Initialize();
	}

	int w = 0;
	int b = 0;
	int d = 0;

	while (ProcessMessage() == 0)
	{
		if (CheckHitKey(KEY_INPUT_R)) {
		//if (CheckHitKey(KEY_INPUT_R) || field.GetEndF() >= 5) {

			switch (field.GetFieldStone().GetMaxColor())
			{
			case eFC_Black:
				b++;
				break;
			case eFC_White:
				w++;
				break;
			case eFC_None:
				d++;
				break;
			default:
				break;
			}

			turnPlayer = eFC_Black;
			for (int i = 0; i < objects.size(); ++i) {
				objects[i]->Initialize();
			}
		}
		for (int i = 0; i < objects.size(); ++i) {
			objects[i]->Update();
			objects[i]->Draw();
		}

		DrawFormatString(MFS_WIDTH, MFS_HEIGHT / 2, MC_WHITE, "������:%d\n������:%d\n��������:%d", b, w, d);
		if (DxLibEndEffect(true)) {
			break;
		}
		DxLibAlways(true);
	}
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}