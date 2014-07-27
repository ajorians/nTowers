//Public domain :)

#include <os.h>
#include "include/TowersLib.h"
#include "Defines.h"

struct TowersItem
{
   int m_nValue;
};

struct TowersIndicator
{
   int m_nSide;
   int m_nPosition;
   int m_nValue;
};

struct TowersBoard
{
   int m_nWidth;
   int m_nHeight;
   struct TowersItem* m_pItems;
   int m_nIndicators;
   struct TowersIndicator* m_pIndicators;
};

struct TowersItem* GetAt(struct TowersBoard* pBoard, int nX, int nY)
{
   if( nX < 0 || nY < 0 || (nX >= pBoard->m_nWidth) || (nY >= pBoard->m_nHeight) ) {
      printf("Accessing non-existant element %d,%d\n", nX, nY);
      return NULL;
   }

   return pBoard->m_pItems + (pBoard->m_nWidth * nY + nX);
}

struct TowersIndicator* GetIndicator(struct TowersBoard* pBoard, int nIndex)
{
   if( nIndex < 0 || nIndex >= pBoard->m_nIndicators ) {
      printf("Accessing non-existant indicator %d\n", nIndex);
      return NULL;
   }

   return pBoard->m_pIndicators + nIndex;
}

struct TowersAction
{
   int m_nX;
   int m_nY;
   int m_nValue;
   struct TowersAction* m_pNext;
};

struct TowersSolution
{
   int m_nX;
   int m_nY;
   int m_nValue;
   struct TowersSolution* m_pNext;
};

struct Towers
{
   struct TowersBoard* m_pBoard;
   char* m_pstrFile;
   struct TowersAction* m_pUndoActions;
   struct TowersAction* m_pRedoActions;
   struct TowersSolution* m_pSolution;
   int m_nLastError;
};

void AddSolutionItem(struct Towers* pT, int nX, int nY, int nValue)
{
   if( nX < 0 || nY < 0 ) return;
   struct TowersSolution* pSol = malloc(sizeof(struct TowersSolution));
   pSol->m_nX = nX;
   pSol->m_nY = nY;
   pSol->m_nValue = nValue;
   pSol->m_pNext = pT->m_pSolution;
   pT->m_pSolution = pSol;
}

void ClearSolution(TowersLib api)
{
   DEBUG_FUNC_NAME;
   
   struct Towers* pT = (struct Towers*)api;

   struct TowersSolution* pCurrent = pT->m_pSolution;
   while(pCurrent != NULL) {
      struct TowersSolution* pTemp = pCurrent;
      pCurrent = pCurrent->m_pNext;
      free(pTemp);
      pTemp = NULL;
      pT->m_pSolution = pCurrent;
   }
}

int TowersLoadBoard(TowersLib api, char* pstrFile)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;

   if( pT->m_pBoard != NULL ) {
      free(pT->m_pBoard);
      pT->m_pBoard = NULL;
   }

   pT->m_pBoard = malloc(sizeof(struct TowersBoard));
   if( pT->m_pBoard == NULL ){//Out of memory
      free(pT);
      pT = NULL;
      return TOWERSLIB_OUT_OF_MEMORY;
   }
   pT->m_pBoard->m_pItems = NULL;
   pT->m_pBoard->m_pIndicators = NULL;

   if( strstr(pstrFile, "Towers ") != pstrFile ) {//Towers file version check
      free(pT->m_pBoard);
      pT->m_pBoard = NULL;
      free(pT);
      pT = NULL;
      return TOWERSLIB_NOT_PUZZLE_LEVEL;
   }

   if( strstr(pstrFile, "1 ") != (pstrFile + strlen("Towers ")) ) {//Version check
      free(pT->m_pBoard);
      pT->m_pBoard = NULL;
      free(pT);
      pT = NULL;
      return TOWERSLIB_UNKNOWN_VERSION;
   }

   char* pstr = pstrFile + strlen("Towers 1 ");
   char buffer[16];
   int nSpotInBuffer = 0;

   int nWidth = -1, nHeight = -1, nIndicators = -1;
   int nIndicator = 0;
   int nSide = -1, nPos = -1;
   while(pstr != '\0') {
      char ch = *pstr; pstr++;
      if( isdigit(ch) ) {
         buffer[nSpotInBuffer++] = ch;
      }
      else {
         if( !isspace(ch) )
            break;
         buffer[nSpotInBuffer] = '\0';
         nSpotInBuffer = 0;
         int nValue = atoi(buffer);

         if( nWidth < 0 ) {
            nWidth = nValue;
            pT->m_pBoard->m_nWidth = nWidth;
         }
         else if( nHeight < 0 ) {
            nHeight = nValue;
            pT->m_pBoard->m_nHeight = nHeight;
            pT->m_pBoard->m_pItems = malloc(nWidth*nHeight*sizeof(struct TowersItem));

            if( pT->m_pBoard->m_pItems == NULL ) {//Out of memory
               free(pT->m_pBoard);
               pT->m_pBoard = NULL;
               free(pT);
               pT = NULL;
               return TOWERSLIB_OUT_OF_MEMORY;
            }

            int x,y;
            for(x=0; x<nWidth; x++)
               for(y=0; y<nHeight; y++)
                  GetAt(pT->m_pBoard, x, y)->m_nValue = 0;

         }
         else if( nIndicators < 0 ) {
            nIndicators = nValue;
            pT->m_pBoard->m_nIndicators = nIndicators;
            pT->m_pBoard->m_pIndicators = malloc(nValue*sizeof(struct TowersIndicator));

            if( pT->m_pBoard->m_pIndicators == NULL ) {//Out of memory
               free(pT->m_pBoard->m_pItems);
               pT->m_pBoard->m_pItems = NULL;
               free(pT->m_pBoard);
               pT->m_pBoard = NULL;
               free(pT);
               pT = NULL;
               return TOWERSLIB_OUT_OF_MEMORY;
            }

            int n;
            for(n=0; n<nIndicators; n++) {
               struct TowersIndicator* pIndicator = GetIndicator(pT->m_pBoard, n);
               pIndicator->m_nSide = 0;
               pIndicator->m_nPosition = 0;
               pIndicator->m_nValue = 0;
            }
         }
         else {
            if( nSide <= -1 ) {
               nSide = nValue;
            }
            else if( nPos <= -1 ) {
               nPos = nValue;
            }
            else {
               struct TowersIndicator* pIndicator = GetIndicator(pT->m_pBoard, nIndicator++);
               pIndicator->m_nSide = nSide;
               pIndicator->m_nPosition = nPos;
               pIndicator->m_nValue = nValue;

               nSide = nPos = -1;
            }

            //Maybe check if has more numbers than it should?
         }
      }
   }
   if( nSide >= 0 && nPos >= 0 ) {
      buffer[nSpotInBuffer] = '\0';
      int nValue = atoi(buffer);
      struct TowersIndicator* pIndicator = GetIndicator(pT->m_pBoard, nIndicator);
      pIndicator->m_nSide = nSide;
      pIndicator->m_nPosition = nPos;
      pIndicator->m_nValue = nValue;
   }

   pT->m_pSolution = NULL;
   pstr = strstr(pstrFile, "Solution ");
   if( pstr == NULL ) {
      //Should never happen!
   }
   pstr = pstr + strlen("Solution ");
   int nSolX = -1, nSolY = -1, nSolVal = -1;
   while(pstr != '\0') {
      char ch = *pstr; pstr++;
      if( isdigit(ch) ) {
         buffer[nSpotInBuffer++] = ch;
      }
      else {
         if( !isspace(ch) || ch == '\r' || ch == '\n' || ch == '\0' )
            break;
         buffer[nSpotInBuffer] = '\0';
         nSpotInBuffer = 0;
         int nValue = atoi(buffer);

         if( nSolX < 0 ) {
            nSolX = nValue;
         }
         else if( nSolY < 0 ) {
            nSolY = nValue;
         } else {
            nSolVal = nValue;
            AddSolutionItem(pT, nSolX, nSolY, nSolVal);
            nSolX = -1;
            nSolY = -1;
            nSolVal = -1;
         }
      }
   }
   buffer[nSpotInBuffer] = '\0';
   int nValue = atoi(buffer);
   nSolVal = nValue;
   AddSolutionItem(pT, nSolX, nSolY, nSolVal);

   return TOWERSLIB_OK;
}

int TowersLibCreate(TowersLib* api, const char* pstrFile)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = malloc(sizeof(struct Towers));
   if( pT == NULL ){//Out of memory
      return TOWERSLIB_OUT_OF_MEMORY;
   }

   pT->m_pstrFile = pstrFile;
   pT->m_pBoard = NULL;

   int nRet = TowersLoadBoard((TowersLib)pT, pstrFile);
   if( nRet != TOWERSLIB_OK )
      return nRet;

   pT->m_pUndoActions = NULL;
   pT->m_pRedoActions = NULL;
   pT->m_nLastError = TOWERSLIB_OK;

   *api = pT;

   return TOWERSLIB_OK;
}

void ClearUndos(TowersLib api)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;

   struct TowersAction* pCurrent = pT->m_pUndoActions;
   while(pCurrent != NULL) {
      struct TowersAction* pTemp = pCurrent;
      pCurrent = pCurrent->m_pNext;
      free(pTemp);
      pTemp = NULL;
      pT->m_pUndoActions = pCurrent;
   }

}

void ClearRedos(TowersLib api)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;

   struct TowersAction* pCurrent = pT->m_pRedoActions;
   while(pCurrent != NULL) {
      struct TowersAction* pTemp = pCurrent;
      pCurrent = pCurrent->m_pNext;
      free(pTemp);
      pTemp = NULL;
      pT->m_pRedoActions = pCurrent;
   }

}

void AddUndo(TowersLib api, int nX, int nY, int nValue)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;

   struct TowersAction* pAction = malloc(sizeof(struct TowersAction));
   if( pAction == NULL ) {
      DEBUG_MSG("Out of memory: AddUndo\n");
   //Assume allocated
   }

   pAction->m_nX = nX;
   pAction->m_nY = nY;
   pAction->m_nValue = nValue;

   struct TowersAction* pRoot = pT->m_pUndoActions;
   pAction->m_pNext = pRoot;
   pT->m_pUndoActions = pAction;
}

void AddRedo(TowersLib api, int nX, int nY, int nValue)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;

   struct TowersAction* pAction = malloc(sizeof(struct TowersAction));
   if( pAction == NULL ) {
      DEBUG_MSG("Out of memory: AddRedo\n");
      //Assume allocated
   }

   pAction->m_nX = nX;
   pAction->m_nY = nY;
   pAction->m_nValue = nValue;

   struct TowersAction* pRoot = pT->m_pRedoActions;
   pAction->m_pNext = pRoot;
   pT->m_pRedoActions = pAction;
}

int TowersLibFree(TowersLib* api)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = *api;

   ClearUndos(*api);
   ClearRedos(*api);
   ClearSolution(*api);

   free(pT->m_pBoard->m_pItems);
   pT->m_pBoard->m_pItems = NULL;

   free(pT->m_pBoard);
   pT->m_pBoard = NULL;
   free(pT);
   pT = NULL;

   *api = NULL;
   return TOWERSLIB_OK;
}

int GetTowersLibError(TowersLib api)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;
   return pT->m_nLastError;
}

void ClearTowersLibError(TowersLib api)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;
   pT->m_nLastError = TOWERSLIB_OK;
}

//TowersLib related functions
int GetTowersBoardWidth(TowersLib api)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;

   return pT->m_pBoard->m_nWidth;
}

int GetTowersBoardHeight(TowersLib api)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;

   return pT->m_pBoard->m_nHeight;
}

int GetTowersSpotValue(TowersLib api, int nX, int nY)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;

   return GetAt(pT->m_pBoard, nX, nY)->m_nValue;
}

int GetTowersDuplicateSpotValue(TowersLib api, int nX, int nY)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;

   int n = 0;
   for(n=0; n<GetTowersBoardWidth(api); n++) {
      if( n == nX )
         continue;
      if( GetTowersSpotValue(api, n, nY) == GetTowersSpotValue(api, nX, nY) )
         return TOWERS_DUPLICATE_SPOT;
   }

   for(n=0; n<GetTowersBoardHeight(api); n++) {
      if( n == nY )
         continue;
      if( GetTowersSpotValue(api, nX, n) == GetTowersSpotValue(api, nX, nY) )
         return TOWERS_DUPLICATE_SPOT;
   }

   return TOWERS_NOT_DUPLICATE_SPOT;
}

int SetTowersSpotValue(TowersLib api, int nX, int nY, int nValue)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;

   ClearRedos(api);

   int nOldValue = GetAt(pT->m_pBoard, nX, nY)->m_nValue;
   AddUndo(api, nX, nY, nOldValue);
   GetAt(pT->m_pBoard, nX, nY)->m_nValue = nValue;

   return TOWERSLIB_OK;
}

int HasIndicatorsOnSide(TowersLib api, int nSide)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;
   struct TowersBoard* pBoard = pT->m_pBoard;

   int nIndicator, nNumIndicators = pBoard->m_nIndicators;
   for(nIndicator=0; nIndicator<nNumIndicators; nIndicator++) {
      struct TowersIndicator* pIndicator = GetIndicator(pBoard, nIndicator);
      if( pIndicator->m_nSide == nSide )
         return TOWERS_HAS_INDICATOR;
   }

   return TOWERS_NO_INDICATOR;
}

int GetNumberOfIndicators(TowersLib api)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;
   struct TowersBoard* pBoard = pT->m_pBoard;

   return pBoard->m_nIndicators;
}

int GetIndicatorSide(TowersLib api, int nIndex)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;
   struct TowersBoard* pBoard = pT->m_pBoard;

   if( nIndex < 0 || nIndex > pBoard->m_nIndicators )
      return -1;

   struct TowersIndicator* pIndicator = GetIndicator(pBoard, nIndex);
   return pIndicator->m_nSide;
}

int GetIndicatorPos(TowersLib api, int nIndex)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;
   struct TowersBoard* pBoard = pT->m_pBoard;

   if( nIndex < 0 || nIndex > pBoard->m_nIndicators )
      return -1;

   struct TowersIndicator* pIndicator = GetIndicator(pBoard, nIndex);
   return pIndicator->m_nPosition;
}

int GetIndicatorValue(TowersLib api, int nIndex)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;
   struct TowersBoard* pBoard = pT->m_pBoard;

   if( nIndex < 0 || nIndex > pBoard->m_nIndicators )
      return -1;

   struct TowersIndicator* pIndicator = GetIndicator(pBoard, nIndex);
   return pIndicator->m_nValue;
}

int IsTowersSolved(TowersLib api)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;

   int nX = 0, nY = 0;
   for(nX = 0; nX<GetTowersBoardWidth(api); nX++)
      for(nY = 0; nY<GetTowersBoardHeight(api); nY++)
         if( GetTowersSpotValue(api, nX, nY) <= 0 )
            return TOWERSLIB_NOTSOLVED_EMPTYSPOTS;

   for(nX = 0; nX<GetTowersBoardWidth(api); nX++) {
      int nValue = 0;
      for(nValue=1; nValue<=GetTowersBoardWidth(api); nValue++) {
         int nFoundValue = 0;
         for(nY = 0; nY<GetTowersBoardHeight(api); nY++) {
            if( GetTowersSpotValue(api, nX, nY) == nValue ) {
               nFoundValue = 1;
               break;
            }
         }
         if( nFoundValue == 0 )
            return TOWERSLIB_NOTSOLVED_MISSCOL;
      }
   }

   for(nY = 0; nY<GetTowersBoardHeight(api); nY++) {
      int nValue = 0;
      for(nValue=1; nValue<=GetTowersBoardWidth(api); nValue++) {
         int nFoundValue = 0;
         for(nX = 0; nX<GetTowersBoardWidth(api); nX++) {
            if( GetTowersSpotValue(api, nX, nY) == nValue ) {
               nFoundValue = 1;
               break;
            }
         }
         if( nFoundValue == 0 )
            return TOWERSLIB_NOTSOLVED_MISSROW;
      }
   }

   int nIndicator = 0;
   for(nIndicator = 0; nIndicator<GetNumberOfIndicators(api); nIndicator++)
      if( IsTowersIndicatorSatisfied(api, nIndicator) == TOWERSLIB_INDICATOR_NOTSATISFIED )
         return TOWERSLIB_NOTSOLVED_INDICATOR;

   return TOWERSLIB_SOLVED;
}

int IsTowersEmpty(TowersLib api)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;

   int nX = 0, nY = 0;
   for(nX = 0; nX<GetTowersBoardWidth(api); nX++)
      for(nY = 0; nY<GetTowersBoardHeight(api); nY++)
         if( GetTowersSpotValue(api, nX, nY) > 0 )
            return TOWERSLIB_NOT_EMPTY;

   return TOWERSLIB_EMPTY;
}

int IsTowersIndicatorSatisfied(TowersLib api, int nIndex)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;
   struct TowersBoard* pBoard = pT->m_pBoard;

   int nSide = GetIndicatorSide(api, nIndex), nPos = GetIndicatorPos(api, nIndex);

   if( nIndex < 0 || nIndex > pBoard->m_nIndicators )
      return TOWERSLIB_INDICATOR_NOTSATISFIED;

   int nValue = GetIndicatorValue(api, nIndex);
   int n = 0;
   int nVisible = 0;
   int nStartX = 0, nStartY = 0;
   int nLastVisible = 0;
   if( nSide == TOWERS_SIDE_TOP ) {
      nStartX = nPos;
      nStartY = 0;
   } else if( nSide == TOWERS_SIDE_LEFT ) {
      nStartX = 0;
      nStartY = nPos;
   } else if( nSide == TOWERS_SIDE_RIGHT ) {
      nStartX = GetTowersBoardWidth(api)-1;
      nStartY = nPos;
   } else {
      nStartX = nPos;
      nStartY = GetTowersBoardHeight(api)-1;
   }
   for(n=0; n<GetTowersBoardWidth(api); n++) {
      int nX = nStartX, nY = nStartY;
      if( nSide == TOWERS_SIDE_TOP ) {
         nY = nStartY + n;
      } else if( nSide == TOWERS_SIDE_LEFT ) {
         nX = nStartX + n;
      } else if( nSide == TOWERS_SIDE_RIGHT ) {
         nX = nStartX - n;
      } else {
         nY = nStartY - n;
      }

      int nSpotValue = GetTowersSpotValue(api, nX, nY);
      if( nSpotValue <= 0 )
         return TOWERSLIB_INDICATOR_NOTSATISFIED;
      if( nSpotValue > nLastVisible ) {
         nVisible++;
         nLastVisible = nSpotValue;
      }
   }

   if( nVisible != nValue )
      return TOWERSLIB_INDICATOR_NOTSATISFIED;

   return TOWERSLIB_INDICATOR_SATISFIED;
}

int SetTowersToSolved(TowersLib api)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;
   struct TowersBoard* pBoard = pT->m_pBoard;

   struct TowersSolution* pCurrent = pT->m_pSolution;
   while(pCurrent != NULL) {
      GetAt(pT->m_pBoard, pCurrent->m_nX, pCurrent->m_nY)->m_nValue = pCurrent->m_nValue;
      pCurrent = pCurrent->m_pNext;
   }

   return TOWERSLIB_OK;
}

int TowersUndo(TowersLib api)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;
   struct TowersBoard* pBoard = pT->m_pBoard;

   struct TowersAction* pRoot = pT->m_pUndoActions;
   if( pRoot == NULL )
      return TOWERSLIB_CANNOT_UNDO;

   pT->m_pUndoActions = pRoot->m_pNext;
   AddRedo(api, pRoot->m_nX, pRoot->m_nY, GetTowersSpotValue(api, pRoot->m_nX, pRoot->m_nY));
   GetAt(pBoard, pRoot->m_nX, pRoot->m_nY)->m_nValue = pRoot->m_nValue;

   free(pRoot);
   pRoot = NULL;

   return TOWERSLIB_OK;
}

int TowersRedo(TowersLib api)
{
   DEBUG_FUNC_NAME;

   struct Towers* pT = (struct Towers*)api;
   struct TowersBoard* pBoard = pT->m_pBoard;

   struct TowersAction* pRoot = pT->m_pRedoActions;
   if( pRoot == NULL )
      return TOWERSLIB_CANNOT_REDO;

   pT->m_pRedoActions = pRoot->m_pNext;
   AddUndo(api, pRoot->m_nX, pRoot->m_nY, GetTowersSpotValue(api, pRoot->m_nX, pRoot->m_nY));
   GetAt(pBoard, pRoot->m_nX, pRoot->m_nY)->m_nValue = pRoot->m_nValue;

   free(pRoot);
   pRoot = NULL;

   return TOWERSLIB_OK;
}


