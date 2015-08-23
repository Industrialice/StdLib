#include "PreHeader.hpp"
#include "LiceMathTypes.hpp"

//////////  M4x4  //////////

M4x4::M4x4()
{
}

M4x4::M4x4( f32 e00, f32 e01, f32 e02, f32 e03, f32 e10, f32 e11, f32 e12, f32 e13, f32 e20, f32 e21, f32 e22, f32 e23, f32 e30, f32 e31, f32 e32, f32 e33 )
{
    this->e00 = e00;
    this->e01 = e01;
    this->e02 = e02;
    this->e03 = e03;

    this->e10 = e10;
    this->e11 = e11;
    this->e12 = e12;
    this->e13 = e13;

    this->e20 = e20;
    this->e21 = e21;
    this->e22 = e22;
    this->e23 = e23;

    this->e30 = e30;
    this->e31 = e31;
    this->e32 = e32;
    this->e33 = e33;
}

M4x4::M4x4( const Vec4 &o_row0, const Vec4 &o_row1, const Vec4 &o_row2, const Vec4 &o_row3 )
{
    this->e00 = o_row0.x;
    this->e01 = o_row0.y;
    this->e02 = o_row0.z;
    this->e03 = o_row0.w;

    this->e10 = o_row1.x;
    this->e11 = o_row1.y;
    this->e12 = o_row1.z;
    this->e13 = o_row1.w;

    this->e20 = o_row2.x;
    this->e21 = o_row2.y;
    this->e22 = o_row2.z;
    this->e23 = o_row2.w;

    this->e30 = o_row3.x;
    this->e31 = o_row3.y;
    this->e32 = o_row3.z;
    this->e33 = o_row3.w;
}

M4x4::M4x4( const M4x3 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;
    this->e03 = 0;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;
    this->e13 = 0;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = o_m.e22;
    this->e23 = 0;

    this->e30 = o_m.e30;
    this->e31 = o_m.e31;
    this->e32 = o_m.e32;
    this->e33 = 1;
}

M4x4::M4x4( const M4x3 &o_m, f32 e03, f32 e13, f32 e23, f32 e33 )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;
    this->e03 = e03;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;
    this->e13 = e13;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = o_m.e22;
    this->e23 = e23;

    this->e30 = o_m.e30;
    this->e31 = o_m.e31;
    this->e32 = o_m.e32;
    this->e33 = e33;
}

M4x4::M4x4( const M4x3 &o_m, const Vec4 &o_lastCol )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;
    this->e03 = o_lastCol.x;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;
    this->e13 = o_lastCol.y;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = o_m.e22;
    this->e23 = o_lastCol.z;

    this->e30 = o_m.e30;
    this->e31 = o_m.e31;
    this->e32 = o_m.e32;
    this->e33 = o_lastCol.w;
}

M4x4::M4x4( const M3x4 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;
    this->e03 = o_m.e03;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;
    this->e13 = o_m.e13;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = o_m.e22;
    this->e23 = o_m.e23;
}

M4x4::M4x4( const M3x3 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;
    this->e03 = 0;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;
    this->e13 = 0;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = o_m.e22;
    this->e23 = 0;

    this->e30 = 0;
    this->e31 = 0;
    this->e32 = 0;
    this->e33 = 1;
}

M4x4::M4x4( const M3x2 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = 0;
    this->e03 = 0;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = 0;
    this->e13 = 0;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = 1;
    this->e23 = 0;

    this->e30 = 0;
    this->e31 = 0;
    this->e32 = 0;
    this->e33 = 1;
}

M4x4::M4x4( const M2x3 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;
    this->e03 = 0;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;
    this->e13 = 0;

    this->e20 = 0;
    this->e21 = 0;
    this->e22 = 1;
    this->e23 = 0;

    this->e30 = 0;
    this->e31 = 0;
    this->e32 = 0;
    this->e33 = 1;
}

M4x4::M4x4( const M2x2 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = 0;
    this->e03 = 0;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = 0;
    this->e13 = 0;

    this->e20 = 0;
    this->e21 = 0;
    this->e22 = 1;
    this->e23 = 0;

    this->e30 = 0;
    this->e31 = 0;
    this->e32 = 0;
    this->e33 = 1;
}

f32 *M4x4::operator []( uiw index )
{
    ASSUME( index < 4 );
    return this->m[ index ];
}

const f32 *M4x4::operator []( uiw index ) const
{
    ASSUME( index < 4 );
    return this->m[ index ];
}

//////////  M4x3  //////////

M4x3::M4x3()
{
}

M4x3::M4x3( f32 e00, f32 e01, f32 e02, f32 e10, f32 e11, f32 e12, f32 e20, f32 e21, f32 e22, f32 e30, f32 e31, f32 e32 )
{
    this->e00 = e00;
    this->e01 = e01;
    this->e02 = e02;

    this->e10 = e10;
    this->e11 = e11;
    this->e12 = e12;

    this->e20 = e20;
    this->e21 = e21;
    this->e22 = e22;

    this->e30 = e30;
    this->e31 = e31;
    this->e32 = e32;
}

M4x3::M4x3( const Vec3 &o_row0, const Vec3 &o_row1, const Vec3 &o_row2, const Vec3 &o_row3 )
{
    this->e00 = o_row0.x;
    this->e01 = o_row0.y;
    this->e02 = o_row0.z;

    this->e10 = o_row1.x;
    this->e11 = o_row1.y;
    this->e12 = o_row1.z;

    this->e20 = o_row2.x;
    this->e21 = o_row2.y;
    this->e22 = o_row2.z;

    this->e30 = o_row3.x;
    this->e31 = o_row3.y;
    this->e32 = o_row3.z;
}

M4x3::M4x3( const M4x4 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = o_m.e22;

    this->e30 = o_m.e30;
    this->e31 = o_m.e31;
    this->e32 = o_m.e32;
}

M4x3::M4x3( const M3x4 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = o_m.e22;

    this->e30 = 0.f;
    this->e31 = 0.f;
    this->e32 = 0.f;
}

M4x3::M4x3( const M3x3 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = o_m.e22;

    this->e30 = 0;
    this->e31 = 0;
    this->e32 = 0;
}

M4x3::M4x3( const M2x2 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = 0;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = 0;

    this->e20 = 0;
    this->e21 = 0;
    this->e22 = 1;

    this->e30 = 0;
    this->e31 = 0;
    this->e32 = 0;
}

f32 *M4x3::operator []( uiw index )
{
    ASSUME( index < 4 );
    return this->m[ index ];
}

const f32 *M4x3::operator []( uiw index ) const
{
    ASSUME( index < 4 );
    return this->m[ index ];
}

//////////  M3x4  //////////

M3x4::M3x4()
{
}

M3x4::M3x4( f32 e00, f32 e01, f32 e02, f32 e03, f32 e10, f32 e11, f32 e12, f32 e13, f32 e20, f32 e21, f32 e22, f32 e23 )
{
    this->e00 = e00;
    this->e01 = e01;
    this->e02 = e02;
    this->e03 = e03;

    this->e10 = e10;
    this->e11 = e11;
    this->e12 = e12;
    this->e13 = e13;

    this->e20 = e20;
    this->e21 = e21;
    this->e22 = e22;
    this->e23 = e23;
}

M3x4::M3x4( const Vec4 &o_row0, const Vec4 &o_row1, const Vec4 &o_row2 )
{
    this->e00 = o_row0.x;
    this->e01 = o_row0.y;
    this->e02 = o_row0.z;
    this->e03 = o_row0.w;

    this->e10 = o_row1.x;
    this->e11 = o_row1.y;
    this->e12 = o_row1.z;
    this->e13 = o_row1.w;

    this->e20 = o_row2.x;
    this->e21 = o_row2.y;
    this->e22 = o_row2.z;
    this->e23 = o_row2.w;
}

M3x4::M3x4( const M4x4 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;
    this->e03 = o_m.e03;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;
    this->e13 = o_m.e13;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = o_m.e22;
    this->e23 = o_m.e23;
}

M3x4::M3x4( const M4x3 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;
    this->e03 = 0;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;
    this->e13 = 0;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = o_m.e22;
    this->e23 = 0;
}

M3x4::M3x4( const M3x3 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;
    this->e03 = 0;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;
    this->e13 = 0;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = o_m.e22;
    this->e23 = 0;
}

M3x4::M3x4( const M2x2 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = 0;
    this->e03 = 0;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = 0;
    this->e13 = 0;

    this->e20 = 0;
    this->e21 = 0;
    this->e22 = 1;
    this->e23 = 0;
}

f32 *M3x4::operator []( uiw index )
{
    ASSUME( index < 3 );
    return this->m[ index ];
}

const f32 *M3x4::operator []( uiw index ) const
{
    ASSUME( index < 3 );
    return this->m[ index ];
}

//////////  M3x3  //////////

M3x3::M3x3()
{
}

M3x3::M3x3( f32 e00, f32 e01, f32 e02, f32 e10, f32 e11, f32 e12, f32 e20, f32 e21, f32 e22 )
{
    this->e00 = e00;
    this->e01 = e01;
    this->e02 = e02;

    this->e10 = e10;
    this->e11 = e11;
    this->e12 = e12;

    this->e20 = e20;
    this->e21 = e21;
    this->e22 = e22;
}

M3x3::M3x3( const Vec3 &o_row0, const Vec3 &o_row1, const Vec3 &o_row2 )
{
    this->e00 = o_row0.x;
    this->e01 = o_row0.y;
    this->e02 = o_row0.z;

    this->e10 = o_row1.x;
    this->e11 = o_row1.y;
    this->e12 = o_row1.z;

    this->e20 = o_row2.x;
    this->e21 = o_row2.y;
    this->e22 = o_row2.z;
}

M3x3::M3x3( const M4x4 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = o_m.e22;
}

M3x3::M3x3( const M4x3 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = o_m.e22;
}

M3x3::M3x3( const M3x4 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = o_m.e02;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = o_m.e12;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
    this->e22 = o_m.e22;
}

M3x3::M3x3( const M2x2 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;
    this->e02 = 0;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
    this->e12 = 0;

    this->e20 = 0;
    this->e21 = 0;
    this->e22 = 1;
}

f32 *M3x3::operator []( uiw index )
{
    ASSUME( index < 3 );
    return this->m[ index ];
}

const f32 *M3x3::operator []( uiw index ) const
{
    ASSUME( index < 3 );
    return this->m[ index ];
}

//////////  M3x2  //////////

M3x2::M3x2()
{
}

M3x2::M3x2( f32 e00, f32 e01, f32 e10, f32 e11, f32 e20, f32 e21 )
{
    this->e00 = e00;
    this->e01 = e01;

    this->e10 = e10;
    this->e11 = e11;

    this->e20 = e20;
    this->e21 = e21;
}

M3x2::M3x2( const Vec2 &o_row0, const Vec2 &o_row1, const Vec2 &o_row2 )
{
    this->e00 = o_row0.x;
    this->e01 = o_row0.y;

    this->e10 = o_row1.x;
    this->e11 = o_row1.y;

    this->e20 = o_row2.x;
    this->e21 = o_row2.y;
}

M3x2::M3x2( const M4x4 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;

    this->e20 = o_m.e20;
    this->e21 = o_m.e21;
}

f32 *M3x2::operator []( uiw index )
{
    ASSUME( index < 3 );
    return this->m[ index ];
}

const f32 *M3x2::operator []( uiw index ) const
{
    ASSUME( index < 3 );
    return this->m[ index ];
}

//////////  M2x2  //////////

M2x2::M2x2()
{
}

M2x2::M2x2( f32 e00, f32 e01, f32 e10, f32 e11 )
{
    this->e00 = e00;
    this->e01 = e01;

    this->e10 = e10;
    this->e11 = e11;
}

M2x2::M2x2( const Vec2 &o_row0, const Vec2 &o_row1 )
{
    this->e00 = o_row0.x;
    this->e01 = o_row0.y;

    this->e10 = o_row1.x;
    this->e11 = o_row1.y;
}

M2x2::M2x2( const M4x4 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
}

M2x2::M2x2( const M4x3 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
}

M2x2::M2x2( const M3x4 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
}

M2x2::M2x2( const M3x3 &o_m )
{
    this->e00 = o_m.e00;
    this->e01 = o_m.e01;

    this->e10 = o_m.e10;
    this->e11 = o_m.e11;
}

f32 *M2x2::operator []( uiw index )
{
    ASSUME( index < 2 );
    return this->m[ index ];
}

const f32 *M2x2::operator []( uiw index ) const
{
    ASSUME( index < 2 );
    return this->m[ index ];
}

//////////  Vec2  //////////

Vec2::Vec2()
{
}

Vec2::Vec2( f32 initAll )
{
    this->x = this->y = initAll;
}

Vec2::Vec2( f32 p0, f32 p1 )
{
    this->arr[ 0 ] = p0;
    this->arr[ 1 ] = p1;
}

f32 &Vec2::operator []( uiw index )
{
    ASSUME( index < 2 );
    return this->arr[ index ];
}

const f32 &Vec2::operator []( uiw index ) const
{
    ASSUME( index < 2 );
    return this->arr[ index ];
}

//////////  Vec3  //////////

Vec3::Vec3()
{
}

Vec3::Vec3( f32 initAll )
{
    this->x = this->y = this->z = initAll;
}

Vec3::Vec3( const Vec2 &o_v, f32 scalar )
{
    this->x = o_v.x;
    this->y = o_v.y;
    this->z = scalar;
}

Vec3::Vec3( f32 scalar, const Vec2 &o_v )
{
    this->x = scalar;
    this->y = o_v.x;
    this->z = o_v.y;
}

Vec3::Vec3( f32 p0, f32 p1, f32 p2 )
{
    this->arr[ 0 ] = p0;
    this->arr[ 1 ] = p1;
    this->arr[ 2 ] = p2;
}

Vec3::Vec3( const F96Color &o_col )
{
    this->r = o_col.r;
    this->g = o_col.g;
    this->b = o_col.b;
}

Vec3::Vec3( const F128Color &o_col )
{
    this->r = o_col.r;
    this->g = o_col.g;
    this->b = o_col.b;
}

f32 &Vec3::operator []( uiw index )
{
    ASSUME( index < 3 );
    return this->arr[ index ];
}

const f32 &Vec3::operator []( uiw index ) const
{
    ASSUME( index < 3 );
    return this->arr[ index ];
}

//////////  Vec4  //////////

Vec4::Vec4()
{
}

Vec4::Vec4( f32 initAll )
{
    this->x = this->y = this->z = this->w = initAll;
}

Vec4::Vec4( const vec2 &o_v0, const vec2 &o_v1 )
{
    this->x = o_v0.x;
    this->y = o_v0.y;
    this->z = o_v1.x;
    this->w = o_v1.y;
}

Vec4::Vec4( const vec3 &o_v, f32 scalar )
{
    this->x = o_v.x;
    this->y = o_v.y;
    this->z = o_v.z;
    this->w = scalar;
}

Vec4::Vec4( const F96Color &o_col, f32 alpha )
{
    this->r = o_col.r;
    this->g = o_col.g;
    this->b = o_col.b;
    this->a = alpha;
}

Vec4::Vec4( const F128Color &o_col )
{
    this->r = o_col.r;
    this->g = o_col.g;
    this->b = o_col.b;
    this->a = o_col.a;
}

Vec4::Vec4( f32 scalar, const vec3 &o_v )
{
    this->x = scalar;
    this->y = o_v.x;
    this->z = o_v.y;
    this->w = o_v.z;
}

Vec4::Vec4( f32 p0, f32 p1, f32 p2, f32 p3 )
{
    this->arr[ 0 ] = p0;
    this->arr[ 1 ] = p1;
    this->arr[ 2 ] = p2;
    this->arr[ 3 ] = p3;
}

f32 &Vec4::operator []( uiw index )
{
    ASSUME( index < 4 );
    return this->arr[ index ];
}

const f32 &Vec4::operator []( uiw index ) const
{
    ASSUME( index < 4 );
    return this->arr[ index ];
}

//////////  I32Color  //////////

I32Color::I32Color()
{
}

I32Color::I32Color( ui8 r, ui8 g, ui8 b, ui8 a /* = 255 */ )
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

I32Color::I32Color( const F96Color &o_col )
{
    this->r = (ui8)(o_col.r * 255.f);
    this->g = (ui8)(o_col.g * 255.f);
    this->b = (ui8)(o_col.b * 255.f);
}

I32Color::I32Color( const F128Color &o_col )
{
    this->r = (ui8)(o_col.r * 255.f);
    this->g = (ui8)(o_col.g * 255.f);
    this->b = (ui8)(o_col.b * 255.f);
    this->a = (ui8)(o_col.a * 255.f);
}

ui8 &I32Color::operator []( uiw index )
{
    ASSUME( index < 4 );
    return this->arr[ index ];
}

const ui8 &I32Color::operator []( uiw index ) const
{
    ASSUME( index < 4 );
    return this->arr[ index ];
}

//////////  F96Color  //////////

F96Color::F96Color()
{
}

F96Color::F96Color( f32 r, f32 g, f32 b )
{
    this->r = r;
    this->g = g;
    this->b = b;
}

F96Color::F96Color( const F128Color &o_col )
{
    this->r = o_col.r;
    this->g = o_col.g;
    this->b = o_col.b;
}

F96Color::F96Color( const vec4 &o_vec )
{
    this->r = o_vec.r;
    this->g = o_vec.g;
    this->b = o_vec.b;
}

F96Color::F96Color( const vec3 &o_vec )
{
    this->r = o_vec.r;
    this->g = o_vec.g;
    this->b = o_vec.b;
}

F96Color::F96Color( const I32Color &o_col )
{
    this->r = o_col.r / 255.f;
    this->g = o_col.g / 255.f;
    this->b = o_col.b / 255.f;
}

f32 &F96Color::operator []( uiw index )
{
    ASSUME( index < 3 );
    return this->arr[ index ];
}

const f32 &F96Color::operator []( uiw index ) const
{
    ASSUME( index < 3 );
    return this->arr[ index ];
}

void F96Color::Saturate()
{
    this->r = Funcs::SaturateF32( this->r );
    this->g = Funcs::SaturateF32( this->g );
    this->b = Funcs::SaturateF32( this->b );
}

//////////  F128Color  //////////

F128Color::F128Color()
{
}

F128Color::F128Color( f32 r, f32 g, f32 b, f32 a /* = 1.f */ )
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

F128Color::F128Color( const F96Color &o_col, f32 alpha /* = 1.f */ )
{
    this->r = o_col.r;
    this->g = o_col.g;
    this->b = o_col.b;
    this->a = alpha;
}

F128Color::F128Color( const vec4 &o_vec )
{
    this->r = o_vec.r;
    this->g = o_vec.g;
    this->b = o_vec.b;
    this->a = o_vec.a;
}

F128Color::F128Color( const vec3 &o_vec, f32 alpha /* = 1.f */ )
{
    this->r = o_vec.r;
    this->g = o_vec.g;
    this->b = o_vec.b;
    this->a = alpha;
}

F128Color::F128Color( const I32Color &o_col )
{
    this->r = o_col.r / 255.f;
    this->g = o_col.g / 255.f;
    this->b = o_col.b / 255.f;
    this->a = o_col.a / 255.f;
}

f32 &F128Color::operator []( uiw index )
{
    ASSUME( index < 4 );
    return this->arr[ index ];
}

const f32 &F128Color::operator []( uiw index ) const
{
    ASSUME( index < 4 );
    return this->arr[ index ];
}

void F128Color::Saturate()
{
    this->r = Funcs::SaturateF32( this->r );
    this->g = Funcs::SaturateF32( this->g );
    this->b = Funcs::SaturateF32( this->b );
    this->a = Funcs::SaturateF32( this->a );
}

F96Color F128Color::AlphaPremultedGet() const
{
    return F96Color( this->r * this->a, this->g * this->a, this->b * this->a );
}