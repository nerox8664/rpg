#include "Core/Utils.hpp"

std::string readFile(std::string path) {

    std::ifstream infile(path.c_str());
    std::string buf, res;
    while(!infile.eof() && infile.good()) {
        std::getline(infile, buf);
        res+=buf+"\n";
    }
    infile.close();
    return res;
}

bool fileExists(std::string path) {
    std::ifstream infile(path.c_str());
    if(infile.good()) {
        infile.close();
        return true;
    }
    else {
        infile.close();
        return false;
    }
}

// данна€ функци€ используетс€ дл€ вывода информации о ходе компил€ции шейдеров
void PrintProgramLog(GLuint obj) {
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    // получение размера лога
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        // выделение пам€ти под лог
        infoLog = (char *)malloc(infologLength);
        // получение лога
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        // вывод лога на консоль
        printf("%s\n",infoLog);
        // освобождение пам€ти лога, т.к. она больше не нужна
        free(infoLog);
    }
}

std::wstring widen( const std::string& str, std::locale loc ) {
    setlocale(LC_ALL, "Russian");
    std::wstring out;
    const char* in = str.c_str();
    unsigned int codepoint = 0;
    int following = 0;
    for (in;  *in != 0;  ++in)
    {
        unsigned char ch = *in;
        if (ch <= 0x7f)
        {
            codepoint = ch;
            following = 0;
        }
        else if (ch <= 0xbf)
        {
            if (following > 0)
            {
                codepoint = (codepoint << 6) | (ch & 0x3f);
                --following;
            }
        }
        else if (ch <= 0xdf)
        {
            codepoint = ch & 0x1f;
            following = 1;
        }
        else if (ch <= 0xef)
        {
            codepoint = ch & 0x0f;
            following = 2;
        }
        else
        {
            codepoint = ch & 0x07;
            following = 3;
        }
        if (following == 0)
        {
            if (codepoint > 0xffff)
            {
                out.append(1, static_cast<wchar_t>(0xd800 + (codepoint >> 10)));
                out.append(1, static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff)));
            }
            else
                out.append(1, static_cast<wchar_t>(codepoint));
            codepoint = 0;
        }
    }
    return out;
}

std::vector<std::string> GetFileList(std::string path) {
    DIR *dir = nullptr;
    struct dirent *dent;

    std::vector<std::string> list;

    dir = opendir( path.c_str() );   //this part


    if(dir!=NULL)
    {
        while((dent=readdir(dir))!=NULL) {
            printf(dent->d_name);
            std::string Cfile (dent->d_name);
            if(Cfile.size()>3) {
                list.push_back(Cfile);
            }
        }
        closedir(dir);
    }
    return list;
}

/*
// convert UTF-8 string to wstring
std::wstring utf8_to_wstring (const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
std::string wstring_to_utf8 (const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}
*/