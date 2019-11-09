#include "Players.h"


#include "PlayerBetter.h"
#include "PlayerDeep.h"
#include "PlayerHuman.h"
#include "PlayerMaxValue.h"
#include "PlayerMinMax.h"
#include "PlayerMinMaxHyper.h"
#include "PlayerMyalgorithm.h"
#include "PlayerMyAlgorithmHyper.h"
#include "PlayerNextMax.h"
#include "PlayerNextMin.h"
#include "PlayerNextPointMin.h"
#include "PlayerRandom.h"
#include "PlayerRandomHyper.h"
#include "PlayerRoler.h"
#include "PlayerMinMaxFromBooks.h"


BasePlayer* SelectPlayer(eplayer ePlayer, Field* field, eFieldColor myColor, bool saveF)
{
	switch (ePlayer)
	{
	case eHuman:
		return new PlayerHuman(field, myColor, saveF);
	case eRandom:
		return new PlayerRandom(field, myColor, saveF);
	case eRoler:
		return new PlayerRoler(field, myColor, saveF);
	case eNextMax:
		return new PlayerNextMax(field, myColor, saveF);
	case eNextMin:
		return new PlayerNextMin(field, myColor, saveF);
	case eMyAlgorithm:
		return new PlayerMyAlgorithm(field, myColor, saveF);
	case eMinMaxHyper:
		return new PlayerMinMaxHyper(field, myColor, saveF);
	case eRandomHyper:
		return new PlayerRandomHyper(field, myColor, saveF);
	case eMyAlgorithmHyper:
		return new PlayerMyAlgorithmHyper(field, myColor, saveF);
	case eNextPointMin:
		return new PlayerNextPointMin(field, myColor, saveF);
	case eDeep:
		return new PlayerDeep(field, myColor, saveF);		
	case eMinMaxFromBooks:
		return new PlayerMinMaxFromBooks(field, myColor, saveF);
	default:
		break;
	}
	return  new PlayerRandom(field, myColor, saveF);
}
