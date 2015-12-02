#include "powerline-zsh.hpp"

/*
	These are the color settings for the prompt
*/

#define PATH_BG 237 		//dark grey
#define PATH_FG 250			//light grey
#define CWD_FG 254 			//near-white grey
#define SEPERATOR_FG 244
#define REPO_CLEAN_BG 148 	// a light green color
#define REPO_CLEAN_FG 16 	// black
#define REPO_DIRTY_BG 161 	// pink/red
#define REPO_DIRTY_FG 15 	// white
#define CMD_PASSED_BG 236
#define CMD_PASSED_FG 15
#define CMD_FAILED_BG 161
#define CMD_FAILED_FG 15
#define SVN_CHANGES_BG 148
#define SVN_CHANGES_FG 22 	// dark green
#define VIRTUAL_ENV_BG 35 	// a mid-tone green
#define VIRTUAL_ENV_FG 22
#define BRANCH_SYMBOL ⭠

static char cCurrentPath[FILENAME_MAX];

std::string exec(std::string cmd) {
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
    	if(fgets(buffer, 128, pipe) != NULL)
    		result += buffer;
    }
    pclose(pipe);
    return result;
}

void addCwdSegment(Powerline* powerline,std::string cwd,bool currentOnly = false){
	std::string home = getenv("HOME"); //grabs the path to our home
	//cwd = getenv("PWD");
	//std::cout << getenv("PWD") << std::endl;

	std::size_t found = cwd.find(home);

	if (found != std::string::npos && found < cwd.size()){
		size_t end = found + home.length();
		cwd.replace(found,end,"~");
	}

	if (strcmp(&cwd[0], "/") == 0){
		std::string temp;
		for (int i = 1; i < cwd.length(); ++i){
			temp.push_back(cwd[i]);
		}
		cwd = temp;
	}

	std::list<std::string> names; //a list of the words in our path, split on '/'
	boost::split(names, cwd, boost::is_any_of("/"));

	if (cwd.size() == 0) names.push_back("/"); //if our list is 0, we clearly need a root indicator

	if (currentOnly){
		powerline->append(Segment(powerline, str(boost::format(" %s ") % names.back()),CWD_FG,PATH_BG));
	} else {
		for (std::list<std::string>::iterator it=names.begin() ; it != names.end(); ++it){
			//std::list<std::string>::iterator next = boost::next(it); //creates new list with next as head
			if (*it == names.back()){
				powerline->append(Segment(powerline, " " + *it + " ",CWD_FG,PATH_BG));
			} else {
				powerline->append(Segment(powerline, " " + *it + " ", PATH_FG, PATH_BG, SEPERATOR_FG, SEPERATOR_THIN));
			}
		}
	}
}

//TODO use git branch -v instead of git status to get 
//how far ahead/behind branch we are
boost::tuple<bool,bool,std::string> getGitStatus(){
	bool hasPendingCommits = true;
	bool hasUntrackedFiles = false;
	std::string originPosition;
	std::string output = exec("git status -v 2> /dev/null");

	//if (lines.front() == "fatal: Not a git repository (or any of the parent directories): .git"){
	//	return boost::make_tuple(hasPendingCommits,hasUntrackedFiles,"fatal");
	//}

	std::regex rgx("\\[(ahead|behind) (\\d+)\\]");
	std::smatch res;

	std::regex_search(output,res,rgx);
	if (!(res.empty())) {
		std::string direction = res[1]; 
		std::string num = res[2];

		originPosition += " " + num;
		if (strcmp(direction.c_str(),"behind") == 0){
			originPosition += "⇣";
		}
		if (strcmp(direction.c_str(),"ahead") == 0){
			originPosition += "⇡";
		}
	}

	std::size_t clean = output.find("nothing to commit");
	if (clean!=std::string::npos) hasPendingCommits = false;


	std::size_t semiclean = output.find("nothing added to commit");
	if (semiclean!=std::string::npos) hasPendingCommits = false;

	std::size_t untracked = output.find("Untracked files");
	if (untracked!=std::string::npos) hasUntrackedFiles = true;

	return boost::make_tuple(hasPendingCommits,hasUntrackedFiles,originPosition);
}

void addGitSegment(Powerline* powerline){

	//Gets the output of git branch for the line with '*' and store it in vector
	std::string output = exec("git branch 2> /dev/null | grep -e '\\*'");
	//Unfortunately, the string for the branch probably has a newline character, so we need to remove it
	output.erase(std::remove(output.begin(), output.end(), '\n'), output.end());
	std::vector<std::string> parts;
	boost::split(parts, output, boost::is_any_of(" "));

	std::string branch; // our empty string for concatenation, the branch status
	boost::tuple<bool,bool,std::string> gitStatus = getGitStatus();

	//if (strcmp(boost::get<2>(gitStatus).c_str(),"fatal") == 0){
	//	return;
	//}

	branch += parts[1] + boost::get<2>(gitStatus);

	if (boost::get<1>(gitStatus)){
		branch += " +";
	}

	int bg = REPO_CLEAN_BG;
	int fg = REPO_CLEAN_FG;

	if (boost::get<0>(gitStatus)){
		bg = REPO_DIRTY_BG;
		fg = REPO_DIRTY_FG;
	}
	//addBranchSegment(powerline,fg,bg); //bg-36
	powerline->append(Segment(powerline, " " + branch + " ",fg,bg));
}

void addBranchSegment(Powerline* powerline,int fg,int bg){
	powerline->append(Segment(powerline, " ⭠ " ,fg,bg,fg, SEPERATOR_THIN));
	
}

bool isGit(){
	if (strcmp(exec("git rev-parse --is-inside-work-tree 2> /dev/null").c_str(),"true\n")==0){
		return true;
	}
	return false;
}

void addRepoSegment(Powerline* powerline, std::string cwd){
	if (isGit()){
		addGitSegment(powerline);
	}
}

void addHostnameIndicator(Powerline* powerline){
	std::string output = exec("hostname -s");
	output.erase(std::remove(output.begin(), output.end(), '\n'), output.end());
	powerline->append(Segment(powerline, " " + output + " ",250,240));
}

void addUserIndicator(Powerline* powerline){
	std::string output = getenv("USER");
	output.erase(std::remove(output.begin(), output.end(), '\n'), output.end());
	powerline->append(Segment(powerline, " " + output + " ",250,238));
}

void addRootIndicator(Powerline* powerline,int error){
	int bg = CMD_PASSED_BG;
	int fg = CMD_PASSED_FG;
	if (error != 0){
		bg = CMD_FAILED_BG;
		fg = CMD_FAILED_FG;
	}
	const char* user = getenv("USER");
	std::string prompt = "$";
	if (strcmp(user, "root") == 0){
		prompt = "#";
	}
	powerline->append(Segment(powerline, " " + prompt,fg,bg));
}

char* getValidCwd(){
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
	return cCurrentPath;
}

int main() {
	Powerline p;
	std::string cwd = getValidCwd();
	addHostnameIndicator(&p);
	addUserIndicator(&p);
   	addCwdSegment(&p, cwd,true);
	addRepoSegment(&p, cwd);
	addRootIndicator(&p,0);
	std::cout << p.draw() << std::endl;
	return 0;
}
