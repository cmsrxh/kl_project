import QtQuick 2.0

// 显示在主页下,状态栏下面的窗口
Item {
    id: root
    property int argvalue: -1
    signal nextPage()
    signal prevPage()
    signal upToTop();

    property Component handle : null

    Loader {
        width: root.width
        height: root.height - 72

        anchors.bottom: root.bottom

        sourceComponent: root.handle
    }
}
