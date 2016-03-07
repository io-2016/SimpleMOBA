#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP
#include <algorithm>

struct b2Vec2;
class QPointF;
class QPoint;
class QSize;

class Vector2d {
    public:
        float x, y;

        enum Direction {
            Left, Right, Straight
        };

        Vector2d();
        Vector2d(float a, float b);
        Vector2d(const std::pair<float, float>&);

        explicit Vector2d(const b2Vec2&);
        explicit operator b2Vec2() const;

        explicit Vector2d(const QPointF&);
        explicit operator QPointF() const;

        explicit Vector2d(const QSize&);
        explicit operator QSize() const;

        bool operator==(const Vector2d&) const;
        bool operator!=(const Vector2d&) const;
        Vector2d operator+(const Vector2d&) const;
        Vector2d operator-(const Vector2d&) const;
        Vector2d operator/(const Vector2d&) const;
        Vector2d operator*(const Vector2d&) const;
        Vector2d operator-() const;
        Vector2d operator+=(const Vector2d&);
        Vector2d operator-=(const Vector2d&);

        Vector2d& rotate(double angle, const Vector2d& start = Vector2d(0, 0));
        Vector2d& translate(float, float);
        Vector2d& translate(const Vector2d&);
        Vector2d perpendicular() const;
        Vector2d projection(const Vector2d&) const;
        float length() const;
        float lengthSquared() const;
        void print() const;
        const char* toString() const;
        Direction direction(const Vector2d&) const;
        void normalize();
        Vector2d normal() const;
        float angle();
};

namespace Geometry {

float det3(const Vector2d& a, const Vector2d& b, const Vector2d& c);
float dotProduct(const Vector2d&, const Vector2d&);
float crossProduct(const Vector2d&, const Vector2d&);

}

Vector2d operator*(const Vector2d&, float);
Vector2d operator*(float, const Vector2d&);
double sqr(double q);


#endif // VECTOR2D_HPP
