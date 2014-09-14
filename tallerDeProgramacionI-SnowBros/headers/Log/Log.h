#ifndef LOG_H_
#define LOG_H_

#include <string>
#include <iostream>
#include <fstream>
#include <time.h>

class Log {
public:

	typedef enum {
		INFO, WARNING, ERROR,
	} LOG_TYPE;

    static Log *instance();

    void append(std::string msg, LOG_TYPE logType);
    void closeLog();


private:

	static Log *log;
	std::string timestamp();
};

#endif /* LOG_H_ */
