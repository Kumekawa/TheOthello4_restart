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

class Field :public BaseClass {
	eFieldColor stone[MFS_XSIZE][MFS_YSIZE];
	eFieldColor *turnPlayer;
	void DrawStone(int x, int y,eFieldColor efc) {
		int i = x * MFS_UNIT + MFS_UNIT / 2;
		int j = y * MFS_UNIT + MFS_UNIT / 2;
		int c = MC_BLACK;
		if (stone[x][y] != eFC_None) {
			if (stone[x][y] == eFC_White) {
				c = MC_WHITE;
			}
			DrawCircle(i, j, MFS_UNIT / 3, c, 1);
		}
	}
public:
	void Initialize()override {
		//�Ֆʃ��Z�b�g
		for (int i = 0; i < MFS_XSIZE; ++i) {
			for (int j = 0; j < MFS_YSIZE; ++j) {
				stone[i][j] = eFC_None;
			}
		}
		int tx = MFS_XSIZE / 2;
		int ty = MFS_YSIZE / 2;
		stone[tx - 1][ty - 1] = eFC_Black;
		stone[tx][ty] = eFC_Black;
		stone[tx - 1][ty] = eFC_White;
		stone[tx][ty - 1] = eFC_White;
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
				DrawStone(i, j, stone[i][j]);
			}
		}
	};

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

	vector<BaseClass*> objects;
	Field field;
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