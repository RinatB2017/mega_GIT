#ifndef IMAGEFORMATTER_H
#define IMAGEFORMATTER_H

#include <QTextImageFormat>
#include <QTextDocumentFragment>

#include "Formatter.h"

// Класс для работы с картинками в тексте

// class QTextDocumentFragment;

class ImageFormatter : public Formatter
{
  Q_OBJECT

public:
  ImageFormatter();

  QTextImageFormat imageFormatOnSelect(void);
  QTextImageFormat imageFormatOnCursor(void);

  void editImageProperties(void);

signals:

  void downloadImagesSuccessfull(const QString html,
                                 const QMap<QString, QByteArray> referencesAndMemoryFiles,
                                 const QMap<QString, QString> referencesAndInternalNames);

public slots:

  void onInsertImageFromFileClicked(void);
  void onContextMenuEditImageProperties(void);

  void onDownloadImages(const QString html);
};

#endif // IMAGEFORMATTER_H
