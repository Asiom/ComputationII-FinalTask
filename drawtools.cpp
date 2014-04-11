////////////////////////////////////////////////////////////
//
// File: drawtools.cpp
// Author: Sven Goossens
// Purpose:
//      Extending the dlist and item classes to provide a set of 'drawtools'
//      Contains an implementation of the pixel class, and skeleton for the
//      Line class
//
///////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <string>
#include <iostream>


#include "glut.h"
#include "drawtools.h"

using namespace std;

// class definitions:

     ///////////////////////////////////////////////////////
    //
   //  pixel class.
  //
 ///////////////////////////////////////////////////////
/*
      Please note that the pixel class is an incredibly inefficient way to draw
      1(!!) pixel to the screen. This class is mearly ment as an example of
      how to use the DrawList. 
*/
pixel::pixel(dlist* list, const float p[2], const float color[3])
	: item(list)    // call the ctor of item and chain into list
{

	if (p)
	_p[0] = p[0];// Store location of the pixel in the member variable _p
	_p[1] = p[1];

	if(!color) {      // When a null pointer is passed to this function
		_color[0] = 0;// black is used
		_color[1] = 0;
		_color[2] = 0;
	}
	else {
		_color[0] = color[0];
		_color[1] = color[1];
		_color[2] = color[2];
	}
}
//---------------------------------------------------------------------------
//
// draw myself on the screen
//
void pixel::draw(void) const
{
	glBegin(GL_POINTS);            // Start drawing mode

	// If a color was passed to the function, set it
	glColor3f(_color[0],_color[1], _color[2]);
	glVertex2fv(this->_p);        // Draw the pixel at (_p[0], _p[1])

	glEnd();
}
//---------------------------------------------------------------------------
//
// Print myself in ELIF format
//
void pixel::print(void) const
{
	printf(".pixel %.0f %.0f %.5f %.5f %.5f\n", _p[0], _p[1], _color[0],
															_color[1], _color[2]);
}
//---------------------------------------------------------------------------
//
// destructor of a pixel
//
pixel::~pixel(void)
{

}
//---------------------------------------------------------------------------
     ///////////////////////////////////////////////////////
    //
   //  line class.
  //
 ///////////////////////////////////////////////////////
line::line(dlist* list, const float p1[2], const float p2[2],
	const float color[3], const float lineWidth)
	: item(list)    // call the ctor of item and chain into list
{
	{
		_lineWidth = lineWidth;
		_p1[0] = p1[0];// Store location of the pixel in the member variable _p
		_p1[1] = p1[1];
		_p2[0] = p2[0];// Store location of the pixel in the member variable _p
		_p2[1] = p2[1];

		if (!color) {      // When a null pointer is passed to this function
			_color[0] = 0;// black is used
			_color[1] = 0;
			_color[2] = 0;
		}
		else {
			_color[0] = color[0];
			_color[1] = color[1];
			_color[2] = color[2];
		}
	}
}
//---------------------------------------------------------------------------
//
// destructor of a line
//
line::~line(void)
{

}
//---------------------------------------------------------------------------
//
// Print myself in EDIF++ format
//
void line::print(void) const
{
	printf(".line x1: %.0f y1: %.0f x2: %.0f y2: %.0f Cr%.5f Cb%.5f Cy%.5f\n", _p1[0], _p1[1], _p2[0], _p2[1], _color[0],
		_color[1], _color[2]);
	//TODO:  line::print(void), To be implemented by you
}
//---------------------------------------------------------------------------
//
// draw myself on the screen
//
void line::draw(void) const
{
	// TODO: line::draw(void), to be implemented by you
	// Aim for about 10 lines of code
	glLineWidth(this->_lineWidth);
	glBegin(GL_LINES);            // Start a new 'drawing' block.

	glColor3f(_color[0], _color[1], _color[2]);
	glVertex2fv(this->_p1);    // draw it
	glVertex2fv(this->_p2);    // draw it
	glEnd();
	line::print();

}

     ///////////////////////////////////////////////////////
    //
   //  text class.
  //
 ///////////////////////////////////////////////////////

text::text(dlist* list, const float p1[2], const float color[3], std::string str, float alpha, char direction)
	: item(list)    // call the ctor of item and chain into list
{
	{
		_direction = direction;
		_alpha = alpha;
		_str = str;
		_p[0] = p1[0];// Store location of the pixel in the member variable _p
		_p[1] = p1[1];

		if (!color) {      // When a null pointer is passed to this function
			_color[0] = 0;// black is used
			_color[1] = 0;
			_color[2] = 0;
		}
		else {
			_color[0] = color[0];
			_color[1] = color[1];
			_color[2] = color[2];
		}
	}
}

text::~text(void){}

void text::print() const   // print myself in EDIF++ format
{
	//printf(".pixel %.0f %.0f %.5f %.5f %.5f %s\n", _p[0], _p[1], _color[0],	_color[1], _color[2], _str);
}
void text::draw(void) const// draw myself to the screen
{
	glColor3f(_color[0], _color[1], _color[2]);
	glRasterPos2f(_p[0], _p[1]);
	for(char c: _str)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);
	}
}

void text::set_alpha(int alpha)
{
	_alpha = alpha;
}

float text::ret_alpha()
{
	return _alpha;
}

void text::special_draw(void)
{
	{
		if (_alpha != 0)
		{
			// this was for the alpha/transparancy :<
			//glEnable(GL_BLEND); // Enable Blending
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			const char * c = _str.c_str();
			glRasterPos2f(_p[0], _p[1]);
			if (_direction == 'd')
			{
				glColor3f(1.0f, 0.0f, 0.0f);
			}
			else if (_direction == 'u')
			{
				glColor3f(0.0f, 1.0f, 0.0f);
			}
			else
			{
				glColor3f(0.0f, 0.0f, 0.0f);
			}
			int len, i;
			len = (int)strlen(c);
			for (i = 0; i < len; i++) {
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c[i]);
			}

		}
	}
	if (_direction == 'd')
	{
		_p[1] -= 0.5;
		_alpha = _alpha + 0.08;
	}
	else if (_direction == 'u')
	{
		_p[1] += 0.5;
		_alpha = _alpha + 0.1;
	}
}



///////////////////////////////////////////////////////
//
//  PLAYER class.
//
///////////////////////////////////////////////////////

player::player(dlist* list)
	:item(list)
{
	_x = 100;
	_y = 100;
	_health = 3;
	_score = 0;
	_level = 0;
	_score_round = 0;
}

player::~player()
{}

void player::draw(void) const
{
	glColor3f(1.0f, 0.0f, 0.0f);
	glRecti(this->_x + 15.0f,this->_y + 45.0f, this->_x + 35.0f, this->_y + 55.0f);
	glColor3f(0.3f, 1.0f, 1.0f);
	glRecti(this->_x, this->_y, this->_x + 50.0f, this->_y + 50.0f);
}

void player::move_right()
{
	if (_x < 900 )
	{
		_x = _x + 50;
	}
	else
	{
		_x = 900;
	}
}

void player::move_left()
{
	if (_x > 100)
	{
		_x = _x - 50;
	}
	else
	{
		_x = 100;
	}
}

void player:: movement(char movement)
{
	_movement = movement;
}

void player::move(float change)
{
	switch (_movement)
	{
	case 'r':
		{
				if ((_x + change) < 950)
				{
					_x = _x + change;
				}
				break;
		}
	case 'l':
		{
				if ((_x - change) > 5)
				{
					_x = _x - change;
				}
				break;
		}
	case 's':
		{
					_x = _x;
					break;
		}
	}
}

void player::is_hit(int hit)
{
	_health = _health - hit;
}

void player::reset()
{
	_x = 100;
	_y = 100;
	_health = 3;
	_score = 0;
}

float player::weap_x()
{
	return _x + 25;
}

float player::weap_y()
{
	return _y + 55;
}

void player::score(int score)
{
	_score = _score + score;
	_score_round = _score_round + score;
}

void player::leveled()
{
	_health = _health + 1;
	_level = _level + 1;
	_score_round = 0;
}

float player::x_loc(){	return _x;}
float player::y_loc(){	return _y;}
int player::width(){	return 50;}
int player::height(){	return 50;}
int player::ret_score(){ return _score; }
int player::ret_health(){ return _health; }
char player::ret_move(){ return _movement; }

void player::restore_score()
{
	_score = _score - _score_round;
	_score_round = 0;
}


///////////////////////////////////////////////////////
//
//  LASER class.
//
///////////////////////////////////////////////////////

laser::laser(dlist* list, int x, int y)
:item(list)
{
	_laser_x = x;
	_laser_y = y;
}
laser::~laser(){}

void laser::draw(void) const
{
	glLineWidth(5);
	glBegin(GL_LINES);
	
	glColor3f(0.0f, 0.0f, 0.1f);
	glVertex2f(this->_laser_x, this->_laser_y);
	glVertex2f(this->_laser_x, this->_laser_y + 10);
	glEnd();
}

void laser::update()
{
	this->_laser_y = this->_laser_y + 5;
}

int laser::height()
{
	return _laser_y;
}

int laser::x_loc()
{
	return _laser_x;
}


///////////////////////////////////////////////////////
//
//  ALIEN class.
//
///////////////////////////////////////////////////////

alien::alien(dlist* list, float x, float y, int score, char movement, float speed_factor)
:item(list)
{
	_movement = movement;
	_alien_x = x;
	_alien_y = y;
	_score = score;
	_speed_factor = speed_factor;
}

alien::~alien(){}

void alien::update()
{
	switch (_movement)
	{
	case 'd':
		move_down();
		break;
	case 'r':
		move_right();
		break;
	case 'l':
		move_left();
		break;
	}
}

void alien::set_movement(char movement)
{
	_movement = movement;
}

void alien::move_right()
{
	_alien_x += 1 * _speed_factor;
}
void alien::move_left()
{
	_alien_x -= 1 * _speed_factor;
}

void alien::move_down()
{
	_alien_y -= 30;
}

void alien::draw(void) const
{
	glColor3f(1.0f, 1.0f, 0.0f);
	glRectf(_alien_x, _alien_y,_alien_x + _width,_alien_y + _height);
}

char alien::_movement = 'r';

int alien::xCoord()	{	return _alien_x;}
int alien::yCoord()	{	return _alien_y;}
int alien::height()	{	return _height;	}
int alien::width()	{	return _width;	}
int alien::score()	{	return _score; }

bool alien::check_collision(list<laser>::iterator shot)
{
	int laserY = shot->height();
	int laserX = shot->x_loc();
	if ((laserX) >= _alien_x){
		if (laserX <= (_alien_x + _width)){
			if ((laserY + 10) >= _alien_y){
				if (laserY <= (_alien_y + _height))
				{ 
					char i = 7;
					cout << i;
					return true; 
				}
				
			}
		}
	}
	return false;
}

bool alien::check_player_collision(player* game)
{
	int playerY = game->y_loc();
	int playerX = game->x_loc();
	int playerW = game->width();
	int playerh = game->height();
	if ((playerX + playerW) >= _alien_x){
		if (playerX <= (_alien_x + _width)){
			if ((playerY + playerh) >= _alien_y){
				if ((playerY) <= (_alien_y + _height)){ return true; }

			}
		}
	}
	return false;
}

int alien::weap_x()
{
	return _alien_x + (_width / 2) - 2;
}
int alien::weap_y()
{
	return _alien_y;
}
///////////////////////////////////////////////////////
//
//  BOMBS class.
//
///////////////////////////////////////////////////////

bomb::bomb(dlist* list, int x, int y)
:item(list)
{
	_bomb_x = x;
	_bomb_y = y;
}

bomb::~bomb(){}

void bomb::draw(void) const
{
	glColor3f(1.0f, 0.0f, 0.1f);
	glRectf(_bomb_x, _bomb_y - _height, _bomb_x + _width, _bomb_y);
}

void bomb::update()
{
	_bomb_y -= 10;
}

int bomb::y_loc(){ return _bomb_y; }
int bomb::x_loc(){ return _bomb_x; }
int bomb::width(){ return _width;  }
int bomb::height(){	return _height;}

bool bomb::check_player_collision(player* game)
{
	int playerY = game->y_loc();
	int playerX = game->x_loc();
	int playerW = game->width();
	int playerh = game->height();
	if ((playerX + playerW) >= _bomb_x){
		if (playerX <= (_bomb_x + _width)){
			if ((playerY + playerh) >= _bomb_y-_height){
				if ((playerY) <= (_bomb_y))
				{ return true; }

			}
		}
	}
	return false;
}

///////////////////////////////////////////////////////
//
//  SPECALIEN class.
//
///////////////////////////////////////////////////////

specalien::specalien(dlist* list, int score, int movement, float speed)
:item(list)
{
	_speed = speed;
	_specalien_y = 850;
	_score = score;
	switch (movement)
	{
	case 1:
	{
			  _movement = 'r';
			  _specalien_x = -100;
			  break;
	}
	case 2:
	{
			  _movement = 'l';
			  _specalien_x = 1100;
			  break;
	}
	}
}

specalien::~specalien(){}

void specalien::update()
{
	switch (_movement)
	{
		case 'l':
		{
					_specalien_x = _specalien_x - _speed * 2;
					break;
		}
		case 'r':
		{
					_specalien_x = _specalien_x +  _speed * 2;
					break;
		}
	}
}

void specalien::draw(void) const
{
	glColor3f(0.0f, 0.78f, 0.0f);
	glRectf(_specalien_x, _specalien_y, _specalien_x + _width, _specalien_y + _height);
}

char specalien::_movement = 'r';

int specalien::xCoord()	{ return _specalien_x; }
int specalien::yCoord()	{ return _specalien_y; }
int specalien::height()	{ return _height; }
int specalien::width()	{ return _width; }
int specalien::ret_score()	{ return _score; }

bool specalien::check_collision(list<laser>::iterator shot)
{
	int laserY = shot->height();
	int laserX = shot->x_loc();
	if ((laserX) >= _specalien_x){
		if (laserX <= (_specalien_x + _width)){
			if ((laserY + 10) >= _specalien_y){
				if (laserY <= (_specalien_y + _height))
				{
					return true;
				}

			}
		}
	}
	return false;
}

bridge::bridge(dlist* list, int p[2])
:item(list)
{
	_p = p[1];
	_health = 10;
	_Rectb11[0] = p[0] - _width / 2;
	_Rectb11[1] = p[1];
	_Rectb12[0] = p[0] + _width / 3 - _width / 2;
	_Rectb12[1] = (p[1] + _height / 2);

	_Rectb21[0] = p[0] + _width / 2 - _width / 3;
	_Rectb21[1] = p[1];
	_Rectb22[0] = p[0] + _width/2;
	_Rectb22[1] = p[1] + _height/2 ;

	_Rectmain1[0] = p[0] - _width / 2;
	_Rectmain1[1] = p[1] + _height / 2;
	_Rectmain2[0] = p[0] + _width/2 ;
	_Rectmain2[1] = p[1] + _height;

	_Recthealth1[0] = p[0] - 4 - _width / 2, p[1];
	_Recthealth1[1] = p[1];
	_Recthealth2[0] = p[0] - _width / 2;
	_Recthealth2[1] = p[1] + _height / (7 - _health);
}
bridge:: ~bridge(void)
{}

void bridge::draw(void)
{
	_Recthealth2[1] = _p + (_height * _health) / 10;
	float _clrHealth[3] = { 1.0f, 0.0f, 0.0f };
	float _clrRect[3] = { 0.4f, 0.3f, 0.7f };
	glColor3fv(_clrRect);
	glRectfv(_Rectb11, _Rectb12);
	glRectfv(_Rectb21, _Rectb22);
	glRectfv(_Rectmain1, _Rectmain2);
	glColor3fv(_clrHealth);
	glRectfv(_Recthealth1, _Recthealth2);
}

void bridge::is_hit()
{
	_health = _health - 1;
}
int bridge::ret_health()
{
	return _health;
}
bool bridge::check_collision(list<bomb>::iterator bomb)
{
	{
		int laserY = bomb->y_loc();
		int laserX = bomb->x_loc();
		if ((laserX + bomb->width()) >= _Rectmain1[0]){
			if (laserX <= (_Rectmain2[0])){
				if ((laserY + bomb->height()) >= _Rectmain1[1]){
					if (laserY <= (_Rectmain2[1]))
					{
						return true;
					}

				}
			}
		}
		return false;
	}

}
bool bridge::check_collision(list<laser>::iterator shot)
{
	{
		int laserY = shot->height();
		int laserX = shot->x_loc();
		if ((laserX) >= _Rectb11[0]){
			if (laserX <= (_Rectb12[0])){
				if ((laserY + 10) >= _Rectb11[1]){
					if (laserY <= (_Rectb12[1]))
					{
						return true;
					}

				}
			}
		}
		if ((laserX) >= _Rectb21[0]){
			if (laserX <= (_Rectb22[0])){
				if ((laserY + 10) >= _Rectb21[1]){
					if (laserY <= (_Rectb22[1]))
					{
						return true;
					}

				}
			}
		}
		if ((laserX) >= _Rectmain1[0]+(_width/3)){
			if (laserX <= (_Rectmain2[0]-(_width/3))){
				if ((laserY + 10) >= _Rectmain1[1]){
					if (laserY <= (_Rectmain2[1]))
					{
						return true;
					}

				}
			}
		}
		return false;
	}

}

bool bridge::check_alien_coll(list<alien>::iterator alien)
{
	{
		int alienY = alien->yCoord();
		int alienX = alien->xCoord();
		if ((alienX + alien->width()) >= _Rectmain1[0]){
			if (alienX <= (_Rectmain2[0])){
				if ((alienY + alien->height()) >= _Rectmain1[1]){
					if (alienY <= (_Rectmain2[1]))
					{
						return true;
					}

				}
			}
		}
		return false;
	}
}



