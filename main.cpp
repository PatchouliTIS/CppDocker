//

// main.cpp

// cpp_docker

//

// created by PatchouliTaisa at Koumakan

//

#define N 1

#include "docker.h"

#include <iostream>

#include <sstream>

int main(int argc, char** argv) {

    std::cout << ">>> start container <<<" << std::endl;

    docker::container_config config;

    // 配置容器

    // 设置容器hostname
    config.host_name = "rangerBtn";

    // 设置容器根目录 也就是镜像位置
    config.root_dir = "/home/ubuntu/testImage/ubuntu-20.04_image/";

    // 配置网络参数
    config.ip = "173.143.0.5";  // 容器 IP
    // 192.168.10   173.143.0
    // TODO: 名称更改
    config.bridge_name = "br0";     // 宿主机网桥名称
    // 宿主机网桥IP应当是 docker0 br0 的信息
    config.bridge_ip   = "173.143.0.1";  // 宿主机网桥 IP


    // 根据 config 构造容器
    docker::container container(config);

    // TODO 1. 启动容器接口
    container.start();             // 启动容器

#ifdef N
    // TODO 2. 创建CGroup子文件夹
    // 在每次容器退出时，自定义的CGroup子文件夹都会被Linux自动回收。
    const char* create_cgrp_file = "sudo sh -c 'mkdir -p /sys/fs/cgroup/cpu/cpp_docker_50'";
    system(create_cgrp_file);

    // TODO 2. 限制容器资源接口
    const char* ctrl_cpu = "sudo sh -c 'echo 50000 > /sys/fs/cgroup/cpu/cpp_docker_50/cpu.cfs_quota_us'";
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
#endif

    waitpid(container.getContainerPid(), nullptr, 0); // 等待子进程的退出

    std::cout << "stop container..." <<"\t container pid:"<<container.getContainerPid()<< std::endl;
    return 0;

}