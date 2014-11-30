//-----------------------------------------------------------------
// Robotron - The New Age
// C++ Header - Robotron.h
//-----------------------------------------------------------------
#ifndef ROBOTRON_H
#define ROBOTRON_H

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <windows.h>
#include "GameEngine.h"
#include "Resource.h"
#include "Background.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "MiscSprite.h"
#include "math.h"

//-----------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------
GameEngine*     g_pGame;
HINSTANCE       g_hInstance;
HDC             g_hOffscreenDC;
HBITMAP         g_hOffscreenBitmap;
Background*     g_pBackground;
Bitmap*         g_pSplashBitmap;
Bitmap*         g_pSmRobotBitmap;
Bitmap*         g_pGameOverBitmap;
Bitmap*         g_pPlayerBitmap;
Bitmap*         g_pLaserBitmap;
Bitmap*         g_pGruntBitmap;
Bitmap*         g_pHulkBitmap;
Bitmap*         g_pElectrodeBitmap;
Bitmap*         g_pMikeyBitmap;
Bitmap*         g_pScoreBitmap;
BOOL            g_bSplash;
BOOL            g_bGameOver;
BOOL            g_bBetweenLevels;
int             g_iHiScores[5];
int             g_iScore;
int             g_iNumLives;
int             g_iLevelDelay;
int             g_iGameOverDelay;
int             g_iLevel;
int             g_iFireDelay;
int             g_iNumGrunts;
int             g_iNumHulks;
int             g_iNumElectrodes;
int             g_iNumMikey;
int             g_iPersonCount;
Sprite*         g_pPlayerSprite;
MiscSprite*     g_pScoreSprite;

//-----------------------------------------------------------------
// Functions
//-----------------------------------------------------------------
void UpdateHiScores();
BOOL ReadHiScores();
BOOL WriteHiScores();
void NewGame();
void NextLevel();
void CreateRobotArmy();
POINT CreateRandomPosition();
void FireLaser(int iDirection);
void UpdateScore();
void CheckPlayerLives();

#endif
