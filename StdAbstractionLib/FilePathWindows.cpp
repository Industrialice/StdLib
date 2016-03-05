#include "PreHeader.hpp"

#ifdef WINDOWS

#include "FilePath.hpp"

FilePath::FilePath()
{}

FilePath::FilePath( const pathChar *path ) : _path( path )
{}

FilePath::FilePath( const pathType &path ) : _path( path )
{}

const pathChar *FilePath::PlatformPath() const
{
	return _path.CStr();
}

void FilePath::Append( const pathChar *path )
{
	_path += path;
}

void FilePath::Append( const FilePath &path )
{
	_path += path._path;
}

void FilePath::Append( const pathType &path )
{
	_path += path;
}

FilePath &FilePath::operator += ( const pathChar *path )
{
	_path += path;
	return *this;
}

FilePath &FilePath::operator += ( pathChar ch )
{
	_path += ch;
	return *this;
}

FilePath &FilePath::operator += ( const FilePath &path )
{
	_path += path._path;
	return *this;
}

FilePath &FilePath::operator += ( const pathType &path )
{
	_path += path;
	return *this;
}

FilePath FilePath::operator + ( const pathChar *path ) const
{
	return FilePath( TRY_MOVE( _path + path ) );
}

FilePath FilePath::operator + ( pathChar ch ) const
{
	return FilePath( TRY_MOVE( _path + ch ) );
}

FilePath FilePath::operator + ( const FilePath &path ) const
{
	return FilePath( TRY_MOVE( _path + path._path ) );
}

FilePath FilePath::operator + ( const pathType &path ) const
{
	return FilePath( TRY_MOVE( _path + path ) );
}

FilePath StdLib::operator + ( const pathChar *left, const FilePath &right )  //  friend
{
	return FilePath( TRY_MOVE( left + right._path ) );
}

FilePath StdLib::operator + ( pathChar left, const FilePath &right )  //  friend
{
	return FilePath( TRY_MOVE( left + right._path ) );
}

FilePath StdLib::operator + ( FilePath::pathType left, const FilePath &right )  //  friend
{
	return FilePath( TRY_MOVE( left + right._path ) );
}

FilePath &FilePath::operator = ( const pathChar *path )
{
	_path = path;
	return *this;
}

FilePath &FilePath::operator = ( const pathType &path )
{
	_path = path;
	return *this;
}

bln FilePath::operator == ( const pathChar *path ) const
{
	return _path == path;
}

bln FilePath::operator == ( const FilePath &path ) const
{
	return _path == path._path;
}

bln FilePath::operator == ( const pathType &path ) const
{
	return _path == path;
}

bln FilePath::operator != ( const pathChar *path ) const
{
	return _path != path;
}

bln FilePath::operator != ( const FilePath &path ) const
{
	return _path != path._path;
}

bln FilePath::operator != ( const pathType &path ) const
{
	return _path != path;
}

FilePath &FilePath::AddLevel()
{
	if( _path.Back() != L'\\' && _path.Back() != L'/' )
	{
		_path += L'\\';
	}
	return *this;
}

FilePath &FilePath::PopLevel()
{
	NOT_IMPLEMENTED;
	return *this;
}

bln FilePath::IsEmpty() const
{
	return _path.IsEmpty();
}

uiw FilePath::Length() const
{
	return _path.Size();
}

void FilePath::Normalize()
{
	for( pathType::Iter it = _path.begin(); it != _path.end(); ++it )
	{
		if( *it == L'/' )
		{
			*it = L'\\';
		}
	}
}

bln FilePath::IsValid() const
{
	if( _path.IsEmpty() )
	{
		return false;
	}

	if( _path.Size() >= MAX_PATH_LENGTH )
	{
		return false;
	}

	return true;  //  TODO: file names length check
}

void FilePath::MakeAbsolute()
{
#ifndef _WIN32_WCE
	wchar_t tempBuf[ MAX_PATH_LENGTH ];
	DWORD result = ::GetFullPathNameW( _path.CStr(), MAX_PATH_LENGTH, tempBuf, 0 );
	if( result )
	{
		ASSUME( tempBuf[ result ] == _path.Back() );  //  I don't thing GetFullPathName can remove/change the last (back)slash?
		_path = tempBuf;
	}
#endif
}

#endif