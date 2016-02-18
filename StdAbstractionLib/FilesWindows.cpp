#include "PreHeader.hpp"

#ifdef WINDOWS

#include "Files.hpp"
#include "Funcs.hpp"

#if defined(_MSC_VER) && !defined(DEBUG)
    #pragma optimize( "s", on )  //  TODO: global solution
#endif

NOINLINE bln Files::RemoveFile( const char *cp_pnn, CError *po_error )
{
    ASSUME( cp_pnn && _StrLen( cp_pnn ) < MAX_PATH );

    bln funcResult = ::DeleteFileA( cp_pnn ) != 0;
    if( po_error )
    {
        if( !funcResult )
        {
            if( ::GetLastError() == ERROR_ACCESS_DENIED )
            {
                *po_error = Error::NoAccess();
            }
            else if( ::GetLastError() == ERROR_FILE_NOT_FOUND )
            {
                *po_error = Error::DoesNotExist();
            }
            else
            {
                *po_error = Error::Unknown();
            }
        }
        else
        {
            *po_error = Error::Ok();
        }
    }

    return funcResult;
}

NOINLINE bln Files::RemoveFolder( const char *cp_path, CError *po_error )
{
    ASSUME( cp_path && _StrLen( cp_path ) + 2 < MAX_PATH );

    char a_buf[ MAX_PATH ];
    uiw cpy = Funcs::StrCpyAndCountWONull( a_buf, cp_path );
    _StrCpy( a_buf + cpy, "/*" );
    ++cpy;
    WIN32_FIND_DATAA o_find;
    HANDLE h_find;
	CError o_error = Error::Ok();
    bln funcResult = false;

    if( !IsFolderExists( cp_path ) )
    {
        o_error = Error::DoesNotExist();
        goto toExit;
    }

    h_find = ::FindFirstFileA( a_buf, &o_find );
    if( h_find == INVALID_HANDLE_VALUE )
    {
        goto toExit;
    }
    do
    {
        if( _StrEqual( o_find.cFileName, "." ) || _StrEqual( o_find.cFileName, ".." ) )
        {
            continue;
        }

        _StrCpy( a_buf + cpy, o_find.cFileName );
        if( o_find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            if( !RemoveFolder( a_buf, po_error ) )
            {
                ::FindClose( h_find );
                goto toExit;
            }
        }
        else if( !RemoveFile( a_buf, po_error ) )
        {
            ::FindClose( h_find );
            goto toExit;
        }
    } while( ::FindNextFileA( h_find, &o_find ) );

    ::FindClose( h_find );

    funcResult = ::RemoveDirectoryA( cp_path ) != 0;
    if( !funcResult )
    {
        o_error = Error::Unknown();
    }

toExit:
    DSA( po_error, o_error );
    return funcResult;
}

bln Files::IsFileOrFolderExists( const char *cp_papn )
{
    ASSUME( cp_papn && _StrLen( cp_papn ) < MAX_PATH );
    DWORD attribs = ::GetFileAttributesA( cp_papn );
    return attribs != INVALID_FILE_ATTRIBUTES;
}

NOINLINE bln Files::IsFileExists( const char *cp_pnn )
{
    ASSUME( cp_pnn && _StrLen( cp_pnn ) < MAX_PATH );
    DWORD attribs = ::GetFileAttributesA( cp_pnn );
    if( attribs == INVALID_FILE_ATTRIBUTES )
    {
        return false;
    }
    if( attribs & FILE_ATTRIBUTE_DIRECTORY )
    {
        return false;
    }
    return true;
}

NOINLINE bln Files::IsFolderExists( const char *cp_path )
{
    ASSUME( cp_path && _StrLen( cp_path ) < MAX_PATH );
    DWORD attribs = ::GetFileAttributesA( cp_path );
    if( attribs == INVALID_FILE_ATTRIBUTES )
    {
        return false;
    }
    if( attribs & FILE_ATTRIBUTE_DIRECTORY )
    {
        return true;
    }
    return false;
}

NOINLINE bln Files::IsFileReadOnlyGet( const char *cp_pnn )
{
    ASSUME( cp_pnn && _StrLen( cp_pnn ) < MAX_PATH );
    DWORD attribs = ::GetFileAttributesA( cp_pnn );
    if( attribs == INVALID_FILE_ATTRIBUTES )
    {
        return false;
    }
    return attribs & FILE_ATTRIBUTE_READONLY;
}

NOINLINE bln Files::IsFileReadOnlySet( const char *cp_pnn, bln is_ro )
{
    ASSUME( cp_pnn && _StrLen( cp_pnn ) < MAX_PATH );
    DWORD new_attribs;
    DWORD old_attribs = ::GetFileAttributesA( cp_pnn );
    if( old_attribs == INVALID_FILE_ATTRIBUTES )
    {
        return false;
    }
    if( is_ro )
    {
        new_attribs = old_attribs | FILE_ATTRIBUTE_READONLY;
    }
    else
    {
        new_attribs = old_attribs & ~FILE_ATTRIBUTE_READONLY;
    }
    if( old_attribs != new_attribs )
    {
        return ::SetFileAttributesA( cp_pnn, new_attribs ) != 0;
    }
    return true;
}

bln Files::IsAbsolutePath( const char *pnn, uiw parseLen /* = uiw_max */ )
{
    return Funcs::IsChrAlpha( pnn[ 0 ] ) && pnn[ 1 ] == ':' && (pnn[ 2 ] == '/' || pnn[ 2 ] == '\\');
}

NOINLINE bln Files::CreateFolder( const char *cp_where, const char *cp_name, CError *po_error )
{
    ASSUME( cp_where && cp_name && (_StrLen( cp_where ) + _StrLen( cp_name ) < MAX_PATH) );

    char a_buf[ MAX_PATH ];
    uiw len = Funcs::StrCpyAndCountWONull( a_buf, cp_where );
    _StrCpy( a_buf + len, cp_name );
    bln funcResult = false;
    CError o_error = Error::Ok();

    if( IsFolderExists( a_buf ) )
    {
        o_error = Error::AlreadyExists();
        goto toExit;
    }

    funcResult = ::CreateDirectoryA( a_buf, 0 ) != 0;
    if( !funcResult )
    {
        o_error = Error::Unknown();
    }

toExit:
    DSA( po_error, o_error );
    return funcResult;
}

NOINLINE uiw Files::ExtractPathFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen/* = uiw_max */ )
{
    ASSUME( cp_str && p_buf );
    uiw lastSlash = -1;
    for( uiw index = 0; cp_str[ index ] && index < parseLen; ++index )
    {
        if( cp_str[ index ] == '\\' || cp_str[ index ] == '/' )
        {
            lastSlash = index;
        }
    }
    ++lastSlash;
    _MemCpy( p_buf, cp_str, lastSlash );
    p_buf[ lastSlash ] = '\0';
    return lastSlash;
}

NOINLINE uiw Files::ExtractNameFromString( const char *cp_str, char *p_buf, uiw parseLen/* = uiw_max */ )
{
    ASSUME( cp_str && p_buf );
    uiw lastSlash = 0;
    for( uiw index = 0; cp_str[ index ] && index < parseLen; ++index )
    {
        if( cp_str[ index ] == '\\' || cp_str[ index ] == '/' )
        {
            lastSlash = index + 1;
        }
    }
    return Funcs::StrCpyAndCount( p_buf, cp_str + lastSlash );
}

NOINLINE uiw Files::ExtractNameWOExtFromString( const char *cp_str, char *p_buf, uiw parseLen/* = uiw_max */ )
{
    ASSUME( cp_str && p_buf );
    uiw lastSlash = 0;
    for( uiw index = 0; cp_str[ index ] && index < parseLen; ++index )
    {
        if( cp_str[ index ] == '\\' || cp_str[ index ] == '/' )
        {
            lastSlash = index + 1;
        }
    }
    cp_str += lastSlash;
    uiw len = 0;
    for( ; cp_str[ len ] && cp_str[ len ] != '.'; ++len )
    {
        p_buf[ len ] = cp_str[ len ];
    }
    p_buf[ len ] = '\0';
    return len;
}

NOINLINE uiw Files::ExtractExtensionFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen/* = uiw_max */ )
{
    cp_str += *cp_str == '.';
    uiw lastDot = ui32_max;
    for( uiw index = 0; cp_str[ index ] && index < parseLen; ++index )
    {
        if( cp_str[ index ] == '.' )
        {
            lastDot = index;
        }
    }
    if( lastDot == ui32_max )
    {
        *p_buf = '\0';
        return 0;
    }
    return Funcs::StrCpyAndCount( p_buf, cp_str + lastDot + 1 );
}

NOINLINE uiw Files::AbsolutePath( const char *RSTR cp_sourcePath, char a_procedPath[ MAX_PATH ] )
{
    #ifdef _WIN32_WCE
        return Funcs::StrCpyAndCount( a_procedPath, cp_sourcePath );
    #else
        return ::GetFullPathNameA( cp_sourcePath, MAX_PATH, a_procedPath, 0 );
    #endif
}

struct CFileEnumerator : public Files::CFileEnumInfo
{
    NOINLINE bln EnumFirstFile( const char *path, const char *mask )
    {
        ASSUME( path && mask );

        if( _handle != INVALID_HANDLE_VALUE )
        {
            ::FindClose( _handle );
        }

        for( _pathLen = 0; path[ _pathLen ]; ++_pathLen )
        {
            _pnn[ _pathLen ] = path[ _pathLen ];
            if( _pnn[ _pathLen ] == '/' )
            {
                _pnn[ _pathLen ] = '\\';
            }
        }

        if( _pathLen == 0 || _pnn[ _pathLen - 1 ] != '\\' )
        {
            _pnn[ _pathLen++ ] = '\\';
        }

        _StrCpy( _pnn + _pathLen, mask );

        WIN32_FIND_DATAA findData;

        _handle = ::FindFirstFileA( _pnn, &findData );

        if( _handle == INVALID_HANDLE_VALUE )
        {
            *this = CFileEnumerator();
            return false;
        }

        if( _StrEqual( findData.cFileName, "." ) )
        {
            if( !::FindNextFileA( _handle, &findData ) )
            {
                *this = CFileEnumerator();
                return false;
            }
            if( _StrEqual( findData.cFileName, ".." ) )
            {
                if( !::FindNextFileA( _handle, &findData ) )
                {
                    *this = CFileEnumerator();
                    return false;
                }
            }
        }

        _StrCpy( _pnn + _pathLen, findData.cFileName );
        if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            _StrCat( _pnn + _pathLen, "\\" );
            _fileSize = ui64_max;
        }
        else
        {
            LARGE_INTEGER large_size;
            large_size.LowPart = findData.nFileSizeLow;
            large_size.HighPart = findData.nFileSizeHigh;
            _fileSize = large_size.QuadPart;
        }

        return true;
    }

    NOINLINE bln EnumNextFile()
    {
        ASSUME( _handle != INVALID_HANDLE_VALUE );

        WIN32_FIND_DATAA findData;

        if( !::FindNextFileA( _handle, &findData ) )
        {
            *this = CFileEnumerator();
            return false;
        }

        _StrCpy( _pnn + _pathLen, findData.cFileName );
        if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            _StrCat( _pnn + _pathLen, "\\" );
            _fileSize = ui64_max;
        }
        else
        {
            LARGE_INTEGER large_size;
            large_size.LowPart = findData.nFileSizeLow;
            large_size.HighPart = findData.nFileSizeHigh;
            _fileSize = large_size.QuadPart;
        }

        return true;
    }
};

bln Files::EnumFirstFile( CFileEnumInfo *info, const char *path, const char *mask )
{
    return ((CFileEnumerator *)info)->EnumFirstFile( path, mask );
}

bln Files::EnumNextFile( CFileEnumInfo *info )
{
    return ((CFileEnumerator *)info)->EnumNextFile();
}

void Files::EnumFilesRecursively( const char *path, const char *mask, EnumFilesCallback callback, void *argument )
{
    UniquePtr < CFileEnumInfo > info( new CFileEnumInfo );
    if( !EnumFirstFile( info, path, "*.*" ) )
    {
        return;
    }

    do
    {
        if( info->IsFolder() )
        {
            EnumFilesRecursively( info->PNN(), mask, callback, argument );
        }
    } while( EnumNextFile( info ) );

    if( !EnumFirstFile( info, path, mask ) )
    {
        return;
    }

    do
    {
        if( info->IsFolder() == false )
        {
            callback( info, argument );
        }
    } while( EnumNextFile( info ) );
}

void Files::Private::CloseEnumHandle( fileEnumHandle handle )
{
    ::FindClose( handle );
}

#endif WINDOWS
