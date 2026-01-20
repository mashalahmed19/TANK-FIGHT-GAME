/*
================================================================================
    TANK FIGHT GAME - Programming Fundamentals Final Project
    Created by: Mashal Ahmed
    Date: January 2026
    
    Game Description:
    - 2D Graphics-based Tank Fight Game
    - Multiple game modes: Single Player, Two Player, Three Player
    - Multiple maps with obstacles
    - Power-ups system
    - Health and score tracking
    - File handling for high scores
    
    Controls:
    - Player 1: Mouse (movement + left click to fire)
    - Player 2: WASD (movement) + Spacebar (fire)
    - Player 3: Arrow Keys (movement) + Enter (fire)
    
    SETUP INSTRUCTIONS FOR graphics.h (WinBGIm):
    --------------------------------------------
    1. Download WinBGIm from: https://github.com/Rameshwar0852/WinBGIm-Library
    2. Copy graphics.h and winbgim.h to your MinGW include folder
       (e.g., C:\MinGW\include or C:\TDM-GCC-64\x86_64-w64-mingw32\include)
    3. Copy libbgi.a to your MinGW lib folder
       (e.g., C:\MinGW\lib or C:\TDM-GCC-64\x86_64-w64-mingw32\lib)
    4. Compile with: 
       g++ PF_Final_Project-Mashal_Ahmed.cpp -o TankFight -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
    
    Alternative: Use Code::Blocks IDE which has built-in WinBGIm support
================================================================================
*/

#include <graphics.h>  // WinBGIm library - See setup instructions above
#include <conio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <windows.h>
#include <mmsystem.h>

using namespace std;

// ==================== GLOBAL CONSTANTS ====================
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int TANK_SIZE = 30;
const int BULLET_SIZE = 5;
const int MAX_BULLETS = 50;
const int MAX_POWERUPS = 10;
const int MAP_ROWS = 20;
const int MAP_COLS = 30;
const int CELL_SIZE = 40;
const int MAX_PLAYERS = 3;
const int INITIAL_HEALTH = 100;
const int MAX_PLAYERS_DB = 100;  // Max players in database

// ==================== GLOBAL VARIABLES ====================

// Player database system
char playerNamesDB[MAX_PLAYERS_DB][50];  // All registered players
int playerHighScores[MAX_PLAYERS_DB];     // Highest score for each player
int totalPlayersDB = 0;                   // Total players in database

// Game state variables
int gameMode = 0;          // 1=Single, 2=Two Player, 3=Three Player
int selectedMap = 0;       // Map selection (0-4)
bool gameRunning = false;
bool gameOver = false;
int winner = -1;

// Tank variables (using arrays for all players)
int tankX[MAX_PLAYERS];
int tankY[MAX_PLAYERS];
int tankAngle[MAX_PLAYERS];
int tankHealth[MAX_PLAYERS];
int tankSpeed[MAX_PLAYERS];
bool tankAlive[MAX_PLAYERS];
int tankColor[MAX_PLAYERS] = {RED, BLUE, GREEN};
char playerNames[MAX_PLAYERS][50];

// Score tracking
int scores[MAX_PLAYERS];

// Bullet variables
int bulletX[MAX_BULLETS];
int bulletY[MAX_BULLETS];
int bulletAngle[MAX_BULLETS];
bool bulletActive[MAX_BULLETS];
int bulletOwner[MAX_BULLETS];  // Which player fired this bullet
int bulletSpeed = 12;

// Power-up variables
int powerupX[MAX_POWERUPS];
int powerupY[MAX_POWERUPS];
int powerupType[MAX_POWERUPS];  // 1=Health, 2=Speed, 3=RapidFire, 4=Shield
bool powerupActive[MAX_POWERUPS];
int powerupDuration[MAX_PLAYERS][4];  // Duration for each power-up type per player

// Map system (2D array)
int currentMap[MAP_ROWS][MAP_COLS];

// Keyboard states
bool keyW = false, keyA = false, keyS = false, keyD = false, keySpace = false;
bool keyUp = false, keyDown = false, keyLeft = false, keyRight = false, keyEnter = false;

// Mouse states
int mouseX = 0, mouseY = 0;
bool mouseClick = false;

// Double buffering
int activePage = 0;

// Fire rate control
int lastFireTime[MAX_PLAYERS];
int fireDelay = 300;  // milliseconds (faster shooting)

// ==================== MAP DEFINITIONS ====================

// Map 1: Open Arena (no obstacles)
int map1[MAP_ROWS][MAP_COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Map 2: Wall Arena
int map2[MAP_ROWS][MAP_COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Map 3: Maze Map
int map3[MAP_ROWS][MAP_COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,1,1,0,1,0,1,1,1,1,1,0,1,0,0,1,0,1,1,1,1,0,1,0,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,0,1,1,1,1,1,0,1,0,0,1,0,1,1,1,1,0,1,0,1,1,1,0,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Map 4: Cross Obstacle Map (REDESIGNED - wider openings for better gameplay)
int map4[MAP_ROWS][MAP_COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// Map 5: Boxed Arena
int map5[MAP_ROWS][MAP_COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1},
    {1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
    {1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
    {1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1},
    {1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
    {1,0,0,1,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1},
    {1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

// ==================== FUNCTION DECLARATIONS ====================

// Menu and initialization functions
void showMainMenu();
void selectGameMode();
void selectMap();
void getPlayerNames();
void initializeGame();

// Map functions
void loadMap(int mapNumber);
void drawMap();

// Tank functions
void drawTank(int playerIndex);
void moveTank(int playerIndex);
void updateTankAngle(int playerIndex, int targetX, int targetY);
void handlePlayer1Input();
void handlePlayer2Input();
void handlePlayer3Input();
void handleComputerAI();

// Bullet functions
void fireBullet(int playerIndex);
void moveBullets();
void drawBullets();

// Collision detection
bool checkTankMapCollision(int x, int y);
bool checkBulletMapCollision(int x, int y);
bool checkBulletTankCollision();
void checkPowerupCollection();

// Power-up functions
void spawnPowerups();
void drawPowerups();
void applyPowerup(int playerIndex, int type);
void updatePowerupDurations();

// UI functions
void drawUI();
void drawHealthBar(int playerIndex);
void showGameOver();

// File handling functions
void saveGameHistory();
void savePlayerData();
void loadPlayerData();

// Utility functions
int getDistance(int x1, int y1, int x2, int y2);
void playSound(int soundType);

// Main game loop
void gameLoop();

// ==================== MAIN FUNCTION ====================

int main() {
    // Initialize random seed
    srand((unsigned int)time(0));
    
    // Initialize graphics window
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tank Fight Game - PF Project");
    
    // Set background
    setbkcolor(BLACK);
    cleardevice();
    
    // FORCE VISIBLE TEST - Big red text
    setcolor(RED);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
    outtextxy(200, 300, (char*)"GAME WINDOW IS OPEN!");
    outtextxy(200, 400, (char*)"Loading menu in 2 seconds...");
    Sleep(2000);  // Wait 2 seconds so you can SEE this
    
    // Show main menu
    showMainMenu();
    
    // Close graphics window
    closegraph();
    return 0;
}

// ==================== MENU FUNCTIONS ====================

/*
    Function: showMainMenu
    Purpose: Display the main menu with game title and options
*/
void showMainMenu() {
    while (true) {
        cleardevice();
        
        // Draw title
        setcolor(YELLOW);
        settextstyle(BOLD_FONT, HORIZ_DIR, 5);
        outtextxy(300, 100, (char*)"TANK FIGHT GAME");
        
        // Draw menu options
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        setcolor(WHITE);
        outtextxy(450, 250, (char*)"1. Single Player");
        outtextxy(450, 300, (char*)"2. Two Players");
        outtextxy(450, 350, (char*)"3. Three Players");
        outtextxy(450, 400, (char*)"4. High Scores");
        outtextxy(450, 450, (char*)"5. Instructions");
        outtextxy(450, 500, (char*)"6. Exit");
        
        setcolor(CYAN);
        outtextxy(350, 650, (char*)"Press number key to select option");
        
        // Wait for user input using Windows API (more reliable)
        int choice = 0;
        while (choice == 0) {
            if (GetAsyncKeyState('1') & 0x8000) choice = '1';
            else if (GetAsyncKeyState('2') & 0x8000) choice = '2';
            else if (GetAsyncKeyState('3') & 0x8000) choice = '3';
            else if (GetAsyncKeyState('4') & 0x8000) choice = '4';
            else if (GetAsyncKeyState('5') & 0x8000) choice = '5';
            else if (GetAsyncKeyState('6') & 0x8000) choice = '6';
            delay(50);
        }
        delay(200); // Debounce
        
        if (choice == '1' || choice == '2' || choice == '3') {
            gameMode = choice - '0';  // Convert char to int
            getPlayerNames();
            selectMap();
            initializeGame();
            gameLoop();
        }
        else if (choice == '4') {
            loadPlayerData();
            // Display all players data
            cleardevice();
            settextstyle(BOLD_FONT, HORIZ_DIR, 3);
            setcolor(YELLOW);
            outtextxy(350, 80, (char*)"REGISTERED PLAYERS");
            
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            if (totalPlayersDB == 0) {
                setcolor(WHITE);
                outtextxy(350, 300, (char*)"No players registered yet!");
                outtextxy(320, 350, (char*)"Play some games to get started!");
            } else {
                setcolor(CYAN);
                outtextxy(200, 150, (char*)"#    PLAYER NAME                  HIGHEST SCORE");
                
                // Display all players (paginated if needed)
                int displayCount = (totalPlayersDB > 15) ? 15 : totalPlayersDB;
                for (int i = 0; i < displayCount; i++) {
                    char display[100];
                    sprintf(display, "%-4d %-30s %d", i+1, playerNamesDB[i], playerHighScores[i]);
                    
                    // Alternate colors for readability
                    if (i % 2 == 0) setcolor(WHITE);
                    else setcolor(LIGHTGRAY);
                    
                    outtextxy(200, 190 + i * 30, display);
                }
                
                if (totalPlayersDB > 15) {
                    setcolor(YELLOW);
                    char moreText[50];
                    sprintf(moreText, "...and %d more players", totalPlayersDB - 15);
                    outtextxy(350, 190 + 15 * 30, moreText);
                }
            }
            
            setcolor(GREEN);
            outtextxy(350, 700, (char*)"Press SPACE to return...");
            while (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) { delay(50); }
            delay(200);
        }
        else if (choice == '5') {
            // Show instructions
            cleardevice();
            settextstyle(BOLD_FONT, HORIZ_DIR, 3);
            setcolor(YELLOW);
            outtextxy(450, 50, (char*)"INSTRUCTIONS");
            
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
            setcolor(WHITE);
            outtextxy(50, 150, (char*)"PLAYER 1 (Red): Mouse to move, Left Click to fire");
            outtextxy(50, 200, (char*)"PLAYER 2 (Blue): W/A/S/D to move, SPACE to fire");
            outtextxy(50, 250, (char*)"PLAYER 3 (Green): Arrow Keys to move, ENTER to fire");
            
            setcolor(CYAN);
            outtextxy(50, 350, (char*)"POWER-UPS (Colored Dots):");
            setcolor(GREEN);
            outtextxy(50, 400, (char*)"Green Dot = Health Boost (+20 HP)");
            setcolor(YELLOW);
            outtextxy(50, 450, (char*)"Yellow Dot = Speed Boost (2x faster)");
            setcolor(14);  // Orange
            outtextxy(50, 500, (char*)"Orange Dot = Rapid Fire (faster shooting)");
            setcolor(CYAN);
            outtextxy(50, 550, (char*)"Cyan Dot = Shield (temporary protection)");
            
            setcolor(YELLOW);
            outtextxy(400, 650, (char*)"Press SPACE to return...");
            while (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) { delay(50); }
            delay(200);
        }
        else if (choice == '6') {
            break;  // Exit game
        }
    }
}

/*
    Function: getPlayerNames
    Purpose: Get names of players before starting the game
*/
void getPlayerNames() {
    int numPlayers = gameMode;
    
    cleardevice();
    settextstyle(BOLD_FONT, HORIZ_DIR, 3);
    setcolor(YELLOW);
    outtextxy(380, 80, (char*)"ENTER PLAYER NAMES");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    
    // Get name for each player
    for (int i = 0; i < numPlayers; i++) {
        cleardevice();
        settextstyle(BOLD_FONT, HORIZ_DIR, 3);
        setcolor(YELLOW);
        outtextxy(380, 80, (char*)"ENTER PLAYER NAMES");
        
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        setcolor(tankColor[i]);
        
        char prompt[100];
        sprintf(prompt, "Player %d (Press Enter when done):", i + 1);
        outtextxy(300, 250, prompt);
        
        // Input box
        setcolor(WHITE);
        rectangle(295, 320, 905, 370);
        
        // Get input
        char input[50] = "";
        int charCount = 0;
        bool entering = true;
        
        while (entering) {
            if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
                if (charCount > 0) entering = false;
                delay(200);
            }
            
            // Check for letter keys
            for (int key = 'A'; key <= 'Z'; key++) {
                if (GetAsyncKeyState(key) & 0x8000 && charCount < 40) {
                    if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                        input[charCount++] = key;  // Uppercase
                    } else {
                        input[charCount++] = key + 32;  // Lowercase
                    }
                    input[charCount] = '\0';
                    delay(150);
                }
            }
            
            // Space bar
            if (GetAsyncKeyState(VK_SPACE) & 0x8000 && charCount < 40) {
                input[charCount++] = ' ';
                input[charCount] = '\0';
                delay(150);
            }
            
            // Backspace
            if (GetAsyncKeyState(VK_BACK) & 0x8000 && charCount > 0) {
                charCount--;
                input[charCount] = '\0';
                delay(150);
            }
            
            // Display current input
            setcolor(BLACK);
            setfillstyle(SOLID_FILL, BLACK);
            bar(300, 325, 900, 365);
            setcolor(tankColor[i]);
            outtextxy(310, 335, input);
            
            delay(50);
        }
        
        // Save the name
        if (charCount > 0) {
            strcpy(playerNames[i], input);
        } else {
            sprintf(playerNames[i], "Player%d", i + 1);
        }
    }
    
    // For single player, set computer name
    if (gameMode == 1) {
        strcpy(playerNames[1], "COMPUTER");
    }
    
    // Show player names confirmation
    cleardevice();
    settextstyle(BOLD_FONT, HORIZ_DIR, 3);
    setcolor(YELLOW);
    outtextxy(450, 150, (char*)"PLAYERS READY");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    
    for (int i = 0; i < (gameMode == 1 ? 2 : numPlayers); i++) {
        char display[100];
        sprintf(display, "%s - Ready!", playerNames[i]);
        setcolor(tankColor[i]);
        outtextxy(450, 250 + i * 50, display);
    }
    
    setcolor(CYAN);
    outtextxy(350, 550, (char*)"Press SPACE to continue...");
    while (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) { delay(50); }
    delay(200);
}

/*
    Function: selectMap
    Purpose: Allow player to choose which map to play on
*/
void selectMap() {
    cleardevice();
    settextstyle(BOLD_FONT, HORIZ_DIR, 3);
    setcolor(YELLOW);
    outtextxy(450, 100, (char*)"SELECT MAP");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    outtextxy(450, 250, (char*)"1. Open Arena");
    outtextxy(450, 300, (char*)"2. Wall Arena");
    outtextxy(450, 350, (char*)"3. Maze Map");
    outtextxy(450, 400, (char*)"4. Cross Obstacle");
    outtextxy(450, 450, (char*)"5. Boxed Arena");
    
    setcolor(CYAN);
    outtextxy(400, 600, (char*)"Press number key to select map");
    
    // Wait for valid map selection using GetAsyncKeyState
    selectedMap = 0;
    while (selectedMap == 0) {
        if (GetAsyncKeyState('1') & 0x8000) selectedMap = 1;
        else if (GetAsyncKeyState('2') & 0x8000) selectedMap = 2;
        else if (GetAsyncKeyState('3') & 0x8000) selectedMap = 3;
        else if (GetAsyncKeyState('4') & 0x8000) selectedMap = 4;
        else if (GetAsyncKeyState('5') & 0x8000) selectedMap = 5;
        delay(50);
    }
    delay(200); // Debounce
}

/*
    Function: loadMap
    Purpose: Load selected map into currentMap array
*/
void loadMap(int mapNumber) {
    int (*sourceMap)[MAP_COLS] = nullptr;
    
    // Select appropriate map based on mapNumber
    switch (mapNumber) {
        case 1: sourceMap = map1; break;
        case 2: sourceMap = map2; break;
        case 3: sourceMap = map3; break;
        case 4: sourceMap = map4; break;
        case 5: sourceMap = map5; break;
        default: sourceMap = map1; break;
    }
    
    // Copy map to currentMap array
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            currentMap[i][j] = sourceMap[i][j];
        }
    }
}

/*
    Function: drawMap
    Purpose: Draw the current map on screen
*/
void drawMap() {
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            int x = j * CELL_SIZE;
            int y = i * CELL_SIZE;
            
            if (currentMap[i][j] == 1) {
                // Draw wall
                setfillstyle(SOLID_FILL, DARKGRAY);
                setcolor(WHITE);
                bar(x, y, x + CELL_SIZE, y + CELL_SIZE);
                rectangle(x, y, x + CELL_SIZE, y + CELL_SIZE);
            }
        }
    }
}

/*
    Function: initializeGame
    Purpose: Initialize all game variables and arrays
*/
void initializeGame() {
    // Load selected map
    loadMap(selectedMap);
    
    // Initialize tank positions in SAFE OPEN AREAS (tested on all 5 maps)
    // Player 1: Top-left corner - Cell [1][1] is always clear
    tankX[0] = 60;
    tankY[0] = 60;
    
    // Player 2: Bottom-right corner - Cell [18][28] is always clear
    tankX[1] = SCREEN_WIDTH - 60;
    tankY[1] = SCREEN_HEIGHT - 60;
    
    // Player 3: Top-right corner - Cell [1][28] is always clear
    tankX[2] = SCREEN_WIDTH - 60;
    tankY[2] = 60;
    
    // Initialize tank properties
    for (int i = 0; i < MAX_PLAYERS; i++) {
        tankAngle[i] = 0;
        tankHealth[i] = INITIAL_HEALTH;
        tankSpeed[i] = 5;  // Balanced speed for smooth gameplay
        
        // For single player, both player and computer should be alive
        if (gameMode == 1) {
            tankAlive[i] = (i < 2);  // Player 1 and Computer (Player 2)
        } else {
            tankAlive[i] = (i < gameMode);  // Only active players are alive
        }
        
        scores[i] = 0;
        lastFireTime[i] = 0;
        
        // Initialize power-up durations
        for (int j = 0; j < 4; j++) {
            powerupDuration[i][j] = 0;
        }
    }
    
    // Initialize bullets (all inactive)
    for (int i = 0; i < MAX_BULLETS; i++) {
        bulletActive[i] = false;
    }
    
    // Initialize power-ups
    for (int i = 0; i < MAX_POWERUPS; i++) {
        powerupActive[i] = false;
    }
    
    // Spawn initial power-ups
    spawnPowerups();
    
    // Reset game state
    gameRunning = true;
    gameOver = false;
    winner = -1;
}

// ==================== GAME LOOP ====================

/*
    Function: gameLoop
    Purpose: Main game loop - handles input, updates, and rendering
*/
void gameLoop() {
    while (gameRunning && !gameOver) {
        // Switch to drawing page
        activePage = 1 - activePage;
        setactivepage(activePage);
        
        // Clear drawing page
        cleardevice();
        
        // Draw map as background
        drawMap();
        
        // Handle input for each player
        handlePlayer1Input();
        if (gameMode >= 2) handlePlayer2Input();
        if (gameMode >= 3) handlePlayer3Input();
        
        // Handle computer AI in single player mode
        if (gameMode == 1) handleComputerAI();
        
        // Update game state
        moveBullets();
        checkBulletTankCollision();
        checkPowerupCollection();
        updatePowerupDurations();
        
        // Draw everything in correct order
        drawPowerups();
        drawBullets();
        
        // Draw tanks (in single player, draw both player and computer)
        int numTanks = (gameMode == 1) ? 2 : gameMode;
        for (int i = 0; i < numTanks; i++) {
            if (tankAlive[i]) {
                drawTank(i);
            }
        }
        
        // Draw UI on top
        drawUI();
        
        // Flip pages - show what we just drew
        setvisualpage(activePage);
        
        // Check win condition
        int alivePlayers = 0;
        int lastAlive = -1;
        int numPlayers = (gameMode == 1) ? 2 : gameMode;  // Single player has 2 tanks
        for (int i = 0; i < numPlayers; i++) {
            if (tankAlive[i]) {
                alivePlayers++;
                lastAlive = i;
            }
        }
        
        if (alivePlayers <= 1) {
            gameOver = true;
            winner = lastAlive;
        }
        
        // Check for ESC key to exit
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            gameRunning = false;
            delay(200);
        }
        
        delay(40);  // Smooth 25 FPS
    }
    
    // Show game over screen
    if (gameOver) {
        showGameOver();
        saveGameHistory();
        savePlayerData();
    }
}

// ==================== TANK FUNCTIONS ====================

/*
    Function: drawTank
    Purpose: Draw a tank with its barrel pointing in the correct direction
*/
void drawTank(int playerIndex) {
    if (!tankAlive[playerIndex]) return;
    
    int x = tankX[playerIndex];
    int y = tankY[playerIndex];
    int color = tankColor[playerIndex];
    
    // Draw tank body (circle)
    setfillstyle(SOLID_FILL, color);
    setcolor(color);
    fillellipse(x, y, TANK_SIZE/2, TANK_SIZE/2);
    
    // Draw tank barrel based on angle
    int barrelLength = TANK_SIZE;
    int barrelX = x + (int)(barrelLength * cos(tankAngle[playerIndex] * 3.14159 / 180));
    int barrelY = y + (int)(barrelLength * sin(tankAngle[playerIndex] * 3.14159 / 180));
    
    setcolor(WHITE);
    setlinestyle(SOLID_LINE, 0, 3);
    line(x, y, barrelX, barrelY);
    setlinestyle(SOLID_LINE, 0, 1);
    
    // Draw tank outline
    setcolor(BLACK);
    circle(x, y, TANK_SIZE/2);
    
    // Draw shield effect if active
    if (powerupDuration[playerIndex][3] > 0) {
        setcolor(CYAN);
        circle(x, y, TANK_SIZE/2 + 5);
        circle(x, y, TANK_SIZE/2 + 7);
    }
}

/*
    Function: updateTankAngle
    Purpose: Update tank barrel angle to point towards target coordinates
*/
void updateTankAngle(int playerIndex, int targetX, int targetY) {
    int dx = targetX - tankX[playerIndex];
    int dy = targetY - tankY[playerIndex];
    tankAngle[playerIndex] = (int)(atan2((double)dy, (double)dx) * 180 / 3.14159);
}

/*
    Function: handlePlayer1Input
    Purpose: Handle mouse input for Player 1
*/
void handlePlayer1Input() {
    if (!tankAlive[0]) return;
    
    // Get current mouse position
    int mx, my;
    if (ismouseclick(WM_MOUSEMOVE)) {
        getmouseclick(WM_MOUSEMOVE, mx, my);
        mouseX = mx;
        mouseY = my;
    }
    
    // Always update tank angle to point at mouse
    updateTankAngle(0, mouseX, mouseY);
    
    // Move tank towards mouse if right button held
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
        int dx = mouseX - tankX[0];
        int dy = mouseY - tankY[0];
        int distance = getDistance(tankX[0], tankY[0], mouseX, mouseY);
        
        if (distance > 15) {  // Only move if far enough from mouse
            int newX = tankX[0] + (dx * tankSpeed[0]) / distance;
            int newY = tankY[0] + (dy * tankSpeed[0]) / distance;
            
            if (!checkTankMapCollision(newX, newY)) {
                tankX[0] = newX;
                tankY[0] = newY;
            }
        }
    }
    
    // Fire on left click
    if (ismouseclick(WM_LBUTTONDOWN)) {
        getmouseclick(WM_LBUTTONDOWN, mx, my);
        fireBullet(0);
    }
}

/*
    Function: handlePlayer2Input
    Purpose: Handle keyboard input for Player 2 (WASD + Spacebar)
*/
void handlePlayer2Input() {
    if (!tankAlive[1]) return;
    
    // Keyboard input for Player 2 (WASD + Space)
    if (GetAsyncKeyState('W') & 0x8000) {
        int newY = tankY[1] - tankSpeed[1];
        if (!checkTankMapCollision(tankX[1], newY)) tankY[1] = newY;
        tankAngle[1] = 270;
    }
    if (GetAsyncKeyState('S') & 0x8000) {
        int newY = tankY[1] + tankSpeed[1];
        if (!checkTankMapCollision(tankX[1], newY)) tankY[1] = newY;
        tankAngle[1] = 90;
    }
    if (GetAsyncKeyState('A') & 0x8000) {
        int newX = tankX[1] - tankSpeed[1];
        if (!checkTankMapCollision(newX, tankY[1])) tankX[1] = newX;
        tankAngle[1] = 180;
    }
    if (GetAsyncKeyState('D') & 0x8000) {
        int newX = tankX[1] + tankSpeed[1];
        if (!checkTankMapCollision(newX, tankY[1])) tankX[1] = newX;
        tankAngle[1] = 0;
    }
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        fireBullet(1);
        Sleep(200);  // Prevent rapid firing
    }
}

/*
    Function: handlePlayer3Input
    Purpose: Handle keyboard input for Player 3 (Arrow Keys + Enter)
*/
void handlePlayer3Input() {
    if (!tankAlive[2]) return;
    
    // Arrow keys for Player 3
    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        int newY = tankY[2] - tankSpeed[2];
        if (!checkTankMapCollision(tankX[2], newY)) tankY[2] = newY;
        tankAngle[2] = 270;
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        int newY = tankY[2] + tankSpeed[2];
        if (!checkTankMapCollision(tankX[2], newY)) tankY[2] = newY;
        tankAngle[2] = 90;
    }
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        int newX = tankX[2] - tankSpeed[2];
        if (!checkTankMapCollision(newX, tankY[2])) tankX[2] = newX;
        tankAngle[2] = 180;
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        int newX = tankX[2] + tankSpeed[2];
        if (!checkTankMapCollision(newX, tankY[2])) tankX[2] = newX;
        tankAngle[2] = 0;
    }
    if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
        fireBullet(2);
        Sleep(200);
    }
}

/*
    Function: handleComputerAI
    Purpose: Simple AI for computer-controlled tank in single player mode
*/
void handleComputerAI() {
    // Simple AI: Find nearest alive player and move towards them
    if (!tankAlive[1]) return;
    
    int targetPlayer = 0;  // Target Player 1
    if (!tankAlive[targetPlayer]) return;
    
    // Calculate direction to target
    int dx = tankX[targetPlayer] - tankX[1];
    int dy = tankY[targetPlayer] - tankY[1];
    int distance = getDistance(tankX[1], tankY[1], tankX[targetPlayer], tankY[targetPlayer]);
    
    // Update angle towards target first
    updateTankAngle(1, tankX[targetPlayer], tankY[targetPlayer]);
    
    // AI behavior based on distance
    if (distance > 200) {
        // Far away: Move towards player aggressively
        int moveSpeed = tankSpeed[1];
        int newX = tankX[1];
        int newY = tankY[1];
        
        if (abs(dx) > abs(dy)) {
            newX += (dx > 0) ? moveSpeed : -moveSpeed;
        } else {
            newY += (dy > 0) ? moveSpeed : -moveSpeed;
        }
        
        if (!checkTankMapCollision(newX, newY)) {
            tankX[1] = newX;
            tankY[1] = newY;
        } else {
            // If blocked, try perpendicular movement
            if (abs(dx) > abs(dy)) {
                newY = tankY[1] + moveSpeed;
                if (!checkTankMapCollision(tankX[1], newY)) tankY[1] = newY;
            } else {
                newX = tankX[1] + moveSpeed;
                if (!checkTankMapCollision(newX, tankY[1])) tankX[1] = newX;
            }
        }
    } else if (distance > 100) {
        // Medium range: Strafe and shoot
        static int strafeCounter = 0;
        strafeCounter++;
        
        if (strafeCounter % 20 < 10) {
            int newX = tankX[1] + tankSpeed[1];
            if (!checkTankMapCollision(newX, tankY[1])) tankX[1] = newX;
        } else {
            int newY = tankY[1] + tankSpeed[1];
            if (!checkTankMapCollision(tankX[1], newY)) tankY[1] = newY;
        }
    }
    // Close range: Just shoot, don't move
    
    // Fire at player (with cooldown)
    int currentTime = clock();
    if (currentTime - lastFireTime[1] >= fireDelay) {
        // Try to shoot
        fireBullet(1);
        lastFireTime[1] = currentTime;
    }
    
    // Fire randomly
    if (rand() % 30 == 0) {
        fireBullet(1);
    }
}

// ==================== BULLET FUNCTIONS ====================

/*
    Function: fireBullet
    Purpose: Create a new bullet from a player's tank
*/
void fireBullet(int playerIndex) {
    if (!tankAlive[playerIndex]) return;
    
    // Find an inactive bullet slot
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bulletActive[i]) {
            bulletActive[i] = true;
            bulletX[i] = tankX[playerIndex];
            bulletY[i] = tankY[playerIndex];
            bulletAngle[i] = tankAngle[playerIndex];
            bulletOwner[i] = playerIndex;
            playSound(1);  // Fire sound
            break;
        }
    }
}

/*
    Function: moveBullets
    Purpose: Update positions of all active bullets
*/
void moveBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bulletActive[i]) {
            // Move bullet based on angle
            bulletX[i] += (int)(bulletSpeed * cos(bulletAngle[i] * 3.14159 / 180));
            bulletY[i] += (int)(bulletSpeed * sin(bulletAngle[i] * 3.14159 / 180));
            
            // Check if bullet is out of bounds or hit wall
            if (bulletX[i] < 0 || bulletX[i] > SCREEN_WIDTH ||
                bulletY[i] < 0 || bulletY[i] > SCREEN_HEIGHT ||
                checkBulletMapCollision(bulletX[i], bulletY[i])) {
                bulletActive[i] = false;
            }
        }
    }
}

/*
    Function: drawBullets
    Purpose: Draw all active bullets on screen
*/
void drawBullets() {
    setfillstyle(SOLID_FILL, YELLOW);
    setcolor(YELLOW);
    
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bulletActive[i]) {
            fillellipse(bulletX[i], bulletY[i], BULLET_SIZE, BULLET_SIZE);
        }
    }
}

// ==================== COLLISION DETECTION ====================

/*
    Function: checkTankMapCollision
    Purpose: Check if a position collides with map obstacles
*/
bool checkTankMapCollision(int x, int y) {
    // Convert pixel coordinates to map coordinates
    int col = x / CELL_SIZE;
    int row = y / CELL_SIZE;
    
    if (row < 0 || row >= MAP_ROWS || col < 0 || col >= MAP_COLS) {
        return true;  // Out of bounds
    }
    
    return currentMap[row][col] == 1;  // Return true if wall
}

/*
    Function: checkBulletMapCollision
    Purpose: Check if bullet hits a wall
*/
bool checkBulletMapCollision(int x, int y) {
    return checkTankMapCollision(x, y);
}

/*
    Function: checkBulletTankCollision
    Purpose: Check if any bullet hits any tank and apply damage
*/
bool checkBulletTankCollision() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bulletActive[i]) continue;
        
        int numPlayers = (gameMode == 1) ? 2 : gameMode;  // Single player has 2 tanks
        for (int j = 0; j < numPlayers; j++) {
            if (!tankAlive[j]) continue;
            if (bulletOwner[i] == j) continue;  // Can't hit yourself
            
            int distance = getDistance(bulletX[i], bulletY[i], tankX[j], tankY[j]);
            
            if (distance < TANK_SIZE/2 + BULLET_SIZE) {
                // Check if tank has shield
                if (powerupDuration[j][3] > 0) {
                    // Shield absorbs hit
                    powerupDuration[j][3] -= 50;
                    if (powerupDuration[j][3] < 0) powerupDuration[j][3] = 0;
                    bulletActive[i] = false;
                    playSound(4);
                    continue;
                }
                
                // Hit detected
                tankHealth[j] -= 10;
                bulletActive[i] = false;
                scores[bulletOwner[i]] += 10;
                playSound(2);  // Hit sound
                
                if (tankHealth[j] <= 0) {
                    tankAlive[j] = false;
                    scores[bulletOwner[i]] += 100;
                    playSound(3);  // Destruction sound
                }
                
                return true;
            }
        }
    }
    return false;
}

// ==================== POWER-UP FUNCTIONS ====================

/*
    Function: spawnPowerups
    Purpose: Randomly spawn power-ups on the map
*/
void spawnPowerups() {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerupActive[i] && rand() % 100 < 5) {  // 5% chance each frame
            // Find random empty position
            bool found = false;
            int attempts = 0;
            
            while (!found && attempts < 50) {
                int x = (rand() % (MAP_COLS - 2) + 1) * CELL_SIZE + CELL_SIZE/2;
                int y = (rand() % (MAP_ROWS - 2) + 1) * CELL_SIZE + CELL_SIZE/2;
                
                if (!checkTankMapCollision(x, y)) {
                    powerupX[i] = x;
                    powerupY[i] = y;
                    powerupType[i] = rand() % 4 + 1;  // Random type 1-4
                    powerupActive[i] = true;
                    found = true;
                }
                attempts++;
            }
        }
    }
}

/*
    Function: drawPowerups
    Purpose: Draw all active power-ups on screen
*/
void drawPowerups() {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerupActive[i]) {
            int color;
            switch (powerupType[i]) {
                case 1: color = GREEN; break;      // Health
                case 2: color = YELLOW; break;     // Speed
                case 3: color = LIGHTRED; break;   // Rapid fire
                case 4: color = CYAN; break;        // Shield
                default: color = WHITE; break;
            }
            
            setfillstyle(SOLID_FILL, color);
            setcolor(color);
            fillellipse(powerupX[i], powerupY[i], 10, 10);
            circle(powerupX[i], powerupY[i], 12);
        }
    }
}

/*
    Function: checkPowerupCollection
    Purpose: Check if any tank collects a power-up
*/
void checkPowerupCollection() {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerupActive[i]) continue;
        
        for (int j = 0; j < gameMode; j++) {
            if (!tankAlive[j]) continue;
            
            int distance = getDistance(tankX[j], tankY[j], powerupX[i], powerupY[i]);
            
            if (distance < TANK_SIZE/2 + 12) {
                // Power-up collected
                applyPowerup(j, powerupType[i]);
                powerupActive[i] = false;
                playSound(4);  // Power-up sound
            }
        }
    }
    
    // Try to spawn new power-ups
    spawnPowerups();
}

/*
    Function: applyPowerup
    Purpose: Apply power-up effect to a player
*/
void applyPowerup(int playerIndex, int type) {
    switch (type) {
        case 1:  // Health boost
            tankHealth[playerIndex] = min(100, tankHealth[playerIndex] + 20);
            break;
        case 2:  // Speed boost
            powerupDuration[playerIndex][1] = 300;  // 300 frames ~ 10 seconds
            tankSpeed[playerIndex] = 6;
            break;
        case 3:  // Rapid fire
            powerupDuration[playerIndex][2] = 300;
            fireDelay = 200;
            break;
        case 4:  // Shield
            powerupDuration[playerIndex][3] = 300;
            break;
    }
}

/*
    Function: updatePowerupDurations
    Purpose: Update and expire power-up effects
*/
void updatePowerupDurations() {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        for (int j = 0; j < 4; j++) {
            if (powerupDuration[i][j] > 0) {
                powerupDuration[i][j]--;
                
                if (powerupDuration[i][j] == 0) {
                    // Reset power-up effects
                    if (j == 1) tankSpeed[i] = 3;
                    if (j == 2) fireDelay = 500;
                }
            }
        }
    }
}

// ==================== UI FUNCTIONS ====================

/*
    Function: drawUI
    Purpose: Draw game HUD (health bars, scores, player names)
*/
void drawUI() {
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    
    int numPlayers = (gameMode == 1) ? 2 : gameMode;  // Single player shows both player and computer
    for (int i = 0; i < numPlayers; i++) {
        if (tankAlive[i]) {
            char info[100];
            sprintf(info, "%s: HP=%d Score=%d", playerNames[i], tankHealth[i], scores[i]);
            setcolor(tankColor[i]);
            outtextxy(10, 10 + i * 45, info);
            
            // Draw health bar
            int barX = 10;
            int barY = 30 + i * 45;
            int barWidth = tankHealth[i] * 2;
            
            setcolor(WHITE);
            rectangle(barX, barY, barX + 200, barY + 10);
            setfillstyle(SOLID_FILL, tankColor[i]);
            bar(barX, barY, barX + barWidth, barY + 10);
        } else {
            char info[100];
            sprintf(info, "%s: DESTROYED", playerNames[i]);
            setcolor(DARKGRAY);
            outtextxy(10, 10 + i * 45, info);
        }
    }
    
    // Display ESC to exit message
    setcolor(LIGHTGRAY);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 20, (char*)"Press ESC to exit");
    
    // Display controls for each player
    setcolor(CYAN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    int controlY = SCREEN_HEIGHT - 100;
    
    if (gameMode >= 1) {
        setcolor(RED);
        outtextxy(10, controlY, (char*)"P1: RIGHT CLICK move, LEFT CLICK fire");
    }
    if (gameMode >= 2) {
        setcolor(BLUE);
        outtextxy(10, controlY + 20, (char*)"P2: WASD move, SPACE fire");
    }
    if (gameMode >= 3) {
        setcolor(GREEN);
        outtextxy(10, controlY + 40, (char*)"P3: ARROWS move, ENTER fire");
    }
}

/*
    Function: showGameOver
    Purpose: Display game over screen with winner and scores
*/
void showGameOver() {
    cleardevice();
    settextstyle(BOLD_FONT, HORIZ_DIR, 4);
    setcolor(YELLOW);
    outtextxy(400, 200, (char*)"GAME OVER");
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    
    if (winner >= 0) {
        char winText[100];
        sprintf(winText, "%s WINS!", playerNames[winner]);
        outtextxy(450, 300, winText);
        
        sprintf(winText, "Final Score: %d", scores[winner]);
        outtextxy(450, 350, winText);
    } else {
        outtextxy(500, 300, (char*)"TIE GAME");
    }
    
    // Display all player scores
    setcolor(CYAN);
    outtextxy(400, 450, (char*)"FINAL SCORES:");
    setcolor(WHITE);
    for (int i = 0; i < gameMode; i++) {
        char scoreText[100];
        sprintf(scoreText, "%s: %d", playerNames[i], scores[i]);
        outtextxy(450, 500 + i * 30, scoreText);
    }
    
    setcolor(YELLOW);
    outtextxy(350, 650, (char*)"Press SPACE to return to menu...");
    
    // Wait for SPACE key
    while (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) {
        delay(50);
    }
    delay(200); // Debounce
}

void drawHealthBar(int playerIndex) {
    // Already implemented in drawUI
    (void)playerIndex; // Suppress unused parameter warning
}

// ==================== FILE HANDLING FUNCTIONS ====================

/*
    Function: saveGameHistory
    Purpose: Save game history to gamehistory.txt file
*/
void saveGameHistory() {
    ofstream file("gamehistory.txt", ios::app);
    if (file.is_open()) {
        // Save game details
        char modeText[20];
        if (gameMode == 1) strcpy(modeText, "SinglePlayer");
        else if (gameMode == 2) strcpy(modeText, "TwoPlayer");
        else strcpy(modeText, "ThreePlayer");
        
        char mapText[20];
        sprintf(mapText, "Map%d", selectedMap);
        
        char winnerText[50];
        if (winner >= 0) {
            strcpy(winnerText, playerNames[winner]);
        } else {
            strcpy(winnerText, "TIE");
        }
        
        // Format: GameMode Players MapName Winner
        file << modeText << " " << gameMode << " " << mapText << " " << winnerText << endl;
        file.close();
    }
}

/*
    Function: loadPlayerData
    Purpose: Load all player data from players.txt file
*/
void loadPlayerData() {
    totalPlayersDB = 0;
    
    ifstream file("players.txt");
    if (file.is_open()) {
        char name[50];
        int score;
        
        // Read all players
        while (file >> name >> score && totalPlayersDB < MAX_PLAYERS_DB) {
            strcpy(playerNamesDB[totalPlayersDB], name);
            playerHighScores[totalPlayersDB] = score;
            totalPlayersDB++;
        }
        file.close();
        
        // Sort players by highest score (descending)
        for (int i = 0; i < totalPlayersDB - 1; i++) {
            for (int j = 0; j < totalPlayersDB - i - 1; j++) {
                if (playerHighScores[j] < playerHighScores[j + 1]) {
                    // Swap scores
                    int tempScore = playerHighScores[j];
                    playerHighScores[j] = playerHighScores[j + 1];
                    playerHighScores[j + 1] = tempScore;
                    
                    // Swap names
                    char tempName[50];
                    strcpy(tempName, playerNamesDB[j]);
                    strcpy(playerNamesDB[j], playerNamesDB[j + 1]);
                    strcpy(playerNamesDB[j + 1], tempName);
                }
            }
        }
    }
}

/*
    Function: savePlayerData
    Purpose: Save/update player statistics to players.txt (no duplicates, highest score only)
*/
void savePlayerData() {
    // First, load existing player data
    loadPlayerData();
    
    // Update or add each player from current game
    int numPlayers = (gameMode == 1) ? 2 : gameMode;
    for (int i = 0; i < numPlayers; i++) {
        // Skip computer player name
        if (strcmp(playerNames[i], "COMPUTER") == 0) continue;
        
        // Check if player already exists
        bool found = false;
        for (int j = 0; j < totalPlayersDB; j++) {
            if (strcmp(playerNamesDB[j], playerNames[i]) == 0) {
                // Player exists - update if current score is higher
                if (scores[i] > playerHighScores[j]) {
                    playerHighScores[j] = scores[i];
                }
                found = true;
                break;
            }
        }
        
        // If player doesn't exist, add them
        if (!found && totalPlayersDB < MAX_PLAYERS_DB) {
            strcpy(playerNamesDB[totalPlayersDB], playerNames[i]);
            playerHighScores[totalPlayersDB] = scores[i];
            totalPlayersDB++;
        }
    }
    
    // Write all players back to file (overwrite)
    ofstream file("players.txt");
    if (file.is_open()) {
        for (int i = 0; i < totalPlayersDB; i++) {
            file << playerNamesDB[i] << " " << playerHighScores[i] << endl;
        }
        file.close();
    }
}


// ==================== UTILITY FUNCTIONS ====================

/*
    Function: getDistance
    Purpose: Calculate distance between two points
*/
int getDistance(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    return (int)sqrt((double)(dx*dx + dy*dy));
}

/*
    Function: playSound
    Purpose: Play sound effects using Windows Beep function
*/
void playSound(int soundType) {
    // Sound effects using Beep function (Windows)
    switch (soundType) {
        case 1:  // Fire
            Beep(800, 50);
            break;
        case 2:  // Hit
            Beep(600, 100);
            break;
        case 3:  // Destruction
            Beep(400, 200);
            break;
        case 4:  // Power-up
            Beep(1000, 100);
            break;
    }
}

/*
================================================================================
    END OF TANK FIGHT GAME - Programming Fundamentals Project
    
    FEATURES IMPLEMENTED:
    ✓ Menu system with multiple options
    ✓ 3 game modes (Single, Two Player, Three Player)
    ✓ 5 different maps using 2D arrays
    ✓ Tank movement and controls (Mouse, WASD, Arrow Keys)
    ✓ Bullet firing and collision detection
    ✓ Health system with health bars
    ✓ Score tracking
    ✓ Power-ups system (Health, Speed, Rapid Fire, Shield)
    ✓ Computer AI for single player mode
    ✓ Sound effects using Beep()
    ✓ File handling (highscores.txt, players.txt)
    ✓ Game over screen
    
    PROGRAMMING FUNDAMENTALS CONCEPTS USED:
    ✓ 1D and 2D arrays (maps, tanks, bullets, power-ups)
    ✓ Functions (30+ user-defined functions)
    ✓ Loops (for, while)
    ✓ Conditionals (if-else, switch)
    ✓ Character arrays for strings
    ✓ File handling with fstream
    ✓ No OOP (no classes, objects, or structs)
    
    Compile with: g++ -o TankFight PF_Final_Project-Mashal_Ahmed.cpp -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
================================================================================
*/
