#include <cstdio>
#include <string>
#include "ctoml/src/include/toml.h"


void setupToml(CToml* toml){
	toml->open(".plcolors.toml");
	toml->parse();
}
/*
int main(){
    CToml toml;
    toml.open(".plcolors.toml");
    toml.parse();
    CTomlValue val = toml.get("PATH_BG");
    printf("%d\n", val.as_int());
    return 0;
}*/


















