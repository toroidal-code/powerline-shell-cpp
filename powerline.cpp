#include "powerline.hpp"

/*------------[CLASSES]------------------*/

/*[POWERLINE]*/

//performs string formatting for fg and bg colors
std::string Powerline::color(int prefix,int code){
	if (prefix == 38) {
		return str(boost::format("%%F{%d}") % code);
	} else if (prefix == 48) {
		return str(boost::format("%%K{%d}") % code);
	} else {
		return "Invalid";
	}
}

//returns a string with the fgcolor in proper format
std::string Powerline::fgcolor(int code){
	return color(38, code);
}

//returns a string with the bgcolor in proper format
std::string Powerline::bgcolor(int code){
	return color(48, code);
}

//Appends a given Segment object to the Powerline.segments list
void Powerline::append(Segment segment){
	segments.push_back(segment);
}

//'draws' the powerline to a string
std::string Powerline::draw(){
	std::list<std::string> line; //a temporary list to hold the strings we get from the segments
	std::string pwrline;

	for (std::list<Segment>::iterator it=segments.begin() ; it != segments.end(); ++it){
		std::list<Segment>::iterator next = boost::next(it); //creates new iterator with next
		Segment nextSeg = *next;
		if (next == segments.end()){
			pwrline.append(it->draw(this, &nextSeg, false));
			//std::cout << "	This is the end " << std::endl;
		} else {
			pwrline.append(it->draw(this, &nextSeg, true));
		}
	}

	pwrline.append(reset);
	return pwrline;
}

/*[SEGMENT]*/

std::ostream& operator<<(std::ostream& os, Segment &segment){
    os << 
    "Content: " << segment.content << "\n" <<
    "FG: " << segment.fg << "\n" << 
    "BG: " << segment.bg << "\n" << 
    "Separator: " << segment.separator << "\n" << 
    "Separator FG: " << segment.separator_fg << "\n" << std::endl;
    return os;
}

std::string Segment::draw(Powerline* powerline, Segment* next_segment, bool flag=false){
	
	//if there is no next segment (signaled by flag=true), 
	//then the separator background shout be our reset value
	std::string separator_bg = powerline->reset;

	if (flag){
		separator_bg = powerline->bgcolor(next_segment->bg);
	}

	std::string seg;
	seg += powerline->fgcolor(this->fg) 
		+ powerline->bgcolor(this->bg)
		+ this->content
		+ separator_bg
		+ powerline->fgcolor(this->separator_fg)
		+ this->separator;
	return seg;
}