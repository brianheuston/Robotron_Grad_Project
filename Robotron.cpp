//-----------------------------------------------------------------
// Robotron: The New Age
// C++ Source File - Robotron.cpp
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Robotron.h"

//-----------------------------------------------------------------
// Game Engine Functions
//-----------------------------------------------------------------
BOOL GameInitialize(HINSTANCE hInstance)
{
    // Create the game engine
    g_pGame = new GameEngine(hInstance, TEXT("Robotron"),
      TEXT("Robotron"), IDI_ICON, IDI_ICON_SM, 800, 600);
    if (g_pGame == NULL)
      return FALSE;

    // Set the frame rate
    g_pGame->SetFrameRate(30);

    // Store the instance handle
    g_hInstance = hInstance;

    return TRUE;
}

void GameStart(HWND hWindow)
{
    // Read the hi scores
    ReadHiScores();
    
    // Seed the random generator
    srand(GetTickCount());
    
    // Create the background
    g_pBackground = new Background(800, 600, RGB(0, 0, 0));
    
    // Create the offscreen device context and bitmap
    g_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
    g_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
      g_pGame->GetWidth(), g_pGame->GetHeight());
    SelectObject(g_hOffscreenDC, g_hOffscreenBitmap);
    
    // Create the bitmaps
    HDC hDC = GetDC(hWindow);
    g_pSplashBitmap = new Bitmap(hDC, IDB_SPLASH, g_hInstance);
    g_pSmRobotBitmap = new Bitmap(hDC, IDB_SM_ROBOT, g_hInstance);
    g_pGameOverBitmap = new Bitmap(hDC, IDB_GAME_OVER, g_hInstance);
    g_pPlayerBitmap = new Bitmap(hDC, IDB_EUGENE, g_hInstance);
    g_pLaserBitmap = new Bitmap(hDC, IDB_LASER, g_hInstance);
    g_pGruntBitmap = new Bitmap(hDC, IDB_GRUNT, g_hInstance);
    g_pHulkBitmap = new Bitmap(hDC, IDB_HULK, g_hInstance);
    g_pElectrodeBitmap = new Bitmap(hDC, IDB_ELECTRODE, g_hInstance);
    g_pMikeyBitmap = new Bitmap(hDC, IDB_MIKEY, g_hInstance);
    g_pScoreBitmap = new Bitmap(hDC, IDB_SCORE, g_hInstance);
    
    // Initialize the global variables
    g_bSplash = TRUE;
    g_bGameOver = FALSE;
    
    // Start the menu MIDI
    g_pGame->PlayMIDISong(TEXT("Menu.mid"), TRUE);
    
    // Delete the DC
    DeleteDC(hDC);
}

void GameEnd()
{
    // Close the MIDI player
    g_pGame->CloseMIDIPlayer();
    
    // Cleanup the bitmaps
    delete g_pSplashBitmap;
    delete g_pSmRobotBitmap;
    delete g_pGameOverBitmap;
    delete g_pPlayerBitmap;
    delete g_pLaserBitmap;
    delete g_pGruntBitmap;
    delete g_pHulkBitmap;
    delete g_pElectrodeBitmap;
    delete g_pMikeyBitmap;
    delete g_pScoreBitmap;
    
    // Cleanup the offscreen device context and bitmap
    DeleteObject(g_hOffscreenBitmap);
    DeleteDC(g_hOffscreenDC);
    
    // Cleanup the background
    delete g_pBackground;

    // Cleanup the game engine
	delete g_pGame;
	
	// Save the hi scores
    WriteHiScores();
}

void GameActivate(HWND hWindow)
{
    // Restart the MIDI song
    g_pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
    // Pause the MIDI song
    g_pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
    // Draw the background
    g_pBackground->Draw(hDC);
    
    if (g_bSplash)
    {
        // Draw the splash image
        g_pSplashBitmap->Draw(hDC,
          (g_pGame->GetWidth() - g_pSplashBitmap->GetWidth()) / 2, 0, TRUE);
          
        // Draw the hi scores
        TCHAR szText[64];
        RECT  rect = { 350, 300, 450, 320};
        SetBkMode(hDC, TRANSPARENT);
        SetTextColor(hDC, RGB(255, 255, 255));
        DrawText(hDC, TEXT("High Scores:"), -1, &rect, DT_SINGLELINE |
          DT_CENTER | DT_VCENTER);
        rect.top += 20;
        rect.bottom += 20;
        for (int i = 0; i < 5; i++)
        {
            wsprintf(szText, "%d", g_iHiScores[i]);
            DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | 
              DT_VCENTER);
            rect.top += 20;
            rect.bottom += 20;
        }
        rect.top += 40;
        rect.bottom += 40;
        rect.left -= 60;
        rect.right += 60;
        DrawText(hDC, TEXT("CREDITS:"), -1, &rect, DT_SINGLELINE | DT_CENTER
          | DT_VCENTER);
        rect.top += 20;
        rect.bottom += 20;
        DrawText(hDC, TEXT("Joe McDonough - Music"), -1, &rect, DT_SINGLELINE |
          DT_CENTER | DT_VCENTER);
        rect.top += 20;
        rect.bottom += 20;
        DrawText(hDC, TEXT("Michael Morrison - Game Engine"), -1, &rect,
          DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        rect.top += 20;
        rect.bottom += 20;
        DrawText(hDC, TEXT("Brian Heuston - Lead Programmer"), -1, &rect,
          DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }
    else
    {
        // Draw the bounding rectangle for the game region
        HPEN hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
        SelectObject(hDC, hPen);
        MoveToEx(hDC, 45, 45, NULL);
        LineTo(hDC, 45, 555);
        LineTo(hDC, 755, 555);
        LineTo(hDC, 755, 45);
        LineTo(hDC, 45, 45);
        
        // Delete the pen
        DeleteObject(hPen);
        
        // Draw the current level if the player lost a life or 
        // reached the next level
        if (g_bBetweenLevels)
        {
            RECT rcBounds = { 50, 50, 750, 550 };
            TCHAR szText[64];
            wsprintf(szText, "LEVEL %d", g_iLevel);
            SetBkMode(hDC, TRANSPARENT);
            SetTextColor(hDC, RGB(255, 0, 0));
            DrawText(hDC, szText, -1, &rcBounds,
              DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
        else
        {    
            // Draw the sprites
            g_pGame->DrawSprites(hDC);
        }
        
        // Draw the score
        TCHAR szText[64];
        RECT  rect = { 460, 0, 510, 30 };
        wsprintf(szText, "%d", g_iScore);
        SetBkMode(hDC, TRANSPARENT);
        SetTextColor(hDC, RGB(255, 255, 255));
        DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

        // Draw the number of remaining lives
        for (int i = 0; i < g_iNumLives; i++)
          g_pSmRobotBitmap->Draw(hDC, 520 + (g_pSmRobotBitmap->GetWidth() * i),
          10, TRUE);

        // Draw the game over message, if necessary
        if (g_bGameOver)
            g_pGameOverBitmap->Draw(hDC, 
            (g_pGame->GetWidth() - g_pGameOverBitmap->GetWidth()) / 2, 
            (g_pGame->GetHeight() - g_pGameOverBitmap->GetHeight()) / 2, TRUE);
    }
}

void GameCycle()
{
    if (!g_bGameOver)
    {
        if (!g_bBetweenLevels)
        {
        	// Update the sprites
        	g_pGame->UpdateSprites();
       	}
       	
        // Obtain a device context for repainting the game
        HWND  hWindow = g_pGame->GetWindow();
        HDC   hDC = GetDC(hWindow);

        // Paint the game to the offscreen device context
        GamePaint(g_hOffscreenDC);

        // Blit the offscreen bitmap to the game screen
        BitBlt(hDC, 0, 0, g_pGame->GetWidth(), g_pGame->GetHeight(),
          g_hOffscreenDC, 0, 0, SRCCOPY);

        // Cleanup
        ReleaseDC(hWindow, hDC);
        
        // Pause the game for a bit when a player dies or starts
        // a new level
        if (g_bBetweenLevels == TRUE && --g_iLevelDelay == 0)
        {
            g_bBetweenLevels = FALSE;
            g_iLevelDelay = 60;
        }
    }
    else
    {
    	if (--g_iGameOverDelay == 0)
    	{
    	    // Basically start the game over at the splash screen
    	    g_bSplash = TRUE;
    	    g_bGameOver = FALSE;
    	    
    	    // Reset the MIDI music to the menu music
    	    g_pGame->CloseMIDIPlayer();
    	    g_pGame->PlayMIDISong(TEXT("menu.mid"));
    	} 
    }
}

void HandleKeys()
{
    if (!g_bSplash && !g_bBetweenLevels)
    {
        // Check to see where the player will move
        if (GetAsyncKeyState(VK_LEFT) < 0)
        {
            g_pPlayerSprite->OffsetPosition(-4, 0);
        }
        else if (GetAsyncKeyState(VK_RIGHT) < 0)
        {
            g_pPlayerSprite->OffsetPosition(4, 0);
        }
        if (GetAsyncKeyState(VK_UP) < 0)
        {
            g_pPlayerSprite->OffsetPosition(0, -4);
        }
        else if (GetAsyncKeyState(VK_DOWN) < 0)
        {
            g_pPlayerSprite->OffsetPosition(0, 4);
        }
        
        // Check to see where the laser should be fired. Inside
        // each key, it will check if another key is also being
        // pressed for the option of diagonal lasers. In the
        // horizontal key presses, there are blank checks
        // to make sure there is no double firing of the laser
        if (++g_iFireDelay == 5)
        {
            if (GetAsyncKeyState(VK_NUMPAD8) < 0)
            {
                if (GetAsyncKeyState(VK_NUMPAD4) < 0)
                    FireLaser(1);
                else if (GetAsyncKeyState(VK_NUMPAD6) < 0)
                    FireLaser(3);
                else
                    FireLaser(2);           
            }
            else if (GetAsyncKeyState(VK_NUMPAD5) < 0)
            {
                if (GetAsyncKeyState(VK_NUMPAD4) < 0)
                    FireLaser(7);
                else if (GetAsyncKeyState(VK_NUMPAD6) < 0)
                    FireLaser(5);
                else
                    FireLaser(6);
            }
            if (GetAsyncKeyState(VK_NUMPAD4) < 0)
            {
                if (GetAsyncKeyState(VK_NUMPAD8) < 0 || 
                  GetAsyncKeyState(VK_NUMPAD5) < 0)
                {
                    // Do nothing
                }    
                else
                    FireLaser(0);
            }
            else if (GetAsyncKeyState(VK_NUMPAD6) < 0)
            {
                if (GetAsyncKeyState(VK_NUMPAD8) < 0 || 
                  GetAsyncKeyState(VK_NUMPAD5) < 0)
                {
                    // Do nothing
                }    
                else
                    FireLaser(4);
            }        
            g_iFireDelay = 0;
        }                        
    }
    if (GetAsyncKeyState(VK_RETURN) < 0)
    {
        if (g_bSplash || g_bGameOver)
            // Start a new game
            NewGame();
    }
}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{
    Bitmap* pHitter = pSpriteHitter->GetBitmap();
    Bitmap* pHittee = pSpriteHittee->GetBitmap();
    
    // Check to see if a laser and a grunt have collided
    if ((pHitter == g_pLaserBitmap && pHittee == g_pGruntBitmap) ||
      (pHitter == g_pGruntBitmap && pHittee == g_pLaserBitmap))
    {
        // Play the explosion sound
        PlaySound((LPCSTR)IDW_SM_EXPLODE, g_hInstance, SND_ASYNC | 
          SND_RESOURCE);
          
        // Kill both sprites
        pSpriteHitter->Kill();
        pSpriteHittee->Kill();
        
        // Update the score
        UpdateScore(); 
        
        // See if any more grunts are left to destroy
        if (--g_iNumGrunts == 0)
        {
            // Start the next level
            NextLevel();
        }    
    }
    
    // Check to see if a grunt and the player have collided
    else if ((pHitter == g_pGruntBitmap && pHittee == g_pPlayerBitmap) ||
      (pHitter == g_pPlayerBitmap && pHittee == g_pGruntBitmap))
    {
        // Check the player's lives
        CheckPlayerLives();
    }
    
    // Check to see if a hulk and the player have collided
    else if ((pHitter == g_pHulkBitmap && pHittee == g_pPlayerBitmap) ||
      (pHitter == g_pPlayerBitmap && pHittee == g_pHulkBitmap))
    {
        // Check the player's lives
        CheckPlayerLives();
    }                    
    
    // Check to see if a hulk and a laser have collided
    else if ((pHitter == g_pHulkBitmap && pHittee == g_pLaserBitmap) ||
      (pHitter == g_pLaserBitmap && pHittee == g_pHulkBitmap))
    {
        // Check which sprite is the hulk
        if (pSpriteHitter->GetBitmap() == g_pHulkBitmap)
        {
            // Check the direction of the laser and move the hulk
            // slightly according to where it is hit
            if (pSpriteHittee->GetCurFrame() == 0)
            	pSpriteHitter->OffsetPosition(-5, 0);
           	else if (pSpriteHittee->GetCurFrame() == 1)
           		pSpriteHitter->OffsetPosition(-5, -5);
           	else if (pSpriteHittee->GetCurFrame() == 2)
           		pSpriteHitter->OffsetPosition(0, -5);
             else if (pSpriteHittee->GetCurFrame() == 3)
           		pSpriteHitter->OffsetPosition(5, -5);
             else if (pSpriteHittee->GetCurFrame() == 4)
           		pSpriteHitter->OffsetPosition(5, 0);
             else if (pSpriteHittee->GetCurFrame() == 5)
           		pSpriteHitter->OffsetPosition(5, 5);
             else if (pSpriteHittee->GetCurFrame() == 6)
           		pSpriteHitter->OffsetPosition(0, 5);
             else if (pSpriteHittee->GetCurFrame() == 7)
           		pSpriteHitter->OffsetPosition(-5, 5);	
           		
      		// Kill the laser sprite
      		pSpriteHittee->Kill();
        }
        else if (pSpriteHittee->GetBitmap() == g_pHulkBitmap)
        {
            // Check the direction of the laser and move the hulk
            // slightly according to where it is hit
            if (pSpriteHitter->GetCurFrame() == 0)
            	pSpriteHittee->OffsetPosition(-5, 0);
           	else if (pSpriteHitter->GetCurFrame() == 1)
           		pSpriteHittee->OffsetPosition(-5, -5);
           	else if (pSpriteHitter->GetCurFrame() == 2)
           		pSpriteHittee->OffsetPosition(0, -5);
             else if (pSpriteHitter->GetCurFrame() == 3)
           		pSpriteHittee->OffsetPosition(5, -5);
             else if (pSpriteHitter->GetCurFrame() == 4)
           		pSpriteHittee->OffsetPosition(5, 0);
             else if (pSpriteHitter->GetCurFrame() == 5)
           		pSpriteHittee->OffsetPosition(5, 5);
             else if (pSpriteHitter->GetCurFrame() == 6)
           		pSpriteHittee->OffsetPosition(0, 5);
             else if (pSpriteHitter->GetCurFrame() == 7)
           		pSpriteHittee->OffsetPosition(-5, 5);	
           		
      		// Kill the laser sprite
      		pSpriteHitter->Kill();
        }       
        else
        {
            // Do nothing
        }    
    }
    
    // Check to see whether a hulk has collided with another hulk.
    // There is nothing to be done with this collision yet.
    else if (pHitter == g_pHulkBitmap && pHittee == g_pHulkBitmap)
    {
    }
    
    // Check to see if a laser and an electrode have collided
    else if ((pHitter == g_pElectrodeBitmap && pHittee == g_pLaserBitmap) ||   
      (pHitter == g_pLaserBitmap && pHittee == g_pElectrodeBitmap))
    {
        pSpriteHitter->Kill();
        pSpriteHittee->Kill();
    }
    
    // Check to see if an electrode and the player have collided
    else if ((pHitter == g_pElectrodeBitmap && pHittee == g_pPlayerBitmap) ||
      (pHitter == g_pPlayerBitmap && pHittee == g_pLaserBitmap))
    {
        CheckPlayerLives();
    }
              
    return FALSE;
}

void SpriteDying(Sprite* pSpriteDying)
{
}

void MIDINotifying(MIDISTATE msMIDIState)
{
    if (msMIDIState == MIDI_DONE)
        g_pGame->PlayMIDISong("", TRUE);
}

//-----------------------------------------------------------------
// Functions
//-----------------------------------------------------------------

void UpdateHiScores()
{
  // See if the current score made the hi score list
  int i;
  for (i = 0; i < 5; i++)
  {
    if (g_iScore > g_iHiScores[i])
      break;
  }

  // Insert the current score into the hi score list
  if (i < 5)
  {
    for (int j = 4; j > i; j--)
    {
      g_iHiScores[j] = g_iHiScores[j - 1];
    }
    g_iHiScores[i] = g_iScore;
  }
}

BOOL ReadHiScores()
{
  // Open the hi score file (HiScores.dat)
  HANDLE hFile = CreateFile(TEXT("HiScores.dat"), GENERIC_READ, 0, NULL,
    OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
  if (hFile == INVALID_HANDLE_VALUE)
  {
    // The hi score file doesn't exist, so initialize the scores to 0
    for (int i = 0; i < 5; i++)
      g_iHiScores[i] = 0;
    return FALSE;
  }

  // Read the scores
  for (int i = 0; i < 5; i++)
  {
    // Read the score
    char  cData[6];
    DWORD dwBytesRead;
    if (!ReadFile(hFile, &cData, 5, &dwBytesRead, NULL))
    {
      // Something went wrong, so close the file handle
      CloseHandle(hFile);
      return FALSE;
    }

    // Extract each integer score from the score data
    g_iHiScores[i] = atoi(cData);
  }

  // Close the file
  return CloseHandle(hFile);
}

BOOL WriteHiScores()
{
  // Create the hi score file (HiScores.dat) for writing
  HANDLE hFile = CreateFile(TEXT("HiScores.dat"), GENERIC_WRITE, 0, NULL,
    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile == INVALID_HANDLE_VALUE)
    // The hi score file couldn't be created, so bail
    return FALSE;

  // Write the scores
  for (int i = 0; i < 5; i++)
  {
    // Format each score for writing
    CHAR cData[6];
    wsprintf(cData, "%05d", g_iHiScores[i]);

    // Write the score
    DWORD dwBytesWritten;
    if (!WriteFile(hFile, &cData, 5, &dwBytesWritten, NULL))
    {
      // Something went wrong, so close the file handle
      CloseHandle(hFile);
      return FALSE;
    }
  }

  // Close the file
  return CloseHandle(hFile);
}

void NewGame()
{
    // Initialize the game variables
    g_iScore = 0;
    g_iNumLives = 3;
    g_iLevelDelay = 60;
    g_iGameOverDelay = 150;
    g_iLevel = 0;
    g_bSplash = FALSE;
    
    // Play the game music
    g_pGame->CloseMIDIPlayer();
    g_pGame->PlayMIDISong(TEXT("Robotron.mid"), TRUE);
    
    // Go to the next level
    NextLevel();
}

void NextLevel()
{
    // Increment the level count
    g_iLevel++;
    
    // Initialize the new robot number variables
    g_iNumHulks += 1;
    g_iNumElectrodes += 2;
    g_iNumGrunts = 5 * g_iLevel;
    g_iNumMikey = g_iLevel;
    
    // The following code is checks to ensure the game does not get extremely
    // difficult. The grunts will remain unchanged, however, because that is 
    // the main source of difficulty. The hulks are limited to 10, the number
    // of Mikeys at 15, and the number of electrodes 20.
    if (g_iNumElectrodes >= 20)
        g_iNumElectrodes = 20;
    if (g_iNumGrunts >= 10)
        g_iNumElectrodes = 10;
    if (g_iNumMikey >= 15)
        g_iNumMikey = 15;
    
    // Create the robot army
    CreateRobotArmy();
}

void CreateRobotArmy()
{
    // Pause the level to give the player a little time to relax
    g_bBetweenLevels = TRUE;
    
    // Reset the player's position to the center of the game screen
    RECT rcBounds = { 50, 50, 750, 550 };
    g_pGame->CleanupSprites();
    g_pPlayerSprite = new Sprite(g_pPlayerBitmap, rcBounds);
    g_pPlayerSprite->SetPosition((g_pGame->GetWidth() - 
      g_pPlayerBitmap->GetWidth()) / 2, (g_pGame->GetHeight() - 
      g_pPlayerBitmap->GetHeight()) / 2);
    g_pGame->AddSprite(g_pPlayerSprite);
    
    // Set the person count
    g_iPersonCount = 0;
    
    // Create the grunts
    for (int i = 1; i <= g_iNumGrunts; i++)
    {
        MiscSprite* pGruntSprite = new MiscSprite(g_pGruntBitmap, rcBounds,
          g_pPlayerSprite);
        pGruntSprite->SetPosition(CreateRandomPosition());
        pGruntSprite->SetNumFrames(4);
        g_pGame->AddSprite(pGruntSprite);
    }
    // Create the hulks
    for (int i = 1; i <= g_iNumHulks; i++)
    {
      MiscSprite* pRobotSprite = new MiscSprite(g_pHulkBitmap,
          rcBounds, g_pPlayerSprite);
        POINT ptPosition = CreateRandomPosition();
        pRobotSprite->SetPosition(ptPosition);
        pRobotSprite->SetNumFrames(4);
        g_pGame->AddSprite(pRobotSprite);
    }
    // Create the electrodes
    for (int i = 1; i <= g_iNumElectrodes; i++)
    {
        MiscSprite* pElectrodeSprite = new 
          MiscSprite(g_pElectrodeBitmap, rcBounds, g_pPlayerSprite);
        pElectrodeSprite->SetPosition(CreateRandomPosition());
        pElectrodeSprite->SetCurFrame(rand() % 18);
        pElectrodeSprite->SetNumFrames(18);
        g_pGame->AddSprite(pElectrodeSprite);
    }
    // Create the Mikeys
    for (int i = 1; i <= g_iNumMikey; i++)
    {
        MiscSprite* pMikeySprite = new MiscSprite(g_pMikeyBitmap, rcBounds,
          g_pPlayerSprite);
        pMikeySprite->SetPosition(CreateRandomPosition());
        pMikeySprite->SetNumFrames(4);
        g_pGame->AddSprite(pMikeySprite);
    }
}    

POINT CreateRandomPosition()
{
    // Set a random position
    int iRandX = rand() % g_pGame->GetWidth();
    int iRandY = rand() % g_pGame->GetHeight();
        
    while (((iRandX >= 300 && iRandX <= 500) && 
      (iRandY >= 200 && iRandY <= 400)) ||
      iRandX <= 50 || iRandX >= 750 || iRandY <= 50 || iRandY >= 550)
    {
    	iRandX = rand() % g_pGame->GetWidth();
     	iRandY = rand() % g_pGame->GetHeight();
    }
    
    POINT ptPosition = { iRandX, iRandY };
    
    return ptPosition;
}

void FireLaser(int iDirection)
{
    // Check to see which direction it is, and create a laser sprite for it
    RECT rcBounds = { 50, 50, 750, 550 };
    MiscSprite* pLaser = new MiscSprite(g_pLaserBitmap, rcBounds,
      g_pPlayerSprite, BA_DIE);
    pLaser->SetPosition(g_pPlayerSprite->GetPosition().left + 
      (g_pPlayerSprite->GetBitmap()->GetWidth() / 2), 
      g_pPlayerSprite->GetPosition().top + 
      (g_pPlayerSprite->GetBitmap()->GetHeight() / 2));
    pLaser->SetNumFrames(8);
    pLaser->SetCurFrame(iDirection);
    
    // Set the velocity based on the direction. The directions start
    // at 0, going left, and rotating clockwise.
    if (iDirection == 0)
        pLaser->SetVelocity(-50, 0);
    else if (iDirection == 1)
        pLaser->SetVelocity(-50, -50);
    else if (iDirection == 2)
        pLaser->SetVelocity(0, -50);
    else if (iDirection == 3)
        pLaser->SetVelocity(50, -50);
    else if (iDirection == 4)
        pLaser->SetVelocity(50, 0);
    else if (iDirection == 5)
        pLaser->SetVelocity(50, 50);
    else if (iDirection == 6)
        pLaser->SetVelocity(0, 50);
    else if (iDirection == 7)
        pLaser->SetVelocity(-50, 50);
        
    // Add the sprite to the engine
    g_pGame->AddSprite(pLaser);
    
    // Play the laser sound
    PlaySound((LPCSTR)IDW_LASER, g_hInstance, SND_ASYNC | 
      SND_RESOURCE);
}

void UpdateScore()
{
    g_iScore += 100;
    if (g_iScore % 3000 == 0)
        g_iNumLives++;
}

void CheckPlayerLives()
{
    g_iNumLives--;
    CreateRobotArmy();
    if (g_iNumLives == 0)
        g_bGameOver = TRUE;
}
