
#ifndef __MATRIX4X4__
#define __MATRIX4X4__
//#include "Math.h"

#include "Inline.h"


typedef struct Matrix4x4
    {
	public:
        float M11;
        float M12;
        float M13;
        float M14;
        float M21;
        float M22;
        float M23;
        float M24;
        float M31;
        float M32;
        float M33;
        float M34;
        float M41;
        float M42;
        float M43;
        float M44;

		INLINE Matrix4x4 operator *(const Matrix4x4& matrix2)
        {
            Matrix4x4 matrix;
            matrix.M11 = (((this->M11 * matrix2.M11) + (this->M12 * matrix2.M21)) + (this->M13 * matrix2.M31)) + (this->M14 * matrix2.M41);
            matrix.M12 = (((this->M11 * matrix2.M12) + (this->M12 * matrix2.M22)) + (this->M13 * matrix2.M32)) + (this->M14 * matrix2.M42);
            matrix.M13 = (((this->M11 * matrix2.M13) + (this->M12 * matrix2.M23)) + (this->M13 * matrix2.M33)) + (this->M14 * matrix2.M43);
            matrix.M14 = (((this->M11 * matrix2.M14) + (this->M12 * matrix2.M24)) + (this->M13 * matrix2.M34)) + (this->M14 * matrix2.M44);
            matrix.M21 = (((this->M21 * matrix2.M11) + (this->M22 * matrix2.M21)) + (this->M23 * matrix2.M31)) + (this->M24 * matrix2.M41);
            matrix.M22 = (((this->M21 * matrix2.M12) + (this->M22 * matrix2.M22)) + (this->M23 * matrix2.M32)) + (this->M24 * matrix2.M42);
            matrix.M23 = (((this->M21 * matrix2.M13) + (this->M22 * matrix2.M23)) + (this->M23 * matrix2.M33)) + (this->M24 * matrix2.M43);
            matrix.M24 = (((this->M21 * matrix2.M14) + (this->M22 * matrix2.M24)) + (this->M23 * matrix2.M34)) + (this->M24 * matrix2.M44);
            matrix.M31 = (((this->M31 * matrix2.M11) + (this->M32 * matrix2.M21)) + (this->M33 * matrix2.M31)) + (this->M34 * matrix2.M41);
            matrix.M32 = (((this->M31 * matrix2.M12) + (this->M32 * matrix2.M22)) + (this->M33 * matrix2.M32)) + (this->M34 * matrix2.M42);
            matrix.M33 = (((this->M31 * matrix2.M13) + (this->M32 * matrix2.M23)) + (this->M33 * matrix2.M33)) + (this->M34 * matrix2.M43);
            matrix.M34 = (((this->M31 * matrix2.M14) + (this->M32 * matrix2.M24)) + (this->M33 * matrix2.M34)) + (this->M34 * matrix2.M44);
            matrix.M41 = (((this->M41 * matrix2.M11) + (this->M42 * matrix2.M21)) + (this->M43 * matrix2.M31)) + (this->M44 * matrix2.M41);
            matrix.M42 = (((this->M41 * matrix2.M12) + (this->M42 * matrix2.M22)) + (this->M43 * matrix2.M32)) + (this->M44 * matrix2.M42);
            matrix.M43 = (((this->M41 * matrix2.M13) + (this->M42 * matrix2.M23)) + (this->M43 * matrix2.M33)) + (this->M44 * matrix2.M43);
            matrix.M44 = (((this->M41 * matrix2.M14) + (this->M42 * matrix2.M24)) + (this->M43 * matrix2.M34)) + (this->M44 * matrix2.M44);
            return matrix;
        }
    } Matrix4x4, *PMatrix4x4, **LPMatrix4x4;

#endif
