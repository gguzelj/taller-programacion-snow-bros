#ifndef LOG_H_
#define LOG_H_

#include <string>
#include <iostream>
#include <fstream>
#include <time.h>

class Log {
public:

	typedef enum {
		INFO = 0, WARNING, ERROR,
	} LOG_TYPE;

    static Log *instance();
    LOG_TYPE loggerLevel;
    void append(std::string msg, LOG_TYPE logType);
    void closeLog();


private:

	static Log *log;
	std::string logTypeToString(LOG_TYPE logType);
	void printNewLogger();
	std::string timestamp();
};

#endif /* LOG_H_ */
