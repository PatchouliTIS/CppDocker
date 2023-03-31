//

// main.cpp

// cpp_docker

//

// created by PatchouliTaisa at Koumakan

// 

#include "docker.h"

#include <iostream>

#include <sstream>

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

    // TODO 1. 启动容器接口
    container.start();             // 启动容器


    // TODO 2. 限制容器资源接口
    const char* ctrl_cpu = "sudo sh -c 'echo 10000 > /sys/fs/cgroup/cpu/cpp_docker_50/cpu.cfs_quota_us'";

    system(ctrl_cpu);

    // TODO 3. 将进程加入cgroup所属的tasks中
    std::stringstream ss;
    ss << container.getContainerPid();
    std::string process_id = "";
    ss >> process_id;
    process_id = "sudo sh -c 'echo " + process_id + " > /sys/fs/cgroup/cpu/cpp_docker_50/tasks'";
    char* pid_cmd = new char[process_id.length() + 1];
    strcpy(pid_cmd, process_id.c_str());

    system(pid_cmd);

    waitpid(container.getContainerPid(), nullptr, 0); // 等待子进程的退出

    std::cout << "stop container..." <<"\t container pid:"<<container.getContainerPid()<< std::endl;


    return 0;

}