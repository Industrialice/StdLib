#ifndef __MEMORY_STREAM_INTERFACE_HPP__
#define __MEMORY_STREAM_INTERFACE_HPP__

namespace StdLib
{
	struct MemoryStreamInterface
	{
		virtual ~MemoryStreamInterface() {}

		virtual uiw Size() const = 0;
		virtual uiw Resize( uiw newSize ) = 0;  //  returns final size, it can be lower than newSize if the stream had failed to allocate enough memory
		virtual ui8 *Memory() = 0;
		virtual const ui8 *Memory() const = 0;
		virtual const ui8 *CMemory() const = 0;
		virtual bln IsReadable() const = 0;
		virtual bln IsWritable() const = 0;
	};

	template < uiw size > class MemoryStreamFixed : public MemoryStreamInterface
	{
		ui8 _buffer[ size ];
		uiw _currentSize;

	public:
		MemoryStreamFixed() : _currentSize( 0 )
		{}

		virtual uiw Size() const override final
		{
			return _currentSize;
		}

		virtual uiw Resize( uiw newSize ) override final
		{
			if( newSize > size )
			{
				newSize = size;
			}
			_currentSize = newSize;
			return _currentSize;
		}

		virtual ui8 *Memory() override final
		{
			return _buffer;
		}

		virtual const ui8 *Memory() const override final
		{
			return _buffer;
		}

		virtual const ui8 *CMemory() const override final
		{
			return _buffer;
		}

		virtual bln IsReadable() const override final
		{
			return true;
		}

		virtual bln IsWritable() const override final
		{
			return true;
		}
	};

	class MemoryStreamFixedExt : public MemoryStreamInterface
	{
		ui8 *_writeBuffer;
		const ui8 *_readBuffer;
		uiw _maxSize;
		uiw _currentSize;

	public:
		MemoryStreamFixedExt() : _writeBuffer( 0 ), _readBuffer( 0 ), _maxSize( 0 ), _currentSize( 0 )
		{}

		MemoryStreamFixedExt( void *buffer, uiw maxSize, uiw currentSize = 0 ) : _writeBuffer( (ui8 *)buffer ), _readBuffer( (ui8 *)buffer ), _maxSize( maxSize ), _currentSize( currentSize )
		{}

		MemoryStreamFixedExt( const void *buffer, uiw maxSize, uiw currentSize = 0 ) : _writeBuffer( 0 ), _readBuffer( (ui8 *)buffer ), _maxSize( maxSize ), _currentSize( currentSize )
		{}

		void SetBuffer( void *buffer, uiw maxSize, uiw currentSize = 0 )
		{
			this->_readBuffer = this->_writeBuffer = (ui8 *)buffer;
			this->_maxSize = maxSize;
			this->_currentSize = currentSize;
		}

		void SetBuffer( const void *buffer, uiw maxSize, uiw currentSize = 0 )
		{
			this->_readBuffer = (ui8 *)buffer;
			this->_writeBuffer = 0;
			this->_maxSize = maxSize;
			this->_currentSize = currentSize;
		}

		virtual uiw Size() const override final
		{
			return _currentSize;
		}

		virtual uiw Resize( uiw newSize ) override final
		{
			if( newSize > _maxSize )
			{
				newSize = _maxSize;
			}
			_currentSize = newSize;
			return _currentSize;
		}

		virtual ui8 *Memory() override final
		{
			return _writeBuffer;
		}

		virtual const ui8 *Memory() const override final
		{
			return _readBuffer;
		}

		virtual const ui8 *CMemory() const override final
		{
			return _readBuffer;
		}

		virtual bln IsReadable() const override final
		{
			return _readBuffer != 0;
		}

		virtual bln IsWritable() const override final
		{
			return _writeBuffer != 0;
		}
	};

	class MemoryStreamHeap : public MemoryStreamInterface
	{
		ui8 *_buffer;
		uiw _currentSize;

	public:
		~MemoryStreamHeap()
		{
			free( _buffer );
		}

		MemoryStreamHeap() : _buffer( 0 ), _currentSize( 0 )
		{}

		virtual uiw Size() const override final
		{
			return _currentSize;
		}

		virtual uiw Resize( uiw newSize ) override final
		{
			if( newSize != _currentSize )
			{
				_currentSize = newSize;
				newSize += newSize == 0;
				_buffer = (ui8 *)realloc( _buffer, newSize );
			}
			return _currentSize;
		}

		virtual ui8 *Memory() override final
		{
			return _buffer;
		}

		virtual const ui8 *Memory() const override final
		{
			return _buffer;
		}

		virtual const ui8 *CMemory() const override final
		{
			return _buffer;
		}

		virtual bln IsReadable() const override final
		{
			return true;
		}

		virtual bln IsWritable() const override final
		{
			return true;
		}
	};
}

#endif