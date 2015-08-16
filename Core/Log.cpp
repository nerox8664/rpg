#include "Log.hpp"

Log::Log(std::string fileName, bool echo = false, logLevels baseLevel=LOG_DEFAULT ):
echo(echo),baseLevel(baseLevel) {
  log.open(fileName.c_str());
  
  #ifdef __LOG_SINGLETONE__
    Log::instance = this;
  #endif
  
}

Log::~Log() {
  log.close();
}

#ifdef __LOG_SINGLETONE__
  Log* Log::instance = nullptr;
  
  Log* Log::GetInstance() {
    return instance;
  }
  
#endif

void Log::put(std::string logMessage, logLevels level) {
  if(level<baseLevel) return;
  
#ifdef _THREADS_
  logMutex.lock();
#endif

  log<<prefixes[level]<<logMessage<<std::endl;
  if(echo) std::cout<<prefixes[level]<<logMessage<<std::endl;
  
#ifdef __LOG_FLUSH_EVER__
  log.flush();
#endif
  
#ifdef _THREADS_
  logMutex.unlock();
#endif

}
