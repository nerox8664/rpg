#ifndef __UTILS_H__
#define __UTILS_H__

#include <Core/Types.hpp>

#include <vector>
#include <locale>
#include <string>
#include <fstream>
#include <dirent.h>

std::string readFile(std::string path);

bool fileExists(std::string path);

void PrintProgramLog(GLuint obj);

std::wstring widen( const std::string& in, std::locale loc  = std::locale() );

std::vector<std::string> GetFileList(std::string path);

/*
#include <string>

// convert UTF-8 string to wstring
std::wstring utf8_to_wstring (const std::string& str);

// convert wstring to UTF-8 string
std::string wstring_to_utf8 (const std::wstring& str);
*/
#endif