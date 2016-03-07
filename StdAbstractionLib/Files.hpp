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
	EXTERNALD bln MoveFileTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace = false, CError *error = 0 );  //  if moving across volumes, source removing is not guaranteed
	EXTERNALD bln MoveFolderTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace = false, CError *error = 0 );  //  if moving across volumes, source removing is not guaranteed
	EXTERNALD bln MoveObjectTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace = false, CError *error = 0 );  //  if moving across volumes, source removing is not guaranteed
	EXTERNALD bln CopyFileTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace = false, CError *error = 0 );
	EXTERNALD bln CopyFolderTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace = false, CError *error = 0 );
	EXTERNALD bln CopyObjectTo( const FilePath &sourcePnn, const FilePath &targetPnn, bln is_replace = false, CError *error = 0 );
    EXTERNALD bln RemoveFile( const FilePath &pnn, CError *po_error = 0 );
    EXTERNALD bln RemoveFolder( const FilePath &path, CError *po_error = 0 );
	EXTERNALD bln RemoveObject( const FilePath &path, CError *po_error = 0 );
	EXTERNALD bln VolumeDriveName( const FilePath &path, char *RSTR output, uiw maxLen );  //  maxLen include null-symbol, if buffer was too small, returns a zero
	EXTERNALD bln IsPointToTheSameFile( const FilePath &pnn0, const FilePath &pnn1, CError *error = 0 );  //  the function may fail if it cannot open one of the files
	EXTERNALD bln IsExists( const FilePath &pnn, CError *error = 0 );  //  on fail returns false
	EXTERNALD bln IsFile( const FilePath &pnn, CError *error = 0 );  //  on fail returns false
	EXTERNALD bln IsFolder( const FilePath &pnn, CError *error = 0 );  //  on fail returns false
	EXTERNALD bln IsEmpty( const FilePath &pnn, CError *error = 0 );  //  on fail returns true, pnn can point to a file or folder
	EXTERNALD bln IsFileReadOnlyGet( const FilePath &pnn, CError *error = 0 );  //  on fail returns false
	EXTERNALD bln IsFileReadOnlySet( const FilePath &pnn, bln is_ro, CError *error = 0 );  //  on fail returns false
	EXTERNALD bln CreateNewFolder( const FilePath &where, const FilePath &name, bln is_overrideExistingObject = false, CError *po_error = 0 );
	EXTERNALD bln CreateNewFile( const FilePath &where, const FilePath &name, bln is_overrideExistingObject = false, CError *po_error = 0 );
	EXTERNALD bln IsRelativePathSupported();
	EXTERNALD FilePath CurrentWorkingPathGet();  //  empty FilePath on error
	EXTERNALD bln CurrentWorkingPathSet( const FilePath &path );
	EXTERNALD bln EnumFirstFile( CFileEnumInfo *info, const FilePath &path, const FilePath &mask );
	EXTERNALD bln EnumNextFile( CFileEnumInfo *info );
	EXTERNALD void EnumFilesRecursively( const FilePath &path, const FilePath &mask, bln is_reportFolders, EnumFilesCallback callback, void *argument );

    namespace Private
    {
        EXTERNALS void CloseEnumHandle( fileEnumHandle handle );
    }

    class CFileEnumInfo
    {
    protected:
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

        ui64 FileSize() const  //  ui64_max if not defined
        {
            return _fileSize;
        }
    };
}

}  //  namespace StdLib

#endif
