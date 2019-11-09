#pragma once
#include "Field.h"
#include "BasePlayer.h"

enum eplayer {
	eHuman,
	eRandom,
	eRoler,
	eNextMax,
	eNextMin,
	eMyAlgorithm,
	eMinMaxHyper,
	eRandomHyper,
	eMyAlgorithmHyper,
	eNextPointMin,
	eDeep,
	eMinMaxFromBooks
};

BasePlayer *SelectPlayer(eplayer ePlayer,Field *field,eFieldColor myColor,bool saveF);
