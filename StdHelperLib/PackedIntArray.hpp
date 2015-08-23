#ifndef __PACKED_INT_ARRAY_HPP__
#define __PACKED_INT_ARRAY_HPP__

#include "CVector.hpp"

namespace StdLib {

//  0. when pack is equal to nativeBits, packiarr will be as fast as a native array
//  1. packs 1, 2 or 4 have noticeable overhead, especially for signed values
//  2. packs [others] will have even more overhead
//  TODO: this is still a proto, probably bugged

enum packiarr_OutOfRangeReaction { packiarr_OutOfRangeIgnore, packiarr_OutOfRangeClamp, packiarr_OutOfRangeWrap };

template < const uiw pack, bln is_signed, const packiarr_OutOfRangeReaction oorr = packiarr_OutOfRangeIgnore, const bln is_runtimeBoundsCheck = false, const bln is_debugOverflowCheck = false > class __packiarr_abstract__
{
protected:
    typedef typename IntWithSizeAndSign
        < pack <= 8 ? 8 :
        pack <= 16 ? 16 :
        pack <= 32 ? 32 : 64,
        is_signed >::type_t native_t;

    enum PackType { PackNative, PackPower, PackOther };

    typedef typename TypeDesc < native_t >::uint_variant unative_t;
    typedef typename TypeDesc < native_t >::int_variant inative_t;

    static const uiw nativeBits = sizeof(native_t) * 8;
    static const PackType packType = TERSWITCH2( pack == 8 || pack == 16 || pack == 32 || pack == 64, PackNative,
                                                 pack == 1 || pack == 2 || pack == 4, PackPower,
                                                 PackOther);
    static const native_t sc_maxVal = TypeDesc < unative_t >::max >> (nativeBits - pack) >> (native_t)is_signed;
    static const native_t sc_minVal = is_signed ? -(inative_t)(TypeDesc < unative_t >::max >> (nativeBits - pack) >> 1) - 1 : 0;
    static const native_t sc_toPositive = sc_maxVal;  //  used only when signed, you must add 1 manually when use
    static const unative_t sc_toPositiveU = (unative_t)sc_maxVal + 1;  //  used only when signed
    static const unative_t sc_mask = TypeDesc < unative_t >::max >> (nativeBits - pack);
    static const unative_t nativeMask = TypeDesc < unative_t >::max;
    static const uiw sc_offsetToSign = nativeBits - pack;
    static const native_t signCheckBit = 1 << nativeBits - pack;

public:
    __packiarr_abstract__()
    {
        STATIC_CHECK( pack > 1 || !is_signed, "value can't be signed when pack is 1" );
        STATIC_CHECK( pack > 0, "incorrect pack value for packiarr" );
        STATIC_CHECK( pack <= nativeBits, "incorrect nearest native type for packiarr" );
        STATIC_CHECK( packType != PackNative || sizeof(native_t) == pack / 8, "incorrect nearest native type for packiarr" );
        STATIC_CHECK( packType != PackPower || sizeof(native_t) == 1, "incorrect nearest native type for packiarr" );
        STATIC_CHECK( packType != PackOther || nativeBits / 2 < pack, "incorrect nearest native type for packiarr" );
    }

    void Set( unative_t *arr, const uiw index, native_t value )
    {
        if( oorr == packiarr_OutOfRangeIgnore )
        {
            if( is_debugOverflowCheck )
            {
                ASSUME( value >= sc_minVal );
                ASSUME( value <= sc_maxVal );
            }
        }
        else if( oorr == packiarr_OutOfRangeClamp )
        {
            value = Funcs::Clamp( value, sc_minVal, sc_maxVal );
        }
        else  //  if( oorr == packiarr_OutOfRangeWrap )  //  TODO:
        {
        }

        if( packType == PackNative )
        {
            *(native_t *)&arr[ index ] = value;
        }
        else
        {
            unative_t procedValue;
            if( !is_signed )
            {
                procedValue = *(unative_t *)&value;
            }
            else
            {
                if( value < 0 )
                {
                    procedValue = value + sc_toPositive + 1;
                }
                else
                {
                    procedValue = value;
                    procedValue += sc_toPositiveU;
                }
            }

            if( packType == PackPower )
            {
                const uiw arrIndex = index / (8 / pack);
                const uiw offset = (index % (8 / pack)) * pack;
                arr[ arrIndex ] &= ~(sc_mask << offset);
                arr[ arrIndex ] |= procedValue << offset;
            }
            else  //  if( packType == PackOther )
            {
                const uiw arrIndex = pack * index / (nativeBits);
                const uiw offset = pack * index % (nativeBits);
                const uiw canWrite = nativeBits - offset;

                arr[ arrIndex ] &= ~(sc_mask << offset);
                arr[ arrIndex ] |= (procedValue << offset) & nativeMask;

                if( canWrite < pack )
                {
                    arr[ arrIndex + 1 ] &= ~(sc_mask >> canWrite);
                    arr[ arrIndex + 1 ] |= procedValue >> canWrite;
                }
            }
        }
    }

    native_t Get( const unative_t *arr, const uiw index ) const
    {
        if( packType == PackNative )
        {
            return *(native_t *)&arr[ index ];
        }
        else
        {
            unative_t value;
            if( packType == PackPower )
            {
                const uiw arrIndex = index / (8 / pack);
                const uiw offset = (index % (8 / pack)) * pack;
                value = (arr[ arrIndex ] >> offset) & sc_mask;
            }
            else  //  if( packType == PackOther )
            {
                const uiw arrIndex = pack * index / (nativeBits);
                const uiw offset = pack * index % (nativeBits);
                const uiw canRead = nativeBits - offset;
                if( canRead >= pack )  //  full read
                {
                    value = (arr[ arrIndex ] >> offset) & sc_mask;
                }
                else  //  partial read
                {
                    value = arr[ arrIndex ] >> offset;
                    value |= (arr[ arrIndex + 1 ] << canRead) & sc_mask;
                }
            }

            if( is_signed )
            {
                return value >= sc_toPositive ? (native_t)(value - sc_toPositiveU) : (native_t)value - sc_toPositive - 1;
            }
            return value;
        }
    }

    uiw PackGet() const
    {
        return pack;
    }

    packiarr_OutOfRangeReaction OutOfRangeReactionGet() const
    {
        return oorr;
    }

    bln IsRuntimeBoundsCheck() const
    {
        return is_runtimeBoundsCheck;
    }

    bln IsDebugOverflowCheck() const
    {
        return is_debugOverflowCheck;
    }

    uiw CellsGet() const;
};

template < const uiw cells, const uiw pack, bln is_signed, const packiarr_OutOfRangeReaction oorr = packiarr_OutOfRangeIgnore, const bln is_runtimeBoundsCheck = false, const bln is_debugOverflowCheck = false > class packiarr_static : public __packiarr_abstract__ < pack, is_signed, oorr, is_runtimeBoundsCheck, is_debugOverflowCheck >
{
    typedef __packiarr_abstract__ < pack, is_signed, oorr, is_runtimeBoundsCheck, is_debugOverflowCheck > parentType;
    typedef typename parentType::native_t native_t;
    typedef typename parentType::unative_t unative_t;
    using parentType::nativeBits;

    static const uiw arrSize = ((cells * pack + (nativeBits - 1)) & ~(nativeBits - 1)) / nativeBits;
    CStaticVec < unative_t, arrSize > _arr;

public:
    packiarr_static()
    {}

    packiarr_static( uiw cellsToFill, native_t value )
    {
        ASSUME( cellsToFill <= cells );
        for( uiw index = 0; index < cellsToFill; ++index )
        {
            parentType::Set( _arr.Data(), index, value );
        }
    }

    void Set( const uiw index, native_t value )
    {
        if( is_runtimeBoundsCheck && index >= cells )
        {
            DBGBREAK;
            return;
        }
        ASSUME( index < cells );

        parentType::Set( _arr.Data(), index, value );
    }

    native_t Get( const uiw index ) const
    {
        if( is_runtimeBoundsCheck && index >= cells )
        {
            DBGBREAK;
            return native_t();
        }
        ASSUME( index < cells );

        return parentType::Get( _arr.Data(), index );
    }

    uiw CellsGet() const
    {
        return cells;
    }
};

template < const uiw pack, bln is_signed, typename reservator = Reservator::Half <>, typename allocator = Allocator::Simple, const packiarr_OutOfRangeReaction oorr = packiarr_OutOfRangeIgnore, const bln is_runtimeBoundsCheck = false, const bln is_debugOverflowCheck = false > class packiarr_dynamic : public __packiarr_abstract__ < pack, is_signed, oorr, is_runtimeBoundsCheck, is_debugOverflowCheck >
{
    typedef __packiarr_abstract__ < pack, is_signed, oorr, is_runtimeBoundsCheck, is_debugOverflowCheck > parentType;
    typedef typename parentType::native_t native_t;
    typedef typename parentType::unative_t unative_t;
    using parentType::nativeBits;

    CVec < unative_t, reservator, Sem_POD, allocator > _arr;

    uiw _cells;

public:
    ~packiarr_dynamic()
    {}

    packiarr_dynamic() : _cells( 0 )
    {}

    explicit packiarr_dynamic( uiw cells ) : _arr( ComputeArrSize( cells ) ), _cells( cells )
    {}

    packiarr_dynamic( uiw cells, native_t value ) : _arr( ComputeArrSize( cells ) )
    {
        for( _cells = 0; _cells < cells; ++_cells )
        {
            parentType::Set( _arr.Data(), _cells, value );
        }
    }

    void Set( const uiw index, native_t value )
    {
        if( is_runtimeBoundsCheck && index >= _cells )
        {
            DBGBREAK;
            return;
        }
        ASSUME( index < _cells );

        parentType::Set( _arr.Data(), index, value );
    }

    native_t Get( const uiw index ) const
    {
        if( is_runtimeBoundsCheck && index >= _cells )
        {
            DBGBREAK;
            return native_t();
        }
        ASSUME( index < _cells );

        return parentType::Get( _arr.Data(), index );
    }

    uiw CellsGet() const
    {
        return _cells;
    }

    void Resize( uiw cells )
    {
        if( cells == _cells )
        {
            return;
        }
        _arr.Resize( ComputeArrSize( cells ) );
        _cells = cells;
    }

    void PushBack( native_t value )
    {
        _arr.Resize( ComputeArrSize( _cells + 1 ) );
        parentType::Set( _arr.Data(), _cells, value );
        ++_cells;
    }

    void PopBack()
    {
        ASSUME( _cells );
        --_cells;
    }

    void Add( uiw cells )
    {
        _cells += cells;
        _arr.Resize( ComputeArrSize( _cells ) );
    }

    void Add( uiw cells, native_t setValue )
    {
        uiw newCells = _cells + cells;
        _arr.Resize( ComputeArrSize( newCells ) );
        for( ; _cells < newCells; ++_cells )
        {
            Set( _cells, setValue );
        }
    }

private:
    uiw ComputeArrSize( uiw cells )
    {
        return ((cells * pack + (nativeBits - 1)) & ~(nativeBits - 1)) / nativeBits;
    }
};

}  //  namespace StdLib

#endif __PACKED_INT_ARRAY_HPP__
