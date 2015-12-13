#include <math.h>

class MathHelper
{
public:
	// math helper functions
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