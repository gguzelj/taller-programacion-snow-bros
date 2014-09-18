#include "../../headers/Log/Log.h"

Log *Log::log = 0;

Log* Log::instance() {
	if (!log)
		log = new Log;
	return log;
}

void Log::closeLog() {
	delete log;
}

void Log::append(std::string msg, LOG_TYPE logType) {

	std::string fileNameError("Log-ERROR-Game.txt");
	std::string fileNameWarning("Log-WARNING-Game.txt");
	std::string fileNameInfo("Log-INFO-Game.txt");

	switch (logType) {
	case INFO: {
		std::ofstream outInfo(fileNameInfo, std::ios_base::app);
		outInfo << timestamp() + ": INFO " + msg + "\n";
		outInfo.close();
		break;
	}
	case WARNING: {
		std::ofstream outWarning(fileNameWarning, std::ios_base::app);
		outWarning << timestamp() + ": WARNING " + msg + "\n";
		outWarning.close();
		break;
	}
	case ERROR: {
		std::ofstream outError(fileNameError, std::ios_base::app);
		outError << timestamp() + ": ERROR " + msg + "\n";
		outError.close();
		break;
	}
	}
}

std::string Log::timestamp() {
	time_t now = time(0);
	std::string dt = ctime(&now);
	return dt.substr(0, dt.size() - 1);
}
