#include "../../headers/Log/Log.h"

Log *Log::log = 0;

Log* Log::instance() {
	if(!log)
		log = new Log;
	return log;
}

void Log::closeLog(){
	delete log;
}

Log::Log() {
}

Log::~Log() {

	std::string fileNameError("Log-ERROR-Game.txt");
	std::string fileNameWarning("Log-WARNING-Game.txt");
	std::string fileNameInfo("Log-INFO-Game.txt");

	std::ofstream outError(fileNameError);
	outError << dataError_;
	outError.close();

	std::ofstream outWarning(fileNameWarning);
	outWarning << dataWarning_;
	outWarning.close();

	std::ofstream outInfo(fileNameInfo);
	outInfo << dataInfo_;
	outInfo.close();
}

void Log::append(std::string msg, LOG_TYPE logType) {

	switch (logType) {
	case INFO:
		dataInfo_.append(timestamp() + ": " + msg);
		break;
	case WARNING:
		dataWarning_.append(timestamp() + ": " + msg);
		break;
	case ERROR:
		dataError_.append(timestamp() + ": " + msg);
		break;
	}

}

std::string Log::timestamp() {
	time_t now = time(0);
	std::string dt = ctime(&now);
	return dt.substr(0, dt.size() - 1);
}
