/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include "webattributedialog.hpp"
#include "ui_webattributedialog.h"
//--------------------------------------------------------------------------------
WebAttributeDialog::WebAttributeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WebAttributeDialog)
{
    init();
}
//--------------------------------------------------------------------------------
WebAttributeDialog::~WebAttributeDialog()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void WebAttributeDialog::init(void)
{
    ui->setupUi(this);

    sl_attr.append({ "AutoLoadImages",                  QWebEngineSettings::AutoLoadImages });
    sl_attr.append({ "JavascriptEnabled",               QWebEngineSettings::JavascriptEnabled });
    sl_attr.append({ "JavascriptCanOpenWindows",        QWebEngineSettings::JavascriptCanOpenWindows });
    sl_attr.append({ "JavascriptCanAccessClipboard",    QWebEngineSettings::JavascriptCanAccessClipboard });
    sl_attr.append({ "LinksIncludedInFocusChain",       QWebEngineSettings::LinksIncludedInFocusChain });
    sl_attr.append({ "LocalStorageEnabled",             QWebEngineSettings::LocalStorageEnabled });
    sl_attr.append({ "LocalContentCanAccessRemoteUrls", QWebEngineSettings::LocalContentCanAccessRemoteUrls });
    sl_attr.append({ "XSSAuditingEnabled",              QWebEngineSettings::XSSAuditingEnabled });
    sl_attr.append({ "SpatialNavigationEnabled",        QWebEngineSettings::SpatialNavigationEnabled });
    sl_attr.append({ "LocalContentCanAccessFileUrls",   QWebEngineSettings::LocalContentCanAccessFileUrls });
    sl_attr.append({ "HyperlinkAuditingEnabled",        QWebEngineSettings::HyperlinkAuditingEnabled });
    sl_attr.append({ "ScrollAnimatorEnabled",           QWebEngineSettings::ScrollAnimatorEnabled });
    sl_attr.append({ "ErrorPageEnabled",                QWebEngineSettings::ErrorPageEnabled });
    sl_attr.append({ "PluginsEnabled",                  QWebEngineSettings::PluginsEnabled });
    sl_attr.append({ "FullScreenSupportEnabled",        QWebEngineSettings::FullScreenSupportEnabled });
    sl_attr.append({ "ScreenCaptureEnabled",            QWebEngineSettings::ScreenCaptureEnabled });
    sl_attr.append({ "WebGLEnabled",                    QWebEngineSettings::WebGLEnabled });
    sl_attr.append({ "Accelerated2dCanvasEnabled",      QWebEngineSettings::Accelerated2dCanvasEnabled });
    sl_attr.append({ "AutoLoadIconsForPage",            QWebEngineSettings::AutoLoadIconsForPage });
    sl_attr.append({ "TouchIconsEnabled",               QWebEngineSettings::TouchIconsEnabled });
    sl_attr.append({ "FocusOnNavigationEnabled",        QWebEngineSettings::FocusOnNavigationEnabled });
    sl_attr.append({ "PrintElementBackgrounds",         QWebEngineSettings::PrintElementBackgrounds });
    sl_attr.append({ "AllowRunningInsecureContent",     QWebEngineSettings::AllowRunningInsecureContent });
    sl_attr.append({ "AllowGeolocationOnInsecureOrigins",   QWebEngineSettings::AllowGeolocationOnInsecureOrigins });
    sl_attr.append({ "AllowWindowActivationFromJavaScript", QWebEngineSettings::AllowWindowActivationFromJavaScript });
    sl_attr.append({ "ShowScrollBars",                  QWebEngineSettings::ShowScrollBars });
    sl_attr.append({ "PlaybackRequiresUserGesture",     QWebEngineSettings::PlaybackRequiresUserGesture });
    sl_attr.append({ "WebRTCPublicInterfacesOnly",      QWebEngineSettings::WebRTCPublicInterfacesOnly });
    sl_attr.append({ "JavascriptCanPaste",              QWebEngineSettings::WebRTCPublicInterfacesOnly });
    sl_attr.append({ "DnsPrefetchEnabled",              QWebEngineSettings::DnsPrefetchEnabled });
    sl_attr.append({ "PdfViewerEnabled",                QWebEngineSettings::PdfViewerEnabled });

    int pos_y = 0;
    foreach(ATTR attr, sl_attr)
    {
        QCheckBox *cb_attr = new QCheckBox(this);
        cb_attr->setProperty(ATTRIBUTE, attr.attr_name);
        sl_checkboxes.append(cb_attr);

        QLabel *label = new QLabel(this);
        label->setText(attr.attr_name);
        ui->grid->addWidget(label,      pos_y, 0);
        ui->grid->addWidget(cb_attr,    pos_y, 1);
        pos_y++;
    }

    connect(ui->buttonBox,  &QDialogButtonBox::accepted,    this,   &WebAttributeDialog::accept);
    connect(ui->buttonBox,  &QDialogButtonBox::rejected,    this,   &WebAttributeDialog::reject);

    connect(ui->btn_default,    &QPushButton::clicked,  this,   &WebAttributeDialog::set_default);

    adjustSize();
    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
void WebAttributeDialog::set_default(void)
{
    QWebEngineSettings *ds = QWebEngineSettings::defaultSettings();
    foreach(ATTR attr, sl_attr)
    {
        set(attr.attr_name, ds->testAttribute(attr.attr_value));
    }
}
//--------------------------------------------------------------------------------
bool WebAttributeDialog::set(const QString &attribute_name, bool value)
{
    foreach (QCheckBox *cb, sl_checkboxes)
    {
        if(cb->property(ATTRIBUTE).toString() == attribute_name)
        {
            cb->setChecked(value);
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
bool WebAttributeDialog::get(const QString &attribute_name)
{
    foreach (QCheckBox *cb, sl_checkboxes)
    {
        if(cb->property(ATTRIBUTE).toString() == attribute_name)
        {
            return cb->isChecked();
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
QWebEngineSettings::WebAttribute WebAttributeDialog::get_attr(const QString &attribute_name)
{
    QWebEngineSettings::WebAttribute res;
    foreach(ATTR attr, sl_attr)
    {
        if(attr.attr_name == attribute_name)
        {
            res = attr.attr_value;
            break;
        }
    }
    return res;
}
//--------------------------------------------------------------------------------
