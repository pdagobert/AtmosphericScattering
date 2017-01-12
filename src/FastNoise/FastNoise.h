// FastNoise.h
//
// MIT License
//
// Copyright(c) 2016 Jordan Peck
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// The developer's email is jorzixdan.me2@gzixmail.com (for great email, take
// off every 'zix'.)
//

#ifndef FASTNOISE_H
#define FASTNOISE_H

#include <cstdint>

class FastNoise
{
public:
	FastNoise(int seed = 1337) { SetSeed(seed); CalculateFractalBounding(); };
	~FastNoise() { delete m_cellularNoiseLookup; }

	enum NoiseType { Value, ValueFractal, Gradient, GradientFractal, Simplex, SimplexFractal, Cellular, WhiteNoise };
	enum Interp { Linear, Hermite, Quintic };
	enum FractalType { FBM, Billow, RigidMulti };
	enum CellularDistanceFunction { Euclidean, Manhattan, Natural };
	enum CellularReturnType { CellValue, NoiseLookup, Distance, Distance2, Distance2Add, Distance2Sub, Distance2Mul, Distance2Div };

	void SetSeed(int seed);
	int GetSeed(void) const { return m_seed; }
	void SetFrequency(double frequency) { m_frequency = frequency; }
	void SetInterp(Interp interp) { m_interp = interp; }
	void SetNoiseType(NoiseType noiseType) { m_noiseType = noiseType; }

	void SetFractalOctaves(unsigned int octaves) { m_octaves = octaves; CalculateFractalBounding(); }
	void SetFractalLacunarity(double lacunarity) { m_lacunarity = lacunarity; }
	void SetFractalGain(double gain) { m_gain = gain; CalculateFractalBounding(); }
	void SetFractalType(FractalType fractalType) { m_fractalType = fractalType; }

	void SetCellularDistanceFunction(CellularDistanceFunction cellularDistanceFunction) { m_cellularDistanceFunction = cellularDistanceFunction; }
	void SetCellularReturnType(CellularReturnType cellularReturnType) { m_cellularReturnType = cellularReturnType; }
	void SetCellularNoiseLookup(FastNoise* noise) { m_cellularNoiseLookup = noise; }

	void SetPositionWarpAmp(double positionWarpAmp) { m_positionWarpAmp = positionWarpAmp / 0.45f; }

	//2D												
	double GetValue(double x, double y);					
	double GetValueFractal(double x, double y);			

	double GetGradient(double x, double y);				
	double GetGradientFractal(double x, double y);			

	double GetSimplex(double x, double y);					
	double GetSimplexFractal(double x, double y);			

	double GetCellular(double x, double y);				

	double GetWhiteNoise(double x, double y);				
	double GetWhiteNoiseInt(int x, int y);				

	double GetNoise(double x, double y);

	void PositionWarp(double& x, double& y);
	void PositionWarpFractal(double& x, double& y);

	//3D												
	double GetValue(double x, double y, double z);			
	double GetValueFractal(double x, double y, double z);	

	double GetGradient(double x, double y, double z);		
	double GetGradientFractal(double x, double y, double z);

	double GetSimplex(double x, double y, double z);		
	double GetSimplexFractal(double x, double y, double z);	

	double GetCellular(double x, double y, double z);		

	double GetWhiteNoise(double x, double y, double z);		
	double GetWhiteNoiseInt(int x, int y, int z);

	double GetNoise(double x, double y, double z);

	void PositionWarp(double& x, double& y, double& z);
	void PositionWarpFractal(double& x, double& y, double& z);

	//4D
	double GetSimplex(double x, double y, double z, double w);

	double GetWhiteNoise(double x, double y, double z, double w);
	double GetWhiteNoiseInt(int x, int y, int z, int w);

protected:
	unsigned char m_perm[512];
	unsigned char m_perm12[512];

	int m_seed = 1337;
	double m_frequency = 0.01f;
	Interp m_interp = Quintic;
	NoiseType m_noiseType = Simplex;

	unsigned int m_octaves = 3;
	double m_lacunarity = 2.0f;
	double m_gain = 0.5f;
	FractalType m_fractalType = FBM;

	double m_fractalBounding;
	void CalculateFractalBounding()
	{
		double amp = m_gain;
		double ampFractal = 1.0f;
		for (unsigned int i = 1; i < m_octaves; i++)
		{
			ampFractal += amp;
			amp *= m_gain;
		}
		m_fractalBounding = 1.0f / ampFractal;
	}

	CellularDistanceFunction m_cellularDistanceFunction = Euclidean;
	CellularReturnType m_cellularReturnType = CellValue;
	FastNoise* m_cellularNoiseLookup = nullptr;

	double m_positionWarpAmp = 1.0f / 0.45f;

	//2D
	double SingleValueFractalFBM(double x, double y);
	double SingleValueFractalBillow(double x, double y);
	double SingleValueFractalRigidMulti(double x, double y);
	double SingleValue(unsigned char offset, double x, double y);

	double SingleGradientFractalFBM(double x, double y);
	double SingleGradientFractalBillow(double x, double y);
	double SingleGradientFractalRigidMulti(double x, double y);
	double SingleGradient(unsigned char offset, double x, double y);

	double SingleSimplexFractalFBM(double x, double y);
	double SingleSimplexFractalBillow(double x, double y);
	double SingleSimplexFractalRigidMulti(double x, double y);
	double SingleSimplex(unsigned char offset, double x, double y);

	double SingleCellular(double x, double y);
	double SingleCellular2Edge(double x, double y);

	void SinglePositionWarp(unsigned char offset, double warpAmp, double frequency, double& x, double& y);

	//3D
	double SingleValueFractalFBM(double x, double y, double z);
	double SingleValueFractalBillow(double x, double y, double z);
	double SingleValueFractalRigidMulti(double x, double y, double z);
	double SingleValue(unsigned char offset, double x, double y, double z);

	double SingleGradientFractalFBM(double x, double y, double z);
	double SingleGradientFractalBillow(double x, double y, double z);
	double SingleGradientFractalRigidMulti(double x, double y, double z);
	double SingleGradient(unsigned char offset, double x, double y, double z);

	double SingleSimplexFractalFBM(double x, double y, double z);
	double SingleSimplexFractalBillow(double x, double y, double z);
	double SingleSimplexFractalRigidMulti(double x, double y, double z);
	double SingleSimplex(unsigned char offset, double x, double y, double z);

	double SingleCellular(double x, double y, double z);
	double SingleCellular2Edge(double x, double y, double z);

	void SinglePositionWarp(unsigned char offset, double warpAmp, double frequency, double& x, double& y, double& z);

	//4D
	double SingleSimplex(unsigned char offset, double x, double y, double z, double w);

private:
	inline unsigned char Index2D_12(unsigned char offset, int x, int y);
	inline unsigned char Index3D_12(unsigned char offset, int64_t x, int64_t y, int64_t z);
	inline unsigned char Index4D_32(unsigned char offset, int x, int y, int z, int w);
	inline unsigned char Index2D_256(unsigned char offset, int x, int y);
	inline unsigned char Index3D_256(unsigned char offset, int x, int y, int z);
	inline unsigned char Index4D_256(unsigned char offset, int x, int y, int z, int w);

	inline double ValCoord2DFast(unsigned char offset, int x, int y);
	inline double ValCoord3DFast(unsigned char offset, int x, int y, int z);
	inline double GradCoord2D(unsigned char offset, int x, int y, double xd, double yd);
	inline double GradCoord3D(unsigned char offset, int64_t x, int64_t y, int64_t z, double xd, double yd, double zd);
	inline double GradCoord4D(unsigned char offset, int x, int y, int z, int w, double xd, double yd, double zd, double wd);
};

#endif
