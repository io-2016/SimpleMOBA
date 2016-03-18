#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP
#include "SceneGraph/Item.hpp"
#include "SceneGraph/Node.hpp"
#include "SceneGraph/Material.hpp"
#include "SceneGraph/Geometry.hpp"
#include "SceneGraph/ShaderSource.hpp"

class LightSystem;

struct Particle {
  qreal x, y, r;
  qreal dx, dy, xvariation, yvariation;
  uint lifespan, time;
};

class ParticleMaterial : public SceneGraph::Material {
 private:
  SceneGraph::ShaderSource::ShaderNode* m_normalMap;
  uint m_time;
  QColor m_color;
  QVector3D m_lightPosition;

  class ParticleShader : public SceneGraph::Shader, public QOpenGLFunctions {
   private:
    GLint m_blendFunc[2];

    int m_matrix;
    int m_normalmap;
    int m_timestamp;
    int m_color;
    int m_lightposition;

   public:
    void activate();
    void deactivate();

    void initialize();

    const char* vertexShader() const;
    const char* fragmentShader() const;

    void updateState(const Material*, const SceneGraph::RenderState&);
    std::vector<std::string> attribute() const;
  };

 public:
  ParticleMaterial();

  inline void setNormalMap(SceneGraph::ShaderSource::ShaderNode* n) {
    m_normalMap = n;
  }
  inline SceneGraph::ShaderSource::ShaderNode* normalMap() const {
    return m_normalMap;
  }

  inline void setColor(QColor c) { m_color = c; }
  inline QColor color() const { return m_color; }

  inline void setLightPosition(QVector3D p) { m_lightPosition = p; }
  inline QVector3D lightPosition() const { return m_lightPosition; }

  inline void setTime(uint t) { m_time = t; }
  inline uint time() const { return m_time; }

  inline SceneGraph::Shader* shader() const {
    return SceneGraph::Shader::get<ParticleShader>();
  }

  void update();
};

class ParticleSystem : public SceneGraph::Item {
 private:
  friend class ViewWorld;

  std::vector<Particle> m_particle;
  uint m_time;
  LightSystem* m_lightSystem;

  class Node : public SceneGraph::GeometryNode {
   private:
    struct Vertex {
      float x, y, tx, ty;
      float lifespan, time;

      Vertex(QPointF pos, QPointF tcoord, const Particle& p);
    };

    SceneGraph::Geometry m_geometry;
    ParticleMaterial m_material;

    static std::vector<SceneGraph::Attribute> attribute();
    void generateTriangleStrip(GLuint* index, uint size);

   public:
    Node();

    inline ParticleMaterial* material() { return &m_material; }

    void update(const std::vector<Particle>& p);
  };

 protected:
  SceneGraph::Node* synchronize(SceneGraph::Node*);

 public:
  ParticleSystem(SceneGraph::Item* = nullptr);
  ~ParticleSystem();

  inline void setLightSystem(LightSystem* l) { m_lightSystem = l; }
  inline LightSystem* lightSystem() const { return m_lightSystem; }

  void addParticle(Particle);
  void step();

  void addExplosion(QPointF p, qreal r, qreal v, uint particleCount);

  inline uint time() const { return m_time; }
};

#endif  // PARTICLESYSTEM_HPP
