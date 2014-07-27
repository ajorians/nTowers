#include "GameDrawer.h"

extern "C"
{
#include "SDL/SDL_gfxPrimitives.h"
}

#include "Metrics.h"
#include "Defines.h"

GameDrawer::GameDrawer(SDL_Surface* pSurface, TowersLib tower, Metrics& metrics)
: m_pSurface(pSurface), m_Towers(tower), m_pBoardMetrics(&metrics)
{
   m_pFont = nSDL_LoadFont(NSDL_FONT_THIN, 0/*R*/, 0/*G*/, 0/*B*/);
   m_pFontBad = nSDL_LoadFont(NSDL_FONT_VGA, 255/*R*/, 0/*G*/, 0/*B*/);
   m_pFontGood = nSDL_LoadFont(NSDL_FONT_VGA, 0/*R*/, 255/*G*/, 0/*B*/);
}

GameDrawer::~GameDrawer()
{
   nSDL_FreeFont(m_pFont);
   nSDL_FreeFont(m_pFontBad);
   nSDL_FreeFont(m_pFontGood);
}

void GameDrawer::Draw()
{
   boxRGBA(m_pSurface, m_pBoardMetrics->GetBoardLeft(), m_pBoardMetrics->GetBoardTop(), m_pBoardMetrics->GetBoardRight(), m_pBoardMetrics->GetBoardBottom(), GAME_BACKGROUND_R, GAME_BACKGROUND_G, GAME_BACKGROUND_B, 230);

   for(int nX=0; nX<=m_pBoardMetrics->GetWidth(); nX++) {
       vlineRGBA(m_pSurface, m_pBoardMetrics->GetLeftSpots() + nX*m_pBoardMetrics->GetPieceSize(), m_pBoardMetrics->GetTopSpots(), m_pBoardMetrics->GetBottomSpots(), 0, 0, 0, 255);
   }

   for(int nY=0; nY<=m_pBoardMetrics->GetHeight(); nY++) {
       hlineRGBA(m_pSurface, m_pBoardMetrics->GetLeftSpots(), m_pBoardMetrics->GetRightSpots(), m_pBoardMetrics->GetTopSpots() + nY*m_pBoardMetrics->GetPieceSize(), 0, 0, 0, 255);
   }

   for(int nX=0; nX<m_pBoardMetrics->GetWidth(); nX++) {
       for(int nY=0; nY<m_pBoardMetrics->GetHeight(); nY++) {
           DrawCell(nX, nY);
       }
   }

   for(int nIndicator=0; nIndicator<GetNumberOfIndicators(m_Towers); nIndicator++)
       DrawIndicator(nIndicator);
}

void GameDrawer::DrawCell(int nX, int nY)
{
   int nValue = GetTowersSpotValue(m_Towers, nX, nY);
   if( nValue <= 0 )
      return;

   if( GetTowersDuplicateSpotValue(m_Towers, nX, nY) == TOWERS_DUPLICATE_SPOT ) {
      SDL_Rect rect;
      rect.w = rect.h = m_pBoardMetrics->GetPieceSize();
      rect.x = m_pBoardMetrics->GetXPos(nX);
      rect.y = m_pBoardMetrics->GetYPos(nY);
      SDL_FillRect(m_pSurface, &rect, SDL_MapRGB(m_pSurface->format, 255, 255, 0));
   }

   char buffer[16];
   Puz_itoa(nValue, buffer, 16);
   int nIndent = (m_pBoardMetrics->GetPieceSize() - nSDL_GetStringWidth(m_pFont, buffer))/2;
   nSDL_DrawString(m_pSurface, m_pFont, m_pBoardMetrics->GetXPos(nX) + nIndent, m_pBoardMetrics->GetYPos(nY) + m_pBoardMetrics->GetPieceSize()/2, buffer);
}

void GameDrawer::DrawIndicator(int nIndex)
{
   int nSide =  GetIndicatorSide(m_Towers, nIndex), nPos = GetIndicatorPos(m_Towers, nIndex), nValue = GetIndicatorValue(m_Towers, nIndex);

   int nX = m_pBoardMetrics->GetIndicatorXPos(nSide, nPos), nY = m_pBoardMetrics->GetIndicatorYPos(nSide, nPos);

   char buffer[16];
   Puz_itoa(nValue, buffer, 16);
   bool bHorizontal = ((nSide == TOWERS_SIDE_TOP) || (nSide == TOWERS_SIDE_BOTTOM));
   int nIndentX = ((bHorizontal ? m_pBoardMetrics->GetPieceSize() : m_pBoardMetrics->GetIndicatorSize()) - nSDL_GetStringWidth(m_pFontGood, buffer))/2;
   int nIndentY = ((bHorizontal ? m_pBoardMetrics->GetIndicatorSize() : m_pBoardMetrics->GetPieceSize()) - nSDL_GetStringHeight(m_pFontGood, buffer))/2;

   nSDL_DrawString(m_pSurface, IsTowersIndicatorSatisfied(m_Towers, nIndex) == TOWERSLIB_INDICATOR_SATISFIED ? m_pFontGood : m_pFontBad, nX + nIndentX, nY + nIndentY, buffer);
}


