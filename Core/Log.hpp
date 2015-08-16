/*
C++ log class
Copyright (C) 2015 nerox8664
nerox8664@gmail.com
version 0.2
*/

#ifndef __LOG_H__
#define __LOG_H__

#include <iostream>
#include <fstream>

///! Configuration
//Enable thread support
#define __LOG_THREADS__

//Enable global hot-macroses and single log
#define __LOG_SINGLETONE__

//Enable flush after put
#define __LOG_FLUSH_EVER__
///!


#ifdef __LOG_THREADS__
  #include <mutex>
#endif


class Log {
  private:

    //file handler
    std::ofstream log;
    bool echo;
    int baseLevel;

    //log prefixes
    std::string prefixes[5] = {
      std::string(">"),
      std::string(">~"),
      std::string(">>(!)"),
      std::string(">>(E)"),
      std::string("(===E===)")
    };


  #ifdef __LOG_THREADS__
    std::mutex logMutex;
  #endif

  #ifdef __LOG_SINGLETONE__
    static Log* instance;
  #endif

  public:

    /*! \enum logLevels
     * Logging levels enumeration.
     */
    enum logLevels {
      //! show all messages and warnings
      LOG_DEBUG=0,
      //! show debug (same LOG_DEFAULT)
      LOG_DEFAULT,
      //! only warnings
      LOG_WARNING,
      //! only errors
      LOG_ERROR,
      //! only critical errors
      LOG_CRITICAL
    };

    //! Create engine object with OpenGL initialization
    /*!
    \param fileName log file for creation/write
    \sa ~Log()
    */
    Log(std::string fileName, bool echo, Log::logLevels baseLevel);

    //! Delete object and sync cache
    ~Log();

    //! Put message to log file
    /*!
    \param logMessage message to write
    \param level log level
    */
    void put(std::string logMessage, Log::logLevels level);

    #ifdef __LOG_SINGLETONE__
      static Log* GetInstance();
    #endif

};

#ifdef __LOG_SINGLETONE__
  inline void log_put(std::string message) {Log::GetInstance()->put(message, Log::LOG_DEFAULT);}
  inline void log_debug(std::string message) {Log::GetInstance()->put(message, Log::LOG_DEBUG);}
  inline void log_warning(std::string message) {Log::GetInstance()->put(message, Log::LOG_WARNING);}
  inline void log_error(std::string message) {Log::GetInstance()->put(message, Log::LOG_ERROR);}
  inline void log_critical(std::string message) {Log::GetInstance()->put(message, Log::LOG_CRITICAL);}
#endif

#endif