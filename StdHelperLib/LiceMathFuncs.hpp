#ifndef __LICE_MATH_FUNCS_HPP__
#define __LICE_MATH_FUNCS_HPP__

#include "LiceMathTypes.hpp"

namespace StdLib {

/*  LastIden for m4x4 means matrix with these values: e03 = 0, e13 = 0, e23 = 0, e33 = 1  */

namespace LiceMath
{
	f128color ColorI32WebToF128( colori32web color );
    EXTERNALS void ColorI32WebToF128Placed( colori32web color, f128color *po_out );
	colori32web ColorF128ToI32Web( const f128color *cpo_color );

	f96color ColorI32WebToF96( colori32web color );
    EXTERNALS void ColorI32WebToF96Placed( colori32web color, f96color *po_out );
	colori32web ColorF96ToI32Web( const f96color *cpo_color );

    EXTERNALS void M4x4RotateX( m4x4 *po_m, f32 x );
    EXTERNALS void M4x4RotateXDoNotIdentity( m4x4 *po_m, f32 x );
    EXTERNALS void M4x4RotateY( m4x4 *po_m, f32 y );
    EXTERNALS void M4x4RotateYDoNotIdentity( m4x4 *po_m, f32 y );
    EXTERNALS void M4x4RotateZ( m4x4 *po_m, f32 z );
    EXTERNALS void M4x4RotateZDoNotIdentity( m4x4 *po_m, f32 z );
    EXTERNALS void M4x4RotateXYZ( m4x4 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x4RotateXYZDoNotIdentity( m4x4 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x4RotateXYZTransformLastIden( m4x4 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x4RotateXY( m4x4 *po_m, f32 x, f32 y );
    EXTERNALS void M4x4RotateXYDoNotIdentity( m4x4 *po_m, f32 x, f32 y );
    EXTERNALS void M4x4RotateXYTransformLastIden( m4x4 *po_m, f32 x, f32 y );
    EXTERNALS void M4x4RotateAxis( m4x4 *po_m, const vec3 *cpo_v, f32 angle );  //  input vector must be normalized
    EXTERNALS void M4x4RotateAxisDoNotIdentity( m4x4 *po_m, const vec3 *cpo_v, f32 angle );  //  input vector must be normalized
    EXTERNALS void M4x4Identity( m4x4 *po_m );
	EXTERNALS bln M4x4IsIdentity( const m4x4 *cpo_m );  //  without epsilon
	EXTERNALS bln M4x4IsLastIdentity( const m4x4 *cpo_m );  //  without epsilon
    EXTERNALS void M4x4Transpose( m4x4 *RSTR po_m0, const m4x4 *cpo_m1 );
    EXTERNALS void M4x4TransposeInplace( m4x4 *po_m );
    EXTERNALS void M4x4MultM4x4( m4x4 *RSTR po_m0, const m4x4 *cpo_m1, const m4x4 *cpo_m2 );
    EXTERNALS void M4x4MultM4x4Inplace( m4x4 *RSTR po_m0, const m4x4 *cpo_m1 );
    EXTERNALS void M4x4MultScalar( m4x4 *RSTR po_m0, const m4x4 *cpo_m1, f32 scalar );
    EXTERNALS void M4x4MultScalarInplace( m4x4 *RSTR po_m, f32 scalar );
    EXTERNALS void M4x4Translate3D( m4x4 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x4Translate3DTransformLastIden( m4x4 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x4Translate3DDoNotIdentity( m4x4 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x4Translate2D( m4x4 *po_m, f32 x, f32 y );
    EXTERNALS void M4x4Translate2DTransformLastIden( m4x4 *po_m, f32 x, f32 y );
    EXTERNALS void M4x4Translate2DDoNotIdentity( m4x4 *po_m, f32 x, f32 y );
    EXTERNALS void M4x4Scale3D( m4x4 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x4Scale3DTransformLastIden( m4x4 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x4Scale3DDoNotIdentity( m4x4 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x4Scale2D( m4x4 *po_m, f32 x, f32 y );
    EXTERNALS void M4x4Scale2DTransformLastIden( m4x4 *po_m, f32 x, f32 y );
    EXTERNALS void M4x4Scale2DDoNotIdentity( m4x4 *po_m, f32 x, f32 y );
    EXTERNALS void M4x4Resize( m4x4 *po_m, f32 koef );
    EXTERNALS void M4x4ResizeTransformLastIden( m4x4 *po_m, f32 koef );
    EXTERNALS void M4x4ResizeDoNotIdentity( m4x4 *po_m, f32 koef );
    EXTERNALS void M4x4Addition( m4x4 *RSTR po_m0, const m4x4 *cpo_m1, const m4x4 *cpo_m2 );
    EXTERNALS void M4x4AdditionInplace( m4x4 *RSTR po_m0, const m4x4 *cpo_m1 );
    EXTERNALS void M4x4Subtract( m4x4 *RSTR po_m0, const m4x4 *cpo_m1, const m4x4 *cpo_m2 );
    EXTERNALS void M4x4SubtractInplace( m4x4 *RSTR po_m0, const m4x4 *cpo_m1 );
    EXTERNALS void M4x4Determinant( const m4x4 *cpo_m );  //  incomplete

    EXTERNALS void M4x3RotateX( m4x3 *po_m, f32 x );
    EXTERNALS void M4x3RotateXDoNotIdentity( m4x3 *po_m, f32 x );
    EXTERNALS void M4x3RotateY( m4x3 *po_m, f32 y );
    EXTERNALS void M4x3RotateYDoNotIdentity( m4x3 *po_m, f32 y );
    EXTERNALS void M4x3RotateZ( m4x3 *po_m, f32 z );
    EXTERNALS void M4x3RotateZDoNotIdentity( m4x3 *po_m, f32 z );
    EXTERNALS void M4x3RotateXYZ( m4x3 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x3RotateXYZDoNotIdentity( m4x3 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x3RotateXYZTransform( m4x3 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x3RotateXYZTranslate3DVec( m4x3 *po_m, const vec3 *cpo_rot, const vec3 *cpo_trans );
    EXTERNALS void M4x3RotateXYZTranslate3D( m4x3 *po_m, f32 rx, f32 ry, f32 rz, f32 tx, f32 ty, f32 tz );
    EXTERNALS void M4x3RotateXY( m4x3 *po_m, f32 x, f32 y );
    EXTERNALS void M4x3RotateXYDoNotIdentity( m4x3 *po_m, f32 x, f32 y );
    EXTERNALS void M4x3RotateXYTransform( m4x3 *RSTR po_m0, const m4x3 *cpo_m1, f32 x, f32 y );
    EXTERNALS void M4x3RotateXYTransformInplace( m4x3 *po_m, f32 x, f32 y );
    EXTERNALS void M4x3RotateAxis( m4x3 *po_m, const vec3 *cpo_v, f32 angle );  //  input vector must be normalized
    EXTERNALS void M4x3RotateAxisDoNotIdentity( m4x3 *po_m, const vec3 *cpo_v, f32 angle );  //  input vector must be normalized
    EXTERNALS void M4x3Identity( m4x3 *po_m );
	EXTERNALS bln M4x3IsIdentity( const m4x3 *cpo_m );  //  without epsilon
	EXTERNALS void M4x3Transpose( m3x4 *po_m0, const m4x3 *cpo_m1 );
	EXTERNALS m3x4 *M4x3TransposeInplaceAs3x4( m4x3 *po_m0 );  //  m4x3 will become m3x4
    EXTERNALS void M4x3MultM4x3AsM4x4LastIden( m4x3 *RSTR po_m0, const m4x3 *cpo_m1, const m4x3 *cpo_m2 );
    EXTERNALS void M4x3MultM4x3AsM4x4LastIdenInplace( m4x3 *RSTR po_m0, const m4x3 *cpo_m1 );
    EXTERNALS void M4x3AsM4x4LastIdenMultM4x4( m4x4 *RSTR po_m0, const m4x3 *cpo_m1, const m4x4 *cpo_m2 );
    EXTERNALS void M4x3MultScalar( m4x3 *RSTR po_m0, const m4x3 *cpo_m1, f32 scalar );
    EXTERNALS void M4x3MultScalarInplace( m4x3 *RSTR po_m, f32 scalar );
    EXTERNALS void M4x3Translate3D( m4x3 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x3Translate3DTransform( m4x3 *RSTR po_m0, const m4x3 *cpo_m1, f32 x, f32 y, f32 z );
    EXTERNALS void M4x3Translate3DTransformInplace( m4x3 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x3Translate3DDoNotIdentity( m4x3 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x3Translate2D( m4x3 *po_m, f32 x, f32 y );
    EXTERNALS void M4x3Translate2DTransform( m4x3 *po_m0, const m4x3 *cpo_m1, f32 x, f32 y );
    EXTERNALS void M4x3Translate2DTransformInplace( m4x3 *po_m, f32 x, f32 y );
    EXTERNALS void M4x3Translate2DDoNotIdentity( m4x3 *po_m, f32 x, f32 y );
    EXTERNALS void M4x3Scale3D( m4x3 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x3Scale3DRotateXYZVec( m4x3 *po_m, const vec3 *cpo_scale, const vec3 *cpo_rot );
    EXTERNALS void M4x3Scale3DRotateXYZ( m4x3 *po_m, f32 sx, f32 sy, f32 sz, f32 rx, f32 ry, f32 rz );
    EXTERNALS void M4x3Scale3DRotateXYZTranslate3DVec( m4x3 *po_m, const vec3 *cpo_scale, const vec3 *cpo_rot, const vec3 *cpo_trans );
    EXTERNALS void M4x3Scale3DRotateXYZTranslate3D( m4x3 *po_m, f32 sx, f32 sy, f32 sz, f32 rx, f32 ry, f32 rz, f32 tx, f32 ty, f32 tz );
    EXTERNALS void M4x3Scale3DTransform( m4x3 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x3Scale3DDoNotIdentity( m4x3 *po_m, f32 x, f32 y, f32 z );
    EXTERNALS void M4x3Scale2D( m4x3 *po_m, f32 x, f32 y );
    EXTERNALS void M4x3Scale2DTransform( m4x3 *po_m, f32 x, f32 y );
    EXTERNALS void M4x3Scale2DDoNotIdentity( m4x3 *po_m, f32 x, f32 y );
    EXTERNALS void M4x3Resize( m4x3 *po_m, f32 koef );
    EXTERNALS void M4x3ResizeTransform( m4x3 *po_m, f32 koef );
    EXTERNALS void M4x3ResizeDoNotIdentity( m4x3 *po_m, f32 koef );
    EXTERNALS void M4x3Addition( m4x3 *RSTR po_m0, const m4x3 *cpo_m1, const m4x3 *cpo_m2 );
    EXTERNALS void M4x3AdditionInplace( m4x3 *RSTR po_m0, const m4x3 *cpo_m1 );
    EXTERNALS void M4x3Subtract( m4x3 *RSTR po_m0, const m4x3 *cpo_m1, const m4x3 *cpo_m2 );
    EXTERNALS void M4x3SubtractInplace( m4x3 *RSTR po_m0, const m4x3 *cpo_m1 );
    EXTERNALS void M4x3Determinant( const m4x3 *cpo_m );  //  TODO: incomplete

    EXTERNALS void M3x4MultScalar( m3x4 *RSTR po_m0, const m3x4 *cpo_m1, f32 scalar );
    EXTERNALS void M3x4MultScalarInplace( m3x4 *RSTR po_m, f32 scalar );

    EXTERNALS void M3x3Identity( m3x3 *po_m );
	EXTERNALS bln M3x3IsIdentity( const m3x3 *cpo_m );  //  without epsilon
    EXTERNALS void M3x3Transpose( m3x3 *RSTR po_m0, const m3x3 *cpo_m1 );
    EXTERNALS void M3x3TransposeInplace( m3x3 *po_m );
    EXTERNALS void M3x3MultM3x3( m3x3 *RSTR po_m0, const m3x3 *cpo_m1, const m3x3 *cpo_m2 );
    EXTERNALS void M3x3MultM3x3Inplace( m3x3 *RSTR po_m0, const m3x3 *cpo_m1 );
    EXTERNALS void M3x3MultScalar( m3x3 *RSTR po_m0, const m3x3 *cpo_m1, f32 scalar );
    EXTERNALS void M3x3MultScalarInplace( m3x3 *RSTR po_m0, f32 scalar );
    EXTERNALS void M3x3Addition( m3x3 *RSTR po_m0, const m3x3 *cpo_m1, const m3x3 *cpo_m2 );
    EXTERNALS void M3x3AdditionInplace( m3x3 *RSTR po_m0, const m3x3 *cpo_m1 );
    EXTERNALS void M3x3Subtract( m3x3 *RSTR po_m0, const m3x3 *cpo_m1, const m3x3 *cpo_m2 );
    EXTERNALS void M3x3SubtractInplace( m3x3 *RSTR po_m0, const m3x3 *cpo_m1 );
    EXTERNALS void M3x3RotateAxis( m3x3 *po_m, const vec3 *cpo_v, f32 angle );  //  input vector must be normalized
    EXTERNALS void M3x3RotateX( m3x3 *po_m, f32 x );
    EXTERNALS void M3x3RotateY( m3x3 *po_m, f32 y );
    EXTERNALS void M3x3RotateZ( m3x3 *po_m, f32 z );
    EXTERNALS void M3x3RotateXYZ( m3x3 *po_m, f32 x, f32 y, f32 z );

    EXTERNALS void M3x2Identity( m3x2 *po_m );
    EXTERNALS void M3x2Transpose( m2x3 *RSTR po_m0, const m3x2 *cpo_m1 );
    EXTERNALS void M3x2Translate( m3x2 *po_m, f32 x, f32 y );
    EXTERNALS void M3x2RotateZ( m3x2 *po_m, f32 rot );
    EXTERNALS void M3x2Scale( m3x2 *po_m, f32 x, f32 y );
    EXTERNALS void M3x2ScaleRotateZ( m3x2 *po_m, f32 x, f32 y, f32 rot );
    EXTERNALS void M3x2ScaleRotateZTranslate( m3x2 *po_m, f32 sx, f32 sy, f32 rot, f32 x, f32 y );
    EXTERNALS void M3x2Mult3x2( m3x2 *RSTR po_m0, const m3x2 *cpo_m1, const m3x2 *cpo_m2 );
    EXTERNALS void M3x2Mult3x2ToM3x3( m3x3 *RSTR po_m0, const m3x2 *cpo_m1, const m3x2 *cpo_m2 );
    EXTERNALS void M3x2Mult3x2Inplace( m3x2 *RSTR po_m0, const m3x2 *cpo_m1 );

    EXTERNALS void Matrix2DTo3D( m4x3 *po_m0, const m3x2 *cpo_m1 );
    EXTERNALS void Matrix3DTo2D( m3x2 *po_m0, const m4x3 *cpo_m1 );

    EXTERNALS void M2x2Identity( m2x2 *po_m );
	EXTERNALS bln M2x2IsIdentity( const m2x2 *cpo_m );  //  without epsilon
    EXTERNALS void M2x2Rotate( m2x2 *po_m, f32 radians );
    EXTERNALS void M2x2RotateTransform( m2x2 *po_m, f32 radians );
    EXTERNALS void M2x2Scale( m2x2 *po_m, f32 x, f32 y );
    EXTERNALS void M2x2ScaleRotate( m2x2 *po_m, f32 x, f32 y, f32 radians );
    EXTERNALS void M2x2ScaleTransform( m2x2 *po_m, f32 x, f32 y );
    EXTERNALS void M2x2ShearingX( m2x2 *po_m, f32 x );
    EXTERNALS void M2x2ShearingY( m2x2 *po_m, f32 y );
    EXTERNALS void M2x2MultM2x2( m2x2 *RSTR po_m0, const m2x2 *cpo_m1, const m2x2 *cpo_m2 );
    EXTERNALS void M2x2MultM2x2Inplace( m2x2 *po_m0, const m2x2 *cpo_m1 );
    EXTERNALS void M2x2MultScalar( m2x2 *RSTR po_m0, const m2x2 *cpo_m1, f32 scalar );
    EXTERNALS void M2x2MultScalarInplace( m2x2 *po_m, f32 scalar );
    EXTERNALS void M2x2Transpose( m2x2 *RSTR po_m0, const m2x2 *cpo_m1 );
    EXTERNALS void M2x2TransposeInplace( m2x2 *po_m );

    EXTERNALS void Vec4MultM4x4( vec4 *RSTR po_v0, const vec4 *cpo_v1, const m4x4 *cpo_m );
    EXTERNALS void Vec4MultM4x4Inplace( vec4 *po_v, const m4x4 *cpo_m );
	EXTERNALS f32 Vec4Length( const vec4 *cpo_v );
	EXTERNALS f32 Vec4SquareLength( const vec4 *cpo_v );
	EXTERNALS f32 Vec4Distance( const vec4 *cpo_v0, const vec4 *cpo_v1 );
	EXTERNALS f32 Vec4SquareDistance( const vec4 *cpo_v0, const vec4 *cpo_v1 );
    EXTERNALS void Vec4Normalize( vec4 *RSTR po_v0, const vec4 *cpo_v1 );
    EXTERNALS void Vec4NormalizeInplace( vec4 *po_v );
    EXTERNALS void Vec4Scale( vec4 *RSTR po_v0, const vec4 *cpo_v1, f32 koef );
    EXTERNALS void Vec4ScaleInplace( vec4 *po_v, f32 koef );
	EXTERNALS f32 Vec4Dot( const vec4 *cpo_v0, const vec4 *cpo_v1 );
    EXTERNALS void Vec4Cross( vec4 *RSTR po_v0, const vec4 *cpo_v1, const vec4 *cpo_v2, const vec4 *cpo_v3 );  //  TODO: does not complete
    EXTERNALS void Vec4CrossInplace( vec4 *RSTR po_v0, const vec4 *cpo_v1, const vec4 *cpo_v2 );  //  TODO: does not complete
    EXTERNALS void Vec4TransformByM4x4( vec4 *RSTR po_v0, const vec4 *cpo_v1, const m4x4 *cpo_m );
    EXTERNALS void Vec4TransformByM4x4Inplace( vec4 *po_v, const m4x4 *cpo_m );
    EXTERNALS void Vec4TransformByM4x3( vec4 *RSTR po_v0, const vec4 *cpo_v1, const m4x3 *cpo_m );
    EXTERNALS void Vec4TransformByM4x3Inplace( vec4 *po_v, const m4x3 *cpo_m );
    EXTERNALS void Vec4Addition( vec4 *RSTR po_v0, const vec4 *cpo_v1, const vec4 *cpo_v2 );
    EXTERNALS void Vec4AdditionInplace( vec4 *RSTR po_v0, const vec4 *cpo_v1 );
    EXTERNALS void Vec4Subtract( vec4 *RSTR po_v0, const vec4 *cpo_v1, const vec4 *cpo_v2 );
    EXTERNALS void Vec4SubtractInplace( vec4 *RSTR po_v0, const vec4 *cpo_v1 );
    EXTERNALS void Vec4Max( vec4 *RSTR po_v0, const vec4 *cpo_v1, const vec4 *cpo_v2 );
    EXTERNALS void Vec4MaxInplace( vec4 *RSTR po_v0, const vec4 *cpo_v1 );
    EXTERNALS void Vec4Min( vec4 *RSTR po_v0, const vec4 *cpo_v1, const vec4 *cpo_v2 );
    EXTERNALS void Vec4MinInplace( vec4 *RSTR po_v0, const vec4 *cpo_v1 );
    EXTERNALS void Vec4Neg( vec4 *RSTR po_v0, const vec4 *cpo_v1 );
    EXTERNALS void Vec4NegInplace( vec4 *po_v );
	EXTERNALS bln  Vec4Equals( const vec4 *cpo_v0, const vec4 *cpo_v1 );
	EXTERNALS bln  Vec4EqualsByEpsilon( const vec4 *cpo_v0, const vec4 *cpo_v1, f32 epsilon );

    EXTERNALS void Vec3MultM3x3( vec3 *RSTR po_v0, const vec3 *cpo_v1, const m3x3 *cpo_m );
    EXTERNALS void Vec3MultM3x3Inplace( vec3 *po_v, const m3x3 *cpo_m );
	EXTERNALS f32 Vec3Length( const vec3 *cpo_v );
	EXTERNALS f32 Vec3SquareLength( const vec3 *cpo_v );
	EXTERNALS f32 Vec3Distance( const vec3 *cpo_v0, const vec3 *cpo_v1 );
	EXTERNALS f32 Vec3SquareDistance( const vec3 *cpo_v0, const vec3 *cpo_v1 );
    EXTERNALS void Vec3Normalize( vec3 *RSTR po_v0, const vec3 *cpo_v1 );
    EXTERNALS void Vec3NormalizeInplace( vec3 *po_v );
    EXTERNALS void Vec3Scale( vec3 *RSTR po_v0, const vec3 *cpo_v1, f32 koef );
    EXTERNALS void Vec3ScaleInplace( vec3 *po_v, f32 koef );
	EXTERNALS f32 Vec3Dot( const vec3 *cpo_v0, const vec3 *cpo_v1 );
    EXTERNALS void Vec3Cross( vec3 *RSTR po_v0, const vec3 *cpo_v1, const vec3 *cpo_v2 );
    EXTERNALS void Vec3CrossInplace( vec3 *RSTR po_v0, const vec3 *cpo_v1 );
    EXTERNALS void Vec3TransformByM4x4( vec4 *RSTR po_v0, const vec3 *cpo_v1, const m4x4 *cpo_m );  //  transform vec3 as vec4( x, y, z, 1 )
    EXTERNALS void Vec3TransformCoordByM4x4( vec3 *RSTR po_v0, const vec3 *cpo_v1, const m4x4 *cpo_m );  //  transform vec3 as vec4( x, y, z, 1 ), project result back to w = 1
    EXTERNALS void Vec3TransformCoordByM4x4Inplace( vec3 *po_v, const m4x4 *cpo_m );  //  transform vec3 as vec4( x, y, z, 1 ), project result back to w = 1
    EXTERNALS void Vec3TransformCoordByM4x4LastIden( vec3 *RSTR po_v0, const vec3 *cpo_v1, const m4x4 *cpo_m );
    EXTERNALS void Vec3TransformCoordByM4x4LastIdenInplace( vec3 *po_v, const m4x4 *cpo_m );
    EXTERNALS void Vec3TransformNormalByM4x4( vec3 *RSTR po_v0, const vec3 *cpo_v1, const m4x4 *cpo_m );  //  transform vec3 as vec4( x, y, z, 0 )
    EXTERNALS void Vec3TransformNormalByM4x4Inplace( vec3 *po_v, const m4x4 *cpo_m );  //  transform vec3 as vec4( x, y, z, 0 )
    EXTERNALS void Vec3TransformByM4x3( vec4 *RSTR po_v0, const vec3 *cpo_v1, const m4x3 *cpo_m );  //  transform vec3 as vec4( x, y, z, 1 )
    EXTERNALS void Vec3TransformCoordByM4x3( vec3 *RSTR po_v0, const vec3 *cpo_v1, const m4x3 *cpo_m );  //  transform vec3 as vec4( x, y, z, 1 ), project result back to w = 1
    EXTERNALS void Vec3TransformCoordByM4x3Inplace( vec3 *po_v, const m4x3 *cpo_m );  //  transform vec3 as vec4( x, y, z, 1 ), project result back to w = 1
    EXTERNALS void Vec3TransformCoordByM4x3LastIden( vec3 *RSTR po_v0, const vec3 *cpo_v1, const m4x3 *cpo_m );
    EXTERNALS void Vec3TransformCoordByM4x3LastIdenInplace( vec3 *po_v, const m4x3 *cpo_m );
    EXTERNALS void Vec3TransformNormalByM4x3( vec3 *RSTR po_v0, const vec3 *cpo_v1, const m4x3 *cpo_m );  //  transform vec3 as vec4( x, y, z, 0 )
    EXTERNALS void Vec3TransformNormalByM4x3Inplace( vec3 *po_v, const m4x3 *cpo_m );  //  transform vec3 as vec4( x, y, z, 0 )
    EXTERNALS void Vec3TransformByM3x3( vec3 *RSTR po_v0, const vec3 *cpo_v1, const m3x3 *cpo_m );
    EXTERNALS void Vec3Addition( vec3 *RSTR po_v0, const vec3 *cpo_v1, const vec3 *cpo_v2 );
    EXTERNALS void Vec3AdditionInplace( vec3 *RSTR po_v0, const vec3 *cpo_v1 );
    EXTERNALS void Vec3Subtract( vec3 *RSTR po_v0, const vec3 *cpo_v1, const vec3 *cpo_v2 );
    EXTERNALS void Vec3SubtractInplace( vec3 *RSTR po_v0, const vec3 *cpo_v1 );
    EXTERNALS void Vec3Max( vec3 *RSTR po_v0, const vec3 *cpo_v1, const vec3 *cpo_v2 );
    EXTERNALS void Vec3MaxInplace( vec3 *RSTR po_v0, const vec3 *cpo_v1 );
    EXTERNALS void Vec3Min( vec3 *RSTR po_v0, const vec3 *cpo_v1, const vec3 *cpo_v2 );
    EXTERNALS void Vec3MinInplace( vec3 *RSTR po_v0, const vec3 *cpo_v1 );
    EXTERNALS void Vec3Neg( vec3 *RSTR po_v0, const vec3 *cpo_v1 );
    EXTERNALS void Vec3NegInplace( vec3 *po_v );
	EXTERNALS bln Vec3Equals( const vec3 *cpo_v0, const vec3 *cpo_v1 );
	EXTERNALS bln Vec3EqualsByEpsilon( const vec3 *cpo_v0, const vec3 *cpo_v1, f32 epsilon );

	EXTERNALS f32 Vec2Length( const vec2 *cpo_v );
	EXTERNALS f32 Vec2SquareLength( const vec2 *cpo_v );
	EXTERNALS f32 Vec2Distance( const vec2 *cpo_v0, const vec2 *cpo_v1 );
	EXTERNALS f32 Vec2SquareDistance( const vec2 *cpo_v0, const vec2 *cpo_v1 );
    EXTERNALS void Vec2Normalize( vec2 *RSTR po_v0, const vec2 *cpo_v1 );
    EXTERNALS void Vec2NormalizeInplace( vec2 *po_v );
    EXTERNALS void Vec2Scale( vec2 *RSTR po_v0, const vec2 *cpo_v1, f32 koef );
    EXTERNALS void Vec2ScaleInplace( vec2 *po_v, f32 koef );
	EXTERNALS f32 Vec2Dot( const vec2 *cpo_v0, const vec2 *cpo_v1 );
    EXTERNALS void Vec2TransformByM2x2( vec2 *RSTR po_v0, const vec2 *cpo_v1, const M2x2 *cpo_m );
    EXTERNALS void Vec2TransformByM2x2Inplace( vec2 *po_v, const M2x2 *cpo_m );
    EXTERNALS void Vec2Addition( vec2 *RSTR po_v0, const vec2 *cpo_v1, const vec2 *cpo_v2 );
    EXTERNALS void Vec2AdditionInplace( vec2 *RSTR po_v0, const vec2 *cpo_v1 );
    EXTERNALS void Vec2Subtract( vec2 *RSTR po_v0, const vec2 *cpo_v1, const vec2 *cpo_v2 );
    EXTERNALS void Vec2SubtractInplace( vec2 *RSTR po_v0, const vec2 *cpo_v1 );
    EXTERNALS void Vec2Max( vec2 *RSTR po_v0, const vec2 *cpo_v1, const vec2 *cpo_v2 );
    EXTERNALS void Vec2MaxInplace( vec2 *RSTR po_v0, const vec2 *cpo_v1 );
    EXTERNALS void Vec2Min( vec2 *RSTR po_v0, const vec2 *cpo_v1, const vec2 *cpo_v2 );
    EXTERNALS void Vec2MinInplace( vec2 *RSTR po_v0, const vec2 *cpo_v1 );
    EXTERNALS void Vec2Neg( vec2 *RSTR po_v0, const vec2 *cpo_v1 );
    EXTERNALS void Vec2NegInplace( vec2 *po_v );
	EXTERNALS bln Vec2Equals( const vec2 *cpo_v0, const vec2 *cpo_v1 );
	EXTERNALS bln Vec2EqualsByEpsilon( const vec2 *cpo_v0, const vec2 *cpo_v1, f32 epsilon );

	EXTERNALS f32 PlaneDotCoord( const plane *cpo_plane, const vec3 *cpo_coord );
    EXTERNALS void PlaneNormalize( plane *RSTR po_plane0, const plane *cpo_plane1 );
    EXTERNALS void PlaneNormalizeInplace( plane *po_plane );

    EXTERNALS void Projection( m4x4 *po_m, f32 fovDegree, f32 nearPlane, f32 farPlane, f32 aspect );

	EXTERNALS bln BSCrossTest3D( const vec3 *cpo_center0, f32 radius0, const vec3 *cpo_center1, f32 radius1 );
}
}

#endif