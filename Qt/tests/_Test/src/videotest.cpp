#include "videotest.h"

#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>
#include <vlc-qt/WidgetVideo.h>

struct VideoTest::Private
{
    VlcInstance * vlcInstance;
    VlcMediaPlayer * vlcMediaPlayer;
    VlcMedia * vlcMedia;
    VlcWidgetVideo * vlcVideoWidget;
};

VideoTest::VideoTest(QWidget * p_parent)
{
    d = new Private();
    ui.setupUi(this);

    d->vlcMedia = 0;
    d->vlcInstance = new VlcInstance(VlcCommon::args(), this);
    d->vlcMediaPlayer = new VlcMediaPlayer(d->vlcInstance);
    d->vlcVideoWidget = new VlcWidgetVideo(this);

    d->vlcMediaPlayer->setVideoWidget(d->vlcVideoWidget);
    d->vlcVideoWidget->setMediaPlayer(d->vlcMediaPlayer);

    ui.videoLayout->addWidget(d->vlcVideoWidget);

    connect(ui.playButton, SIGNAL(clicked()), d->vlcMediaPlayer, SLOT(play()));
    connect(ui.pauseButton, SIGNAL(clicked()), d->vlcMediaPlayer, SLOT(pause()));
    connect(ui.videoUrlEdit, SIGNAL(editingFinished()), this, SLOT(sourceChanged()));
}

VideoTest::~VideoTest()
{
    delete d->vlcInstance;
    delete d->vlcMediaPlayer;
    delete d->vlcMedia;

    delete d;
}

VideoTest::sourceChanged()
{
    QUrl url(ui.videoUrlEdit->test());
    if(url.isValid() == false)
    {
        return;
    }

    d->vlcMediaPlayer->stop();

    delete d->vlcMedia;
    d->vlcMedia = new VlcMedia(url.toString(), d->vlcInstance);
    d->vlcMediaPlayer->open(d->vlcMedia);
}
