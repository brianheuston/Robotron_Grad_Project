//-----------------------------------------------------------------
// Robotron - The New Age
// C++ Header - MiscSprite.h
//-----------------------------------------------------------------
#ifndef MISCSPRITE_H
#define MISCSPRITE_H

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "Sprite.h"

//-----------------------------------------------------------------
// MiscSprite Class
//-----------------------------------------------------------------
class MiscSprite : public Sprite
{
    protected:
        // Member Variables
        Sprite* m_pPlayer;
        int     m_iHulkTurnCount;
        int     m_iHulkDirection;
        int     m_iMoveDelay;
        int     m_iFrameCount;
        int     m_iPersonRescuedCount;
        int     m_iScoreTimer;
        
    public:
        // Constructor(s)/Destructor
        MiscSprite(Bitmap* pBitmap, RECT& rcBounds, Sprite* pPlayerSprite,
          BOUNDSACTION baBoundsAction = BA_STOP);
        virtual ~MiscSprite();
        
        // Helper methods
        virtual void UpdateFrame();
        virtual SPRITEACTION Update();
        virtual void SetCurFrame(int iCurFrame);
        int GetDirection() { return m_iHulkDirection; };
        int GetPersonRescuedCount() { return m_iPersonRescuedCount; };
        virtual void SetPersonRescuedCount(int iPersonRescuedCount)
          { m_iPersonRescuedCount = iPersonRescuedCount; };
};

#endif
