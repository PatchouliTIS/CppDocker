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

// C 标准库
#include <cstring>

// C++ 标准库
#include <string>       // std::string

#define STACK_SIZE (512 * 512) // 定义子进程空间大小

namespace docker {
    // ..docker 魔法开始的地方
    typedef int proc_stat;
    proc_stat proc_err = -1;
    proc_stat proc_exit = 0;
    proc_stat proc_wait = 1;

    typedef struct container_config {
        std::string host_name;     // 主机名
        std::string root_dir;      // 容器根目录
    } container_config;


    class container {

    private:

        // 可读性增强

        typedef int process_pid;

        // 子进程栈

        char child_stack[STACK_SIZE];

        // 容器配置

        container_config config;

        // 1. 使用/bin/bash将容器保持运行状态，不至于因为没有进行任何操作而直接返回。
        static void start_bash() {

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

    public:
        container(container_config &config) {

            this->config = config;

        }

        void start() {
            auto setup = [](void *args) -> int {
                auto _this = reinterpret_cast<container *>(args);

                // 对容器进行相关配置

                std::cout << "<<< ENTERING START()" << std::endl;

                _this->set_hostname();

                std::cout << "<<< hostname set" << std::endl;

                _this->set_rootdir();

                // std::cout << "<<< rootdir set" << std::endl;

                 _this->set_procsys();

                std::cout << "<<< start bash" << std::endl;

                _this->start_bash();

                // ...

                return proc_wait;
            };

            process_pid child_pid = clone(setup, child_stack+STACK_SIZE, // 移动到栈底

                                          CLONE_NEWUTS |   // UTS NameSpace 需要root权限

                                          CLONE_NEWNS |         // Mount NameSpace 需要root权限

                                           CLONE_NEWPID |        // PID NameSpace 需要root权限

                                          SIGCHLD,              // 子进程退出时会发出信号给父进程

                                          this);

            waitpid(child_pid, nullptr, 0); // 等待子进程的退出

        }

    };
}


