#include <Engine/Log.h>

#include <easyloggingpp/easylogging++.h>
INITIALIZE_EASYLOGGINGPP

namespace afex {
namespace log {
void Init() {
	::el::Configurations conf("log-config.txt");
	::el::Loggers::reconfigureAllLoggers(conf);
	START_EASYLOGGINGPP(__argc, __argv);
	LOG(INFO) << "Application Started";
}
}
}