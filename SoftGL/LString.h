#ifndef LSTRING_H__
#define LSTRING_H__
#include <string>

#ifdef _UNICODE
#define _text(str) L##str
typedef std::wstring String;
#define PRINT wprintf
#define STRCMP wcscmp
typedef wchar_t tchar;
#define string_size(str) (str.length() * 2)
#else
#define _text(str) str
#define PRINT printf
typedef std::string String;
typedef char tchar;
#define STRCMP strcmp
#define string_size(str) (str.length())
#endif

#ifdef WIN32
#define PATH_SPLITTER _text("\\")
#else
#define PATH_SPLITTER _text("/")
#endif

extern String GetFileName(const String& path);

extern String GetFileName(const String& path, const String& pathSplitter);

extern String GetFileNameWithoutExtention(const String& path);

extern String GetFilePath(const String& path, const String& pathSplitter);

extern void GetRootFolderS(const String& path, String& folderName, String& resultPath);

extern void GetRootFolderS(const String& path, String& folderName);

extern String GetDirectoryName(const String& path);

#define AS_STRING(omg) String(omg)
#define STR(omg) String(omg)

#endif
