#ifndef TOWERSLIB_H_INCLUDED
#define TOWERSLIB_H_INCLUDED

typedef void* TowersLib;

#define TOWERSLIB_OK			(0)
#define TOWERSLIB_BADARGUMENT		(-1)
#define TOWERSLIB_OUT_OF_MEMORY		(-2)
#define TOWERSLIB_NOT_PUZZLE_LEVEL	(-3)
#define TOWERSLIB_UNKNOWN_VERSION	(-4)

#define TOWERSLIB_SOLVED		(0)
#define TOWERSLIB_NOTSOLVED_EMPTYSPOTS  (-1)
#define TOWERSLIB_NOTSOLVED_MISSCOL     (-2)
#define TOWERSLIB_NOTSOLVED_MISSROW     (-3)
#define TOWERSLIB_NOTSOLVED_INDICATOR   (-4)

#define TOWERSLIB_INDICATOR_SATISFIED   (0)
#define TOWERSLIB_INDICATOR_NOTSATISFIED (-1)

#define TOWERSLIB_CANNOT_UNDO		(1)
#define TOWERSLIB_CANNOT_REDO		(1)

#define TOWERS_SIDE_TOP			(1)
#define TOWERS_SIDE_LEFT		(2)
#define TOWERS_SIDE_RIGHT		(3)
#define TOWERS_SIDE_BOTTOM		(4)

#define TOWERS_HAS_INDICATOR		(1)
#define TOWERS_NO_INDICATOR		(0)

#define TOWERS_DUPLICATE_SPOT		(1)
#define TOWERS_NOT_DUPLICATE_SPOT	(0)

#define TOWERSLIB_EMPTY			(1)
#define TOWERSLIB_NOT_EMPTY		(0)

//////////////////////////////////////////////
//Initalization/Error checking/Mode functions
//////////////////////////////////////////////
int TowersLibCreate(TowersLib* api, const char* pstrFile );
int TowersLibFree(TowersLib* api);

int GetTowersLibError(TowersLib api);
void ClearTowersLibError(TowersLib api);

//////////////////////////////////////////////
//TowersLib related functions
//////////////////////////////////////////////
int GetTowersBoardWidth(TowersLib api);
int GetTowersBoardHeight(TowersLib api);
int HasIndicatorsOnSide(TowersLib api, int nSide);
int GetNumberOfIndicators(TowersLib api);
int GetIndicatorSide(TowersLib api, int nIndex);
int GetIndicatorPos(TowersLib api, int nIndex);
int GetIndicatorValue(TowersLib api, int nIndex);
int GetTowersSpotValue(TowersLib api, int nX, int nY);
int GetTowersDuplicateSpotValue(TowersLib api, int nX, int nY);
int SetTowersSpotValue(TowersLib api, int nX, int nY, int nValue);
int IsTowersSolved(TowersLib api);
int IsTowersEmpty(TowersLib api);
int IsTowersIndicatorSatisfied(TowersLib api, int nIndex);
int SetTowersToSolved(TowersLib api);
int TowersUndo(TowersLib api);
int TowersRedo(TowersLib api);

#endif //TOWERSLIB_H_INCLUDED
