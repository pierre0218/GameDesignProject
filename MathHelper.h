#include <math.h>

class MathHelper
{
public:
	// math helper functions
	static float Abs(float n)
	{
		return abs(n);
	}


	static void EulerToQuarternion(float x, float y, float z, float* quarternion)
	{
		// Assuming the angles are in radians.
		double c1 = cos(y / 2);
		double s1 = sin(y / 2);
		double c2 = cos(z / 2);
		double s2 = sin(z / 2);
		double c3 = cos(x / 2);
		double s3 = sin(x / 2);
		double c1c2 = c1*c2;
		double s1s2 = s1*s2;
		float qw = c1c2*c3 - s1s2*s3;
		float qx = c1c2*s3 + s1s2*c3;
		float qy = s1*c2*c3 + c1*s2*s3;
		float qz = c1*s2*c3 - s1*c2*s3;

		quarternion[0] = qw;
		quarternion[1] = qx;
		quarternion[2] = qy;
		quarternion[3] = qz;
	}

	static void QuaternionMultiVector(float* quat, float* vec, float* result)
	{
		float num = quat[1] * 2;
		float num2 = quat[2] * 2;
		float num3 = quat[3] * 2;
		float num4 = quat[1] * num;
		float num5 = quat[2] * num2;
		float num6 = quat[3] * num3;
		float num7 = quat[1] * num2;
		float num8 = quat[1] * num3;
		float num9 = quat[2] * num3;
		float num10 = quat[0] * num;
		float num11 = quat[0] * num2;
		float num12 = quat[0] * num3;

		result[0] = (1 - (num5 + num6)) * vec[0] + (num7 - num12) * vec[1] + (num8 + num11) * vec[2];
		result[1] = (num7 + num12) * vec[0] + (1 - (num4 + num6)) * vec[1] + (num9 - num10) * vec[2];
		result[2] = (num8 - num11) * vec[0] + (num9 + num10) * vec[1] + (1 - (num4 + num5)) * vec[2];
	}

	static float DegToRad(float d)
	{
		return d*3.14159f / 180;
	}

	static void VectorCross(float* vec1, float* vec2, float* result)
	{
		result[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
		result[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
		result[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
	}

	static float VectorDot(float* vec1, float* vec2)
	{
		float result;

		result = vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2];

		return result;
	}

	static bool IsPointInsideTriangle(float* a, float* b, float* c, float* p)
	{
		// Compute vectors       
		float v0[3], v1[3], v2[3];
		v0[0] = c[0] - a[0];
		v0[1] = c[1] - a[1];
		v0[2] = 0;

		v1[0] = b[0] - a[0];
		v1[1] = b[1] - a[1];
		v1[2] = 0;

		v2[0] = p[0] - a[0];
		v2[1] = p[1] - a[1];
		v2[2] = 0;


		// Compute dot products
		float dot00 = VectorDot(v0, v0);
		float dot01 = VectorDot(v0, v1);
		float dot02 = VectorDot(v0, v2);
		float dot11 = VectorDot(v1, v1);
		float dot12 = VectorDot(v1, v2);

		// Compute barycentric coordinates
		float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
		float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
		float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

		// Check if point is in triangle
		return (u > 0) && (v > 0) && (u + v < 1);
	}

	static float VectorDistance(float* vec1, float* vec2)
	{
		float result;
		float distanceVec[3];
		distanceVec[0] = vec1[0] - vec2[0];
		distanceVec[1] = vec1[1] - vec2[1];
		distanceVec[2] = vec1[2] - vec2[2];

		result = VectorMagnitude(distanceVec);

		return result;
	}

	static void VectorNormalize(float* vec1, float* result)
	{
		float magnitude = VectorMagnitude(vec1);

		result[0] = vec1[0] / magnitude;
		result[1] = vec1[1] / magnitude;
		result[2] = vec1[2] / magnitude;
	}

	static float VectorMagnitude(float* vec1)
	{
		float magnitude = sqrt(vec1[0] * vec1[0] + vec1[1] * vec1[1] + vec1[2] * vec1[2]);
		return magnitude;
	}

	static void VectorScalerMultiply(float s, float* vec1, float* result)
	{
		result[0] = s*vec1[0];
		result[1] = s*vec1[1];
		result[2] = s*vec1[2];
	}

	static void Matrix16MultiplyVector(float* M1, float* Vec, float* result)
	{
		result[0] = M1[0] * Vec[0] + M1[4] * Vec[1] + M1[8] * Vec[2] + M1[12] * Vec[3];
		result[1] = M1[1] * Vec[0] + M1[5] * Vec[1] + M1[9] * Vec[2] + M1[13] * Vec[3];
		result[2] = M1[2] * Vec[0] + M1[6] * Vec[1] + M1[10] * Vec[2] + M1[14] * Vec[3];
		result[3] = M1[3] * Vec[0] + M1[7] * Vec[1] + M1[11] * Vec[2] + M1[15] * Vec[3];
	}

	static void Matrix16Multiply(float* M1, float* M2, float* result)
	{
		result[0] = M2[0] * M1[0] + M2[1] * M1[4] + M2[2] * M1[8] + M2[3] * M1[12];
		result[1] = M2[0] * M1[1] + M2[1] * M1[5] + M2[2] * M1[9] + M2[3] * M1[13];
		result[2] = M2[0] * M1[2] + M2[1] * M1[6] + M2[2] * M1[10] + M2[3] * M1[14];
		result[3] = M2[0] * M1[3] + M2[1] * M1[7] + M2[2] * M1[11] + M2[3] * M1[15];

		result[4] = M2[4] * M1[0] + M2[5] * M1[4] + M2[6] * M1[8] + M2[7] * M1[12];
		result[5] = M2[4] * M1[1] + M2[5] * M1[5] + M2[6] * M1[9] + M2[7] * M1[13];
		result[6] = M2[4] * M1[2] + M2[5] * M1[6] + M2[6] * M1[10] + M2[7] * M1[14];
		result[7] = M2[4] * M1[3] + M2[5] * M1[7] + M2[6] * M1[11] + M2[7] * M1[15];

		result[8] = M2[8] * M1[0] + M2[9] * M1[4] + M2[10] * M1[8] + M2[11] * M1[12];
		result[9] = M2[8] * M1[1] + M2[9] * M1[5] + M2[10] * M1[9] + M2[11] * M1[13];
		result[10] = M2[8] * M1[2] + M2[9] * M1[6] + M2[10] * M1[10] + M2[11] * M1[14];
		result[11] = M2[8] * M1[3] + M2[9] * M1[7] + M2[10] * M1[11] + M2[11] * M1[15];

		result[12] = M2[12] * M1[0] + M2[13] * M1[4] + M2[14] * M1[8] + M2[15] * M1[12];
		result[13] = M2[12] * M1[1] + M2[13] * M1[5] + M2[14] * M1[9] + M2[15] * M1[13];
		result[14] = M2[12] * M1[2] + M2[13] * M1[6] + M2[14] * M1[10] + M2[15] * M1[14];
		result[15] = M2[12] * M1[3] + M2[13] * M1[7] + M2[14] * M1[11] + M2[15] * M1[15];
	}

	static bool InvertMatrix16(float* m, float* invOut)
	{
		float inv[16], det;
		int i;

		inv[0] = m[5] * m[10] * m[15] +
			m[9] * m[14] * m[7] +
			m[6] * m[11] * m[13] -
			m[13] * m[10] * m[7] -
			m[9] * m[6] * m[15] -
			m[14] * m[11] * m[5];

		inv[4] = m[4] * m[10] * m[15] -
			m[4] * m[11] * m[14] -
			m[8] * m[6] * m[15] +
			m[8] * m[7] * m[14] +
			m[12] * m[6] * m[11] -
			m[12] * m[7] * m[10];

		inv[8] = -m[4] * m[9] * m[15] +
			m[4] * m[11] * m[13] +
			m[8] * m[5] * m[15] -
			m[8] * m[7] * m[13] -
			m[12] * m[5] * m[11] +
			m[12] * m[7] * m[9];

		inv[12] = m[4] * m[9] * m[14] -
			m[4] * m[10] * m[13] -
			m[8] * m[5] * m[14] +
			m[8] * m[6] * m[13] +
			m[12] * m[5] * m[10] -
			m[12] * m[6] * m[9];

		inv[1] = m[1] * m[10] * m[15] -
			m[1] * m[11] * m[14] -
			m[9] * m[2] * m[15] +
			m[9] * m[3] * m[14] +
			m[13] * m[2] * m[11] -
			m[13] * m[3] * m[10];

		inv[5] = -m[0] * m[10] * m[15] +
			m[0] * m[11] * m[14] +
			m[8] * m[2] * m[15] -
			m[8] * m[3] * m[14] -
			m[12] * m[2] * m[11] +
			m[12] * m[3] * m[10];

		inv[9] = m[0] * m[9] * m[15] -
			m[0] * m[11] * m[13] -
			m[8] * m[1] * m[15] +
			m[8] * m[3] * m[13] +
			m[12] * m[1] * m[11] -
			m[12] * m[3] * m[9];

		inv[13] = -m[0] * m[9] * m[14] +
			m[0] * m[10] * m[13] +
			m[8] * m[1] * m[14] -
			m[8] * m[2] * m[13] -
			m[12] * m[1] * m[10] +
			m[12] * m[2] * m[9];

		inv[2] = -m[1] * m[6] * m[15] +
			m[1] * m[7] * m[14] +
			m[5] * m[2] * m[15] -
			m[5] * m[3] * m[14] -
			m[13] * m[2] * m[7] +
			m[13] * m[3] * m[6];

		inv[6] = m[0] * m[6] * m[15] -
			m[0] * m[7] * m[14] -
			m[4] * m[2] * m[15] +
			m[4] * m[3] * m[14] +
			m[12] * m[2] * m[7] -
			m[12] * m[3] * m[6];

		inv[10] = -m[0] * m[5] * m[15] +
			m[0] * m[7] * m[13] +
			m[4] * m[1] * m[15] -
			m[4] * m[3] * m[13] -
			m[12] * m[1] * m[7] +
			m[12] * m[3] * m[5];

		inv[14] = m[0] * m[5] * m[14] -
			m[0] * m[6] * m[13] -
			m[4] * m[1] * m[14] +
			m[4] * m[2] * m[13] +
			m[12] * m[1] * m[6] -
			m[12] * m[2] * m[5];

		inv[3] = m[1] * m[6] * m[11] -
			m[1] * m[7] * m[10] -
			m[5] * m[2] * m[11] +
			m[5] * m[3] * m[10] +
			m[9] * m[2] * m[7] -
			m[9] * m[3] * m[6];

		inv[7] = -m[0] * m[6] * m[11] +
			m[0] * m[7] * m[10] +
			m[4] * m[2] * m[11] -
			m[4] * m[3] * m[10] -
			m[8] * m[2] * m[7] +
			m[8] * m[3] * m[6];

		inv[11] = m[0] * m[5] * m[11] -
			m[0] * m[7] * m[9] -
			m[4] * m[1] * m[11] +
			m[4] * m[3] * m[9] +
			m[8] * m[1] * m[7] -
			m[8] * m[3] * m[5];

		inv[15] = -m[0] * m[5] * m[10] +
			m[0] * m[6] * m[9] +
			m[4] * m[1] * m[10] -
			m[4] * m[2] * m[9] -
			m[8] * m[1] * m[6] +
			m[8] * m[2] * m[5];

		det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

		if (det == 0)
			return false;

		det = 1.0 / det;

		for (i = 0; i < 16; i++)
			invOut[i] = inv[i] * det;

		return true;
	}

	static float ClampAngle(float angle, float min, float max)
	{
		if (angle < -360.0F)
			angle += 360.0F;
		if (angle > 360.0F)
			angle -= 360.0F;

		float result = angle;

		if (angle > max)
			result = max;
		if (angle < min)
			result = min;
		return result;
	}
};