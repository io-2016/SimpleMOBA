#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "Edge.hpp"
#include <vector>
#include <string>
#include <set>
const float EPS = 1e-3;
const float INF = 1e9;

namespace Geometry {

Vector2d normalizedToLength(Vector2d a, float p);
void convexHull(std::vector<Vector2d> &verts);
void uniquePoints(std::vector<Vector2d> &verts);
bool inRange(float a, float b1, float b2);
bool collision1d(float a1, float a2, float b1, float b2);
bool isLower(const Vector2d& low, const Vector2d& high);
bool angleCmp(const Vector2d &a, const Vector2d &b);
std::vector<Vector2d> simplifyPolygon(std::vector<Vector2d> polygon, float eps);

struct CmpAngleSort {
    Vector2d m_lowest;

    CmpAngleSort(float minX, float minY);
    CmpAngleSort(const Vector2d& mid);
    bool operator()(const Vector2d &a, const Vector2d &b) const;
};

template <class iterator> void angleSort(iterator begin, iterator end) {
    if (begin == end || std::next(begin) == end)
        return;
    iterator lowest = std::min_element(begin, end, isLower);

    std::swap(*lowest, *begin);

    std::sort(std::next(begin), end, CmpAngleSort(*begin));
}

template <class iterator> float area(iterator begin, iterator end) {
    float area = 0;
    iterator p = std::prev(end);
    for (iterator it = begin; it != end; it++)
        area+=(p->x+it->x)*(p->y-it->y), p = it;

    return area*0.5;
}

template <class iterator> bool pointInPolygon(iterator begin,
                                              iterator end,
                                              const Vector2d& p) {
    iterator j = std::prev(end);
    bool oddNodes = false;

    for (iterator i = begin; i != end; i++) {
        if ((i->y < p.y && j->y >= p.y) || (j->y < p.y && i->y >= p.y))
            if (i->x <= p.x || j->x <= p.x)
                oddNodes^=(i->x+(p.y-i->y)/(j->y-i->y)*(j->x-i->x) < p.x);

        j = i;
    }

    return oddNodes;
}

template<typename iterator> void correctWinding(iterator begin, iterator end) {
    if (area(begin, end) > 0)
        std::reverse(begin, end);
    }
}

#endif
