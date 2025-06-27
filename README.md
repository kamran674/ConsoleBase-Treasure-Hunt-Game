# ConsoleBase-Treasure-Hunt-Game
🎮Console-Treasure-Hunt is a terminal-based C++ adventure game that challenges players to find hidden treasures on a dynamically generated grid while avoiding deadly traps and racing against the clock. Designed with retro-inspired visuals using ANSI escape codes and Windows API for console control, this project offers a fun blend of logic, memory, and strategy.

🔍 Features:
🎯 Three Difficulty Levels

Easy (5x5 grid) – 100 health, 70 seconds

Medium (6x6 grid) – 80 health, 60 seconds

Hard (7x7 grid) – 70 health, 50 seconds

💥 Grid Cell Types

T (Treasure): Collect to increase your score

X (Trap): Decreases health based on difficulty

. (Safe): Neutral cell

? (Hidden): Unrevealed cell

🎨 Color-Coded Visuals
Uses ANSI escape codes to give each game element its own color:

Green for unrevealed cells

Yellow for treasures

Red for traps

Cyan for safe spots

Purple-highlighted player movement

🧠 Strategic Movement
Move with W, A, S, D keys. Cells are revealed only once, rewarding exploration and memory. The game ends when you find all treasures or run out of time or health.

⏳ Timer Mechanics
Each level includes a countdown timer. Stay focused or risk running out of time!

🖥️ Windows Console Support
Uses the Windows API for key capture, cursor management, and screen control (<windows.h>, _getch(), Sleep(), system("cls"), etc.)

👤 Personalization
Players enter their name, which is shown on the main game screen alongside score and stats.

📜 Instructions and Credits

In-game instructions for controls and objectives

Credits screen for developer attribution

🧩 Technologies Used:
C++ (Standard)

Windows-specific headers: windows.h, conio.h

ANSI Escape Codes for styling

chrono for time tracking

🛠️ Setup & Running:
Compile the code using any Windows-based C++ compiler (e.g., MSVC, Code::Blocks with MinGW, Dev C++)

Run the executable in a Windows console (ANSI colors work best in newer terminals or PowerShell)

⚠️ Requirements:
Windows OS (due to windows.h and conio.h usage)

Terminal that supports ANSI escape codes (Windows 10 or later recommended)

📌 To-Do / Future Enhancements:
Add file-based high score saving

Enable grid randomization seed input

Implement more trap types or power-ups

Add sound feedback with Beep() or external libraries

Portability support for Unix/Linux (replace Windows-specific code)

👤 Author:
Malik Kamran Ali
