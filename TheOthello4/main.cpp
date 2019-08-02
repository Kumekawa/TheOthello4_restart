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
		//盤面リセット
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
	SetOutApplicationLogValidFlag(false);//ログを出力しない

	ChangeWindowMode(TRUE);
	//ジョイパッドがうまく起動しない場合は下の関数を使ってください。
	SetUseJoypadVibrationFlag(FALSE);

	SetWindowSizeChangeEnableFlag(FALSE, FALSE); //ユーザー側のウィンドウサイズ変更不可
	//SetAlwaysRunFlag(TRUE); //最前面にない時も動作
	SetGraphMode(MWS_XMAX, MWS_YMAX, 32); //ウィンドウの最大サイズ指定
	SetWindowSize(MWS_XMAX, MWS_YMAX); //ウィンドウのサイズ指定

	//DxLibの初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	//描画画面を裏に
	SetDrawScreen(DX_SCREEN_BACK);

	while (ProcessMessage() == 0)
	{

		if (DxLibEndEffect(true)) {
			break;
		}
		DxLibAlways(true);
	}
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}