import QtQuick 2.0

Item {
   Component.onCompleted: {
       catePlayListProc.qmlObtainCatePlayList()
   }

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
