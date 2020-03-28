TEMPLATE = subdirs 

## 验证考拉所有的采集数据的接口，并封装成相应的对象
#SUBDIRS += klApi/klApi.pro

## qml测试例子，只有main.cpp和qml
#SUBDIRS += netNotify

## kl完整UI项目
#SUBDIRS += klUI

## 播放控制接口服务，用来测试跟播放器服务进行进程间的通信
#SUBDIRS += playerClient

## 播放服务器，使用mpv进行播放
#SUBDIRS += player

## 测试代码，测试qml，图片控件
SUBDIRS += netImage

## 测试klUI向外提供的接口，主要是使用到考拉的数据搜索功能
#SUBDIRS += klIface

# QQ音乐
SUBDIRS += qqmusic

# 测试网络接口
#SUBDIRS += netLoad
