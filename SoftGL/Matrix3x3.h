#ifndef Matrix3x3_h__
#define Matrix3x3_h__


class Matrix3x3 {
public:
	Matrix3x3() {

	}
	~Matrix3x3() {

	}

	union {
		struct {
			float m11;
			float m12;
			float m13;

			float m21;
			float m22;
			float m23;

			float m31;
			float m32;
			float m33;
		};

		struct {
			float data[3 * 3];
		};
	};


	Matrix3x3 operator *(const Matrix3x3& matrix2) {
		Matrix3x3 matrix;
		matrix.m11 = (((this->m11 * matrix2.m11) + (this->m12 * matrix2.m21)) + (this->m13 * matrix2.m31));
		matrix.m12 = (((this->m11 * matrix2.m12) + (this->m12 * matrix2.m22)) + (this->m13 * matrix2.m32));
		matrix.m13 = (((this->m11 * matrix2.m13) + (this->m12 * matrix2.m23)) + (this->m13 * matrix2.m33));

		matrix.m21 = (((this->m21 * matrix2.m11) + (this->m22 * matrix2.m21)) + (this->m23 * matrix2.m31));
		matrix.m22 = (((this->m21 * matrix2.m12) + (this->m22 * matrix2.m22)) + (this->m23 * matrix2.m32));
		matrix.m23 = (((this->m21 * matrix2.m13) + (this->m22 * matrix2.m23)) + (this->m23 * matrix2.m33));

		matrix.m31 = (((this->m31 * matrix2.m11) + (this->m32 * matrix2.m21)) + (this->m33 * matrix2.m31));
		matrix.m32 = (((this->m31 * matrix2.m12) + (this->m32 * matrix2.m22)) + (this->m33 * matrix2.m32));
		matrix.m33 = (((this->m31 * matrix2.m13) + (this->m32 * matrix2.m23)) + (this->m33 * matrix2.m33));


		return matrix;
	}

};
#endif // Matrix3x3_h__
