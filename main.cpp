
#include "appcontrol.h"
#include "zlog.h"
#include <iostream>
#include <stdio.h>
#include <thread>

int main()
{   
    zlog_category_t *c;
    std::string path = CONFIGPATH;
    path = path + "zlog.conf";

    int zinit = dzlog_init(path.c_str(),"ccoRouter");
    // std::cout << "zlog path" << path << std::endl;
    // printf("zlog init %d\n", zinit);
    c = zlog_get_category("main");

    zlog_info(c, "ccoRouter begin init ");

    AppControl *app = new AppControl();

    int nInit = app->init();
    if (nInit < 0)
    {
        zlog_info(c, "ccoRouter  init error ");
        return -1;
    }

    app->start();
    return 0;
}
