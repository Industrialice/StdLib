#include "PreHeader.hpp"
#include "LiceMathFuncs.hpp"

f128color LiceMath::ColorI32WebToF128( colori32web color )
{
    return f128color( COLORI32WEB_GETR( color ) / 255.f, COLORI32WEB_GETG( color ) / 255.f, COLORI32WEB_GETB( color ) / 255.f, COLORI32WEB_GETA( color ) / 255.f );
}

void LiceMath::ColorI32WebToF128Placed( colori32web color, f128color *po_out )
{
    po_out->r = COLORI32WEB_GETR( color ) / 255.f;
    po_out->g = COLORI32WEB_GETG( color ) / 255.f;
    po_out->b = COLORI32WEB_GETB( color ) / 255.f;
    po_out->a = COLORI32WEB_GETA( color ) / 255.f;
}

colori32web LiceMath::ColorF128ToI32Web( const f128color *cpo_color )
{
    return COLORI32WEB_ARGB( (i32)(cpo_color->a * 255), (i32)(cpo_color->r * 255), (i32)(cpo_color->g * 255), (i32)(cpo_color->b * 255) );
}

f96color LiceMath::ColorI32WebToF96( colori32web color )
{
    return f96color( COLORI32WEB_GETR( color ) / 255.f, COLORI32WEB_GETG( color ) / 255.f, COLORI32WEB_GETB( color ) / 255.f );
}

void LiceMath::ColorI32WebToF96Placed( colori32web color, f96color *po_out )
{
    po_out->r = COLORI32WEB_GETR( color ) / 255.f;
    po_out->g = COLORI32WEB_GETG( color ) / 255.f;
    po_out->b = COLORI32WEB_GETB( color ) / 255.f;
}

colori32web LiceMath::ColorF96ToI32Web( const f96color *cpo_color )
{
    return COLORI32WEB_ARGB( 255, (i32)(cpo_color->r * 255), (i32)(cpo_color->g * 255), (i32)(cpo_color->b * 255) );
}

void LiceMath::M4x4RotateX( m4x4 *m, f32 x )
{
    m->e00 = 1; m->e01 = 0;          m->e02 = 0;         m->e03 = 0;
    m->e10 = 0; m->e11 = cosf( x );  m->e12 = sinf( x ); m->e13 = 0;
    m->e20 = 0; m->e21 = -sinf( x ); m->e22 = cosf( x ); m->e23 = 0;
    m->e30 = 0; m->e31 = 0;          m->e32 = 0;         m->e33 = 1;
}

void LiceMath::M4x4RotateXDoNotIdentity( m4x4 *m, f32 x )
{
    m->e11 = cosf( x );
    m->e12 = sinf( x );

    m->e21 = -sinf( x );
    m->e22 = cosf( x );
}

void LiceMath::M4x4RotateY( m4x4 *m, f32 y )
{
    m->e00 = cosf( y ); m->e01 = 0; m->e02 = -sinf( y ); m->e03 = 0;
    m->e10 = 0;         m->e11 = 1; m->e12 = 0;          m->e13 = 0;
    m->e20 = sinf( y ); m->e21 = 0; m->e22 = cosf( y );  m->e23 = 0;
    m->e30 = 0;         m->e31 = 0; m->e32 = 0;          m->e33 = 1;
}

void LiceMath::M4x4RotateYDoNotIdentity( m4x4 *m, f32 y )
{
    m->e00 = cosf( y );
    m->e02 = -sinf( y );

    m->e20 = sinf( y );
    m->e22 = cosf( y );
}

void LiceMath::M4x4RotateZ( m4x4 *m, f32 z )
{
    m->e00 = cosf( z );  m->e01 = sinf( z ); m->e02 = 0; m->e03 = 0;
    m->e10 = -sinf( z ); m->e11 = cosf( z ); m->e12 = 0; m->e13 = 0;
    m->e20 = 0;          m->e21 = 0;         m->e22 = 1; m->e23 = 0;
    m->e30 = 0;          m->e31 = 0;         m->e32 = 0; m->e33 = 1;
}

void LiceMath::M4x4RotateZDoNotIdentity( m4x4 *m, f32 z )
{
    m->e00 = cosf( z );
    m->e01 = sinf( z );

    m->e10 = -sinf( z );
    m->e11 = cosf( z );
}

void LiceMath::M4x4RotateXYZ( m4x4 *m, f32 x, f32 y, f32 z )
{
    m->e00 = cosf( y ) * cosf( z );
    m->e01 = cosf( y ) * sinf( z );
    m->e02 = -sinf( y );
    m->e03 = 0;

    m->e10 = sinf( x ) * sinf( y ) * cosf( z ) - cosf( x ) * sinf( z );
    m->e11 = sinf( x ) * sinf( y ) * sinf( z ) + cosf( x ) * cosf( z );
    m->e12 = sinf( x ) * cosf( y );
    m->e13 = 0;

    m->e20 = cosf( x ) * sinf( y ) * cosf( z ) + sinf( x ) * sinf( z );
    m->e21 = cosf( x ) * sinf( y ) * sinf( z ) - sinf( x ) * cosf( z );
    m->e22 = cosf( x ) * cosf( y );
    m->e23 = 0;

    m->e30 = 0;
    m->e31 = 0;
    m->e32 = 0;
    m->e33 = 1;
}

void LiceMath::M4x4RotateXYZDoNotIdentity( m4x4 *m, f32 x, f32 y, f32 z )
{
    m->e00 = cosf( y ) * cosf( z );
    m->e01 = cosf( y ) * sinf( z );
    m->e02 = -sinf( y );

    m->e10 = sinf( x ) * sinf( y ) * cosf( z ) - cosf( x ) * sinf( z );
    m->e11 = sinf( x ) * sinf( y ) * sinf( z ) + cosf( x ) * cosf( z );
    m->e12 = sinf( x ) * cosf( y );

    m->e20 = cosf( x ) * sinf( y ) * cosf( z ) + sinf( x ) * sinf( z );
    m->e21 = cosf( x ) * sinf( y ) * sinf( z ) - sinf( x ) * cosf( z );
    m->e22 = cosf( x ) * cosf( y );
}

void LiceMath::M4x4RotateXYZTransformLastIden( m4x4 *m, f32 x, f32 y, f32 z )  //  TODO: temporary
{
    m4x4 o_m;
    M4x4RotateXYZ( &o_m, x, y, z );
    M4x4MultM4x4Inplace( m, &o_m );
}

void LiceMath::M4x4RotateXY( m4x4 *m, f32 x, f32 y )
{
    m->e00 = cosf( y );
    m->e01 = 0;
    m->e02 = -sinf( y );
    m->e03 = 0;

    m->e10 = sinf( x ) * sinf( y );
    m->e11 = cosf( x );
    m->e12 = sinf( x ) * cosf( y );
    m->e13 = 0;

    m->e20 = cosf( x ) * sinf( y );
    m->e21 = -sinf( x );
    m->e22 = cosf( x ) * cosf( y );
    m->e23 = 0;

    m->e30 = 0;
    m->e31 = 0;
    m->e32 = 0;
    m->e33 = 1;
}

void LiceMath::M4x4RotateXYDoNotIdentity( m4x4 *m, f32 x, f32 y )
{
    m->e00 = cosf( y );
    m->e02 = -sinf( y );

    m->e10 = sinf( x ) * sinf( y );
    m->e11 = cosf( x );
    m->e12 = sinf( x ) * cosf( y );

    m->e20 = cosf( x ) * sinf( y );
    m->e21 = -sinf( x );
    m->e22 = cosf( x ) * cosf( y );
}

void LiceMath::M4x4RotateXYTransformLastIden( m4x4 *m, f32 x, f32 y )  //  TODO: temporary
{
    m4x4 o_m;
    M4x4RotateXY( &o_m, x, y );
    M4x4MultM4x4Inplace( m, &o_m );
}

void LiceMath::M4x4RotateAxis( m4x4 *m, const vec3 *v, f32 angle )
{
    angle = -angle;

    m->e00 = v->x * v->x + (1 - v->x * v->x) * cosf( angle );
    m->e01 = v->x * v->y * (1 - cosf( angle )) - v->z * sinf( angle );
    m->e02 = v->x * v->z * (1 - cosf( angle )) + v->y * sinf( angle );
    m->e03 = 0;

    m->e10 = v->x * v->y * (1 - cosf( angle )) + v->z * sinf( angle );
    m->e11 = v->y * v->y + (1 - v->y * v->y) * cosf( angle );
    m->e12 = v->y * v->z * (1 - cosf( angle )) - v->x * sinf( angle );
    m->e13 = 0;

    m->e20 = v->x * v->z * (1 - cosf( angle )) - v->y * sinf( angle );
    m->e21 = v->y * v->z * (1 - cosf( angle )) + v->x * sinf( angle );
    m->e22 = v->z * v->z + (1 - v->z * v->z) * cosf( angle );
    m->e23 = 0;

    m->e30 = 0;
    m->e31 = 0;
    m->e32 = 0;
    m->e33 = 1;
}

void LiceMath::M4x4RotateAxisDoNotIdentity( m4x4 *m, const vec3 *v, f32 angle )
{
    angle = -angle;

    m->e00 = v->x * v->x + (1 - v->x * v->x) * cosf( angle );
    m->e01 = v->x * v->y * (1 - cosf( angle )) - v->z * sinf( angle );
    m->e02 = v->x * v->z * (1 - cosf( angle )) + v->y * sinf( angle );

    m->e10 = v->x * v->y * (1 - cosf( angle )) + v->z * sinf( angle );
    m->e11 = v->y * v->y + (1 - v->y * v->y) * cosf( angle );
    m->e12 = v->y * v->z * (1 - cosf( angle )) - v->x * sinf( angle );

    m->e20 = v->x * v->z * (1 - cosf( angle )) - v->y * sinf( angle );
    m->e21 = v->y * v->z * (1 - cosf( angle )) + v->x * sinf( angle );
    m->e22 = v->z * v->z + (1 - v->z * v->z) * cosf( angle );
}

void LiceMath::M4x4Identity( m4x4 *m )
{
    m->e00 = 1; m->e01 = 0; m->e02 = 0; m->e03 = 0;
    m->e10 = 0; m->e11 = 1; m->e12 = 0; m->e13 = 0;
    m->e20 = 0; m->e21 = 0; m->e22 = 1; m->e23 = 0;
    m->e30 = 0; m->e31 = 0; m->e32 = 0; m->e33 = 1;
}

bln LiceMath::M4x4IsIdentity( const m4x4 *m )
{
    return Funcs::MemEquals( m, &co_Iden4x4, sizeof(m4x4) );
}

bln LiceMath::M4x4IsLastIdentity( const m4x4 *m )
{
    return m->e03 == 0.f && m->e13 == 0.f && m->e23 == 0.f && m->e33 == 1.f;
}

void LiceMath::M4x4Transpose( m4x4 *RSTR m0, const m4x4 *m1 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 4; ++w )
        {
            m0->m[ h ][ w ] = m1->m[ w ][ h ];
        }
    }
}

void LiceMath::M4x4TransposeInplace( m4x4 *m )
{
    Funcs::Swap( &m->m[ 0 ][ 1 ], &m->m[ 1 ][ 0 ] );
    Funcs::Swap( &m->m[ 0 ][ 2 ], &m->m[ 2 ][ 0 ] );
    Funcs::Swap( &m->m[ 0 ][ 3 ], &m->m[ 3 ][ 0 ] );
    Funcs::Swap( &m->m[ 1 ][ 2 ], &m->m[ 2 ][ 1 ] );
    Funcs::Swap( &m->m[ 1 ][ 3 ], &m->m[ 3 ][ 1 ] );
    Funcs::Swap( &m->m[ 2 ][ 3 ], &m->m[ 3 ][ 2 ] );

    /*for( ui32 index = 0; index < 3; ++index )
    {
        for( ui32 i = index + 1; i < 4; ++i )
        {
            f32 temp = m->m[ i ][ index ];
            m->m[ i ][ index ] = m->m[ index ][ i ];
            m->m[ index ][ i ] = temp;
        }
    }*/
}

void LiceMath::M4x4MultM4x4( m4x4 *RSTR m0, const m4x4 *m1, const m4x4 *m2 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 4; ++w )
        {
            m0->m[ h ][ w ] = m1->m[ h ][ 0 ] * m2->m[ 0 ][ w ] +
                              m1->m[ h ][ 1 ] * m2->m[ 1 ][ w ] +
                              m1->m[ h ][ 2 ] * m2->m[ 2 ][ w ] +
                              m1->m[ h ][ 3 ] * m2->m[ 3 ][ w ] ;
        }
    }
}

void LiceMath::M4x4MultM4x4Inplace( m4x4 *RSTR m0, const m4x4 *m1 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        f32 temp[ 4 ];
        for( ui32 w = 0; w < 4; ++w )
        {
            temp[ w ] = m0->m[ h ][ 0 ] * m1->m[ 0 ][ w ] +
                        m0->m[ h ][ 1 ] * m1->m[ 1 ][ w ] +
                        m0->m[ h ][ 2 ] * m1->m[ 2 ][ w ] +
                        m0->m[ h ][ 3 ] * m1->m[ 3 ][ w ] ;
        }
        _MemCpy( m0->m[ h ], temp, sizeof(temp) );
    }
}

void LiceMath::M4x4MultScalar( m4x4 *RSTR m0, const m4x4 *m1, f32 scalar )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 4; ++w )
        {
            m0->m[ h ][ w ] = m1->m[ h ][ w ] * scalar;
        }
    }
}

void LiceMath::M4x4MultScalarInplace( m4x4 *RSTR m, f32 scalar )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 4; ++w )
        {
            m->m[ h ][ w ] *= scalar;
        }
    }
}

void LiceMath::M4x4Translate3D( m4x4 *m, f32 x, f32 y, f32 z )
{
    m->e00 = 1; m->e01 = 0; m->e02 = 0; m->e03 = 0;
    m->e10 = 0; m->e11 = 1; m->e12 = 0; m->e13 = 0;
    m->e20 = 0; m->e21 = 0; m->e22 = 1; m->e23 = 0;
    m->e30 = x; m->e31 = y; m->e32 = z; m->e33 = 1;
}

void LiceMath::M4x4Translate3DTransformLastIden( m4x4 *m, f32 x, f32 y, f32 z )
{
    m->e30 += x;
    m->e31 += y;
    m->e32 += z;
}

void LiceMath::M4x4Translate3DDoNotIdentity( m4x4 *m, f32 x, f32 y, f32 z )
{
    m->e30 = x;
    m->e31 = y;
    m->e32 = z;
}

void LiceMath::M4x4Translate2D( m4x4 *m, f32 x, f32 y )
{
    m->e00 = 1; m->e01 = 0; m->e02 = 0; m->e03 = 0;
    m->e10 = 0; m->e11 = 1; m->e12 = 0; m->e13 = 0;
    m->e20 = 0; m->e21 = 0; m->e22 = 1; m->e23 = 0;
    m->e30 = x; m->e31 = y; m->e32 = 0; m->e33 = 1;
}

void LiceMath::M4x4Translate2DTransformLastIden( m4x4 *m, f32 x, f32 y )
{
    m->e30 += x;
    m->e31 += y;
}

void LiceMath::M4x4Translate2DDoNotIdentity( m4x4 *m, f32 x, f32 y )
{
    m->e30 = x;
    m->e31 = y;
}

void LiceMath::M4x4Scale3D( m4x4 *m, f32 x, f32 y, f32 z )
{
    m->e00 = x; m->e01 = 0; m->e02 = 0; m->e03 = 0;
    m->e10 = 0; m->e11 = y; m->e12 = 0; m->e13 = 0;
    m->e20 = 0; m->e21 = 0; m->e22 = z; m->e23 = 0;
    m->e30 = 0; m->e31 = 0; m->e32 = 0; m->e33 = 1;
}

void LiceMath::M4x4Scale3DTransformLastIden( m4x4 *m, f32 x, f32 y, f32 z )
{
    m->e00 *= x; m->e01 *= y; m->e02 *= z;
    m->e10 *= x; m->e11 *= y; m->e12 *= z;
    m->e20 *= x; m->e21 *= y; m->e22 *= z;
}

void LiceMath::M4x4Scale3DDoNotIdentity( m4x4 *m, f32 x, f32 y, f32 z )
{
    m->e00 = x;
    m->e11 = y;
    m->e22 = z;
}

void LiceMath::M4x4Scale2D( m4x4 *m, f32 x, f32 y )
{
    m->e00 = x; m->e01 = 0; m->e02 = 0; m->e03 = 0;
    m->e10 = 0; m->e11 = y; m->e12 = 0; m->e13 = 0;
    m->e20 = 0; m->e21 = 0; m->e22 = 1; m->e23 = 0;
    m->e30 = 0; m->e31 = 0; m->e32 = 0; m->e33 = 1;
}

void LiceMath::M4x4Scale2DTransformLastIden( m4x4 *m, f32 x, f32 y )
{
    m->e00 *= x; m->e01 *= y;
    m->e10 *= x; m->e11 *= y;
    m->e20 *= x; m->e21 *= y;
}

void LiceMath::M4x4Scale2DDoNotIdentity( m4x4 *m, f32 x, f32 y )
{
    m->e00 = x;
    m->e11 = y;
}

void LiceMath::M4x4Resize( m4x4 *m, f32 k )
{
    m->e00 = k; m->e01 = 0; m->e02 = 0; m->e03 = 0;
    m->e10 = 0; m->e11 = k; m->e12 = 0; m->e13 = 0;
    m->e20 = 0; m->e21 = 0; m->e22 = k; m->e23 = 0;
    m->e30 = 0; m->e31 = 0; m->e32 = 0; m->e33 = 1;
}

void LiceMath::M4x4ResizeTransformLastIden( m4x4 *m, f32 koef )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        m->m[ h ][ 0 ] *= koef;
        m->m[ h ][ 1 ] *= koef;
        m->m[ h ][ 2 ] *= koef;
    }
}

void LiceMath::M4x4ResizeDoNotIdentity( m4x4 *m, f32 koef )
{
    m->e00 = koef;
    m->e11 = koef;
    m->e22 = koef;
}

void LiceMath::M4x4Addition( m4x4 *RSTR m0, const m4x4 *m1, const m4x4 *m2 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 4; ++w )
        {
            m0->m[ h ][ w ] = m1->m[ h ][ w ] + m2->m[ h ][ w ];
        }
    }
}

void LiceMath::M4x4AdditionInplace( m4x4 *RSTR m0, const m4x4 *m1 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 4; ++w )
        {
            m0->m[ h ][ w ] += m1->m[ h ][ w ];
        }
    }
}

void LiceMath::M4x4Subtract( m4x4 *RSTR m0, const m4x4 *m1, const m4x4 *m2 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 4; ++w )
        {
            m0->m[ h ][ w ] = m1->m[ h ][ w ] - m2->m[ h ][ w ];
        }
    }
}

void LiceMath::M4x4SubtractInplace( m4x4 *RSTR m0, const m4x4 *m1 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 4; ++w )
        {
            m0->m[ h ][ w ] -= m1->m[ h ][ w ];
        }
    }
}

void LiceMath::M4x4Determinant( const m4x4 *m )
{

}

void LiceMath::M4x3RotateX( m4x3 *m, f32 x )
{
    f32 cx = ::cosf( x );
    f32 sx = ::sinf( x );

    m->e00 = 1; m->e01 = 0;   m->e02 = 0;
    m->e10 = 0; m->e11 = cx;  m->e12 = sx;
    m->e20 = 0; m->e21 = -sx; m->e22 = cx;
    m->e30 = 0; m->e31 = 0;   m->e32 = 0;
}

void LiceMath::M4x3RotateXDoNotIdentity( m4x3 *m, f32 x )
{
    m->e11 = cosf( x );  m->e12 = sinf( x );
    m->e21 = -sinf( x ); m->e22 = cosf( x );
}

void LiceMath::M4x3RotateY( m4x3 *m, f32 y )
{
    f32 cy = ::cosf( y );
    f32 sy = ::sinf( y );

    m->e00 = cy; m->e01 = 0; m->e02 = -sy;
    m->e10 = 0;  m->e11 = 1; m->e12 = 0;
    m->e20 = sy; m->e21 = 0; m->e22 = cy;
    m->e30 = 0;  m->e31 = 0; m->e32 = 0;
}

void LiceMath::M4x3RotateYDoNotIdentity( m4x3 *m, f32 y )
{
    m->e00 = cosf( y ); m->e02 = -sinf( y );
    m->e20 = sinf( y ); m->e22 = cosf( y );
}

void LiceMath::M4x3RotateZ( m4x3 *m, f32 z )
{
    f32 cz = ::cosf( z );
    f32 sz = ::sinf( z );

    m->e00 = cz;  m->e01 = sz; m->e02 = 0;
    m->e10 = -sz; m->e11 = cz; m->e12 = 0;
    m->e20 = 0;   m->e21 = 0;  m->e22 = 1;
    m->e30 = 0;   m->e31 = 0;  m->e32 = 0;
}

void LiceMath::M4x3RotateZDoNotIdentity( m4x3 *m, f32 z )
{
    f32 cz = ::cosf( z );
    f32 sz = ::sinf( z );

    m->e00 = cz;  m->e01 = sz;
    m->e10 = -sz; m->e11 = cz;
}

void LiceMath::M4x3RotateXYZ( m4x3 *m, f32 x, f32 y, f32 z )
{
    M4x3RotateXYZDoNotIdentity( m, x, y, z );

    m->e30 = 0;
    m->e31 = 0;
    m->e32 = 0;
}

void LiceMath::M4x3RotateXYZDoNotIdentity( m4x3 *m, f32 x, f32 y, f32 z )
{
    f32 cx = ::cosf( x );
    f32 cy = ::cosf( y );
    f32 cz = ::cosf( z );

    f32 sx = ::sinf( x );
    f32 sy = ::sinf( y );
    f32 sz = ::sinf( z );

    m->e00 = cy * cz;
    m->e01 = cy * sz;
    m->e02 = -sy;

    m->e10 = sx * sy * cz - cx * sz;
    m->e11 = sx * sy * sz + cx * cz;
    m->e12 = sx * cy;

    m->e20 = cx * sy * cz + sx * sz;
    m->e21 = cx * sy * sz - sx * cz;
    m->e22 = cx * cy;
}

void LiceMath::M4x3RotateXYZTransform( m4x3 *m, f32 x, f32 y, f32 z )  //  TODO: temporary
{
    m4x3 o_m;
    M4x3RotateXYZ( &o_m, x, y, z );
    M4x3MultM4x3AsM4x4LastIdenInplace( m, &o_m );
}

void LiceMath::M4x3RotateXYZTranslate3DVec( m4x3 *m, const vec3 *rot, const vec3 *tlate )
{
    f32 cx = ::cosf( rot->x );
    f32 cy = ::cosf( rot->y );
    f32 cz = ::cosf( rot->z );

    f32 sx = ::sinf( rot->x );
    f32 sy = ::sinf( rot->y );
    f32 sz = ::sinf( rot->z );

    m->e00 = (cy * cz);
    m->e01 = (cy * sz);
    m->e02 = (-sy);

    m->e10 = (sx * sy * cz - cx * sz);
    m->e11 = (sx * sy * sz + cx * cz);
    m->e12 = (sx * cy);

    m->e20 = (cx * sy * cz + sx * sz);
    m->e21 = (cx * sy * sz - sx * cz);
    m->e22 = (cx * cy);

    m->e30 = tlate->x;
    m->e31 = tlate->y;
    m->e32 = tlate->z;
}

void LiceMath::M4x3RotateXYZTranslate3D( m4x3 *m, f32 rx, f32 ry, f32 rz, f32 tx, f32 ty, f32 tz )
{
    M4x3RotateXYZTranslate3DVec( m, &vec3( rx, ry, rz ), &vec3( tx, ty, tz ) );
}

void LiceMath::M4x3RotateXY( m4x3 *m, f32 x, f32 y )
{
    M4x3RotateXYDoNotIdentity( m, x, y );

    m->e30 = 0;
    m->e31 = 0;
    m->e32 = 0;
}

void LiceMath::M4x3RotateXYDoNotIdentity( m4x3 *m, f32 x, f32 y )
{
    f32 cx = ::cosf( x );
    f32 cy = ::cosf( y );

    f32 sx = ::sinf( x );
    f32 sy = ::sinf( y );

    m->e00 = cy;
    m->e02 = -sy;

    m->e10 = sx * sy;
    m->e11 = cx;
    m->e12 = sx * cy;

    m->e20 = cx * sy;
    m->e21 = -sx;
    m->e22 = cx * cy;
}

void LiceMath::M4x3RotateXYTransform( m4x3 *m0, const m4x3 *m1, f32 x, f32 y )  //  TODO: temporary
{
    m4x3 o_m;
    M4x3RotateXY( &o_m, x, y );
    M4x3MultM4x3AsM4x4LastIden( m0, m1, &o_m );
}

void LiceMath::M4x3RotateXYTransformInplace( m4x3 *m, f32 x, f32 y )  //  TODO: temporary
{
    m4x3 o_m;
    M4x3RotateXY( &o_m, x, y );
    M4x3MultM4x3AsM4x4LastIdenInplace( m, &o_m );
}

void LiceMath::M4x3RotateAxis( m4x3 *m, const vec3 *v, f32 angle )
{
    M4x3RotateAxisDoNotIdentity( m, v, angle );

    m->e30 = 0;
    m->e31 = 0;
    m->e32 = 0;
}

void LiceMath::M4x3RotateAxisDoNotIdentity( m4x3 *m, const vec3 *v, f32 angle )
{
    angle = -angle;

    m->e00 = v->x * v->x + (1 - v->x * v->x) * cosf( angle );
    m->e01 = v->x * v->y * (1 - cosf( angle )) - v->z * sinf( angle );
    m->e02 = v->x * v->z * (1 - cosf( angle )) + v->y * sinf( angle );

    m->e10 = v->x * v->y * (1 - cosf( angle )) + v->z * sinf( angle );
    m->e11 = v->y * v->y + (1 - v->y * v->y) * cosf( angle );
    m->e12 = v->y * v->z * (1 - cosf( angle )) - v->x * sinf( angle );

    m->e20 = v->x * v->z * (1 - cosf( angle )) - v->y * sinf( angle );
    m->e21 = v->y * v->z * (1 - cosf( angle )) + v->x * sinf( angle );
    m->e22 = v->z * v->z + (1 - v->z * v->z) * cosf( angle );
}

void LiceMath::M4x3Identity( m4x3 *m )
{
    m->e00 = 1; m->e01 = 0; m->e02 = 0;
    m->e10 = 0; m->e11 = 1; m->e12 = 0;
    m->e20 = 0; m->e21 = 0; m->e22 = 1;
    m->e30 = 0; m->e31 = 0; m->e32 = 0;
}

bln LiceMath::M4x3IsIdentity( const m4x3 *m )
{
    return Funcs::MemEquals( m, &co_Iden4x3, sizeof(m4x3) );
}

void LiceMath::M4x3Transpose( m3x4 *m0, const m4x3 *m1 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        m0->m[ 0 ][ h ] = m1->m[ h ][ 0 ];
        m0->m[ 1 ][ h ] = m1->m[ h ][ 1 ];
        m0->m[ 2 ][ h ] = m1->m[ h ][ 2 ];
    }
}
/*
//  4x3
f32 e00, e01, e02,
    e10, e11, e12,
    e20, e21, e22,
    e30, e31, e32;

//  4x3 in mem
f32 e00, e01, e02, e10,
    e11, e12, e20, e21,
    e22, e30, e31, e32;

//  3x4
f32 e00, e10, e20, e30,
    e01, e11, e21, e31,
    e02, e12, e22, e32;
*/
m3x4 *LiceMath::M4x3TransposeInplaceAs3x4( m4x3 *m )
{
    f32 e01 = m->e10;
    f32 e02 = m->e20;
    f32 e10 = m->e30;
    f32 e11 = m->e01;
    f32 e12 = m->e11;
    f32 e20 = m->e21;
    f32 e21 = m->e31;
    f32 e22 = m->e02;
    f32 e30 = m->e12;
    f32 e31 = m->e22;

                  m->e01 = e01; m->e02 = e02;
    m->e10 = e10; m->e11 = e11; m->e12 = e12;
    m->e20 = e20; m->e21 = e21; m->e22 = e22;
    m->e30 = e30; m->e31 = e31;

    return (m3x4 *)m;
}

void LiceMath::M4x3MultM4x3AsM4x4LastIden( m4x3 *RSTR m0, const m4x3 *m1, const m4x3 *m2 )
{
    for( ui32 h = 0; h < 3; ++h )
    {
        for( ui32 w = 0; w < 3; ++w )
        {
            m0->m[ h ][ w ] = m1->m[ h ][ 0 ] * m2->m[ 0 ][ w ] +
                              m1->m[ h ][ 1 ] * m2->m[ 1 ][ w ] +
                              m1->m[ h ][ 2 ] * m2->m[ 2 ][ w ];
        }
    }
    for( ui32 w = 0; w < 3; ++w )
    {
        m0->m[ 3 ][ w ] = m1->m[ 3 ][ 0 ] * m2->m[ 0 ][ w ] +
                          m1->m[ 3 ][ 1 ] * m2->m[ 1 ][ w ] +
                          m1->m[ 3 ][ 2 ] * m2->m[ 2 ][ w ] +
                                            m2->m[ 3 ][ w ];
    }
}

void LiceMath::M4x3MultM4x3AsM4x4LastIdenInplace( m4x3 *RSTR m0, const m4x3 *m1 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        f32 temp[ 3 ];
        for( ui32 w = 0; w < 3; ++w )
        {
            temp[ w ] = m0->m[ h ][ 0 ] * m1->m[ 0 ][ w ] +
                        m0->m[ h ][ 1 ] * m1->m[ 1 ][ w ] +
                        m0->m[ h ][ 2 ] * m1->m[ 2 ][ w ];
            if( h == 3 )
            {
                temp[ w ] += m1->m[ 3 ][ w ];
            }
        }
        _MemCpy( m0->m[ h ], temp, sizeof(temp) );
    }
}

void LiceMath::M4x3AsM4x4LastIdenMultM4x4( m4x4 *RSTR m0, const m4x3 *m1, const m4x4 *m2 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 4; ++w )
        {
            m0->m[ h ][ w ] = m1->m[ h ][ 0 ] * m2->m[ 0 ][ w ] +
                              m1->m[ h ][ 1 ] * m2->m[ 1 ][ w ] +
                              m1->m[ h ][ 2 ] * m2->m[ 2 ][ w ];
            if( h == 3 )
            {
                m0->m[ h ][ w ] += m2->m[ 3 ][ w ];
            }
        }
    }
}

void LiceMath::M4x3MultScalar( m4x3 *RSTR m0, const m4x3 *m1, f32 scalar )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 3; ++w )
        {
            m0->m[ h ][ w ] = m1->m[ h ][ w ] * scalar;
        }
    }
}

void LiceMath::M4x3MultScalarInplace( m4x3 *RSTR m, f32 scalar )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 3; ++w )
        {
            m->m[ h ][ w ] *= scalar;
        }
    }
}

void LiceMath::M4x3Translate3D( m4x3 *m, f32 x, f32 y, f32 z )
{
    m->e00 = 1; m->e01 = 0; m->e02 = 0;
    m->e10 = 0; m->e11 = 1; m->e12 = 0;
    m->e20 = 0; m->e21 = 0; m->e22 = 1;
    m->e30 = x; m->e31 = y; m->e32 = z;
}

void LiceMath::M4x3Translate3DTransform( m4x3 *RSTR m0, const m4x3 *m1, f32 x, f32 y, f32 z )
{
    *m0 = *m1;
    m0->e30 += x;
    m0->e31 += y;
    m0->e32 += z;
}

void LiceMath::M4x3Translate3DTransformInplace( m4x3 *m, f32 x, f32 y, f32 z )
{
    m->e30 += x;
    m->e31 += y;
    m->e32 += z;
}

void LiceMath::M4x3Translate3DDoNotIdentity( m4x3 *m, f32 x, f32 y, f32 z )
{
    m->e30 = x;
    m->e31 = y;
    m->e32 = z;
}

void LiceMath::M4x3Translate2D( m4x3 *m, f32 x, f32 y )
{
    m->e00 = 1; m->e01 = 0; m->e02 = 0;
    m->e10 = 0; m->e11 = 1; m->e12 = 0;
    m->e20 = 0; m->e21 = 0; m->e22 = 1;
    m->e30 = x; m->e31 = y; m->e32 = 0;
}

void LiceMath::M4x3Translate2DTransform( m4x3 *m0, const m4x3 *m1, f32 x, f32 y )
{
    *m0 = *m1;
    m0->e30 += x;
    m0->e31 += y;
}

void LiceMath::M4x3Translate2DTransformInplace( m4x3 *m, f32 x, f32 y )
{
    m->e30 += x;
    m->e31 += y;
}

void LiceMath::M4x3Translate2DDoNotIdentity( m4x3 *m, f32 x, f32 y )
{
    m->e30 = x;
    m->e31 = y;
}

void LiceMath::M4x3Scale3D( m4x3 *m, f32 x, f32 y, f32 z )
{
    m->e00 = x; m->e01 = 0; m->e02 = 0;
    m->e10 = 0; m->e11 = y; m->e12 = 0;
    m->e20 = 0; m->e21 = 0; m->e22 = z;
    m->e30 = 0; m->e31 = 0; m->e32 = 0;
}

void LiceMath::M4x3Scale3DRotateXYZVec( m4x3 *m, const vec3 *cpo_scale, const vec3 *rot )
{
    M4x3Scale3DRotateXYZTranslate3DVec( m, cpo_scale, rot, &vec3( 0, 0, 0 ) );
}

void LiceMath::M4x3Scale3DRotateXYZ( m4x3 *m, f32 sx, f32 sy, f32 sz, f32 rx, f32 ry, f32 rz )
{
    M4x3Scale3DRotateXYZVec( m, &vec3( sx, sy, sz ), &vec3( rx, ry, rz ) );
}

void LiceMath::M4x3Scale3DRotateXYZTranslate3DVec( m4x3 *m, const vec3 *cpo_scale, const vec3 *rot, const vec3 *tlate )
{
    f32 cx = ::cosf( rot->x );
    f32 cy = ::cosf( rot->y );
    f32 cz = ::cosf( rot->z );

    f32 sx = ::sinf( rot->x );
    f32 sy = ::sinf( rot->y );
    f32 sz = ::sinf( rot->z );

    m->e00 = cpo_scale->x * (cy * cz);
    m->e01 = cpo_scale->x * (cy * sz);
    m->e02 = cpo_scale->x * (-sy);

    m->e10 = cpo_scale->y * (sx * sy * cz - cx * sz);
    m->e11 = cpo_scale->y * (sx * sy * sz + cx * cz);
    m->e12 = cpo_scale->y * (sx * cy);

    m->e20 = cpo_scale->z * (cx * sy * cz + sx * sz);
    m->e21 = cpo_scale->z * (cx * sy * sz - sx * cz);
    m->e22 = cpo_scale->z * (cx * cy);

    m->e30 = tlate->x;
    m->e31 = tlate->y;
    m->e32 = tlate->z;
}

void LiceMath::M4x3Scale3DRotateXYZTranslate3D( m4x3 *m, f32 sx, f32 sy, f32 sz, f32 rx, f32 ry, f32 rz, f32 tx, f32 ty, f32 tz )
{
    M4x3Scale3DRotateXYZTranslate3DVec( m, &vec3( sx, sy, sz ), &vec3( rx, ry, rz ), &vec3( tx, ty, tz ) );
}

void LiceMath::M4x3Scale3DTransform( m4x3 *m, f32 x, f32 y, f32 z )
{
    m->e00 *= x; m->e10 *= x; m->e20 *= x;
    m->e01 *= y; m->e11 *= y; m->e21 *= y;
    m->e02 *= z; m->e12 *= z; m->e22 *= z;
}

void LiceMath::M4x3Scale3DDoNotIdentity( m4x3 *m, f32 x, f32 y, f32 z )
{
    m->e00 = x;
    m->e11 = y;
    m->e22 = z;
}

void LiceMath::M4x3Scale2D( m4x3 *m, f32 x, f32 y )
{
    m->e00 = x; m->e01 = 0; m->e02 = 0;
    m->e10 = 0; m->e11 = y; m->e12 = 0;
    m->e20 = 0; m->e21 = 0; m->e22 = 1;
    m->e30 = 0; m->e31 = 0; m->e32 = 0;
}

void LiceMath::M4x3Scale2DTransform( m4x3 *m, f32 x, f32 y )
{
    m->e00 *= x; m->e01 *= y;
    m->e10 *= x; m->e11 *= y;
    m->e20 *= x; m->e21 *= y;
}

void LiceMath::M4x3Scale2DDoNotIdentity( m4x3 *m, f32 x, f32 y )
{
    m->e00 = x;
    m->e11 = y;
}

void LiceMath::M4x3Resize( m4x3 *m, f32 k )
{
    m->e00 = k; m->e01 = 0; m->e02 = 0;
    m->e10 = 0; m->e11 = k; m->e12 = 0;
    m->e20 = 0; m->e21 = 0; m->e22 = k;
    m->e30 = 0; m->e31 = 0; m->e32 = 0;
}

void LiceMath::M4x3ResizeTransform( m4x3 *m, f32 koef )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        m->m[ h ][ 0 ] *= koef;
        m->m[ h ][ 1 ] *= koef;
        m->m[ h ][ 2 ] *= koef;
    }
}

void LiceMath::M4x3ResizeDoNotIdentity( m4x3 *m, f32 koef )
{
    m->e00 = koef;
    m->e11 = koef;
    m->e22 = koef;
}

void LiceMath::M4x3Addition( m4x3 *RSTR m0, const m4x3 *m1, const m4x3 *m2 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 3; ++w )
        {
            m0->m[ h ][ w ] = m1->m[ h ][ w ] + m2->m[ h ][ w ];
        }
    }
}

void LiceMath::M4x3AdditionInplace( m4x3 *RSTR m0, const m4x3 *m1 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 3; ++w )
        {
            m0->m[ h ][ w ] += m1->m[ h ][ w ];
        }
    }
}

void LiceMath::M4x3Subtract( m4x3 *RSTR m0, const m4x3 *m1, const m4x3 *m2 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 3; ++w )
        {
            m0->m[ h ][ w ] = m1->m[ h ][ w ] - m2->m[ h ][ w ];
        }
    }
}

void LiceMath::M4x3SubtractInplace( m4x3 *RSTR m0, const m4x3 *m1 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 3; ++w )
        {
            m0->m[ h ][ w ] -= m1->m[ h ][ w ];
        }
    }
}

void LiceMath::M4x3Determinant( const m4x3 *m )
{
    DBGBREAK;
}

void LiceMath::M3x4MultScalar( m3x4 *RSTR m0, const m3x4 *m1, f32 scalar )
{
    for( ui32 index = 0; index < 12; ++index )
    {
        m0->arr[ index ] = m1->arr[ index ] * scalar;
    }
}

void LiceMath::M3x4MultScalarInplace( m3x4 *RSTR m, f32 scalar )
{
    for( ui32 index = 0; index < 12; ++index )
    {
        m->arr[ index ] *= scalar;
    }
}

void LiceMath::M3x3Identity( m3x3 *m )
{
    m->e00 = 1; m->e01 = 0; m->e02 = 0;
    m->e10 = 0; m->e11 = 1; m->e12 = 0;
    m->e20 = 0; m->e21 = 0; m->e22 = 1;
}

bln LiceMath::M3x3IsIdentity( const m3x3 *m )
{
    return Funcs::MemEquals( m, &co_Iden3x3, sizeof(m3x3) );
}

void LiceMath::M3x3Transpose( m3x3 *RSTR m0, const m3x3 *m1 )
{
    for( ui32 h = 0; h < 3; ++h )
    {
        m0->m[ h ][ 0 ] = m1->m[ 0 ][ h ];
        m0->m[ h ][ 1 ] = m1->m[ 1 ][ h ];
        m0->m[ h ][ 2 ] = m1->m[ 2 ][ h ];
    }
}

void LiceMath::M3x3TransposeInplace( m3x3 *m )
{
    Funcs::Swap( &m->m[ 0 ][ 1 ], &m->m[ 1 ][ 0 ] );
    Funcs::Swap( &m->m[ 0 ][ 2 ], &m->m[ 2 ][ 0 ] );
    Funcs::Swap( &m->m[ 1 ][ 2 ], &m->m[ 2 ][ 1 ] );
}

void LiceMath::M3x3MultM3x3( m3x3 *RSTR m0, const m3x3 *m1, const m3x3 *m2 )
{
    for( ui32 h = 0; h < 3; ++h )
    {
        for( ui32 w = 0; w < 3; ++w )
        {
            m0->m[ h ][ w ] = m1->m[ h ][ 0 ] * m2->m[ 0 ][ w ] +
                              m1->m[ h ][ 1 ] * m2->m[ 1 ][ w ] +
                              m1->m[ h ][ 2 ] * m2->m[ 2 ][ w ] ;
        }
    }
}

void LiceMath::M3x3MultM3x3Inplace( m3x3 *RSTR m0, const m3x3 *m1 )
{
    for( ui32 h = 0; h < 3; ++h )
    {
        f32 temp[ 3 ];
        for( ui32 w = 0; w < 3; ++w )
        {
            temp[ w ] = m0->m[ h ][ 0 ] * m1->m[ 0 ][ w ] +
                        m0->m[ h ][ 1 ] * m1->m[ 1 ][ w ] +
                        m0->m[ h ][ 2 ] * m1->m[ 2 ][ w ] ;
        }
        _MemCpy( m0->m[ h ], temp, sizeof(temp) );
    }
}

void LiceMath::M3x3MultScalar( m3x3 *RSTR m0, const m3x3 *m1, f32 scalar )
{
    for( ui32 h = 0; h < 3; ++h )
    {
        for( ui32 w = 0; w < 3; ++w )
        {
            m0->m[ h ][ w ] = m1->m[ h ][ w ] * scalar;
        }
    }
}

void LiceMath::M3x3MultScalarInplace( m3x3 *RSTR m, f32 scalar )
{
    for( ui32 h = 0; h < 3; ++h )
    {
        for( ui32 w = 0; w < 3; ++w )
        {
            m->m[ h ][ w ] *= scalar;
        }
    }
}

void LiceMath::M3x3Addition( m3x3 *RSTR m0, const m3x3 *m1, const m3x3 *m2 )
{
    for( ui32 h = 0; h < 3; ++h )
    {
        for( ui32 w = 0; w < 3; ++w )
        {
            m0->m[ h ][ w ] = m1->m[ h ][ w ] + m2->m[ h ][ w ];
        }
    }
}

void LiceMath::M3x3AdditionInplace( m3x3 *RSTR m0, const m3x3 *m1 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 4; ++w )
        {
            m0->m[ h ][ w ] += m1->m[ h ][ w ];
        }
    }
}

void LiceMath::M3x3Subtract( m3x3 *RSTR m0, const m3x3 *m1, const m3x3 *m2 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 4; ++w )
        {
            m0->m[ h ][ w ] = m1->m[ h ][ w ] - m2->m[ h ][ w ];
        }
    }
}

void LiceMath::M3x3SubtractInplace( m3x3 *RSTR m0, const m3x3 *m1 )
{
    for( ui32 h = 0; h < 4; ++h )
    {
        for( ui32 w = 0; w < 4; ++w )
        {
            m0->m[ h ][ w ] -= m1->m[ h ][ w ];
        }
    }
}

void LiceMath::M3x3RotateAxis( m3x3 *m, const vec3 *v, f32 angle )
{
    angle = -angle;

    m->e00 = v->x * v->x + (1 - v->x * v->x) * cosf( angle );
    m->e01 = v->x * v->y * (1 - cosf( angle )) - v->z * sinf( angle );
    m->e02 = v->x * v->z * (1 - cosf( angle )) + v->y * sinf( angle );

    m->e10 = v->x * v->y * (1 - cosf( angle )) + v->z * sinf( angle );
    m->e11 = v->y * v->y + (1 - v->y * v->y) * cosf( angle );
    m->e12 = v->y * v->z * (1 - cosf( angle )) - v->x * sinf( angle );

    m->e20 = v->x * v->z * (1 - cosf( angle )) - v->y * sinf( angle );
    m->e21 = v->y * v->z * (1 - cosf( angle )) + v->x * sinf( angle );
    m->e22 = v->z * v->z + (1 - v->z * v->z) * cosf( angle );
}

void LiceMath::M3x3RotateX( m3x3 *m, f32 x )
{
    m->e00 = 1; m->e01 = 0;          m->e02 = 0;
    m->e10 = 0; m->e11 = cosf( x );  m->e12 = sinf( x );
    m->e20 = 0; m->e21 = -sinf( x ); m->e22 = cosf( x );
}

void LiceMath::M3x3RotateY( m3x3 *m, f32 y )
{
    m->e00 = cosf( y ); m->e01 = 0; m->e02 = -sinf( y );
    m->e10 = 0;         m->e11 = 1; m->e12 = 0;
    m->e20 = sinf( y ); m->e21 = 0; m->e22 = cosf( y );
}

void LiceMath::M3x3RotateZ( m3x3 *m, f32 z )
{
    m->e00 = cosf( z );  m->e01 = sinf( z ); m->e02 = 0;
    m->e10 = -sinf( z ); m->e11 = cosf( z ); m->e12 = 0;
    m->e20 = 0;          m->e21 = 0;         m->e22 = 1;
}

void LiceMath::M3x3RotateXYZ( m3x3 *m, f32 x, f32 y, f32 z )
{
    f32 cx = ::cosf( x );
    f32 cy = ::cosf( y );
    f32 cz = ::cosf( z );

    f32 sx = ::sinf( x );
    f32 sy = ::sinf( y );
    f32 sz = ::sinf( z );

    m->e00 = cy * cz;
    m->e01 = cy * sz;
    m->e02 = -sy;

    m->e10 = sx * sy * cz - cx * sz;
    m->e11 = sx * sy * sz + cx * cz;
    m->e12 = sx * cy;

    m->e20 = cx * sy * cz + sx * sz;
    m->e21 = cx * sy * sz - sx * cz;
    m->e22 = cx * cy;
}

void LiceMath::M3x2Identity( m3x2 *m )
{
    m->e00 = 1; m->e01 = 0;
    m->e10 = 0; m->e11 = 1;
    m->e20 = 0; m->e21 = 0;
}

void LiceMath::M3x2Transpose( m2x3 *RSTR m0, const m3x2 *m1 )
{
    m0->e00 = m1->e00; m0->e01 = m1->e10; m0->e02 = m1->e20;
    m0->e10 = m1->e01; m0->e11 = m1->e11; m0->e12 = m1->e21;
}

void LiceMath::M3x2Translate( m3x2 *m, f32 x, f32 y )
{
    m->e00 = 1; m->e01 = 0;
    m->e10 = 0; m->e11 = 1;
    m->e20 = x; m->e21 = y;
}

void LiceMath::M3x2RotateZ( m3x2 *m, f32 rot )
{
    m->e00 = cosf( rot );  m->e01 = sinf( rot );
    m->e10 = -sinf( rot ); m->e11 = cosf( rot );
    m->e20 = 0;            m->e21 = 0;
}

void LiceMath::M3x2Scale( m3x2 *m, f32 x, f32 y )
{
    m->e00 = x; m->e01 = 0;
    m->e10 = 0; m->e11 = y;
    m->e20 = 0; m->e21 = 0;
}

void LiceMath::M3x2ScaleRotateZ( m3x2 *m, f32 x, f32 y, f32 rot )
{
    m->e00 = cosf( rot ) * x;  m->e01 = sinf( rot ) * x;
    m->e10 = -sinf( rot ) * y; m->e11 = cosf( rot ) * y;
    m->e20 = 0;                m->e21 = 0;
}

void LiceMath::M3x2ScaleRotateZTranslate( m3x2 *m, f32 sx, f32 sy, f32 rot, f32 x, f32 y )
{
    m->e00 = cosf( rot ) * sx;  m->e01 = sinf( rot ) * sx;
    m->e10 = -sinf( rot ) * sy; m->e11 = cosf( rot ) * sy;
    m->e20 = x;                 m->e21 = y;
}

void LiceMath::M3x2Mult3x2( m3x2 *RSTR m0, const m3x2 *m1, const m3x2 *m2 )
{
    for( uiw h = 0; h < 3; ++h )
    {
        m0->m[ h ][ 0 ] = m1->m[ h ][ 0 ] * m2->m[ 0 ][ 0 ] +
                          m1->m[ h ][ 1 ] * m2->m[ 1 ][ 0 ];

        m0->m[ h ][ 1 ] = m1->m[ h ][ 0 ] * m2->m[ 0 ][ 1 ] +
                          m1->m[ h ][ 1 ] * m2->m[ 1 ][ 1 ];
    }

    m0->m[ 2 ][ 0 ] += m2->m[ 2 ][ 0 ];
    m0->m[ 2 ][ 1 ] += m2->m[ 2 ][ 1 ];
}

void LiceMath::M3x2Mult3x2( m3x3 *RSTR m0, const m3x2 *m1, const m3x2 *m2 )
{
    for( uiw h = 0; h < 3; ++h )
    {
        m0->m[ h ][ 0 ] = m1->m[ h ][ 0 ] * m2->m[ 0 ][ 0 ] +
                          m1->m[ h ][ 1 ] * m2->m[ 1 ][ 0 ];

        m0->m[ h ][ 1 ] = m1->m[ h ][ 0 ] * m2->m[ 0 ][ 1 ] +
                          m1->m[ h ][ 1 ] * m2->m[ 1 ][ 1 ];
    }

    m0->m[ 2 ][ 0 ] += m2->m[ 2 ][ 0 ];
    m0->m[ 2 ][ 1 ] += m2->m[ 2 ][ 1 ];

    m0->m[ 0 ][ 2 ] = 0;
    m0->m[ 1 ][ 2 ] = 0;
    m0->m[ 2 ][ 2 ] = 1;
}

void LiceMath::M3x2Mult3x2Inplace( m3x2 *RSTR m0, const m3x2 *m1 )
{
    for( uiw h = 0; h < 3; ++h )
    {
        f32 el = m0->m[ h ][ 0 ] * m1->m[ 0 ][ 0 ] +
                 m0->m[ h ][ 1 ] * m1->m[ 1 ][ 0 ];

        m0->m[ h ][ 1 ] = m0->m[ h ][ 0 ] * m1->m[ 0 ][ 1 ] +
                          m0->m[ h ][ 1 ] * m1->m[ 1 ][ 1 ];

        m0->m[ h ][ 0 ] = el;
    }

    m0->m[ 2 ][ 0 ] += m1->m[ 2 ][ 0 ];
    m0->m[ 2 ][ 1 ] += m1->m[ 2 ][ 1 ];
}

void LiceMath::Matrix2DTo3D( m4x3 *m0, const m3x2 *m1 )
{
    m0->e00 = m1->e00; m0->e01 = m1->e01; m0->e02 = 0;
    m0->e10 = m1->e10; m0->e11 = m1->e11; m0->e12 = 0;
    m0->e20 = 0;       m0->e21 = 0;       m0->e22 = 1;
    m0->e30 = m1->e20; m0->e31 = m1->e21; m0->e32 = 0;
}

void LiceMath::Matrix3DTo2D( m3x2 *m0, const m4x3 *m1 )
{
    m0->e00 = m1->e00; m0->e01 = m1->e01;
    m0->e10 = m1->e10; m0->e11 = m1->e11;
    m0->e20 = m1->e30; m0->e21 = m1->e31;
}

void LiceMath::M2x2Identity( m2x2 *m )
{
    m->e00 = 1; m->e01 = 0;
    m->e10 = 0; m->e11 = 1;
}

bln LiceMath::M2x2IsIdentity( const m2x2 *m )
{
    return Funcs::MemEquals( m, &co_Iden2x2, sizeof(m2x2) );
}

void LiceMath::M2x2Rotate( m2x2 *m, f32 radians )
{
    m->e00 = ::cosf( radians );
    m->e01 = -::sinf( radians );

    m->e10 = ::sinf( radians );
    m->e11 = ::cosf( radians );
}

void LiceMath::M2x2RotateTransform( m2x2 *m, f32 radians )
{
    f32 cr = ::cosf( radians );
    f32 sr = ::sinf( radians );

    f32 e00 = m->e00 * cr + m->e10 * sr;
    m->e01 = m->e00 * -sr + m->e01 * cr;
    m->e00 = e00;

    f32 e10 = m->e10 * cr + m->e11 * sr;
    m->e11 = m->e10 * -sr + m->e11 * cr;
    m->e10 = e10;
}

void LiceMath::M2x2Scale( m2x2 *m, f32 x, f32 y )
{
    m->e00 = x; m->e01 = 0;
    m->e10 = 0; m->e11 = y;
}

void LiceMath::M2x2ScaleRotate( m2x2 *m, f32 x, f32 y, f32 radians )
{
    f32 cr = ::cosf( radians );
    f32 sr = ::sinf( radians );

    m->e00 = x * cr;  m->e01 = x * sr;
    m->e10 = y * -sr; m->e11 = y * cr;
}

void LiceMath::M2x2ScaleTransform( m2x2 *m, f32 x, f32 y )
{
    m->e00 *= x; m->e01 *= y;
    m->e10 *= x; m->e11 *= y;
}

void LiceMath::M2x2ShearingX( m2x2 *m, f32 x )
{
    m->e00 = 1; m->e01 = x;
    m->e10 = 0; m->e11 = 1;
}

void LiceMath::M2x2ShearingY( m2x2 *m, f32 y )
{
    m->e00 = 1; m->e01 = 0;
    m->e10 = y; m->e11 = 1;
}

void LiceMath::M2x2MultM2x2( m2x2 *RSTR m0, const m2x2 *m1, const m2x2 *m2 )
{
    m0->e00 = m1->e00 * m2->e00 + m1->e01 * m2->e10;
    m0->e01 = m1->e00 * m2->e01 + m1->e01 * m2->e11;

    m0->e10 = m1->e10 * m2->e00 + m1->e11 * m2->e10;
    m0->e11 = m1->e10 * m2->e01 + m1->e11 * m2->e11;
}

void LiceMath::M2x2MultM2x2Inplace( m2x2 *m0, const m2x2 *m1 )
{
    f32 e00 = m0->e00 * m1->e00 + m0->e01 * m1->e10;
    m0->e01 = m0->e00 * m1->e01 + m0->e01 * m1->e11;
    m0->e00 = e00;

    f32 e10 = m0->e10 * m1->e00 + m0->e11 * m1->e10;
    m0->e11 = m0->e10 * m1->e01 + m0->e11 * m1->e11;
    m0->e10 = e10;
}

void LiceMath::M2x2MultScalar( m2x2 *RSTR m0, const m2x2 *m1, f32 scalar )
{
    m0->e00 = m1->e00 * scalar; m0->e01 = m1->e01 * scalar;
    m0->e10 = m1->e10 * scalar; m0->e11 = m1->e11 * scalar;
}

void LiceMath::M2x2MultScalarInplace( m2x2 *m, f32 scalar )
{
    m->e00 *= scalar; m->e01 *= scalar;
    m->e10 *= scalar; m->e11 *= scalar;
}

void LiceMath::M2x2Transpose( m2x2 *RSTR m0, const m2x2 *m1 )
{
    m0->e00 = m1->e00; m0->e01 = m1->e10;
    m0->e10 = m1->e01; m0->e11 = m1->e11;
}

void LiceMath::M2x2TransposeInplace( m2x2 *m )
{
    Funcs::Swap( &m->e10, &m->e01 );
}

void LiceMath::Vec4MultM4x4( vec4 *RSTR v0, const vec4 *v1, const m4x4 *m )
{
    for( ui32 w = 0; w < 4; ++w )
    {
        v0->arr[ w ] = v1->arr[ 0 ] * m->m[ 0 ][ w ] +
                          v1->arr[ 1 ] * m->m[ 1 ][ w ] +
                          v1->arr[ 2 ] * m->m[ 2 ][ w ] +
                          v1->arr[ 3 ] * m->m[ 3 ][ w ] ;
    }
}

void LiceMath::Vec4MultM4x4Inplace( vec4 *v, const m4x4 *m )
{
    f32 temp[ 4 ];
    for( ui32 w = 0; w < 4; ++w )
    {
        temp[ w ] = v->arr[ 0 ] * m->m[ 0 ][ w ] +
                    v->arr[ 1 ] * m->m[ 1 ][ w ] +
                    v->arr[ 2 ] * m->m[ 2 ][ w ] +
                    v->arr[ 3 ] * m->m[ 3 ][ w ] ;
    }
    _MemCpy( v->arr, temp, sizeof(temp) );
}

f32 LiceMath::Vec4Length( const vec4 *v )
{
    return ::sqrtf( v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w );
}

f32 LiceMath::Vec4SquareLength( const vec4 *v )
{
    return v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w;
}

f32 LiceMath::Vec4Distance( const vec4 *v0, const vec4 *v1 )
{
    f32 x = v0->x - v1->x;
    f32 y = v0->y - v1->y;
    f32 z = v0->z - v1->z;
    f32 w = v0->w - v1->w;
    return ::sqrtf( x * x + y * y + z * z + w * w );
}

f32 LiceMath::Vec4SquareDistance( const vec4 *v0, const vec4 *v1 )
{
    f32 x = v0->x - v1->x;
    f32 y = v0->y - v1->y;
    f32 z = v0->z - v1->z;
    f32 w = v0->w - v1->w;
    return x * x + y * y + z * z + w * w;
}

void LiceMath::Vec4Normalize( vec4 *RSTR v0, const vec4 *v1 )
{
    //  TODO: zero vectors
    f32 revLen = 1.f / ::sqrtf( v1->x * v1->x + v1->y * v1->y + v1->z * v1->z + v1->w * v1->w );
    v0->x = v1->x * revLen;
    v0->y = v1->y * revLen;
    v0->z = v1->z * revLen;
    v0->w = v1->w * revLen;
}

void LiceMath::Vec4NormalizeInplace( vec4 *v )
{
    //  TODO: zero vectors
    f32 revLen = 1.f / ::sqrtf( v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w );
    v->x *= revLen;
    v->y *= revLen;
    v->z *= revLen;
    v->w *= revLen;
}

void LiceMath::Vec4Scale( vec4 *RSTR v0, const vec4 *v1, f32 koef )
{
    v0->x = v1->x * koef;
    v0->y = v1->y * koef;
    v0->z = v1->z * koef;
    v0->w = v1->w * koef;
}

void LiceMath::Vec4ScaleInplace( vec4 *v, f32 koef )
{
    v->x *= koef;
    v->y *= koef;
    v->z *= koef;
    v->w *= koef;
}

f32 LiceMath::Vec4Dot( const vec4 *v0, const vec4 *v1 )
{
    return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z + v0->w * v1->w;
}

void LiceMath::Vec4Cross( vec4 *RSTR v0, const vec4 *v1, const vec4 *v2, const vec4 *cpo_v3 )
{
    DBGBREAK;
/*
    v0->x = v1->y * (v2->z * cpo_v3->w - cpo_v3->z * v2->w) - v1->z * (v2->y * cpo_v3->w - cpo_v3->y * v2->w) + v1->w * (v2->y * cpo_v3->z - cpo_v3->y * v2->z);
    v0->y = v1->z * (v2->x * cpo_v3->w - cpo_v3->x * v2->w) - v1->x * (v2->z * cpo_v3->w - cpo_v3->z * v2->w) - v1->w * (v2->x * cpo_v3->z - cpo_v3->x * v2->z);
    v0->z = v1->x * (v2->y * cpo_v3->w - cpo_v3->y * v2->w) - v1->y * (v2->x *cpo_v3->w - cpo_v3->x * v2->w) + v1->w * (v2->x * cpo_v3->y - cpo_v3->x * v2->y);
    v0->w = -(v1->x * (v2->y * cpo_v3->z - cpo_v3->y * v2->z) - v1->y * (v2->x * cpo_v3->z - cpo_v3->x *v2->z) + v1->z * (v2->x * cpo_v3->y - cpo_v3->x * v2->y));
*/}

void LiceMath::Vec4CrossInplace( vec4 *RSTR v0, const vec4 *v1, const vec4 *v2 )
{
    DBGBREAK;
    /*f32 x = v0->y * v1->z - v0->z * v1->y;
    f32 y = v0->z * v1->x - v0->x * v1->z;
    v0->z = v0->x * v1->y - v0->y * v1->x;
    v0->x = x;
    v0->y = y;*/
}

void LiceMath::Vec4TransformByM4x4( vec4 *RSTR v0, const vec4 *v1, const m4x4 *m )
{
    v0->x = v1->x * m->e00 + v1->y * m->e10 + v1->z * m->e20 + v1->w * m->e30;
    v0->y = v1->x * m->e01 + v1->y * m->e11 + v1->z * m->e21 + v1->w * m->e31;
    v0->z = v1->x * m->e02 + v1->y * m->e12 + v1->z * m->e22 + v1->w * m->e32;
    v0->w = v1->x * m->e03 + v1->y * m->e13 + v1->z * m->e23 + v1->w * m->e33;
}

void LiceMath::Vec4TransformByM4x4Inplace( vec4 *v, const m4x4 *m )
{
    f32 x = v->x * m->e00 + v->y * m->e10 + v->z * m->e20 + v->w * m->e30;
    f32 y = v->x * m->e01 + v->y * m->e11 + v->z * m->e21 + v->w * m->e31;
    f32 z = v->x * m->e02 + v->y * m->e12 + v->z * m->e22 + v->w * m->e32;
    v->w = v->x * m->e03 + v->y * m->e13 + v->z * m->e23 + v->w * m->e33;
    v->x = x;
    v->y = y;
    v->z = z;
}

void LiceMath::Vec4TransformByM4x3( vec4 *RSTR v0, const vec4 *v1, const m4x3 *m )
{
    v0->x = v1->x * m->e00 + v1->y * m->e10 + v1->z * m->e20 + v1->w * m->e30;
    v0->y = v1->x * m->e01 + v1->y * m->e11 + v1->z * m->e21 + v1->w * m->e31;
    v0->z = v1->x * m->e02 + v1->y * m->e12 + v1->z * m->e22 + v1->w * m->e32;
    v0->w = v1->w;
}

void LiceMath::Vec4TransformByM4x3Inplace( vec4 *v, const m4x3 *m )
{
    f32 x = v->x * m->e00 + v->y * m->e10 + v->z * m->e20 + v->w * m->e30;
    f32 y = v->x * m->e01 + v->y * m->e11 + v->z * m->e21 + v->w * m->e31;
    f32 z = v->x * m->e02 + v->y * m->e12 + v->z * m->e22 + v->w * m->e32;
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = v->w;
}

void LiceMath::Vec4Addition( vec4 *RSTR v0, const vec4 *v1, const vec4 *v2 )
{
    v0->x = v1->x + v2->x;
    v0->y = v1->y + v2->y;
    v0->z = v1->z + v2->z;
    v0->w = v1->w + v2->w;
}

void LiceMath::Vec4AdditionInplace( vec4 *RSTR v0, const vec4 *v1 )
{
    v0->x += v1->x;
    v0->y += v1->y;
    v0->z += v1->z;
    v0->x += v1->w;
}

void LiceMath::Vec4Subtract( vec4 *RSTR v0, const vec4 *v1, const vec4 *v2 )
{
    v0->x = v1->x - v2->x;
    v0->y = v1->y - v2->y;
    v0->z = v1->z - v2->z;
    v0->w = v1->w - v2->w;
}

void LiceMath::Vec4SubtractInplace( vec4 *RSTR v0, const vec4 *v1 )
{
    v0->x -= v1->x;
    v0->y -= v1->y;
    v0->z -= v1->z;
    v0->x -= v1->w;
}

void LiceMath::Vec4Max( vec4 *RSTR v0, const vec4 *v1, const vec4 *v2 )
{
    v0->x = Funcs::Max( v1->x, v2->x );
    v0->y = Funcs::Max( v1->y, v2->y );
    v0->z = Funcs::Max( v1->z, v2->z );
    v0->w = Funcs::Max( v1->w, v2->w );
}

void LiceMath::Vec4MaxInplace( vec4 *RSTR v0, const vec4 *v1 )
{
    v0->x = Funcs::Max( v0->x, v1->x );
    v0->y = Funcs::Max( v0->y, v1->y );
    v0->z = Funcs::Max( v0->z, v1->z );
    v0->w = Funcs::Max( v0->w, v1->w );
}

void LiceMath::Vec4Min( vec4 *RSTR v0, const vec4 *v1, const vec4 *v2 )
{
    v0->x = Funcs::Min( v1->x, v2->x );
    v0->y = Funcs::Min( v1->y, v2->y );
    v0->z = Funcs::Min( v1->z, v2->z );
    v0->w = Funcs::Min( v1->w, v2->w );
}

void LiceMath::Vec4MinInplace( vec4 *RSTR v0, const vec4 *v1 )
{
    v0->x = Funcs::Min( v0->x, v1->x );
    v0->y = Funcs::Min( v0->y, v1->y );
    v0->z = Funcs::Min( v0->z, v1->z );
    v0->w = Funcs::Min( v0->w, v1->w );
}

void LiceMath::Vec4Neg( vec4 *RSTR v0, const vec4 *v1 )
{
    v0->x = -v1->x;
    v0->y = -v1->y;
    v0->z = -v1->z;
    v0->w = -v1->w;
}

void LiceMath::Vec4NegInplace( vec4 *v )
{
    v->x = -v->x;
    v->y = -v->y;
    v->z = -v->z;
    v->w = -v->w;
}

bln LiceMath::Vec4Equals( const vec4 *v0, const vec4 *v1 )
{
    return v0->x == v1->x && v0->y == v1->y && v0->z == v1->z && v0->w == v1->w;
}

bln LiceMath::Vec4EqualsByEpsilon( const vec4 *v0, const vec4 *v1, f32 epsilon )
{
    return _IsF32Equal( v0->x, v1->x, epsilon ) &&
           _IsF32Equal( v0->y, v1->y, epsilon ) &&
           _IsF32Equal( v0->z, v1->z, epsilon ) &&
           _IsF32Equal( v0->w, v1->w, epsilon )  ;
}

void LiceMath::Vec3MultM3x3( vec3 *RSTR v0, const vec3 *v1, const m3x3 *m )
{
    for( ui32 w = 0; w < 3; ++w )
    {
        v0->arr[ w ] = v1->arr[ 0 ] * m->m[ 0 ][ w ] +
                       v1->arr[ 1 ] * m->m[ 1 ][ w ] +
                       v1->arr[ 2 ] * m->m[ 2 ][ w ] ;
    }
}

void LiceMath::Vec3MultM3x3Inplace( vec3 *v, const m3x3 *m )
{
    f32 temp[ 3 ];
    for( ui32 w = 0; w < 3; ++w )
    {
        temp[ w ] = v->arr[ 0 ] * m->m[ 0 ][ w ] +
                    v->arr[ 1 ] * m->m[ 1 ][ w ] +
                    v->arr[ 2 ] * m->m[ 2 ][ w ] ;
    }
    _MemCpy( v->arr, temp, sizeof(temp) );
}

f32 LiceMath::Vec3Length( const vec3 *v )
{
    return ::sqrtf( v->x * v->x + v->y * v->y + v->z * v->z );
}

f32 LiceMath::Vec3SquareLength( const vec3 *v )
{
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

f32 LiceMath::Vec3Distance( const vec3 *v0, const vec3 *v1 )
{
    f32 x = v0->x - v1->x;
    f32 y = v0->y - v1->y;
    f32 z = v0->z - v1->z;
    return ::sqrtf( x * x + y * y + z * z );
}

f32 LiceMath::Vec3SquareDistance( const vec3 *v0, const vec3 *v1 )
{
    f32 x = v0->x - v1->x;
    f32 y = v0->y - v1->y;
    f32 z = v0->z - v1->z;
    return x * x + y * y + z * z;
}

void LiceMath::Vec3Normalize( vec3 *RSTR v0, const vec3 *v1 )
{
    //  TODO: zero vectors
    f32 revLen = 1.f / ::sqrtf( v1->x * v1->x + v1->y * v1->y + v1->z * v1->z );
    v0->x = v1->x * revLen;
    v0->y = v1->y * revLen;
    v0->z = v1->z * revLen;
}

void LiceMath::Vec3NormalizeInplace( vec3 *v )
{
    //  TODO: zero vectors
    f32 revLen = 1.f / ::sqrtf( v->x * v->x + v->y * v->y + v->z * v->z );
    v->x *= revLen;
    v->y *= revLen;
    v->z *= revLen;
}

void LiceMath::Vec3Scale( vec3 *RSTR v0, const vec3 *v1, f32 koef )
{
    v0->x = v1->x * koef;
    v0->y = v1->y * koef;
    v0->z = v1->z * koef;
}

void LiceMath::Vec3ScaleInplace( vec3 *v, f32 koef )
{
    v->x *= koef;
    v->y *= koef;
    v->z *= koef;
}

f32 LiceMath::Vec3Dot( const vec3 *v0, const vec3 *v1 )
{
    return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z;
}

void LiceMath::Vec3Cross( vec3 *RSTR v0, const vec3 *v1, const vec3 *v2 )
{
    v0->x = v1->y * v2->z - v1->z * v2->y;
    v0->y = v1->z * v2->x - v1->x * v2->z;
    v0->z = v1->x * v2->y - v1->y * v2->x;
}

void LiceMath::Vec3CrossInplace( vec3 *RSTR v0, const vec3 *v1 )
{
    f32 x = v0->y * v1->z - v0->z * v1->y;
    f32 y = v0->z * v1->x - v0->x * v1->z;
    v0->z = v0->x * v1->y - v0->y * v1->x;
    v0->x = x;
    v0->y = y;
}

void LiceMath::Vec3TransformByM4x4( vec4 *RSTR v0, const vec3 *v1, const m4x4 *m )
{
    v0->x = v1->x * m->e00 + v1->y * m->e10 + v1->z * m->e20 + m->e30;
    v0->y = v1->x * m->e01 + v1->y * m->e11 + v1->z * m->e21 + m->e31;
    v0->z = v1->x * m->e02 + v1->y * m->e12 + v1->z * m->e22 + m->e32;
    v0->w = v1->x * m->e03 + v1->y * m->e13 + v1->z * m->e23 + m->e33;
}

void LiceMath::Vec3TransformCoordByM4x4( vec3 *RSTR v0, const vec3 *v1, const m4x4 *m )
{
    //  THERE IS NO CHECKING FOR norm == 0 CASE
    f32 norm = v1->x * m->e03 + v1->y * m->e13 + v1->z * m->e23 + m->e33;
    norm = 1.f / norm;
    v0->x = v1->x * m->e00 + v1->y * m->e10 + v1->z * m->e20 + m->e30 * norm;
    v0->y = v1->x * m->e01 + v1->y * m->e11 + v1->z * m->e21 + m->e31 * norm;
    v0->z = v1->x * m->e02 + v1->y * m->e12 + v1->z * m->e22 + m->e32 * norm;
}

void LiceMath::Vec3TransformCoordByM4x4Inplace( vec3 *v, const m4x4 *m )
{
    //  THERE IS NO CHECKING FOR norm == 0 CASE
    f32 norm = v->x * m->e03 + v->y * m->e13 + v->z * m->e23 + m->e33;
    norm = 1.f / norm;
    f32 x = v->x * m->e00 + v->y * m->e10 + v->z * m->e20 + m->e30 * norm;
    f32 y = v->x * m->e01 + v->y * m->e11 + v->z * m->e21 + m->e31 * norm;
    v->z = v->x * m->e02 + v->y * m->e12 + v->z * m->e22 + m->e32 * norm;
    v->y = y;
    v->x = x;
}

void LiceMath::Vec3TransformCoordByM4x4LastIden( vec3 *RSTR v0, const vec3 *v1, const m4x4 *m )
{
    v0->x = v1->x * m->e00 + v1->y * m->e10 + v1->z * m->e20 + m->e30;
    v0->y = v1->x * m->e01 + v1->y * m->e11 + v1->z * m->e21 + m->e31;
    v0->z = v1->x * m->e02 + v1->y * m->e12 + v1->z * m->e22 + m->e32;
}

void LiceMath::Vec3TransformCoordByM4x4LastIdenInplace( vec3 *v, const m4x4 *m )
{
    f32 x = v->x * m->e00 + v->y * m->e10 + v->z * m->e20 + m->e30;
    f32 y = v->x * m->e01 + v->y * m->e11 + v->z * m->e21 + m->e31;
    v->z = v->x * m->e02 + v->y * m->e12 + v->z * m->e22 + m->e32;
    v->y = y;
    v->x = x;
}

void LiceMath::Vec3TransformNormalByM4x4( vec3 *RSTR v0, const vec3 *v1, const m4x4 *m )
{
    v0->x = v1->x * m->e00 + v1->y * m->e10 + v1->z * m->e20;
    v0->y = v1->x * m->e01 + v1->y * m->e11 + v1->z * m->e21;
    v0->z = v1->x * m->e02 + v1->y * m->e12 + v1->z * m->e22;
}

void LiceMath::Vec3TransformNormalByM4x4Inplace( vec3 *v, const m4x4 *m )
{
    f32 x = v->x * m->e00 + v->y * m->e10 + v->z * m->e20;
    f32 y = v->x * m->e01 + v->y * m->e11 + v->z * m->e21;
    v->z = v->x * m->e02 + v->y * m->e12 + v->z * m->e22;
    v->y = y;
    v->x = x;
}

void LiceMath::Vec3TransformByM4x3( vec4 *RSTR v0, const vec3 *v1, const m4x3 *m )
{
    v0->x = v1->x * m->e00 + v1->y * m->e10 + v1->z * m->e20 + m->e30;
    v0->y = v1->x * m->e01 + v1->y * m->e11 + v1->z * m->e21 + m->e31;
    v0->z = v1->x * m->e02 + v1->y * m->e12 + v1->z * m->e22 + m->e32;
    v0->w = 1.f;
}

void LiceMath::Vec3TransformCoordByM4x3( vec3 *RSTR v0, const vec3 *v1, const m4x3 *m )
{
    v0->x = v1->x * m->e00 + v1->y * m->e10 + v1->z * m->e20 + m->e30;
    v0->y = v1->x * m->e01 + v1->y * m->e11 + v1->z * m->e21 + m->e31;
    v0->z = v1->x * m->e02 + v1->y * m->e12 + v1->z * m->e22 + m->e32;
}

void LiceMath::Vec3TransformCoordByM4x3Inplace( vec3 *v, const m4x3 *m )
{
    f32 x = v->x * m->e00 + v->y * m->e10 + v->z * m->e20 + m->e30;
    f32 y = v->x * m->e01 + v->y * m->e11 + v->z * m->e21 + m->e31;
    v->z = v->x * m->e02 + v->y * m->e12 + v->z * m->e22 + m->e32;
    v->y = y;
    v->x = x;
}

void LiceMath::Vec3TransformCoordByM4x3LastIden( vec3 *RSTR v0, const vec3 *v1, const m4x3 *m )
{
    v0->x = v1->x * m->e00 + v1->y * m->e10 + v1->z * m->e20 + m->e30;
    v0->y = v1->x * m->e01 + v1->y * m->e11 + v1->z * m->e21 + m->e31;
    v0->z = v1->x * m->e02 + v1->y * m->e12 + v1->z * m->e22 + m->e32;
}

void LiceMath::Vec3TransformCoordByM4x3LastIdenInplace( vec3 *v, const m4x3 *m )
{
    f32 x = v->x * m->e00 + v->y * m->e10 + v->z * m->e20 + m->e30;
    f32 y = v->x * m->e01 + v->y * m->e11 + v->z * m->e21 + m->e31;
    v->z = v->x * m->e02 + v->y * m->e12 + v->z * m->e22 + m->e32;
    v->y = y;
    v->x = x;
}

void LiceMath::Vec3TransformNormalByM4x3( vec3 *RSTR v0, const vec3 *v1, const m4x3 *m )
{
    v0->x = v1->x * m->e00 + v1->y * m->e10 + v1->z * m->e20;
    v0->y = v1->x * m->e01 + v1->y * m->e11 + v1->z * m->e21;
    v0->z = v1->x * m->e02 + v1->y * m->e12 + v1->z * m->e22;
}

void LiceMath::Vec3TransformNormalByM4x3Inplace( vec3 *v, const m4x3 *m )
{
    f32 x = v->x * m->e00 + v->y * m->e10 + v->z * m->e20;
    f32 y = v->x * m->e01 + v->y * m->e11 + v->z * m->e21;
    v->z = v->x * m->e02 + v->y * m->e12 + v->z * m->e22;
    v->y = y;
    v->x = x;
}

void LiceMath::Vec3TransformByM3x3( vec3 *RSTR v0, const vec3 *v1, const m3x3 *m )
{
    v0->x = v1->x * m->e00 + v1->y * m->e10 + v1->z * m->e20;
    v0->y = v1->x * m->e01 + v1->y * m->e11 + v1->z * m->e21;
    v0->z = v1->x * m->e02 + v1->y * m->e12 + v1->z * m->e22;
}

void LiceMath::Vec3Addition( vec3 *RSTR v0, const vec3 *v1, const vec3 *v2 )
{
    v0->x = v1->x + v2->x;
    v0->y = v1->y + v2->y;
    v0->z = v1->z + v2->z;
}

void LiceMath::Vec3AdditionInplace( vec3 *RSTR v0, const vec3 *v1 )
{
    v0->x += v1->x;
    v0->y += v1->y;
    v0->z += v1->z;
}

void LiceMath::Vec3Subtract( vec3 *RSTR v0, const vec3 *v1, const vec3 *v2 )
{
    v0->x = v1->x - v2->x;
    v0->y = v1->y - v2->y;
    v0->z = v1->z - v2->z;
}

void LiceMath::Vec3SubtractInplace( vec3 *RSTR v0, const vec3 *v1 )
{
    v0->x -= v1->x;
    v0->y -= v1->y;
    v0->z -= v1->z;
}

void LiceMath::Vec3Max( vec3 *RSTR v0, const vec3 *v1, const vec3 *v2 )
{
    v0->x = Funcs::Max( v1->x, v2->x );
    v0->y = Funcs::Max( v1->y, v2->y );
    v0->z = Funcs::Max( v1->z, v2->z );
}

void LiceMath::Vec3MaxInplace( vec3 *RSTR v0, const vec3 *v1 )
{
    v0->x = Funcs::Max( v0->x, v1->x );
    v0->y = Funcs::Max( v0->y, v1->y );
    v0->z = Funcs::Max( v0->z, v1->z );
}

void LiceMath::Vec3Min( vec3 *RSTR v0, const vec3 *v1, const vec3 *v2 )
{
    v0->x = Funcs::Min( v1->x, v2->x );
    v0->y = Funcs::Min( v1->y, v2->y );
    v0->z = Funcs::Min( v1->z, v2->z );
}

void LiceMath::Vec3MinInplace( vec3 *RSTR v0, const vec3 *v1 )
{
    v0->x = Funcs::Min( v0->x, v1->x );
    v0->y = Funcs::Min( v0->y, v1->y );
    v0->z = Funcs::Min( v0->z, v1->z );
}

void LiceMath::Vec3Neg( vec3 *RSTR v0, const vec3 *v1 )
{
    v0->x = -v1->x;
    v0->y = -v1->y;
    v0->z = -v1->z;
}

void LiceMath::Vec3NegInplace( vec3 *v )
{
    v->x = -v->x;
    v->y = -v->y;
    v->z = -v->z;
}

bln LiceMath::Vec3Equals( const vec3 *v0, const vec3 *v1 )
{
    return v0->x == v1->x && v0->y == v1->y && v0->z == v1->z;
}

bln LiceMath::Vec3EqualsByEpsilon( const vec3 *v0, const vec3 *v1, f32 epsilon )
{
    return _IsF32Equal( v0->x, v1->x, epsilon ) &&
           _IsF32Equal( v0->y, v1->y, epsilon ) &&
           _IsF32Equal( v0->z, v1->z, epsilon )  ;
}

f32 LiceMath::Vec2Length( const vec2 *v )
{
    return ::sqrtf( v->x * v->x + v->y * v->y );
}

f32 LiceMath::Vec2SquareLength( const vec2 *v )
{
    return v->x * v->x + v->y * v->y;
}

f32 LiceMath::Vec2Distance( const vec2 *v0, const vec2 *v1 )
{
    f32 x = v0->x - v1->x;
    f32 y = v0->y - v1->y;
    return ::sqrtf( x * x + y * y );
}

f32 LiceMath::Vec2SquareDistance( const vec2 *v0, const vec2 *v1 )
{
    f32 x = v0->x - v1->x;
    f32 y = v0->y - v1->y;
    return x * x + y * y;
}

void LiceMath::Vec2Normalize( vec2 *RSTR v0, const vec2 *v1 )
{
    //  TODO: zero vectors
    f32 revLen = 1.f / ::sqrtf( v1->x * v1->x + v1->y * v1->y );
    v0->x = v1->x * revLen;
    v0->y = v1->y * revLen;
}

void LiceMath::Vec2NormalizeInplace( vec2 *v )
{
    //  TODO: zero vectors
    f32 revLen = 1.f / ::sqrtf( v->x * v->x + v->y * v->y );
    v->x *= revLen;
    v->y *= revLen;
}

void LiceMath::Vec2Scale( vec2 *RSTR v0, const vec2 *v1, f32 koef )
{
    v0->x = v1->x * koef;
    v0->y = v1->y * koef;
}

void LiceMath::Vec2ScaleInplace( vec2 *v, f32 koef )
{
    v->x *= koef;
    v->y *= koef;
}

f32 LiceMath::Vec2Dot( const vec2 *v0, const vec2 *v1 )
{
    return v0->x * v1->x + v0->y * v1->y;
}

void LiceMath::Vec2TransformByM2x2( vec2 *RSTR v0, const vec2 *v1, const M2x2 *m )
{
    v0->x = v1->x * m->e00 + v1->y * m->e10;
    v0->y = v1->x * m->e01 + v1->y * m->e11;
}

void LiceMath::Vec2TransformByM2x2Inplace( vec2 *v, const M2x2 *m )
{
    f32 x = v->x * m->e00 + v->y * m->e10;
    v->y = v->x * m->e01 + v->y * m->e11;
    v->x = x;
}

void LiceMath::Vec2Addition( vec2 *RSTR v0, const vec2 *v1, const vec2 *v2 )
{
    v0->x = v1->x + v2->x;
    v0->y = v1->y + v2->y;
}

void LiceMath::Vec2AdditionInplace( vec2 *RSTR v0, const vec2 *v1 )
{
    v0->x += v1->x;
    v0->y += v1->y;
}

void LiceMath::Vec2Subtract( vec2 *RSTR v0, const vec2 *v1, const vec2 *v2 )
{
    v0->x = v1->x - v2->x;
    v0->y = v1->y - v2->y;
}

void LiceMath::Vec2SubtractInplace( vec2 *RSTR v0, const vec2 *v1 )
{
    v0->x -= v1->x;
    v0->y -= v1->y;
}

void LiceMath::Vec2Max( vec2 *RSTR v0, const vec2 *v1, const vec2 *v2 )
{
    v0->x = Funcs::Max( v1->x, v2->x );
    v0->y = Funcs::Max( v1->y, v2->y );
}

void LiceMath::Vec2MaxInplace( vec2 *RSTR v0, const vec2 *v1 )
{
    v0->x = Funcs::Max( v0->x, v1->x );
    v0->y = Funcs::Max( v0->y, v1->y );
}

void LiceMath::Vec2Min( vec2 *RSTR v0, const vec2 *v1, const vec2 *v2 )
{
    v0->x = Funcs::Min( v1->x, v2->x );
    v0->y = Funcs::Min( v1->y, v2->y );
}

void LiceMath::Vec2MinInplace( vec2 *RSTR v0, const vec2 *v1 )
{
    v0->x = Funcs::Min( v0->x, v1->x );
    v0->y = Funcs::Min( v0->y, v1->y );
}

void LiceMath::Vec2Neg( vec2 *RSTR v0, const vec2 *v1 )
{
    v0->x = -v1->x;
    v0->y = -v1->y;
}

void LiceMath::Vec2NegInplace( vec2 *v )
{
    v->x = -v->x;
    v->y = -v->y;
}

bln LiceMath::Vec2Equals( const vec2 *v0, const vec2 *v1 )
{
    return v0->x == v1->x && v0->y == v1->y;
}

bln LiceMath::Vec2EqualsByEpsilon( const vec2 *v0, const vec2 *v1, f32 epsilon )
{
    return _IsF32Equal( v0->x, v1->x, epsilon ) &&
           _IsF32Equal( v0->y, v1->y, epsilon ) ;
}

f32 LiceMath::PlaneDotCoord( const plane *cpo_plane, const vec3 *cpo_coord )
{
    return cpo_plane->a * cpo_coord->x + cpo_plane->b * cpo_coord->y + cpo_plane->c * cpo_coord->z + cpo_plane->d;
}

void LiceMath::PlaneNormalize( plane *RSTR plane0, const plane *plane1 )
{
    f32 len = ::sqrtf( plane1->a * plane1->a + plane1->b * plane1->b + plane1->c * plane1->c );
    plane0->a = plane1->a * len;
    plane0->b = plane1->b * len;
    plane0->c = plane1->c * len;
    plane0->d = plane1->d * len;
}

void LiceMath::PlaneNormalizeInplace( plane *plane )
{
    //  TODO: zero planes
    f32 len = 1.f / ::sqrtf( plane->a * plane->a + plane->b * plane->b + plane->c * plane->c );
    plane->a *= len;
    plane->b *= len;
    plane->c *= len;
    plane->d *= len;
}

void LiceMath::Projection( m4x4 *m, f32 fovDegree, f32 nearPlane, f32 farPlane, f32 aspect )
{
    f32 fovH = DEGREETORADIAN( fovDegree );
    f32 fovV = 2.f * ::atanf( ::tanf( fovH / 2.f ) / aspect );
    f32 q = farPlane / (farPlane - nearPlane);
    f32 w = 1.f / ::tanf( fovH * 0.5f );
    f32 h = 1.f / ::tanf( fovV * 0.5f );
    f32 l = -q * nearPlane;

    m->e00 = w; m->e01 = 0; m->e02 = 0; m->e03 = 0;
    m->e10 = 0; m->e11 = h; m->e12 = 0; m->e13 = 0;
    m->e20 = 0; m->e21 = 0; m->e22 = q; m->e23 = 1;
    m->e30 = 0; m->e31 = 0; m->e32 = l; m->e33 = 0;
}

bln LiceMath::BSCrossTest3D( const vec3 *cpo_center0, f32 radius0, const vec3 *cpo_center1, f32 radius1 )
{
    vec3 o_dir;
    Vec3Subtract( &o_dir, cpo_center0, cpo_center1 );
    f32 len = Vec3Length( &o_dir );
    return len <= (radius0 + radius1);
}