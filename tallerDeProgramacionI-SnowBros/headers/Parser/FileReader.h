#ifndef FILEREADER_H_
#define FILEREADER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>

class FileReader {
public:

	static std::string read(std::string path);

};

#endif /* FILEREADER_H_ */
