#ifndef SHADER_HPP
#define SHADER_HPP
#include <QOpenGLShaderProgram>
#define GLSL(shader) #shader

namespace SceneGraph {

namespace Private {

template <class T>
class SingletonImpl {
 private:
  T* m_data;

  SingletonImpl() : m_data(new T) {}

 public:
  static SingletonImpl* instance() {
    static SingletonImpl data;
    return &data;
  }

  T* data() const { return m_data; }
};
}

class Material;
class RenderState;

const int MAX_ATTRIBUTE_COUNT = 8;

class Shader {
 private:
  bool m_initialized;
  QOpenGLShaderProgram m_program;
  int m_attributeLocation[MAX_ATTRIBUTE_COUNT];

 public:
  Shader();

  inline QOpenGLShaderProgram* program() { return &m_program; }
  inline bool bind() { return program()->bind(); }
  inline bool initialized() const { return m_initialized; }

  virtual void initialize();

  virtual void activate() = 0;
  virtual void deactivate() = 0;
  virtual const char* vertexShader() const = 0;
  virtual const char* fragmentShader() const = 0;
  virtual void updateState(const Material*, const RenderState&) = 0;
  virtual std::vector<std::string> attribute() const = 0;
  const int* attributeLocation() const { return m_attributeLocation; }

  template <class T>
  static T* get() {
    return Private::SingletonImpl<T>::instance()->data();
  }
};
}

#endif  // SHADER_HPP
