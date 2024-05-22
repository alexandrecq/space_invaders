#pragma once

#include <Eigen/Dense>

using Eigen::Array2f;

class AABB {
public:
	AABB(Array2f min, Array2f max) : m_min(min), m_max(max) {};
	bool intersects(AABB other);
	void print();
private:
	Array2f m_min;
	Array2f m_max;
};
