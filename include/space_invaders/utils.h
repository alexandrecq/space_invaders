#pragma once

#include <string>
#include <vector>
#include <Eigen/Dense>

using std::string;
using std::vector;
using Eigen::Array2f;
using Eigen::Array3f;

const vector<string> generateTexturePaths(const std::string& baseName, int numTextures);
