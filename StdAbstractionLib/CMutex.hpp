#ifndef __CMUTEX_HPP__
#define __CMUTEX_HPP__

#include <StdCoreLib.hpp>

namespace StdLib
{
	class CMutex
	{
		mutexHandle _handle;

		CMutex( const CMutex & );
		CMutex & operator = ( const CMutex & );

	public:
		~CMutex();
		CMutex( unsigned int spinCount = 0 );
		void Lock();
		void Unlock();
		bool TryLock();  //  will return true if lock succeeded

	protected:
		static void Initialize();
	};

	template < bool is_nullable = false > class CScopeLock
	{
		CMutex *_mutex;

		CScopeLock();
		CScopeLock( const CScopeLock & );
		CScopeLock & operator = ( const CScopeLock & );

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

#endif __CMUTEX_HPP__