#include "Logging.h"

Logger* Logger::Get()
{
	static Logger* kInstace = nullptr;
	if (!kInstace)
	{
		kInstace = new Logger;
	}
	return kInstace;
}

void Logger::Add(const char* msg)
{
	m_log.push_back(std::string(msg));
}

void Logger::FlushToFile()
{
	FILE* logFile;
	fopen_s(&logFile, "log.txt", "w");
	if (!logFile)
	{
		return;
	}
	for (const auto l : m_log)
	{
		fwrite(l.c_str(), sizeof(char), l.size(), logFile);
	}
	fclose(logFile);
}
