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

class BaseClass {
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

struct fieldstone {
	//�ǂ��ɒu���Ă��̔ՖʂɂȂ�����
	int x, y;
	eFieldColor stone[MFS_XSIZE][MFS_YSIZE];
};

class Field :public BaseClass {
	fieldstone fieldStone;
	//���̔Ֆʂ̌����i�[���Ă���
	vector<fieldstone> nextStones;

	eFieldColor *turnPlayer;
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
	void SetNextStone() {
		bool putF = false;
		//2��J��Ԃ�����u����Ƃ��낪�����̂ŃQ�[���I��
		nextStones.clear();
		int n = 0;
		do {
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
		} while (!putF && n < 2);

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
	};
	void Update()override {};
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
	};

	//�Ăяo���ꂽ�Ƃ��A�u���ꂽ�ʒu�Ɏ��̔Ֆʌ�₪���邩���ׂ�
	//��������΂�����Ăяo���Ď��̃^�[����
	void SetStone(int x,int y) {
		
	}
};

class BasePlayer :public BaseClass {
	Field *_field;
	eFieldColor *turnPlayer;
public:
	BasePlayer(Field *field) {
		_field = field;
	}
	void Initialize()override {
	
	};
	void Update()override {};
	void Draw()override {};
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

	for (int i = 0; i < objects.size(); ++i) {
		objects[i]->Initialize();
	}

	while (ProcessMessage() == 0)
	{
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