#ifndef QJSONVIEW_HPP
#define QJSONVIEW_HPP

#include <QWidget>
#include <QVariant>
#include <QLabel>

/**
  Widget to display JSON or QVariant data.
  This widget will display any JSON-encoded string or a hierarchically nested QVariant tree in an expandable way.
  Per default, the whole data gets displayed in one single (non-wrapped) line, which can be expanded using a button
  if the JSON / QVariant data is of type JSON-array (QVariantList) or JSON-object (QVariantMap).
*/
class QJsonView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool hoverEffects READ hoverEffects WRITE setHoverEffects);
    Q_PROPERTY(bool expandable READ isExpandable);
    Q_PROPERTY(bool expanded READ isExpanded WRITE setExpanded);


public:
    /**
      Constructor for QJsonView, taking the parent widget as a single argument.
    */
    explicit QJsonView(QWidget *parent = 0);

    /**
      Static and public helper function returning the HTML code which will be used to visualize the data (by applying syntax highlighting rules).
      This function is kept public since you may want to use this to layout some other QVariant data the same way like QJsonView does.
    */
    static QString variantToHtml(QVariant data);


signals:
    /**
      Emitted whenever this widget or one of its children has been expanded or collapsed.
      (The signal gets propagated to the root QJsonView object.)
    */
    void resized();


public slots:
    /**
      Set the value to be displayed to a QVariant value. The only supported QVariant-types are Invalid, Bool, Int, LongLong, List, Map. Any other types are untested!
    */
    void setValue(QVariant value);

    /**
      Set the value to be displayed to a JSON serialized string, which will be decoded before being viewed.
    */
    void setJsonValue(QString json);

    /**
      Enables or disables hover effects.
    */
    void setHoverEffects(bool enabled = true);

    /**
      Returns true if hover effects are enabled.
    */
    bool hoverEffects();

    /**
      Returns true if this QJsonView is expandable.
      This is the case for JSON-objects and JSON-arrays having at least one entry.
    */
    bool isExpandable();

    /**
      Returns true if this QJsonView is currently expanded.
    */
    bool isExpanded();

    /**
      Expands or collapses this view (convenient slot for expand() or collapse(), depending on the argument).
    */
    void setExpanded(bool expanded);

    /**
      Expands this view if it is expandable and not expanded.
    */
    void expand();

    /**
      Collapses this view if it is expanded.
    */
    void collapse();


protected:
    /**
      \reimp
    */
    void mousePressEvent(QMouseEvent *);
    /**
      \reimp
    */
    void paintEvent(QPaintEvent *);
    /**
      \reimp
    */
    void contextMenuEvent(QContextMenuEvent *);
    /**
      \reimp
    */
    void enterEvent(QEvent *);
    /**
      \reimp
    */
    void leaveEvent(QEvent *);

    /**
      Called by a child in order to inform this widget that the mouse cursor is now over the child instead of this widget.
    */
    void childEntered();

    /**
      Called by a child in order to inform this widget that the mouse cursor isn't over the child anymore.
    */
    void childLeaved();




private:
    // value to be displayed, as a QVariant
    QVariant v;
    // if this is no container type, this points to the QLabel representing the single value
    QLabel *lblSingle;
    // if this is a container type, these point to child widgets
    QList<QWidget*> childWidgets;
    // true if this is a container type and is currently in expanded view
    bool expanded;
    // true if hover effects are enabled
    bool hoverEffectsEnabled;

    // apply hover effect
    void hover();
    // revert hover effect
    void unhover();
};

#endif // QJSONVIEW_HPP
