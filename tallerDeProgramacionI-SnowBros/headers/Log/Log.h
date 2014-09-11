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

	Log();
	virtual ~Log();

	static Log *log;

	std::string dataError_;
	std::string dataWarning_;
	std::string dataInfo_;
	std::string timestamp();
};

#endif /* LOG_H_ */
