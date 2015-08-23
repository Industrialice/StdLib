#include "PreHeader.hpp"

#ifdef POSIX

#include "Files.hpp"
#include "Funcs.hpp"

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>

NOINLINE bln Files::DeleteFile( const char *cp_pnn, SError *po_error )
{
	ASSUME( cp_pnn && _StrLen( cp_pnn ) < MAX_PATH );

	bln funcResult = ::unlink( cp_pnn ) == 0;
    if( po_error )
    {
        if( !funcResult )
        {
            if( errno == EACCES )
            {
                *po_error = Error::Get( Error::NoAccess );
            }
            else if( errno == ENOENT )
            {
                *po_error = Error::Get( Error::DoesNotExists );
            }
            else
            {
                *po_error = Error::Get( Error::Unknown );
            }
        }
        else
        {
            *po_error = Error::Get( Error::Ok );
        }
    }

    return funcResult;
}

NOINLINE bln Files::DeleteFolder( const char *cp_path, SError *po_error )
{
	ASSUME( cp_path && _StrLen( cp_path ) < MAX_PATH );
	DIR *p_dir;
	struct dirent *po_dir;
	SError o_error = Error::Get( Error::Ok );
	bln funcResult = false;
	char a_buf[ MAX_PATH ];
	uiw cpyIndex = Funcs::StrCpyAndCountWONull( a_buf, cp_path );
	a_buf[ cpyIndex++ ] = '/';

    if( !IsFolderExists( cp_path ) )
    {
        o_error = Error::Get( Error::DoesNotExists );
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
				o_error = Error::Get( Error::Unknown );
				::closedir( p_dir );
				goto toExit;
			}

			if( S_ISDIR( o_stat.st_mode ) )
			{
				if( !Files::DeleteFolder( a_buf, po_error ) )
				{
					::closedir( p_dir );
					goto toExit;
				}
			}
			else
			{
				ASSUME( S_ISREG( o_stat.st_mode ) );
				if( !Files::DeleteFile( a_buf, po_error ) )
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
		o_error = Error::Get( Error::Unknown );
	}

toExit:
    DSA( po_error, o_error );
	return funcResult;
}

bln Files::IsFileOrFolderExists( const char *cp_papn )
{
    ASSUME( cp_papn && _StrLen( cp_papn ) < MAX_PATH );
	struct stat o_stat;
	return ::stat( cp_papn, &o_stat ) == 0 && (S_ISDIR( o_stat.st_mode ) || S_ISREG( o_stat.st_mode ));
}

bln Files::IsFileExists( const char *cp_pnn )
{
	ASSUME( cp_pnn && _StrLen( cp_pnn ) < MAX_PATH );
	return ::access( cp_pnn, F_OK ) == 0;
}

bln Files::IsFolderExists( const char *cp_path )
{
	ASSUME( cp_path && _StrLen( cp_path ) < MAX_PATH );
	struct stat o_stat;
	return ::stat( cp_path, &o_stat ) == 0 && S_ISDIR( o_stat.st_mode );
}

bln Files::IsFileReadOnlyGet( const char *cp_pnn )
{
    ASSUME( cp_pnn && _StrLen( cp_pnn ) < MAX_PATH );
	DBGBREAK;  //  TODO: complete
    return false;
}

bln Files::IsFileReadOnlySet( const char *cp_pnn, bln is_ro )
{
    ASSUME( cp_pnn && _StrLen( cp_pnn ) < MAX_PATH );
	DBGBREAK;  //  TODO: complete
    return false;
}

bln Files::IsAbsolutePath( const char *pnn, uiw parseLen = uiw_max )
{
	DBGBREAK;  //  TODO: complete
	return true;
}

NOINLINE bln Files::CreateFolder( const char *cp_where, const char *cp_name, SError *po_error )
{
	ASSUME( cp_where && cp_name && (_StrLen( cp_where ) + _StrLen( cp_name ) < MAX_PATH) );

	char a_buf[ MAX_PATH ];
    uiw len = Funcs::StrCpyAndCountWONull( a_buf, cp_where );
	_StrCpy( a_buf + len, cp_name );
    bln funcResult = false;
    SError o_error = Error::Get( Error::Ok );
    mode_t process_mask;

    if( IsFolderExists( a_buf ) )
    {
        o_error = Error::Get( Error::AlreadyExists );
        goto toExit;
    }

	process_mask = ::umask( 0 );
    funcResult = ::mkdir( a_buf, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH ) == 0;
	::umask( process_mask );
    if( !funcResult )
    {
        o_error = Error::Get( Error::Unknown );
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

NOINLINE uiw Files::ExtractNameFromString( const char *cp_str, char *p_buf, uiw parseLen/* = uiw_max */ )
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

NOINLINE uiw Files::ExtractNameWOExtFromString( const char *cp_str, char *p_buf, uiw parseLen/* = uiw_max */ )
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
    
bln Files::EnumFirstFile( CFileEnumInfo *info, const char *path, const char *mask )
{
#error
	return false;
}
    
bln Files::EnumNextFile( CFileEnumInfo *info )
{
#error
	return false;
}

void Files::EnumFilesRecursively( const char *path, const char *mask, EnumFilesCallback callback, void *argument )
{
#error
}

#endif POSIX
