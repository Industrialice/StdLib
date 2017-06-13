#include "PreHeader.hpp"

#ifdef POSIX

#include "FilePath.hpp"

FilePath &FilePath::AddLevel()
{
	if( _path.IsEmpty() || _path.Back() != '/' )
	{
		_path += '/';
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
		} while( !_path.IsEmpty() && _path.Back() != '/' );
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
	NOIMPL;
}

bool FilePath::IsAbsolute() const
{
	NOIMPL;
	return false;
}

bool FilePath::HasExtension() const
{
	NOIMPL;
	return false;
}

FilePath FilePath::FileName() const
{
	NOIMPL;
	return FilePath();
}

FilePath FilePath::Extension() const
{
	NOIMPL;
	return FilePath();
}

#endif