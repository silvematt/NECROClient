#ifndef NECRO_FILE_LOGGER_H
#define NECRO_FILE_LOGGER_H

#include <fstream>

#include "Logger.h"

const std::string DEFAULT_LOG_FILE_NAME = "ServerLog.txt";

class FileLogger : public Logger
{
private:
	std::ofstream logFile;

public:
	static FileLogger* Instance();

	FileLogger();
	FileLogger(const std::string& filePath);

	~FileLogger();

	virtual void Log(const std::string& message, Logger::LogLevel lvl, const char* file, int line, ...) override;
};

#endif
