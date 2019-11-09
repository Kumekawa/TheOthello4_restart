#include "Field.h"
#include "MacroColor.h"
#include "Between.h"
#include "DrawLineStright.h"

void Field::DrawStone(int x, int y, fieldstone efc) {

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

bool Field::SetNextStonePoint(fieldstone* s, int x, int y){
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
				if (s->stone[m][n] == turnPlayer) {
					if (k > 1) {
						for (; k >= 0; --k) {
							int a = x + i * k;
							int b = y + j * k;
							s->stone[a][b] = turnPlayer;
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

bool Field::SetNextStone(){
	bool putF = false;
	//2回繰り返したら置けるところが無いのでゲーム終了
	nextStones.clear();
	int n = 0;
	do {
		if (n > 0) {
			ChangeFieldColor(&turnPlayer);
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

Field::Field(eFieldColor turnPlayer){
	this->turnPlayer = turnPlayer;
	Initialize();
}

void Field::Initialize() {
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
	turnPlayer = eFC_Black;
	SetNextStone();
	endCounter = 0;
	endF = false;
	elapsedTurn = 0;
	history.clear();
}

void Field::Update(){
	if (endCounter > 0) {
		++endCounter;
		endF = true;
	}
}

void Field::Draw() {
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
	DrawFormatString(MFS_WIDTH, 0, MC_WHITE, "\n\nblack:%d\nwhite%d\nturn:%d\nPlayer:%s", b, w, elapsedTurn, GetTurnPlayer() == eFC_Black ? "black" : "white");
	if (endCounter > 0) {
		int t = b - w;
		string s;
		if (t > 0) {
			s = "黒の勝ち！";
		}
		else if (t < 0) {
			s = "白の勝ち！";
		}
		else {
			s = "引き分け";
		}
		if (endCounter < 5) {

		}
		else {
			DrawFormatString(MFS_WIDTH, 0, MC_WHITE, "Press R To Restart");
		}
		DrawFormatString(MFS_WIDTH, 0, MC_WHITE, ("\n" + s).c_str());
	}
}


void Field::SetStone(int x, int y) {
	for (int i = 0; i < nextStones.size(); ++i) {
		fieldstone f = nextStones[i];
		if (f.x == x && f.y == y) {
			//ここにきていれば次の盤面がある
			fieldStone = f;
			ChangeFieldColor(&turnPlayer);
			if (SetNextStone() == false) {
				endCounter++;
			}
			history.push_back(fieldStone);
			fieldStone.SetAmount();
			++elapsedTurn;
			break;
		}
	}
}

vector<fieldstone> Field::GetNextStones() {
	return nextStones;
}
void Field::SetFieldStone(fieldstone fieldStone) {
	this->fieldStone = fieldStone;
	//ターンプレイヤーは書き換えない。歴史も書き換えない
	if (SetNextStone() == false) {
		endCounter++;
	}
	fieldStone.SetAmount();
}
fieldstone Field::GetFieldStone() {
	return fieldStone;
}
int Field::GetElapsedTurn() {
	return elapsedTurn;
}
int Field::GetEndCounter() {
	return endCounter;
}
bool Field::GetEndF() {
	return endF;
}
vector<fieldstone> Field::GetHistory() {
	return history;
}

eFieldColor Field::GetTurnPlayer()
{
	return turnPlayer;
}
