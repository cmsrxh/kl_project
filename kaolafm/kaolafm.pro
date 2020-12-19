TEMPLATE = subdirs 

## 测试klUI向外提供的接口，主要是使用到考拉的数据搜索功能
#SUBDIRS += klIface

## 验证考拉所有的采集数据的接口，并封装成相应的对象
#SUBDIRS += klApi/klApi.pro

## kl完整UI项目
SUBDIRS += klUI
