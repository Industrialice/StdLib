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

bool FilePath::IsEmpty() const
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

bool FilePath::IsValid() const
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
		ASSUME( tempBuf[ result - 1 ] == _path.Back() );  //  I don't think GetFullPathName can remove/change the last (back)slash?
		_path = tempBuf;
	}
#endif
}

bool FilePath::IsAbsolute() const
{
#ifndef _WIN32_WCE
	NOT_IMPLEMENTED;
	return false;
#else
	return true;
#endif
}

bool FilePath::HasExtension() const
{
	NOT_IMPLEMENTED;
	return false;
}

FilePath FilePath::FileName() const
{
	if( _path.IsEmpty() || _path.Back() == L'\\' || _path.Back() == L'/' )
	{
		return FilePath();
	}

	ASSUME( IsValid() );

	pathType::IterRevConst it;
	for( it = _path.beginRev(); it != _path.endRev(); ++it )
	{
		if( *it == L'\\' || *it == L'/' )
		{
			return pathType( (it - 1).Ptr(), _path.beginRev() - it );
		}
	}

	return FilePath();
}

FilePath FilePath::Extension() const
{
	if( _path.IsEmpty() || _path.Back() == L'\\' || _path.Back() == L'/' )
	{
		return FilePath();
	}

	ASSUME( IsValid() );

	pathType::IterRevConst it;
	for( it = _path.beginRev(); it != _path.endRev(); ++it )
	{
		if( *it == L'\\' || *it == L'/' )
		{
			return FilePath();
		}

		if( *it == L'.' )
		{
			return pathType( (it - 1).Ptr(), _path.beginRev() - it );
		}
	}

	return FilePath();
}

#endif