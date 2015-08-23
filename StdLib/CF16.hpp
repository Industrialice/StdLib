#ifndef __C_F16_HPP__
#define __C_F16_HPP__

namespace StdLib {

//  TODO: it's does not following IEEE 754-2008 and probably i should fix this
//  +/-inf and NaN conversion to integer is undefined behavior
class f16
{
    ui16 _val;

public:
    const ui16 *GetRawPointer();
    ui16 RawValueGet() const;
    void RawValueSet( ui16 source );

    f32 ToF32();
    f64 ToF64();
    i32 ToI32();

    void FromF32IEEE( f32 source );
    void FromF32Clamp( f32 source );
    void FromF64IEEE( f64 source );
    void FromF64Clamp( f64 source );
    void FromUI32Clamp( ui32 source );
    void FromI32Clamp( i32 source );

    bln IsNaN();
    bln IsInf();
    bln IsInfPos();
    bln IsInfNeg();

private:
    enum EnumDecl {};

    void FromF32( f32 source, EnumDecl flowAction );
    void FromF64( f64 source, EnumDecl flowAction );
};

const ui16 f16_sign_mask = 0x8000;
const ui16 f16_exp_mask = 0x7C00;
const ui16 f16_frac_mask = 0x03FF;
const ui16 f16_nan = 0xFFFF;  //  one of the possible NaN values
const ui16 f16_inf = 0x7C00;
const ui16 f16_inf_pos = 0x7C00;
const ui16 f16_inf_neg = 0xFC00;
const ui16 f16_zero = 0x0000;
const ui16 f16_zero_pos = 0x0000;
const ui16 f16_zero_neg = 0x8000;
const ui16 f16_zero_test = 0x7FFF;
const ui16 f16_max = 0x7BFF;
const ui16 f16_min = 0xFBFF;
const i32 f16_int_max = 65504;
const i32 f16_int_min = -65504;

}  //  namespace StdLib

#endif __C_F16_HPP__