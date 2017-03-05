#ifndef __MEMORY_STREAM_CONTAINER_HPP__
#define __MEMORY_STREAM_CONTAINER_HPP__

#include <MemoryStreamInterface.hpp>

namespace StdLib
{
	//  must be a linear container
	template < typename Container > class MemoryStreamContainer final : public MemoryStreamInterface
	{
		Container *_writableContainer;
		const Container *_readableContainer;

	public:
		MemoryStreamContainer() : _writableContainer( 0 ), _readableContainer( 0 )
		{}

		MemoryStreamContainer( Container *container ) : _writableContainer( container ), _readableContainer( container )
		{}

		MemoryStreamContainer( const Container *container ) : _writableContainer( 0 ), _readableContainer( container )
		{}

		void SetContainer( Container *container )
		{
			this->_readableContainer = this->_writableContainer = container;
		}

		void SetContainer( const Container *container )
		{
			this->_readableContainer = container;
			this->_writableContainer = 0;
		}

		virtual uiw Size() const override
		{
			if (_readableContainer == 0 )
			{
				return 0;
			}
			return _readableContainer->Size();
		}

		virtual uiw Resize( uiw newSize ) override
		{
			if (_writableContainer == 0 )
			{
				return 0;
			}
			_writableContainer->Resize( newSize, false );
			return _writableContainer->Size();
		}

		virtual ui8 *Memory() override
		{
			if (_writableContainer == 0 )
			{
				return 0;
			}
			return (ui8 *)_writableContainer->Data();
		}

		virtual const ui8 *Memory() const override
		{
			if (_readableContainer == 0 )
			{
				return 0;
			}
			return (const ui8 *)_readableContainer->Data();
		}

		virtual const ui8 *CMemory() const override
		{
			return MemoryStreamContainer::Memory();
		}

		virtual bool IsReadable() const override
		{
			return _readableContainer != 0;
		}

		virtual bool IsWritable() const override
		{
			return _writableContainer != 0;
		}
	};

	template < typename Container > MemoryStreamContainer < Container > CreateMemoryStreamContainer( Container *container )
	{
		return MemoryStreamContainer < Container >( container );
	}

	//  must be a linear container
	template < typename Container > class MemoryStreamSTL final : public MemoryStreamInterface
	{
		Container *_writableContainer;
		const Container *_readableContainer;

	public:
		MemoryStreamSTL() : _writableContainer( 0 ), _readableContainer( 0 )
		{}

		MemoryStreamSTL( Container *container ) : _writableContainer( container ), _readableContainer( container )
		{}

		MemoryStreamSTL( const Container *container ) : _writableContainer( 0 ), _readableContainer( container )
		{}

		void SetContainer( Container *container )
		{
			this->_readableContainer = this->_writableContainer = container;
		}

		void SetContainer( const Container *container )
		{
			this->_readableContainer = container;
			this->_writableContainer = 0;
		}

		virtual uiw Size() const override
		{
			if (_readableContainer == 0 )
			{
				return 0;
			}
			return _readableContainer->size();
		}

		virtual uiw Resize( uiw newSize ) override
		{
			if (_writableContainer == 0 )
			{
				return 0;
			}
			_writableContainer->resize( newSize, false );
			return _writableContainer->size();
		}

		virtual ui8 *Memory() override
		{
			if (_writableContainer == 0 )
			{
				return 0;
			}
			return (ui8 *)_writableContainer->data();
		}

		virtual const ui8 *Memory() const override
		{
			if (_readableContainer == 0 )
			{
				return 0;
			}
			return (const ui8 *)_readableContainer->data();
		}

		virtual const ui8 *CMemory() const override
		{
			return MemoryStreamSTL::Memory();
		}

		virtual bool IsReadable() const override
		{
			return _readableContainer != 0;
		}

		virtual bool IsWritable() const override
		{
			return _writableContainer != 0;
		}
	};

	template < typename Container > MemoryStreamSTL < Container > CreateMemoryStreamSTL( Container *container )
	{
		return MemoryStreamSTL < Container >( container );
	}
}

#endif
