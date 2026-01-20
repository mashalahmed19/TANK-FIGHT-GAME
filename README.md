Tank Fight Game (PF Final Project)
1. Project Overview
Tank Fight Game is a 2D graphics-based shooting game developed in C++ using Programming Fundamentals concepts only. The game supports 1 Player vs Computer, 2 Players (1v1), and 3 Players (1v1v1) modes. Players control tanks on different maps, fire bullets, avoid obstacles, and try to achieve the highest score.
This project strictly avoids Object-Oriented Programming (OOP). No classes, objects, or structures are used. All logic is implemented using arrays, functions, loops, conditionals, strings, file handling, and graphics functions.
________________________________________
2. Objectives of the Project
•	To demonstrate strong understanding of arrays (1D & 2D)
•	To use user-defined and built-in functions effectively
•	To implement graphics, keyboard handling, timing, and collision detection
•	To store and retrieve data using text files (.txt)
•	To design a complete game without using OOP concepts
________________________________________
3. Tools & Technologies Used
•	Language: C++
•	IDE: Visual Studio Code
•	Graphics Library: graphics.h (WinBGIm)
•	Input Handling: getch(), kbhit()
•	Timing: delay()
•	File Handling: fstream (.txt files)
________________________________________
4. Game Modes
4.1 Single Player (1 Player vs Computer)
•	Player fights against a computer-controlled tank
•	Computer tank movement and firing are randomized
•	Player score increases by hitting the computer tank
4.2 Two Players (1v1)
•	Two human players play against each other
•	Each player has independent controls
•	The player with the higher score wins
4.3 Three Players (1v1v1)
•	Three human players play simultaneously
•	All tanks move and fire independently
•	Last surviving player or highest scorer wins
________________________________________
5. Maps System
The game includes three different maps, selectable in every mode.
Map Types:
1.	Open Arena Map – No obstacles
2.	Obstacle Map – Walls placed using 2D arrays
3.	Maze Map – Complex paths and barriers
Maps are implemented using 2D arrays, where each cell represents: - Empty space - Wall - Tank position
________________________________________
6. Game Controls
Player 1 Controls
•	Move: W, A, S, D
•	Fire: Spacebar
Player 2 Controls
•	Move: Arrow Keys
•	Fire: Enter
Player 3 Controls
•	Move: I, J, K, L
•	Fire: M
________________________________________
7. Data Structures Used
7.1 Arrays
•	Tank positions: int tankX[3], tankY[3]
•	Tank health: int health[3]
•	Scores: int score[3]
•	Bullet positions: int bulletX[20], bulletY[20]
•	Map layout: int map[20][20]
•	Player names: char playerName[3][30]
________________________________________
8. Functions Used
Core Functions
•	main() – Game entry point
•	menu() – Displays main menu
•	selectMode() – Selects game mode
•	selectMap() – Map selection
Graphics & Drawing
•	drawMap()
•	drawTank(int id)
•	drawBullet()
Game Logic
•	moveTank(int id, char key)
•	fireBullet(int id)
•	moveBullet()
•	checkCollision()
•	updateScore()
File Handling
•	loadHighScores()
•	saveHighScores()
•	savePlayerData()
________________________________________
9. File Handling System
Files Used
•	highscores.txt – Stores top 3 scores with names
•	players.txt – Stores each player’s highest score
Stored Data
•	Player Name
•	Highest Score
•	Game Mode
•	Date (optional)
File handling is implemented using fstream without structs.
________________________________________
10. Scoring System
•	Each successful hit increases score
•	Destroying a tank gives bonus points
•	Scores are updated in real time
At game end: - Player score is compared with existing records - Top 3 scores are saved permanently
________________________________________
11. Game Over Conditions
•	Tank health reaches zero
•	Only one tank remains alive
•	Player exits the game
________________________________________
12. Error Handling & Validation
•	Boundary checking for tank movement
•	Bullet collision validation
•	File existence check
•	Input validation for menu options
________________________________________
13. Limitations
•	No sound effects
•	No advanced AI for computer tank
•	Graphics limited to 2D shapes
________________________________________
14. Conclusion
The Tank Fight Game is a complete and challenging Programming Fundamentals project. It effectively demonstrates the use of arrays, functions, graphics, file handling, and game logic, while strictly following PF rules. The project is scalable, easy to explain in viva, and visually engaging.
________________________________________
15. Advanced Features Added
15.1 Sound Effects
Sound effects are added to enhance user experience. Simple sound files are played for: - Tank firing - Bullet collision - Tank destruction - Power-up collection
Sound is implemented using basic sound functions compatible with C++ (such as PlaySound()), without using any external game engine or OOP concepts.
________________________________________
15.2 Power-Ups System
Power-ups appear randomly on the map during gameplay. They are stored and managed using arrays.
Types of Power-Ups: 1. Health Boost – Increases tank health 2. Speed Boost – Temporarily increases tank movement speed 3. Rapid Fire – Allows faster bullet firing 4. Shield – Temporary protection from bullets
Implementation Concepts: - Power-up positions stored in arrays - Collision detection with tanks - Timers handled using delays and counters
________________________________________
15.3 Additional Maps
The game now includes more than three maps, each designed using 2D arrays.
Map Variations: - Open Field - Wall Arena - Maze Map - Cross Obstacle Map - Boxed Arena
Each map has unique obstacle placement, making gameplay different in every match.
________________________________________
15.4 Mixed Control System (Keyboard + Mouse)
The control system is enhanced to support both mouse and keyboard input:
•	Player 1: Mouse control
o	Mouse movement controls tank direction
o	Mouse click fires bullets
•	Player 2: Keyboard (W, A, S, D)
o	Spacebar to fire
•	Player 3: Keyboard (Arrow Keys)
o	Enter key to fire
This mixed-input system demonstrates advanced event handling while remaining within Programming Fundamentals concepts.
________________________________________
15.5 Updated Conclusion
With the addition of sound effects, power-ups, multiple maps, and mixed input controls, the Tank Fight Game becomes a highly interactive and feature-rich Programming Fundamentals project. Despite these advanced features, the project strictly avoids OOP and relies only on arrays, functions, and basic C++ constructs, making it ideal for academic evaluation.
________________________________________
Developed By: Mashal Ahmed Course: Programming Fundamentals Language: C++
