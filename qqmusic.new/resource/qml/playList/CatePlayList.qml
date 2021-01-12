import QtQuick 2.0
import com.hongjing.catePlaylist 1.0 as CatePL

Item {
   Component.onCompleted: {
       CatePL.Api.qmlObtainCatePlayList()
   }
   Rectangle {
       anchors.right: parent.right
       anchors.bottom: parent.bottom

       width: parent.width - 60
       height: parent.height - 72

       CategoryList {
           id: cate
           width: 200
           anchors.top: parent.top
           anchors.topMargin: 80
           anchors.bottom: parent.bottom
       }

       PlayList {
           anchors.left: cate.right
           anchors.right: parent.right
           anchors.top: parent.top
           anchors.bottom: parent.bottom
       }
   }
}
