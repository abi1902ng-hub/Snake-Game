# 🐍 Snake Game — Console Game in C++

A classic Snake game built entirely in C++, featuring two input modes — a **turn-based mode** that works on OnlineGDB and any terminal, and a **real-time mode** for local terminals. Includes a colour-gradient snake, level progression, high score tracking, and replay functionality — all rendered with ANSI colours.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Two Input Modes](#two-input-modes)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [How to Compile](#how-to-compile)
- [How to Run](#how-to-run)
- [Controls](#controls)
- [Gameplay](#gameplay)
- [Scoring & Levels](#scoring--levels)
- [Project Structure](#project-structure)
- [Core Concepts Used](#core-concepts-used)
- [Platform Notes](#platform-notes)
- [Sample Output](#sample-output)

---

## Overview

This is a fully interactive, terminal-based Snake game on a 16×24 grid. The snake eats food to grow longer and score points. The board re-renders after every move with a live colour-gradient snake, glowing food, and a HUD showing score, best, level, and length. Speed increases as levels advance. Collision with walls or the snake's own body ends the game.

---

## 🎮 Two Input Modes

The game ships with **two modes in a single file**, selected at compile time.

### Mode 1 — Turn-Based *(default)*
> ✅ Works on **OnlineGDB**, Replit, and any standard terminal.

The board prints after every move and waits for you to type a direction + `Enter`. No special terminal setup needed.

```
  Move (w/a/s/d/q): d
  Move (w/a/s/d/q): w
  Move (w/a/s/d/q): d
```

### Mode 2 — Real-Time *(local terminal only)*
> ✅ Works on **local Linux / macOS terminals**.

The snake moves continuously and keys respond instantly without pressing `Enter`. Enabled by compiling with `-DREALTIME`.

| | Turn-Based | Real-Time |
|---|---|---|
| Works on OnlineGDB | ✅ Yes | ❌ No |
| Works on local terminal | ✅ Yes | ✅ Yes |
| Needs Enter after each key | ✅ Yes | ❌ No |
| Snake moves automatically | ❌ No | ✅ Yes |
| Arrow key support | ❌ No | ✅ Yes |

---

## ✨ Features

| Feature | Details |
|---|---|
| 🎮 Two Input Modes | Turn-based (OnlineGDB) and real-time (local terminal) |
| 🎨 Colour Gradient Snake | Head `██` bright green → body `▓▓` → tail `░░` dim green |
| 🔴 Glowing Food | Red `●●` at a random position, never on the snake |
| 📊 Live HUD | Score, best score, level, and length shown every frame |
| ⚡ Level Progression | Speed increases every 100 points (real-time mode) |
| 🏆 High Score | Persists across all rounds within the same session |
| 🛡️ Input Validation | Invalid direction keys and replay prompt both handled gracefully |
| 🔄 Replay Option | Full reset on restart; high score is kept |
| 🖥️ Cross-Platform | Linux, macOS, Windows all supported |

---

## Prerequisites

- A C++ compiler supporting **C++17** or later
  - Linux / macOS: `g++` (GCC) or `clang++`
  - Windows: `g++` via MinGW, or MSVC
- A terminal that supports **ANSI escape codes**
  - Linux / macOS: all standard terminals
  - Windows: Windows Terminal recommended
- Standard C++ libraries: `<iostream>`, `<vector>`, `<cstdlib>`, `<ctime>`, `<string>`, `<limits>`
- Real-time mode additionally needs: `<chrono>`, `<thread>`, `<termios.h>` (Linux/macOS) or `<conio.h>` (Windows)

---

## 🔧 How to Compile

### Turn-Based Mode — OnlineGDB & any terminal (default)

**OnlineGDB:**
1. Go to [https://www.onlinegdb.com](https://www.onlinegdb.com)
2. Select **C++** as the language
3. Paste the full contents of `snake.cpp`
4. Click **Run** — it works immediately

**Linux / macOS:**
```bash
g++ -std=c++17 -o snake snake.cpp
```

**Windows (MinGW):**
```bash
g++ -std=c++17 -o snake.exe snake.cpp
```

---

### Real-Time Mode — local terminal only

**Linux / macOS:**
```bash
g++ -std=c++17 -DREALTIME -o snake snake.cpp
```

**Windows (MinGW):**
```bash
g++ -std=c++17 -DREALTIME -o snake.exe snake.cpp
```

> The `-DREALTIME` flag activates non-blocking keyboard input and the continuous game loop.

---

## ▶️ How to Run

```bash
./snake        # Linux / macOS
snake.exe      # Windows
```

A welcome screen is shown first. Press `Enter` to begin.

---

## ⌨️ Controls

### Turn-Based Mode
| Input | Action |
|---|---|
| `W` + Enter | Move Up |
| `S` + Enter | Move Down |
| `A` + Enter | Move Left |
| `D` + Enter | Move Right |
| `Q` + Enter | Quit current game |

### Real-Time Mode
| Key | Action |
|---|---|
| `W` or `↑` | Move Up |
| `S` or `↓` | Move Down |
| `A` or `←` | Move Left |
| `D` or `→` | Move Right |
| `Q` | Quit current game |

> In both modes, the snake **cannot reverse directly** into itself (e.g. pressing Left while moving Right is ignored).

---

## 🕹️ Gameplay

1. The snake starts at the centre of the board with a length of **3**, moving **Right**.
2. Steer the snake to eat the red `●●` food pellet.
3. Each food eaten **grows the snake by one segment** and adds to your score.
4. Avoid hitting the **cyan border walls** or the **snake's own body**.
5. The game ends on collision — your final score, high score, and length are displayed.
6. Choose to **play again** or quit. High score carries over.

---

## 📈 Scoring & Levels

| Event | Points Earned |
|---|---|
| Eat food (Level 1) | 10 pts |
| Eat food (Level 2+) | 10 + (level − 1) × 5 pts |

| Level | Score Threshold | Frame Delay (real-time) |
|---|---|---|
| 1 | 0 – 99 | 180 ms |
| 2 | 100 – 199 | 165 ms |
| 3 | 200 – 299 | 150 ms |
| 4 | 300 – 399 | 135 ms |
| … | … | minimum 60 ms |

> Level is tracked in both modes but frame speed only applies in real-time mode. In turn-based mode, you control the pace by how fast you type.

The high score is preserved across all rounds in the same session.

---

## 🗂️ Project Structure

```
snake.cpp
│
├── #ifdef REALTIME block    — includes termios/conio + chrono/thread
│                              only when compiled with -DREALTIME
├── ANSI colour macros       — RESET, BOLD, RED, GREEN, LGREEN, CYAN, etc.
├── Constants                — W=24 (cols), H=16 (rows)
├── Direction enum           — UP, DOWN, LEFT, RIGHT
├── Point struct             — {int x, int y} for grid coordinates
│
├── Global state             — snake (vector<Point>), food, dir, nextDir,
│                              score, highScore, level, gameOver
│
├── clearScreen()            — cross-platform screen clear
├── printBanner()            — renders the title box
├── printHUD()               — score / best / level / length bar
├── spawnFood()              — random position not occupied by snake
│
├── render()                 — builds char grid[H][W], colour-maps each
│                              cell (██ head, ▓▓ body, ░░ tail, ●● food)
│                              and prints the full bordered board
│
├── applyKey(char)           — maps w/a/s/d/q to direction or quit;
│                              ignores reverse direction attempts
│
├── step()                   — moves head, checks wall & self collision,
│                              grows snake on food or removes tail
│
├── showGameOver()           — final board + score/best/length summary box
├── initGame()               — resets all state, places snake, spawns food
│
├── [#ifndef REALTIME]
│   ├── playTurnBased()      — cin-based loop: render → prompt → move
│   └── main()               — welcome screen, turn-based loop, replay
│
└── [#ifdef REALTIME]
    ├── frameMs()            — delay in ms based on current level
    ├── playRealTime()       — kb_hit loop: read key → step → render → sleep
    └── main()               — enableRaw, welcome, real-time loop, replay
```

---

## 💡 Core Concepts Used

| Concept | Where Applied |
|---|---|
| **Arrays** | `char grid[H][W]` built every frame for rendering |
| **Vectors** | `vector<Point> snake` — grows dynamically as food is eaten |
| **Loops** | `while` game loop; `for` for grid fill, border draw, collision scan |
| **Conditional Logic** | `if/else` for collision, direction blocking, food, level-up |
| **Structs** | `Point {x, y}` for all snake segments and food position |
| **Enums** | `Dir` enum for clean, readable directional state |
| **Functions** | Each responsibility isolated — render, input, movement, HUD |
| **Compile-time Macros** | `#ifdef REALTIME` / `#ifndef REALTIME` selects input mode |
| **Non-blocking I/O** | `termios` raw mode (Linux/macOS) / `_kbhit` (Windows) in REALTIME |
| **Timing** | `std::this_thread::sleep_for` + `std::chrono::milliseconds` in REALTIME |
| **Input Validation** | `tolower()` + loop re-prompts on invalid keys and replay answer |

---

## 🖥️ Platform Notes

### OnlineGDB
- Use **Turn-Based mode** (default, no flag needed).
- OnlineGDB buffers input until `Enter` is pressed, so real-time key detection is not possible there. Turn-based mode was designed specifically for this.

### Linux / macOS
- Both modes work.
- Real-time mode uses `termios.h` to put the terminal into **raw mode** so keystrokes are read immediately without `Enter`.
- The terminal is fully restored to its original state on exit.
- Arrow keys send a 3-byte escape sequence (`ESC [ A/B/C/D`) parsed in `playRealTime()`.

### Windows
- Both modes work with MinGW.
- Real-time mode uses `<conio.h>` with `_kbhit()` / `_getch()` — already included via the `#ifdef _WIN32` block inside `#ifdef REALTIME`.
- Use **Windows Terminal** for proper ANSI colour rendering. On classic `cmd`, run `chcp 65001` first.

---

## 🖥️ Sample Output

**Turn-Based Mode — in game:**
```
  ╔══════════════════════════════════╗
  ║   🐍  S N A K E   G A M E  🐍   ║
  ╚══════════════════════════════════╝
  Score : 30   Best  : 30   Level : 1   Length: 6

  ╔════════════════════════════════════════════════╗
  ║                                                ║
  ║              ●●                                ║
  ║                                                ║
  ║          ████▓▓▓▓▓▓░░░░                        ║
  ║                                                ║
  ╚════════════════════════════════════════════════╝

  Move (w/a/s/d/q):
```

**Game Over screen:**
```
  ╔══════════════════════════════════╗
  ║       💀   GAME  OVER   💀      ║
  ╠══════════════════════════════════╣
  ║  Final Score  : 30               ║
  ║  High  Score  : 60               ║
  ║  Snake Length : 6                ║
  ╚══════════════════════════════════╝

  Play again? (y/n):
```

---

## 📝 Notes

- **Turn-based mode** has no automatic movement — the snake only moves when you enter a command, so take your time.
- **Real-time mode** speeds up with each level; at max level the frame delay drops to 60 ms.
- Food is guaranteed to never spawn on a cell occupied by the snake.
- Pressing `Q` in either mode ends the round immediately and shows the game-over screen.
- Entering anything other than `y` or `n` at the replay prompt will show an error and re-ask.
