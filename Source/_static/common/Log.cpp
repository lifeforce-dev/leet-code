//---------------------------------------------------------------
//
// Log.cpp
//

#include "Log.h"

namespace Log {

LoggerManager::LoggerManager()
{
	Init();
}

LoggerManager::~LoggerManager()
{

}

void LoggerManager::CreateLogger(const std::string& loggerName)
{
	// Initialize the log sinks.
	std::vector<spdlog::sink_ptr> sinks;

	using namespace spdlog::sinks;

	auto msvcSink = std::make_shared<msvc_sink_mt>();
	msvcSink->set_level(spdlog::level::trace);
	msvcSink->set_pattern("%^%l%$ %Y-%m-%d %T.%f [%n] %v (%@)");

	auto logger = std::make_shared<spdlog::logger>(loggerName, msvcSink);

	spdlog::register_logger(logger);
}

void LoggerManager::Init()
{
	spdlog::init_thread_pool(32768, 1);
	spdlog::flush_every(std::chrono::seconds(1));
}

void CreateAndRegisterLogger(const std::string& name)
{
	static LoggerManager loggerManager;
	loggerManager.CreateLogger(name);
}

} // namespace Logger
