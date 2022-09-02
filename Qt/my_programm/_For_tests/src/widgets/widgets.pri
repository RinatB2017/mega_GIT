
include (check_in_widget/check_in_widget.pri)
include (controls_widget/controls_widget.pri)
include (default_widget/default_widget.pri)
include (table_widget/table_widget.pri)
include (file_widget/file_widget.pri)
include (mdi_widget/mdi_widget.pri)
include (om_widget/om_widget.pri)
include (test_class/test_class.pri)

lessThan(QT_MAJOR_VERSION, 6) {
    include (render/render.pri)
}
