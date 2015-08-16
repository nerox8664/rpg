/*
C++ INI files parser
Copyright (C) 2014 nerox8664
nerox8664@gmail.com
version 0.1.1
*/

#ifndef __INI__CONF__
#define __INI__CONF__

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>

/*! \class INIConf
 *  \brief Configuration class.
 *
 * Read and write ini-files
 * [Section]
 * Key = Value
 !! Only 1-line supports
 */

//#define __DEBUG_OUTPUT__ 1
#define __INI_SINGLETON__

struct INIConfEntity {
    std::string svalue;
    int ivalue;
    float fvalue;
    bool lvalue;
};


class INIConf {
private:

  #ifdef __INI_SINGLETON__
    static INIConf* instance;
  #endif

  //! standard map [section, key] => [value]
  std::map< std::string, std::map <std::string, INIConfEntity > > iniMap;

  //Parse globals
  void parse ( std::string filename );
  std::string section;

  //General flags
  bool isLoadedFlag;

  //Set and convert value(s)
  void setUpdate(std::string section, std::string key, std::string value);


  /* CONVERTORS */
  static inline float stringToFloat(std::string);
  static inline int stringToInt(std::string);
  static inline bool stringToBool(std::string);
  static inline std::string stringToString(std::string); //proxy

  std::vector<std::string> split(const std::string &s, char delim);

public:

  //! Empty constructor of INI reader
  /*!
  \sa INIConf( std::string filename)
  */
  INIConf (  );

  //! Constructor of INI reader with load
  /*!
  Create object and open ini-file to read. Parse and make associative array from configuration
  \param filename name of configuration file
  \sa INIConf( )
  */
  INIConf ( std::string filename );

  //! Destructor of INI reader
  /*!
  Close file without save and delete object
  */
  ~INIConf (  );


  #ifdef __INI_SINGLETON__
    static INIConf* GetInstance();
  #endif
  
  //! Check for loaded
  /*!
  Returns true if load was called;
  */
  bool isLoaded();


  //! Return true, if config exists
  bool isSet(std::string section, std::string key);

  //! Get int value from config
  /*!
  Get value from configuration file
  \param section section of configuration file
  \param key key in this section
  \sa getFloat( )
  \return int value
  */
  int getInt( std::string section, std::string key);

  //! Get float value from config
  /*!
  Get value from configuration file
  \param section section of configuration file
  \param key key in this section
  \sa getInt( )
  \return float value
  */
  float getFloat( std::string section, std::string key);

  //! Get boolen value from config
  /*!
  Get value from configuration file; Supports values in config file ( 0, 1, true, false)
  \param section section of configuration file
  \param key key in this section
  \sa getString( ) getInt( )
  \return boolen value
  */
  bool getBool( std::string section, std::string key);

  //! Get string value from config
  /*!
  Get value from configuration file
  \param section section of configuration file
  \param key key in this section
  \sa getString( ) getInt( )
  \return string value
  */
  std::string getString( std::string section, std::string key);

  //! Set int value to config map
  /*!
  Set value to config map
  \param section section of configuration
  \param key key in this section
  \param value value to be set
  \sa setString( ) setFloat( )
  \return int 0 if ok, negative values with errors
  */
  void setInt( std::string section, std::string key, int value);

  //! Set float value to config map
  /*!
  Set value to config map
  \param section section of configuration
  \param key key in this section
  \param value value to be set
  \sa setString( ) setFloat( )
  \return int 0 if ok, negative values with errors
  */
  void setFloat( std::string section, std::string key, float value);

  //! Set bool value to config map
  /*!
  Set value to config map
  \param section section of configuration
  \param key key in this section
  \param value value to be set
  \sa setString( ) setFloat( )
  \return int 0 if ok, negative values with errors
  */
  void setBool( std::string section, std::string key, bool value);

  //! Set string value to config map
  /*!
  Set value to config map
  \param section section of configuration
  \param key key in this section
  \param value value to be set
  \sa setString( ) setFloat( )
  \return int 0 if ok, negative values with errors
  */
  void setString( std::string section, std::string key, std::string value);

  //! Load config file
  /*!
  Load config into map. Current map may be lost.
  \param filename configuration file
  \return int 0 if ok, negative values with errors
  */
  void load ( std::string filename );

  //! Save config file
  /*!
  Save config from map to file
  \param filename configuration file
  \return int 0 if ok, negative values with errors
  */
  void save( std::string filename );

  //! Copy exists INIConf

  void merge(INIConf *);


  //!Get array
  template<typename T>
  std::vector<T> getArray(std::string section, std::string key, T (*conv)(std::string), char delim=',');

  //!Set array
  template<typename T>
  int setArray(std::string section, std::string key, std::vector<T> array , std::string (*conv)(T) ,char delim=',');

  //!Get int array
  std::vector<int> getIntArray(std::string section, std::string key, char delim=',');

  //!Get string array
  std::vector<std::string> getStringArray(std::string section, std::string key, char delim=',');

  //!Get float array
  std::vector<float> getFloatArray(std::string section, std::string key, char delim=',');
};

#ifdef __INI_SINGLETON__
  #define ini_int(section, key) (INIConf::GetInstance()->getInt(section, key)) 
  #define ini_str(section, key) (INIConf::GetInstance()->getString(section, key))
  #define ini_float(section, key) (INIConf::GetInstance()->getFloat(section, key))
#endif

#endif