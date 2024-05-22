#pragma once

#include <random>
#include <string>
#include <vector>
#include <Eigen/Dense>

using std::string;
using std::vector;
using Eigen::Array2f;
using Eigen::Array3f;

const vector<string> generateTexturePaths(const std::string& baseName, int numTextures);


class RandomNumberGenerator{
public:
	RandomNumberGenerator();
	void generateNumber(float* numberOut);
private:
	std::mt19937 m_gen;
	std::uniform_real_distribution<> m_distribution;
};
