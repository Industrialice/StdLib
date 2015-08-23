#include <StdCoreLib.hpp>
#include <FileIO.hpp>
#include <CVector.hpp>
#include <Files.hpp>
#include <stdio.h>

using namespace StdLib;

void GetSizes()
{
    SIZE size;
    HFONT hFont = CreateFontA(18, 18, NULL, NULL, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, "MS Sans Serif Regular");
    ASSUME( hFont );
    HDC hdc = GetDC( 0 );
    ::SelectObject( hdc, hFont );

    ui32 widthes[ 256 ] = {};
    const char *text = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789<>,./\\|][()-_=+*&^%$#@!~?;:'\" ";
    ui32 maxHeight = 0;

    for( uiw index = 0; text[ index ]; ++index )
    {
        BOOL result = GetTextExtentPoint32A( hdc, &text[ index ], 1, &size );
        ASSUME( result );
        widthes[ (uiw)text[ index ] ] = size.cx;
        if( maxHeight < size.cy )
        {
            maxHeight = size.cy;
        }
    }

    FileIO::CFile file( "widths.txt", FileIO::OpenMode::CreateAlways, FileIO::ProcMode::Write, 0 );
    if( !file.IsOpened() )
    {
        ::printf( "failed to create file widths.txt\n" );
        return;
    }

    char buf[ 1024 ];
    VC( Funcs::PrintToStr, buf,, 1023, 0, "%u", maxHeight );
    file.Write( buf, _StrLen( buf ) );

    for( uiw index = 0; text[ index ]; ++index )
    {
        VC( Funcs::PrintToStr, buf,, 1023, 0, "\x0A%c\x0A%u", text[ index ], widthes[ (uiw)text[ index ] ] );
        file.Write( buf, _StrLen( buf ) );
    }

    BOOL result = GetTextExtentPoint32A( hdc, "j", 1, &size );
    ASSUME( result );
    ::printf( "%u\n", size.cx );

    ::ReleaseDC( 0, hdc );

    ::printf( "done\n" );
}