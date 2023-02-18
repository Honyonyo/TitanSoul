#pragma once
enum eObjectKinds
{
	ePlayer,
	eArrow,
	eMonster,
	eMonsterProjectile,

	eObjectKindsNumCount
};

enum eMoveDirection
{
	eRight = 0,
	eUp,
	eLeft,
	eDown,

	eRightDown,
	eLeftDown,
	eLeftUp,
	eRightUp,

	eMoveDirectionNumCount
};

enum eTileSpec
{
	eTileTextureStart = 0,
	eTileGrass = 1,
	eTileWater = 3,
	eTileWater2 = 19,
	eTileSnow = 8,
	eTileIce,
	eTileSand = 11,
	eTileStone,
	eTileTextureEnd = eTileStone+1,
	eTileFuncStart = 47,
	eTileStaireRight = 48,
	eTileStaireLeft,
	eTileVine,
	eTileStaireUp,
	eTileStaireDown,
	eTileFuncEnd = eTileStaireDown+1
};