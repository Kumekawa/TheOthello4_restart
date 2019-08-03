#include "DxLib.h"
#include "MacroWindowSize.h"
#include "DxLibAlways.h"
#include "DxLibEndEffect.h"
#include "Between.h"
#include "DrawLineStright.h"
#include <vector>
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
void ChangeFieldColor(eFieldColor *turn) {
	switch (*turn)
	{
	case eFC_Black:
		*turn = eFC_White;
		break;
	case eFC_White:
		*turn = eFC_Black;
		break;
	default:
		break;
	}
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
};

class Field :public BaseClass {
	fieldstone fieldStone;
	//���̔Ֆʂ̌����i�[���Ă���
	vector<fieldstone> nextStones;
	eFieldColor *turnPlayer;
	bool endF;
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
		endF = false;
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
		DrawFormatString(MFS_WIDTH, 0, MC_WHITE, "\nblack:%d\nwhite%d\nturn:%d\nPlayer:%s", b, w, elapsedTurn,*turnPlayer == eFC_Black?"black":"white");
		if (endF) {
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
			DrawFormatString(MFS_WIDTH, 0, MC_WHITE, (s).c_str());
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
					endF = true;
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
			endF = true;
		}
		fieldStone.SetAmount();
	}
	fieldstone GetFieldStone() {
		return fieldStone;
	}
	int GetElapsedTurn() {
		return elapsedTurn;
	}
};

class BasePlayer :public BaseClass {
protected:
	Field *field;
	eFieldColor *turnPlayer;
	eFieldColor myColor;
	int myDrawColor;
	int fx, fy;

	//�z�u�ꏊ�����߂邽�߂̊֐��B
	//fx��fy������������B
	//���ۂɒu���^�C�~���O�ł͕Ԃ�l��true�ɂ���B
	virtual bool SetPosition() = 0;
public:
	BasePlayer(Field *field,eFieldColor *turnPlayer,eFieldColor myColor) {
		this->field = field;
		this->turnPlayer = turnPlayer;
		this->myColor = myColor;
		Initialize();
	}
	void Initialize() override {
		fx = 0;
		fy = 0;
		if (myColor == eFC_Black) {
			myDrawColor = MC_BLACK;
		}
		else {
			myDrawColor = MC_WHITE;
		}
	}
	void Update()override {
		if (*turnPlayer == myColor) {
			if (SetPosition()) {
				field->SetStone(fx, fy);
			}
		}
	}
	void Draw() override {
		if (*turnPlayer == myColor) {
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
			return true;
		}
		return false;
	}
public:
	PlayerHuman(Field *field, eFieldColor *turnPlayer, eFieldColor myColor) :BasePlayer(field, turnPlayer, myColor) {
	
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
	PlayerRandom(Field *field, eFieldColor *turnPlayer, eFieldColor myColor) :BasePlayer(field, turnPlayer, myColor) {

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
	PlayerRoler(Field *field, eFieldColor *turnPlayer, eFieldColor myColor) :BasePlayer(field, turnPlayer, myColor) {
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
	PlayerNextMax(Field *field, eFieldColor *turnPlayer, eFieldColor myColor) :BasePlayer(field, turnPlayer, myColor) {

	}
};

//minmax���Ɋ�Â��Ĕz�u����B�Ƃ肠����2�i�K
class PlayerMinMax :public BasePlayer {
	bool SetPosition() override {
		//���z�������A�����̎�Ԃ��ő�ɂȂ�悤�ɑI��
		eFieldColor _turnPlayer = myColor;
		Field _field(&_turnPlayer);
		PlayerNextMax Player1(&_field, &_turnPlayer, myColor);
		PlayerNextMax Player2(&_field, &_turnPlayer, myColor);

		return false;
	}
public:
	PlayerMinMax(Field *field, eFieldColor *turnPlayer, eFieldColor myColor) :BasePlayer(field, turnPlayer, myColor) {
	
	}
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(false);//���O���o�͂��Ȃ�

	ChangeWindowMode(TRUE);
	//�W���C�p�b�h�����܂��N�����Ȃ��ꍇ�͉��̊֐����g���Ă��������B
	SetUseJoypadVibrationFlag(FALSE);

	SetWindowSizeChangeEnableFlag(FALSE, FALSE); //���[�U�[���̃E�B���h�E�T�C�Y�ύX�s��
	//SetAlwaysRunFlag(TRUE); //�őO�ʂɂȂ���������
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

	PlayerHuman player1(&field, &turnPlayer, eFC_Black);
	//PlayerRandom player1(&field, &turnPlayer, eFC_Black);
	//PlayerRoler player1(&field, &turnPlayer, eFC_Black);
	//PlayerNextMax player1(&field, &turnPlayer, eFC_Black);

	//PlayerHuman player2(&field, &turnPlayer, eFC_White);
	PlayerRandom player2(&field, &turnPlayer, eFC_White);
	//PlayerRoler player2(&field, &turnPlayer, eFC_White);
	//PlayerNextMax player2(&field, &turnPlayer, eFC_White);
	//PlayerMinMax player2(&field, &turnPlayer, eFC_White);

	
	objects.push_back(&player1);
	objects.push_back(&player2);

	

	for (int i = 0; i < objects.size(); ++i) {
		objects[i]->Initialize();
	}

	while (ProcessMessage() == 0)
	{
		if (CheckHitKey(KEY_INPUT_R)) {
			turnPlayer = eFC_Black;
			for (int i = 0; i < objects.size(); ++i) {
				objects[i]->Initialize();
			}
		}
		for (int i = 0; i < objects.size(); ++i) {
			objects[i]->Update();
			objects[i]->Draw();
		}
		if (DxLibEndEffect(true)) {
			break;
		}
		DxLibAlways(true);
	}
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}