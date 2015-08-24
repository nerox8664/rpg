#ifndef __UTILS__
#define __UTILS__

#include <Core/Types.hpp>

std::string readFile(std::string path);
bool fileExists(std::string path);
void PrintProgramLog(GLuint obj);
std::wstring widen( const std::string& in, std::locale loc  = std::locale() );
std::vector<std::string> GetFileList(std::string path);


#endif