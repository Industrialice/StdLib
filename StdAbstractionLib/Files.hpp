#ifndef __FILES_HPP__
#define __FILES_HPP__

#include <StdCoreLib.hpp>
#include "Error.hpp"

namespace StdLib {

namespace Files
{
    class CFileEnumInfo;
    typedef void (*EnumFilesCallback)( CFileEnumInfo *info, void *argument );

    EXTERNAL bln RemoveFile( const char *cp_pnn, SError *po_error );
    EXTERNAL bln RemoveFolder( const char *cp_path, SError *po_error );
    EXTERNAL bln IsFileOrFolderExists( const char *cp_papn );
    EXTERNAL bln IsFileExists( const char *cp_pnn );
    EXTERNAL bln IsFolderExists( const char *cp_path );
    EXTERNAL bln IsFileReadOnlyGet( const char *cp_pnn );  //  false if file does not exist or error
    EXTERNAL bln IsFileReadOnlySet( const char *cp_pnn, bln is_ro );
    EXTERNAL bln IsAbsolutePath( const char *pnn, uiw parseLen = uiw_max );
    EXTERNAL bln CreateFolder( const char *cp_where, const char *cp_name, SError *po_error );
    EXTERNAL uiw ExtractPathFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );  //  with last slash
    EXTERNAL uiw ExtractNameFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );  //  without slashes or something
    EXTERNAL uiw ExtractNameWOExtFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );
    EXTERNAL uiw ExtractExtensionFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );  //  without dot
    EXTERNAL uiw AbsolutePath( const char *RSTR cp_sourcePath, char a_procedPath[ MAX_PATH ] );
    EXTERNAL bln EnumFirstFile( CFileEnumInfo *info, const char *path, const char *mask );
    EXTERNAL bln EnumNextFile( CFileEnumInfo *info );
    EXTERNAL void EnumFilesRecursively( const char *path, const char *mask, EnumFilesCallback callback, void *argument );

    namespace Private
    {
        EXTERNAL void CloseEnumHandle( fileEnumHandle handle );
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

#endif __FILES_HPP__