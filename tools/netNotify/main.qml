import QtQuick 2.6
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log(qsTr('Clicked on background. Text: "' + textEdit.text + '"'))
        }
    }

    TextEdit {
        id: textEdit
        text: qsTr("Enter some text...")
        verticalAlignment: Text.AlignVCenter
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        Rectangle {
            anchors.fill: parent
            anchors.margins: -10
            color: "transparent"
            border.width: 1
        }
    }

    Text {
        id: name
        anchors.centerIn: parent
        text: qsTr("<html>
<head>
<meta http-equiv=\"Content-Type\" content=\"text/html;charset=ISO-8859-1\"/>
<title>Error 503 </title>
</head>
<body>
<h2>HTTP ERROR: 503</h2>
<p>Problem accessing /v2/app/active. Reason:
<pre>    Service Unavailable</pre></p>
<hr /><i><small>Powered by Jetty://</small></i>
</body>
</html>")
    }
}
