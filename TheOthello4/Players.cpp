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


BasePlayer* SelectPlayer(eplayer ePlayer, Field* field, eFieldColor* turnPlayer, eFieldColor myColor, bool saveF)
{
	switch (ePlayer)
	{
	case eHuman:
		return new PlayerHuman(field, turnPlayer, myColor, saveF);
	case eRandom:
		return new PlayerRandom(field, turnPlayer, myColor, saveF);
	case eRoler:
		return new PlayerRoler(field, turnPlayer, myColor, saveF);
	case eNextMax:
		return new PlayerNextMax(field, turnPlayer, myColor, saveF);
	case eNextMin:
		return new PlayerNextMin(field, turnPlayer, myColor, saveF);
	case eMyAlgorithm:
		return new PlayerMyAlgorithm(field, turnPlayer, myColor, saveF);
	case eMinMaxHyper:
		return new PlayerMinMaxHyper(field, turnPlayer, myColor, saveF);
	case eRandomHyper:
		return new PlayerRandomHyper(field, turnPlayer, myColor, saveF);
	case eMyAlgorithmHyper:
		return new PlayerMyAlgorithmHyper(field, turnPlayer, myColor, saveF);
	case eNextPointMin:
		return new PlayerNextPointMin(field, turnPlayer, myColor, saveF);
	case eDeep:
		return new PlayerDeep(field, turnPlayer, myColor, saveF);		
	default:
		break;
	}
}
