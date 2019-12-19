[1] http开发指南
	git clone https://github.com/kaolafm/api.git

[2] shell 操作参数
gethttp://open.kaolafm.com/v1/app/activecn5290d673b807770b49b4077cb3bfa0d6aedd

curl -k -L -d "appid=cn5290&sign=a6309ffd3ba9e291bdbd0a64a1b62a35" "http://open.kaolafm.com/v1/app/active"

[3] 相关参数信息
    const char *appId         = "cn5290";
    const char *secretKey     = "d673b807770b49b4077cb3bfa0d6aedd";
    const char *channel       = "linuxwhhj";
    const char *childChannel  = "0";
    const char *packageName   = "com.edog.car.linuxwhhj";

    //const char *dataSavePath  = NULL;                     // 默认为当前工作目录
    //const char *dataSavePath  = "/data/kaolafm";        // 一个没有权限的目录
    const char *dataSavePath  = "/var/tmp/kaolafm";     // 一个拥有权限的目录

