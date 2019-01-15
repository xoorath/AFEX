#pragma once

#include <easyloggingpp/easylogging++.h>

#define AFEX_ASSERT_TRUE(cnd) if(false == !!(cnd)) { LOG(ERROR) << #cnd ; __debugbreak(); }
#define AFEX_ASSERT_FALSE(cnd) if(true == !!(cnd)) { LOG(ERROR) << #cnd ; __debugbreak(); }
#define AFEX_ENSURE_TRUE(cnd) afex::log::LogIf(false == !!(cnd), true, "Condition failed: %v", #cnd)
#define AFEX_ENSURE_FALSE(cnd) !afex::log::LogIf(true == !!(cnd), true, "Condition failed: %v", #cnd)

namespace afex {
namespace log {
void Init();

template<typename... Args>
__forceinline bool LogIf(bool condition, bool breakIf, char const* fmt = nullptr, Args&&... args) {
	if (condition) {
		el::Logger* errorLogger = el::Loggers::getLogger("default", false);
		if (nullptr != errorLogger) {
			errorLogger->error(fmt, std::forward<Args>(args)...);
		}
		if(breakIf) __debugbreak();
	}
	return !condition;
}

}
}