// Orignal file:
//
// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2021
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
// https://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// Version: 4.0.2019.08.13

#pragma once

#include "./_def.h"
#include <algorithm>
#include <array>
#include <cstring>
#include "./ValueInterpolator.h"
#include "./Volume/DicomVolume.h"

// The interpolator is for uniformly spaced(x,y z)-values.  The input samples
// must be stored in lexicographical order to represent f(x,y,z); that is,
// F[c + xBound*(r + yBound*s)] corresponds to f(x,y,z), where c is the index
// corresponding to x, r is the index corresponding to y, and s is the index
// corresponding to z.

namespace SokarAlg {
	class AkimaInterpolation : public ValueInterpolator {

	protected:
		void dicomVolumeChanged() override;

	public:
		// Construction and destruction.
		AkimaInterpolation() :
				mXBound(xBound),
				mYBound(yBound),
				mZBound(zBound),
				mQuantity(xBound * yBound * zBound),
				mXMin(xMin),
				mXSpacing(xSpacing),
				mYMin(yMin),
				mYSpacing(ySpacing),
				mZMin(zMin),
				mZSpacing(zSpacing),
				mF(F),
				mPoly(xBound - 1, yBound - 1, zBound - 1) {
			// At least a 3x3x3 block of data points is needed to construct
			// the estimates of the boundary derivatives.
			LogAssert(mXBound >= 3 && mYBound >= 3 && mZBound >= 3 && mF != nullptr, "Invalid input.");
			LogAssert(mXSpacing > (float) 0 && mYSpacing > (float) 0 && mZSpacing > (float) 0, "Invalid input.");

			mXMax = mXMin + mXSpacing * static_cast<float>(mXBound - 1);
			mYMax = mYMin + mYSpacing * static_cast<float>(mYBound - 1);
			mZMax = mZMin + mZSpacing * static_cast<float>(mZBound - 1);

			// Create a 3D wrapper for the 1D samples.
			Vector3<float> Fmap(mXBound, mYBound, mZBound, const_cast<float *>(mF));

			// Construct first-order derivatives.
			Vector3<float> FX(mXBound, mYBound, mZBound);
			Vector3<float> FY(mXBound, mYBound, mZBound);
			Vector3<float> FZ(mXBound, mYBound, mZBound);
			GetFX(Fmap, FX);
			GetFX(Fmap, FY);
			GetFX(Fmap, FZ);

			// Construct second-order derivatives.
			Vector3<float> FXY(mXBound, mYBound, mZBound);
			Vector3<float> FXZ(mXBound, mYBound, mZBound);
			Vector3<float> FYZ(mXBound, mYBound, mZBound);
			GetFX(Fmap, FXY);
			GetFX(Fmap, FXZ);
			GetFX(Fmap, FYZ);

			// Construct third-order derivatives.
			Vector3<float> FXYZ(mXBound, mYBound, mZBound);
			GetFXYZ(Fmap, FXYZ);

			// Construct polynomials.
			GetPolynomials(Fmap, FX, FY, FZ, FXY, FXZ, FYZ, FXYZ);
		}

		~AkimaInterpolation() = default;

		// Evaluate the function and its derivatives.  The functions clamp the
		// inputs to xmin <= x <= xmax, ymin <= y <= ymax and
		// zmin <= z <= zmax.  The first operator is for function evaluation.
		// The second operator is for function or derivative evaluations.  The
		// xOrder argument is the order of the x-derivative, the yOrder
		// argument is the order of the y-derivative, and the zOrder argument
		// is the order of the z-derivative.  All orders are zero to get the
		// function value itself.
		float operator()(float x, float y, float z) const {
			x = std::min(std::max(x, mXMin), mXMax);
			y = std::min(std::max(y, mYMin), mYMax);
			z = std::min(std::max(z, mZMin), mZMax);
			int ix, iy, iz;
			float dx, dy, dz;
			XLookup(x, ix, dx);
			YLookup(y, iy, dy);
			ZLookup(z, iz, dz);
			return mPoly[iz][iy][ix](dx, dy, dz);
		}

		float operator()(int xOrder, int yOrder, int zOrder, float x, float y, float z) const {
			x = std::min(std::max(x, mXMin), mXMax);
			y = std::min(std::max(y, mYMin), mYMax);
			z = std::min(std::max(z, mZMin), mZMax);
			int ix, iy, iz;
			float dx, dy, dz;
			XLookup(x, ix, dx);
			YLookup(y, iy, dy);
			ZLookup(z, iz, dz);
			return mPoly[iz][iy][ix](xOrder, yOrder, zOrder, dx, dy, dz);
		}

	private:
		class Polynomial {
		public:
			Polynomial() {
				for (size_t ix = 0; ix < 4; ++ix) {
					for (size_t iy = 0; iy < 4; ++iy) {
						mCoeff[ix][iy].fill((float) 0);
					}
				}
			}

			// P(x,y,z) = sum_{i=0}^3 sum_{j=0}^3 sum_{k=0}^3 a_{ijk} x^i y^j z^k.
			// The tensor term A[ix][iy][iz] corresponds to the polynomial term
			// x^{ix} y^{iy} z^{iz}.
			float &A(int ix, int iy, int iz) {
				return mCoeff[ix][iy][iz];
			}

			float operator()(float x, float y, float z) const {
				std::array<float, 4> xPow = {(float) 1, x, x * x, x * x * x};
				std::array<float, 4> yPow = {(float) 1, y, y * y, y * y * y};
				std::array<float, 4> zPow = {(float) 1, z, z * z, z * z * z};

				float p = (float) 0;
				for (size_t iz = 0; iz <= 3; ++iz) {
					for (size_t iy = 0; iy <= 3; ++iy) {
						for (size_t ix = 0; ix <= 3; ++ix) {
							p += mCoeff[ix][iy][iz] * xPow[ix] * yPow[iy] * zPow[iz];
						}
					}
				}

				return p;
			}

			float operator()(int xOrder, int yOrder, int zOrder, float x, float y, float z) const {
				std::array<float, 4> xPow{};
				switch (xOrder) {
					case 0:
						xPow[0] = (float) 1;
						xPow[1] = x;
						xPow[2] = x * x;
						xPow[3] = x * x * x;
						break;
					case 1:
						xPow[0] = (float) 0;
						xPow[1] = (float) 1;
						xPow[2] = (float) 2 * x;
						xPow[3] = (float) 3 * x * x;
						break;
					case 2:
						xPow[0] = (float) 0;
						xPow[1] = (float) 0;
						xPow[2] = (float) 2;
						xPow[3] = (float) 6 * x;
						break;
					case 3:
						xPow[0] = (float) 0;
						xPow[1] = (float) 0;
						xPow[2] = (float) 0;
						xPow[3] = (float) 6;
						break;
					default:
						return (float) 0;
				}

				std::array<float, 4> yPow;
				switch (yOrder) {
					case 0:
						yPow[0] = (float) 1;
						yPow[1] = y;
						yPow[2] = y * y;
						yPow[3] = y * y * y;
						break;
					case 1:
						yPow[0] = (float) 0;
						yPow[1] = (float) 1;
						yPow[2] = (float) 2 * y;
						yPow[3] = (float) 3 * y * y;
						break;
					case 2:
						yPow[0] = (float) 0;
						yPow[1] = (float) 0;
						yPow[2] = (float) 2;
						yPow[3] = (float) 6 * y;
						break;
					case 3:
						yPow[0] = (float) 0;
						yPow[1] = (float) 0;
						yPow[2] = (float) 0;
						yPow[3] = (float) 6;
						break;
					default:
						return (float) 0;
				}

				std::array<float, 4> zPow;
				switch (zOrder) {
					case 0:
						zPow[0] = (float) 1;
						zPow[1] = z;
						zPow[2] = z * z;
						zPow[3] = z * z * z;
						break;
					case 1:
						zPow[0] = (float) 0;
						zPow[1] = (float) 1;
						zPow[2] = (float) 2 * z;
						zPow[3] = (float) 3 * z * z;
						break;
					case 2:
						zPow[0] = (float) 0;
						zPow[1] = (float) 0;
						zPow[2] = (float) 2;
						zPow[3] = (float) 6 * z;
						break;
					case 3:
						zPow[0] = (float) 0;
						zPow[1] = (float) 0;
						zPow[2] = (float) 0;
						zPow[3] = (float) 6;
						break;
					default:
						return (float) 0;
				}

				float p = (float) 0;

				for (size_t iz = 0; iz <= 3; ++iz) {
					for (size_t iy = 0; iy <= 3; ++iy) {
						for (size_t ix = 0; ix <= 3; ++ix) {
							p += mCoeff[ix][iy][iz] * xPow[ix] * yPow[iy] * zPow[iz];
						}
					}
				}

				return p;
			}

		private:
			std::array<std::array<std::array<float, 4>, 4>, 4> mCoeff;
		};

		// Support for construction.
		void GetFX(Array3<float> const &F, Array3<float> &FX) {
			std::vector<float> slope(mXBound + 3, mYBound, mZBound);
			float invDX = (float) 1 / mXSpacing;
			int ix, iy, iz;
			for (iz = 0; iz < mZBound; ++iz) {
				for (iy = 0; iy < mYBound; ++iy) {
					for (ix = 0; ix < mXBound - 1; ++ix) {
						slope[iz][iy][ix + 2] = (F[iz][iy][ix + 1] - F[iz][iy][ix]) * invDX;
					}

					slope[iz][iy][1] = (float) 2 * slope[iz][iy][2] - slope[iz][iy][3];
					slope[iz][iy][0] = (float) 2 * slope[iz][iy][1] - slope[iz][iy][2];
					slope[iz][iy][mXBound + 1] = (float) 2 * slope[iz][iy][mXBound] - slope[iz][iy][mXBound - 1];
					slope[iz][iy][mXBound + 2] = (float) 2 * slope[iz][iy][mXBound + 1] - slope[iz][iy][mXBound];
				}
			}

			for (iz = 0; iz < mZBound; ++iz) {
				for (iy = 0; iy < mYBound; ++iy) {
					for (ix = 0; ix < mXBound; ++ix) {
						FX[iz][iy][ix] = ComputeDerivative(slope[iz][iy] + ix);
					}
				}
			}
		}

		void GetFY(Array3<float> const &F, Array3<float> &FY) {
			Array3<float> slope(mYBound + 3, mXBound, mZBound);
			float invDY = (float) 1 / mYSpacing;
			int ix, iy, iz;
			for (iz = 0; iz < mZBound; ++iz) {
				for (ix = 0; ix < mXBound; ++ix) {
					for (iy = 0; iy < mYBound - 1; ++iy) {
						slope[iz][ix][iy + 2] = (F[iz][iy + 1][ix] - F[iz][iy][ix]) * invDY;
					}

					slope[iz][ix][1] = (float) 2 * slope[iz][ix][2] - slope[iz][ix][3];
					slope[iz][ix][0] = (float) 2 * slope[iz][ix][1] - slope[iz][ix][2];
					slope[iz][ix][mYBound + 1] = (float) 2 * slope[iz][ix][mYBound] - slope[iz][ix][mYBound - 1];
					slope[iz][ix][mYBound + 2] = (float) 2 * slope[iz][ix][mYBound + 1] - slope[iz][ix][mYBound];
				}
			}

			for (iz = 0; iz < mZBound; ++iz) {
				for (ix = 0; ix < mXBound; ++ix) {
					for (iy = 0; iy < mYBound; ++iy) {
						FY[iz][iy][ix] = ComputeDerivative(slope[iz][ix] + iy);
					}
				}
			}
		}

		void GetFZ(Vector3<float> const &F, Vector3<float> &FZ) {
			Vector3<float> slope(mZBound + 3, mXBound, mYBound);
			float invDZ = (float) 1 / mZSpacing;
			int ix, iy, iz;
			for (iy = 0; iy < mYBound; ++iy) {
				for (ix = 0; ix < mXBound; ++ix) {
					for (iz = 0; iz < mZBound - 1; ++iz) {
						slope[iy][ix][iz + 2] = (F[iz + 1][iy][ix] - F[iz][iy][ix]) * invDZ;
					}

					slope[iy][ix][1] = (float) 2 * slope[iy][ix][2] - slope[iy][ix][3];
					slope[iy][ix][0] = (float) 2 * slope[iy][ix][1] - slope[iy][ix][2];
					slope[iy][ix][mZBound + 1] = (float) 2 * slope[iy][ix][mZBound] - slope[iy][ix][mZBound - 1];
					slope[iy][ix][mZBound + 2] = (float) 2 * slope[iy][ix][mZBound + 1] - slope[iy][ix][mZBound];
				}
			}

			for (iy = 0; iy < mYBound; ++iy) {
				for (ix = 0; ix < mXBound; ++ix) {
					for (iz = 0; iz < mZBound; ++iz) {
						FZ[iz][iy][ix] = ComputeDerivative(slope[iy][ix] + iz);
					}
				}
			}
		}

		void GetFXY(Array3<float> const &F, Array3<float> &FXY) {
			int xBoundM1 = mXBound - 1;
			int yBoundM1 = mYBound - 1;
			int ix0 = xBoundM1, ix1 = ix0 - 1, ix2 = ix1 - 1;
			int iy0 = yBoundM1, iy1 = iy0 - 1, iy2 = iy1 - 1;
			int ix, iy, iz;

			float invDXDY = (float) 1 / (mXSpacing * mYSpacing);
			for (iz = 0; iz < mZBound; ++iz) {
				// corners of z-slice
				FXY[iz][0][0] = (float) 0.25 * invDXDY * (
						(float) 9 * F[iz][0][0]
						- (float) 12 * F[iz][0][1]
						+ (float) 3 * F[iz][0][2]
						- (float) 12 * F[iz][1][0]
						+ (float) 16 * F[iz][1][1]
						- (float) 4 * F[iz][1][2]
						+ (float) 3 * F[iz][2][0]
						- (float) 4 * F[iz][2][1]
						+ F[iz][2][2]);

				FXY[iz][0][xBoundM1] = (float) 0.25 * invDXDY * (
						(float) 9 * F[iz][0][ix0]
						- (float) 12 * F[iz][0][ix1]
						+ (float) 3 * F[iz][0][ix2]
						- (float) 12 * F[iz][1][ix0]
						+ (float) 16 * F[iz][1][ix1]
						- (float) 4 * F[iz][1][ix2]
						+ (float) 3 * F[iz][2][ix0]
						- (float) 4 * F[iz][2][ix1]
						+ F[iz][2][ix2]);

				FXY[iz][yBoundM1][0] = (float) 0.25 * invDXDY * (
						(float) 9 * F[iz][iy0][0]
						- (float) 12 * F[iz][iy0][1]
						+ (float) 3 * F[iz][iy0][2]
						- (float) 12 * F[iz][iy1][0]
						+ (float) 16 * F[iz][iy1][1]
						- (float) 4 * F[iz][iy1][2]
						+ (float) 3 * F[iz][iy2][0]
						- (float) 4 * F[iz][iy2][1]
						+ F[iz][iy2][2]);

				FXY[iz][yBoundM1][xBoundM1] = (float) 0.25 * invDXDY * (
						(float) 9 * F[iz][iy0][ix0]
						- (float) 12 * F[iz][iy0][ix1]
						+ (float) 3 * F[iz][iy0][ix2]
						- (float) 12 * F[iz][iy1][ix0]
						+ (float) 16 * F[iz][iy1][ix1]
						- (float) 4 * F[iz][iy1][ix2]
						+ (float) 3 * F[iz][iy2][ix0]
						- (float) 4 * F[iz][iy2][ix1]
						+ F[iz][iy2][ix2]);

				// x-edges of z-slice
				for (ix = 1; ix < xBoundM1; ++ix) {
					FXY[iz][0][ix] = (float) 0.25 * invDXDY * (
							(float) 3 * (F[iz][0][ix - 1] - F[iz][0][ix + 1]) -
							(float) 4 * (F[iz][1][ix - 1] - F[iz][1][ix + 1]) +
							(F[iz][2][ix - 1] - F[iz][2][ix + 1]));

					FXY[iz][yBoundM1][ix] = (float) 0.25 * invDXDY * (
							(float) 3 * (F[iz][iy0][ix - 1] - F[iz][iy0][ix + 1])
							- (float) 4 * (F[iz][iy1][ix - 1] - F[iz][iy1][ix + 1]) +
							(F[iz][iy2][ix - 1] - F[iz][iy2][ix + 1]));
				}

				// y-edges of z-slice
				for (iy = 1; iy < yBoundM1; ++iy) {
					FXY[iz][iy][0] = (float) 0.25 * invDXDY * (
							(float) 3 * (F[iz][iy - 1][0] - F[iz][iy + 1][0]) -
							(float) 4 * (F[iz][iy - 1][1] - F[iz][iy + 1][1]) +
							(F[iz][iy - 1][2] - F[iz][iy + 1][2]));

					FXY[iz][iy][xBoundM1] = (float) 0.25 * invDXDY * (
							(float) 3 * (F[iz][iy - 1][ix0] - F[iz][iy + 1][ix0])
							- (float) 4 * (F[iz][iy - 1][ix1] - F[iz][iy + 1][ix1]) +
							(F[iz][iy - 1][ix2] - F[iz][iy + 1][ix2]));
				}

				// interior of z-slice
				for (iy = 1; iy < yBoundM1; ++iy) {
					for (ix = 1; ix < xBoundM1; ++ix) {
						FXY[iz][iy][ix] = (float) 0.25 * invDXDY * (
								F[iz][iy - 1][ix - 1] - F[iz][iy - 1][ix + 1] -
								F[iz][iy + 1][ix - 1] + F[iz][iy + 1][ix + 1]);
					}
				}
			}
		}

		void GetFXZ(Vector3<float> const &F, Vector3<float> &FXZ) {
			int xBoundM1 = mXBound - 1;
			int zBoundM1 = mZBound - 1;
			int ix0 = xBoundM1, ix1 = ix0 - 1, ix2 = ix1 - 1;
			int iz0 = zBoundM1, iz1 = iz0 - 1, iz2 = iz1 - 1;
			int ix, iy, iz;

			float invDXDZ = (float) 1 / (mXSpacing * mZSpacing);
			for (iy = 0; iy < mYBound; ++iy) {
				// corners of z-slice
				FXZ[0][iy][0] = (float) 0.25 * invDXDZ * (
						(float) 9 * F[0][iy][0]
						- (float) 12 * F[0][iy][1]
						+ (float) 3 * F[0][iy][2]
						- (float) 12 * F[1][iy][0]
						+ (float) 16 * F[1][iy][1]
						- (float) 4 * F[1][iy][2]
						+ (float) 3 * F[2][iy][0]
						- (float) 4 * F[2][iy][1]
						+ F[2][iy][2]);

				FXZ[0][iy][xBoundM1] = (float) 0.25 * invDXDZ * (
						(float) 9 * F[0][iy][ix0]
						- (float) 12 * F[0][iy][ix1]
						+ (float) 3 * F[0][iy][ix2]
						- (float) 12 * F[1][iy][ix0]
						+ (float) 16 * F[1][iy][ix1]
						- (float) 4 * F[1][iy][ix2]
						+ (float) 3 * F[2][iy][ix0]
						- (float) 4 * F[2][iy][ix1]
						+ F[2][iy][ix2]);

				FXZ[zBoundM1][iy][0] = (float) 0.25 * invDXDZ * (
						(float) 9 * F[iz0][iy][0]
						- (float) 12 * F[iz0][iy][1]
						+ (float) 3 * F[iz0][iy][2]
						- (float) 12 * F[iz1][iy][0]
						+ (float) 16 * F[iz1][iy][1]
						- (float) 4 * F[iz1][iy][2]
						+ (float) 3 * F[iz2][iy][0]
						- (float) 4 * F[iz2][iy][1]
						+ F[iz2][iy][2]);

				FXZ[zBoundM1][iy][xBoundM1] = (float) 0.25 * invDXDZ * (
						(float) 9 * F[iz0][iy][ix0]
						- (float) 12 * F[iz0][iy][ix1]
						+ (float) 3 * F[iz0][iy][ix2]
						- (float) 12 * F[iz1][iy][ix0]
						+ (float) 16 * F[iz1][iy][ix1]
						- (float) 4 * F[iz1][iy][ix2]
						+ (float) 3 * F[iz2][iy][ix0]
						- (float) 4 * F[iz2][iy][ix1]
						+ F[iz2][iy][ix2]);

				// x-edges of y-slice
				for (ix = 1; ix < xBoundM1; ++ix) {
					FXZ[0][iy][ix] = (float) 0.25 * invDXDZ * (
							(float) 3 * (F[0][iy][ix - 1] - F[0][iy][ix + 1]) -
							(float) 4 * (F[1][iy][ix - 1] - F[1][iy][ix + 1]) +
							(F[2][iy][ix - 1] - F[2][iy][ix + 1]));

					FXZ[zBoundM1][iy][ix] = (float) 0.25 * invDXDZ * (
							(float) 3 * (F[iz0][iy][ix - 1] - F[iz0][iy][ix + 1])
							- (float) 4 * (F[iz1][iy][ix - 1] - F[iz1][iy][ix + 1]) +
							(F[iz2][iy][ix - 1] - F[iz2][iy][ix + 1]));
				}

				// z-edges of y-slice
				for (iz = 1; iz < zBoundM1; ++iz) {
					FXZ[iz][iy][0] = (float) 0.25 * invDXDZ * (
							(float) 3 * (F[iz - 1][iy][0] - F[iz + 1][iy][0]) -
							(float) 4 * (F[iz - 1][iy][1] - F[iz + 1][iy][1]) +
							(F[iz - 1][iy][2] - F[iz + 1][iy][2]));

					FXZ[iz][iy][xBoundM1] = (float) 0.25 * invDXDZ * (
							(float) 3 * (F[iz - 1][iy][ix0] - F[iz + 1][iy][ix0])
							- (float) 4 * (F[iz - 1][iy][ix1] - F[iz + 1][iy][ix1]) +
							(F[iz - 1][iy][ix2] - F[iz + 1][iy][ix2]));
				}

				// interior of y-slice
				for (iz = 1; iz < zBoundM1; ++iz) {
					for (ix = 1; ix < xBoundM1; ++ix) {
						FXZ[iz][iy][ix] = ((float) 0.25) * invDXDZ * (
								F[iz - 1][iy][ix - 1] - F[iz - 1][iy][ix + 1] -
								F[iz + 1][iy][ix - 1] + F[iz + 1][iy][ix + 1]);
					}
				}
			}
		}

		void GetFYZ(Vector3<float> const &F, Vector3<float> &FYZ) {
			int yBoundM1 = mYBound - 1;
			int zBoundM1 = mZBound - 1;
			int iy0 = yBoundM1, iy1 = iy0 - 1, iy2 = iy1 - 1;
			int iz0 = zBoundM1, iz1 = iz0 - 1, iz2 = iz1 - 1;
			int ix, iy, iz;

			float invDYDZ = (float) 1 / (mYSpacing * mZSpacing);
			for (ix = 0; ix < mXBound; ++ix) {
				// corners of x-slice
				FYZ[0][0][ix] = (float) 0.25 * invDYDZ * (
						(float) 9 * F[0][0][ix]
						- (float) 12 * F[0][1][ix]
						+ (float) 3 * F[0][2][ix]
						- (float) 12 * F[1][0][ix]
						+ (float) 16 * F[1][1][ix]
						- (float) 4 * F[1][2][ix]
						+ (float) 3 * F[2][0][ix]
						- (float) 4 * F[2][1][ix]
						+ F[2][2][ix]);

				FYZ[0][yBoundM1][ix] = (float) 0.25 * invDYDZ * (
						(float) 9 * F[0][iy0][ix]
						- (float) 12 * F[0][iy1][ix]
						+ (float) 3 * F[0][iy2][ix]
						- (float) 12 * F[1][iy0][ix]
						+ (float) 16 * F[1][iy1][ix]
						- (float) 4 * F[1][iy2][ix]
						+ (float) 3 * F[2][iy0][ix]
						- (float) 4 * F[2][iy1][ix]
						+ F[2][iy2][ix]);

				FYZ[zBoundM1][0][ix] = (float) 0.25 * invDYDZ * (
						(float) 9 * F[iz0][0][ix]
						- (float) 12 * F[iz0][1][ix]
						+ (float) 3 * F[iz0][2][ix]
						- (float) 12 * F[iz1][0][ix]
						+ (float) 16 * F[iz1][1][ix]
						- (float) 4 * F[iz1][2][ix]
						+ (float) 3 * F[iz2][0][ix]
						- (float) 4 * F[iz2][1][ix]
						+ F[iz2][2][ix]);

				FYZ[zBoundM1][yBoundM1][ix] = (float) 0.25 * invDYDZ * (
						(float) 9 * F[iz0][iy0][ix]
						- (float) 12 * F[iz0][iy1][ix]
						+ (float) 3 * F[iz0][iy2][ix]
						- (float) 12 * F[iz1][iy0][ix]
						+ (float) 16 * F[iz1][iy1][ix]
						- (float) 4 * F[iz1][iy2][ix]
						+ (float) 3 * F[iz2][iy0][ix]
						- (float) 4 * F[iz2][iy1][ix]
						+ F[iz2][iy2][ix]);

				// y-edges of x-slice
				for (iy = 1; iy < yBoundM1; ++iy) {
					FYZ[0][iy][ix] = (float) 0.25 * invDYDZ * (
							(float) 3 * (F[0][iy - 1][ix] - F[0][iy + 1][ix]) -
							(float) 4 * (F[1][iy - 1][ix] - F[1][iy + 1][ix]) +
							(F[2][iy - 1][ix] - F[2][iy + 1][ix]));

					FYZ[zBoundM1][iy][ix] = (float) 0.25 * invDYDZ * (
							(float) 3 * (F[iz0][iy - 1][ix] - F[iz0][iy + 1][ix])
							- (float) 4 * (F[iz1][iy - 1][ix] - F[iz1][iy + 1][ix]) +
							(F[iz2][iy - 1][ix] - F[iz2][iy + 1][ix]));
				}

				// z-edges of x-slice
				for (iz = 1; iz < zBoundM1; ++iz) {
					FYZ[iz][0][ix] = (float) 0.25 * invDYDZ * (
							(float) 3 * (F[iz - 1][0][ix] - F[iz + 1][0][ix]) -
							(float) 4 * (F[iz - 1][1][ix] - F[iz + 1][1][ix]) +
							(F[iz - 1][2][ix] - F[iz + 1][2][ix]));

					FYZ[iz][yBoundM1][ix] = (float) 0.25 * invDYDZ * (
							(float) 3 * (F[iz - 1][iy0][ix] - F[iz + 1][iy0][ix])
							- (float) 4 * (F[iz - 1][iy1][ix] - F[iz + 1][iy1][ix]) +
							(F[iz - 1][iy2][ix] - F[iz + 1][iy2][ix]));
				}

				// interior of x-slice
				for (iz = 1; iz < zBoundM1; ++iz) {
					for (iy = 1; iy < yBoundM1; ++iy) {
						FYZ[iz][iy][ix] = (float) 0.25 * invDYDZ * (
								F[iz - 1][iy - 1][ix] - F[iz - 1][iy + 1][ix] -
								F[iz + 1][iy - 1][ix] + F[iz + 1][iy + 1][ix]);
					}
				}
			}
		}

		void GetFXYZ(Vector3<float> const &F, Vector3<float> &FXYZ) {
			int xBoundM1 = mXBound - 1;
			int yBoundM1 = mYBound - 1;
			int zBoundM1 = mZBound - 1;
			int ix, iy, iz, ix0, iy0, iz0;

			float invDXDYDZ = ((float) 1) / (mXSpacing * mYSpacing * mZSpacing);

			// convolution masks
			//   centered difference, O(h^2)
			float CDer[3] = {-(float) 0.5, (float) 0, (float) 0.5};
			//   one-sided difference, O(h^2)
			float ODer[3] = {-(float) 1.5, (float) 2, -(float) 0.5};
			float mask;

			// corners
			FXYZ[0][0][0] = (float) 0;
			FXYZ[0][0][xBoundM1] = (float) 0;
			FXYZ[0][yBoundM1][0] = (float) 0;
			FXYZ[0][yBoundM1][xBoundM1] = (float) 0;
			FXYZ[zBoundM1][0][0] = (float) 0;
			FXYZ[zBoundM1][0][xBoundM1] = (float) 0;
			FXYZ[zBoundM1][yBoundM1][0] = (float) 0;
			FXYZ[zBoundM1][yBoundM1][xBoundM1] = (float) 0;
			for (iz = 0; iz <= 2; ++iz) {
				for (iy = 0; iy <= 2; ++iy) {
					for (ix = 0; ix <= 2; ++ix) {
						mask = invDXDYDZ * ODer[ix] * ODer[iy] * ODer[iz];
						FXYZ[0][0][0] += mask * F[iz][iy][ix];
						FXYZ[0][0][xBoundM1] += mask * F[iz][iy][xBoundM1 - ix];
						FXYZ[0][yBoundM1][0] += mask * F[iz][yBoundM1 - iy][ix];
						FXYZ[0][yBoundM1][xBoundM1] += mask * F[iz][yBoundM1 - iy][xBoundM1 - ix];
						FXYZ[zBoundM1][0][0] += mask * F[zBoundM1 - iz][iy][ix];
						FXYZ[zBoundM1][0][xBoundM1] += mask * F[zBoundM1 - iz][iy][xBoundM1 - ix];
						FXYZ[zBoundM1][yBoundM1][0] += mask * F[zBoundM1 - iz][yBoundM1 - iy][ix];
						FXYZ[zBoundM1][yBoundM1][xBoundM1] += mask * F[zBoundM1 - iz][yBoundM1 - iy][xBoundM1 - ix];
					}
				}
			}

			// x-edges
			for (ix0 = 1; ix0 < xBoundM1; ++ix0) {
				FXYZ[0][0][ix0] = (float) 0;
				FXYZ[0][yBoundM1][ix0] = (float) 0;
				FXYZ[zBoundM1][0][ix0] = (float) 0;
				FXYZ[zBoundM1][yBoundM1][ix0] = (float) 0;
				for (iz = 0; iz <= 2; ++iz) {
					for (iy = 0; iy <= 2; ++iy) {
						for (ix = 0; ix <= 2; ++ix) {
							mask = invDXDYDZ * CDer[ix] * ODer[iy] * ODer[iz];
							FXYZ[0][0][ix0] += mask * F[iz][iy][ix0 + ix - 1];
							FXYZ[0][yBoundM1][ix0] += mask * F[iz][yBoundM1 - iy][ix0 + ix - 1];
							FXYZ[zBoundM1][0][ix0] += mask * F[zBoundM1 - iz][iy][ix0 + ix - 1];
							FXYZ[zBoundM1][yBoundM1][ix0] += mask * F[zBoundM1 - iz][yBoundM1 - iy][ix0 + ix - 1];
						}
					}
				}
			}

			// y-edges
			for (iy0 = 1; iy0 < yBoundM1; ++iy0) {
				FXYZ[0][iy0][0] = (float) 0;
				FXYZ[0][iy0][xBoundM1] = (float) 0;
				FXYZ[zBoundM1][iy0][0] = (float) 0;
				FXYZ[zBoundM1][iy0][xBoundM1] = (float) 0;
				for (iz = 0; iz <= 2; ++iz) {
					for (iy = 0; iy <= 2; ++iy) {
						for (ix = 0; ix <= 2; ++ix) {
							mask = invDXDYDZ * ODer[ix] * CDer[iy] * ODer[iz];
							FXYZ[0][iy0][0] += mask * F[iz][iy0 + iy - 1][ix];
							FXYZ[0][iy0][xBoundM1] += mask * F[iz][iy0 + iy - 1][xBoundM1 - ix];
							FXYZ[zBoundM1][iy0][0] += mask * F[zBoundM1 - iz][iy0 + iy - 1][ix];
							FXYZ[zBoundM1][iy0][xBoundM1] += mask * F[zBoundM1 - iz][iy0 + iy - 1][xBoundM1 - ix];
						}
					}
				}
			}

			// z-edges
			for (iz0 = 1; iz0 < zBoundM1; ++iz0) {
				FXYZ[iz0][0][0] = (float) 0;
				FXYZ[iz0][0][xBoundM1] = (float) 0;
				FXYZ[iz0][yBoundM1][0] = (float) 0;
				FXYZ[iz0][yBoundM1][xBoundM1] = (float) 0;
				for (iz = 0; iz <= 2; ++iz) {
					for (iy = 0; iy <= 2; ++iy) {
						for (ix = 0; ix <= 2; ++ix) {
							mask = invDXDYDZ * ODer[ix] * ODer[iy] * CDer[iz];
							FXYZ[iz0][0][0] += mask * F[iz0 + iz - 1][iy][ix];
							FXYZ[iz0][0][xBoundM1] += mask * F[iz0 + iz - 1][iy][xBoundM1 - ix];
							FXYZ[iz0][yBoundM1][0] += mask * F[iz0 + iz - 1][yBoundM1 - iy][ix];
							FXYZ[iz0][yBoundM1][xBoundM1] += mask * F[iz0 + iz - 1][yBoundM1 - iy][xBoundM1 - ix];
						}
					}
				}
			}

			// xy-faces
			for (iy0 = 1; iy0 < yBoundM1; ++iy0) {
				for (ix0 = 1; ix0 < xBoundM1; ++ix0) {
					FXYZ[0][iy0][ix0] = (float) 0;
					FXYZ[zBoundM1][iy0][ix0] = (float) 0;
					for (iz = 0; iz <= 2; ++iz) {
						for (iy = 0; iy <= 2; ++iy) {
							for (ix = 0; ix <= 2; ++ix) {
								mask = invDXDYDZ * CDer[ix] * CDer[iy] * ODer[iz];
								FXYZ[0][iy0][ix0] += mask * F[iz][iy0 + iy - 1][ix0 + ix - 1];
								FXYZ[zBoundM1][iy0][ix0] += mask * F[zBoundM1 - iz][iy0 + iy - 1][ix0 + ix - 1];
							}
						}
					}
				}
			}

			// xz-faces
			for (iz0 = 1; iz0 < zBoundM1; ++iz0) {
				for (ix0 = 1; ix0 < xBoundM1; ++ix0) {
					FXYZ[iz0][0][ix0] = (float) 0;
					FXYZ[iz0][yBoundM1][ix0] = (float) 0;
					for (iz = 0; iz <= 2; ++iz) {
						for (iy = 0; iy <= 2; ++iy) {
							for (ix = 0; ix <= 2; ++ix) {
								mask = invDXDYDZ * CDer[ix] * ODer[iy] * CDer[iz];
								FXYZ[iz0][0][ix0] += mask * F[iz0 + iz - 1][iy][ix0 + ix - 1];
								FXYZ[iz0][yBoundM1][ix0] += mask * F[iz0 + iz - 1][yBoundM1 - iy][ix0 + ix - 1];
							}
						}
					}
				}
			}

			// yz-faces
			for (iz0 = 1; iz0 < zBoundM1; ++iz0) {
				for (iy0 = 1; iy0 < yBoundM1; ++iy0) {
					FXYZ[iz0][iy0][0] = (float) 0;
					FXYZ[iz0][iy0][xBoundM1] = (float) 0;
					for (iz = 0; iz <= 2; ++iz) {
						for (iy = 0; iy <= 2; ++iy) {
							for (ix = 0; ix <= 2; ++ix) {
								mask = invDXDYDZ * ODer[ix] * CDer[iy] * CDer[iz];
								FXYZ[iz0][iy0][0] += mask * F[iz0 + iz - 1][iy0 + iy - 1][ix];
								FXYZ[iz0][iy0][xBoundM1] += mask * F[iz0 + iz - 1][iy0 + iy - 1][xBoundM1 - ix];
							}
						}
					}
				}
			}

			// interiors
			for (iz0 = 1; iz0 < zBoundM1; ++iz0) {
				for (iy0 = 1; iy0 < yBoundM1; ++iy0) {
					for (ix0 = 1; ix0 < xBoundM1; ++ix0) {
						FXYZ[iz0][iy0][ix0] = (float) 0;

						for (iz = 0; iz <= 2; ++iz) {
							for (iy = 0; iy <= 2; ++iy) {
								for (ix = 0; ix <= 2; ++ix) {
									mask = invDXDYDZ * CDer[ix] * CDer[iy] * CDer[iz];
									FXYZ[iz0][iy0][ix0] += mask * F[iz0 + iz - 1][iy0 + iy - 1][ix0 + ix - 1];
								}
							}
						}
					}
				}
			}
		}

		void GetPolynomials(Vector3<float> const &F, Vector3<float> const &FX,
							Vector3<float> const &FY, Vector3<float> const &FZ, Vector3<float> const &FXY,
							Vector3<float> const &FXZ, Vector3<float> const &FYZ, Vector3<float> const &FXYZ) {
			int xBoundM1 = mXBound - 1;
			int yBoundM1 = mYBound - 1;
			int zBoundM1 = mZBound - 1;
			for (int iz = 0; iz < zBoundM1; ++iz) {
				for (int iy = 0; iy < yBoundM1; ++iy) {
					for (int ix = 0; ix < xBoundM1; ++ix) {
						// Note the 'transposing' of the 2x2x2 blocks (to match
						// notation used in the polynomial definition).
						float G[2][2][2] =
								{
										{
												{
														F[iz][iy][ix],
														F[iz + 1][iy][ix]
												},
												{
														F[iz][iy + 1][ix],
														F[iz + 1][iy + 1][ix]
												}
										},
										{
												{
														F[iz][iy][ix + 1],
														F[iz + 1][iy][ix + 1]
												},
												{
														F[iz][iy + 1][ix + 1],
														F[iz + 1][iy + 1][ix + 1]
												}
										}
								};

						float GX[2][2][2] =
								{
										{
												{
														FX[iz][iy][ix],
														FX[iz + 1][iy][ix]
												},
												{
														FX[iz][iy + 1][ix],
														FX[iz + 1][iy + 1][ix]
												}
										},
										{
												{
														FX[iz][iy][ix + 1],
														FX[iz + 1][iy][ix + 1]
												},
												{
														FX[iz][iy + 1][ix + 1],
														FX[iz + 1][iy + 1][ix + 1]
												}
										}
								};

						float GY[2][2][2] =
								{
										{
												{
														FY[iz][iy][ix],
														FY[iz + 1][iy][ix]
												},
												{
														FY[iz][iy + 1][ix],
														FY[iz + 1][iy + 1][ix]
												}
										},
										{
												{
														FY[iz][iy][ix + 1],
														FY[iz + 1][iy][ix + 1]
												},
												{
														FY[iz][iy + 1][ix + 1],
														FY[iz + 1][iy + 1][ix + 1]
												}
										}
								};

						float GZ[2][2][2] =
								{
										{
												{
														FZ[iz][iy][ix],
														FZ[iz + 1][iy][ix]
												},
												{
														FZ[iz][iy + 1][ix],
														FZ[iz + 1][iy + 1][ix]
												}
										},
										{
												{
														FZ[iz][iy][ix + 1],
														FZ[iz + 1][iy][ix + 1]
												},
												{
														FZ[iz][iy + 1][ix + 1],
														FZ[iz + 1][iy + 1][ix + 1]
												}
										}
								};

						float GXY[2][2][2] =
								{
										{
												{
														FXY[iz][iy][ix],
														FXY[iz + 1][iy][ix]
												},
												{
														FXY[iz][iy + 1][ix],
														FXY[iz + 1][iy + 1][ix]
												}
										},
										{
												{
														FXY[iz][iy][ix + 1],
														FXY[iz + 1][iy][ix + 1]
												},
												{
														FXY[iz][iy + 1][ix + 1],
														FXY[iz + 1][iy + 1][ix + 1]
												}
										}
								};

						float GXZ[2][2][2] =
								{
										{
												{
														FXZ[iz][iy][ix],
														FXZ[iz + 1][iy][ix]
												},
												{
														FXZ[iz][iy + 1][ix],
														FXZ[iz + 1][iy + 1][ix]
												}
										},
										{
												{
														FXZ[iz][iy][ix + 1],
														FXZ[iz + 1][iy][ix + 1]
												},
												{
														FXZ[iz][iy + 1][ix + 1],
														FXZ[iz + 1][iy + 1][ix + 1]
												}
										}
								};

						float GYZ[2][2][2] =
								{
										{
												{
														FYZ[iz][iy][ix],
														FYZ[iz + 1][iy][ix]
												},
												{
														FYZ[iz][iy + 1][ix],
														FYZ[iz + 1][iy + 1][ix]
												}
										},
										{
												{
														FYZ[iz][iy][ix + 1],
														FYZ[iz + 1][iy][ix + 1]
												},
												{
														FYZ[iz][iy + 1][ix + 1],
														FYZ[iz + 1][iy + 1][ix + 1]
												}
										}
								};

						float GXYZ[2][2][2] =
								{
										{
												{
														FXYZ[iz][iy][ix],
														FXYZ[iz + 1][iy][ix]
												},
												{
														FXYZ[iz][iy + 1][ix],
														FXYZ[iz + 1][iy + 1][ix]
												}
										},
										{
												{
														FXYZ[iz][iy][ix + 1],
														FXYZ[iz + 1][iy][ix + 1]
												},
												{
														FXYZ[iz][iy + 1][ix + 1],
														FXYZ[iz + 1][iy + 1][ix + 1]
												}
										}
								};

						Construct(mPoly[iz][iy][ix], G, GX, GY, GZ, GXY, GXZ, GYZ, GXYZ);
					}
				}
			}
		}

		float ComputeDerivative(float const *slope) const {
			if (slope[1] != slope[2]) {
				if (slope[0] != slope[1]) {
					if (slope[2] != slope[3]) {
						float ad0 = std::fabs(slope[3] - slope[2]);
						float ad1 = std::fabs(slope[0] - slope[1]);
						return (ad0 * slope[1] + ad1 * slope[2]) / (ad0 + ad1);
					} else {
						return slope[2];
					}
				} else {
					if (slope[2] != slope[3]) {
						return slope[1];
					} else {
						return (float) 0.5 * (slope[1] + slope[2]);
					}
				}
			} else {
				return slope[1];
			}
		}

		void Construct(Polynomial &poly,
					   float const F[2][2][2], float const FX[2][2][2], float const FY[2][2][2],
					   float const FZ[2][2][2], float const FXY[2][2][2], float const FXZ[2][2][2],
					   float const FYZ[2][2][2], float const FXYZ[2][2][2]) {
			float dx = mXSpacing, dy = mYSpacing, dz = mZSpacing;
			float invDX = (float) 1 / dx, invDX2 = invDX * invDX;
			float invDY = (float) 1 / dy, invDY2 = invDY * invDY;
			float invDZ = (float) 1 / dz, invDZ2 = invDZ * invDZ;
			float b0, b1, b2, b3, b4, b5, b6, b7;

			poly.A(0, 0, 0) = F[0][0][0];
			poly.A(1, 0, 0) = FX[0][0][0];
			poly.A(0, 1, 0) = FY[0][0][0];
			poly.A(0, 0, 1) = FZ[0][0][0];
			poly.A(1, 1, 0) = FXY[0][0][0];
			poly.A(1, 0, 1) = FXZ[0][0][0];
			poly.A(0, 1, 1) = FYZ[0][0][0];
			poly.A(1, 1, 1) = FXYZ[0][0][0];

			// solve for Aij0
			b0 = (F[1][0][0] - poly(0, 0, 0, dx, (float) 0, (float) 0)) * invDX2;
			b1 = (FX[1][0][0] - poly(1, 0, 0, dx, (float) 0, (float) 0)) * invDX;
			poly.A(2, 0, 0) = (float) 3 * b0 - b1;
			poly.A(3, 0, 0) = ((float) -2 * b0 + b1) * invDX;

			b0 = (F[0][1][0] - poly(0, 0, 0, (float) 0, dy, (float) 0)) * invDY2;
			b1 = (FY[0][1][0] - poly(0, 1, 0, (float) 0, dy, (float) 0)) * invDY;
			poly.A(0, 2, 0) = (float) 3 * b0 - b1;
			poly.A(0, 3, 0) = ((float) -2 * b0 + b1) * invDY;

			b0 = (FY[1][0][0] - poly(0, 1, 0, dx, (float) 0, (float) 0)) * invDX2;
			b1 = (FXY[1][0][0] - poly(1, 1, 0, dx, (float) 0, (float) 0)) * invDX;
			poly.A(2, 1, 0) = (float) 3 * b0 - b1;
			poly.A(3, 1, 0) = ((float) -2 * b0 + b1) * invDX;

			b0 = (FX[0][1][0] - poly(1, 0, 0, (float) 0, dy, (float) 0)) * invDY2;
			b1 = (FXY[0][1][0] - poly(1, 1, 0, (float) 0, dy, (float) 0)) * invDY;
			poly.A(1, 2, 0) = (float) 3 * b0 - b1;
			poly.A(1, 3, 0) = ((float) -2 * b0 + b1) * invDY;

			b0 = (F[1][1][0] - poly(0, 0, 0, dx, dy, (float) 0)) * invDX2 * invDY2;
			b1 = (FX[1][1][0] - poly(1, 0, 0, dx, dy, (float) 0)) * invDX * invDY2;
			b2 = (FY[1][1][0] - poly(0, 1, 0, dx, dy, (float) 0)) * invDX2 * invDY;
			b3 = (FXY[1][1][0] - poly(1, 1, 0, dx, dy, (float) 0)) * invDX * invDY;
			poly.A(2, 2, 0) = (float) 9 * b0 - (float) 3 * b1 - (float) 3 * b2 + b3;
			poly.A(3, 2, 0) = ((float) -6 * b0 + (float) 3 * b1 + (float) 2 * b2 - b3) * invDX;
			poly.A(2, 3, 0) = ((float) -6 * b0 + (float) 2 * b1 + (float) 3 * b2 - b3) * invDY;
			poly.A(3, 3, 0) = ((float) 4 * b0 - (float) 2 * b1 - (float) 2 * b2 + b3) * invDX * invDY;

			// solve for Ai0k
			b0 = (F[0][0][1] - poly(0, 0, 0, (float) 0, (float) 0, dz)) * invDZ2;
			b1 = (FZ[0][0][1] - poly(0, 0, 1, (float) 0, (float) 0, dz)) * invDZ;
			poly.A(0, 0, 2) = (float) 3 * b0 - b1;
			poly.A(0, 0, 3) = ((float) -2 * b0 + b1) * invDZ;

			b0 = (FZ[1][0][0] - poly(0, 0, 1, dx, (float) 0, (float) 0)) * invDX2;
			b1 = (FXZ[1][0][0] - poly(1, 0, 1, dx, (float) 0, (float) 0)) * invDX;
			poly.A(2, 0, 1) = (float) 3 * b0 - b1;
			poly.A(3, 0, 1) = ((float) -2 * b0 + b1) * invDX;

			b0 = (FX[0][0][1] - poly(1, 0, 0, (float) 0, (float) 0, dz)) * invDZ2;
			b1 = (FXZ[0][0][1] - poly(1, 0, 1, (float) 0, (float) 0, dz)) * invDZ;
			poly.A(1, 0, 2) = (float) 3 * b0 - b1;
			poly.A(1, 0, 3) = ((float) -2 * b0 + b1) * invDZ;

			b0 = (F[1][0][1] - poly(0, 0, 0, dx, (float) 0, dz)) * invDX2 * invDZ2;
			b1 = (FX[1][0][1] - poly(1, 0, 0, dx, (float) 0, dz)) * invDX * invDZ2;
			b2 = (FZ[1][0][1] - poly(0, 0, 1, dx, (float) 0, dz)) * invDX2 * invDZ;
			b3 = (FXZ[1][0][1] - poly(1, 0, 1, dx, (float) 0, dz)) * invDX * invDZ;
			poly.A(2, 0, 2) = (float) 9 * b0 - (float) 3 * b1 - (float) 3 * b2 + b3;
			poly.A(3, 0, 2) = ((float) -6 * b0 + (float) 3 * b1 + (float) 2 * b2 - b3) * invDX;
			poly.A(2, 0, 3) = ((float) -6 * b0 + (float) 2 * b1 + (float) 3 * b2 - b3) * invDZ;
			poly.A(3, 0, 3) = ((float) 4 * b0 - (float) 2 * b1 - (float) 2 * b2 + b3) * invDX * invDZ;

			// solve for A0jk
			b0 = (FZ[0][1][0] - poly(0, 0, 1, (float) 0, dy, (float) 0)) * invDY2;
			b1 = (FYZ[0][1][0] - poly(0, 1, 1, (float) 0, dy, (float) 0)) * invDY;
			poly.A(0, 2, 1) = (float) 3 * b0 - b1;
			poly.A(0, 3, 1) = ((float) -2 * b0 + b1) * invDY;

			b0 = (FY[0][0][1] - poly(0, 1, 0, (float) 0, (float) 0, dz)) * invDZ2;
			b1 = (FYZ[0][0][1] - poly(0, 1, 1, (float) 0, (float) 0, dz)) * invDZ;
			poly.A(0, 1, 2) = (float) 3 * b0 - b1;
			poly.A(0, 1, 3) = ((float) -2 * b0 + b1) * invDZ;

			b0 = (F[0][1][1] - poly(0, 0, 0, (float) 0, dy, dz)) * invDY2 * invDZ2;
			b1 = (FY[0][1][1] - poly(0, 1, 0, (float) 0, dy, dz)) * invDY * invDZ2;
			b2 = (FZ[0][1][1] - poly(0, 0, 1, (float) 0, dy, dz)) * invDY2 * invDZ;
			b3 = (FYZ[0][1][1] - poly(0, 1, 1, (float) 0, dy, dz)) * invDY * invDZ;
			poly.A(0, 2, 2) = (float) 9 * b0 - (float) 3 * b1 - (float) 3 * b2 + b3;
			poly.A(0, 3, 2) = ((float) -6 * b0 + (float) 3 * b1 + (float) 2 * b2 - b3) * invDY;
			poly.A(0, 2, 3) = ((float) -6 * b0 + (float) 2 * b1 + (float) 3 * b2 - b3) * invDZ;
			poly.A(0, 3, 3) = ((float) 4 * b0 - (float) 2 * b1 - (float) 2 * b2 + b3) * invDY * invDZ;

			// solve for Aij1
			b0 = (FYZ[1][0][0] - poly(0, 1, 1, dx, (float) 0, (float) 0)) * invDX2;
			b1 = (FXYZ[1][0][0] - poly(1, 1, 1, dx, (float) 0, (float) 0)) * invDX;
			poly.A(2, 1, 1) = (float) 3 * b0 - b1;
			poly.A(3, 1, 1) = ((float) -2 * b0 + b1) * invDX;

			b0 = (FXZ[0][1][0] - poly(1, 0, 1, (float) 0, dy, (float) 0)) * invDY2;
			b1 = (FXYZ[0][1][0] - poly(1, 1, 1, (float) 0, dy, (float) 0)) * invDY;
			poly.A(1, 2, 1) = (float) 3 * b0 - b1;
			poly.A(1, 3, 1) = ((float) -2 * b0 + b1) * invDY;

			b0 = (FZ[1][1][0] - poly(0, 0, 1, dx, dy, (float) 0)) * invDX2 * invDY2;
			b1 = (FXZ[1][1][0] - poly(1, 0, 1, dx, dy, (float) 0)) * invDX * invDY2;
			b2 = (FYZ[1][1][0] - poly(0, 1, 1, dx, dy, (float) 0)) * invDX2 * invDY;
			b3 = (FXYZ[1][1][0] - poly(1, 1, 1, dx, dy, (float) 0)) * invDX * invDY;
			poly.A(2, 2, 1) = (float) 9 * b0 - (float) 3 * b1 - (float) 3 * b2 + b3;
			poly.A(3, 2, 1) = ((float) -6 * b0 + (float) 3 * b1 + (float) 2 * b2 - b3) * invDX;
			poly.A(2, 3, 1) = ((float) -6 * b0 + (float) 2 * b1 + (float) 3 * b2 - b3) * invDY;
			poly.A(3, 3, 1) = ((float) 4 * b0 - (float) 2 * b1 - (float) 2 * b2 + b3) * invDX * invDY;

			// solve for Ai1k
			b0 = (FXY[0][0][1] - poly(1, 1, 0, (float) 0, (float) 0, dz)) * invDZ2;
			b1 = (FXYZ[0][0][1] - poly(1, 1, 1, (float) 0, (float) 0, dz)) * invDZ;
			poly.A(1, 1, 2) = (float) 3 * b0 - b1;
			poly.A(1, 1, 3) = ((float) -2 * b0 + b1) * invDZ;

			b0 = (FY[1][0][1] - poly(0, 1, 0, dx, (float) 0, dz)) * invDX2 * invDZ2;
			b1 = (FXY[1][0][1] - poly(1, 1, 0, dx, (float) 0, dz)) * invDX * invDZ2;
			b2 = (FYZ[1][0][1] - poly(0, 1, 1, dx, (float) 0, dz)) * invDX2 * invDZ;
			b3 = (FXYZ[1][0][1] - poly(1, 1, 1, dx, (float) 0, dz)) * invDX * invDZ;
			poly.A(2, 1, 2) = (float) 9 * b0 - (float) 3 * b1 - (float) 3 * b2 + b3;
			poly.A(3, 1, 2) = ((float) -6 * b0 + (float) 3 * b1 + (float) 2 * b2 - b3) * invDX;
			poly.A(2, 1, 3) = ((float) -6 * b0 + (float) 2 * b1 + (float) 3 * b2 - b3) * invDZ;
			poly.A(3, 1, 3) = ((float) 4 * b0 - (float) 2 * b1 - (float) 2 * b2 + b3) * invDX * invDZ;

			// solve for A1jk
			b0 = (FX[0][1][1] - poly(1, 0, 0, (float) 0, dy, dz)) * invDY2 * invDZ2;
			b1 = (FXY[0][1][1] - poly(1, 1, 0, (float) 0, dy, dz)) * invDY * invDZ2;
			b2 = (FXZ[0][1][1] - poly(1, 0, 1, (float) 0, dy, dz)) * invDY2 * invDZ;
			b3 = (FXYZ[0][1][1] - poly(1, 1, 1, (float) 0, dy, dz)) * invDY * invDZ;
			poly.A(1, 2, 2) = (float) 9 * b0 - (float) 3 * b1 - (float) 3 * b2 + b3;
			poly.A(1, 3, 2) = ((float) -6 * b0 + (float) 3 * b1 + (float) 2 * b2 - b3) * invDY;
			poly.A(1, 2, 3) = ((float) -6 * b0 + (float) 2 * b1 + (float) 3 * b2 - b3) * invDZ;
			poly.A(1, 3, 3) = ((float) 4 * b0 - (float) 2 * b1 - (float) 2 * b2 + b3) * invDY * invDZ;

			// solve for remaining Aijk with i >= 2, j >= 2, k >= 2
			b0 = (F[1][1][1] - poly(0, 0, 0, dx, dy, dz)) * invDX2 * invDY2 * invDZ2;
			b1 = (FX[1][1][1] - poly(1, 0, 0, dx, dy, dz)) * invDX * invDY2 * invDZ2;
			b2 = (FY[1][1][1] - poly(0, 1, 0, dx, dy, dz)) * invDX2 * invDY * invDZ2;
			b3 = (FZ[1][1][1] - poly(0, 0, 1, dx, dy, dz)) * invDX2 * invDY2 * invDZ;
			b4 = (FXY[1][1][1] - poly(1, 1, 0, dx, dy, dz)) * invDX * invDY * invDZ2;
			b5 = (FXZ[1][1][1] - poly(1, 0, 1, dx, dy, dz)) * invDX * invDY2 * invDZ;
			b6 = (FYZ[1][1][1] - poly(0, 1, 1, dx, dy, dz)) * invDX2 * invDY * invDZ;
			b7 = (FXYZ[1][1][1] - poly(1, 1, 1, dx, dy, dz)) * invDX * invDY * invDZ;
			poly.A(2, 2, 2) = (float) 27 * b0 - (float) 9 * b1 - (float) 9 * b2 -
							  (float) 9 * b3 + (float) 3 * b4 + (float) 3 * b5 + (float) 3 * b6 - b7;
			poly.A(3, 2, 2) = ((float) -18 * b0 + (float) 9 * b1 + (float) 6 * b2 +
							   (float) 6 * b3 - (float) 3 * b4 - (float) 3 * b5 - (float) 2 * b6 + b7) * invDX;
			poly.A(2, 3, 2) = ((float) -18 * b0 + (float) 6 * b1 + (float) 9 * b2 +
							   (float) 6 * b3 - (float) 3 * b4 - (float) 2 * b5 - (float) 3 * b6 + b7) * invDY;
			poly.A(2, 2, 3) = ((float) -18 * b0 + (float) 6 * b1 + (float) 6 * b2 +
							   (float) 9 * b3 - (float) 2 * b4 - (float) 3 * b5 - (float) 3 * b6 + b7) * invDZ;
			poly.A(3, 3, 2) = ((float) 12 * b0 - (float) 6 * b1 - (float) 6 * b2 -
							   (float) 4 * b3 + (float) 3 * b4 + (float) 2 * b5 + (float) 2 * b6 - b7) *
							  invDX * invDY;
			poly.A(3, 2, 3) = ((float) 12 * b0 - (float) 6 * b1 - (float) 4 * b2 -
							   (float) 6 * b3 + (float) 2 * b4 + (float) 3 * b5 + (float) 2 * b6 - b7) *
							  invDX * invDZ;
			poly.A(2, 3, 3) = ((float) 12 * b0 - (float) 4 * b1 - (float) 6 * b2 -
							   (float) 6 * b3 + (float) 2 * b4 + (float) 2 * b5 + (float) 3 * b6 - b7) *
							  invDY * invDZ;
			poly.A(3, 3, 3) = ((float) -8 * b0 + (float) 4 * b1 + (float) 4 * b2 +
							   (float) 4 * b3 - (float) 2 * b4 - (float) 2 * b5 - (float) 2 * b6 + b7) *
							  invDX * invDY * invDZ;
		}

		void XLookup(float x, int &xIndex, float &dx) const {
			for (xIndex = 0; xIndex + 1 < mXBound; ++xIndex) {
				if (x < mXMin + mXSpacing * (xIndex + 1)) {
					dx = x - (mXMin + mXSpacing * xIndex);
					return;
				}
			}

			--xIndex;
			dx = x - (mXMin + mXSpacing * xIndex);
		}

		void YLookup(float y, int &yIndex, float &dy) const {
			for (yIndex = 0; yIndex + 1 < mYBound; ++yIndex) {
				if (y < mYMin + mYSpacing * (yIndex + 1)) {
					dy = y - (mYMin + mYSpacing * yIndex);
					return;
				}
			}

			--yIndex;
			dy = y - (mYMin + mYSpacing * yIndex);
		}

		void ZLookup(float z, int &zIndex, float &dz) const {
			for (zIndex = 0; zIndex + 1 < mZBound; ++zIndex) {
				if (z < mZMin + mZSpacing * (zIndex + 1)) {
					dz = z - (mZMin + mZSpacing * zIndex);
					return;
				}
			}

			--zIndex;
			dz = z - (mZMin + mZSpacing * zIndex);
		}

		int mXBound, mYBound, mZBound, mQuantity;
		float mXMin, mXMax, mXSpacing;
		float mYMin, mYMax, mYSpacing;
		float mZMin, mZMax, mZSpacing;
		float const *mF;
		Vector3 <Polynomial> mPoly;
	};
}