////////////////////////////////////////////////////////////
//
// File: drawtools.cpp
// Author: Sven Goossens ( largely based on schematic.h by Patrick Groeneveld )
// Purpose:
//      Extending the dlist and item classes to provide a set of 'drawtools'
//      Contains an implementation of the pixel class, and skeleton for the
//      Line class
//
///////////////////////////////////////////////////////////

#ifndef DRAWTOOLS_H // this avoids multiple inclusion of this file
#define DRAWTOOLS_H

#include "dlist.h"  // to include the dlist class
#include <string>
#include <string.h>
#include <list>

using namespace std;

class line;          // forward: the classes in this file
class pixel;
class text;
class player;
class laser;
class alien;

// class declarations
     ///////////////////////////////////////////////////////
    //
   //  pixel class.
  //
 ///////////////////////////////////////////////////////

class pixel :
             public item    // inherits from item because its part of a DrawList
{
public:
	// constructors. Specify the list of the item, postion and color
	pixel(dlist* list, const float p[2], const float color[3]);
	~pixel(void);

	void print() const;   // print myself in EDIF++ format
	void draw(void) const;// draw myself to the screen

	// you can add more public members here, if needed

private:
	float _p[2];    // Coordinates
	float _color[3];
	// you can add more private members here, if needed


};

     ///////////////////////////////////////////////////////
    //
   //  line class.
  //
 ///////////////////////////////////////////////////////

class line :
             public item    // inherits from item because its part of a DrawList
{
public:
	// constructors. Specify the list of the line, postion, color and width
	line(dlist* list, const float p1[2], const float p2[2],
									const float color[3], const float lineWidth);
	~line(void);

	void print() const;   // print myself in EDIF++ format
	void draw(void) const;// draw myself to the screen

	// you can add more public members here, if needed

private:
	// you can add more private members here, if needed
	float _color[3];
	float _p1[2];
	float _p2[2];
	float _lineWidth;
};

     ///////////////////////////////////////////////////////
    //
   //  text class.
  //
 ///////////////////////////////////////////////////////

class text :
			public item    // inherits from item because its part of a DrawList
{
public:
	text::text(dlist* list, const float p1[2], const float color[3], std::string str, float alpha, char direction);
	~text(void);

	void print() const;   // print myself in EDIF++ format
	void draw(void) const;// draw myself to the screen
	void special_draw(void);
	void set_alpha(int alpha);
	float ret_alpha();

	// you can add more public members here, if needed

private:
	char _direction;
	float _alpha;
	std::string _str;
	float _p[2];
	float _color[3];
	// you can add more private members here, if needed

};
#endif

class player :
	public item
{
	public:
		player(dlist* list);
		~player(void);

		void move_right(); // move right
		void move_left(); // move left
		void draw(void) const;
		void is_hit(int hit);
		void movement(char movement);
		void move(float change);  // move
		float weap_x();			// return x coorcinate of weapon
		float weap_y();			// return y coordinate of weapon
		float x_loc();			// return x location of alien
		float y_loc();			// return y location of alien
		int width();			// return width
		int height();			// return height
		int ret_score();		// return score
		int ret_health();		// return health
		char ret_move();		// return movement
		void reset();			// reset to initial values
		void leveled();			// level up
		void restore_score();

		void score(int score);	// incremeent with score
			
	private:
		float _x;	// x coord
		float _y;	// y coord	
		char _movement; // movement
		int _health;	// health
		int _score;		// score
		int _weapon_x;	// weapon x coord
		int	_weapon_y;	// weapon y coord
		int _level;
		int _score_round;

};

class laser :
	public item
{
public:
	laser(dlist* list, int x, int y);
	~laser();

	void draw(void) const;
	void update();	// update location laser
	int height();	// return y coord of laser
	int x_loc();	// return x coord of laser

private:
	int _laser_x;	//x coord
	int _laser_y;	// y coord
};

class alien :
	public item
{
	public:
		alien(dlist* list, float x, float y, int score, char movement, float speed_factor);
		~alien(void);

			
		void draw(void) const;		// draw
		void update();					// update position laser
		void set_movement(char movement); // set movement alien
		void move_right();
		void move_left();	
		bool check_collision(list<laser>::iterator shot); // check collision laser
		bool check_player_collision(player* game);			// check collision player
		void move_down();
		// return several properties of alien
		static char _movement;
		int xCoord();
		int yCoord();
		int height();
		int width();
		int weap_x();
		int weap_y();
		int score();


		private:
		float _alien_x;
		float _alien_y;
		int _score;
		float _height = 30;
		float _width = 30;
		float _speed_factor;
};

class bomb :
	public item
{
public:
	bomb(dlist* list, int x, int y);
	~bomb();

	void draw(void) const;
	void update();
	bool check_player_collision(player* game);

	// return several properties of the bomb
	int y_loc();
	int x_loc();
	int width();
	int height();

private:
	int _bomb_x;
	int _bomb_y;
	int _exist;
	int _width = 4;
	int _height = 10;

};

class specalien :
	public item
{
public:
	specalien(dlist* list, int score, int movement, float speed);
	~specalien(void);

	void draw(void) const;
	void update();
	void set_movement(char movement);
	void move();
	bool check_collision(list<laser>::iterator shot);

	//return several properties of specalien
	int ret_score();	// return worth of score
	static char _movement;
	int xCoord();
	int yCoord();
	int height();
	int width();


private:
	float _speed;
	float _specalien_x;
	float _specalien_y;
	int _score;
	float _height = 20;
	float _width = 60;
};

class bridge :
	public item
{
public:
	bridge(dlist* list, int p[2]);
	~bridge(void);

	void draw(void);
	void update();
	void is_hit();
	int ret_health();
	bool check_collision(list<bomb>::iterator bomb);
	bool check_collision(list<laser>::iterator shot);
	bool check_alien_coll(list<alien>::iterator alien);

private:
	float _Rectb11[2];
	float _Rectb12[2];
	float _Rectmain1[2];
	float _Rectmain2[2];
	float _Rectb21[2];
	float _Rectb22[2];
	float _Recthealth1[2];
	float _Recthealth2[2];
	int _health;
	float _height = 40;
	float _width = 120;
	int _p;
};

