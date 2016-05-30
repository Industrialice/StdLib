#include <StdCoreLib.hpp>
#include <FileIO.hpp>
#include <CVector.hpp>
#include <Files.hpp>
#include <stdio.h>
#include <conio.h>
#include <CThread.hpp>

using namespace StdLib;

namespace
{
    CThread Animator;

    HANDLE ConsoleOutut = ::GetStdHandle( STD_OUTPUT_HANDLE );
}

static void PrintWordAndCount( const char *word, uiw index, uiw count )
{
    ::system( "cls" );
    CONSOLE_SCREEN_BUFFER_INFO cbi;
    ::GetConsoleScreenBufferInfo( ConsoleOutut, &cbi );
    char buf[ 128 ];
    uiw len = Funcs::PrintToStr( buf, 127, "[%u out of %u]", index, count );
    COORD coord = { (cbi.dwSize.X - len) / 2, 0 };
    ::SetConsoleCursorPosition( ConsoleOutut, coord );
    ::printf( "%s\n%s", buf, word );
}

void Words()
{
    FileIO::CFile file( L"d:\\other\\words.txt", FileIO::OpenMode::OpenExisting, FileProcMode::Read, FileCacheMode::LinearRead );
    if( !file.IsOpened() )
    {
        ::printf( "failed to open words.txt\n" );
        return;
    }

    ui32 start, range;
    ::printf( "start from, num words: " );
    ::scanf( "%u %u", &start, &range );

    ui64 len = file.SizeGet();
    char *buf = new char[ len ];

    file.Read( buf, len, 0 );

    file.Close();

    CVec < CVec < char > > words;
    uiw pos = 0;
    ui32 skipped = 0;

    while( skipped < start && pos < len )
    {
        if( buf[ pos ] == '\n' )
        {
            ++skipped;
        }
        ++pos;
    }

    while( words.Size() < range && pos < len )
    {
        words.AppendNum();
        CVec < char > &word = words.Back();
        while( pos < len )
        {
            if( buf[ pos ] == '\n' )
            {
                ++pos;
                break;
            }
            word.Append( buf[ pos ] );
            ++pos;
        }
        word.Append( '\0' );
    }

    delete[] buf;

    LARGE_INTEGER li;
    ::QueryPerformanceCounter( &li );
    ::srand( li.LowPart );

    CVec < uiw > randoms( 0, words.Size() );

    CVec < uiw > randomsSource( 0, words.Size() );
    for( uiw index = 0; index < words.Size(); ++index )
    {
        randomsSource.Append( index );
    }

    for( uiw index = 0; index < words.Size(); ++index )
    {
        uiw r = Funcs::RandomRangeUI32( 0, randomsSource.Size() - 1 );
        randoms.Append( randomsSource[ r ] );
        randomsSource.Erase( r, 1 );
    }

    uiw index = 0;

    for( ; ; )
    {
        PrintWordAndCount( words[ randoms[ index ] ].Data(), index + 1, words.Size() );

        char ch;
        do
        {
            ch = ::getchar();
        } while( ch != 'a' && ch != 'd' && ch != 'e' && ch != 'q' && ch != 'w' );

        if( ch == 'e' )
        {
            break;
        }

        if( ch == 'q' )
        {
            index = 0;
        }
        else if( ch == 'w' )
        {
            index = words.Size() - 1;
        }
        else if( ch == 'a' )
        {
            if( index )
            {
                --index;
            }
        }
        else if( ch == 'd' )
        {
            if( index < words.Size() - 1 )
            {
                ++index;
            }
        }
    }

    ::system( "cls" );
    ::printf( "words done\n" );
}