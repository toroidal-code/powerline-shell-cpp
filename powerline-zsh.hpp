
#include <regex>
#include <boost/algorithm/string.hpp>
#include "boost/tuple/tuple.hpp"
#include <dirent.h>
#include "boost/lexical_cast.hpp"
#include "powerline.hpp"

#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

std::string exec(std::string);
void print(const std::list<std::string>);
void addHostnameIndicator(Powerline*);
void addCwdSegment(Powerline*,std::string,bool);
void addGitSegment(Powerline*);
void addBranchSegment(Powerline*,int,int);
void addRepoSegment(Powerline*,std::string);
char* getValidCwd();
int main();
