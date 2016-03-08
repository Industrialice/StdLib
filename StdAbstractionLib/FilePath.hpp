#ifndef __FILE_PATH_HPP__
#define __FILE_PATH_HPP__

#include <CString.hpp>

namespace StdLib
{
	class FilePath
	{
	public:
		typedef TCStr < pathChar > pathType;

	private:
		pathType _path;

	public:
		/*  including null-terminator  */
		static const uiw maxPathLength = MAX_PATH_LENGTH;
		static const uiw maxFileNameLength = MAX_FILENAME_LENGTH;

		FilePath() {}
		FilePath( const pathChar *path ) : _path( path ) {}
		FilePath( const pathType &path ) : _path( path ) {}
		const pathChar *PlatformPath() const { return _path.CStr(); }
		void Append( const pathChar *path ) { _path += path; }
		void Append( const FilePath &path ) { _path += path._path; }
		void Append( const pathType &path ) { _path += path; }
		FilePath &operator += ( const pathChar *path ) { _path += path;	return *this; }
		FilePath &operator += ( pathChar ch ) {	_path += ch; return *this; }
		FilePath &operator += ( const FilePath &path ) { _path += path._path; return *this;	}
		FilePath &operator += ( const pathType &path ) { _path += path;	return *this; }
		FilePath operator + ( const pathChar *path ) const { return FilePath( TRY_MOVE( _path + path ) ); }
		FilePath operator + ( pathChar ch ) const {	return FilePath( TRY_MOVE( _path + ch ) ); }
		FilePath operator + ( const FilePath &path ) const { return FilePath( TRY_MOVE( _path + path._path ) );	}
		FilePath operator + ( const pathType &path ) const { return FilePath( TRY_MOVE( _path + path ) ); }
		friend FilePath operator + ( const pathChar *left, const FilePath &right ) { return FilePath( TRY_MOVE( left + right._path ) ); }
		friend FilePath operator + ( pathChar left, const FilePath &right ) { return FilePath( TRY_MOVE( left + right._path ) ); }
		friend FilePath operator + ( pathType left, const FilePath &right ) { return FilePath( TRY_MOVE( left + right._path ) ); }
		FilePath &operator = ( const pathChar *path ) {	_path = path; return *this;	}
		FilePath &operator = ( const pathType &path ) {	_path = path; return *this;	}
		bln operator == ( const pathChar *path ) const { return _path == path; }
		bln operator == ( const FilePath &path ) const { return _path == path._path; }
		bln operator == ( const pathType &path ) const { return _path == path; }
		bln operator != ( const pathChar *path ) const { return _path != path; }
		bln operator != ( const FilePath &path ) const { return _path != path._path; }
		bln operator != ( const pathType &path ) const { return _path != path; }
		FilePath &AddLevel();  //  ignored if the path ends on path delimiter
		FilePath &PopLevel();  /*  does nothing if empty, C:\Pictures\ becomes C:\, C:\Pictures becomes C:\  */
		bln IsEmpty() const;
		uiw Length() const;
		void Normalize();  /*  in Windows, replaces / with \, does nothing on POSIX  */
		bln IsValid() const;  //  false if empty or too big
		void MakeAbsolute();
	};
}

#endif