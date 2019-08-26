#include "DxLib.h"
#include "MacroWindowSize.h"
#include "DxLibAlways.h"
#include "DxLibEndEffect.h"
#include "MacroColor.h"
#include "Players.h"
#include <vector>
using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(false);//ログを出力しない

	ChangeWindowMode(TRUE);
	//ジョイパッドがうまく起動しない場合は下の関数を使ってください。
	SetUseJoypadVibrationFlag(FALSE);

	SetWindowSizeChangeEnableFlag(FALSE, FALSE); //ユーザー側のウィンドウサイズ変更不可
	SetAlwaysRunFlag(TRUE); //最前面にない時も動作
	SetGraphMode(MWS_XMAX, MWS_YMAX, 32); //ウィンドウの最大サイズ指定
	SetWindowSize(MWS_XMAX, MWS_YMAX); //ウィンドウのサイズ指定

	//DxLibの初期化処理
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	//描画画面を裏に
	SetDrawScreen(DX_SCREEN_BACK);

	eFieldColor turnPlayer = eFC_Black;

	vector<BaseClass*> objects;
	Field field(&turnPlayer);
	objects.push_back(&field);

	bool saveF = false;

	BasePlayer* player1;
	player1 = SelectPlayer(eRoler, &field, &turnPlayer, eFC_Black, saveF);

	BasePlayer* player2;
	player2 = SelectPlayer(eDeep, &field, &turnPlayer, eFC_White, saveF);


	objects.push_back(player1);
	objects.push_back(player2);

	
	for (int i = 0; i < objects.size(); ++i) {
		objects[i]->Initialize();
	}

	int w = 0;
	int b = 0;
	int d = 0;

	bool t = true;

	while (ProcessMessage() == 0)
	{
		//if (CheckHitKey(KEY_INPUT_R) && player1.GetEndF() && player2.GetEndF()) {
		//if (CheckHitKey(KEY_INPUT_R) || field.GetEndF() >= 5) {
		//if (field.GetEndF()) {
		if (player1->GetEndF() && player2->GetEndF()) {

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

		DrawFormatString(MFS_WIDTH, MFS_HEIGHT / 2, MC_WHITE, "黒勝ち:%d\n白勝ち:%d\n引き分け:%d", b, w, d);
		if (DxLibEndEffect(true)) {
			break;
		}
		DxLibAlways(true);
	}
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

