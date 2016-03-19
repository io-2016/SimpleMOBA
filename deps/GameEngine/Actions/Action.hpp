#ifndef ACTION_HPP
#define ACTION_HPP
#include "SceneGraph/Item.hpp"

class QWorld;
class Action;
class SubAction;

class Callback {
public:
  virtual ~Callback() {}

  virtual void onTriggered() = 0;
};

class ActionObject : public QObject {
 private:
  Q_OBJECT

  Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged)
  Q_PROPERTY(
      QString currentAction READ currentAction NOTIFY currentActionChanged)

  Action* m_action;

 public:
  ActionObject(Action*);

  inline Action* action() const { return m_action; }

  QString currentAction() const;

  bool enabled() const;
  Q_INVOKABLE void toggleEnabled();

  Q_INVOKABLE void finished();

 signals:
  void enabledChanged();
  void currentActionChanged();
};

class Action : public SceneGraph::Item {
 private:
  friend class ActionObject;
  friend class SubAction;

  SubAction* m_currentSubAction;
  bool m_enabled;

  ActionObject* m_object;
  QWorld* m_world;

 protected:
  inline virtual void onInit() {}
  virtual void enabledChanged();

  virtual void subActionFinished(SubAction*);
  virtual void subActionEnabledChanged(SubAction*);

 public:
  Action(SceneGraph::Item* = nullptr, QWorld* = nullptr);

  virtual void reset();

  inline bool enabled() const { return m_enabled; }
  void setEnabled(bool);

  inline ActionObject* object() const { return m_object; }

  inline QWorld* world() const { return m_world; }
  inline void setWorld(QWorld* w) { m_world = w; }

  inline virtual void finished() {}
};

#endif  // ACTION_HPP
