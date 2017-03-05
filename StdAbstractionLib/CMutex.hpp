#ifndef __CMUTEX_HPP__
#define __CMUTEX_HPP__

namespace StdLib
{
	class CMutex
	{
		mutexHandle _handle;

		CMutex( const CMutex & ) = delete;
		CMutex & operator = ( const CMutex & ) = delete;

	public:
		~CMutex();
		explicit CMutex( ui32 spinCount = 0 );
		void Lock();
		void Unlock();
		bool TryLock();  //  will return true if lock succeeded

	protected:
		static void Initialize();
	};

	template < bool is_nullable = false > class CScopeLock
	{
		CMutex *_mutex;

		CScopeLock() = delete;
		CScopeLock( const CScopeLock & ) = delete;
		CScopeLock & operator = ( const CScopeLock & ) = delete;

	public:
		~CScopeLock()
		{
			if( is_nullable == false || _mutex )
			{
				_mutex->Unlock();
			}
		}

		CScopeLock( CMutex *mutex )
		{
			if( is_nullable == false || mutex )
			{
				mutex->Lock();
			}
			_mutex = mutex;
		}
	};
}

#endif