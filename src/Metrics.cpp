#include "Metrics.h"
extern "C"
{
        #include "TowersLib/TowersLib.h"
}

Metrics::Metrics(int nWidthScreen, int nHeightScreen)
: m_nWidthScreen(nWidthScreen), m_nHeightScreen(nHeightScreen)
{
}

bool Metrics::SetDimensions(int nWidth, int nHeight, bool bTopIndicators, bool bLeftIndicators, bool bRightIndicators, bool bBottomIndicators)
{
   //It just looks better to assume there are indicators on all 4 sides.
   m_bTopInd = true;
   m_bLeftInd = true;
   m_bRightInd = true;
   m_bBottomInd = true;

   m_nWidth = nWidth;
   m_nHeight = nHeight;

   int nPieceWidth = m_nWidthScreen/((double)nWidth + (m_bLeftInd ? INDICATOR_PORTION : 0) + (m_bRightInd ? INDICATOR_PORTION : 0));
   int nPieceHeight = m_nHeightScreen/((double)nHeight + (m_bTopInd ? INDICATOR_PORTION : 0) + (m_bBottomInd ? INDICATOR_PORTION : 0));

   m_nPieceSize = nPieceWidth < nPieceHeight ? nPieceWidth : nPieceHeight;

   int nBoardWidth = m_nPieceSize * nWidth;
   int nBoardHeight = m_nPieceSize * nHeight;

   m_nTopSpots = (m_nHeightScreen - nBoardHeight) / 2;
   m_nLeftSpots = (m_nWidthScreen - nBoardWidth) / 2;

   return true;
}

int Metrics::GetWidth() const { return m_nWidth; }
int Metrics::GetHeight() const { return m_nHeight; }

int Metrics::GetXPos(int nBoardX) const
{
   return m_nLeftSpots + nBoardX*m_nPieceSize;
}

int Metrics::GetYPos(int nBoardY) const
{
   return m_nTopSpots + nBoardY*m_nPieceSize;
}

int Metrics::GetPieceSize() const
{
   return m_nPieceSize;
}

int Metrics::GetBoardLeft() const
{
   return GetLeftSpots() - (m_bLeftInd ? GetIndicatorSize() : 0);
}

int Metrics::GetBoardRight() const
{
   return GetRightSpots() + (m_bRightInd ? GetIndicatorSize() : 0);
}

int Metrics::GetBoardTop() const
{
   return GetTopSpots() - (m_bTopInd ? GetIndicatorSize() : 0);
}

int Metrics::GetBoardBottom() const
{
   return GetBottomSpots() + (m_bBottomInd ? GetIndicatorSize() : 0);
}

int Metrics::GetLeftSpots() const
{
   return m_nLeftSpots;
}

int Metrics::GetRightSpots() const
{
   return m_nLeftSpots + m_nWidth * m_nPieceSize;
}

int Metrics::GetTopSpots() const
{
   return m_nTopSpots;
}

int Metrics::GetBottomSpots() const
{
   return m_nTopSpots + m_nHeight * m_nPieceSize;
}

int Metrics::GetIndicatorSize() const
{
   return GetPieceSize() * INDICATOR_PORTION;
}

int Metrics::GetIndicatorXPos(int nSide, int nPos) const
{
   if( nSide == TOWERS_SIDE_TOP )
      return GetXPos(nPos);
   else if( nSide == TOWERS_SIDE_LEFT )
      return GetBoardLeft();
   else if( nSide == TOWERS_SIDE_RIGHT )
      return GetRightSpots();
   return GetXPos(nPos);
}

int Metrics::GetIndicatorYPos(int nSide, int nPos) const
{
   if( nSide == TOWERS_SIDE_TOP )
      return GetBoardTop();
   else if( nSide == TOWERS_SIDE_LEFT )
      return GetYPos(nPos);
   else if( nSide == TOWERS_SIDE_RIGHT )
      return GetYPos(nPos);
   return GetBottomSpots();
}

