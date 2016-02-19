#ifndef __LICE_MATH_TYPES_HPP__
#define __LICE_MATH_TYPES_HPP__

namespace StdLib {

struct M4x4;
struct M4x3;
struct M3x4;
struct M3x3;
struct M3x2;
struct M2x3;
struct M2x2;

struct Vec2;
struct Vec3;
struct Vec4;

struct I32Color;
struct F96Color;
struct F128Color;

typedef struct Plane : CharPOD
{
	f32 a, b, c, d;
} plane;

typedef ui32 colori32web;

#define COLORI32WEB_ARGB( a, r, g, b ) ((a) << 24 | (r) << 16 | (g) << 8 | (b))
#define COLORI32WEB_RGB( r, g, b ) (0xFF000000 | (r) << 16 | (g) << 8 | (b))
#define COLORI32WEB_GETA( color ) ((color) >> 24)
#define COLORI32WEB_GETR( color ) ((color) >> 16 & 0xFF)
#define COLORI32WEB_GETG( color ) ((color) >> 8 & 0xFF)
#define COLORI32WEB_GETB( color ) ((color) & 0xFF)

const f32 FULLTURN = f32_pi * 2.f;

#define DEGREETORADIAN( deg ) ((deg) * (f32_pi / 180.f))
#define RADIANTODEGREE( rad ) ((rad) * (180.f / f32_pi))

typedef struct M4x4 : CharPOD
{
    union
    {
        f32 m[ 4 ][ 4 ];

        f32 arr[ 4 * 4 ];

        struct
        {
            f32 e00, e01, e02, e03,
                e10, e11, e12, e13,
                e20, e21, e22, e23,
                e30, e31, e32, e33;
        };
    };

    M4x4();  //  do nothing
    M4x4( f32 e00, f32 e01, f32 e02, f32 e03, f32 e10, f32 e11, f32 e12, f32 e13, f32 e20, f32 e21, f32 e22, f32 e23, f32 e30, f32 e31, f32 e32, f32 e33 );
    M4x4( const Vec4 &o_row0, const Vec4 &o_row1, const Vec4 &o_row2, const Vec4 &o_row3 );
    explicit M4x4( const M3x3 &o_m );
    explicit M4x4( const M4x3 &o_m );
    M4x4( const M4x3 &o_m, f32 e03, f32 e13, f32 e23, f32 e33 );
    M4x4( const M4x3 &o_m, const Vec4 &o_lastCol );
    explicit M4x4( const M3x4 &o_m );
 //   explicit M4x4( const M3x3 &o_m );
    explicit M4x4( const M3x2 &o_m );
    explicit M4x4( const M2x3 &o_m );
    explicit M4x4( const M2x2 &o_m );
    f32 *operator []( uiw index );
    const f32 *operator []( uiw index ) const;
} m4x4;

typedef struct M4x3 : CharPOD
{
    union
    {
        f32 m[ 4 ][ 3 ];

        f32 arr[ 4 * 3 ];

        struct
        {
            f32 e00, e01, e02,
                e10, e11, e12,
                e20, e21, e22,
                e30, e31, e32;
        };
    };

    M4x3();  //  do nothing
    M4x3( f32 e00, f32 e01, f32 e02, f32 e10, f32 e11, f32 e12, f32 e20, f32 e21, f32 e22, f32 e30, f32 e31, f32 e32 );
    M4x3( const Vec3 &o_row0, const Vec3 &o_row1, const Vec3 &o_row2, const Vec3 &o_row3 );
    explicit M4x3( const M4x4 &o_m );
    explicit M4x3( const M3x4 &o_m );
    explicit M4x3( const M3x3 &o_m );
    //explicit M4x3( const M3x2 &o_m );
    //explicit M4x3( const M2x3 &o_m );
    explicit M4x3( const M2x2 &o_m );
    f32 *operator []( uiw index );
    const f32 *operator []( uiw index ) const;
} m4x3;

typedef struct M3x4 : CharPOD
{
    union
    {
        f32 m[ 3 ][ 4 ];

        f32 arr[ 3 * 4 ];

        struct
        {
            f32 e00, e01, e02, e03,
                e10, e11, e12, e13,
                e20, e21, e22, e23;
        };
    };

    M3x4();  //  do nothing
    M3x4( f32 e00, f32 e01, f32 e02, f32 e03, f32 e10, f32 e11, f32 e12, f32 e13, f32 e20, f32 e21, f32 e22, f32 e23 );
    M3x4( const Vec4 &o_row0, const Vec4 &o_row1, const Vec4 &o_row2 );
    explicit M3x4( const M4x4 &o_m );
    explicit M3x4( const M4x3 &o_m );
    explicit M3x4( const M3x3 &o_m );
    //explicit M3x4( const M3x2 &o_m );
    //explicit M3x4( const M2x3 &o_m );
    explicit M3x4( const M2x2 &o_m );
    f32 *operator []( uiw index );
    const f32 *operator []( uiw index ) const;
} m3x4;

typedef struct M3x3 : CharPOD
{
    union
    {
        f32 m[ 3 ][ 3 ];

        f32 arr[ 3 * 3 ];

        struct
        {
            f32 e00, e01, e02,
                e10, e11, e12,
                e20, e21, e22;
        };
    };

    M3x3();  //  do nothing
    M3x3( f32 e00, f32 e01, f32 e02, f32 e10, f32 e11, f32 e12, f32 e20, f32 e21, f32 e22 );
    M3x3( const Vec3 &o_row0, const Vec3 &o_row1, const Vec3 &o_row2 );
    explicit M3x3( const M4x4 &o_m );
    explicit M3x3( const M4x3 &o_m );
    explicit M3x3( const M3x4 &o_m );
    //explicit M3x3( const M3x2 &o_m );
    //explicit M3x3( const M2x3 &o_m );
    explicit M3x3( const M2x2 &o_m );
    f32 *operator []( uiw index );
    const f32 *operator []( uiw index ) const;
} m3x3;

typedef struct M3x2 : CharPOD
{
    union
    {
        f32 m[ 3 ][ 2 ];

        f32 arr[ 3 * 2 ];

        struct
        {
            f32 e00, e01,
                e10, e11,
                e20, e21;
        };
    };

    M3x2();  //  do nothing
    M3x2( f32 e00, f32 e01, f32 e10, f32 e11, f32 e20, f32 e21 );
    M3x2( const Vec2 &o_row0, const Vec2 &o_row1, const Vec2 &o_row2 );
    explicit M3x2( const M4x4 &o_m );
 //   explicit M3x2( const M4x3 &o_m );
 //   explicit M3x2( const M3x4 &o_m );
    //explicit M3x2( const M3x3 &o_m );
    //explicit M3x2( const M2x3 &o_m );
 //   explicit M3x2( const M2x2 &o_m );
    f32 *operator []( uiw index );
    const f32 *operator []( uiw index ) const;
} m3x2;

typedef struct M2x3 : CharPOD
{
    union
    {
        f32 m[ 2 ][ 3 ];

        f32 arr[ 2 * 3 ];

        struct
        {
            f32 e00, e01, e02,
                e10, e11, e12;
        };
    };

 //   M2x3();  //  do nothing
 //   M2x3( f32 e00, f32 e01, f32 e02, f32 e10, f32 e11, f32 e12 );
 //   M2x3( const Vec3 &o_row0, const Vec3 &o_row1, const Vec3 &o_row2 );
 //   explicit M2x3( const M4x4 &o_m );
 //   explicit M2x3( const M4x3 &o_m );
 //   explicit M2x3( const M3x4 &o_m );
    //explicit M2x3( const M3x3 &o_m );
    //explicit M2x3( const M3x2 &o_m );
 //   explicit M2x3( const M2x2 &o_m );
 //   f32 *operator []( uiw index );
 //   const f32 *operator []( uiw index ) const;
} m2x3;

typedef struct M2x2 : CharPOD
{
    union
    {
        f32 m[ 2 ][ 2 ];

        f32 arr[ 2 * 2 ];

        struct
        {
            f32 e00, e01,
                e10, e11;
        };
    };

    M2x2();  //  do nothing
    M2x2( f32 e00, f32 e01, f32 e10, f32 e11 );
    M2x2( const Vec2 &o_row0, const Vec2 &o_row1 );
    explicit M2x2( const M4x4 &o_m );
    explicit M2x2( const M4x3 &o_m );
    explicit M2x2( const M3x4 &o_m );
    explicit M2x2( const M3x3 &o_m );
    //explicit M2x2( const M3x2 &o_m );
    //explicit M2x2( const M2x3 &o_m );
    f32 *operator []( uiw index );
    const f32 *operator []( uiw index ) const;
} m2x2;

typedef struct Vec2 : CharPOD
{
    union
    {
        f32 arr[ 2 ];

        f32 xy[ 2 ];

        struct
        {
            f32 x, y;
        };
        struct
        {
            f32 r, g;
        };
        struct
        {
            f32 u, v;
        };
    };

    Vec2();  //  do nothing
    explicit Vec2( f32 initAll );
    Vec2( f32 p0, f32 p1 );
    f32 &operator []( uiw index );
    const f32 &operator []( uiw index ) const;
} vec2;

typedef struct Vec3 : CharPOD
{
    union
    {
        f32 arr[ 3 ];

        f32 xyz[ 3 ];

        f32 xy[ 2 ];

        struct
        {
            f32 x, y, z;
        };
        struct
        {
            f32 r, g, b;
        };
    };

    Vec3();  //  do nothing
    explicit Vec3( f32 initAll );
    Vec3( const Vec2 &o_v, f32 scalar );
    Vec3( f32 scalar, const Vec2 &o_v );
    Vec3( f32 p0, f32 p1, f32 p2 );
    Vec3( const F96Color &o_col );
    Vec3( const F128Color &o_col );
    f32 &operator []( uiw index );
    const f32 &operator []( uiw index ) const;
} vec3;

typedef struct Vec4 : CharPOD
{
    union
    {
        f32 arr[ 4 ];

        f32 xyzw[ 4 ];

        f32 xyz[ 3 ];

        f32 xy[ 2 ];

        struct
        {
            f32 x, y, z, w;
        };
        struct
        {
            f32 r, g, b, a;
        };
    };

    Vec4();  //  do nothing
    explicit Vec4( f32 initAll );
    Vec4( const Vec2 &o_v0, const Vec2 &o_v1 );
    Vec4( const Vec3 &o_v, f32 scalar );
    Vec4( const F96Color &o_col, f32 alpha );
    Vec4( const F128Color &o_col );
    Vec4( f32 scalar, const Vec3 &o_v );
    Vec4( f32 p0, f32 p1, f32 p2, f32 p3 );
    f32 &operator []( uiw index );
    const f32 &operator []( uiw index ) const;
} vec4;

typedef struct I32Color : CharPOD
{
    union
    {
        struct
        {
            ui8 r, g, b, a;
        };
        ui32 rgba;
        ui8 arr[ 4 ];
    };

    I32Color();  //  do nothing
    I32Color( ui8 r, ui8 g, ui8 b, ui8 a = 255 );
    I32Color( const F96Color &o_col );  //  alpha will not be touched at all
    I32Color( const F128Color &o_col );
    ui8 &operator []( uiw index );
    const ui8 &operator []( uiw index ) const;
} i32color;

typedef struct F96Color : CharPOD
{
    union
    {
        struct
        {
            f32 r, g, b;
        };
        f32 arr[ 3 ];
    };

    F96Color();  //  do nothing
    F96Color( f32 r, f32 g, f32 b );
    F96Color( const F128Color &o_col );
    F96Color( const Vec4 &o_vec );
    F96Color( const Vec3 &o_vec );
    F96Color( const I32Color &o_col );
    f32 &operator []( uiw index );
    const f32 &operator []( uiw index ) const;
    void Saturate();
} f96color;

typedef struct F128Color : CharPOD
{
    union
    {
        struct
        {
            f32 r, g, b, a;
        };
        f32 arr[ 4 ];
    };

    F128Color();  //  do nothing
    F128Color( f32 r, f32 g, f32 b, f32 a = 1.f );
    F128Color( const F96Color &o_col, f32 alpha = 1.f );
    F128Color( const Vec4 &o_vec );
    F128Color( const Vec3 &o_vec, f32 alpha = 1.f );
    F128Color( const I32Color &o_col );
    f32 &operator []( uiw index );
    const f32 &operator []( uiw index ) const;
    void Saturate();
    F96Color AlphaPremultedGet() const;
} f128color;

template < ui32 rbits, ui32 gbits, ui32 bbits, ui32 abits = 0 > struct TColorRGBA : CharPOD
{
    enum { bitsSum = rbits + gbits + bbits + abits };
    enum { roundedBitsSum = (bitsSum > 32) ? (64) : ((bitsSum > 16) ? (32) : ((bitsSum > 8) ? 16 : 8)) };

    typedef typename IntWithSize < roundedBitsSum >::uint_t color_t;

    color_t color;

    TColorRGBA()
    {
    }

    TColorRGBA( ui8 r8, ui8 g8, ui8 b8 )
    {
        Construct( r8, g8, b8, 0 );
    }

    TColorRGBA( ui8 r8, ui8 g8, ui8 b8, ui8 a8 )
    {
        Construct( r8, g8, b8, a8 );
    }

    TColorRGBA( ui16 colorInR5G6B5 )
    {
        if( rbits == 5 && gbits == 6 && bbits == 5 && abits == 0 )
        {
            color = colorInR5G6B5;
        }
        else
        {
            NOT_IMPLEMENTED;
        }
    }

    TColorRGBA( i32color colorInA8R8B8G8 )
    {
        if( rbits == 8 && gbits == 8 && bbits == 8 && abits == 0 )
        {
            color = colorInA8R8B8G8.rgba & 0xFFffFF;
        }
        else if( rbits == 8 && gbits == 8 && bbits == 8 && abits == 8 )
        {
            color = colorInA8R8B8G8.rgba << 8;
            color |= colorInA8R8B8G8.rgba >> 24;
        }
        else
        {
            Construct( COLORI32WEB_GETR( colorInA8R8B8G8.rgba ), COLORI32WEB_GETG( colorInA8R8B8G8.rgba ), COLORI32WEB_GETB( colorInA8R8B8G8.rgba ), COLORI32WEB_GETA( colorInA8R8B8G8.rgba ) );
        }
    }

    TColorRGBA( const F96Color &o_col )
    {
        NOT_IMPLEMENTED;
    }

    TColorRGBA( const F128Color &o_col )
    {
        NOT_IMPLEMENTED;
    }

    ui8 R( ui8 bits = rbits ) const
    {
        if( bits == rbits )
        {
            return color >> (bitsSum - rbits);
        }
        NOT_IMPLEMENTED;
    }

    ui8 G( ui8 bits = gbits ) const
    {
        if( bits == gbits )
        {
            return color >> (bbits + abits) & Funcs::MaxValue < ui8 >( gbits );
        }
        NOT_IMPLEMENTED;
    }

    ui8 B( ui8 bits = bbits ) const
    {
        if( bits == bbits )
        {
            return color >> abits & Funcs::MaxValue < ui8 >( bbits );
        }
        NOT_IMPLEMENTED;
    }

    ui8 A( ui8 bits = abits ) const
    {
        if( bits == abits )
        {
            return color & Funcs::MaxValue < ui8 >( abits );
        }
        NOT_IMPLEMENTED;
    }

private:
    void Construct( ui8 r8, ui8 g8, ui8 b8, ui8 a8 )
    {
        color = 0;
        if( rbits )
        {
            r8 /= 255.f / Funcs::MaxValue < ui8 >( rbits );
            color |= r8 << (gbits + bbits + abits);
        }
        if( gbits )
        {
            g8 /= 255.f / Funcs::MaxValue < ui8 >( gbits );
            color |= g8 << (bbits + abits);
        }
        if( bbits )
        {
            b8 /= 255.f / Funcs::MaxValue < ui8 >( bbits );
            color |= b8 << abits;
        }
        if( abits )
        {
            a8 /= 255.f / Funcs::MaxValue < ui8 >( abits );
            color |= a8;
        }
    }
};

const m4x4 co_Iden4x4( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 );
const m4x3 co_Iden4x3( 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 );
const m3x4 co_Iden3x4( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 );
const m3x3 co_Iden3x3( 1, 0, 0, 0, 1, 0, 0, 0, 1 );
const m2x2 co_Iden2x2( 1, 0, 0, 1 );

}  //  namespace StdLib

#endif __LICE_MATH_TYPES_HPP__
