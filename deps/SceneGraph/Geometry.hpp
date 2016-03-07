#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP
#include <QOpenGLFunctions>

namespace SceneGraph {

#undef M_PI
const float M_PI = 3.14159265358979323846;

struct Attribute {
  int tupleSize;
  int primitiveType;
};

class Geometry : public QOpenGLFunctions {
 private:
  GLuint m_vbo;
  std::vector<Attribute> m_attribute;
  uint m_vertexCount;
  uint m_vertexSize;
  void* m_vertexData;
  uint m_vertexDataSize;
  uint m_indexCount;
  uint m_indexType;
  void* m_indexData;
  uint m_indexDataSize;
  uint m_drawingMode;

 public:
  Geometry(std::vector<Attribute> set, uint vertexCount, uint vertexSize,
           uint indexCount = 0, uint indexType = GL_UNSIGNED_INT);
  virtual ~Geometry();

  void allocate(uint vertexCount, uint indexCount);
  void updateVertexData();

  void bind(const int* attributeLocation);
  void release();

  inline const std::vector<Attribute>& attribute() const { return m_attribute; }

  inline uint vertexDataSize() const { return m_vertexDataSize; }
  inline uint indexDataSize() const { return m_indexDataSize; }

  inline uint vertexCount() const { return m_vertexCount; }
  inline void setVertexCount(int t) { m_vertexCount = t; }

  inline uint indexCount() const { return m_indexCount; }
  inline void setIndexCount(int t) { m_indexCount = t; }

  inline uint vertexSize() const { return m_vertexSize; }
  inline uint indexType() const { return m_indexType; }

  inline uint drawingMode() const { return m_drawingMode; }
  inline void setDrawingMode(uint m) { m_drawingMode = m; }

  template <class T = void>
  inline T* vertexData() const {
    return static_cast<T*>(m_vertexData);
  }

  template <class T = void>
  inline T* indexData() const {
    return static_cast<T*>(m_indexData);
  }

  static uint sizeOfType(GLuint type);
};
}

#endif  // GEOMETRY_HPP
