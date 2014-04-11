////////////////////////////////////////////////////////////
//
// File: main.cpp
// Author: Sven Goossens
// Purpose:
//      A skeleton for the final assignment
//      Without adaptations, this should draw 2 pixels on the screen
//
///////////////////////////////////////////////////////////
#define  _CRT_SECURE_NO_WARNINGS
#define	 UPDATE_SPEED 20
#include <stdio.h>
#include <stdlib.h>
#include "glut.h"			 // glut header file. Never include glut before stdlib.h!
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <Windows.h>	// for time difference
#include <limits>

#define _USE_MATH_DEFINES	// Signal math.h that we would like defines like M_PI
#include <math.h>			// Might come in usefull for cosine functions and stuff like that

#include "main.h"			// Function declarations and some definitions
#include "dlist.h"			// base class of the drawtools
#include "drawtools.h"      // contains all you need to draw stuff

const int X_MOUSE = 300;
const int Y_MOUSE = 300;


//    Global variables
char keytext[50];
char alien_movm = 's';
int level;
float elapsed_time=0, last_time=0;
int enable = 0, number = 1, down = 0, limit = 5, pause = 0, reset = 0;
dlist DrawList;
list<laser> laserz;
list<bomb>	bombz;
list<alien> alienz;
list<specalien> specalienz;
list<text>	textz;
list<bridge> bridgez;
int keyprotect = 10;
list<list<alien>> alien_rows;
list<list<alien>>::iterator alien_rowsit;
list<laser>::iterator laserzit;
list<alien>::iterator alienit;
list<bomb>::iterator bombit;
list<specalien>::iterator specalienit;
list<text>::iterator textit;
list<bridge>::iterator bridgeit;
player* Speler = new player(&DrawList);//&DrawList);


//function declaration
void update(int number);
void calc_change(int number);

/*
      void init(void)
      This function is called when glut is initialized. Use it to initialize
      variables and setup things that have to be done BEFORE the actual
      picture is drawn.
*/
void init(void)
{
	int number = 0;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);	// Double buffering, RGB format
	glutInitWindowSize(WIDTH, HEIGHT);				// 1024 x 500 pixel window
	glutInitWindowPosition(0, 0);					// Window appear in the top-left corner of the screen
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); // Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutCreateWindow("Space Invaders by Niels Vertegaal");				// Window title
	glutDisplayFunc(display);						// Main display function
	glutReshapeFunc(reshape);						// Reshape function, called when the window resizes
	glutKeyboardFunc(keyfunc);
	glutKeyboardUpFunc(keyprotection);
	glutSpecialFunc(SpecialInput);
	glutSpecialUpFunc(SpecialInputUP);
	make_aliens();
	make_bridge();
	// You might want to remove these after you are done testing with them
	glutTimerFunc(UPDATE_SPEED, update, number);
 	glutTimerFunc(rand() % (200 -  level * 10) + (1000 - level * 10), drop_bomb, number++);
	glutTimerFunc(20, calc_change, number);
	glutTimerFunc(50, check_status, number);
	glutTimerFunc(22, alien_OoM, number);
	glutTimerFunc(rand() % 10000 + 2000, create_specalien, number);
	glClearColor(1.0, 1.0, 1.0, 1.0);   // white background
	glColor3f(1.0, 0.0, 0.0);           //draw in red
	glMatrixMode(GL_PROJECTION);        // Next lines will adjust the projection matrix


	glLoadIdentity();                   // Reset matrix, returns viewport to origin
	gluOrtho2D(0.0, (float)WIDTH, 0.0, (float)HEIGHT);

	glMatrixMode(GL_MODELVIEW);
	//readEDIF("test.edif");
}

//---------------------------------------------------------------------------
//	reshape: Handle window resizing (reshaping) events
//	The reshape callback is called when the window is resized or moved
//	In the current implementation, the drawing grid is extended or truncated 
//	when the window is reshaped

void reshape(int w, int h) 
{	
	glViewport(0.0f, 0.0f, (GLfloat)w, (GLfloat)h);
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}
//---------------------------------------------------------------------------
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	if (pause == 0)
	{
		Speler->draw();
		update_alien();
		update_bomb();
		update_laser();
		update_text(number);
		update_specalien();
		update_bridge();
		check_text();
	}
	else if (pause == 1)
	{
		print_game_paused(number);
	}


	glFlush();			// Execute all commands waiting to be executed
	glutSwapBuffers();	// Swap the backbuffer and frontbuffer,
						// display what has been drawn
 }

float elap_time(){
	float current_time = GetTickCount();
	elapsed_time = current_time - last_time;
	return elapsed_time/1000;
}

//---------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	// standard GLUT initialization 
	glutInit(&argc,argv);
	init();						// Call init function
	printf("starting Glut Loop...\n");
	glutMainLoop();				// Start drawing proceidure
	return 0;
}
//---------------------------------------------------------------------------
void update(int number){
	if (pause == 0)
	{
		if (enable)
		{
			// check if out of map
			laserzit = laserz.begin();
			while (laserzit != laserz.end())
			{
				if (laserzit->height() > glutGet(GLUT_WINDOW_HEIGHT))
				{
					laserz.erase(laserzit++);
				}
				else
				{
					laserzit++;
				}
			}
		}
		// determine the movement of the aliens
		alien_rowsit = alien_rows.begin();
		for (int k = 0; k < alien_rows.size(); k++){
			alienit = alien_rowsit->begin();
			if (alien_rowsit->size() != 0)
			{
				det_alien_movement(down);
			}
			else { goto stop; }
			//while (alienit != alien_rowsit->end())
			//{
			//	alienit->draw();
			//	alienit++;
			//}
			alienit = alien_rowsit->begin();
		stop:
			alien_rowsit++;
		}
		if (down == 6){ down = 0; }
		// fix iterator if neccesary
		if (alien_rowsit != alien_rows.end())
		{
			if (alien_rowsit->size() != 0)
			{
				alienit = alien_rowsit->begin();
			}
		}
		// make it go down several time for smooth transition
		if (alien_movm == 'd')
		{
			down++;
		}
	}
		glutPostRedisplay();
		glutTimerFunc(UPDATE_SPEED, update, enable);
}


void det_alien_movement(int down)	// Determine the movement of the aliens
{
	// initial values
	int left_x = 100;	
	int right_x = 0;
		alien_rowsit = alien_rows.begin();
		// loop trough list of list
		for (int k = 0; k < alien_rows.size(); k++)
		{
			alienit = alien_rowsit->begin();

			list<alien>::iterator begin = alien_rowsit->begin();
			list<alien>::iterator end = alien_rowsit->end();
			list<list<alien>>::iterator coord = alien_rows.begin();
			if (begin == end)  	{}
			else{ end--; } // end = 1 step too far

			while (coord != alien_rows.end()) // EoL
			{
				alienit = coord->end();
				if (coord->size() != 0)
				{ 
					alienit--; 
				}
				if (coord->size() != 0)
				{
					// determine left and right limit of current level
					if (coord->begin()->xCoord() < left_x)
					{
						left_x = coord->begin()->xCoord();
					}
					if (alienit->xCoord() > right_x)
					{
						right_x = alienit->xCoord();
					}
				}
				coord++; //iterate through them
			}
			alienit = alien_rowsit->begin();

			if (left_x - 20 < 50 && alien_rowsit->size() != 0)	// Alien's are on the left side of the map
			{
				if (down == 6)	// go down 6 times for smooth animation
				{
					alienit->set_movement('r');
					alien_movm = 'r';
				}
				else
				{
					alienit->set_movement('d');
					alien_movm = 'd';
				}
			}
			if (right_x + 20 > 900 && alien_rowsit->size() != 0) // "" right side""
			{
				if (down == 6)
				{
					alienit->set_movement('l');
					alien_movm = 'l';
				}
				else
				{
					alienit->set_movement('d');
					alien_movm = 'd';
				}
			}
		}
}


void SpecialInput(int key, int x, int y) // Function for key arrows
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Speler->movement('l');
		break;
	case GLUT_KEY_RIGHT:
		Speler->movement('r');
		break;
	}
	glutPostRedisplay();
}
void keyfunc(unsigned char key, int x, int y) //Function for normal keys
{
	const int spacebar = 32;
	if (elap_time() > 2000) //Disable Spamming
	{
		if (keyprotect)		// Disable Holding Key
		{
			keyprotect = 0;
			switch (key)
			{
			case spacebar:
				if (laserz.size() < limit)
				{
					laserz.push_back(laser(&DrawList, Speler->weap_x(), Speler->weap_y()));
					enable = 1;
				}
			}
			glutPostRedisplay();
		}
		if (key == 'a')	//Infinite or not infinite ammo
		{
			static int lim_choice = 0;
			switch (lim_choice)
				{
				case 0:
				{
						  limit = (std::numeric_limits<int>::max)();
						  lim_choice++;
						  break;
				}
				case 1:
				{
						  limit = 5;
						  lim_choice--;
						  break;
				}
			}
		}
		if (key == 'p')	// pause the game
		{
			switch (pause)
			{
			case 0:
			{
					  pause++;
					  break;
			}
			case 1:
			{
					  pause--;
					  break;
			}
			}
		}
		if (key == 'r')	// reset the game
		{
			reset = 1;
		}
	}
}

void update_laser()	// Draw lasers
{
	laserzit = laserz.begin();
	while (laserzit != laserz.end())
	{
		laserzit->update();
		laserzit->draw();
		laserzit++;
	}
}
void update_alien() // update aliens
{
	alien_rowsit = alien_rows.begin();
	for (int k = 0; k < alien_rows.size(); k++)
	{
		alienit = alien_rowsit->begin();
		laserzit = laserz.begin();
		list<alien>::iterator end = alien_rowsit->end();

		while (alienit != end && alien_rowsit->size() != 0)
		{
			while (laserzit != laserz.end())
			{
				if (alienit->check_collision(laserzit)) // check for collision with laser
				{
					Speler->score(alienit->score());
					stringstream stream;
					string score;
					stream << alienit->score();
					score = stream.str();
					fade_text(alienit->xCoord() + alienit->width()/2, alienit->yCoord()+alienit->height()/2, score, (char)'u');
					laserz.erase(laserzit++);
					alien_rowsit->erase(alienit++);
					break;
				}
				if (laserz.size() == 0){ break; }
				else{ laserzit++; }
			}
			laserzit = laserz.begin();
			if (alienit != alien_rowsit->end())
			{
				if (alienit->check_player_collision(Speler)) // check for collision with player
				{
					alien_rowsit->erase(alienit++); // erase alien if hit
					Speler->is_hit(1); // decrease health if hit
				}
			}
			if (alienit != end)
			{
				alienit->update();
				alienit->draw();
				alienit++;
			}
		}
		alien_rowsit++;
	}
}
void update_bomb() // update and check bombz
{
	bombit = bombz.begin();
	while (bombit != bombz.end() && bombz.size() != 0) // check if not at end of iterator and size isnt 0
	{
		if (bombit->check_player_collision(Speler)) // check if bombs collide with player
		{
			//fade_text(Speler->x_loc() + Speler->width() / 2, Speler->y_loc() + Speler->height() / 2, "1",(char) 'd');
			fade_text(835, 790, "-1", (char) 'd');
			bombz.erase(bombit++);	// erase bomb
			Speler->is_hit(1);		// decrease health player
		}
		if (bombz.size() != 0)
		{
			if (bombit->y_loc() < 0) // erase bomb if out of the window
			{
				bombz.erase(bombit++);
			}
		}
		if (bombz.size() != 0) // update
		{
			bombit->update();
			bombit->draw();
			bombit++;
		}
	}
}

void update_text(int number) // update status text
{
	// initial variables
	int hp = Speler->ret_health();
	int score = Speler->ret_score();
	float p[2];
	float clr[3] {0.0f, 0.0f, 0.0f};
	string ammo;
	string status;
	stringstream ammostream;
	stringstream stream;
	if (limit == 5) // a keypress
	{
		// if limited calculate real time how much ammo is left
		ammostream << 5 - laserz.size();
		ammo = ammostream.str();
	}
	else
		ammo = "Infinite";

	stream << "(Score:  " << score << " Health: " << hp << " Level: " << level << ")" << endl << " Ammo: " << ammo << endl;
	status = stream.str();
	stream.str("");
	stream.clear();

	p[0] = 750; //glutGet(GLUT_WINDOW_WIDTH - 300);
	p[1] = 800; // glutGet(GLUT_WINDOW_HEIGHT - 10);
	text* Status = new text(&DrawList, p, clr, status, 1, 'd');
	Status->draw();
}


void keyprotection(unsigned char key, int x, int y) // disable holding the spacebar to shoot
 {
	const int spacebar = 32;
	switch (key)
	{
		case spacebar:
			keyprotect = 1;
	}
}

void drop_bomb(int num_of_bombs)	// create bomb
{
	if (alien_rows.size() != 0 && pause == 0)
	{
		// initial iterators
		alien_rowsit = alien_rows.begin();
		alienit = alien_rowsit->begin();
		if (alien_rowsit->size() != 0)
		{
			int k = rand() % alien_rows.size(); // random row
			advance(alien_rowsit, k);
			if (alien_rowsit->size() == 1)
			{
				alienit = alien_rowsit->begin();
				bombz.push_back(bomb(&DrawList, alienit->weap_x(), alienit->weap_y())); // create bomb
			}
			if (alien_rowsit->size() > 1)
			{
				int i = rand() % (alien_rowsit->size() - 1); // random column
				advance(alienit, i);
				if (alienit != alien_rowsit->end())
				{
					bombz.push_back(bomb(&DrawList, alienit->weap_x(), alienit->weap_y())); // create bomb
				}
				else
				{
					alienit = alien_rowsit->begin();
					bombz.push_back(bomb(&DrawList, alienit->weap_x(), alienit->weap_y())); // create bomb
				}
			}
		}
		
	}
	glutTimerFunc(rand() % (200 - level * 10) + (1000 - level * 10), drop_bomb, number++);	// random factor based on level to create bomb
		
}

void create_specalien(int number) // create random based specalien
{
	if (specalienz.size() == 0)
	{
  		specalienz.push_back(specalien(&DrawList, (500 + level * 50), rand() % 2 , level / 2 + 2));
	}
	glutTimerFunc(rand() % 10000 + 4000, create_specalien, number);
}

void clear_board(int win) // clear board, erase all leftover lasers and bombs
{
	laserz.clear();
	bombz.clear();
	if (alien_rows.size() != 0)
	{
		alien_rowsit = alien_rows.begin();
		for (int k = 0; k < alien_rows.size(); k++)
		{
			alien_rowsit->clear();
		}
		alien_rows.clear();
	}
	if (reset == 1) // if you want to reset the current level
	{
		bridgez.clear();
		make_bridge();
		specalienz.clear();
		Speler->restore_score();	// restore score at beginning of lvl
		return;
	}
	if (win == 1)		// if lost and hp < 0, then also reset the player
	{
		bridgez.clear();
		make_bridge();
		Speler->reset();
	}
}

void make_aliens() 		// initial creations of aliens
{
	int i = 200;
	int iterations = 0;
	float speed_factor = 1;
	int num_of_alien_row = (750 - floor(level / 2) * 60);
	if ((750 - floor(level / 2) * 60) < 540)
	{
		num_of_alien_row = 540; 
		speed_factor = 1 + (level / 16); // increase speed of aliens after lvl 8
	}
	for (int k = 780; k > num_of_alien_row; k = k - 60)
	{
		iterations++; // calculate rows
	}
	for (int k = 780; k > num_of_alien_row; k = k - 60)
	{
		int score = iterations * 100 + 100; // calculate score depending on row
		for (i = 200; i < glutGet(GLUT_WINDOW_WIDTH) - 100; i += 50)
		{
			alienz.push_back(alien(&DrawList, i, k, score, 'r', speed_factor));
		}
		alien_rows.push_back(alienz);
		alienz.clear();
		iterations--;
	}
	glutPostRedisplay();
}

void calc_change(int number) // calculate player change: smooth animation
{
	static int hold = 0;
	if (pause == 0)
	{
		char movement = Speler->ret_move();
		static char mov_prev;
		static float vx;
		if ((mov_prev == 'r' && movement == 'l') || (mov_prev == 'l' && movement == 'r'))
		{
			vx = vx;	 // smooth left to right transistion while in motion
		}
		float DT = 0.02;
		switch (movement)
		{
		case 'r':
		{
					float dvy = 2000 * DT;
					vx = vx + dvy;
					if (vx > 1000)
					{
						vx = 1000;
					}
					float dx = vx * DT;
					Speler->move(dx);
					mov_prev = 'r';
					break;
		}
		case 'l':
		{
					float dvy = 2000 * DT;
					vx = vx + dvy;
					if (vx > 1000)
					{
						vx = 1000;
					}
					float dx = vx * DT;
					Speler->move(dx);
					mov_prev = 'l';
					break;
		}
		case 's': // no movement button pressed
		{
					  if (hold > 10)
					  {
						  vx = 0;
						  int dx = vx * DT;
						  Speler->move(dx);
						  hold = 0;
						  break;
					  }
					  else
					  {
						  vx = vx/3;
						  int dx = 0* DT;
						  Speler->move(dx);
						  break;
					  }
		}
		}
	}
	glutTimerFunc(20, calc_change, hold++);
}

void SpecialInputUP(int key, int x, int y) // actions when special keys are pushed up
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Speler->movement('s');
		break;
	case GLUT_KEY_RIGHT:
		Speler->movement('s');
		break;
	}
	glutPostRedisplay();
}

void level_up()
{
	clear_board(0);
	level = level + 1;
	make_aliens();
}

void check_status(int number) // check if won or lost
{
	int size = 0;
	int win;
	int hp = Speler->ret_health();

	size = alien_rows.size();

	// if you lost or you want to reset
	if (hp <= 0 || reset == 1)
	{
		win = 1;
		clear_board(win);
		reset = 0;
		make_aliens();
	}
	// if you won , lvl up, clear board and remake the bridges if level %3 == 0
	if (size == 0 && hp > 0)
	{
		level_up();
		if (level % 3 == 0)
		{
			make_bridge();
		}
		Speler->leveled();
	}
	glutTimerFunc(50, check_status, number);
}

void alien_OoM(int number)	// check if aliens go out of the map
{
	if (pause == 0)
	{
		// first clean up the alien rows that are not being used anymore
		alien_rowsit = alien_rows.begin();
		while (alien_rowsit != alien_rows.end())
		{
			if (alien_rowsit->size() == 0)
			{
				alien_rows.erase(alien_rowsit++);
			}
			else
			{
				alien_rowsit++;
			}

		}
		// now check with the leftover rows if they are below the screen
		if (alien_rows.size() != 0)
		{

			alien_rowsit--;
			int height = alien_rowsit->begin()->yCoord();
			if (height < 0)
			{
				Speler->is_hit(alien_rows.back().size());
				alien_rows.erase(alien_rowsit++);
			}
		}
	}
	glutTimerFunc(20, alien_OoM, number);
}

void update_specalien()	// update specialien
{
	if (specalienz.size() != 0) // check if size == 0
	{
		//init iterators
		laserzit = laserz.begin();
		specalienit = specalienz.begin();
		while (laserzit != laserz.end()) // check collision laser
		{
			if (specalienit->check_collision(laserzit))
			{
				stringstream converter;
				converter << specalienit->ret_score();
				string result = converter.str();
				fade_text(specalienit->xCoord() + specalienit->width()/2, specalienit->yCoord() + specalienit->height()/2, result, (char)'u'); // print text for fade
				laserz.erase(laserzit++); // erase the laser
				Speler->score(specalienit->ret_score()); // add score value of special alien to player score
				specalienz.erase(specalienit++); // erase special alien
				break; // if there is 1 collision, it is done
			}
			// if there is 1 laser, stop
			if (laserz.size() == 0){ break; }
			else{ laserzit++; }
			// if there is no special alien left, no point in further iteratating
			if (specalienz.size() == 0){ break; }
		}
	}
	if (specalienz.size() != 0) // check if out of map and then delete them if true 
	{
		specalienit = specalienz.begin();
		if (specalienit->xCoord() < -100 || specalienit->xCoord() > 1200)
		{
			specalienz.clear();
		}
	}
	// otherwise update the special aliens position
	if (specalienz.size() != 0)
	{
		specalienit->update();
		specalienit->draw();
	}
}

// crete the fade text and put in the text list
void fade_text(int x, int y, string score, char direction)
{
	GLfloat p[2], clr[3] = { 0, 1, 0 };

	p[0] = x-5; //glutGet(GLUT_WINDOW_WIDTH - 300);
	p[1] = y; // glutGet(GLUT_WINDOW_HEIGHT - 10);

	textz.push_back(text(&DrawList, p, clr, score, 0.0, direction));
	
}
// makes the fading text and when alpha is higher als one deletes the text
void check_text()
{
	if (textz.size() != 0) // cant iterate if size == 0
	{
		textit = textz.begin();
		while (textit != textz.end())
		{
			if (textit->ret_alpha() >= 1)
			{
				textz.erase(textit++);
			}
			else
			{
				textit->special_draw();
				textit++;
			}
		}
	}
}

// make the bridges and push them into the bridge list
void make_bridge()
{
	int x = glutGet(GLUT_WINDOW_WIDTH) / 3 - 100; 
	for (int k = x; k != 4 * x; k = k + x)
	{
		int p[2] = { k, 400 };
		bridgez.push_back(bridge(&DrawList, p));
	}
}

// checks if the bridges is having a collision with a bomb laser or an alien and updates it afterwards
void update_bridge()
{
	bridgeit = bridgez.begin();
	if (bridgez.size() != 0)
	{
		while (bridgeit != bridgez.end())
		{
			laserzit = laserz.begin();
			while (laserzit != laserz.end() && laserz.size() != 0 && bridgez.size() != 0)
			{
 				if (bridgeit->check_collision(laserzit))
				{
					if (bridgeit->ret_health() <= 0)
					{
						bridgez.erase(bridgeit++);
						laserz.erase(laserzit++);
					}
					else
					{
						laserz.erase(laserzit++);
						bridgeit->is_hit();
					}
				}
				if (laserzit == laserz.end()){ break; }
				if (laserz.size() == 0){ break; }
				else{ laserzit++; }
			}
			bombit = bombz.begin();
			while (bombit != bombz.end() && bombz.size() != 0 && bridgez.size() != 0)
			{
				if (bridgeit->check_collision(bombit))
				{
					if (bridgeit->ret_health() <= 0)
					{
						bridgez.erase(bridgeit++);
						bombz.erase(bombit++);
					}
					else
					{
						bombz.erase(bombit++);
						bridgeit->is_hit();
					}
				}
				if (bombit == bombz.end()){ break; }
				if (bombz.size() == 0){ break; }
				else{ bombit++; }
			}
			if (bridgeit == bridgez.end()){ break; }
			if (alien_rows.size() != 0)
			{
				alien_rowsit = alien_rows.begin();
				for (int k = 0; k < alien_rows.size(); k++)
				{
					alienit = alien_rowsit->begin();
					while (alienit != alien_rowsit->end() && alien_rowsit->size() != 0 && bridgez.size() != 0)
					{
						if (bridgeit->check_alien_coll(alienit))
						{
							bridgeit->is_hit();
							if (bridgeit->ret_health() <= 0)
							{
								bridgez.erase(bridgeit++);
							}
							alien_rowsit->erase(alienit++);
						}
						else
						{
							alienit++;
						}
					}
				}
			}
			if (bridgeit != bridgez.end())
			{
				bridgeit->draw();
				bridgeit++;
			}
		}
	}
}

// Print that the game is paused
void print_game_paused(int number)
{
	string _str = "Game is paused. Press \"P\" to continue.";
	const char * c = _str.c_str();
	glRasterPos2f(300, 500);
	glColor3f(0.0f, 0.0f, 0.0f);
	int len, i;
	len = (int)strlen(c);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c[i]);
	}
	glutPostRedisplay();
}

