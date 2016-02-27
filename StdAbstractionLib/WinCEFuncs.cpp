#include "PreHeader.hpp"

#ifdef _WIN32_WCE

class CToWChar
{
    ui16 *_ptr;
    ui16 _str[ MAX_PATH_LENGTH ];

public:
    ~CToWChar()
    {
        if( _ptr != _str )
        {
            ::free( _ptr );
        }
    }

    NOINLINE CToWChar( LPCSTR str )
    {
        if( !str )
        {
            _ptr = 0;
            return;
        }
        ui32 len = _StrLen( str );
        _ptr = len < sizeof(_str) ? _str : (ui16 *)::malloc( sizeof(ui16) * len + 1 );
        for( ui32 index = 0; index <= len; ++index )
        {
            _ptr[ index ] = str[ index ];
        }
        _ptr[ len ] = 0;
    }

    operator LPWSTR() const
    {
        return (LPWSTR)_ptr;
    }
};

WINBASEAPI HMODULE WINAPI GetModuleHandleA( LPCSTR lpModuleName )
{
    return ::GetModuleHandleW( CToWChar( lpModuleName ) );
}

WINBASEAPI HMODULE WINAPI LoadLibraryExA( LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags )
{
    return ::LoadLibraryExW( CToWChar( lpLibFileName ), hFile, dwFlags );
}

WINBASEAPI HMODULE WINAPI LoadLibraryA( LPCSTR lpLibFileName )
{
    return ::LoadLibraryW( CToWChar( lpLibFileName ) );
}

WINBASEAPI DWORD WINAPI GetModuleFileNameA( HMODULE hModule, LPCH lpFilename, DWORD nSize )
{
    wchar_t temp[ MAX_PATH_LENGTH ];
    DWORD len = ::GetModuleFileNameW( hModule, temp, Funcs::Min < DWORD >( MAX_PATH_LENGTH, nSize ) );
    for( uiw index = 0; index < len; ++index )
    {
        *lpFilename++ = temp[ index ];
    }
    *lpFilename = '\0';
    return len;
}

WINBASEAPI VOID WINAPI OutputDebugStringA( LPCSTR lpOutputString )
{
    return ::OutputDebugStringW( CToWChar( lpOutputString ) );
}

WINBASEAPI HANDLE WINAPI CreateFileA( LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile )
{
    return ::CreateFileW( CToWChar( lpFileName ), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );
}

WINBASEAPI BOOL WINAPI DeleteFileA( LPCSTR lpFileName )
{
    return ::DeleteFileW( CToWChar( lpFileName ) );
}

WINBASEAPI BOOL WINAPI RemoveDirectoryA( LPCSTR lpPathName )
{
    return ::RemoveDirectoryW( CToWChar( lpPathName ) );
}

WINBASEAPI DWORD WINAPI GetFileAttributesA( LPCSTR lpFileName )
{
    return ::GetFileAttributesW( CToWChar( lpFileName ) );
}

WINBASEAPI BOOL WINAPI SetFileAttributesA( LPCSTR lpFileName, DWORD dwFileAttributes )
{
    return ::SetFileAttributesW( CToWChar( lpFileName ), dwFileAttributes );
}

WINBASEAPI BOOL WINAPI CreateDirectoryA( LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes )
{
    return ::CreateDirectoryW( CToWChar( lpPathName ), lpSecurityAttributes );
}

int WINAPI MessageBoxA( HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType )
{
    return ::MessageBoxW( hWnd, CToWChar( lpText ), CToWChar( lpCaption ), uType );
}

WINUSERAPI HWND WINAPI FindWindowA( LPCSTR lpClassName, LPCSTR lpWindowName )
{
    return ::FindWindowW( CToWChar( lpClassName ), CToWChar( lpWindowName ) );
}

HWND WINAPI CreateWindowExA( DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam )
{
    return ::CreateWindowExW( dwExStyle, CToWChar( lpClassName ), CToWChar( lpWindowName ), dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam );
}

WINBASEAPI BOOL WINAPI InitializeCriticalSectionEx( LPCRITICAL_SECTION lpCriticalSection, DWORD dwSpinCount, DWORD Flags )
{
    InitializeCriticalSection( lpCriticalSection );
    return TRUE;
}

WINBASEAPI BOOL WINAPI SetFilePointerEx( HANDLE hFile, LARGE_INTEGER liDistanceToMove, PLARGE_INTEGER lpNewFilePointer, DWORD dwMoveMethod )
{
    liDistanceToMove.LowPart = ::SetFilePointer( hFile, liDistanceToMove.LowPart, &liDistanceToMove.HighPart, dwMoveMethod );
    if( liDistanceToMove.LowPart == INVALID_SET_FILE_POINTER && ::GetLastError() != NO_ERROR )
    {
        return FALSE;
    }
    if( lpNewFilePointer )
    {
        *lpNewFilePointer = liDistanceToMove;
    }
    return TRUE;
}

WINBASEAPI BOOL WINAPI GetFileSizeEx( HANDLE hFile, PLARGE_INTEGER lpFileSize )
{
    LARGE_INTEGER result;
    result.LowPart = ::GetFileSize( hFile, (DWORD *)&result.HighPart );
    if( result.LowPart == INVALID_FILE_SIZE && ::GetLastError() != NO_ERROR )
    {
        return FALSE;
    }
    if( lpFileSize )
    {
        *lpFileSize = result;
    }
    return TRUE;
}

WINUSERAPI BOOL WINAPI SetWindowTextA( HWND hWnd, LPCSTR lpString )
{
    return ::SetWindowTextW( hWnd, CToWChar( lpString ) );
}

static void DataAtoW( LPWIN32_FIND_DATAA output, LPWIN32_FIND_DATAW source )
{
    output->dwFileAttributes = source->dwFileAttributes;
    output->ftCreationTime = source->ftCreationTime;
    output->ftLastAccessTime = source->ftLastAccessTime;
    output->ftLastWriteTime = source->ftLastWriteTime;
    output->nFileSizeHigh = source->nFileSizeHigh;
    output->nFileSizeLow = source->nFileSizeLow;
    output->dwReserved0 = 0;
    output->dwReserved1 = 0;
    for( uiw index = 0; index < MAX_PATH_LENGTH; ++index )
    {
        output->cFileName[ index ] = source->cFileName[ index ];
    }
    for( uiw index = 0; index < 14; ++index )
    {
        output->cAlternateFileName[ index ] = 0;
    }
}

WINBASEAPI HANDLE WINAPI FindFirstFileA( LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData )
{
    ASSUME( lpFileName && lpFindFileData );
    WIN32_FIND_DATAW data;
    HANDLE file = ::FindFirstFileW( CToWChar( lpFileName ), &data );
    if( file == INVALID_HANDLE_VALUE )
    {
        return file;
    }
    DataAtoW( lpFindFileData, &data );
    return file;
}

WINBASEAPI BOOL WINAPI FindNextFileA( HANDLE hFindFile, LPWIN32_FIND_DATAA lpFindFileData )
{
    ASSUME( lpFindFileData );
    WIN32_FIND_DATAW data;
    BOOL result = ::FindNextFileW( hFindFile, &data );
    DataAtoW( lpFindFileData, &data );
    return result;
}

#endif