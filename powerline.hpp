#include <cstdio>
#include <list>
#include <iostream>
#include <boost/format.hpp>
#include <boost/utility.hpp>
#include <sstream>

// Seperators
#define SEPERATOR "⮀"
#define SEPERATOR_THIN "⮁"

class Powerline;

//The Segment container class

class Segment
{
public:
	Powerline* powerline;
	std::string content;
	int fg;
	int bg;
	std::string separator;
	int separator_fg;
	
	Segment(Powerline* _powerline, std::string _content, int _fg, int _bg, int _separator_fg=0, std::string _separator=SEPERATOR)
	{
		powerline = _powerline; 
		content = _content; 
		fg = _fg; 
		bg = _bg;
		separator = _separator;
		if (_separator_fg == 0){
			separator_fg = _bg;
		} else {
			separator_fg = _separator_fg;
		}
	}

	friend std::ostream& operator<< (std::ostream &out, Segment &segment);
	std::string draw(Powerline*, Segment*, bool);
};


//The Powerline container class

class Powerline
{
public:
	std::list<Segment> segments;
	std::string LSQESCRSQ = "\\[\\e%s\\]";
	std::string reset = " %f%k";
	std::string seperator = SEPERATOR;
	std::string seperator_thin = SEPERATOR_THIN;


	std::string color(int,int);
	std::string fgcolor(int);
	std::string bgcolor(int);
	void append(Segment);
	std::string draw();
};