#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP
#include <QPointF>
#include <vector>

class Triangle {
    public:
        std::vector<QPointF> m_vertices;

        Triangle();
        Triangle(const QPointF& p1, const QPointF& p2, const QPointF& p3);
        void setPoint(int id, const QPointF& p);
        const QPointF& getPoint(int id) const;
        const QPointF& operator[](int it) const;
        QPointF& operator[](int it);
        inline std::vector<QPointF>::const_iterator begin() const
        { return m_vertices.begin(); }
        inline std::vector<QPointF>::const_iterator end() const
        { return m_vertices.end(); }
};

namespace Geometry {

void triangulate(const std::vector<QPointF>& polygon,
                 std::vector<Triangle>& result);

void toTriangleStrip(std::vector<QPointF> convexPolygon,
                     std::vector<QPointF>& output);

void toTriangles(std::vector<QPointF> polygon,
                 std::vector<QPointF>& output);

}

#endif // TRIANGLE_HPP
