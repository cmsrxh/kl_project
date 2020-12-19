TEMPLATE = subdirs 

SUBDIRS += iface

## 完整UI项目
SUBDIRS += mediaHmi

## 播放控制接口服务，用来测试跟播放器服务进行进程间的通信
SUBDIRS += playerClient

## 播放服务器，使用mpv进行播放
SUBDIRS += player

contains (QT_ARCH, arm|arm64){
SUBDIRS += autPlayer
}

