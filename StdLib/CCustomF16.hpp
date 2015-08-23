#ifndef __C_CUSTOM_F16_HPP__
#define __C_CUSTOM_F16_HPP__

namespace StdLib {

//  passing NaN and +/-inf is undefined behavior
template < const ui32 c_rangeNumerator, const ui32 c_rangeDenominator, const bln cis_unsigned = false > class cf16
{
    static const ui32 c_significandSize = cis_unsigned ? 16 : 15;

    ui16 _val;

public:
    ~cf16()
    {
        /*  void  */ 
    }

    cf16()
    {
        STATIC_CHECK( c_rangeNumerator <= i32_max, "numerator for range overflow" );
        STATIC_CHECK( c_rangeDenominator <= i32_max, "denominator for range overflow" );
        STATIC_CHECK( c_rangeNumerator != 0, "numerator for range is zero" );
        STATIC_CHECK( c_rangeDenominator != 0, "denominator for range is zero" );
    }

    NOINLINE void FromF32Wrap( f32 source )
    {
        const f32 range = Funcs::MakeF32ByND( c_rangeNumerator, c_rangeDenominator );
        const f32 rangeInv = Funcs::MakeF32ByND( c_rangeDenominator, c_rangeNumerator );
        if( cis_unsigned )
        {
            _val = 0;
        }
        else
        {
            ui32 isource = *(ui32 *)&source;
            _val = (isource & f32_sign_mask) >> 16;
        }
        source = ::fabsf( source );
        union
        {
            f32 f;
            ui32 i;
        } u = { Funcs::F32FracPart( source * rangeInv ) + 1.f };
        ui32 s;
        if( u.f >= 2.f )
        {
            s = f32_significand_mask;
        }
        else
        {
            ASSUME( u.f >= 1.f );
            s = u.i & f32_significand_mask;
        }
        _val |= s >> (23 - c_significandSize);
    }

    NOINLINE void FromF32Clamp( f32 source )
    {
        const f32 range = Funcs::MakeF32ByND( c_rangeNumerator, c_rangeDenominator );
        const f32 rangeInv = Funcs::MakeF32ByND( c_rangeDenominator, c_rangeNumerator );

        if( cis_unsigned )
        {
            _val = 0;
        }
        else
        {
            ui32 isource = *(ui32 *)&source;
            _val = (isource & f32_sign_mask) >> 16;
            source = ::fabsf( source );
        }
        union
        {
            f32 f;
            ui32 i;
        } u = { Funcs::Clamp( source, 0.f, range ) };
        u.f = u.f * rangeInv + 1.f;
        ui32 s;
        if( u.f >= 2.f ) 
        {
            s = f32_significand_mask;
        }
        else
        {
            ASSUME( u.f >= 1.f );
            s = u.i & f32_significand_mask;
        }
        _val |= s >> (23 - c_significandSize);
    }

    NOINLINE void FromF64Wrap( f64 source )
    {
        const f64 range = Funcs::MakeF64ByND( c_rangeNumerator, c_rangeDenominator );
        const f64 rangeInv = Funcs::MakeF64ByND( c_rangeDenominator, c_rangeNumerator );

        if( cis_unsigned )
        {
            _val = 0;
        }
        else
        {
            ui64 isource = *(ui64 *)&source;
            _val = (isource & f64_sign_mask) >> 48;
        }
        source = ::fabs( source );
        union
        {
            f64 f;
            ui64 i;
        } u = { Funcs::F64FracPart( source * rangeInv ) + 1.0 };
        ui64 s;
        if( u.f >= 2.0 )
        {
            s = f64_significand_mask;
        }
        else
        {
            ASSUME( u.f >= 1.0 );
            s = u.i & f64_significand_mask;
        }
        _val |= s >> (52 - c_significandSize);
    }

    NOINLINE void FromF64Clamp( f64 source )
    {
        const f64 range = Funcs::MakeF64ByND( c_rangeNumerator, c_rangeDenominator );
        const f64 rangeInv = Funcs::MakeF64ByND( c_rangeDenominator, c_rangeNumerator );

        if( cis_unsigned )
        {
            _val = 0;
        }
        else
        {
            ui64 isource = *(ui64 *)&source;
            _val = (isource & f64_sign_mask) >> 48;
            source = ::fabs( source );
        }
        union
        {
            f64 f;
            ui64 i;
        } u = { Funcs::Clamp( source, 0.0, range ) };
        u.f = u.f * rangeInv + 1.0;
        ui64 s;
        if( u.f >= 2.0 ) 
        {
            s = f64_significand_mask;
        }
        else
        {
            ASSUME( u.f >= 1.0 );
            s = u.i & f64_significand_mask;
        }
        _val |= s >> (52 - c_significandSize);
    }

    f32 ToF32()
    {
        const f32 range = Funcs::MakeF32ByND( c_rangeNumerator, c_rangeDenominator );

        ui32 s, f;
        if( cis_unsigned )
        {
            s = 0;
            f = (ui32)_val <<  (23 - c_significandSize);
        }
        else
        {
            s = _val >> 15;
            f = ((ui32)_val & 0x7FFF) << (23 - c_significandSize);
        }
        static const f32 sca_normalizators[ 2 ] = { -1.f, 1.f };
        return (Funcs::MakeF32ByBits( s, 127, f ) + sca_normalizators[ s ]) * range;
    }

    f64 ToF64()
    {
        const f64 range = Funcs::MakeF64ByND( c_rangeNumerator, c_rangeDenominator );

        ui64 s, f;
        if( cis_unsigned )
        {
            s = 0;
            f = (ui64)_val << (52 - c_significandSize);
        }
        else
        {
            s = _val >> 15;
            f = ((ui64)_val & 0x7FFF) << (52 - c_significandSize);
        }
        static const f64 sca_normalizators[ 2 ] = { -1.0, 1.0 };
        return (Funcs::MakeF64ByBits( s, 1023, f ) + sca_normalizators[ s ]) * range;
    }
};

}  //  namespace StdLib

#endif __C_CUSTOM_F16_HPP__