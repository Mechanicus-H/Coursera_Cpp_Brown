#pragma once

#include <chrono>
#include <string>
#include <iostream>
#include <map>


class LogDuration
{
private:
	static std::map<std::string, LogDuration*> m_mapProfilers;
	std::chrono::steady_clock::time_point start;
	std::string message;
public:
	explicit LogDuration(const std::string& mes = "") : start(std::chrono::steady_clock::now()), message(mes)
	{}
	~LogDuration()
	{
		auto end = std::chrono::steady_clock::now();

		auto duration = end - start;
		std::cerr << message << ' ';
		std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << "ms\n";
	}
	static void __StartProfiler(std::string strName)
	{
		if (!strName.empty() && m_mapProfilers.count(strName) == 0)
		{
			m_mapProfilers[strName] = new LogDuration(strName);
		}
	}
	static void __StopProfiler(std::string strName)
	{
		if (!strName.empty() && m_mapProfilers.count(strName) > 0)
		{
			auto pr = m_mapProfilers[strName];
			if (pr)
				delete pr;
			pr = nullptr;
			m_mapProfilers.erase(strName);
		}
	}
};

std::map<std::string, LogDuration*> LogDuration::m_mapProfilers;

#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)

#define LOG_DURATION(message)				\
		LogDuration UNIQ_ID(__LINE__)(message)

#define START_PROFILER(name) \
	LogDuration::__StartProfiler( std::string(name) );

#define STOP_PROFILER(name) \
	LogDuration::__StopProfiler( std::string(name) );
	

