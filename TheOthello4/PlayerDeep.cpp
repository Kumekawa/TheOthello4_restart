#include "PlayerDeep.h"
#include "PlayerMaxValue.h"
#include "DxLib.h"

double PlayerDeep::GetValue(fieldstone fieldStone, int turn) {
	double sum = 0;
	for (int i = 0; i < MFS_XSIZE; ++i) {
		for (int j = 0; j < MFS_YSIZE; ++j) {
			int t = 0;
			auto f = fieldStone.stone[i][j];
			if (f == myColor) {
				t = 1;
			}
			else if (f == GetChangeFieldColor(myColor)) {
				t = -1;
			}
			else {
				t = 0;
			}
			sum += t * w[turn][i][j];
		}
	}
	return sum;
}

void PlayerDeep::SetWeight() {
	auto tc = field->GetFieldStone().GetMaxColor();
	auto history = field->GetHistory();
	for (int i = 0; i < history.size(); ++i) {
		//ゲーム終了時、自分の色が多ければwは増加、相手が多ければwを減少させる。範囲は-1から1まで
		double base;
		double me = 0;
		double you = 0;
		double e = 0.01;
		if (tc == myColor) {
			me += 1;
			you += -1;
		}
		else if (tc == GetChangeFieldColor(myColor)) {
			me += -1;
			you += 1;
		}
		else {
			me += -1;
			you += -1;
		}
		for (int x = 0; x < MFS_XSIZE; ++x) {
			for (int y = 0; y < MFS_YSIZE; ++y) {
				base = (double)(GetRand(100) - 50) / 100.0;
				if (history[i].stone[x][y] == myColor) {
					w[i][x][y] += (me + base) * e * (1.0 + (double)(GetRand(100) - 50) / 100.0);
				}
				else if (history[i].stone[x][y] == GetChangeFieldColor(myColor)) {
					w[i][x][y] += (you + base) * e * (1.0 + (double)(GetRand(100) - 50) / 100.0);
				}
				//SetBetweenDouble(-1, &w[i][x][y], 1);
			}
		}
	}

	auto error = fopen_s(&fp, ("_" + fname).c_str(), "wb");
	if (error == 0) {
		for (int i = 0; i < MFS_AMOUNT - 4; ++i) {
			for (int x = 0; x < MFS_XSIZE; ++x) {
				for (int y = 0; y < MFS_YSIZE; ++y) {
					fwrite(&w[i][x][y], sizeof(double), 1, fp);
				}
			}
		}
		fclose(fp);
	}
}

bool PlayerDeep::SetPosition() {
	if (endF) {
		SetWeight();
		//stones.clear();
		startF = false;
	}
	if (*turnPlayer == myColor) {

		auto tf = field->GetNextStones();
		if (!(tf.size() > 0)) {
			return false;
		}
		double max;
		bool firstF = true;

		for (int i = 0; i < tf.size(); ++i) {
			auto _turn = GetChangeFieldColor(*turnPlayer);
			Field _field(&_turn);
			_field.SetFieldStone(tf[i]);

			BasePlayer* _player1;
			BasePlayer* _player2;
			bool f = true;
			//_player1 = new PlayerNextMin(&_field, &_turn, myColor, false);
			//_player2 = new PlayerNextMin(&_field, &_turn, GetChangeFieldColor(myColor), false);
			_player1 = new PlayerMaxValue(&_field, &_turn, myColor, w, false);
			_player2 = new PlayerMaxValue(&_field, &_turn, GetChangeFieldColor(myColor), w, false);
			vector<BaseClass*> _objects;
			_objects.push_back(&_field);
			_objects.push_back(_player1);
			_objects.push_back(_player2);

			double t = 0;
			double s = field->GetElapsedTurn();
			while (_field.GetEndF() == false) {
				//if (f && _field.GetElapsedTurn() >= MFS_AMOUNT - 4 - 10) {
				//	f = false;
				//	
				//	_objects.clear();
				//	_player1 = new PlayerMyAlgorithmHyper(&_field, &_turn, myColor, false);
				//	_player2 = new PlayerMyAlgorithmHyper(&_field, &_turn, GetChangeFieldColor(myColor), false);
				//	
				//	_objects.push_back(&_field);
				//	_objects.push_back(_player1);
				//	_objects.push_back(_player2);
				//}
				auto tempF1 = _field.GetFieldStone();
				for (int i = 0; i < _objects.size(); ++i) {
					_objects[i]->Update();

					if (tempF1.GetEquals(_field.GetFieldStone())) {
						tempF1 = _field.GetFieldStone();
						t += GetValue(_field.GetFieldStone(), _field.GetElapsedTurn()) / (MFS_AMOUNT - _field.GetElapsedTurn());
					}
				}
			}
			delete _player1;
			delete _player2;

			if (firstF || max < t) {
				firstF = false;
				max = t;
				fx = tf[i].x;
				fy = tf[i].y;
			}
		}
	}
	return true;
}

PlayerDeep::PlayerDeep(Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF) :BasePlayer(field, turnPlayer, myColor, saveF) {
	if (myColor == eFC_Black) {
		fname = "Black";
	}
	else {
		fname = "White";
	}
	fname += "Weight_" + to_string(MFS_XSIZE) + "x" + to_string(MFS_XSIZE) + ".dat";
	auto error = fopen_s(&fp, fname.c_str(), "rb");
	if (error == 0) {
		for (int i = 0; i < MFS_AMOUNT - 4; ++i) {
			for (int x = 0; x < MFS_XSIZE; ++x) {
				for (int y = 0; y < MFS_YSIZE; ++y) {
					fread_s(&w[i][x][y], sizeof(double), sizeof(double), 1, fp);
				}
			}
		}
		fclose(fp);
	}
	else {
		for (int i = 0; i < MFS_AMOUNT - 4; ++i) {
			for (int x = 0; x < MFS_XSIZE; ++x) {
				for (int y = 0; y < MFS_YSIZE; ++y) {
					w[i][x][y] = 0.5;
				}
			}
		}
	}
}
