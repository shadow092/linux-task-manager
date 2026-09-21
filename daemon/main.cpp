#include "SystemMonitor.h"
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>


int main() {
    SystemMonitor monitor;
    int serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    unlink("/tmp/sysmon.sock");
    int active_clients = 0;
    monitor.readSystemProcesses();
    sockaddr_un server_addr{};
    server_addr.sun_family = AF_UNIX;
    std::strncpy(server_addr.sun_path, "/tmp/sysmon.sock", sizeof(server_addr.sun_path) - 1);

    if( bind(serverSocket, (sockaddr*)&server_addr, sizeof(server_addr)) == -1 ){
        perror("bind");
        close(serverSocket);
        return 1;
    }

    if(listen(serverSocket,1) == -1){
        perror("listen");
        close(serverSocket);
        return 1;
    }

    int flags = fcntl(serverSocket,F_GETFL);
    fcntl(serverSocket, F_SETFL, flags | O_NONBLOCK);

    int epfd = epoll_create1(0);

    epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = serverSocket;

    epoll_ctl(epfd,EPOLL_CTL_ADD,serverSocket,&event);

    epoll_event events[1];

    while(true){
        // int timeout = (active_clients > 0) ? 1000 : -1;
        int n=epoll_wait(epfd,events,1,-1);

        // if(active_clients > 0){
        // monitor.readSystemStats();
        // std::cout << "[sysmond] CPU: " << monitor.cpuUsage() << "% | RAM: " << monitor.ramUsage() << "%" << std::endl;
        // }

        for(int i=0;i<n;i++){

            int fd=events[i].data.fd;
            if(fd == serverSocket){

            int client_fd = accept(serverSocket,nullptr,nullptr);
                if (client_fd < 0) {
                    perror("accept");
                    continue;
                }
                active_clients++;

                int flags = fcntl(client_fd,F_GETFL);
                fcntl(client_fd,F_SETFL,flags | O_NONBLOCK);
                epoll_event client_event{};
                client_event.events = EPOLLIN;
                client_event.data.fd = client_fd;

                epoll_ctl(
                    epfd,
                    EPOLL_CTL_ADD,
                    client_fd,
                    &client_event
                    );

                std::cout << "Client connected"<<std::endl;
            }
            else {
                char buffer[1024];

                int bytes = read(fd, buffer, sizeof(buffer) - 1);

                if (bytes > 0) {
                    buffer[bytes] = '\0';

                    std::string page(buffer);

                    if (!page.empty() && page.back() == '\n')
                        page.pop_back();

                    if (page == "p") {
                        // std::cout << "Page: p" << std::endl;

                        monitor.readSystemProcesses();

                        std::string msg;
                        msg.reserve(monitor.getProcessList().size() * 50);

                        for (const auto& process : monitor.getProcessList()) {
                            msg += std::to_string(process.pid);
                            msg += "|";
                            msg += process.name;
                            msg += "|";
                            msg += std::to_string(process.ramKb);
                            msg += "\n";
                        }

                        write(fd, msg.c_str(), msg.size());
                    }
                    else if (page == "s") {
                        // std::cout << "Page: s" << std::endl;

                        monitor.readSystemStats();

                        std::string msg =
                            std::to_string(monitor.cpuUsage()) + " " +
                            std::to_string(monitor.ramUsage()) + "\n";

                        write(fd, msg.c_str(), msg.length());
                    }
                    else if (!page.empty() && page.back() == 'k') {
                        int pid = std::stoi(page.substr(0, page.size() - 1));

                        // std::cout << "Killing PID: " << pid << std::endl;

                        if (monitor.killProcess(pid)) {
                            // std::cout << "Process killed"<<std::endl;
                        } else {
                            // std::cout << "Failed to kill process"<<std::endl;
                        }
                    }
                }
                else if (bytes == 0) {
                    close(fd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);

                    std::cout << "Client disconnected"<<std::endl;
                    active_clients = std::max(0, active_clients - 1);
                }
                else {
                    if (errno != EAGAIN && errno != EWOULDBLOCK) {
                        close(fd);
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
                    }
                }
            }

        }
    }

}