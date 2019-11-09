#include "PlayerMinMaxFromBooks.h"
#include "PlayerNextMax.h"


bool PlayerMinMaxFromBooks::SetPosition()
{
	//‰¼‘zŠÂ‹«‚ðì‚èAŽ©•ª‚ÌŽè”Ô‚ªÅ‘å‚É‚È‚é‚æ‚¤‚É‘I‚Ô
	eFieldColor _turnPlayer = myColor;
	
	//Field f(_turnPlayer);
	//f.SetFieldStone(field->GetFieldStone());
	Field f = *field;

	static int nowTurn = -1;
	if (nowTurn != f.GetElapsedTurn() /*&& f.GetTurnPlayer() == myColor*/) {
		amount = 0;
		drawFields.nextDrawFields.clear();
		bestX.clear();
		bestY.clear();

		drawFields.fieldStone = field->GetFieldStone();
		nowTurn = f.GetElapsedTurn();
		int max = -1;
		int maxNum = -1;
		alphaMax = -1;
		betaMin = MFS_AMOUNT + 1;
		for (int i = 0; i < f.GetNextStones().size(); ++i) {
			drawFields.Add(f.GetNextStones()[i]);
			int t = SearchBestPosition(f, f.GetNextStones()[i].x, f.GetNextStones()[i].y, 0,drawFields.GetLast(),alphaMax,betaMin);
			if (max < t) {
				max = t;
				maxNum = i;
			}
		}
		if (maxNum != -1) {
			fx = f.GetNextStones()[maxNum].x;
			fy = f.GetNextStones()[maxNum].y;
		}
	}


	int leftX = MWS_YMAX;
	int rightX = MWS_XMAX_WINDOW - 50;
	int size = 50;
	//for (int i = 0; i < drawFields.nextDrawFields.size() && i < 1;++i) {
	//	/*if (i != 0) {
	//		offset += drawFields.nextDrawFields[i - 1].nextDrawFields.size();
	//	}*/
	//	DrawDrawField(drawFields.nextDrawFields[i], (leftX + rightX) / 2, 0, leftX,rightX, (leftX + rightX) / 2, 0,size);
	//}
	drawFields.bestF = true;
	
	DrawDrawField(drawFields, (leftX + rightX) / 2, 0, leftX, rightX, (leftX + rightX) / 2, 0, size);
	DrawFormatString(0, MWS_YMAX, MC_WHITE, "%ld", amount);

	//for (int i = 0; i < drawFields.nextDrawFields.size(); ++i) {
	//	drawFields.nextDrawFields[i].fieldStone.DrawField(MWS_XMAX + 10, 10 + i * 21, 20);
	//}
	return true;
}

/*int PlayerMinMaxFromBooks::SearchBestPosition()
{
	return 0;
}*/

PlayerMinMaxFromBooks::PlayerMinMaxFromBooks(Field* field, eFieldColor myColor, bool saveF) :BasePlayer(field, myColor, saveF) {

}
