#include "PreHeader.hpp"

#ifdef WINDOWS

#include "Files.hpp"
#include "Funcs.hpp"

#if defined(_MSC_VER) && !defined(DEBUG)
    #pragma optimize( "s", on )  //  TODO: global solution
#endif

NOINLINE CError<> StdLib_FileError()
{
	DWORD code = ::GetLastError();

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
		return Error::UnknownError();
	}
}

CError<> Files::MoveFileTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace )
{
	CResult < bln > is_file = Files::IsFile( sourcePnn );  //  will check the pnn
	if( !is_file.Ok() )
	{
		return is_file.UnwrapError();
	}
	if( !is_file.Unwrap() )
	{
		return Error::InvalidArgument();
	}

	return Files::MoveObjectTo( sourcePnn, targetPnn, is_replace );  //  will check the pnn
}

CError<> Files::MoveFolderTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace )
{
	CResult < bln > is_folder = Files::IsFolder( sourcePnn );  //  will check the pnn
	if( !is_folder.Ok() )
	{
		return is_folder.UnwrapError();
	}
	if( !is_folder.Unwrap() )
	{
		return Error::InvalidArgument();
	}

	return Files::MoveObjectTo( sourcePnn, targetPnn, is_replace );  //  will check the pnn
}

NOINLINE CError<> Files::MoveObjectTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace )
{
	FilePath source = sourcePnn, target = targetPnn;
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
		return Error::InvalidArgument();
	}

	wapiResult = ::MoveFileExW( source.PlatformPath(), target.PlatformPath(), flags );
	if( wapiResult != TRUE )
	{
		return Error::UnknownError();
	}

	return Error::Ok();
}

CError<> Files::CopyFileTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace )
{
	CResult < bln > is_file = Files::IsFile( sourcePnn );  //  will check the pnn
	if( !is_file.Ok() )
	{
		return is_file.UnwrapError();
	}
	if( !is_file.Unwrap() )
	{
		return Error::InvalidArgument();
	}

	return CopyObjectTo( sourcePnn, targetPnn, is_replace );  //  will check the pnn
}

CError<> Files::CopyFolderTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace )
{
	CResult < bln > is_folder = Files::IsFolder( sourcePnn );  //  will check the pnn
	if( !is_folder.Ok() )
	{
		return is_folder.UnwrapError();
	}
	if( !is_folder.Unwrap() )
	{
		return Error::InvalidArgument();
	}

	return CopyObjectTo( sourcePnn, targetPnn, is_replace );  //  will check the pnn
}

CError<> Files::CopyObjectTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace )
{
	if( !sourcePnn.IsValid() || !targetPnn.IsValid() )
	{
		return Error::InvalidArgument();
	}

	//  TODO: Windows 7, Windows Server 2008 R2, Windows Server 2008, Windows Vista, Windows Server 2003, and Windows XP: Security resource properties for the existing file are not copied to the new file until Windows 8 and Windows Server 2012.
	BOOL result = ::CopyFileW( sourcePnn.PlatformPath(), targetPnn.PlatformPath(), is_replace ? FALSE : TRUE );
	if( result == FALSE )
	{
		return StdLib_FileError();
	}

	return Error::Ok();
}

NOINLINE CError<> Files::RemoveFile( const FilePath &pnn )
{
	CResult < bln > is_file = Files::IsFile( pnn );  //  will check the pnn
	if( !is_file.Ok() )
	{
		return is_file.UnwrapError();
	}
	if( !is_file.Unwrap() )
	{
		return Error::InvalidArgument();
	}

    BOOL result = ::DeleteFileW( pnn.PlatformPath() ) != 0;
    if( result == false )
    {
		return StdLib_FileError();
    }

	return Error::Ok();
}

NOINLINE CError<> Files::RemoveFolder( const FilePath &path )  //  potentially recursive
{
	uiw len;
	CWStr buf;
    WIN32_FIND_DATAW o_find;
    HANDLE h_find;
	
	CResult < bln > is_folder = Files::IsFolder( path );  //  will check the pnn
	if( !is_folder.Ok() )
	{
		return is_folder.UnwrapError();
	}
	if( !is_folder.Unwrap() )
	{
		return Error::InvalidArgument();
	}

	buf = path.PlatformPath();
	len = buf.Size() + 1;
	buf += L"\\*";

    h_find = ::FindFirstFileW( buf.CStr(), &o_find );
    if( h_find == INVALID_HANDLE_VALUE )
    {
        return Error::UnknownError();
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
			CError<> removeFolderResult = RemoveFolder( buf.CStr() );
            if( !removeFolderResult.Ok() )
            {
                ::FindClose( h_find );
                return removeFolderResult;
            }
        }
        else
		{
			CError<> removeFileResult = RemoveFile( buf.CStr() );
			if( !removeFileResult.Ok() )
			{
				::FindClose( h_find );
				return removeFileResult;
			}
		}
    } while( ::FindNextFileW( h_find, &o_find ) );

    ::FindClose( h_find );

    BOOL winResult = ::RemoveDirectoryW( path.PlatformPath() ) != 0;
    if( winResult != TRUE )
    {
        return Error::UnknownError();
    }

	return Error::Ok();
}

CError<> Files::RemoveObject( const FilePath &path )
{
	CResult < bln > is_file = Files::IsFile( path );
	if( is_file.Ok() && is_file.Unwrap() )
	{
		return Files::RemoveFile( path );
	}

	CResult < bln > is_folder = Files::IsFolder( path );
	if( is_folder.Ok() && is_folder.Unwrap() )
	{
		return Files::RemoveFolder( path );
	}

	if( !is_file.Ok() )
	{
		return is_file.UnwrapError();
	}
	if( !is_folder.Ok() )
	{
		return is_folder.UnwrapError();
	}
	return Error::UnknownError();
}

CError<> Files::VolumeDriveName( const FilePath &path, char *RSTR output, uiw maxLen )
{
	//  TODO:
	return Error::Unimplemented();
}

NOINLINE CResult < bln > Files::IsPointToTheSameFile( const FilePath &pnn0, const FilePath &pnn1 )
{
	using returnType = CResult < bln >;

	HANDLE h0 = INVALID_HANDLE_VALUE, h1 = INVALID_HANDLE_VALUE;
	BY_HANDLE_FILE_INFORMATION inf0, inf1;

	if( !pnn0.IsValid() || !pnn1.IsValid() )
	{
		return returnType( false, Error::InvalidArgument() );
	}

	if( pnn0 == pnn1 )
	{
		return true;
	}

	h0 = ::CreateFileW( pnn0.PlatformPath(), GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0 );
	if( h0 == INVALID_HANDLE_VALUE )
	{
		h0 = ::CreateFileW( pnn0.PlatformPath(), GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0 );
		if( h0 == INVALID_HANDLE_VALUE )
		{
			return returnType( false, Error::CannotOpenFile() );
		}
	}

	h1 = ::CreateFileW( pnn1.PlatformPath(), GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0 );
	if( h1 == INVALID_HANDLE_VALUE )
	{
		h1 = ::CreateFileW( pnn1.PlatformPath(), GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0 );
		if( h1 == INVALID_HANDLE_VALUE )
		{
			return returnType( false, Error::CannotOpenFile() );
		}
	}

	if( ::GetFileInformationByHandle( h0, &inf0 ) != TRUE )
	{
		return returnType( false, Error::UnknownError() );
	}

	if( ::GetFileInformationByHandle( h1, &inf1 ) != TRUE )
	{
		return returnType( false, Error::UnknownError() );
	}

	bln result = inf0.nFileIndexLow == inf1.nFileIndexLow;
	result &= inf0.nFileIndexHigh == inf1.nFileIndexHigh;
	result &= inf0.dwVolumeSerialNumber == inf1.dwVolumeSerialNumber;

toExit:
	::CloseHandle( h0 );
	::CloseHandle( h1 );

	return result;
}

NOINLINE CResult < bln > Files::IsExists( const FilePath &pnn )
{
	if( !pnn.IsValid() )
	{
		return CResult < bln >( false, Error::InvalidArgument() );
	}
	DWORD attribs = ::GetFileAttributesW( pnn.PlatformPath() );
	return attribs != INVALID_FILE_ATTRIBUTES;
}

CResult < bln > Files::IsFile( const FilePath &pnn )
{
	using returnType = CResult < bln >;

	DWORD attribs;

	if( !pnn.IsValid() )
	{
		return returnType( false, Error::InvalidArgument() );
	}

	attribs = ::GetFileAttributesW( pnn.PlatformPath() );
	if( attribs == INVALID_FILE_ATTRIBUTES )
	{
		return returnType( false, StdLib_FileError() );
	}

	return ((attribs & FILE_ATTRIBUTE_DIRECTORY) == 0);
}

CResult < bln > Files::IsFolder( const FilePath &pnn )
{
	auto is_file = Files::IsFile( pnn );
	if( !is_file.Ok() )
	{
		return is_file;
	}

	return is_file.Unwrap() == false;
}

CResult < bln > Files::IsFolderEmpty( const FilePath &pnn )
{
	auto is_file = Files::IsFile( pnn );
	if( !is_file.Ok() )
	{
		return is_file;
	}

	if( is_file.Unwrap() )
	{
		return CResult < bln >( false, Error::InvalidArgument() );
	}

	return CResult < bln >( false, Error::Unimplemented() );
}

NOINLINE CResult < bln > Files::IsFileReadOnlyGet( const FilePath &pnn )
{
	if( !pnn.IsValid() )
	{
		return CResult < bln >( false, Error::InvalidArgument() );
	}
    
	DWORD attribs = ::GetFileAttributesW( pnn.PlatformPath() );
    if( attribs == INVALID_FILE_ATTRIBUTES )
    {
		return CResult < bln >( false, StdLib_FileError() );
    }

    return (attribs & FILE_ATTRIBUTE_READONLY) != 0;
}

NOINLINE CError<> Files::IsFileReadOnlySet( const FilePath &pnn, bln is_ro )
{
	if( !pnn.IsValid() )
	{
		return Error::InvalidArgument();
	}

    DWORD new_attribs;
    DWORD old_attribs = ::GetFileAttributesW( pnn.PlatformPath() );
    if( old_attribs == INVALID_FILE_ATTRIBUTES )
    {
		return StdLib_FileError();
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
			return StdLib_FileError();
		}
    }

	return Error::Ok();
}

NOINLINE CError<> Files::CreateNewFolder( const FilePath &where, const FilePath &name, bln is_overrideExistingObject )
{
	if( !where.IsValid() || !name.IsValid() )
	{
		return Error::InvalidArgument();
	}

	FilePath fullPath = where;
	fullPath.AddLevel();
	fullPath += name;

	auto is_exists = Files::IsExists( fullPath );
	if( !is_exists.Ok() )
	{
		return is_exists.UnwrapError();
	}

	if( is_exists.Unwrap() )
	{
		if( is_overrideExistingObject )
		{
			auto removeResult = Files::RemoveObject( fullPath );
			if( !removeResult.Ok() )
			{
				return removeResult;
			}
		}
		else
		{
			return Error::AlreadyExists();
		}
	}

	if( ::CreateDirectoryW( fullPath.PlatformPath(), 0 ) != TRUE )
	{
		return StdLib_FileError();
	}

	return Error::Ok();
}

NOINLINE CError<> Files::CreateNewFile( const FilePath &where, const FilePath &name, bln is_overrideExistingObject )
{
	if( !where.IsValid() || !name.IsValid() )
	{
		return Error::InvalidArgument();
	}

	FilePath fullPath = where;
	fullPath.AddLevel();
	fullPath += name;

	auto is_exists = Files::IsExists( fullPath );
	if( !is_exists.Ok() )
	{
		return is_exists.UnwrapError();
	}

	if( is_exists.Unwrap() )
	{
		if( is_overrideExistingObject )
		{
			auto removeResult = Files::RemoveObject( fullPath );
			if( !removeResult.Ok() )
			{
				return removeResult;
			}
		}
		else
		{
			return Error::AlreadyExists();
		}
	}

	HANDLE file = ::CreateFileW( fullPath.PlatformPath(), GENERIC_WRITE, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, 0, is_overrideExistingObject ? CREATE_ALWAYS : CREATE_NEW, 0, 0 );
	if( file == INVALID_HANDLE_VALUE )
	{
		return StdLib_FileError();
	}

	::CloseHandle( file );

	return Error::Ok();
}

bln Files::IsRelativePathSupported()
{
#ifdef _WIN32_WCE
	return false;
#else
	return true;
#endif
}

CResult < FilePath > Files::CurrentWorkingPathGet()
{
	wchar_t tempBuf[ MAX_PATH_LENGTH ];

	DWORD result = ::GetCurrentDirectoryW( MAX_PATH_LENGTH, tempBuf );
	if( result > MAX_PATH_LENGTH )  //  if result bigger than MAX_PATH_LENGTH, MAX_PATH_LENGTH wasn't enough
	{
		return CResult < FilePath >( FilePath(), Error::OutOfMemory() );
	}
	if( result == 0 )
	{
		return CResult < FilePath >( FilePath(), Error::UnknownError() );
	}
	return FilePath( tempBuf );
}

CError<> Files::CurrentWorkingPathSet( const FilePath &path )
{
	if( !path.IsValid() )
	{
		return Error::InvalidArgument();
	}
	BOOL result = ::SetCurrentDirectoryW( path.PlatformPath() );
	if( result != TRUE )
	{
		return Error::UnknownError();
	}
	return Error::Ok();
}

CError<> Files::CFileEnumInfo::EnumFirstFile( const FilePath &path, const FilePath &mask )
{
    if( _handle != INVALID_HANDLE_VALUE )
    {
        ::FindClose( _handle );
    }

	_pnn = path;
	_pnn.Normalize();
	_pnn.AddLevel();
	_pathLen = _pnn.Length();

	FilePath pathAndMask = _pnn + mask;
	WIN32_FIND_DATAW findData;
    _handle = ::FindFirstFileW( pathAndMask.PlatformPath(), &findData );
    if( _handle == INVALID_HANDLE_VALUE )
    {
        *this = CFileEnumInfo();
        return Error::UnknownError();
    }

    if( !::wcscmp( findData.cFileName, L"." ) )
    {
        if( !::FindNextFileW( _handle, &findData ) )
        {
            *this = CFileEnumInfo();
            return Error::UnknownError();
        }
        if( !::wcscmp( findData.cFileName, L".." ) )
        {
            if( !::FindNextFileW( _handle, &findData ) )
            {
                *this = CFileEnumInfo();
                return Error::UnknownError();
            }
        }
    }

	_pnn += findData.cFileName;
    if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
    {
		_pnn.AddLevel();
        _fileSize = ui64_max;
    }
    else
    {
        LARGE_INTEGER large_size;
        large_size.LowPart = findData.nFileSizeLow;
        large_size.HighPart = findData.nFileSizeHigh;
        _fileSize = large_size.QuadPart;
    }

    return Error::Ok();
}

CError<> Files::CFileEnumInfo::EnumNextFile()
{
    ASSUME( _handle != INVALID_HANDLE_VALUE );

    WIN32_FIND_DATAW findData;
    if( !::FindNextFileW( _handle, &findData ) )
    {
        *this = CFileEnumInfo();
        return Error::UnknownError();
    }

	_pnn = FilePath( FilePath::pathType( _pnn.PlatformPath(), _pathLen, findData.cFileName, ::wcslen( findData.cFileName ) ) );
    if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
    {
		_pnn.AddLevel();
        _fileSize = ui64_max;
    }
    else
    {
        LARGE_INTEGER large_size;
        large_size.LowPart = findData.nFileSizeLow;
        large_size.HighPart = findData.nFileSizeHigh;
        _fileSize = large_size.QuadPart;
    }

    return Error::Ok();
}

CError<> Files::EnumFirstFile( CFileEnumInfo *info, const FilePath &path, const FilePath &mask )  //  friend of CFileEnumInfo
{
    return info->EnumFirstFile( path, mask );
}

CError<> Files::EnumNextFile( CFileEnumInfo *info )  //  friend of CFileEnumInfo
{
    return info->EnumNextFile();
}

void Files::Private::CloseEnumHandle( fileEnumHandle handle )
{
	if( handle != INVALID_HANDLE_VALUE )
	{
		::FindClose( handle );
	}
}

#endif
