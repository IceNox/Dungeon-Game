#include "Font.h"

Font::Font()
{
	
}

void Font::set_variables(std::string type)
{
	if (type == "small") {
		characterW.push_back(8);
		characterW.push_back(4);
		characterW.push_back(12);
		characterW.push_back(20);
		characterW.push_back(20);
		characterW.push_back(20);
		characterW.push_back(16);
		characterW.push_back(4);
		characterW.push_back(8);
		characterW.push_back(8);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(4);
		characterW.push_back(12);
		characterW.push_back(4);
		characterW.push_back(20);
		characterW.push_back(12); // 0
		characterW.push_back(4);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(12); // 9
		characterW.push_back(4);
		characterW.push_back(4);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(8);
		characterW.push_back(20);
		characterW.push_back(16); // A
		characterW.push_back(16);
		characterW.push_back(12);
		characterW.push_back(16);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(16);
		characterW.push_back(16);
		characterW.push_back(4);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(20);
		characterW.push_back(16);
		characterW.push_back(16);
		characterW.push_back(12);
		characterW.push_back(16);
		characterW.push_back(12);
		characterW.push_back(16);
		characterW.push_back(12);
		characterW.push_back(16);
		characterW.push_back(12);
		characterW.push_back(20);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(12); // Z
		characterW.push_back(8);
		characterW.push_back(20);
		characterW.push_back(8);
		characterW.push_back(12);
		characterW.push_back(16);
		characterW.push_back(8);
		characterW.push_back(16); // A
		characterW.push_back(16);
		characterW.push_back(12);
		characterW.push_back(16);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(16);
		characterW.push_back(16);
		characterW.push_back(4);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(20);
		characterW.push_back(16);
		characterW.push_back(16);
		characterW.push_back(12);
		characterW.push_back(16);
		characterW.push_back(12);
		characterW.push_back(16);
		characterW.push_back(12);
		characterW.push_back(16);
		characterW.push_back(12);
		characterW.push_back(20);
		characterW.push_back(12);
		characterW.push_back(12);
		characterW.push_back(12); // Z
		characterW.push_back(12);
		characterW.push_back(4);
		characterW.push_back(12);
		characterW.push_back(16);
		characterW.push_back(8);
	}
	else if (type == "large") {
		for (int i = 0; i < 96; i++) {
			characterW.push_back(32);
		}
	}
}