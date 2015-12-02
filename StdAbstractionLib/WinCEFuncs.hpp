#ifdef _WIN32_WCE

WINBASEAPI HMODULE WINAPI GetModuleHandleA( LPCSTR lpModuleName );
WINBASEAPI HMODULE WINAPI LoadLibraryExA( LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags );
WINBASEAPI HMODULE WINAPI LoadLibraryA( LPCSTR lpLibFileName );
WINBASEAPI DWORD WINAPI GetModuleFileNameA( HMODULE hModule, LPCH lpFilename, DWORD nSize );
WINBASEAPI VOID WINAPI OutputDebugStringA( LPCSTR lpOutputString );
WINBASEAPI HANDLE WINAPI CreateFileA( LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile );
WINBASEAPI BOOL WINAPI DeleteFileA( LPCSTR lpFileName );
WINBASEAPI BOOL WINAPI RemoveDirectoryA( LPCSTR lpPathName );
WINBASEAPI DWORD WINAPI GetFileAttributesA( LPCSTR lpFileName );
WINBASEAPI BOOL WINAPI SetFileAttributesA( LPCSTR lpFileName, DWORD dwFileAttributes );
WINBASEAPI BOOL WINAPI CreateDirectoryA( LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes );
int WINAPI MessageBoxA( HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType );
WINUSERAPI HWND WINAPI FindWindowA( LPCSTR lpClassName, LPCSTR lpWindowName );
HWND WINAPI CreateWindowExA( DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam );
#ifndef CRITICAL_SECTION_NO_DEBUG_INFO
    #define CRITICAL_SECTION_NO_DEBUG_INFO 0
#endif
WINBASEAPI BOOL WINAPI InitializeCriticalSectionEx( LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount, DWORD Flags );
WINBASEAPI BOOL WINAPI SetFilePointerEx( HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod );
WINBASEAPI BOOL WINAPI GetFileSizeEx( HANDLE hFile, PLARGE_INTEGER lpFileSize );
WINUSERAPI BOOL WINAPI SetWindowTextA( HWND hWnd, LPCSTR lpString );

/*typedef struct _WIN32_FIND_DATAA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    CHAR   cFileName[ MAX_PATH ];
    CHAR   cAlternateFileName[ 14 ];
} WIN32_FIND_DATAA, *PWIN32_FIND_DATAA, *LPWIN32_FIND_DATAA;*/

WINBASEAPI HANDLE WINAPI FindFirstFileA( LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData );
WINBASEAPI BOOL WINAPI FindNextFileA( HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData );

#endif