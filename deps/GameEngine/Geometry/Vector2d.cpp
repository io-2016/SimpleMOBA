#include "Vector2d.hpp"
#include "Edge.hpp"
#include "Functions.hpp"
#include <Box2D/Box2D.h>
#include <QPointF>
#include <QSize>
const int BUF_SIZE = 100;

Vector2d::Vector2d() : x(), y() {}

Vector2d::Vector2d(float a, float b) : x(a), y(b) {}

Vector2d::Vector2d(const std::pair<float, float>& a)
    : x(a.first), y(a.second) {}

Vector2d::Vector2d(const b2Vec2& a) : x(a.x), y(a.y) {}

Vector2d::operator b2Vec2() const { return b2Vec2(x, y); }

Vector2d::Vector2d(const QPointF& a) : x(a.x()), y(a.y()) {}

Vector2d::operator QPointF() const { return QPointF(x, y); }

Vector2d::Vector2d(const QSize& a) : x(a.width()), y(a.height()) {}

Vector2d::operator QSize() const { return QSize(x, y); }

bool Vector2d::operator==(const Vector2d& a) const {
  return fabs(x - a.x) < EPS && fabs(y - a.y) < EPS;
}

bool Vector2d::operator!=(const Vector2d& a) const { return !(*this == a); }

Vector2d Vector2d::operator+(const Vector2d& a) const {
  return Vector2d(x + a.x, y + a.y);
}

Vector2d Vector2d::operator-(const Vector2d& a) const {
  return Vector2d(x - a.x, y - a.y);
}

Vector2d Vector2d::operator/(const Vector2d& a) const {
  return Vector2d(x / a.x, y / a.y);
}

Vector2d Vector2d::operator*(const Vector2d& a) const {
  return Vector2d(x * a.x, y * a.y);
}

Vector2d Vector2d::operator-() const { return Vector2d(-x, -y); }

Vector2d operator*(const Vector2d& a, float b) {
  return Vector2d(a.x * b, a.y * b);
}

Vector2d Vector2d::operator+=(const Vector2d& a) { return * this = *this + a; }

Vector2d Vector2d::operator-=(const Vector2d& a) { return * this = *this - a; }

Vector2d operator*(float a, const Vector2d& b) { return b * a; }

Vector2d& Vector2d::rotate(double angle, const Vector2d& start) {
  Vector2d tmp;
  tmp = *this;
  tmp.x -= start.x, tmp.y -= start.y;

  x = cos(angle) * tmp.x - sin(angle) * tmp.y;
  y = sin(angle) * tmp.x + cos(angle) * tmp.y;

  x += start.x, y += start.y;

  return *this;
}

Vector2d& Vector2d::translate(float x, float y) {
  this->x += x;
  this->y += y;

  return *this;
}

Vector2d& Vector2d::translate(const Vector2d& p) { return translate(p.x, p.y); }

Vector2d Vector2d::perpendicular() const { return Vector2d(-y, x); }

float Vector2d::length() const { return sqrt(x * x + y * y); }

float Vector2d::lengthSquared() const { return x * x + y * y; }

void Vector2d::print() const { printf("%f %f\n", x, y); }

const char* Vector2d::toString() const {
  static char buffer[BUF_SIZE];
  sprintf(buffer, "%.8f %.8f", x, y);

  return buffer;
}

Vector2d::Direction Vector2d::direction(const Vector2d& a) const {
  if (fabs(Geometry::crossProduct(*this, a)) < 1e-9) return Straight;

  if (Geometry::crossProduct(*this, a) > 0) return Right;
  return Left;
}

void Vector2d::normalize() {
  float len = length();
  x /= len, y /= len;
}

Vector2d Vector2d::normal() const {
  if (length() < EPS) return *this;
  return 1.f / length() * (*this);
}

Vector2d Vector2d::projection(const Vector2d& a) const {
  float k = (x * a.x + y * a.y) / (x * x + y * y);

  return k * Vector2d(x, y);
}

float Vector2d::angle() { return atan2(y, x); }

double sqr(double q) { return q * q; }

namespace Geometry {

float det3(const Vector2d& a, const Vector2d& b, const Vector2d& c) {
  return a.x * b.y + b.x * c.y + c.x * a.y - a.x * c.y - b.x * a.y - c.x * b.y;
}

float dotProduct(const Vector2d& a, const Vector2d& b) {
  return a.x * b.x + a.y * b.y;
}

float crossProduct(const Vector2d& a, const Vector2d& b) {
  return a.x * b.y - a.y * b.x;
}
}
