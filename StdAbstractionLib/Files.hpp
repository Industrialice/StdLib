#ifndef __FILES_HPP__
#define __FILES_HPP__

#include <StdCoreLib.hpp>
#include "Error.hpp"

namespace StdLib {

namespace Files
{
    class CFileEnumInfo;
    typedef void (*EnumFilesCallback)( CFileEnumInfo *info, void *argument );

    EXTERNALS bln RemoveFile( const char *cp_pnn, CError *po_error );
    EXTERNALS bln RemoveFolder( const char *cp_path, CError *po_error );
    EXTERNALS bln IsFileOrFolderExists( const char *cp_papn );
    EXTERNALS bln IsFileExists( const char *cp_pnn );
    EXTERNALS bln IsFolderExists( const char *cp_path );
    EXTERNALS bln IsFileReadOnlyGet( const char *cp_pnn );  //  false if the file does not exist or an error occured  TODO: add errors struct?
    EXTERNALS bln IsFileReadOnlySet( const char *cp_pnn, bln is_ro );
    EXTERNALS bln CreateFolder( const char *cp_where, const char *cp_name, CError *po_error );
    EXTERNALS uiw ExtractPathFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );  //  with the last slash
    EXTERNALS uiw ExtractNameFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );  //  without slashes or something
    EXTERNALS uiw ExtractNameWOExtFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );
    EXTERNALS uiw ExtractExtensionFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );  //  without a dot
	EXTERNALS bln IsRelativePathSupported();
    EXTERNALS bln IsAbsolutePath( const char *pnn, uiw parseLen = uiw_max );
    EXTERNALS uiw AbsolutePath( const char *RSTR cp_sourcePath, char (&a_procedPath)[ MAX_PATH ] );
	EXTERNALS bln CurrentWorkingPathGet( char *buf, uiw maxLen, uiw *copied /* optional */ );  //  maxLen include null-symbol, if buffer was too small, returns false and sets copied to uiw_max, if error, returns false and sets copied to 0
	EXTERNALS bln CurrentWorkingPathSet( const char *path );
    EXTERNALS bln EnumFirstFile( CFileEnumInfo *info, const char *path, const char *mask );
    EXTERNALS bln EnumNextFile( CFileEnumInfo *info );
    EXTERNALS void EnumFilesRecursively( const char *path, const char *mask, EnumFilesCallback callback, void *argument );

    namespace Private
    {
        EXTERNALS void CloseEnumHandle( fileEnumHandle handle );
    }

    class CFileEnumInfo
    {
    protected:
        fileEnumHandle _handle;
        char _pnn[ MAX_PATH ];
        uiw _pathLen;
        ui64 _fileSize;

    public:
        ~CFileEnumInfo()
        {
            Private::CloseEnumHandle( _handle );
        }

        CFileEnumInfo() : _handle( fileEnumHandle_undefined )
        {
            _pnn[ 0 ] = '\0';
            _pathLen = 0;
        }

        const char *PNN() const
        {
            return _pnn;
        }

        bln IsFolder() const
        {
            if( _pathLen == 0 )
            {
                return false;
            }

            const char *start = _pnn + _pathLen;
            if( _StrChr( start, FSPathSep ) )
            {
                return true;
            }

            return false;
        }

        ui64 FileSize() const  //  ui64_max if not defined
        {
            return _fileSize;
        }
    };
}

}  //  namespace StdLib

#endif
