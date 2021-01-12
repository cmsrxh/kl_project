import QtQuick 2.0
import "../public"
import com.hongjing.home 1.0 as Home
import com.hongjing.pagesManager 1.0 as ActivityManager

Item {
    Rectangle {
        anchors.fill: parent
        color: "gray"
    }
    Column {
        spacing: 20
        anchors.verticalCenter: parent.verticalCenter

        TextButton {
            width: 70
            height: 40
            text: "playList"
            onClicked: Home.Api.qmlHomeOneBtn();
        }

        TextButton {
            width: 70
            height: 40
            text: "singerList"
            onClicked: Home.Api.qmlHomeTwoBtn();
        }
        TextButton {
            width: 70
            height: 40
            text: "Btn Three"
            onClicked: Home.Api.qmlHomeThreeBtn();
        }
        TextButton {
            width: 70
            height: 40
            text: "BACK"
            onClicked: ActivityManager.Api.back();
        }
    }
}
