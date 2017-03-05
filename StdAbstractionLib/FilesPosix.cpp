#include "PreHeader.hpp"

#ifdef POSIX

#include "Files.hpp"
#include "Funcs.hpp"

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>

bool Files::MoveFileTo( const FilePath &sourcePnn, const FilePath &targetPnn, bool is_replace, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

bool Files::MoveFolderTo( const FilePath &sourcePnn, const FilePath &targetPnn, bool is_replace, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

NOINLINE bool Files::MoveObjectTo( const FilePath &sourcePnn, const FilePath &targetPnn, bool is_replace, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

bool Files::CopyFileTo( const FilePath &sourcePnn, const FilePath &targetPnn, bool is_replace, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

bool Files::CopyFolderTo( const FilePath &sourcePnn, const FilePath &targetPnn, bool is_replace, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

bool Files::CopyObjectTo( const FilePath &sourcePnn, const FilePath &targetPnn, bool is_replace, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

NOINLINE bool Files::RemoveFile( const FilePath &pnn, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

NOINLINE bool Files::RemoveFolder( const FilePath &path, CError *error )  //  potentially recursive
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

bool Files::RemoveObject( const FilePath &path, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

bool Files::VolumeDriveName( const FilePath &path, char *RSTR output, uiw maxLen )
{
	NOT_IMPLEMENTED;
	return false;
}

NOINLINE bool Files::IsPointToTheSameFile( const FilePath &pnn0, const FilePath &pnn1, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

NOINLINE bool Files::IsExists( const FilePath &pnn, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

bool Files::IsFile( const FilePath &pnn, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

bool Files::IsFolder( const FilePath &pnn, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

bool Files::IsEmpty( const FilePath &pnn, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

NOINLINE bool Files::IsFileReadOnlyGet( const FilePath &pnn, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

NOINLINE bool Files::IsFileReadOnlySet( const FilePath &pnn, bool is_ro, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

NOINLINE bool Files::CreateNewFolder( const FilePath &where, const FilePath &name, bool is_overrideExistingObject, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

NOINLINE bool Files::CreateNewFile( const FilePath &where, const FilePath &name, bool is_overrideExistingObject, CError *error )
{
	NOT_IMPLEMENTED;
	DSA( error, Error::Unimplemented() );
	return false;
}

bool Files::IsRelativePathSupported()
{
	return true;
}

FilePath Files::CurrentWorkingPathGet()
{
	char buf[ PATH_MAX ];
	char *result = ::getcwd( buf, sizeof(buf) - 1 );
	if( result == NULL )
	{
		FilePath();
	}
	return FilePath( result );
}

bool Files::CurrentWorkingPathSet( const FilePath &path )
{
	if( !path.IsValid() )
	{
		return false;
	}
	return chdir( path.PlatformPath() ) == 0;
}

struct CFileEnumerator : public Files::CFileEnumInfo
{
	NOINLINE bool EnumFirstFile( const FilePath &path, const FilePath &mask )
	{
		NOT_IMPLEMENTED;
		return false;
	}

	NOINLINE bool EnumNextFile()
	{
		NOT_IMPLEMENTED;
		return false;
	}
};

bool Files::EnumFirstFile( CFileEnumInfo *info, const FilePath &path, const FilePath &mask )
{
	return ((CFileEnumerator *)info)->EnumFirstFile( path, mask );
}

bool Files::EnumNextFile( CFileEnumInfo *info )
{
	return ((CFileEnumerator *)info)->EnumNextFile();
}

void Files::Private::CloseEnumHandle( fileEnumHandle handle )
{
	NOT_IMPLEMENTED;
}

#if 0
NOINLINE bool Files::RemoveFile( const char *cp_pnn, CError *error )
{
    ASSUME( cp_pnn && _StrLen( cp_pnn ) < MAX_PATH_LENGTH );

    bool funcResult = ::unlink( cp_pnn ) == 0;
    if( error )
    {
        if( !funcResult )
        {
            if( errno == EACCES )
            {
                *error = Error::NoAccess();
            }
            else if( errno == ENOENT )
            {
                *error = Error::DoesNotExist();
            }
            else
            {
                *error = Error::Unknown();
            }
        }
        else
        {
            *error = Error::Ok();
        }
    }

    return funcResult;
}

NOINLINE bool Files::RemoveFolder( const char *cp_path, CError *error )
{
    ASSUME( cp_path && _StrLen( cp_path ) < MAX_PATH_LENGTH );
    DIR *p_dir;
    struct dirent *po_dir;
    CError o_error = Error::Ok();
    bool funcResult = false;
    char a_buf[ MAX_PATH_LENGTH ];
    uiw cpyIndex = Funcs::StrCpyAndCountWONull( a_buf, cp_path );
    a_buf[ cpyIndex++ ] = '/';

    if( !IsFolderExists( cp_path ) )
    {
        o_error = Error::DoesNotExist();
        goto toExit;
    }

    p_dir = ::opendir( cp_path );
    if( p_dir )
    {
        for( ; ; )
        {
            po_dir = ::readdir( p_dir );
            if( !po_dir )
            {
                break;
            }

            if( _StrEqual( po_dir->d_name, "." ) || _StrEqual( po_dir->d_name, ".." ) )
            {
                continue;
            }

            _StrCpy( a_buf + cpyIndex, po_dir->d_name );

            struct stat o_stat;
            if( ::stat( a_buf, &o_stat ) != 0 )
            {
                o_error = Error::Unknown();
                ::closedir( p_dir );
                goto toExit;
            }

            if( S_ISDIR( o_stat.st_mode ) )
            {
                if( !Files::RemoveFolder( a_buf, error ) )
                {
                    ::closedir( p_dir );
                    goto toExit;
                }
            }
            else
            {
                ASSUME( S_ISREG( o_stat.st_mode ) );
                if( !Files::RemoveFile( a_buf, error ) )
                {
                    ::closedir( p_dir );
                    goto toExit;
                }
            }
        }

        ::closedir( p_dir );
    }

    funcResult = ::rmdir( cp_path ) == 0;
    if( !funcResult )
    {
        o_error = Error::Unknown();
    }

toExit:
    DSA( error, o_error );
    return funcResult;
}

bool Files::IsFileOrFolderExists( const char *cp_papn )
{
    ASSUME( cp_papn && _StrLen( cp_papn ) < MAX_PATH_LENGTH );
    struct stat o_stat;
    return ::stat( cp_papn, &o_stat ) == 0 && (S_ISDIR( o_stat.st_mode ) || S_ISREG( o_stat.st_mode ));
}

bool Files::IsFileExists( const char *cp_pnn )
{
    ASSUME( cp_pnn && _StrLen( cp_pnn ) < MAX_PATH_LENGTH );
    return ::access( cp_pnn, F_OK ) == 0;
}

bool Files::IsFolderExists( const char *cp_path )
{
    ASSUME( cp_path && _StrLen( cp_path ) < MAX_PATH_LENGTH );
    struct stat o_stat;
    return ::stat( cp_path, &o_stat ) == 0 && S_ISDIR( o_stat.st_mode );
}

bool Files::IsFileReadOnlyGet( const char *cp_pnn )
{
    ASSUME( cp_pnn && _StrLen( cp_pnn ) < MAX_PATH_LENGTH );
    DBGBREAK;  //  TODO: complete
    return false;
}

bool Files::IsFileReadOnlySet( const char *cp_pnn, bool is_ro )
{
    ASSUME( cp_pnn && _StrLen( cp_pnn ) < MAX_PATH_LENGTH );
    DBGBREAK;  //  TODO: complete
    return false;
}

bool Files::IsRelativePathSupported()
{
	return true;
}

bool Files::IsAbsolutePath( const char *pnn, uiw parseLen )
{
    DBGBREAK;  //  TODO: complete
    return true;
}

uiw Files::AbsolutePath( const char *RSTR cp_sourcePath, char *absPath, uiw maxLen )
{
	DBGBREAK;  //  TODO: complete
	return 0;
}

bool Files::CurrentWorkingPathGet( char *buf, uiw maxLen, uiw *copied )
{
	char *result = ::getcwd( buf, maxLen );
	if( result == NULL )
	{
		if( errno == ERANGE )
		{
			DSA( copied, uiw_max );
		}
		else
		{
			DSA( copied, 0 );
		}
		return false;
	}
	DSA( copied, _StrLen( buf ) );
	return true;
}

bool Files::CurrentWorkingPathSet( const char *path )
{
	ASSUME( path );
	return chdir( path ) == 0;
}

NOINLINE bool Files::CreateNewFolder( const char *cp_where, const char *cp_name, CError *error )
{
    ASSUME( cp_where && cp_name && (_StrLen( cp_where ) + _StrLen( cp_name ) < MAX_PATH_LENGTH) );

    char a_buf[ MAX_PATH_LENGTH ];
    uiw len = Funcs::StrCpyAndCountWONull( a_buf, cp_where );
    _StrCpy( a_buf + len, cp_name );
    bool funcResult = false;
    CError o_error = Error::Ok();
    mode_t process_mask;

    if( IsFolderExists( a_buf ) )
    {
        o_error = Error::AlreadyExists();
        goto toExit;
    }

    process_mask = ::umask( 0 );
    funcResult = ::mkdir( a_buf, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH ) == 0;
    ::umask( process_mask );
    if( !funcResult )
    {
        o_error = Error::Unknown();
    }

toExit:
    DSA( error, o_error );
    return funcResult;
}

NOINLINE uiw Files::ExtractPathFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen/* = uiw_max */ )
{
    ASSUME( cp_str && p_buf );
    uiw lastSlash = -1;
    for( uiw index = 0; cp_str[ index ] && index < parseLen; ++index )
    {
        if( cp_str[ index ] == '/' )
        {
            lastSlash = index;
        }
    }
    ++lastSlash;
    _MemCpy( p_buf, cp_str, lastSlash );
    p_buf[ lastSlash ] = '\0';
    return lastSlash;
}

NOINLINE uiw Files::ExtractNameFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen/* = uiw_max */ )
{
    ASSUME( cp_str && p_buf );
    uiw lastSlash = 0;
    for( uiw index = 0; cp_str[ index ] && index < parseLen; ++index )
    {
        if( cp_str[ index ] == '/' )
        {
            lastSlash = index;
        }
    }
    return Funcs::StrCpyAndCount( p_buf, cp_str + lastSlash + 1 );
}

NOINLINE uiw Files::ExtractNameWOExtFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen/* = uiw_max */ )
{
    ASSUME( cp_str && p_buf );
    uiw lastSlash = 0;
    for( uiw index = 0; cp_str[ index ] && index < parseLen; ++index )
    {
        if( cp_str[ index ] == '/' )
        {
            lastSlash = index;
        }
    }
    cp_str += lastSlash + 1;
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

bool Files::EnumFirstFile( CFileEnumInfo *info, const char *path, const char *mask )  //  friend of CFileEnumInfo
{
	DBGBREAK;  //  TODO:
    return false;
}

bool Files::EnumNextFile( CFileEnumInfo *info )  //  friend of CFileEnumInfo
{
	DBGBREAK;  //  TODO:
    return false;
}

void Files::EnumFilesRecursively( const char *path, const char *mask, bool is_reportFolders, EnumFilesCallback callback, void *argument )
{
	DBGBREAK;  //  TODO:
}
#endif

#endif
