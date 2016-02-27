#ifndef __FILES_HPP__
#define __FILES_HPP__

#include <StdCoreLib.hpp>
#include <CString.hpp>

namespace StdLib {

namespace Files
{
    class CFileEnumInfo;
    typedef void (*EnumFilesCallback)( CFileEnumInfo *info, void *argument );

    EXTERNALD bln RemoveFile( const char *cp_pnn, CError *po_error );
    EXTERNALD bln RemoveFolder( const char *cp_path, CError *po_error );
	EXTERNALD bln IsFileOrFolderExists( const char *cp_papn );
	EXTERNALD bln IsFileExists( const char *cp_pnn );
	EXTERNALD bln IsFolderExists( const char *cp_path );
	EXTERNALD bln IsFileReadOnlyGet( const char *cp_pnn );  //  false if the file does not exist or an error occured  TODO: add errors struct?
	EXTERNALD bln IsFileReadOnlySet( const char *cp_pnn, bln is_ro );
	EXTERNALD bln CreateFolder( const char *cp_where, const char *cp_name, CError *po_error );
	EXTERNALD uiw ExtractPathFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );  //  with the last slash
	EXTERNALD uiw ExtractNameFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );  //  without slashes or something
	EXTERNALD uiw ExtractNameWOExtFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );
	EXTERNALD uiw ExtractExtensionFromString( const char *cp_str, char *RSTR p_buf, uiw parseLen = uiw_max );  //  without a dot
	EXTERNALD bln IsRelativePathSupported();
	EXTERNALD bln IsAbsolutePath( const char *pnn, uiw parseLen = uiw_max );
	EXTERNALD uiw AbsolutePath( const char *RSTR cp_sourcePath, char *absPath, uiw maxLen );  //  maxLen include null-symbol, if buffer was too small, returns a zero
	EXTERNALD bln CurrentWorkingPathGet( char *buf, uiw maxLen, uiw *copied /* optional */ );  //  maxLen include null-symbol, if buffer was too small, returns false and sets copied to uiw_max, if error, returns false and sets copied to 0
	EXTERNALD bln CurrentWorkingPathSet( const char *path );
	EXTERNALD bln EnumFirstFile( CFileEnumInfo *info, const char *path, const char *mask );
	EXTERNALD bln EnumNextFile( CFileEnumInfo *info );
	EXTERNALD void EnumFilesRecursively( const char *path, const char *mask, bln is_reportFolders, EnumFilesCallback callback, void *argument );

    namespace Private
    {
        EXTERNALS void CloseEnumHandle( fileEnumHandle handle );
    }

    class CFileEnumInfo
    {
    protected:
        fileEnumHandle _handle;
		TCStr < char, 128 > _pnn;  //  TODO: won't work from a dll
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

        const char *PNN() const
        {
            return _pnn.CStr();
        }

        bln IsFolder() const
        {
            if( _pathLen == 0 )
            {
                return false;
            }

            const char *start = _pnn.CStr() + _pathLen;
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
