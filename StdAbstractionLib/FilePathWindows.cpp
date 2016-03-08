#include "PreHeader.hpp"

#ifdef WINDOWS

#include "FilePath.hpp"

FilePath &FilePath::AddLevel()
{
	if( _path.IsEmpty() || (_path.Back() != L'\\' && _path.Back() != L'/') )
	{
		_path += L'\\';
	}
	return *this;
}

FilePath &FilePath::PopLevel()
{
	if( !_path.IsEmpty() )
	{
		do
		{
			_path.PopBack();
		} while( !_path.IsEmpty() && (_path.Back() != L'\\' && _path.Back() != L'/') );
	}
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
		ASSUME( tempBuf[ result ] == _path.Back() );  //  I don't think GetFullPathName can remove/change the last (back)slash?
		_path = tempBuf;
	}
#endif
}

#endif