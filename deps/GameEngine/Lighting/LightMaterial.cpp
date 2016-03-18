#include "LightMaterial.hpp"
#include <cassert>
#define GLSL(shader) #shader

LightMaterial::LightMaterial() : m_normalMap() {}

void LightMaterial::Shader::initialize() {
  SceneGraph::Shader::initialize();
  initializeOpenGLFunctions();

  m_id_matrix = program()->uniformLocation("matrix");
  m_id_opacity = program()->uniformLocation("opacity");
  m_id_lightPosition = program()->uniformLocation("lightPosition");
  m_id_attenuation = program()->uniformLocation("attenuation");
  m_id_color = program()->uniformLocation("color");
  m_id_normalMap = program()->uniformLocation("normalMap");
}

void LightMaterial::Shader::activate() {
#ifdef GL_MAX
  glGetIntegerv(GL_BLEND_EQUATION, &m_blendEquation);
  glBlendEquation(GL_MAX);
#else
  glGetIntegerv(GL_BLEND_SRC_RGB, m_blendFunc + 0);
  glGetIntegerv(GL_BLEND_DST_RGB, m_blendFunc + 1);
  glBlendFunc(GL_ONE, GL_ONE);
#endif
}

void LightMaterial::Shader::deactivate() {
#ifdef GL_MAX
  glBlendEquation(m_blendEquation);
#else
  glBlendFunc(m_blendFunc[0], m_blendFunc[1]);
#endif
}

const char* LightMaterial::Shader::vertexShader() const {
  return GLSL(attribute vec4 vertex; uniform mat4 matrix;
              varying vec2 lightcoord; varying vec2 normalcoord;

              void main() {
                lightcoord = vertex.xy;
                normalcoord = 0.5 * ((matrix * vertex).xy + vec2(1.0, 1.0));
                gl_Position = matrix * vertex;
              });
}

const char* LightMaterial::Shader::fragmentShader() const {
  return GLSL(uniform float opacity; uniform vec3 lightPosition;
              uniform vec3 attenuation; uniform vec4 color;
              uniform sampler2D normalMap;

              varying vec2 normalcoord; varying vec2 lightcoord;

              void main() {
                vec3 normalColor = texture2D(normalMap, normalcoord.xy).rgb;
                vec3 lightDir = lightPosition - vec3(lightcoord, 0.0);
                float d = length(lightDir);

                vec3 n = normalize(normalColor * 2.0 - 1.0);
                vec3 l = normalize(lightDir);
                vec3 diffuse = color.rgb * color.a * max(dot(n, l), 0.0);

                float att = 1.0 / (attenuation.x + attenuation.y * d +
                                   attenuation.z * d * d);

                vec3 ambient = vec3(0.0, 0.0, 0.0);
                vec3 intensity = ambient + diffuse * att;

                gl_FragColor = opacity * color * vec4(intensity, 1.0);
              });
}

std::vector<std::string> LightMaterial::Shader::attribute() const {
  return {"vertex"};
}

void LightMaterial::Shader::updateState(const SceneGraph::Material* mat,
                                        const SceneGraph::RenderState& state) {
  const LightMaterial* material = static_cast<const LightMaterial*>(mat);
  if (!material->normalMap() || !material->normalMap()->texture())
    return;

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, material->normalMap()->texture()->texture());
  program()->setUniformValue(m_id_normalMap, 0);
  program()->setUniformValue(m_id_lightPosition, material->lightPosition());
  program()->setUniformValue(m_id_attenuation, material->attenuation());
  program()->setUniformValue(m_id_color, material->color());
  program()->setUniformValue(m_id_matrix, state.matrix());
  program()->setUniformValue(m_id_opacity, 1.f);
}
