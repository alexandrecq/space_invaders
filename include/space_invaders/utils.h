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


template <typename T>
class RandomNumberGenerator{
public:
	RandomNumberGenerator<T>(T maxValue = 100000);
	void generateNumber(T* numberOut);
private:
	std::mt19937 m_gen;
	std::uniform_int_distribution<int> m_distribution;
};
