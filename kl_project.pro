TEMPLATE = subdirs 

## 网络下载相关库文件
SUBDIRS += netLoad/netLoadLib.pro

## 播放服务器，使用mpv进行播放
#SUBDIRS += player/mediaPlayer.pro

## kaolaFM
SUBDIRS += kaolafm/kaolafm.pro

# QQ音乐
#SUBDIRS += qqmusic

## 测试树形列表
#SUBDIRS += tools/treeView
## 测试网络接口
#SUBDIRS += netLoad/netLoadTest.pro
## qml测试例子，只有main.cpp和qml
#SUBDIRS += tools/netNotify
## 测试代码，测试qml，图片控件
#SUBDIRS += tools/netImage
SUBDIRS += tools/testLoadItem/testLoadItem.pro
