#include "DxLib.h"
#include "MacroWindowSize.h"
#include "DxLibAlways.h"
#include "DxLibEndEffect.h"
#include "Between.h"

#define MFS_XSIZE 8
#define MFS_YSIZE 8


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
public:
	void Initialize()override {
		//�Ֆʃ��Z�b�g
		for (int i = 0; i < MFS_XSIZE; ++i) {
			for (int j = 0; j < MFS_YSIZE; ++j) {
				stone[i][j] = eFC_None;
			}
		}
		stone[MFS_XSIZE - 1][MFS_XSIZE - 1] = eFC_Black;
		stone[MFS_XSIZE][MFS_XSIZE] = eFC_Black;
		stone[MFS_XSIZE - 1][MFS_XSIZE] = eFC_White;
		stone[MFS_XSIZE][MFS_XSIZE - 1] = eFC_White;
	};
	void Update()override {};
	void Draw()override {};

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

	while (ProcessMessage() == 0)
	{

		if (DxLibEndEffect(true)) {
			break;
		}
		DxLibAlways(true);
	}
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}