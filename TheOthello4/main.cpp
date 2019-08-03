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
	//実際の盤面
	eFieldColor stone[MFS_XSIZE][MFS_YSIZE];
	//実際にいくつずつあるか
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
	//次の盤面の候補を格納しておく
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

	//指定点に置けるか調べ、置けるなら置く。置けたらtrueを返す
	bool SetNextStonePoint(fieldstone *s,int x,int y) {
		//指定点に石があれば置けないので終了
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
				s = "黒の勝ち！";
			}
			else if(t < 0){
				s = "白の勝ち！";
			}
			else {
				s = "引き分け";
			}
			DrawFormatString(MFS_WIDTH, 0, MC_WHITE, (s).c_str());
		}
	}

	//呼び出されたとき、置かれた位置に次の盤面候補があるか調べる
	//もしあればそれを呼び出して次のターンへ
	void SetStone(int x,int y) {
		for (int i = 0; i < nextStones.size(); ++i) {
			fieldstone f = nextStones[i];
			if (f.x == x && f.y == y) {
				//ここにきていれば次の盤面がある
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
		//ターンプレイヤーは書き換えない
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

//マウス入力
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

//完全ランダム
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

//左上からローラー作戦
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

//次が最大のものを探す
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

//minmax方に基づいて配置する。とりあえず2段階
class PlayerMinMax :public BasePlayer {
	bool SetPosition() override {
		//仮想環境を作り、自分の手番が最大になるように選ぶ
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
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}