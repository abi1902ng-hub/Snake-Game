/*
 ============================================================
   SNAKE GAME — Console Game in C++
   Works on: OnlineGDB, local Linux/macOS, local Windows

   HOW INPUT WORKS:
   ─────────────────────────────────────────────────────────
   DEFAULT (OnlineGDB & any terminal):
     Turn-based — type w/a/s/d and press Enter each turn.
     Simple, reliable, works everywhere.

   REAL-TIME (local Linux/macOS terminal only):
     Compile: g++ -std=c++17 -DREALTIME -o snake snake.cpp
     Snake moves continuously; keys work without Enter.
 ============================================================
*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>

#ifdef REALTIME
  #include <chrono>
  #include <thread>
  #ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    inline bool kb_hit()    { return _kbhit() != 0; }
    inline char kb_get()    { return (char)_getch(); }
    inline void sleep_ms(int ms){ Sleep(ms); }
  #else
    #include <termios.h>
    #include <unistd.h>
    static struct termios _orig;
    void  enableRaw(){ struct termios r=_orig; r.c_lflag&=~(ICANON|ECHO); r.c_cc[VMIN]=0; r.c_cc[VTIME]=0; tcsetattr(STDIN_FILENO,TCSANOW,&r); }
    void  disableRaw(){ tcsetattr(STDIN_FILENO,TCSANOW,&_orig); }
    bool  kb_hit(){ char c; return read(STDIN_FILENO,&c,1)>0?(ungetc(c,stdin),true):false; }
    char  kb_get(){ char c=0; read(STDIN_FILENO,&c,1); return c; }
    void  sleep_ms(int ms){ std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }
  #endif
#endif

using namespace std;

// ── ANSI colours ───────────────────────────────────────────
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define LGREEN  "\033[92m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define WHITE   "\033[37m"

// ── Board dimensions ───────────────────────────────────────
const int W = 24;
const int H = 16;

enum Dir { UP, DOWN, LEFT, RIGHT };
struct Point { int x, y; };

// ── Global state ───────────────────────────────────────────
vector<Point> snake;
Point  food;
Dir    dir, nextDir;
int    score, highScore = 0, level;
bool   gameOver;

// ── Utilities ──────────────────────────────────────────────
void clearScreen(){
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printBanner(){
    cout << BOLD << LGREEN;
    cout << "  ╔══════════════════════════════════╗\n";
    cout << "  ║   🐍  S N A K E   G A M E  🐍      ║\n";
    cout << "  ╚══════════════════════════════════╝\n" << RESET;
}

void printHUD(){
    cout << "  " << BOLD
         << LGREEN  << "Score : " << score
         << "   " << YELLOW << "Best  : " << highScore
         << "   " << CYAN   << "Level : " << level
         << "   " << WHITE  << "Length: " << (int)snake.size()
         << RESET << "\n\n";
}

void spawnFood(){
    srand((unsigned)time(0) ^ (unsigned)score ^ (unsigned)snake.size());
    bool onSnake;
    do {
        onSnake = false;
        food.x = rand() % W;
        food.y = rand() % H;
        for (auto& s : snake)
            if (s.x == food.x && s.y == food.y){ onSnake = true; break; }
    } while(onSnake);
}

// ── Render ─────────────────────────────────────────────────
void render(){
    char grid[H][W];
    for (int y=0;y<H;y++) for (int x=0;x<W;x++) grid[y][x]=' ';
    grid[food.y][food.x] = '*';
    for (int i=(int)snake.size()-1;i>=0;i--)
        grid[snake[i].y][snake[i].x] = (i==0)?'H':(i<(int)snake.size()/3?'O':'o');

    clearScreen();
    printBanner();
    printHUD();

    cout << "  " << BOLD << CYAN << "╔";
    for (int x=0;x<W;x++) cout<<"══";
    cout << "╗\n" << RESET;

    for (int y=0;y<H;y++){
        cout << "  " << BOLD << CYAN << "║" << RESET;
        for (int x=0;x<W;x++){
            char ch=grid[y][x];
            if      (ch=='H') cout<<BOLD<<LGREEN<<"██"<<RESET;
            else if (ch=='O') cout<<GREEN<<"▓▓"<<RESET;
            else if (ch=='o') cout<<"\033[32m"<<"░░"<<RESET;
            else if (ch=='*') cout<<BOLD<<RED<<"●●"<<RESET;
            else               cout<<"  ";
        }
        cout << BOLD << CYAN << "║\n" << RESET;
    }

    cout << "  " << BOLD << CYAN << "╚";
    for (int x=0;x<W;x++) cout<<"══";
    cout << "╝\n\n" << RESET;
}

// ── Apply key → direction ──────────────────────────────────
bool applyKey(char ch){
    switch(ch){
        case 'w': case 'W': if(dir!=DOWN)  { nextDir=UP;    return true; } break;
        case 's': case 'S': if(dir!=UP)    { nextDir=DOWN;  return true; } break;
        case 'a': case 'A': if(dir!=RIGHT) { nextDir=LEFT;  return true; } break;
        case 'd': case 'D': if(dir!=LEFT)  { nextDir=RIGHT; return true; } break;
        case 'q': case 'Q': gameOver=true; return true;
    }
    return false;
}

// ── Advance snake one step ─────────────────────────────────
void step(){
    dir = nextDir;
    Point head = snake.front();
    switch(dir){
        case UP:    head.y--; break;
        case DOWN:  head.y++; break;
        case LEFT:  head.x--; break;
        case RIGHT: head.x++; break;
    }
    if (head.x<0||head.x>=W||head.y<0||head.y>=H){ gameOver=true; return; }
    for (auto& s:snake) if(s.x==head.x&&s.y==head.y){ gameOver=true; return; }

    snake.insert(snake.begin(), head);
    if (head.x==food.x && head.y==food.y){
        score += 10+(level-1)*5;
        if(score>highScore) highScore=score;
        level = score/100+1;
        spawnFood();
    } else {
        snake.pop_back();
    }
}

// ── Game over screen ───────────────────────────────────────
void showGameOver(){
    render();
    cout << RED << BOLD;
    cout << "  ╔═══════════════════════════════════╗\n";
    cout << "  ║       💀   GAME  OVER   💀          ║\n";
    cout << "  ╠═══════════════════════════════════╣\n";
    auto pad=[](int n,int w){ string s=to_string(n); while((int)s.size()<w)s+=' '; return s; };
    cout << "  ║  " << YELLOW << "Final Score  : " << pad(score,18)       << RED << "║\n";
    cout << "  ║  " << CYAN   << "High  Score  : " << pad(highScore,18)   << RED << "║\n";
    cout << "  ║  " << GREEN  << "Snake Length : " << pad((int)snake.size(),18) << RED << "║\n";
    cout << "  ╚═══════════════════════════════════╝\n" << RESET;
}

// ── Init ───────────────────────────────────────────────────
void initGame(){
    snake.clear();
    Point p={W/2,H/2};
    snake.push_back(p); p.x--; snake.push_back(p); p.x--; snake.push_back(p);
    dir=nextDir=RIGHT; score=0; level=1; gameOver=false;
    spawnFood();
}

// ══════════════════════════════════════════════════════════
//   TURN-BASED MODE  —  default, works on OnlineGDB
// ══════════════════════════════════════════════════════════
#ifndef REALTIME

void playTurnBased(){
    initGame();
    render();
    cout << BOLD << WHITE  << "  Controls  : "
         << YELLOW << "W" << WHITE << "=Up  "
         << YELLOW << "S" << WHITE << "=Down  "
         << YELLOW << "A" << WHITE << "=Left  "
         << YELLOW << "D" << WHITE << "=Right  "
         << RED    << "Q" << WHITE << "=Quit\n"
         << "  Type a letter and press Enter each turn.\n\n" << RESET;

    while(!gameOver){
        char ch;
        while(true){
            cout << MAGENTA << BOLD << "  Move (w/a/s/d/q): " << RESET;
            if(!(cin>>ch)){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cout << RED << "  ✗ Type w, a, s, d, or q.\n" << RESET;
                continue;
            }
            ch = (char)tolower(ch);
            if(ch=='w'||ch=='s'||ch=='a'||ch=='d'||ch=='q') break;
            cout << RED << "  ✗ Invalid! Use w a s d or q.\n" << RESET;
        }
        applyKey(ch);
        if(!gameOver) step();
        if(!gameOver){ render(); }
    }
    showGameOver();
}

int main(){
    clearScreen();
    cout << BOLD << LGREEN;
    cout << "\n\n  ╔════════════════════════════════════════╗\n";
    cout << "  ║    🐍   WELCOME TO SNAKE GAME   🐍       ║\n";
    cout << "  ╠════════════════════════════════════════╣\n";
    cout << "  ║  MODE : Turn-Based  (OnlineGDB ready)  ║\n";
    cout << "  ║  Type a direction + Enter each turn.   ║\n";
    cout << "  ╠════════════════════════════════════════╣\n";
    cout << "  ║  " << YELLOW << "W" << LGREEN << " = Up        " << YELLOW << "S" << LGREEN << " = Down                ║\n";
    cout << "  ║  " << YELLOW << "A" << LGREEN << " = Left      " << YELLOW << "D" << LGREEN << " = Right               ║\n";
    cout << "  ║  " << RED    << "Q" << LGREEN << " = Quit                              ║\n";
    cout << "  ╠════════════════════════════════════════╣\n";
    cout << "  ║  Eat " << RED << "●●" << LGREEN << " to grow. Don't hit walls       ║\n";
    cout << "  ║  or yourself. Score up to level up!    ║\n";
    cout << "  ╚════════════════════════════════════════╝\n\n" << RESET;
    cout << WHITE << "  Press Enter to start..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');

    char replay;
    do {
        playTurnBased();
        while(true){
            cout << MAGENTA << BOLD << "\n  Play again? (y/n): " << RESET;
            cin >> replay;
            if(replay=='y'||replay=='Y'||replay=='n'||replay=='N') break;
            cout << RED << "  ✗ Invalid! Please enter 'y' for Yes or 'n' for No.\n" << RESET;
        }
    } while(replay=='y'||replay=='Y');

    cout << GREEN << BOLD << "\n  Thanks for playing! 👋\n\n" << RESET;
    return 0;
}

// ══════════════════════════════════════════════════════════
//   REAL-TIME MODE  —  local terminal, compile with -DREALTIME
// ══════════════════════════════════════════════════════════
#else

int frameMs(){ int d=180-(level-1)*15; return d<60?60:d; }

void playRealTime(){
    initGame();
    while(!gameOver){
        if(kb_hit()){
            char ch=kb_get();
            if(ch=='\033'){ kb_get(); char a=kb_get();
                if     (a=='A'&&dir!=DOWN)  nextDir=UP;
                else if(a=='B'&&dir!=UP)    nextDir=DOWN;
                else if(a=='C'&&dir!=LEFT)  nextDir=RIGHT;
                else if(a=='D'&&dir!=RIGHT) nextDir=LEFT;
            } else applyKey(ch);
        }
        step();
        if(!gameOver) render();
        sleep_ms(frameMs());
    }
    showGameOver();
}

int main(){
#ifndef _WIN32
    tcgetattr(STDIN_FILENO,&_orig);
    enableRaw();
#endif
    clearScreen();
    cout<<BOLD<<LGREEN<<"\n  🐍 SNAKE — REAL-TIME MODE (local terminal)\n"
        <<"  Arrow keys or WASD to move | Q to quit\n"
        <<"  Press Enter to start...\n"<<RESET;
#ifndef _WIN32
    disableRaw();
#endif
    cin.get();
#ifndef _WIN32
    enableRaw();
#endif

    char replay;
    do {
        playRealTime();
#ifndef _WIN32
        disableRaw();
#endif
        while(true){
            cout<<MAGENTA<<BOLD<<"\n  Play again? (y/n): "<<RESET;
            cin>>replay;
            if(replay=='y'||replay=='Y'||replay=='n'||replay=='N') break;
            cout<<RED<<"  ✗ Invalid! Please enter 'y' for Yes or 'n' for No.\n"<<RESET;
        }
#ifndef _WIN32
        enableRaw();
#endif
    } while(replay=='y'||replay=='Y');

#ifndef _WIN32
    disableRaw();
#endif
    cout<<GREEN<<BOLD<<"\n  Thanks for playing! 👋\n\n"<<RESET;
    return 0;
}
#endif
