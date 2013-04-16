#include <cstdio>
#include <string>
#include "ctoml/src/include/toml.h"

void setupToml(CToml* toml){
	toml->open(".plcolors.toml");
	toml->parse();
}
	
int main(){
	CToml toml;
	setupToml(&toml);

	printf("%d",  toml["PATH_BG"].as_int());
	return 0;
}




















