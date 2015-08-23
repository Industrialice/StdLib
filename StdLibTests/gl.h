#ifndef __GL_H__
#define __GL_H__

#include <windows.h>
#include <GL/gl.h>
#include <cstddef>

// Macro to return the byte offset into a buffer object in machine units.
// This is used primarily for Buffer Objects.
inline const GLubyte *BUFFER_OFFSET(size_t bytes)
{
    return reinterpret_cast<const GLubyte *>(0) + bytes;
}

namespace gl
{
    // Determines whether an OpenGL extension is supported.
    extern bool ExtensionSupported(const char *pszExtensionName);

    // Returns the highest supported GL version.
    extern void GetGLVersion(int &major, int &minor);

    // Returns the highest supported GLSL version.
    extern void GetGLSLVersion(int &major, int &minor);

    // Determines whether the specified GL version is supported.
    extern bool SupportsGLVersion(int major, int minor);

    // Determines whether the specified GLSL version is supported.
    extern bool SupportsGLSLVersion(int major, int minor);
}

extern "C" 
{
//
// OpenGL 1.2
//

#define GL_UNSIGNED_BYTE_3_3_2            0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034
#define GL_UNSIGNED_INT_8_8_8_8           0x8035
#define GL_UNSIGNED_INT_10_10_10_2        0x8036
#define GL_RESCALE_NORMAL                 0x803A
#define GL_TEXTURE_BINDING_3D             0x806A
#define GL_PACK_SKIP_IMAGES               0x806B
#define GL_PACK_IMAGE_HEIGHT              0x806C
#define GL_UNPACK_SKIP_IMAGES             0x806D
#define GL_UNPACK_IMAGE_HEIGHT            0x806E
#define GL_TEXTURE_3D                     0x806F
#define GL_PROXY_TEXTURE_3D               0x8070
#define GL_TEXTURE_DEPTH                  0x8071
#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_MAX_3D_TEXTURE_SIZE            0x8073
#define GL_UNSIGNED_BYTE_2_3_3_REV        0x8362
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV       0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV     0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV     0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV       0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV    0x8368
#define GL_BGR                            0x80E0
#define GL_BGRA                           0x80E1
#define GL_MAX_ELEMENTS_VERTICES          0x80E8
#define GL_MAX_ELEMENTS_INDICES           0x80E9
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_TEXTURE_MIN_LOD                0x813A
#define GL_TEXTURE_MAX_LOD                0x813B
#define GL_TEXTURE_BASE_LEVEL             0x813C
#define GL_TEXTURE_MAX_LEVEL              0x813D
#define GL_LIGHT_MODEL_COLOR_CONTROL      0x81F8
#define GL_SINGLE_COLOR                   0x81F9
#define GL_SEPARATE_SPECULAR_COLOR        0x81FA
#define GL_SMOOTH_POINT_SIZE_RANGE        0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY  0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE        0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY  0x0B23
#define GL_ALIASED_POINT_SIZE_RANGE       0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE       0x846E

// GL_ARB_imaging

#define GL_CONSTANT_COLOR                 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR       0x8002
#define GL_CONSTANT_ALPHA                 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA       0x8004
#define GL_BLEND_COLOR                    0x8005
#define GL_FUNC_ADD                       0x8006
#define GL_MIN                            0x8007
#define GL_MAX                            0x8008
#define GL_BLEND_EQUATION                 0x8009
#define GL_FUNC_SUBTRACT                  0x800A
#define GL_FUNC_REVERSE_SUBTRACT          0x800B
#define GL_CONVOLUTION_1D                 0x8010
#define GL_CONVOLUTION_2D                 0x8011
#define GL_SEPARABLE_2D                   0x8012
#define GL_CONVOLUTION_BORDER_MODE        0x8013
#define GL_CONVOLUTION_FILTER_SCALE       0x8014
#define GL_CONVOLUTION_FILTER_BIAS        0x8015
#define GL_REDUCE                         0x8016
#define GL_CONVOLUTION_FORMAT             0x8017
#define GL_CONVOLUTION_WIDTH              0x8018
#define GL_CONVOLUTION_HEIGHT             0x8019
#define GL_MAX_CONVOLUTION_WIDTH          0x801A
#define GL_MAX_CONVOLUTION_HEIGHT         0x801B
#define GL_POST_CONVOLUTION_RED_SCALE     0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE   0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE    0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE   0x801F
#define GL_POST_CONVOLUTION_RED_BIAS      0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS    0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS     0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS    0x8023
#define GL_HISTOGRAM                      0x8024
#define GL_PROXY_HISTOGRAM                0x8025
#define GL_HISTOGRAM_WIDTH                0x8026
#define GL_HISTOGRAM_FORMAT               0x8027
#define GL_HISTOGRAM_RED_SIZE             0x8028
#define GL_HISTOGRAM_GREEN_SIZE           0x8029
#define GL_HISTOGRAM_BLUE_SIZE            0x802A
#define GL_HISTOGRAM_ALPHA_SIZE           0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE       0x802C
#define GL_HISTOGRAM_SINK                 0x802D
#define GL_MINMAX                         0x802E
#define GL_MINMAX_FORMAT                  0x802F
#define GL_MINMAX_SINK                    0x8030
#define GL_TABLE_TOO_LARGE                0x8031
#define GL_COLOR_MATRIX                   0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH       0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH   0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE    0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE  0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE   0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE  0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS     0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS   0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS    0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS   0x80BB
#define GL_COLOR_TABLE                    0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE   0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE  0x80D2
#define GL_PROXY_COLOR_TABLE              0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE 0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE 0x80D5
#define GL_COLOR_TABLE_SCALE              0x80D6
#define GL_COLOR_TABLE_BIAS               0x80D7
#define GL_COLOR_TABLE_FORMAT             0x80D8
#define GL_COLOR_TABLE_WIDTH              0x80D9
#define GL_COLOR_TABLE_RED_SIZE           0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE         0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE          0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE         0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE     0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE     0x80DF
#define GL_CONSTANT_BORDER                0x8151
#define GL_REPLICATE_BORDER               0x8153
#define GL_CONVOLUTION_BORDER_COLOR       0x8154

extern void glBlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern void glBlendEquation(GLenum mode);
extern void glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
extern void glColorTable(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
extern void glColorTableParameterfv(GLenum target, GLenum pname, const GLfloat *params);
extern void glColorTableParameteriv(GLenum target, GLenum pname, const GLint *params);
extern void glCopyColorTable(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
extern void glGetColorTable(GLenum target, GLenum format, GLenum type, GLvoid *table);
extern void glGetColorTableParameterfv(GLenum target, GLenum pname, GLfloat *params);
extern void glGetColorTableParameteriv(GLenum target, GLenum pname, GLint *params);
extern void glColorSubTable(GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
extern void glCopyColorSubTable(GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
extern void glConvolutionFilter1D(GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
extern void glConvolutionFilter2D(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
extern void glConvolutionParameterf(GLenum target, GLenum pname, GLfloat params);
extern void glConvolutionParameterfv(GLenum target, GLenum pname, const GLfloat *params);
extern void glConvolutionParameteri(GLenum target, GLenum pname, GLint params);
extern void glConvolutionParameteriv(GLenum target, GLenum pname, const GLint *params);
extern void glCopyConvolutionFilter1D(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
extern void glCopyConvolutionFilter2D(GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
extern void glGetConvolutionFilter(GLenum target, GLenum format, GLenum type, GLvoid *image);
extern void glGetConvolutionParameterfv(GLenum target, GLenum pname, GLfloat *params);
extern void glGetConvolutionParameteriv(GLenum target, GLenum pname, GLint *params);
extern void glGetSeparableFilter(GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
extern void glSeparableFilter2D(GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);
extern void glGetHistogram(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
extern void glGetHistogramParameterfv(GLenum target, GLenum pname, GLfloat *params);
extern void glGetHistogramParameteriv(GLenum target, GLenum pname, GLint *params);
extern void glGetMinmax(GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
extern void glGetMinmaxParameterfv(GLenum target, GLenum pname, GLfloat *params);
extern void glGetMinmaxParameteriv(GLenum target, GLenum pname, GLint *params);
extern void glHistogram(GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
extern void glMinmax(GLenum target, GLenum internalformat, GLboolean sink);
extern void glResetHistogram(GLenum target);
extern void glResetMinmax(GLenum target);
extern void glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
extern void glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

//
// OpenGL 1.3
//

#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF
#define GL_TEXTURE16                      0x84D0
#define GL_TEXTURE17                      0x84D1
#define GL_TEXTURE18                      0x84D2
#define GL_TEXTURE19                      0x84D3
#define GL_TEXTURE20                      0x84D4
#define GL_TEXTURE21                      0x84D5
#define GL_TEXTURE22                      0x84D6
#define GL_TEXTURE23                      0x84D7
#define GL_TEXTURE24                      0x84D8
#define GL_TEXTURE25                      0x84D9
#define GL_TEXTURE26                      0x84DA
#define GL_TEXTURE27                      0x84DB
#define GL_TEXTURE28                      0x84DC
#define GL_TEXTURE29                      0x84DD
#define GL_TEXTURE30                      0x84DE
#define GL_TEXTURE31                      0x84DF
#define GL_ACTIVE_TEXTURE                 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE          0x84E1
#define GL_MAX_TEXTURE_UNITS              0x84E2
#define GL_TRANSPOSE_MODELVIEW_MATRIX     0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX    0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX       0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX         0x84E6
#define GL_MULTISAMPLE                    0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SAMPLE_ALPHA_TO_ONE            0x809F
#define GL_SAMPLE_COVERAGE                0x80A0
#define GL_SAMPLE_BUFFERS                 0x80A8
#define GL_SAMPLES                        0x80A9
#define GL_SAMPLE_COVERAGE_VALUE          0x80AA
#define GL_SAMPLE_COVERAGE_INVERT         0x80AB
#define GL_MULTISAMPLE_BIT                0x20000000
#define GL_NORMAL_MAP                     0x8511
#define GL_REFLECTION_MAP                 0x8512
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP       0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP         0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C
#define GL_COMPRESSED_ALPHA               0x84E9
#define GL_COMPRESSED_LUMINANCE           0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA     0x84EB
#define GL_COMPRESSED_INTENSITY           0x84EC
#define GL_COMPRESSED_RGB                 0x84ED
#define GL_COMPRESSED_RGBA                0x84EE
#define GL_TEXTURE_COMPRESSION_HINT       0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE  0x86A0
#define GL_TEXTURE_COMPRESSED             0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS     0x86A3
#define GL_CLAMP_TO_BORDER                0x812D
#define GL_COMBINE                        0x8570
#define GL_COMBINE_RGB                    0x8571
#define GL_COMBINE_ALPHA                  0x8572
#define GL_SOURCE0_RGB                    0x8580
#define GL_SOURCE1_RGB                    0x8581
#define GL_SOURCE2_RGB                    0x8582
#define GL_SOURCE0_ALPHA                  0x8588
#define GL_SOURCE1_ALPHA                  0x8589
#define GL_SOURCE2_ALPHA                  0x858A
#define GL_OPERAND0_RGB                   0x8590
#define GL_OPERAND1_RGB                   0x8591
#define GL_OPERAND2_RGB                   0x8592
#define GL_OPERAND0_ALPHA                 0x8598
#define GL_OPERAND1_ALPHA                 0x8599
#define GL_OPERAND2_ALPHA                 0x859A
#define GL_RGB_SCALE                      0x8573
#define GL_ADD_SIGNED                     0x8574
#define GL_INTERPOLATE                    0x8575
#define GL_SUBTRACT                       0x84E7
#define GL_CONSTANT                       0x8576
#define GL_PRIMARY_COLOR                  0x8577
#define GL_PREVIOUS                       0x8578
#define GL_DOT3_RGB                       0x86AE
#define GL_DOT3_RGBA                      0x86AF

extern void glActiveTexture(GLenum texture);
extern void glClientActiveTexture(GLenum texture);
extern void glMultiTexCoord1d(GLenum target, GLdouble s);
extern void glMultiTexCoord1dv(GLenum target, const GLdouble *v);
extern void glMultiTexCoord1f(GLenum target, GLfloat s);
extern void glMultiTexCoord1fv(GLenum target, const GLfloat *v);
extern void glMultiTexCoord1i(GLenum target, GLint s);
extern void glMultiTexCoord1iv(GLenum target, const GLint *v);
extern void glMultiTexCoord1s(GLenum target, GLshort s);
extern void glMultiTexCoord1sv(GLenum target, const GLshort *v);
extern void glMultiTexCoord2d(GLenum target, GLdouble s, GLdouble t);
extern void glMultiTexCoord2dv(GLenum target, const GLdouble *v);
extern void glMultiTexCoord2f(GLenum target, GLfloat s, GLfloat t);
extern void glMultiTexCoord2fv(GLenum target, const GLfloat *v);
extern void glMultiTexCoord2i(GLenum target, GLint s, GLint t);
extern void glMultiTexCoord2iv(GLenum target, const GLint *v);
extern void glMultiTexCoord2s(GLenum target, GLshort s, GLshort t);
extern void glMultiTexCoord2sv(GLenum target, const GLshort *v);
extern void glMultiTexCoord3d(GLenum target, GLdouble s, GLdouble t, GLdouble r);
extern void glMultiTexCoord3dv(GLenum target, const GLdouble *v);
extern void glMultiTexCoord3f(GLenum target, GLfloat s, GLfloat t, GLfloat r);
extern void glMultiTexCoord3fv(GLenum target, const GLfloat *v);
extern void glMultiTexCoord3i(GLenum target, GLint s, GLint t, GLint r);
extern void glMultiTexCoord3iv(GLenum target, const GLint *v);
extern void glMultiTexCoord3s(GLenum target, GLshort s, GLshort t, GLshort r);
extern void glMultiTexCoord3sv(GLenum target, const GLshort *v);
extern void glMultiTexCoord4d(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern void glMultiTexCoord4dv(GLenum target, const GLdouble *v);
extern void glMultiTexCoord4f(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern void glMultiTexCoord4fv(GLenum target, const GLfloat *v);
extern void glMultiTexCoord4i(GLenum target, GLint s, GLint t, GLint r, GLint q);
extern void glMultiTexCoord4iv(GLenum target, const GLint *v);
extern void glMultiTexCoord4s(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
extern void glMultiTexCoord4sv(GLenum target, const GLshort *v);
extern void glLoadTransposeMatrixf(const GLfloat *m);
extern void glLoadTransposeMatrixd(const GLdouble *m);
extern void glMultTransposeMatrixf(const GLfloat *m);
extern void glMultTransposeMatrixd(const GLdouble *m);
extern void glSampleCoverage(GLclampf value, GLboolean invert);
extern void glCompressedTexImage3D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
extern void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
extern void glCompressedTexImage1D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
extern void glCompressedTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
extern void glCompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
extern void glCompressedTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
extern void glGetCompressedTexImage(GLenum target, GLint level, GLvoid *img);

//
// OpenGL 1.4
//

#define GL_BLEND_DST_RGB                  0x80C8
#define GL_BLEND_SRC_RGB                  0x80C9
#define GL_BLEND_DST_ALPHA                0x80CA
#define GL_BLEND_SRC_ALPHA                0x80CB
#define GL_POINT_SIZE_MIN                 0x8126
#define GL_POINT_SIZE_MAX                 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE      0x8128
#define GL_POINT_DISTANCE_ATTENUATION     0x8129
#define GL_GENERATE_MIPMAP                0x8191
#define GL_GENERATE_MIPMAP_HINT           0x8192
#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_DEPTH_COMPONENT24              0x81A6
#define GL_DEPTH_COMPONENT32              0x81A7
#define GL_MIRRORED_REPEAT                0x8370
#define GL_FOG_COORDINATE_SOURCE          0x8450
#define GL_FOG_COORDINATE                 0x8451
#define GL_FRAGMENT_DEPTH                 0x8452
#define GL_CURRENT_FOG_COORDINATE         0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE      0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE    0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER   0x8456
#define GL_FOG_COORDINATE_ARRAY           0x8457
#define GL_COLOR_SUM                      0x8458
#define GL_CURRENT_SECONDARY_COLOR        0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE     0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE     0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE   0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER  0x845D
#define GL_SECONDARY_COLOR_ARRAY          0x845E
#define GL_MAX_TEXTURE_LOD_BIAS           0x84FD
#define GL_TEXTURE_FILTER_CONTROL         0x8500
#define GL_TEXTURE_LOD_BIAS               0x8501
#define GL_INCR_WRAP                      0x8507
#define GL_DECR_WRAP                      0x8508
#define GL_TEXTURE_DEPTH_SIZE             0x884A
#define GL_DEPTH_TEXTURE_MODE             0x884B
#define GL_TEXTURE_COMPARE_MODE           0x884C
#define GL_TEXTURE_COMPARE_FUNC           0x884D
#define GL_COMPARE_R_TO_TEXTURE           0x884E

extern void glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
extern void glFogCoordf(GLfloat coord);
extern void glFogCoordfv(const GLfloat *coord);
extern void glFogCoordd(GLdouble coord);
extern void glFogCoorddv(const GLdouble *coord);
extern void glFogCoordPointer(GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glMultiDrawArrays(GLenum mode, GLint *first, GLsizei *count, GLsizei primcount);
extern void glMultiDrawElements(GLenum mode, const GLsizei *count, GLenum type, const GLvoid* *indices, GLsizei primcount);
extern void glPointParameterf(GLenum pname, GLfloat param);
extern void glPointParameterfv(GLenum pname, const GLfloat *params);
extern void glPointParameteri(GLenum pname, GLint param);
extern void glPointParameteriv(GLenum pname, const GLint *params);
extern void glSecondaryColor3b(GLbyte red, GLbyte green, GLbyte blue);
extern void glSecondaryColor3bv(const GLbyte *v);
extern void glSecondaryColor3d(GLdouble red, GLdouble green, GLdouble blue);
extern void glSecondaryColor3dv(const GLdouble *v);
extern void glSecondaryColor3f(GLfloat red, GLfloat green, GLfloat blue);
extern void glSecondaryColor3fv(const GLfloat *v);
extern void glSecondaryColor3i(GLint red, GLint green, GLint blue);
extern void glSecondaryColor3iv(const GLint *v);
extern void glSecondaryColor3s(GLshort red, GLshort green, GLshort blue);
extern void glSecondaryColor3sv(const GLshort *v);
extern void glSecondaryColor3ub(GLubyte red, GLubyte green, GLubyte blue);
extern void glSecondaryColor3ubv(const GLubyte *v);
extern void glSecondaryColor3ui(GLuint red, GLuint green, GLuint blue);
extern void glSecondaryColor3uiv(const GLuint *v);
extern void glSecondaryColor3us(GLushort red, GLushort green, GLushort blue);
extern void glSecondaryColor3usv(const GLushort *v);
extern void glSecondaryColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glWindowPos2d(GLdouble x, GLdouble y);
extern void glWindowPos2dv(const GLdouble *v);
extern void glWindowPos2f(GLfloat x, GLfloat y);
extern void glWindowPos2fv(const GLfloat *v);
extern void glWindowPos2i(GLint x, GLint y);
extern void glWindowPos2iv(const GLint *v);
extern void glWindowPos2s(GLshort x, GLshort y);
extern void glWindowPos2sv(const GLshort *v);
extern void glWindowPos3d(GLdouble x, GLdouble y, GLdouble z);
extern void glWindowPos3dv(const GLdouble *v);
extern void glWindowPos3f(GLfloat x, GLfloat y, GLfloat z);
extern void glWindowPos3fv(const GLfloat *v);
extern void glWindowPos3i(GLint x, GLint y, GLint z);
extern void glWindowPos3iv(const GLint *v);
extern void glWindowPos3s(GLshort x, GLshort y, GLshort z);
extern void glWindowPos3sv(const GLshort *v);

//
// OpenGL 1.5
//

#define GL_BUFFER_SIZE                    0x8764
#define GL_BUFFER_USAGE                   0x8765
#define GL_QUERY_COUNTER_BITS             0x8864
#define GL_CURRENT_QUERY                  0x8865
#define GL_QUERY_RESULT                   0x8866
#define GL_QUERY_RESULT_AVAILABLE         0x8867
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ARRAY_BUFFER_BINDING           0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING    0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING    0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING     0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING     0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING    0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_READ_ONLY                      0x88B8
#define GL_WRITE_ONLY                     0x88B9
#define GL_READ_WRITE                     0x88BA
#define GL_BUFFER_ACCESS                  0x88BB
#define GL_BUFFER_MAPPED                  0x88BC
#define GL_BUFFER_MAP_POINTER             0x88BD
#define GL_STREAM_DRAW                    0x88E0
#define GL_STREAM_READ                    0x88E1
#define GL_STREAM_COPY                    0x88E2
#define GL_STATIC_DRAW                    0x88E4
#define GL_STATIC_READ                    0x88E5
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_DYNAMIC_READ                   0x88E9
#define GL_DYNAMIC_COPY                   0x88EA
#define GL_SAMPLES_PASSED                 0x8914
#define GL_FOG_COORD_SRC                  GL_FOG_COORDINATE_SOURCE
#define GL_FOG_COORD                      GL_FOG_COORDINATE
#define GL_CURRENT_FOG_COORD              GL_CURRENT_FOG_COORDINATE
#define GL_FOG_COORD_ARRAY_TYPE           GL_FOG_COORDINATE_ARRAY_TYPE
#define GL_FOG_COORD_ARRAY_STRIDE         GL_FOG_COORDINATE_ARRAY_STRIDE
#define GL_FOG_COORD_ARRAY_POINTER        GL_FOG_COORDINATE_ARRAY_POINTER
#define GL_FOG_COORD_ARRAY                GL_FOG_COORDINATE_ARRAY
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING
#define GL_SRC0_RGB                       GL_SOURCE0_RGB
#define GL_SRC1_RGB                       GL_SOURCE1_RGB
#define GL_SRC2_RGB                       GL_SOURCE2_RGB
#define GL_SRC0_ALPHA                     GL_SOURCE0_ALPHA
#define GL_SRC1_ALPHA                     GL_SOURCE1_ALPHA
#define GL_SRC2_ALPHA                     GL_SOURCE2_ALPHA

// GL types for handling large vertex buffer objects.
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

extern void glGenQueries(GLsizei n, GLuint *ids);
extern void glDeleteQueries(GLsizei n, const GLuint *ids);
extern GLboolean glIsQuery(GLuint id);
extern void glBeginQuery(GLenum target, GLuint id);
extern void glEndQuery(GLenum target);
extern void glGetQueryiv(GLenum target, GLenum pname, GLint *params);
extern void glGetQueryObjectiv(GLuint id, GLenum pname, GLint *params);
extern void glGetQueryObjectuiv(GLuint id, GLenum pname, GLuint *params);
extern void glBindBuffer(GLenum target, GLuint buffer);
extern void glDeleteBuffers(GLsizei n, const GLuint *buffers);
extern void glGenBuffers(GLsizei n, GLuint *buffers);
extern GLboolean glIsBuffer(GLuint buffer);
extern void glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
extern void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
extern void glGetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
extern GLvoid* glMapBuffer(GLenum target, GLenum access);
extern GLboolean glUnmapBuffer(GLenum target);
extern void glGetBufferParameteriv(GLenum target, GLenum pname, GLint *params);
extern void glGetBufferPointerv(GLenum target, GLenum pname, GLvoid* *params);

//
// OpenGL 2.0
//

#define GL_BLEND_EQUATION_RGB             GL_BLEND_EQUATION
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED    0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE       0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE     0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE       0x8625
#define GL_CURRENT_VERTEX_ATTRIB          0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE      0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE        0x8643
#define GL_VERTEX_ATTRIB_ARRAY_POINTER    0x8645
#define GL_STENCIL_BACK_FUNC              0x8800
#define GL_STENCIL_BACK_FAIL              0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL   0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS   0x8803
#define GL_MAX_DRAW_BUFFERS               0x8824
#define GL_DRAW_BUFFER0                   0x8825
#define GL_DRAW_BUFFER1                   0x8826
#define GL_DRAW_BUFFER2                   0x8827
#define GL_DRAW_BUFFER3                   0x8828
#define GL_DRAW_BUFFER4                   0x8829
#define GL_DRAW_BUFFER5                   0x882A
#define GL_DRAW_BUFFER6                   0x882B
#define GL_DRAW_BUFFER7                   0x882C
#define GL_DRAW_BUFFER8                   0x882D
#define GL_DRAW_BUFFER9                   0x882E
#define GL_DRAW_BUFFER10                  0x882F
#define GL_DRAW_BUFFER11                  0x8830
#define GL_DRAW_BUFFER12                  0x8831
#define GL_DRAW_BUFFER13                  0x8832
#define GL_DRAW_BUFFER14                  0x8833
#define GL_DRAW_BUFFER15                  0x8834
#define GL_BLEND_EQUATION_ALPHA           0x883D
#define GL_POINT_SPRITE                   0x8861
#define GL_COORD_REPLACE                  0x8862
#define GL_MAX_VERTEX_ATTRIBS             0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_MAX_TEXTURE_COORDS             0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS        0x8872
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS  0x8B4A
#define GL_MAX_VARYING_FLOATS             0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_SHADER_TYPE                    0x8B4F
#define GL_FLOAT_VEC2                     0x8B50
#define GL_FLOAT_VEC3                     0x8B51
#define GL_FLOAT_VEC4                     0x8B52
#define GL_INT_VEC2                       0x8B53
#define GL_INT_VEC3                       0x8B54
#define GL_INT_VEC4                       0x8B55
#define GL_BOOL                           0x8B56
#define GL_BOOL_VEC2                      0x8B57
#define GL_BOOL_VEC3                      0x8B58
#define GL_BOOL_VEC4                      0x8B59
#define GL_FLOAT_MAT2                     0x8B5A
#define GL_FLOAT_MAT3                     0x8B5B
#define GL_FLOAT_MAT4                     0x8B5C
#define GL_SAMPLER_1D                     0x8B5D
#define GL_SAMPLER_2D                     0x8B5E
#define GL_SAMPLER_3D                     0x8B5F
#define GL_SAMPLER_CUBE                   0x8B60
#define GL_SAMPLER_1D_SHADOW              0x8B61
#define GL_SAMPLER_2D_SHADOW              0x8B62
#define GL_DELETE_STATUS                  0x8B80
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_ATTACHED_SHADERS               0x8B85
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define GL_SHADER_SOURCE_LENGTH           0x8B88
#define GL_ACTIVE_ATTRIBUTES              0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH    0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_CURRENT_PROGRAM                0x8B8D
#define GL_POINT_SPRITE_COORD_ORIGIN      0x8CA0
#define GL_LOWER_LEFT                     0x8CA1
#define GL_UPPER_LEFT                     0x8CA2
#define GL_STENCIL_BACK_REF               0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK        0x8CA4
#define GL_STENCIL_BACK_WRITEMASK         0x8CA5

// GL type for program/shader text
typedef char GLchar;			// native character

extern void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
extern void glDrawBuffers(GLsizei n, const GLenum *bufs);
extern void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
extern void glStencilFuncSeparate(GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
extern void glStencilMaskSeparate(GLenum face, GLuint mask);
extern void glAttachShader(GLuint program, GLuint shader);
extern void glBindAttribLocation(GLuint program, GLuint index, const GLchar *name);
extern void glCompileShader(GLuint shader);
extern GLuint glCreateProgram();
extern GLuint glCreateShader(GLenum type);
extern void glDeleteProgram(GLuint program);
extern void glDeleteShader(GLuint shader);
extern void glDetachShader(GLuint program, GLuint shader);
extern void glDisableVertexAttribArray(GLuint index);
extern void glEnableVertexAttribArray(GLuint index);
extern void glGetActiveAttrib(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
extern void glGetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
extern void glGetAttachedShaders(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj);
extern GLint glGetAttribLocation(GLuint program, const GLchar *name);
extern void glGetProgramiv(GLuint program, GLenum pname, GLint *params);
extern void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
extern void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void glGetShaderSource(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
extern GLint glGetUniformLocation(GLuint program, const GLchar *name);
extern void glGetUniformfv(GLuint program, GLint location, GLfloat *params);
extern void glGetUniformiv(GLuint program, GLint location, GLint *params);
extern void glGetVertexAttribdv(GLuint index, GLenum pname, GLdouble *params);
extern void glGetVertexAttribfv(GLuint index, GLenum pname, GLfloat *params);
extern void glGetVertexAttribiv(GLuint index, GLenum pname, GLint *params);
extern void glGetVertexAttribPointerv(GLuint index, GLenum pname, GLvoid* *pointer);
extern GLboolean glIsProgram(GLuint program);
extern GLboolean glIsShader(GLuint shader);
extern void glLinkProgram(GLuint program);
extern void glShaderSource(GLuint shader, GLsizei count, const GLchar* *string, const GLint *length);
extern void glUseProgram(GLuint program);
extern void glUniform1f(GLint location, GLfloat v0);
extern void glUniform2f(GLint location, GLfloat v0, GLfloat v1);
extern void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void glUniform1i(GLint location, GLint v0);
extern void glUniform2i(GLint location, GLint v0, GLint v1);
extern void glUniform3i(GLint location, GLint v0, GLint v1, GLint v2);
extern void glUniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern void glUniform1fv(GLint location, GLsizei count, const GLfloat *value);
extern void glUniform2fv(GLint location, GLsizei count, const GLfloat *value);
extern void glUniform3fv(GLint location, GLsizei count, const GLfloat *value);
extern void glUniform4fv(GLint location, GLsizei count, const GLfloat *value);
extern void glUniform1iv(GLint location, GLsizei count, const GLint *value);
extern void glUniform2iv(GLint location, GLsizei count, const GLint *value);
extern void glUniform3iv(GLint location, GLsizei count, const GLint *value);
extern void glUniform4iv(GLint location, GLsizei count, const GLint *value);
extern void glUniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void glValidateProgram(GLuint program);
extern void glVertexAttrib1d(GLuint index, GLdouble x);
extern void glVertexAttrib1dv(GLuint index, const GLdouble *v);
extern void glVertexAttrib1f(GLuint index, GLfloat x);
extern void glVertexAttrib1fv(GLuint index, const GLfloat *v);
extern void glVertexAttrib1s(GLuint index, GLshort x);
extern void glVertexAttrib1sv(GLuint index, const GLshort *v);
extern void glVertexAttrib2d(GLuint index, GLdouble x, GLdouble y);
extern void glVertexAttrib2dv(GLuint index, const GLdouble *v);
extern void glVertexAttrib2f(GLuint index, GLfloat x, GLfloat y);
extern void glVertexAttrib2fv(GLuint index, const GLfloat *v);
extern void glVertexAttrib2s(GLuint index, GLshort x, GLshort y);
extern void glVertexAttrib2sv(GLuint index, const GLshort *v);
extern void glVertexAttrib3d(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern void glVertexAttrib3dv(GLuint index, const GLdouble *v);
extern void glVertexAttrib3f(GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern void glVertexAttrib3fv(GLuint index, const GLfloat *v);
extern void glVertexAttrib3s(GLuint index, GLshort x, GLshort y, GLshort z);
extern void glVertexAttrib3sv(GLuint index, const GLshort *v);
extern void glVertexAttrib4Nbv(GLuint index, const GLbyte *v);
extern void glVertexAttrib4Niv(GLuint index, const GLint *v);
extern void glVertexAttrib4Nsv(GLuint index, const GLshort *v);
extern void glVertexAttrib4Nub(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern void glVertexAttrib4Nubv(GLuint index, const GLubyte *v);
extern void glVertexAttrib4Nuiv(GLuint index, const GLuint *v);
extern void glVertexAttrib4Nusv(GLuint index, const GLushort *v);
extern void glVertexAttrib4bv(GLuint index, const GLbyte *v);
extern void glVertexAttrib4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void glVertexAttrib4dv(GLuint index, const GLdouble *v);
extern void glVertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void glVertexAttrib4fv(GLuint index, const GLfloat *v);
extern void glVertexAttrib4iv(GLuint index, const GLint *v);
extern void glVertexAttrib4s(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern void glVertexAttrib4sv(GLuint index, const GLshort *v);
extern void glVertexAttrib4ubv(GLuint index, const GLubyte *v);
extern void glVertexAttrib4uiv(GLuint index, const GLuint *v);
extern void glVertexAttrib4usv(GLuint index, const GLushort *v);
extern void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

//
// OpenGL 2.1
//

#define GL_CURRENT_RASTER_SECONDARY_COLOR 0x845F
#define GL_PIXEL_PACK_BUFFER              0x88EB
#define GL_PIXEL_UNPACK_BUFFER            0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING      0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING    0x88EF
#define GL_FLOAT_MAT2x3                   0x8B65
#define GL_FLOAT_MAT2x4                   0x8B66
#define GL_FLOAT_MAT3x2                   0x8B67
#define GL_FLOAT_MAT3x4                   0x8B68
#define GL_FLOAT_MAT4x2                   0x8B69
#define GL_FLOAT_MAT4x3                   0x8B6A
#define GL_SRGB                           0x8C40
#define GL_SRGB8                          0x8C41
#define GL_SRGB_ALPHA                     0x8C42
#define GL_SRGB8_ALPHA8                   0x8C43
#define GL_SLUMINANCE_ALPHA               0x8C44
#define GL_SLUMINANCE8_ALPHA8             0x8C45
#define GL_SLUMINANCE                     0x8C46
#define GL_SLUMINANCE8                    0x8C47
#define GL_COMPRESSED_SRGB                0x8C48
#define GL_COMPRESSED_SRGB_ALPHA          0x8C49
#define GL_COMPRESSED_SLUMINANCE          0x8C4A
#define GL_COMPRESSED_SLUMINANCE_ALPHA    0x8C4B

extern void glUniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void glUniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void glUniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void glUniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void glUniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void glUniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

//
// OpenGL 3.0
//

#define GL_ALPHA_INTEGER                  				    0x8D97
#define GL_BGRA_INTEGER                   				    0x8D9B
#define GL_BGR_INTEGER                    				    0x8D9A
#define GL_BLUE_INTEGER                   				    0x8D96
#define GL_CLAMP_FRAGMENT_COLOR           				    0x891B
#define GL_CLAMP_READ_COLOR               				    0x891C
#define GL_CLAMP_VERTEX_COLOR             				    0x891A
#define GL_CLIP_DISTANCE0                 				    GL_CLIP_PLANE0
#define GL_CLIP_DISTANCE1                 				    GL_CLIP_PLANE1
#define GL_CLIP_DISTANCE2                 				    GL_CLIP_PLANE2
#define GL_CLIP_DISTANCE3                 				    GL_CLIP_PLANE3
#define GL_CLIP_DISTANCE4                 				    GL_CLIP_PLANE4
#define GL_CLIP_DISTANCE5                 				    GL_CLIP_PLANE5
#define GL_COLOR_ATTACHMENT0              				    0x8CE0
#define GL_COLOR_ATTACHMENT0              				    0x8CE0
#define GL_COLOR_ATTACHMENT1              				    0x8CE1
#define GL_COLOR_ATTACHMENT1              				    0x8CE1
#define GL_COLOR_ATTACHMENT10             				    0x8CEA
#define GL_COLOR_ATTACHMENT10             				    0x8CEA
#define GL_COLOR_ATTACHMENT11             				    0x8CEB
#define GL_COLOR_ATTACHMENT11             				    0x8CEB
#define GL_COLOR_ATTACHMENT12             				    0x8CEC
#define GL_COLOR_ATTACHMENT12             				    0x8CEC
#define GL_COLOR_ATTACHMENT13             				    0x8CED
#define GL_COLOR_ATTACHMENT13             				    0x8CED
#define GL_COLOR_ATTACHMENT14             				    0x8CEE
#define GL_COLOR_ATTACHMENT14             				    0x8CEE
#define GL_COLOR_ATTACHMENT15             				    0x8CEF
#define GL_COLOR_ATTACHMENT15             				    0x8CEF
#define GL_COLOR_ATTACHMENT2              				    0x8CE2
#define GL_COLOR_ATTACHMENT2              				    0x8CE2
#define GL_COLOR_ATTACHMENT3             				    0x8CE3
#define GL_COLOR_ATTACHMENT3              				    0x8CE3
#define GL_COLOR_ATTACHMENT4             				    0x8CE4
#define GL_COLOR_ATTACHMENT4              				    0x8CE4
#define GL_COLOR_ATTACHMENT5             				    0x8CE5
#define GL_COLOR_ATTACHMENT5              				    0x8CE5
#define GL_COLOR_ATTACHMENT6             				    0x8CE6
#define GL_COLOR_ATTACHMENT6              				    0x8CE6
#define GL_COLOR_ATTACHMENT7             				    0x8CE7
#define GL_COLOR_ATTACHMENT7              				    0x8CE7
#define GL_COLOR_ATTACHMENT8             				    0x8CE8
#define GL_COLOR_ATTACHMENT8              				    0x8CE8
#define GL_COLOR_ATTACHMENT9             				    0x8CE9
#define GL_COLOR_ATTACHMENT9             				    0x8CE9
#define GL_COMPARE_REF_TO_TEXTURE         				    0x884E
#define GL_COMPRESSED_RED                 				    0x8225
#define GL_COMPRESSED_RED_RGTC1           				    0x8DBB
#define GL_COMPRESSED_RG                  				    0x8226
#define GL_COMPRESSED_RG_RGTC2            				    0x8DBD
#define GL_COMPRESSED_SIGNED_RED_RGTC1    				    0x8DBC
#define GL_COMPRESSED_SIGNED_RG_RGTC2     				    0x8DBE
#define GL_CONTEXT_FLAGS                  				    0x821E
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 			    0x0001
#define GL_DEPTH24_STENCIL8               				    0x88F0
#define GL_DEPTH24_STENCIL8               				    0x88F0
#define GL_DEPTH32F_STENCIL8              				    0x8CAD
#define GL_DEPTH32F_STENCIL8              				    0x8CAD
#define GL_DEPTH_ATTACHMENT               				    0x8D00
#define GL_DEPTH_ATTACHMENT               				    0x8D00
#define GL_DEPTH_BUFFER                   				    0x8223
#define GL_DEPTH_COMPONENT32F             				    0x8CAC
#define GL_DEPTH_COMPONENT32F             				    0x8CAC
#define GL_DEPTH_STENCIL                  				    0x84F9
#define GL_DEPTH_STENCIL                  				    0x84F9
#define GL_DEPTH_STENCIL_ATTACHMENT       				    0x821A
#define GL_DEPTH_STENCIL_ATTACHMENT       				    0x821A
#define GL_DRAW_FRAMEBUFFER               				    0x8CA9
#define GL_DRAW_FRAMEBUFFER               				    0x8CA9
#define GL_DRAW_FRAMEBUFFER_BINDING       				    0x8CA6
#define GL_DRAW_FRAMEBUFFER_BINDING       				    0x8CA6
#define GL_FIXED_ONLY                     				    0x891D
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 				    0x8DAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 				    0x8DAD
#define GL_FRAMEBUFFER                    				    0x8D40
#define GL_FRAMEBUFFER                    				    0x8D40
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 			    0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 			    0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 			    0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 			    0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 		    0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 		    0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 		    0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 		    0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 			    0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 			    0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 			    0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 			    0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 			    0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 			    0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 			    0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 			    0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 				    0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 				    0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 			    0x8217
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 			    0x8217
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE     0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE     0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 		    0x8CD4
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 		    0x8CD4
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 		    0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 		    0x8CD2
#define GL_FRAMEBUFFER_BINDING            				    0x8CA6
#define GL_FRAMEBUFFER_BINDING            				    0x8CA6
#define GL_FRAMEBUFFER_COMPLETE           				    0x8CD5
#define GL_FRAMEBUFFER_COMPLETE           				    0x8CD5
#define GL_FRAMEBUFFER_DEFAULT            				    0x8218
#define GL_FRAMEBUFFER_DEFAULT            				    0x8218
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 			    0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 			    0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 			    0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 			    0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 	    0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 	    0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 			    0x8D56
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 			    0x8D56
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 			    0x8CDC
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 			    0x8CDC
#define GL_FRAMEBUFFER_SRGB               				    0x8DB9
#define GL_FRAMEBUFFER_UNDEFINED          				    0x8219
#define GL_FRAMEBUFFER_UNDEFINED          				    0x8219
#define GL_FRAMEBUFFER_UNSUPPORTED        				    0x8CDD
#define GL_FRAMEBUFFER_UNSUPPORTED        				    0x8CDD
#define GL_GREEN_INTEGER                  				    0x8D95
#define GL_HALF_FLOAT                     				    0x140B
#define GL_INDEX                          				    0x8222
#define GL_INDEX                          				    0x8222
#define GL_INTERLEAVED_ATTRIBS            				    0x8C8C
#define GL_INT_SAMPLER_1D                 				    0x8DC9
#define GL_INT_SAMPLER_1D_ARRAY           				    0x8DCE
#define GL_INT_SAMPLER_2D                 				    0x8DCA
#define GL_INT_SAMPLER_2D_ARRAY           				    0x8DCF
#define GL_INT_SAMPLER_3D                 				    0x8DCB
#define GL_INT_SAMPLER_CUBE               				    0x8DCC
#define GL_INVALID_FRAMEBUFFER_OPERATION  				    0x0506
#define GL_INVALID_FRAMEBUFFER_OPERATION  				    0x0506
#define GL_MAJOR_VERSION                 			 	    0x821B
#define GL_MAP_FLUSH_EXPLICIT_BIT         				    0x0010
#define GL_MAP_INVALIDATE_BUFFER_BIT      				    0x0008
#define GL_MAP_INVALIDATE_RANGE_BIT       				    0x0004
#define GL_MAP_READ_BIT                   				    0x0001
#define GL_MAP_UNSYNCHRONIZED_BIT         				    0x0020
#define GL_MAP_WRITE_BIT                  				    0x0002
#define GL_MAX_ARRAY_TEXTURE_LAYERS       				    0x88FF
#define GL_MAX_CLIP_DISTANCES             				    GL_MAX_CLIP_PLANES
#define GL_MAX_COLOR_ATTACHMENTS          				    0x8CDF
#define GL_MAX_COLOR_ATTACHMENTS          				    0x8CDF
#define GL_MAX_PROGRAM_TEXEL_OFFSET       				    0x8905
#define GL_MAX_RENDERBUFFER_SIZE          				    0x84E8
#define GL_MAX_RENDERBUFFER_SIZE          				    0x84E8
#define GL_MAX_SAMPLES                    				    0x8D57
#define GL_MAX_SAMPLES                    				    0x8D57
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS    0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 		    0x8C8B
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 	    0x8C80
#define GL_MAX_VARYING_COMPONENTS         				    0x8B4B
#define GL_MINOR_VERSION                  				    0x821C
#define GL_MIN_PROGRAM_TEXEL_OFFSET       				    0x8904
#define GL_NUM_EXTENSIONS                 				    0x821D
#define GL_PRIMITIVES_GENERATED           				    0x8C87
#define GL_PROXY_TEXTURE_1D_ARRAY         				    0x8C19
#define GL_PROXY_TEXTURE_2D_ARRAY         				    0x8C1B
#define GL_QUERY_BY_REGION_NO_WAIT        				    0x8E16
#define GL_QUERY_BY_REGION_WAIT           				    0x8E15
#define GL_QUERY_NO_WAIT                  				    0x8E14
#define GL_QUERY_WAIT                     				    0x8E13
#define GL_R11F_G11F_B10F                 				    0x8C3A
#define GL_R16                            				    0x822A
#define GL_R16F                           				    0x822D
#define GL_R16I                           				    0x8233
#define GL_R16UI                          				    0x8234
#define GL_R32F                           				    0x822E
#define GL_R32I                           				    0x8235
#define GL_R32UI                          				    0x8236
#define GL_R8                             				    0x8229
#define GL_R8I                            				    0x8231
#define GL_R8UI                           				    0x8232
#define GL_RASTERIZER_DISCARD             				    0x8C89
#define GL_READ_FRAMEBUFFER               				    0x8CA8
#define GL_READ_FRAMEBUFFER               				    0x8CA8
#define GL_READ_FRAMEBUFFER_BINDING       				    0x8CAA
#define GL_READ_FRAMEBUFFER_BINDING       				    0x8CAA
#define GL_RED_INTEGER                    				    0x8D94
#define GL_RENDERBUFFER                   				    0x8D41
#define GL_RENDERBUFFER                   				    0x8D41
#define GL_RENDERBUFFER_ALPHA_SIZE        				    0x8D53
#define GL_RENDERBUFFER_ALPHA_SIZE        				    0x8D53
#define GL_RENDERBUFFER_BINDING           				    0x8CA7
#define GL_RENDERBUFFER_BINDING           				    0x8CA7
#define GL_RENDERBUFFER_BLUE_SIZE         				    0x8D52
#define GL_RENDERBUFFER_BLUE_SIZE         				    0x8D52
#define GL_RENDERBUFFER_DEPTH_SIZE        				    0x8D54
#define GL_RENDERBUFFER_DEPTH_SIZE        				    0x8D54
#define GL_RENDERBUFFER_GREEN_SIZE        				    0x8D51
#define GL_RENDERBUFFER_GREEN_SIZE        				    0x8D51
#define GL_RENDERBUFFER_HEIGHT            				    0x8D43
#define GL_RENDERBUFFER_HEIGHT            				    0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT   				    0x8D44
#define GL_RENDERBUFFER_INTERNAL_FORMAT   				    0x8D44
#define GL_RENDERBUFFER_RED_SIZE          				    0x8D50
#define GL_RENDERBUFFER_RED_SIZE          				    0x8D50
#define GL_RENDERBUFFER_SAMPLES           				    0x8CAB
#define GL_RENDERBUFFER_SAMPLES           				    0x8CAB
#define GL_RENDERBUFFER_STENCIL_SIZE      				    0x8D55
#define GL_RENDERBUFFER_STENCIL_SIZE      				    0x8D55
#define GL_RENDERBUFFER_WIDTH             				    0x8D42
#define GL_RENDERBUFFER_WIDTH             				    0x8D42
#define GL_RG                             				    0x8227
#define GL_RG16                          				    0x822C
#define GL_RG16F                          				    0x822F
#define GL_RG16I                          				    0x8239
#define GL_RG16UI                         				    0x823A
#define GL_RG32F                          				    0x8230
#define GL_RG32I                          				    0x823B
#define GL_RG32UI                         				    0x823C
#define GL_RG8                            				    0x822B
#define GL_RG8I                           				    0x8237
#define GL_RG8UI                          				    0x8238
#define GL_RGB16F                         				    0x881B
#define GL_RGB16I                         				    0x8D89
#define GL_RGB16UI                        				    0x8D77
#define GL_RGB32F                         				    0x8815
#define GL_RGB32I                         				    0x8D83
#define GL_RGB32UI                        				    0x8D71
#define GL_RGB8I                          				    0x8D8F
#define GL_RGB8UI                         				    0x8D7D
#define GL_RGB9_E5                        				    0x8C3D
#define GL_RGBA16F                        				    0x881A
#define GL_RGBA16I                        				    0x8D88
#define GL_RGBA16UI                       				    0x8D76
#define GL_RGBA32F                        				    0x8814
#define GL_RGBA32I                        				    0x8D82
#define GL_RGBA32UI                       				    0x8D70
#define GL_RGBA8I                         				    0x8D8E
#define GL_RGBA8UI                        				    0x8D7C
#define GL_RGBA_INTEGER                   				    0x8D99
#define GL_RGB_INTEGER                    				    0x8D98
#define GL_RG_INTEGER                     				    0x8228
#define GL_SAMPLER_1D_ARRAY               				    0x8DC0
#define GL_SAMPLER_1D_ARRAY_SHADOW       			 	    0x8DC3
#define GL_SAMPLER_2D_ARRAY               				    0x8DC1
#define GL_SAMPLER_2D_ARRAY_SHADOW        				    0x8DC4
#define GL_SAMPLER_CUBE_SHADOW            				    0x8DC5
#define GL_SEPARATE_ATTRIBS               				    0x8C8D
#define GL_STENCIL_ATTACHMENT             				    0x8D20
#define GL_STENCIL_ATTACHMENT             				    0x8D20
#define GL_STENCIL_BUFFER                 				    0x8224
#define GL_STENCIL_INDEX1                 				    0x8D46
#define GL_STENCIL_INDEX1                 				    0x8D46
#define GL_STENCIL_INDEX16                				    0x8D49
#define GL_STENCIL_INDEX16                				    0x8D49
#define GL_STENCIL_INDEX4                 				    0x8D47
#define GL_STENCIL_INDEX4                 				    0x8D47
#define GL_STENCIL_INDEX8                 				    0x8D48
#define GL_STENCIL_INDEX8                 				    0x8D48
#define GL_TEXTURE_1D_ARRAY               				    0x8C18
#define GL_TEXTURE_2D_ARRAY               				    0x8C1A
#define GL_TEXTURE_ALPHA_TYPE             				    0x8C13
#define GL_TEXTURE_ALPHA_TYPE             				    0x8C13
#define GL_TEXTURE_BINDING_1D_ARRAY       				    0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY       				    0x8C1D
#define GL_TEXTURE_BLUE_TYPE              				    0x8C12
#define GL_TEXTURE_BLUE_TYPE              				    0x8C12
#define GL_TEXTURE_DEPTH_TYPE             				    0x8C16
#define GL_TEXTURE_DEPTH_TYPE             				    0x8C16
#define GL_TEXTURE_GREEN_TYPE             				    0x8C11
#define GL_TEXTURE_GREEN_TYPE             				    0x8C11
#define GL_TEXTURE_INTENSITY_TYPE         				    0x8C15
#define GL_TEXTURE_INTENSITY_TYPE         				    0x8C15
#define GL_TEXTURE_LUMINANCE_TYPE         				    0x8C14
#define GL_TEXTURE_LUMINANCE_TYPE         				    0x8C14
#define GL_TEXTURE_RED_TYPE               				    0x8C10
#define GL_TEXTURE_RED_TYPE               				    0x8C10
#define GL_TEXTURE_SHARED_SIZE            				    0x8C3F
#define GL_TEXTURE_STENCIL_SIZE           				    0x88F1
#define GL_TEXTURE_STENCIL_SIZE           				    0x88F1
#define GL_TRANSFORM_FEEDBACK_BUFFER      				    0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 			    0x8C8F
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 				    0x8C7F
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 				    0x8C85
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 				    0x8C84
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 		    0x8C88
#define GL_TRANSFORM_FEEDBACK_VARYINGS    				    0x8C83
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 		    0x8C76
#define GL_UNSIGNED_INT_10F_11F_11F_REV   				    0x8C3B
#define GL_UNSIGNED_INT_24_8              				    0x84FA
#define GL_UNSIGNED_INT_24_8              				    0x84FA
#define GL_UNSIGNED_INT_5_9_9_9_REV       				    0x8C3E
#define GL_UNSIGNED_INT_SAMPLER_1D        				    0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY  				    0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D        				    0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY  				    0x8DD7
#define GL_UNSIGNED_INT_SAMPLER_3D        				    0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE      				    0x8DD4
#define GL_UNSIGNED_INT_VEC2              				    0x8DC6
#define GL_UNSIGNED_INT_VEC3              				    0x8DC7
#define GL_UNSIGNED_INT_VEC4              				    0x8DC8
#define GL_UNSIGNED_NORMALIZED            				    0x8C17
#define GL_UNSIGNED_NORMALIZED            				    0x8C17
#define GL_VERTEX_ARRAY_BINDING           				    0x85B5
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER    				    0x88FD

// GL_VERSION_3_0
extern void glColorMaski(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
extern void glGetBooleani_v(GLenum target, GLuint index, GLboolean *data);
extern void glGetIntegeri_v(GLenum target, GLuint index, GLint *data);
extern void glEnablei(GLenum target, GLuint index);
extern void glDisablei(GLenum target, GLuint index);
extern GLboolean glIsEnabledi(GLenum target, GLuint index);
extern void glBeginTransformFeedback(GLenum primitiveMode);
extern void glEndTransformFeedback(void);
extern void glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
extern void glBindBufferBase(GLenum target, GLuint index, GLuint buffer);
extern void glTransformFeedbackVaryings(GLuint program, GLsizei count, const GLchar **varyings, GLenum bufferMode);
extern void glGetTransformFeedbackVarying(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
extern void glClampColor(GLenum target, GLenum clamp);
extern void glBeginConditionalRender(GLuint id, GLenum mode);
extern void glEndConditionalRender(void);
extern void glVertexAttribI1i(GLuint index, GLint x);
extern void glVertexAttribI2i(GLuint index, GLint x, GLint y);
extern void glVertexAttribI3i(GLuint index, GLint x, GLint y, GLint z);
extern void glVertexAttribI4i(GLuint index, GLint x, GLint y, GLint z, GLint w);
extern void glVertexAttribI1ui(GLuint index, GLuint x);
extern void glVertexAttribI2ui(GLuint index, GLuint x, GLuint y);
extern void glVertexAttribI3ui(GLuint index, GLuint x, GLuint y, GLuint z);
extern void glVertexAttribI4ui(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
extern void glVertexAttribI1iv(GLuint index, const GLint *v);
extern void glVertexAttribI2iv(GLuint index, const GLint *v);
extern void glVertexAttribI3iv(GLuint index, const GLint *v);
extern void glVertexAttribI4iv(GLuint index, const GLint *v);
extern void glVertexAttribI1uiv(GLuint index, const GLuint *v);
extern void glVertexAttribI2uiv(GLuint index, const GLuint *v);
extern void glVertexAttribI3uiv(GLuint index, const GLuint *v);
extern void glVertexAttribI4uiv(GLuint index, const GLuint *v);
extern void glVertexAttribI4bv(GLuint index, const GLbyte *v);
extern void glVertexAttribI4sv(GLuint index, const GLshort *v);
extern void glVertexAttribI4ubv(GLuint index, const GLubyte *v);
extern void glVertexAttribI4usv(GLuint index, const GLushort *v);
extern void glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glGetVertexAttribIiv(GLuint index, GLenum pname, GLint *params);
extern void glGetVertexAttribIuiv(GLuint index, GLenum pname, GLuint *params);
extern void glGetUniformuiv(GLuint program, GLint location, GLuint *params);
extern void glBindFragDataLocation(GLuint program, GLuint color, const GLchar *name);
extern GLint glGetFragDataLocation(GLuint program, const GLchar *name);
extern void glUniform1ui(GLint location, GLuint v0);
extern void glUniform2ui(GLint location, GLuint v0, GLuint v1);
extern void glUniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2);
extern void glUniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
extern void glUniform1uiv(GLint location, GLsizei count, const GLuint *value);
extern void glUniform2uiv(GLint location, GLsizei count, const GLuint *value);
extern void glUniform3uiv(GLint location, GLsizei count, const GLuint *value);
extern void glUniform4uiv(GLint location, GLsizei count, const GLuint *value);
extern void glTexParameterIiv(GLenum target, GLenum pname, const GLint *params);
extern void glTexParameterIuiv(GLenum target, GLenum pname, const GLuint *params);
extern void glGetTexParameterIiv(GLenum target, GLenum pname, GLint *params);
extern void glGetTexParameterIuiv(GLenum target, GLenum pname, GLuint *params);
extern void glClearBufferiv(GLenum buffer, GLint drawbuffer, const GLint *value);
extern void glClearBufferuiv(GLenum buffer, GLint drawbuffer, const GLuint *value);
extern void glClearBufferfv(GLenum buffer, GLint drawbuffer, const GLfloat *value);
extern void glClearBufferfi(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
extern const GLubyte *glGetStringi(GLenum name, GLuint index);

// GL_ARB_framebuffer_object
extern GLboolean glIsRenderbuffer(GLuint renderbuffer);
extern void glBindRenderbuffer(GLenum target, GLuint renderbuffer);
extern void glDeleteRenderbuffers(GLsizei n, const GLuint *renderbuffers);
extern void glGenRenderbuffers(GLsizei n, GLuint *renderbuffers);
extern void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern void glGetRenderbufferParameteriv(GLenum target, GLenum pname, GLint *params);
extern GLboolean glIsFramebuffer(GLuint framebuffer);
extern void glBindFramebuffer(GLenum target, GLuint framebuffer);
extern void glDeleteFramebuffers(GLsizei n, const GLuint *framebuffers);
extern void glGenFramebuffers(GLsizei n, GLuint *framebuffers);
extern GLenum glCheckFramebufferStatus(GLenum target);
extern void glFramebufferTexture1D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void glFramebufferTexture3D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
extern void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern void glGetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint *params);
extern void glGenerateMipmap(GLenum target);
extern void glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
extern void glRenderbufferStorageMultisample(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern void glFramebufferTextureLayer(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

// GL_ARB_map_buffer_range
extern GLvoid *glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
extern void glFlushMappedBufferRange(GLenum target, GLintptr offset, GLsizeiptr length);

// GL_ARB_vertex_array_object
extern void glBindVertexArray(GLuint array);
extern void glDeleteVertexArrays(GLsizei n, const GLuint *arrays);
extern void glGenVertexArrays(GLsizei n, GLuint *arrays);
extern GLboolean glIsVertexArray(GLuint array);

//
// OpenGL 3.1
//

#define GL_INT_SAMPLER_2D_RECT            		        0x8DCD
#define GL_INT_SAMPLER_BUFFER             		        0x8DD0
#define GL_MAX_RECTANGLE_TEXTURE_SIZE     		        0x84F8
#define GL_MAX_TEXTURE_BUFFER_SIZE        		        0x8C2B
#define GL_PRIMITIVE_RESTART              		        0x8F9D
#define GL_PRIMITIVE_RESTART_INDEX        		        0x8F9E
#define GL_PROXY_TEXTURE_RECTANGLE        		        0x84F7
#define GL_R16_SNORM                      		        0x8F98
#define GL_R8_SNORM                       		        0x8F94
#define GL_RED_SNORM                      		        0x8F90
#define GL_RG16_SNORM                     		        0x8F99
#define GL_RG8_SNORM                      		        0x8F95
#define GL_RGB16_SNORM                    		        0x8F9A
#define GL_RGB8_SNORM                     		        0x8F96
#define GL_RGBA16_SNORM                   		        0x8F9B
#define GL_RGBA8_SNORM                    		        0x8F97
#define GL_RGBA_SNORM                     		        0x8F93
#define GL_RGB_SNORM                      		        0x8F92
#define GL_RG_SNORM                       		        0x8F91
#define GL_SAMPLER_2D_RECT                		        0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW         		        0x8B64
#define GL_SAMPLER_BUFFER                 		        0x8DC2
#define GL_SIGNED_NORMALIZED              		        0x8F9C
#define GL_TEXTURE_BINDING_BUFFER         		        0x8C2C
#define GL_TEXTURE_BINDING_RECTANGLE      		        0x84F6
#define GL_TEXTURE_BUFFER                 		        0x8C2A
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING 	        0x8C2D
#define GL_TEXTURE_BUFFER_FORMAT          		        0x8C2E
#define GL_TEXTURE_RECTANGLE              		        0x84F5
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT   		        0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_BUFFER    		        0x8DD8

// GL_ARB_copy_buffer
#define GL_COPY_READ_BUFFER               		        0x8F36
#define GL_COPY_WRITE_BUFFER              		        0x8F37

// GL_ARB_uniform_buffer_object
#define GL_ACTIVE_UNIFORM_BLOCKS          			    0x8A36
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 	    0x8A35
#define GL_INVALID_INDEX                  				0xFFFFFFFFu
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS     0x8A33
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS     0x8A32
#define GL_MAX_COMBINED_UNIFORM_BLOCKS    			    0x8A2E
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 	    0x8A31
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS    			    0x8A2D
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS    			    0x8A2C
#define GL_MAX_UNIFORM_BLOCK_SIZE         			    0x8A30
#define GL_MAX_UNIFORM_BUFFER_BINDINGS    			    0x8A2F
#define GL_MAX_VERTEX_UNIFORM_BLOCKS      			    0x8A2B
#define GL_UNIFORM_ARRAY_STRIDE           			    0x8A3C
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS  				0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 		0x8A43
#define GL_UNIFORM_BLOCK_BINDING          				0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE        				0x8A40
#define GL_UNIFORM_BLOCK_INDEX            			    0x8A3A
#define GL_UNIFORM_BLOCK_NAME_LENGTH      				0x8A41
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 	0x8A46
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER 	0x8A45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 	0x8A44
#define GL_UNIFORM_BUFFER                 			    0x8A11
#define GL_UNIFORM_BUFFER_BINDING         			    0x8A28
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 			    0x8A34
#define GL_UNIFORM_BUFFER_SIZE            			    0x8A2A
#define GL_UNIFORM_BUFFER_START           			    0x8A29
#define GL_UNIFORM_IS_ROW_MAJOR           				0x8A3E
#define GL_UNIFORM_MATRIX_STRIDE          			    0x8A3D
#define GL_UNIFORM_NAME_LENGTH            			    0x8A39
#define GL_UNIFORM_OFFSET                 			    0x8A3B
#define GL_UNIFORM_SIZE                   			    0x8A38
#define GL_UNIFORM_TYPE                   			    0x8A37

// GL_VERSION_3_1
extern void glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
extern void glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);
extern void glPrimitiveRestartIndex(GLuint index);
extern void glTexBuffer(GLenum target, GLenum internalformat, GLuint buffer);

// GL_ARB_copy_buffer
extern void glCopyBufferSubData(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);

// GL_ARB_uniform_buffer_object
extern void glGetActiveUniformBlockName(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
extern void glGetActiveUniformBlockiv(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
extern void glGetActiveUniformName(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
extern void glGetActiveUniformsiv(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
extern GLuint glGetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName);
extern void glGetUniformIndices(GLuint program, GLsizei uniformCount, const GLchar **uniformNames, GLuint *uniformIndices);
extern void glUniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

//
// OpenGL 3.2
//

#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT         0x00000002
#define GL_CONTEXT_CORE_PROFILE_BIT                  0x00000001
#define GL_CONTEXT_PROFILE_MASK                      0x9126
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED            0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS      0x8DA8
#define GL_GEOMETRY_INPUT_TYPE                       0x8917
#define GL_GEOMETRY_OUTPUT_TYPE                      0x8918
#define GL_GEOMETRY_SHADER                           0x8DD9
#define GL_GEOMETRY_VERTICES_OUT                     0x8916
#define GL_LINES_ADJACENCY                           0x000A
#define GL_LINE_STRIP_ADJACENCY                      0x000B
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS             0x9125
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS             0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS            0x9124
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES              0x8DE0
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS          0x8C29
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS      0x8DE1
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS           0x8DDF
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS              0x9122
#define GL_PROGRAM_POINT_SIZE                        0x8642
#define GL_TRIANGLES_ADJACENCY                       0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY                  0x000D

// GL_ARB_depth_clamp
#define GL_DEPTH_CLAMP                               0x864F

// GL_ARB_provoking_vertex
#define GL_FIRST_VERTEX_CONVENTION                   0x8E4D
#define GL_LAST_VERTEX_CONVENTION                    0x8E4E
#define GL_PROVOKING_VERTEX                          0x8E4F
#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION  0x8E4C

// GL_ARB_seamless_cube_map
#define GL_TEXTURE_CUBE_MAP_SEAMLESS                 0x884F

// GL_ARB_sync
#define GL_ALREADY_SIGNALED                          0x911A
#define GL_CONDITION_SATISFIED                       0x911C
#define GL_MAX_SERVER_WAIT_TIMEOUT                   0x9111
#define GL_OBJECT_TYPE                               0x9112
#define GL_SIGNALED                                  0x9119
#define GL_SYNC_CONDITION                            0x9113
#define GL_SYNC_FENCE                                0x9116
#define GL_SYNC_FLAGS                                0x9115
#define GL_SYNC_FLUSH_COMMANDS_BIT                   0x00000001
#define GL_SYNC_GPU_COMMANDS_COMPLETE                0x9117
#define GL_SYNC_STATUS                               0x9114
#define GL_TIMEOUT_EXPIRED                           0x911B
#define GL_TIMEOUT_IGNORED                           0xFFFFFFFFFFFFFFFFull
#define GL_UNSIGNALED                                0x9118
#define GL_WAIT_FAILED                               0x911D

typedef __int64 GLint64;
typedef unsigned __int64 GLuint64;
typedef struct __GLsync *GLsync;

// GL_ARB_texture_multisample
#define GL_INT_SAMPLER_2D_MULTISAMPLE                0x9109
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY          0x910C
#define GL_MAX_COLOR_TEXTURE_SAMPLES                 0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES                 0x910F
#define GL_MAX_INTEGER_SAMPLES                       0x9110
#define GL_MAX_SAMPLE_MASK_WORDS                     0x8E59
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE              0x9101
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY        0x9103
#define GL_SAMPLER_2D_MULTISAMPLE                    0x9108
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY              0x910B
#define GL_SAMPLE_MASK                               0x8E51
#define GL_SAMPLE_MASK_VALUE                         0x8E52
#define GL_SAMPLE_POSITION                           0x8E50
#define GL_TEXTURE_2D_MULTISAMPLE                    0x9100
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY              0x9102
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE            0x9104
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY      0x9105
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS            0x9107
#define GL_TEXTURE_SAMPLES                           0x9106
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE       0x910A
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D

// GL_VERSION_3_2
extern void glFramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level);
extern void glFramebufferTextureFace(GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
extern void glGetBufferParameteri64v(GLenum target, GLenum pname, GLint64 *params);
extern void glGetInteger64i_v(GLenum target, GLuint index, GLint64 *data);
extern void glProgramParameteri(GLuint program, GLenum pname, GLint value);

// GL_ARB_draw_elements_base_vertex
extern void glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
extern void glDrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLint basevertex);
extern void glDrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
extern void glMultiDrawElementsBaseVertex(GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei primcount, const GLint *basevertex);

// GL_ARB_provoking_vertex
extern void glProvokingVertex(GLenum mode);

// GL_ARB_sync
extern GLenum glClientWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);
extern void glDeleteSync(GLsync sync);
extern GLsync glFenceSync(GLenum condition, GLbitfield flags);
extern void glGetInteger64v(GLenum pname, GLint64 *params);
extern void glGetSynciv(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
extern GLboolean glIsSync(GLsync sync);
extern void glWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout);

// GL_ARB_texture_multisample
extern void glGetMultisamplefv(GLenum pname, GLuint index, GLfloat *val);
extern void glTexImage2DMultisample(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
extern void glTexImage3DMultisample(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
extern void glSampleMaski(GLuint index, GLbitfield mask);

} // extern "C"

#endif __GL_H__