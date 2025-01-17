#pragma once

#include "../base/base_common.h"

// @Scalar =====================================================================================

f32 sin_1f(f32 angle);
f32 cos_1f(f32 angle);
f32 tan_1f(f32 angle);

f32 lerp_1f(f32 curr, f32 target, f32 rate);

// @Vec2F ======================================================================================

#define V2F_ZERO ((Vec2F) {0.0f, 0.0f})

Vec2F v2f(f32 x, f32 y);
Vec2I v2i(i32 x, i32 y);

Vec2F add_2f(Vec2F a, Vec2F b);
Vec2F sub_2f(Vec2F a, Vec2F b);
Vec2F mul_2f(Vec2F a, Vec2F b);
Vec2F div_2f(Vec2F a, Vec2F b);
f32 dot_2f(Vec2F a, Vec2F b);
f32 cross_2f(Vec2F a, Vec2F b);
Vec2F scale_2f(Vec2F v, f32 scale);
Vec2F shift_2f(Vec2F v, f32 shift);
Vec2F transform_2f(Vec2F v, Mat2x2F m);

f32 magnitude_2f(Vec2F a);
f32 magnitude_squared_2f(Vec2F a);
f32 distance_2f(Vec2F a, Vec2F b);
f32 distance_squared_2f(Vec2F a, Vec2F b);
Vec2F normalize_2f(Vec2F a);
Vec2F project_2f(Vec2F a, Vec2F b);

Vec2F lerp_2f(Vec2F current, Vec2F target, f32 rate);
Vec2F normal_2f(Vec2F a, Vec2F b);
Vec2F midpoint_2f(Vec2F a, Vec2F b);
Vec2F intersection_2f(Vec2F a, Vec2F b, Vec2F c, Vec2F d);

f32 atan_2f(Vec2F a);

// @Vec3F ======================================================================================

#define V3F_ZERO ((Vec3F) {0.0f, 0.0f, 0.0f})

Vec3F v3f(f32 x, f32 y, f32 z);

Vec3F add_3f(Vec3F a, Vec3F b);
Vec3F sub_3f(Vec3F a, Vec3F b);
Vec3F mul_3f(Vec3F a, Vec3F b);
Vec3F div_3f(Vec3F a, Vec3F b);
f32 dot_3f(Vec3F a, Vec3F b);
Vec3F cross_3f(Vec3F a, Vec3F b);
Vec3F scale_3f(Vec3F v, f32 scale);
Vec3F transform_3f(Vec3F v, Mat3x3F m);

f32 magnitude_3f(Vec3F v);
f32 magnitude_squared_3f(Vec3F v);
f32 distance_3f(Vec3F a, Vec3F b);
f32 distance_squared_3f(Vec3F a, Vec3F b);
Vec3F normalize_3f(Vec3F v);

Vec3F lerp_3f(Vec3F curr, Vec3F target, f32 rate);

// @Vec4F ======================================================================================

#define V4F_ZERO ((Vec4F) {0.0f, 0.0f, 0.0f, 0.0f})

Vec4F v4f(f32 x, f32 y, f32 z, f32 w);

Vec4F add_4f(Vec4F a, Vec4F b);
Vec4F sub_4f(Vec4F a, Vec4F b);
Vec4F mul_4f(Vec4F a, Vec4F b);
Vec4F div_4f(Vec4F a, Vec4F b);
f32 dot_4f(Vec4F a, Vec4F b);
Vec4F scale_4f(Vec4F v, f32 scale);
Vec4F transform_4f(Vec4F v, Mat4x4F m);

f32 magnitude_4f(Vec4F v);
f32 magnitude_squared_4f(Vec4F v);
f32 distance_4f(Vec4F a, Vec4F b);
f32 distance_squared_4f(Vec4F a, Vec4F b);
Vec4F normalize_4f(Vec4F v);

Vec4F lerp_4f(Vec4F curr, Vec4F target, f32 rate);

// @Mat2x2F =================================================================================

Mat2x2F m2x2f(f32 k);
Mat2x2F rows_2x2f(Vec2F v1, Vec2F v2);
Mat2x2F cols_2x2f(Vec2F v1, Vec2F v2);

Mat2x2F mul_2x2f(Mat2x2F a, Mat2x2F b);
Mat2x2F transpose_2x2f(Mat2x2F m);
Mat2x2F inverse_2x2f(Mat2x2F m);

// @Mat3x3F =================================================================================

Mat3x3F m3x3f(f32 k);
Mat3x3F rows_3x3f(Vec3F v1, Vec3F v2, Vec3F v3);
Mat3x3F cols_3x3f(Vec3F v1, Vec3F v2, Vec3F v3);

Mat3x3F mul_3x3f(Mat3x3F a, Mat3x3F b);
Mat3x3F transpose_3x3f(Mat3x3F m);
Mat3x3F invert_3x3f(Mat3x3F m);

Mat3x3F translate_3x3f(f32 x_shift, f32 y_shift);
Mat3x3F rotate_3x3f(f32 angle);
Mat3x3F scale_3x3f(f32 x_scale, f32 y_scale);
Mat3x3F shear_3x3f(f32 x_shear, f32 y_shear);

Mat3x3F orthographic_3x3f(f32 left, f32 right, f32 bot, f32 top);

// @Mat4x4F ===================================================================================

Mat4x4F m4x4f(f32 k);
Mat4x4F rows_4x4f(Vec4F v1, Vec4F v2, Vec4F v3, Vec4F v4);
Mat4x4F cols_4x4f(Vec4F v1, Vec4F v2, Vec4F v3, Vec4F v4);
Mat4x4F m4x4f(f32 k);

Mat4x4F mul_4x4f(Mat4x4F a, Mat4x4F b);
Mat4x4F transpose_4x4f(Mat4x4F m);

Mat4x4F translate_4x4f(f32 x_shift, f32 y_shift, f32 z_shift);
Mat4x4F rotate_4x4f(f32 angle, Vec3F axis); // Should use quaternion
Mat4x4F scale_4x4f(f32 x_scale, f32 y_scale, f32 z_scale);
Mat4x4F shear_4x4f(f32 x_shear, f32 y_shear, f32 z_shear);

Mat3x3F orthographic_3x3f(f32 left, f32 right, f32 top, f32 bot);

#ifdef __cplusplus

// @Overloading ================================================================================

Vec2F operator+(Vec2F a, Vec2F b);
Vec3F operator+(Vec3F a, Vec3F b);
Vec4F operator+(Vec4F a, Vec4F b);
Vec2F operator-(Vec2F a, Vec2F b);
Vec3F operator-(Vec3F a, Vec3F b);
Vec4F operator-(Vec4F a, Vec4F b);
f32 operator*(Vec2F a, Vec2F b);
f32 operator*(Vec3F a, Vec3F b);
f32 operator*(Vec4F a, Vec4F b);
Mat3x3F operator*(Mat3x3F a, Mat3x3F b);
Mat4x4F operator*(Mat4x4F a, Mat4x4F b);

#endif
