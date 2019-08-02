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
	//どこに置いてこの盤面になったか
	int x, y;
	eFieldColor stone[MFS_XSIZE][MFS_YSIZE];
};

class Field :public BaseClass {
	fieldstone fieldStone;
	//次の盤面の候補を格納しておく
	vector<fieldstone> nextStones;
	eFieldColor *turnPlayer;
	bool endF;

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

	//指定点に置けるか調べ、置けるなら置く。置けたらtrueを返す
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
					//画面外にあったりしたら終了
					if (CheckBetween(0, m, MFS_XSIZE) != 0) {
						break;
					}
					if (CheckBetween(0, n, MFS_YSIZE) != 0) {
						break;
					}

					//石が無ければ終了
					if (s->stone[m][n] == eFC_None) {
						break;
					}
					//ターンプレイヤーの石があったら、そこまでに非ターンプレイヤーの石があれば色を変えて終了
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
	//2連続置けなかった場合、falseを返す
	bool SetNextStone() {
		bool putF = false;
		//2回繰り返したら置けるところが無いのでゲーム終了
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
		if (n >= 2) {
			return false;
		}
		return true;
	}
public:
	Field(eFieldColor *turnPlayer) {
		this->turnPlayer = turnPlayer;
	}
	void Initialize()override {
		//盤面リセット
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
	};
	void Update()override {
		
	};
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

	//呼び出されたとき、置かれた位置に次の盤面候補があるか調べる
	//もしあればそれを呼び出して次のターンへ
	void SetStone(int x,int y) {
		for (int i = 0; i < nextStones.size(); ++i) {
			fieldstone f = nextStones[i];
			if (f.x == x && f.y == y) {
				//次の盤面がある
				fieldStone = f;
				ChangeFieldColor(turnPlayer);
				if (SetNextStone() == false) {
					endF = true;
				}
			}
		}
	}
};

class BasePlayer :public BaseClass {
protected:
	Field *field;
	eFieldColor *turnPlayer;
	eFieldColor myColor;
	int fx, fy;

	//配置場所を決めるための関数。
	//fxとfyを書き換える。
	//実際に置くタイミングでは返り値をtrueにする。
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
	};
	void Update()override {
		if (*turnPlayer == myColor) {
			if (SetPosition()) {
				field->SetStone(fx, fy);
			}
		}
	};
	void Draw() override {
		int tx = fx * MFS_UNIT + MFS_UNIT / 2;
		int ty = fy * MFS_UNIT + MFS_UNIT / 2;
		DrawCircle(tx, ty, MFS_UNIT / 3, MC_BLUE, 1);
	};
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
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}