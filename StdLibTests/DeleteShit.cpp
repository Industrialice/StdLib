#include <StdHelperLib.hpp>
#include <CVector.hpp>
#include <Files.hpp>
#include <FileIO.hpp>
#include <stdio.h>
#include <Misc.hpp>
#include <CThread.hpp>
#include <CEvent.hpp>
#include <CAtomicFlag.hpp>
#include <CMutex.hpp>

using namespace StdLib;

static void ThreadFunc( void *arg );

namespace
{
    CMutex PrintfMutex;
    CEvent ThreadFreeEvent( true, true );
    struct SThreadInfo
    {
        ui64 totalSize;
        ui32 deletedCount;
        CEvent startEvent;
        CEvent endEvent;
        CAtomicFlag is_finished;
        CAtomicFlag is_continue;
        CVec < char, void > extensionToProcess;
        CVec < char, void > path;
        CThread thread;  //  keep it last, it must be initialized last

        SThreadInfo() : totalSize( 0 ), deletedCount( 0 ), startEvent( false, true ), endEvent( true, false ), is_finished( true ), is_continue( false ),
            thread( 128 * 1024, ThreadFunc, this )
        {}
    };
    UniquePtr < SThreadInfo > ThreadInfos;
    ui32 ThreadsCount;
}

static void EnumFilesCallback( Files::CFileEnumInfo *info, void *argument )
{
    SThreadInfo *ti = (SThreadInfo *)argument;
    bln result = Files::RemoveFile( info->PNN(), 0 );
    PrintfMutex.Lock();
    ::printf( "deleting file %s, result %s\n", info->PNN(), result ? "success" : "fail" );
    PrintfMutex.Unlock();
    if( result )
    {
        if( info->FileSize() != ui64_max )
        {
            ti->totalSize += info->FileSize();
        }
        ++ti->deletedCount;
    }
}

void ThreadFunc( void *arg )
{
    SThreadInfo *ti = (SThreadInfo *)arg;

    for( ; ; )
    {
        ti->startEvent.WaitFor();
        if( ti->is_continue.IsSet() == false )
        {
            break;
        }
        ASSUME( ti->is_finished.IsSet() == false );

        Files::CFileEnumInfo info;
        Files::EnumFilesRecursively( ti->path.Data(), ti->extensionToProcess.Data(), EnumFilesCallback, ti );

        ti->is_finished.Set();
        ThreadFreeEvent.Raise();
        ti->endEvent.Raise();
    }

    ti->is_finished.Set();
    ThreadFreeEvent.Raise();
    ti->endEvent.Raise();
}

static void AddFileJob( const CVec < char > &path, const CVec < char > &ext )
{
    ThreadFreeEvent.WaitFor();
    for( uiw index = 0; ; ++index )
    {
        ASSUME( index < ThreadsCount );

        if( ThreadInfos[ index ].is_finished.IsSet() )
        {
            ThreadInfos[ index ].endEvent.WaitFor();  //  make sure that the thread has completely finished its job and waiting for a new job
            if( ThreadInfos[ index ].path.Size() || ThreadInfos[ index ].extensionToProcess.Size() )
            {
                PrintfMutex.Lock();
                const char *tpath = ThreadInfos[ index ].path.Size() ? ThreadInfos[ index ].path.Data() : "";
                const char *text = ThreadInfos[ index ].extensionToProcess.Size() ? ThreadInfos[ index ].extensionToProcess.Data() : "";
                ::printf( "#thread %u# job %s%s has finished\n", index, tpath, text );
                PrintfMutex.Unlock();
            }
            ThreadInfos[ index ].path = path.ToRef();
            ThreadInfos[ index ].extensionToProcess = ext.ToRef();
            ThreadInfos[ index ].is_finished.Reset();
            ThreadInfos[ index ].endEvent.Reset();
            ThreadInfos[ index ].is_continue.Set();
            ThreadInfos[ index ].startEvent.Raise();  //  start the new job
            break;
        }
    }
    //  there can be a few waiting threads
    for( uiw index = 0; index < ThreadsCount; ++index )
    {
        if( ThreadInfos[ index ].is_finished.IsSet() )
        {
            ThreadFreeEvent.Raise();
            break;
        }
    }
}

static void WaitingForFinish()
{
    for( uiw index = 0; index < ThreadsCount; ++index )
    {
        ThreadInfos[ index ].is_continue.Reset();
        ThreadInfos[ index ].startEvent.Raise();
        ThreadInfos[ index ].endEvent.WaitFor();
        ASSUME( ThreadInfos[ index ].is_finished.IsSet() );
    }
}

void DeleteShit()
{
    CTC tc = CTC( true );

    FileIO::CFile extensions = FileIO::CFile( "exts.txt", FileIO::OpenMode::OpenExisting, FileIO::ProcMode::Read | FileIO::ProcMode::SequentialScan, 0 );
    if( !extensions.IsOpened() )
    {
        ::printf( "can't open file exts.txt\n" );
        return;
    }

    uiw size = extensions.SizeGet();
    UniquePtr < char[] > bufMem( new char[ size + 1 ] );
    extensions.Read( bufMem, size, 0 );
    bufMem[ size ] = '\0';

    CVec < CVec < char > > exts;

    char *next = bufMem;
    for( ; ; )
    {
        uiw len = Funcs::StrLenAdv( next, uiw_max, '\n' );
        exts.PushBackNum();
        exts.Back().Resize( len + 1 );
        Funcs::StrCpyAdv( exts.Back().Data(), next, len );

        ::printf( "adding extension: %s\n", exts.Back().Data() );

        next = _StrChr( next, '\n' );
        if( next == 0 )
        {
            break;
        }
        ++next;
    }

    char moduleName[ MAX_PATH ], path[ MAX_PATH ];
    ::GetModuleFileNameA( ::GetModuleHandleA( 0 ), moduleName, MAX_PATH );
    Files::ExtractPathFromString( moduleName, path );

    ThreadsCount = CPU::CoresNum();
    ::printf( "creating %u threads\n", ThreadsCount );
    ThreadInfos = new SThreadInfo[ ThreadsCount ];

    for( uiw index = 0; index < exts.Size(); ++index )
    {
        AddFileJob( CVec < char >( path, MAX_PATH, MAX_PATH ), exts[ index ] );
    }

    WaitingForFinish();
    
    for( ui32 index = 0; index < ThreadsCount; ++index )
    {
        char normalizedMem[ 256 ];
        Funcs::NormalizeMem64( ThreadInfos[ index ].totalSize, normalizedMem );
        ::printf( "#thread %u# deleted count %u, deleted size %s\n", index, ThreadInfos[ index ].deletedCount, normalizedMem );
    }
    ::printf( "time spent %.14lf\n", tc.Get64() );
}