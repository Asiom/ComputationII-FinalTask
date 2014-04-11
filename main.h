#ifndef MAIN_H
#define MAIN_H
#include <string>

using namespace std;
//---------------------------------------------------------------------------
/*
      The following defines determine the amount of points on the drawing grid.
*/
#define WIDTH 1000
#define HEIGHT 1000
//---------------------------------------------------------------------------

//    Function declarations
int main(int argc, char* argv[]);
void init(void);
void keyfunc(unsigned char key, int x, int y);
void keyprotection(unsigned char key, int x, int y);
void SpecialInput(int key, int x, int y); // ARROW KEYS IMPLEMENTATION
void det_alien_movement(int down);
void make_aliens();
void update_laser();
void update_alien();
float elap_time();
void update_bomb();
void drop_bomb(int num_of_bombs);
void update_text(int number);
void clear_board(int win);
void set(char* move);
void SpecialInputUP(int key, int x, int y);
void check_status(int number);
void level_up();
void alien_OoM(int number);
void update_specalien();
void create_specalien(int number);
void check_text();
void fade_text(int x, int y, string score, char direction);
void make_bridge();
void update_bridge();
void print_game_paused(int number);
void calculateFPS();


// Place your glut callback declarations here:
void reshape(int w, int h);
void display(void);
void alarm(int alarmnumber);
void drawLine(const float p1[2], const float p2[2], const float color[3], const float lineWidth);

#endif