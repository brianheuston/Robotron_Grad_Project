//-----------------------------------------------------------------
// Robotron - The New Age
// C++ Source - Robotron.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "MiscSprite.h"

//-----------------------------------------------------------------
// External Global Variables
//-----------------------------------------------------------------
extern Bitmap*    g_pLaserBitmap;
extern Bitmap*    g_pGruntBitmap;
extern Bitmap*    g_pHulkBitmap;
extern Bitmap*    g_pElectrodeBitmap;
extern Bitmap*    g_pMikeyBitmap;
extern Bitmap*    g_pScoreBitmap;

//-----------------------------------------------------------------
// Constructor(s)/Destructor
//-----------------------------------------------------------------
MiscSprite::MiscSprite(Bitmap* pBitmap, RECT& rcBounds, Sprite* pPlayerSprite,
  BOUNDSACTION baBoundsAction) : Sprite(pBitmap, rcBounds, baBoundsAction)
{
    m_pPlayer = pPlayerSprite;
    m_iFrameCount = 1;
    m_iHulkDirection = rand() % 4;
}

MiscSprite::~MiscSprite()
{
}

//-----------------------------------------------------------------
// Helper Methods
//-----------------------------------------------------------------
SPRITEACTION MiscSprite::Update()
{
    // Call the base sprite Update() method
    SPRITEACTION saSpriteAction;
    saSpriteAction = Sprite::Update();
    
    if (GetBitmap() == g_pGruntBitmap || GetBitmap() == g_pHulkBitmap)
    {
        // Get the player position
        RECT& rcPlayerPos = m_pPlayer->GetPosition();
        
        if (++m_iMoveDelay == 5)
        {
            // AI for the grunts
            if (GetBitmap() == g_pGruntBitmap)
            {
                // See where the robot is compared to the player
                if ((rcPlayerPos.left + ((rcPlayerPos.right - rcPlayerPos.left) / 2)) > 
                  (GetPosition().left + ((GetPosition().right - GetPosition().left) / 2)))
                    OffsetPosition(3, 0);
                else if ((rcPlayerPos.left + ((rcPlayerPos.right - rcPlayerPos.left) / 2)) < 
                  (GetPosition().left + ((GetPosition().right - GetPosition().left) / 2)))
                    OffsetPosition(-3, 0);
        
                if ((rcPlayerPos.top + ((rcPlayerPos.bottom - rcPlayerPos.top) / 2)) > 
                  (GetPosition().top + ((GetPosition().bottom - GetPosition().top) / 2)))
                    OffsetPosition(0, 3);
                else if ((rcPlayerPos.top + ((rcPlayerPos.bottom - rcPlayerPos.top) / 2)) < 
                  (GetPosition().top + ((GetPosition().bottom - GetPosition().top) / 2)))
                    OffsetPosition(0, -3);
            }
    
            // AI for the hulks
            else if (GetBitmap() == g_pHulkBitmap)
            {
                if (++m_iHulkTurnCount == 50)
                {
                    m_iHulkDirection = rand() % 4;
                    m_iHulkTurnCount = 0;
                }
        
                // Move the hulk based on the direction
                if (m_iHulkDirection == 0)
                {
                    OffsetPosition(0, -3);
                    m_iCurFrame = 0;
                }
                else if (m_iHulkDirection == 1)
                {
                    OffsetPosition(3, 0);
                    m_iCurFrame = 4;
                }
                else if (m_iHulkDirection == 2)
                {
                    OffsetPosition(0, 3);
                    m_iCurFrame = 8;
                }
                else if (m_iHulkDirection == 3)
                {
                    OffsetPosition(-3, 0);
                    m_iCurFrame = 12; 
                }
            }
            m_iMoveDelay = 0;
        }
    }
    else if (GetBitmap() == g_pMikeyBitmap)
    {
        if (++m_iHulkTurnCount == 50)
        {
            m_iHulkDirection = rand() % 4;
            m_iHulkTurnCount = 0;
        }
        if (m_iHulkDirection == 0)
        {
            OffsetPosition(0, -3);
            m_iCurFrame = 0;
        }
        else if (m_iHulkDirection == 1)
        {
            OffsetPosition(3, 0);
            m_iCurFrame = 4;
        }
        else if (m_iHulkDirection == 2)
        {
            OffsetPosition(0, 3);
            m_iCurFrame = 8;
        }
        else if (m_iHulkDirection == 3)
        {
            OffsetPosition(-3, 0);
            m_iCurFrame = 12; 
        }     
    }
    
    return saSpriteAction;
}

void MiscSprite::UpdateFrame()
{
    if (GetBitmap() == g_pElectrodeBitmap || GetBitmap() == g_pLaserBitmap)
    {
    }
    else if (GetBitmap() == g_pHulkBitmap)
    {
        Sprite::UpdateFrame();
    }
    else if (GetBitmap() == g_pScoreBitmap)
    {
        if (++m_iScoreTimer == 50)
        {
            Kill();
        }
    }
    else
    {
        Sprite::UpdateFrame();
    }
}

void MiscSprite::SetCurFrame(int iCurFrame)
{
    m_iCurFrame = iCurFrame;
}
