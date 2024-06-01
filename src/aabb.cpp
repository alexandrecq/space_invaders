#include "space_invaders/aabb.h"

bool AABB::intersects(AABB other) const {
    return (m_max.x() >= other.m_min.x() && m_min.x() <= other.m_max.x() &&
            m_max.y() >= other.m_min.y() && m_min.y() <= other.m_max.y());
}

void AABB::print() {
    printf("(%.3f, %.3f) -> (%.3f, %.3f)\n", m_min.x(), m_min.y(), m_max.x(), m_max.y());
}
