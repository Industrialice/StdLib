#include "PreHeader.hpp"

#ifdef WINDOWS

#include "Files.hpp"
#include "Funcs.hpp"

#if defined(_MSC_VER) && !defined(DEBUG)
    #pragma optimize( "s", on )  //  TODO: global solution
#endif

NOINLINE static CError FileError( DWORD code )
{
	switch( code )
	{
	case ERROR_FILE_NOT_FOUND:
	case ERROR_PATH_NOT_FOUND:
		return Error::DoesNotExist();
	case ERROR_ACCESS_DENIED:
	case ERROR_WRITE_PROTECT:
	case ERROR_SHARING_VIOLATION:
	case ERROR_LOCK_VIOLATION:
		return Error::NoAccess();
	case ERROR_NOT_ENOUGH_MEMORY:
	case ERROR_OUTOFMEMORY:
		return Error::OutOfMemory();
	case ERROR_FILE_EXISTS:
	case ERROR_ALREADY_EXISTS:
		return Error::AlreadyExists();
	default:
		return Error::CannotOpenFile();
	}
}

bln Files::MoveFileTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace, CError *error )
{
	if( !targetPnn.IsValid() )
	{
		DSA( error, Error::InvalidArgument() );
		return false;
	}

	CError checkError;
	if( !Files::IsFile( sourcePnn, &checkError ) )  //  will check the pnn
	{
		if( checkError == Error::Ok() )
		{
			checkError = Error::InvalidArgument();
		}
		DSA( error, checkError );
		return false;
	}

	return Files::MoveObjectTo( sourcePnn, targetPnn, is_replace, error );
}

bln Files::MoveFolderTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace, CError *error )
{
	if( !targetPnn.IsValid() )
	{
		DSA( error, Error::InvalidArgument() );
		return false;
	}

	CError checkError;
	if( !Files::IsFolder( sourcePnn, &checkError ) )  //  will check the pnn
	{
		if( checkError == Error::Ok() )
		{
			checkError = Error::InvalidArgument();
		}
		DSA( error, checkError );
		return false;
	}

	return Files::MoveObjectTo( sourcePnn, targetPnn, is_replace, error );
}

NOINLINE bln Files::MoveObjectTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace, CError *error )
{
	FilePath source = sourcePnn, target = targetPnn;
	bln result = false;
	CError retError;
	BOOL wapiResult;
	DWORD flags = MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH;
	if( is_replace )
	{
		flags |= MOVEFILE_REPLACE_EXISTING;
	}

	source.MakeAbsolute();
	target.MakeAbsolute();

	if( !source.IsValid() || !target.IsValid() || source == target )
	{
		retError = Error::InvalidArgument();
		goto toRet;
	}

	wapiResult = ::MoveFileExW( source.PlatformPath(), target.PlatformPath(), flags );
	if( wapiResult != TRUE )
	{
		retError = Error::Unknown();
		goto toRet;
	}

	result = true;

toRet:
	DSA( error, retError );
	return result;
}

bln Files::CopyFileTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace, CError *error )
{
	CError retError;
	bln funcResult = false;

	if( Files::IsFile( sourcePnn, &retError ) == false )  //  will check the pnn
	{
		if( retError == Error::Ok() )
		{
			retError = Error::InvalidArgument();
		}
		goto toExit;
	}

	funcResult = CopyObjectTo( sourcePnn, targetPnn, is_replace, &retError );  //  will check the pnn

toExit:
	DSA( error, retError );
	return funcResult;
}

bln Files::CopyFolderTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace, CError *error )
{
	CError retError;
	bln funcResult = false;

	if( Files::IsFolder( sourcePnn, &retError ) == false )  //  will check the pnn
	{
		if( retError == Error::Ok() )
		{
			retError = Error::InvalidArgument();
		}
		goto toExit;
	}

	funcResult = CopyObjectTo( sourcePnn, targetPnn, is_replace, &retError );  //  will check the pnn

toExit:
	DSA( error, retError );
	return funcResult;
}

bln Files::CopyObjectTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace, CError *error )
{
	CError retError;
	BOOL result = FALSE;

	if( !sourcePnn.IsValid() || !targetPnn.IsValid() )
	{
		retError = Error::InvalidArgument();
		goto toExit;
	}

	result = ::CopyFileW( sourcePnn.PlatformPath(), targetPnn.PlatformPath(), is_replace ? FALSE : TRUE );
	if( result == FALSE )
	{
		retError = FileError( ::GetLastError() );
	}

toExit:
	DSA( error, retError );
	return result == TRUE;
}

bln Files::RenameFile( const FilePath &sourcePnn, const char *newName, bln is_replace, CError *error )
{
	//  TODO:
	DSA( error, Error::Unimplemented() );
	return false;
}

bln Files::RenameFolder( const FilePath &sourcePnn, const char *newName, bln is_replace, CError *error )
{
	//  TODO:
	DSA( error, Error::Unimplemented() );
	return false;
}

bln Files::RenameObject( const FilePath &sourcePnn, const char *newName, bln is_replace, CError *error )
{
	//  TODO:
	DSA( error, Error::Unimplemented() );
	return false;
}

NOINLINE bln Files::RemoveFile( const FilePath &pnn, CError *po_error )
{
	CError retError;
	bln funcResult = false;

	if( !Files::IsFile( pnn, &retError ) )  //  will check the pnn
	{
		if( retError == Error::Ok() )
		{
			retError = Error::InvalidArgument();
		}
		goto toExit;
	}

    funcResult = ::DeleteFileW( pnn.PlatformPath() ) != 0;
    if( funcResult == false )
    {
		switch( ::GetLastError() )
		{
		case ERROR_ACCESS_DENIED:
			retError = Error::NoAccess();
			break;
		case ERROR_FILE_NOT_FOUND:
			retError = Error::NoAccess();
			break;
		default:
			retError = Error::Unknown();
		}
    }

toExit:
	DSA( po_error, retError );
    return funcResult;
}

NOINLINE bln Files::RemoveFolder( const FilePath &path, CError *po_error )  //  potentially recursive
{
	uiw len;
	CWStr buf;
    WIN32_FIND_DATAW o_find;
    HANDLE h_find;
	CError o_error;
    bln funcResult = false;
	
	if( !Files::IsFolder( path, &o_error ) )  //  will check the pnn
	{
		if( o_error == Error::Ok() )
		{
			o_error = Error::InvalidArgument();
		}
		goto toExit;
	}

	buf = path.PlatformPath();
	len = buf.Size() + 1;
	buf += L"\\*";

    h_find = ::FindFirstFileW( buf.CStr(), &o_find );
    if( h_find == INVALID_HANDLE_VALUE )
    {
        goto toExit;
    }
    do
    {
        if( !::wcscmp( o_find.cFileName, L"." ) || !::wcscmp( o_find.cFileName, L".." ) )
        {
            continue;
        }

		buf.Resize( len );
		buf += o_find.cFileName;
        if( o_find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            if( !RemoveFolder( buf.CStr(), po_error ) )
            {
                ::FindClose( h_find );
                goto toExit;
            }
        }
        else if( !RemoveFile( buf.CStr(), po_error ) )
        {
            ::FindClose( h_find );
            goto toExit;
        }
    } while( ::FindNextFileW( h_find, &o_find ) );

    ::FindClose( h_find );

    funcResult = ::RemoveDirectoryW( path.PlatformPath() ) != 0;
    if( !funcResult )
    {
        o_error = Error::Unknown();
    }

toExit:
    DSA( po_error, o_error );
    return funcResult;
}

bln Files::RemoveObject( const FilePath &path, CError *po_error )
{
	CError retError;
	bln funcResult = false;

	if( Files::IsFile( path, &retError ) )
	{
		return Files::RemoveFile( path, po_error );
	}
	else if( Files::IsFolder( path, &retError ) )
	{
		return Files::RemoveFolder( path, po_error );
	}
	DSA( po_error, retError );
	return false;
}

bln Files::VolumeDriveName( const FilePath &path, char *RSTR output, uiw maxLen )
{
	//  TODO:
	return false;
}

NOINLINE bln Files::IsPointToTheSameFile( const FilePath &pnn0, const FilePath &pnn1, CError *error )
{
	HANDLE h0 = INVALID_HANDLE_VALUE, h1 = INVALID_HANDLE_VALUE;
	CError retError;
	bln result = false;
	BY_HANDLE_FILE_INFORMATION inf0, inf1;

	if( !pnn0.IsValid() || !pnn1.IsValid() )
	{
		retError = Error::InvalidArgument();
		goto toExit;
	}

	if( pnn0 == pnn1 )
	{
		result = true;
		goto toExit;
	}

	h0 = ::CreateFileW( pnn0.PlatformPath(), GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0 );
	if( h0 == INVALID_HANDLE_VALUE )
	{
		h0 = ::CreateFileW( pnn0.PlatformPath(), GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0 );
		if( h0 == INVALID_HANDLE_VALUE )
		{
			retError = Error::CannotOpenFile();
			goto toExit;
		}
	}

	h1 = ::CreateFileW( pnn1.PlatformPath(), GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0 );
	if( h1 == INVALID_HANDLE_VALUE )
	{
		h1 = ::CreateFileW( pnn1.PlatformPath(), GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0 );
		if( h1 == INVALID_HANDLE_VALUE )
		{
			retError = Error::CannotOpenFile();
			goto toExit;
		}
	}

	if( ::GetFileInformationByHandle( h0, &inf0 ) != TRUE )
	{
		retError = Error::Unknown();
		goto toExit;
	}

	if( ::GetFileInformationByHandle( h1, &inf1 ) != TRUE )
	{
		retError = Error::Unknown();
		goto toExit;
	}

	result = inf0.nFileIndexLow == inf1.nFileIndexLow;
	result &= inf0.nFileIndexHigh == inf1.nFileIndexHigh;
	result &= inf0.dwVolumeSerialNumber == inf1.dwVolumeSerialNumber;

toExit:
	::CloseHandle( h0 );
	::CloseHandle( h1 );
	DSA( error, retError );
	return result;
}

NOINLINE bln Files::IsExists( const FilePath &pnn, CError *error )
{
	CError retError;
	bln is_file = Files::IsFile( pnn, &retError );
	DSA( error, retError );
	return retError == Error::Ok();
}

bln Files::IsFile( const FilePath &pnn, CError *error )
{
	CError retError;
	bln funcResult = false;
	DWORD attribs;

	if( !pnn.IsValid() )
	{
		retError = Error::InvalidArgument();
		goto toExit;
	}

	attribs = ::GetFileAttributesW( pnn.PlatformPath() );
	if( attribs == INVALID_FILE_ATTRIBUTES )
	{
		retError = FileError( ::GetLastError() );
		goto toExit;
	}

	funcResult = ((attribs & FILE_ATTRIBUTE_DIRECTORY) == 0);

toExit:
	DSA( error, retError );
	return funcResult;
}

bln Files::IsFolder( const FilePath &pnn, CError *error )
{
	CError retError;
	bln is_file = Files::IsFile( pnn, &retError );
	DSA( error, retError );
	if( retError == Error::Ok() )
	{
		return is_file == false;
	}
	return false;
}

bln Files::IsEmpty( const FilePath &pnn, CError *error )
{
	CError retError;
	bln is_file = Files::IsFile( pnn, error );
	DSA( error, retError );
	if( retError != Error::Ok() )
	{
		return false;
	}

	//  TODO:
	DSA( error, Error::Unimplemented() );
	return false;
}

NOINLINE bln Files::IsFileReadOnlyGet( const FilePath &pnn, CError *error )
{
	if( !pnn.IsValid() )
	{
		DSA( error, Error::InvalidArgument() );
		return false;
	}
    
	DWORD attribs = ::GetFileAttributesW( pnn.PlatformPath() );
    if( attribs == INVALID_FILE_ATTRIBUTES )
    {
		DSA( error, FileError( ::GetLastError() ) );
        return false;
    }
	DSA( error, Error::Ok() );
    return attribs & FILE_ATTRIBUTE_READONLY;
}

NOINLINE bln Files::IsFileReadOnlySet( const FilePath &pnn, bln is_ro, CError *error )
{
	if( !pnn.IsValid() )
	{
		DSA( error, Error::InvalidArgument() );
		return false;
	}

    DWORD new_attribs;
    DWORD old_attribs = ::GetFileAttributesW( pnn.PlatformPath() );
    if( old_attribs == INVALID_FILE_ATTRIBUTES )
    {
		DSA( error, FileError( ::GetLastError() ) );
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
        if( ::SetFileAttributesW( pnn.PlatformPath(), new_attribs ) != TRUE )
		{
			DSA( error, FileError( ::GetLastError() ) );
			return false;
		}
    }

	DSA( error, Error::Ok() );
    return true;
}

NOINLINE bln Files::CreateNewFolder( const FilePath &where, const FilePath &name, bln is_overrideExistingObject, CError *po_error )
{
	bln funcResult = false;
	CError o_error, existError;
	bln is_fileAlreadyExists;
	FilePath fullPath;

	if( !where.IsValid() || !name.IsValid() )
	{
		o_error = Error::InvalidArgument();
		goto toExit;
	}

	fullPath = where;
	fullPath.AddLevel();
	fullPath += name;

	is_fileAlreadyExists = Files::IsFile( fullPath, &existError );
    if( existError == Error::Ok() )
    {
		if( is_overrideExistingObject )
		{
			if( !Files::RemoveObject( fullPath, &o_error ) )
			{
				goto toExit;
			}
		}
		else
		{
			o_error = Error::AlreadyExists();
			goto toExit;
		}
    }

    funcResult = ::CreateDirectoryW( fullPath.PlatformPath(), 0 ) != 0;
    if( !funcResult )
    {
        o_error = FileError( ::GetLastError() );
    }

toExit:
    DSA( po_error, o_error );
    return funcResult;
}

NOINLINE bln Files::CreateNewFile( const FilePath &where, const FilePath &name, bln is_overrideExistingObject, CError *po_error )
{
	bln funcResult = false;
	CError o_error, existError;
	bln is_fileAlreadyExists;
	FilePath fullPath;
	HANDLE file;

	if( !where.IsValid() || !name.IsValid() )
	{
		o_error = Error::InvalidArgument();
		goto toExit;
	}

	fullPath = where;
	fullPath.AddLevel();
	fullPath += name;

	is_fileAlreadyExists = Files::IsFile( fullPath, &existError );
	if( existError == Error::Ok() )
	{
		if( is_overrideExistingObject )
		{
			if( !Files::RemoveObject( fullPath, &o_error ) )
			{
				goto toExit;
			}
		}
		else
		{
			o_error = Error::AlreadyExists();
			goto toExit;
		}
	}

	file = ::CreateFileW( fullPath.PlatformPath(), GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, is_overrideExistingObject ? CREATE_ALWAYS : CREATE_NEW, 0, 0 );
	if( file == INVALID_HANDLE_VALUE )
	{
		o_error = FileError( ::GetLastError() );
		goto toExit;
	}

	::CloseHandle( file );
	funcResult = true;

toExit:
	DSA( po_error, o_error );
	return funcResult;
}

#if 0
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

NOINLINE uiw Files::ExtractNameFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen/* = uiw_max */ )
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

NOINLINE uiw Files::ExtractNameWOExtFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen/* = uiw_max */ )
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
#endif

bln Files::IsRelativePathSupported()
{
#ifdef _WIN32_WCE
	return false;
#else
	return true;
#endif
}

#if 0
bln Files::IsAbsolutePath( const char *pnn, uiw parseLen /* = uiw_max */ )
{
	return Funcs::IsChrAlpha( pnn[ 0 ] ) && pnn[ 1 ] == ':' && (pnn[ 2 ] == '/' || pnn[ 2 ] == '\\');  //  TODO:
}

NOINLINE uiw Files::AbsolutePath( const char *RSTR cp_sourcePath, char *absPath, uiw maxLen )
{
	ASSUME( cp_sourcePath );
    #ifdef _WIN32_WCE
		uiw len = _StrLen( cp_sourcePath );
		if( len + 1 > maxLen )
		{
			return 0;
		}
		_StrCpy( cp_sourcePath, cp_sourcePath );
        return len;
    #else
        return ::GetFullPathNameA( cp_sourcePath, maxLen, absPath, 0 );
    #endif
}
#endif

FilePath Files::CurrentWorkingPathGet()
{
	wchar_t tempBuf[ MAX_PATH_LENGTH ];

	DWORD result = ::GetCurrentDirectoryW( MAX_PATH_LENGTH, tempBuf );
	if( result == 0 || result >= MAX_PATH_LENGTH )  //  if result bigger than MAX_PATH_LENGTH, MAX_PATH_LENGTH wasn't enough
	{
		return FilePath();
	}
	return FilePath( tempBuf );
}

bln Files::CurrentWorkingPathSet( const FilePath &path )
{
	if( !path.IsValid() )
	{
		return false;
	}
	return ::SetCurrentDirectoryW( path.PlatformPath() ) != FALSE;
}

struct CFileEnumerator : public Files::CFileEnumInfo
{
    NOINLINE bln EnumFirstFile( const FilePath &path, const FilePath &mask )
    {
        if( _handle != INVALID_HANDLE_VALUE )
        {
            ::FindClose( _handle );
        }

		_pnn = path;
		_pnn.Normalize();
		_pnn.AddLevel();
		_pathLen = _pnn.Length();

        WIN32_FIND_DATAW findData;

		FilePath pathAndMask = _pnn + mask;

        _handle = ::FindFirstFileW( pathAndMask.PlatformPath(), &findData );

        if( _handle == INVALID_HANDLE_VALUE )
        {
            *this = CFileEnumerator();
            return false;
        }

        if( !::wcscmp( findData.cFileName, L"." ) )
        {
            if( !::FindNextFileW( _handle, &findData ) )
            {
                *this = CFileEnumerator();
                return false;
            }
            if( !::wcscmp( findData.cFileName, L".." ) )
            {
                if( !::FindNextFileW( _handle, &findData ) )
                {
                    *this = CFileEnumerator();
                    return false;
                }
            }
        }

		_pnn += findData.cFileName;
        if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
			_pnn.AddLevel();
            _fileSize = ui64_max;
			_is_folder = true;
        }
        else
        {
            LARGE_INTEGER large_size;
            large_size.LowPart = findData.nFileSizeLow;
            large_size.HighPart = findData.nFileSizeHigh;
            _fileSize = large_size.QuadPart;
			_is_folder = false;
        }

        return true;
    }

    NOINLINE bln EnumNextFile()
    {
        ASSUME( _handle != INVALID_HANDLE_VALUE );

        WIN32_FIND_DATAW findData;

        if( !::FindNextFileW( _handle, &findData ) )
        {
            *this = CFileEnumerator();
            return false;
        }

		_pnn = FilePath( FilePath::pathType( _pnn.PlatformPath(), _pathLen ) );
		_pnn += findData.cFileName;
        if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
			_pnn.AddLevel();
            _fileSize = ui64_max;
			_is_folder = true;
        }
        else
        {
            LARGE_INTEGER large_size;
            large_size.LowPart = findData.nFileSizeLow;
            large_size.HighPart = findData.nFileSizeHigh;
            _fileSize = large_size.QuadPart;
			_is_folder = false;
        }

        return true;
    }
};

bln Files::EnumFirstFile( CFileEnumInfo *info, const FilePath &path, const FilePath &mask )
{
    return ((CFileEnumerator *)info)->EnumFirstFile( path, mask );
}

bln Files::EnumNextFile( CFileEnumInfo *info )
{
    return ((CFileEnumerator *)info)->EnumNextFile();
}

void Files::EnumFilesRecursively( const FilePath &path, const FilePath &mask, bln is_reportFolders, EnumFilesCallback callback, void *argument )
{
    UniquePtr < CFileEnumInfo > info( new CFileEnumInfo );
    if( !EnumFirstFile( info, path, L"*.*" ) )
    {
        return;
    }

    do
    {
        if( info->IsFolder() )
        {
			if( is_reportFolders )
			{
				callback( info, argument );
			}
            EnumFilesRecursively( info->PNN(), mask, is_reportFolders, callback, argument );
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

#endif
