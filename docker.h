//
// Created by 62389 on 2023/2/25.
//

#ifndef CPPDOCKER_DOCKER_H
#define CPPDOCKER_DOCKER_H

#endif //CPPDOCKER_DOCKER_H

// 本节实验要用到的系统调用所在的头文件
#include <sys/wait.h>   // waitpid
#include <sys/mount.h>  // mount
#include <fcntl.h>      // open
#include <unistd.h>     // execv, sethostname, chroot,
#include <iostream>
// fchdir
#include <sched.h>      // clone

#include<stdlib.h>


// 网络标准库
#include <net/if.h>     // if_nametoindex
#include <arpa/inet.h>  // inet_pton
#include "network.h"

// C 标准库
#include <cstring>

// C++ 标准库
#include <string>       // std::string
#include <sstream>

#define STACK_SIZE (512 * 512) // 定义子进程空间大小
#define N 1

namespace docker {
    // 自有容器命名空间
    typedef int proc_stat;
    proc_stat proc_err = -1;
    proc_stat proc_exit = 0;
    proc_stat proc_wait = 1;

    typedef struct container_config {
        std::string host_name;      // 主机名
        std::string root_dir;       // 容器根目录
        std::string ip;             // 容器 IP
        std::string bridge_name;    // 网桥名
        std::string bridge_ip;      // 网桥 IP
    } container_config;


    class container {

    private:

        // 可读性增强
        typedef int process_pid;

        // 保存容器网络设备, 用于删除
        char* veth1;
        char* veth2;

        // 子进程栈
        char child_stack[STACK_SIZE];

        // 当前子进程PID
        process_pid curContainerPid;

        // 容器配置
        container_config config;

        // 1. 使用/bin/bash将容器保持运行状态，不至于因为没有进行任何操作而直接返回。
        void start_bash() {

            // 将 C++ std::string 安全的转换为 C 风格的字符串 char *

            // 从 C++14 开始, C++编译器将禁止这种写法 `char *str = "test";`

            std::string bash = "/bin/bash";

            char *c_bash = new char[bash.length()+1];   // +1 用于存放 '\0'

            strcpy(c_bash, bash.c_str());

            char* const child_args[] = { c_bash, nullptr };

            std::cout << "<<< execv" << std::endl;

            execv(child_args[0], child_args);           // 在子进程中执行 /bin/bash

            std::cout << "<<< execv done" << std::endl;

            delete []c_bash;
        }

        // 2. 设置容器hostname
        void set_hostname() const {
            sethostname(this->config.host_name.c_str(),
                        this->config.host_name.length());
        }

        // 3. 设置容器的根目录，屏蔽子进程对外部进程和资源的访问
        void set_rootdir() const {
            // Lunix chdir() 系统调用，切换到当前进程目录下
            chdir(this->config.root_dir.c_str());

            // Lunix chroot() 系统调用，将当前的文件夹设置为根目录地址
            // （根目录路径保存在container_config结构体中）
            // 只是单纯的 const char* 形式，也就是将根目录地址作为字符串形式保存。
            chroot(".");
        }

        // 4. 设置独立的PID空间，使得在检查进程时不会查找到父进程
        static void set_procsys() {
            // 挂载 proc 文件系统
            mount("none", "/proc", "proc", 0, nullptr);
            mount("none", "/sys", "sysfs", 0, nullptr);
        }

        // 5. 设置网络
        void set_network() {
            /*
            int ifindex = if_nametoindex("eth0");
            struct in_addr ipv4;
            struct in_addr bcast;
            struct in_addr gateway;

            int prefix = 16;
            if (inet_pton(AF_INET, this->config.ip.c_str(), &ipv4) <= 0) {
                perror("!!!inet_pton ip error!!!");
            }

            if (inet_pton(AF_INET, "255.255.255.0", &bcast) <= 0) {
                perror("!!!inet_pton bcast error!!!");
            }

            if (inet_pton(AF_INET, this->config.bridge_ip.c_str(), &gateway) <= 0) {
                perror("!!!inet_pton bridge_ip error!!!");
            }
            */


            // 启用 lo 网络设备
            int no = lxc_netdev_up("lo");
            std::cout << "<<< set_network lxc_netdev_up LO: " << no << "  >>>" << std::endl;

            // 启用 eth0 网络设备
            no = lxc_netdev_up("eth0");
            std::cout << "<<< set_network lxc_netdev_up ETH0:" << no << "  >>>" << std::endl;

            // TODO:设置 eth0 IP 地址  如何根据掩码动态设置其广播地址？
            // no = lxc_ipv4_addr_add(ifindex, &ipv4, &bcast, prefix);
            // std::cout << "<<< set_network lxc_ipv4_addr_add:" << no << "  >>>" << std::endl;
            std::string baddr = this->config.ip;
            int idx = (int)baddr.length() - 1;
            while(idx >= 0 && baddr[idx] != '.') {
                idx--;
            }
            baddr = baddr.substr(0, idx);
            std::string ipv4_addr_add = "ip address add " + this->config.ip + "/24 broadcast " + baddr + ".255 dev eth0";
            const char* ipv4_addr_add_cmd = ipv4_addr_add.c_str();
            system(ipv4_addr_add_cmd);



            // 设置网关
            // no = lxc_ipv4_gateway_add(ifindex, &gateway);
            // std::cout << "<<< set_network lxc_ipv4_gateway_add:" << no << "  >>>" << std::endl;
            std::string gateway_set = "ip route add default via " + this->config.bridge_ip;
            const char* gateway_set_cmd = gateway_set.c_str();
            system(gateway_set_cmd);

            // 设置 MAC 地址
            char mac[18];
            new_hwaddr(mac);
            no = setup_hw_addr(mac, "eth0");
            std::cout << "<<< set_network setup_hw_addr: " << no << "  >>>" << std::endl;
        }

    public:
        container(container_config &config) {
            this->config = config;
        }

        void setContainerPid(const int& pid) {
            this->curContainerPid = pid;
        }

        process_pid getContainerPid() {
            return this->curContainerPid;
        }

        void start() {
#ifdef N
            char veth1buf[IFNAMSIZ] = "rangerBtn0X";
            char veth2buf[IFNAMSIZ] = "rangerBtn0X";

            // 创建一对网络设备, 一个用于加载到宿主机, 另一个用于转移到子进程容器中
            // lxc_mkifname 这个 API 在网络设备名字后面至少需要添加一个 "X" 来
            // 支持随机创建虚拟网络设备，用于保证网络设备的正确创建。
            veth1 = lxc_mkifname(veth1buf);
            std::cout << "<<< veth1: " << veth1 << " >>>" << std::endl;
            veth2 = lxc_mkifname(veth2buf);
            std::cout << "<<< veth2: " << veth2 << " >>>" << std::endl;

            // 在主进程中连接这个veth-pair
            int no = lxc_veth_create(veth1, veth2);
            std::cout << "<<< lxc_veth_create: " << no << ">>>" << std::endl;

            // 设置 veth1 的 MAC 地址
            no = setup_private_host_hw_addr(veth1);
            std::cout << "<<< setup_private_host_hw_addr: " << no << ">>>" << std::endl;

            // 将 veth1 添加到网桥中
            no = lxc_bridge_attach(config.bridge_name.c_str(), veth1);
            std::cout << "<<< lxc_bridge_attach: " << no << ">>>" << std::endl;

            // 激活 veth1
            no = lxc_netdev_up(veth1);
            std::cout << "<<< lxc_netdev_up: " << no << ">>>" << std::endl;
#endif
            auto setup = [](void *args) -> int {
                auto _this = reinterpret_cast<container *>(args);
                // 对容器进行相关配置

                std::cout << "<<< ENTERING START()" << std::endl;

                _this->set_hostname();

                // std::cout << "<<< hostname set" << std::endl;

                _this->set_rootdir();

                // std::cout << "<<< rootDir set" << std::endl;

                _this->set_procsys();

                // std::cout << "<<< network set >>>" << std::endl;

                _this->set_network();

                // std::cout << "<<< start bash" << std::endl;

                _this->start_bash();

                // ...

                return proc_wait;
            };

            process_pid child_pid = clone(setup, child_stack+STACK_SIZE, // 移动到栈底
                                           CLONE_NEWUTS |   // UTS NameSpace 需要root权限
                                           CLONE_NEWNS |         // Mount NameSpace 需要root权限
                                           CLONE_NEWPID |        // PID NameSpace 需要root权限
#ifdef N
                                           CLONE_NEWNET |        // Net NameSpace 需要root权限
#endif
                                          SIGCHLD,              // 子进程退出时会发出信号给父进程
                                          this);
            // 将生成的容器子进程号保存至当前docker类的实例对象中去。
            this->setContainerPid(child_pid);

#ifdef N
            // 将 veth2 转移到容器内部, 并命名为 eth0
            std::cout<<"<<< docker veth2 move to subProcess >>>"<<std::endl;
            lxc_netdev_move_by_name(veth2, child_pid, "eth0");
#endif

            std::cout<<"<<< docker start >>>"<<"\t container pid:"<<this->getContainerPid() << "  " <<std::endl;
            // waitpid(child_pid, nullptr, 0); // 等待子进程的退出
        }

#ifdef N
        // 析构函数，在容器退出时将建立的虚拟网络设备删除。
        ~container() {
            lxc_netdev_delete_by_name(veth1);
            lxc_netdev_delete_by_name(veth2);
        }
#endif

    };
}

