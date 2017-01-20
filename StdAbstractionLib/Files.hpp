#ifndef __FILES_HPP__
#define __FILES_HPP__

#include "FilePath.hpp"

namespace StdLib {

//  WARNING: there's no standard about folder name endings. It will be resolved in the future

namespace Files
{
    class CFileEnumInfo;
    typedef void (*EnumFilesCallback)( CFileEnumInfo *info, void *argument );

	//  use move functions to rename
	EXTERNALD CError<> MoveFileTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace = false );  //  if moving across volumes, source removing is not guaranteed
	EXTERNALD CError<> MoveFolderTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace = false );  //  if moving across volumes, source removing is not guaranteed
	EXTERNALD CError<> MoveObjectTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace = false );  //  if moving across volumes, source removing is not guaranteed
	EXTERNALD CError<> CopyFileTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace = false );
	EXTERNALD CError<> CopyFolderTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace = false );
	EXTERNALD CError<> CopyObjectTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace = false );
    EXTERNALD CError<> RemoveFile( const FilePath &pnn );
    EXTERNALD CError<> RemoveFolder( const FilePath &path );
	EXTERNALD CError<> RemoveObject( const FilePath &path );
	EXTERNALD CError<> VolumeDriveName( const FilePath &path, char *RSTR output, uiw maxLen );  //  maxLen include null-symbol, if buffer was too small, returns a zero
	EXTERNALD CResult < bln > IsPointToTheSameFile( const FilePath &pnn0, const FilePath &pnn1 );  //  the function may fail if it cannot open one of the files
	EXTERNALD CResult < bln > IsExists( const FilePath &pnn );
	EXTERNALD CResult < bln > IsFile( const FilePath &pnn );
	EXTERNALD CResult < bln > IsFolder( const FilePath &pnn );
	EXTERNALD CResult < bln > IsFolderEmpty( const FilePath &pnn );
	EXTERNALD CResult < bln > IsFileReadOnlyGet( const FilePath &pnn );
	EXTERNALD CError<> IsFileReadOnlySet( const FilePath &pnn, bln is_ro );
	EXTERNALD CError<> CreateNewFolder( const FilePath &where, const FilePath &name, bln is_overrideExistingObject = false );
	EXTERNALD CError<> CreateNewFile( const FilePath &where, const FilePath &name, bln is_overrideExistingObject = false );
	EXTERNALD bln IsRelativePathSupported();
	EXTERNALD CResult < FilePath > CurrentWorkingPathGet();
	EXTERNALD CError<> CurrentWorkingPathSet( const FilePath &path );
	EXTERNALD CError<> EnumFirstFile( CFileEnumInfo *info, const FilePath &path, const FilePath &mask );  //  returns Error::Ok if file info has been extracted
	EXTERNALD CError<> EnumNextFile( CFileEnumInfo *info );  //  returns Error::Ok if file info has been extracted
	EXTERNALD uiw EnumFiles( const FilePath &path, const FilePath &mask, bln is_reportFolders, EnumFilesCallback callback, void *argument );  //  returns the number of enumerated files
	EXTERNALD uiw EnumFilesRecursively( const FilePath &path, const FilePath &mask, bln is_reportFolders, EnumFilesCallback callback, void *argument );  //  returns the number of enumerated files

    namespace Private
    {
        EXTERNALS void CloseEnumHandle( fileEnumHandle handle );
    }

    class CFileEnumInfo
    {
        fileEnumHandle _handle;
		FilePath _pnn;
        uiw _pathLen;
        ui64 _fileSize;

    public:
        ~CFileEnumInfo()
        {
            Private::CloseEnumHandle( _handle );
        }

        CFileEnumInfo() : _handle( fileEnumHandle_undefined )
        {
            _pathLen = 0;
        }

        const pathChar *PlatformPNN() const
        {
            return _pnn.PlatformPath();
        }

		const FilePath &PNN() const
		{
			return _pnn;
		}

        bln IsFolder() const
        {
			return _fileSize == ui64_max;
        }

        Nullable < ui64 > FileSize() const
        {
			if( _fileSize == ui64_max )
			{
				return nullptr;
			}
            return _fileSize;
        }

	private:
		friend CError<> EnumFirstFile( CFileEnumInfo *info, const FilePath &path, const FilePath &mask );
		friend CError<> EnumNextFile( CFileEnumInfo *info );

		NOINLINE CError<> EnumFirstFile( const FilePath &path, const FilePath &mask );
		NOINLINE CError<> EnumNextFile();
    };
}

}  //  namespace StdLib

#endif
