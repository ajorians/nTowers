#ifndef METRICS_H
#define METRICS_H

extern "C"
{
        #include <os.h>
}

#define INDICATOR_PORTION	(0.5)

class Metrics
{
public:
   Metrics(int nWidthScreen, int nHeightScreen);
   bool SetDimensions(int nWidth, int nHeight, bool bTopIndicators, bool bLeftIndicators, bool bRightIndicators, bool bBottomIndicators);//TODO: Rename; call it DetermineDemensions?
   int GetWidth() const;
   int GetHeight() const;
   int GetXPos(int nBoardX) const;
   int GetYPos(int nBoardY) const;
   int GetPieceSize() const;

   int GetBoardLeft() const;
   int GetBoardRight() const;
   int GetBoardTop() const;
   int GetBoardBottom() const;

   int GetLeftSpots() const;
   int GetRightSpots() const;
   int GetTopSpots() const;
   int GetBottomSpots() const;

   int GetIndicatorSize() const;
   int GetIndicatorXPos(int nSide, int nPos) const;
   int GetIndicatorYPos(int nSide, int nPos) const;

protected:
   int m_nWidthScreen, m_nHeightScreen;
   bool m_bTopInd, m_bLeftInd, m_bRightInd, m_bBottomInd;
   int m_nWidth;
   int m_nHeight;
   int m_nPieceSize;
   int m_nLeftSpots;
   int m_nTopSpots;
};

#endif

