#ifndef ANIMATIONPOOL_H
#define ANIMATIONPOOL_H

#include <QMap>
#include <QPixmap>
#include "singleton.h"

template <typename AnimationEnum>
class AnimationPool {
public:
  struct LoadException : public std::exception {
    virtual const char* what() const noexcept {
      return "Animation not loaded\n";
    }
  };

  void load(AnimationEnum id, QString filename, int nFrames, int height, int width) {
      QPixmap frames(filename);
      for (int nFrame = 0; nFrame < nFrames; ++nFrame)
          m_animations[id].push_back(cropFrame(frames, nFrames, height, width, nFrame));
  }

  QVector<QPixmap*>& get(AnimationEnum id) throw(LoadException) {
      if (false == m_animations.contains(id))
          throw LoadException();
      return m_animations[id];
  }

private:
  QPixmap* cropFrame(const QPixmap frames, const int nFrames,
                     const int height, const int width, const int nFrame) {
      const int frameWidth = frames.width() / nFrames,
              frameHeight = frames.height();
      return new QPixmap(
          frames.copy(nFrame*frameWidth, 0, frameWidth, frameHeight)
              .scaled(width, height, Qt::IgnoreAspectRatio, Qt::FastTransformation)
          );
  }

  QMap<AnimationEnum, QVector<QPixmap*> > m_animations;

  friend class Singleton<AnimationPool<AnimationEnum> >;
};

#endif // ANIMATIONPOOL_H
