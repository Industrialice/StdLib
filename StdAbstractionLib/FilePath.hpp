#ifndef __FILE_PATH_HPP__
#define __FILE_PATH_HPP__

#include <CString.hpp>

namespace StdLib
{
	class FilePath
	{
	public:
		typedef TCStr < pathChar > pathType;

	protected:
		pathType _path;

	public:
		/*  including null-terminator  */
		static const uiw maxPathLength = MAX_PATH_LENGTH;
		static const uiw maxFileNameLength = MAX_FILENAME_LENGTH;

		FilePath();
		FilePath( const pathChar *path );
		FilePath( const pathType &path );
		const pathChar *PlatformPath() const;
		void Append( const pathChar *path );
		void Append( const FilePath &path );
		void Append( const pathType &path );
		//  + or += operators won't add new levels, it's a simple append
		FilePath &operator += ( const pathChar *path );
		FilePath &operator += ( pathChar ch );
		FilePath &operator += ( const FilePath &path );
		FilePath &operator += ( const pathType &path );
		FilePath operator + ( const pathChar *path ) const;
		FilePath operator + ( pathChar ch ) const;
		FilePath operator + ( const FilePath &path ) const;
		FilePath operator + ( const pathType &path ) const;
		friend FilePath operator + ( const pathChar *left, const FilePath &right );
		friend FilePath operator + ( pathChar left, const FilePath &right );
		friend FilePath operator + ( pathType left, const FilePath &right );
		FilePath &operator = ( const pathChar *path );
		FilePath &operator = ( const pathType &path );
		//  binary comparisions
		bln operator == ( const pathChar *path ) const;
		bln operator == ( const FilePath &path ) const;
		bln operator == ( const pathType &path ) const;
		bln operator != ( const pathChar *path ) const;
		bln operator != ( const FilePath &path ) const;
		bln operator != ( const pathType &path ) const;
		FilePath &AddLevel();  //  ignored if the path ends on path delimiter
		FilePath &PopLevel();  //  does nothing if empty
		bln IsEmpty() const;
		uiw Length() const;
		void Normalize();  /*  in Windows, replaces / with \  */
		bln IsValid() const;  //  false if empty or too big
		void MakeAbsolute();
	};
}

#endif