#pragma once
#include <iostream>
#include <fstream>

class Output
{
private:
	std::fstream output;
public:
	Output(std::string textFileName) {
		output.open(textFileName, std::ios::out);
		if (!output.is_open()) {
			std::cerr << "We're in a problem :)" << std::endl;
			
		}
	}
	
	~Output() {
		output.close();
	}

	void writeTheFile(std::string input);
};