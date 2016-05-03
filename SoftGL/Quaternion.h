#ifndef __QUATERNION__
#define __QUATERNION__    
//#include "Math.h"
#include <cmath>

	#undef INLINE
	#define INLINE inline

 struct Quaternion
    {
	public:
		float X,Y,Z,W;
		INLINE Quaternion()
        {
            X = 0.0f;
            Y = 0.0f;
            Z = 0.0f;
            W = 0.0f;
        }
        INLINE Quaternion(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }
        INLINE Quaternion operator *(Quaternion& other) const
        {
            Quaternion quaternion;
            float x = this->X;
            float y = this->Y;
            float z = this->Z;
            float w = this->W;
            float num4 = other.X;
            float num3 = other.Y;
            float num2 = other.Z;
            float num = other.W;
            float num12 = (y * num2) - (z * num3);
            float num11 = (z * num4) - (x * num2);
            float num10 = (x * num3) - (y * num4);
            float num9 = ((x * num4) + (y * num3)) + (z * num2);
            quaternion.X = ((x * num) + (num4 * w)) + num12;
            quaternion.Y = ((y * num) + (num3 * w)) + num11;
            quaternion.Z = ((z * num) + (num2 * w)) + num10;
            quaternion.W = (w * num) - num9;
            return quaternion;
        }

       /*

 

 


        public static CQuaternion RotationBasis(MatrixBasis matrix)
        {
            float det = matrix.M11 + matrix.M22 + matrix.M33;
            CQuaternion quaternion = new CQuaternion();
            if (det > 0.0f)
            {
                float num = (float)Math.Sqrt((double)(det + 1f));
                quaternion.W = num * 0.5f;
                num = 0.5f / num;
                quaternion.X = (matrix.M23 - matrix.M32) * num;
                quaternion.Y = (matrix.M31 - matrix.M13) * num;
                quaternion.Z = (matrix.M12 - matrix.M21) * num;
                return quaternion;
            }
            if ((matrix.M11 >= matrix.M22) && (matrix.M11 >= matrix.M33))
            {
                float num7 = (float)Math.Sqrt((double)((1f + matrix.M11 - matrix.M22) - matrix.M33));
                float num4 = 0.5f / num7;
                quaternion.X = 0.5f * num7;
                quaternion.Y = (matrix.M12 + matrix.M21) * num4;
                quaternion.Z = (matrix.M13 + matrix.M31) * num4;
                quaternion.W = (matrix.M23 - matrix.M32) * num4;
                return quaternion;
            }
            if (matrix.M22 > matrix.M33)
            {
                float num6 = (float)Math.Sqrt((double)((1f + matrix.M22 - matrix.M11) - matrix.M33));
                float num3 = 0.5f / num6;
                quaternion.X = (matrix.M21 + matrix.M12) * num3;
                quaternion.Y = 0.5f * num6;
                quaternion.Z = (matrix.M32 + matrix.M23) * num3;
                quaternion.W = (matrix.M31 - matrix.M13) * num3;
                return quaternion;
            }
            float num5 = (float)Math.Sqrt((double)(((1f + matrix.M33) - matrix.M11) - matrix.M22));
            float num2 = 0.5f / num5;
            quaternion.X = (matrix.M31 + matrix.M13) * num2;
            quaternion.Y = (matrix.M32 + matrix.M23) * num2;
            quaternion.Z = 0.5f * num5;
            quaternion.W = (matrix.M12 - matrix.M21) * num2;
            return quaternion;
        }*/
    };

#endif
