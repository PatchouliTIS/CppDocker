//

// main.cpp

// cpp_docker

//

// created by changkun at [shiyanlou.com](http://shiyanlou.com)

// 

#include "docker.h"

#include <iostream>

int main(int argc, char** argv) {

    std::cout << "...start container" << std::endl;

    docker::container_config config;

    // 配置容器

    // 设置容器hostname
    config.host_name = "koumakan";

    // 设置容器根目录
    // TODO: 能否根据配置文件手动配置
    config.root_dir = "./image";

    // ...

    docker::container container(config);// 根据 config 构造容器

    container.start();                  // 启动容器

    std::cout << "stop container..." << std::endl;

    return 0;

}