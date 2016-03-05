#ifndef __CMUTEX_HPP__
#define __CMUTEX_HPP__

namespace StdLib
{
	class CMutex
	{
		mutexHandle _handle;

		CMutex( const CMutex & );
		CMutex & operator = ( const CMutex & );

	public:
		~CMutex();
		CMutex( ui32 spinCount = 0 );
		void Lock();
		void Unlock();
		bln TryLock();  //  will return true if lock succeeded

	protected:
		static void Initialize();
	};

	template < bln is_nullable = false > class CScopeLock
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

#endif