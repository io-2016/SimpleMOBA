#ifndef LIGHTMATERIAL_HPP
#define LIGHTMATERIAL_HPP
#include "SceneGraph/Material.hpp"
#include "SceneGraph/Renderer.hpp"
#include "SceneGraph/ShaderSource.hpp"
#include <QOpenGLFunctions>
#include <QColor>

class LightMaterial : public SceneGraph::Material {
 private:
  class Shader : public SceneGraph::Shader, public QOpenGLFunctions {
   private:
    int m_id_matrix;
    int m_id_opacity;
    int m_id_lightPosition;
    int m_id_attenuation;
    int m_id_color;
    int m_id_normalMap;

    GLint m_blendEquation;
    GLint m_blendFunc[2];

    void initialize();
    void activate();
    void deactivate();
    const char* vertexShader() const;
    const char* fragmentShader() const;

   public:
    std::vector<std::string> attribute() const;

    void updateState(const Material* mat, const SceneGraph::RenderState& state);
  };

  QColor m_color;
  QVector3D m_lightPosition;
  QVector3D m_attenuation;
  SceneGraph::ShaderSource::ShaderNode* m_normalMap;

 public:
  LightMaterial();

  SceneGraph::Shader* shader() const {
    return SceneGraph::Shader::get<Shader>();
  }

  inline QColor color() const { return m_color; }
  inline void setColor(QColor c) { m_color = c; }

  inline QVector3D lightPosition() const { return m_lightPosition; }
  inline void setLightPosition(QVector3D p) { m_lightPosition = p; }

  inline QVector3D attenuation() const { return m_attenuation; }
  inline void setAttenuation(QVector3D att) { m_attenuation = att; }

  inline SceneGraph::ShaderSource::ShaderNode* normalMap() const {
    return m_normalMap;
  }
  inline void setNormalMap(SceneGraph::ShaderSource::ShaderNode* t) {
    m_normalMap = t;
  }
};
#endif  // LIGHTMATERIAL_HPP
