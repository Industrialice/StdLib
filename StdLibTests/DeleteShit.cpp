#include <StdHelperLib.hpp>
#include "DeleteShit.hpp"
#include <CVector.hpp>
#include <Files.hpp>
#include <FileIO.hpp>
#include <stdio.h>
#include <Misc.hpp>
#include <CString.hpp>
#include <thread>
#include <mutex>
#include <CEvent.hpp>
#include <atomic>

using namespace StdLib;

class DeleteShitImpl
{
    std::mutex PrintfMutex;
    CEvent ThreadFreeEvent;
    struct SThreadInfo
    {
        ui64 totalSize;
        ui32 deletedCount;
        CEvent startEvent;
        CEvent endEvent;
        std::atomic_flag is_finished;
        std::atomic_flag is_continue;
        CWStr extensionToProcess;
        CWStr path;
        CEvent *threadFreeEventPointer;
        std::mutex *printfMutexPointer;
        std::thread thread;  //  keep it last, it must be initialized last

        SThreadInfo() : totalSize( 0 ), deletedCount( 0 ), startEvent( false, true ), endEvent( true, false ), is_finished( true ),
            is_continue( false ), thread( 128 * 1024, ThreadFunc, this ), threadFreeEventPointer( 0 ), printfMutexPointer( 0 )
        {}
    };
    std::unique_ptr < SThreadInfo > ThreadInfos;
    ui32 ThreadsCount;

public:
    DeleteShitImpl() : ThreadFreeEvent( true, true )
    {}

    void Perform()
    {
        TimeMoment tm = TimeMoment::CreateCurrent();

        FileIO::CFile extensions = FileIO::CFile( L"exts.txt", FileIO::OpenMode::OpenExisting, FileProcMode::Read, FileCacheMode::LinearRead, 0 );
        if( !extensions.IsOpened() )
        {
            ::printf( "can't open file exts.txt\n" );
            return;
        }

        uiw size = extensions.SizeGet();
        std::unique_ptr < char[] > bufMem( new char[ size + 1 ] );
        extensions.Read( bufMem, size, 0 );
        bufMem[ size ] = '\0';

        CVec < CStr > exts;

        char *next = bufMem;
        for( ; ; )
        {
            uiw len = Funcs::StrLenAdv( next, uiw_max, '\n' );
            exts.AppendNum();
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

        wchar_t moduleName[ MAX_PATH_LENGTH ];
        ::GetModuleFileNameW( ::GetModuleHandleW( 0 ), moduleName, MAX_PATH_LENGTH );
		FilePath pModulePath( moduleName );
		pModulePath.PopLevel();

        ThreadsCount = CPU::CPUCoresNum();
        ::printf( "creating %u threads\n", ThreadsCount );
        ThreadInfos = new SThreadInfo[ ThreadsCount ];
        for( ui32 index = 0; index < ThreadsCount; ++index )
        {
            ThreadInfos[ index ].threadFreeEventPointer = &ThreadFreeEvent;
            ThreadInfos[ index ].printfMutexPointer = &PrintfMutex;
        }

        for( uiw index = 0; index < exts.Size(); ++index )
        {
            AddFileJob( CRefVec < const char >( path, MAX_PATH_LENGTH ), exts[ index ] );
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

private:
    static void EnumFilesCallback( Files::CFileEnumInfo *info, void *argument )
    {
		SThreadInfo *ti = (SThreadInfo *)argument;
        bool result = Files::RemoveFile( info->PNN(), 0 );
        ti->printfMutexPointer->Lock();
        ::printf( "deleting file %s, result %s\n", info->PNN(), result ? "success" : "fail" );
        ti->printfMutexPointer->Unlock();
        if( result )
        {
            if( info->FileSize() != ui64_max )
            {
                ti->totalSize += info->FileSize();
            }
            ++ti->deletedCount;
        }
    }

    static void ThreadFunc( void *arg )
    {
		SThreadInfo *ti = (SThreadInfo *)arg;

        do
        {
            ti->startEvent.WaitFor();
            if( ti->is_continue.IsSet() )
            {
                Files::CFileEnumInfo info;
                Files::EnumFilesRecursively( ti->path.Data(), ti->extensionToProcess.Data(), false, EnumFilesCallback, ti );
            }

            ti->is_finished.Set();
            if( ti->is_continue.IsSet() )
            {
                ti->threadFreeEventPointer->Raise();
            }
            ti->endEvent.Raise();
        } while( ti->is_continue.IsSet() );
    }

    void AddFileJob( const CWStr &path, const CWStr &ext )
    {
        ThreadFreeEvent.WaitFor();
        for( uiw index = 0; ; ++index )
        {
            ASSUME( index < ThreadsCount );

            if( ThreadInfos[ index ].is_finished )
            {
                ThreadInfos[ index ].endEvent.WaitFor();  //  make sure that the thread has completely finished its job and waiting for a new one
                if( ThreadInfos[ index ].path.Size() || ThreadInfos[ index ].extensionToProcess.Size() )
                {
                    PrintfMutex.lock();
                    const char *tpath = ThreadInfos[ index ].path.Size() ? ThreadInfos[ index ].path.Data() : "";
                    const char *text = ThreadInfos[ index ].extensionToProcess.Size() ? ThreadInfos[ index ].extensionToProcess.Data() : "";
                    ::printf( "#thread %u# job %s%s has been completed\n", index, tpath, text );
                    PrintfMutex.unlock();
                }
                ThreadInfos[ index ].path.Assign( path.begin(), path.end() );
                ThreadInfos[ index ].extensionToProcess = ext;
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

    void WaitingForFinish()
    {
        for( uiw index = 0; index < ThreadsCount; ++index )
        {
            if( ThreadInfos[ index ].is_finished.IsSet() )
            {
                ThreadInfos[ index ].endEvent.WaitFor();
            }
            ThreadInfos[ index ].is_continue.Reset();
            ThreadInfos[ index ].startEvent.Raise();
            ThreadInfos[ index ].endEvent.WaitFor();
        }
    }
};

DeleteShit *DeleteShit::Create()
{
    return (DeleteShit *)new DeleteShitImpl();
}
    
void DeleteShit::Destroy()
{
    delete this;
}
    
void DeleteShit::Perform()
{
    ((DeleteShitImpl *)this)->Perform();
}