#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <memory>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctime>
#include "AddressMap.h"
#include "AddressInterpreter.h"

#define multicast_port 54000
#define check_delay 5 // seconds

void roflBind(int socket);

std::shared_ptr<sockaddr> createMulticastEndpoint(char *multicastAddress, bool IPv4_mode) {
    if (IPv4_mode) {
        auto *ipv4Endpoint = (sockaddr_in *) calloc(1, sizeof(sockaddr_in));
        ipv4Endpoint->sin_family = AF_INET;
        ipv4Endpoint->sin_port = htons(multicast_port); // htons - для порта. htonl - для адреса ipv4

        if (inet_pton(AF_INET, multicastAddress, &ipv4Endpoint->sin_addr) != 1) {
            perror("CREATE ENDPOINT: INET_PTON IPv4");
        }
        return {(sockaddr *) ipv4Endpoint, free};
    } else {
        auto *ipv6Endpoint = (sockaddr_in6 *) calloc(1, sizeof(sockaddr_in6));
        ipv6Endpoint->sin6_family = AF_INET6;
        ipv6Endpoint->sin6_port = htons(multicast_port);

        if (inet_pton(AF_INET6, multicastAddress, &ipv6Endpoint->sin6_addr) != 1) {
            perror("CREATE ENDPOINT: INET_PTON IPv6");
        }
        return {(sockaddr *) ipv6Endpoint, free};
    }
}

void bindMulticastUdpSocket(int multicastUdpSocket, const std::shared_ptr<sockaddr>& multicastEndpoint,
                            bool IPv4_mode) {
    if (bind(multicastUdpSocket, multicastEndpoint.get(),
             IPv4_mode? sizeof(sockaddr_in) : sizeof(sockaddr_in6)) == -1) {
        perror("BIND ERROR \n");
        close(multicastUdpSocket);
        exit(22);
    }
}

void joinMulticastGroup(int multicastUdpSocket, char *multicastAddress, bool IPv4_mode) {
    if (IPv4_mode) {
        ip_mreq IPv4Group = {};
        IPv4Group.imr_interface.s_addr = htonl(INADDR_ANY);
        inet_pton(AF_INET, multicastAddress, &(IPv4Group.imr_multiaddr));
        if (setsockopt(multicastUdpSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &IPv4Group, sizeof(IPv4Group)) == -1) {
            perror("JOIN IPv4 ERROR \n");
            close(multicastUdpSocket);
            exit(24);
        }
    } else {
        ipv6_mreq IPv6Group = {};
        IPv6Group.ipv6mr_interface = 0;
        inet_pton(AF_INET6, multicastAddress, &(IPv6Group.ipv6mr_multiaddr));
        if (setsockopt(multicastUdpSocket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP,
                       &IPv6Group, sizeof(IPv6Group)) == -1) {
            perror("JOIN IPv6 ERROR \n");
            close(multicastUdpSocket);
            exit(25);
        }
    }
}

int main(int argc, char **argv) {
    char *multicastAddress = argv[1];
    bool IPv4_mode = strstr(multicastAddress, ":") == nullptr;

    if (IPv4_mode) {
        std::cout << "IPv4_MODE" << std::endl;
    } else {
        std::cout << "IPv6_MODE" << std::endl;
    }

    int multicastUdpSocket = socket(IPv4_mode ? AF_INET : AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    fcntl(multicastUdpSocket, F_SETFL, O_NONBLOCK); // делаем неблокирующим

    const int optval = 1;
    if (setsockopt(multicastUdpSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        perror("BLOCKLESS SOCKET ERROR");
        close(multicastUdpSocket);
        exit(19);
    } // разрешаем реюзать порт

    std::shared_ptr<sockaddr> multicastEndpoint = createMulticastEndpoint(multicastAddress, IPv4_mode);

//    bindMulticastUdpSocket(multicastUdpSocket, multicastEndpoint, IPv4_mode);
    roflBind(multicastUdpSocket);
    joinMulticastGroup(multicastUdpSocket, multicastAddress, IPv4_mode);

    pid_t pid = getpid();
    pid_t senderPid;
    AddressMap addressMap;
    AddressInterpreter addressInterpreter(IPv4_mode);

    timespec curTime{};
    timespec lastCheckTime{};

    while (true) {
        sendto(multicastUdpSocket, &pid, sizeof(pid_t), 0, multicastEndpoint.get(),
               IPv4_mode? sizeof(sockaddr_in) : sizeof(sockaddr_in6));

        auto *senderAddress = (sockaddr *) calloc(1, IPv4_mode? sizeof(sockaddr_in) : sizeof(sockaddr_in6));
        socklen_t len = IPv4_mode? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
        // пакет с IP 0.0.0.0 - появлялся из-за того, что я не инициализировал len

        int cnt = recvfrom(multicastUdpSocket, &senderPid, sizeof(senderPid), 0, senderAddress, &len);

        clock_gettime(CLOCK_REALTIME, &curTime);
        if (cnt > 0) {
            addressMap.handleNewContact(addressInterpreter.convertAddress(*senderAddress, senderPid), curTime);
        }

        if (addressMap.hasUpdate() || curTime.tv_sec - lastCheckTime.tv_sec > check_delay) {
            addressMap.check();
            if (addressMap.hasUpdate()) {
                addressMap.print();
            }
            lastCheckTime = curTime;
        }

        free(senderAddress);
        sleep(1);
    }
}

void roflBind(int socket) {
//    struct sockaddr_in6 address = {AF_INET6, htons(multicast_port)};
    struct sockaddr_in address = {AF_INET, htons(multicast_port)};
    bind(socket, (struct sockaddr*)&address, sizeof address);
}
