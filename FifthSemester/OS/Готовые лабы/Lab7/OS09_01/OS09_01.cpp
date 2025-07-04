#include <Windows.h>
#include <iostream>
using namespace std;

#define FILE_PATH L"C:/Users/super/Desktop/Lab7/OS09_01.txt"
#define READ_BYTES 500

string getFileName(wchar_t* filePath)
{
    wstring ws(filePath);
    string filename(ws.begin(), ws.end());
    const size_t last_slash_idx = filename.find_last_of("\\/");
    if (string::npos != last_slash_idx)
        filename.erase(0, last_slash_idx + 1);
    return filename;
}

LPCWSTR getFileType(HANDLE file)
{
    switch (GetFileType(file)) {
    case FILE_TYPE_UNKNOWN: return L"FILE_TYPE_UNKNOWN";
    case FILE_TYPE_DISK: return L"FILE_TYPE_DISK";
    case FILE_TYPE_CHAR: return L"FILE_TYPE_CHAR";
    case FILE_TYPE_PIPE: return L"FILE_TYPE_PIPE";
    default: return L"ERROR: UNKNOWN FILE TYPE";
    }
}

BOOL printFileInfo(LPWSTR path)
{
    try {
        HANDLE file = CreateFile(
            path,
            GENERIC_READ,
            NULL,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        SYSTEMTIME sysTime;
        BY_HANDLE_FILE_INFORMATION fi;
        BOOL fResult = GetFileInformationByHandle(file, &fi);
        if (fResult)
        {
            cout << "File name:\t" << getFileName((wchar_t*)FILE_PATH);
            wcout << "\nFile type:\t" << getFileType(file);
            cout << "\nFile size:\t" << fi.nFileSizeLow << " bytes";
            FileTimeToSystemTime(&fi.ftCreationTime, &sysTime);
            cout << "\nCreate time:\t" << sysTime.wDay << '.' << sysTime.wMonth << '.' << sysTime.wYear << " " << sysTime.wHour + 3 << '.' << sysTime.wMinute << '.' << sysTime.wSecond;
            FileTimeToSystemTime(&fi.ftLastWriteTime, &sysTime);
            cout << "\nUpdate time:\t" << sysTime.wDay << '.' << sysTime.wMonth << '.' << sysTime.wYear << " " << sysTime.wHour + 3 << '.' << sysTime.wMinute << '.' << sysTime.wSecond << '\n';
        }
        CloseHandle(file);
        return true;
    }
    catch (const char* ex)
    {
        cout << ex << " \n";
        return false;
    }
}

BOOL printFileTxt(LPWSTR fileName)
{
    try {
        HANDLE file = CreateFile(
            fileName,
            GENERIC_READ,
            NULL,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (file == INVALID_HANDLE_VALUE) 
            throw "ERROR: Create or open file failed";

        DWORD n = NULL;
        char buf[1024];
        ZeroMemory(buf, sizeof(buf));
        BOOL b = ReadFile(
            file,
            &buf,
            READ_BYTES,
            &n,
            NULL);
        if (!b) throw "ERROR: Read file failed";
        cout << "Print file " << n << " byte succesfull:\n" << buf << '\n';
        CloseHandle(file);
        return true;
    }
    catch (const char* ex)
    {
        cout << ex << " \n";
        return false;
    }
}

int main()
{
    LPWSTR path = (LPWSTR)FILE_PATH;
    printFileInfo(path);
    cout << "------------------------------------------\n";
    printFileTxt(path);
}