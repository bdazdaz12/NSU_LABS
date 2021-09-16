nclude <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>
#include <signal.h>
#include <libnet.h>

//#define multicast_port 54000
#define multicast_port "54000"

addrinfo* resolveGroupAddress(char *groupAddress, bool IPv4_mode) {
    addrinfo hints {}; // структура, в которую записываются ограничения на резолв адресов для сокета
    bzero((char *) &hints, sizeof(hints)); // зануляем область памяти

    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;
    // NUMERIC - резолвим IP-адрес, а не доменное имя.
    // PASSIVE - говорит о том, что хотим разрезолвить адрес, к которому можно забиндиться.
    hints.ai_family = IPv4_mode? AF_INET : AF_INET6;

    addrinfo *result;
    if (getaddrinfo(groupAddress, multicast_port, &hints, &result)) {
        perror("getaddrinfo ERROR");
        exit(21);
    }

    return result;
}

std::shared_ptr<struct sockaddr> makeSocketAddr(char *groupAddress, bool IPv4_mode) {
    std::shared_ptr<struct sockaddr> sockAddrPtr = std::make_shared<struct sockaddr>();
    bzero((char *) sockAddrPtr.get(), sizeof(struct sockaddr)); // get - возвращает хранимый внутри указатель
//    if (IPv4_mode) {
//
//    }
    return sockAddrPtr;
}

int main(int argc, char **argv) {
    char *groupAddress = argv[1];
    bool IPv4_mode = strstr(groupAddress, ":") == nullptr;

    int updSocket = socket(IPv4_mode? AF_INET : AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    fcntl(updSocket, F_SETFL, O_NONBLOCK); // делаем неблокирующим
    const int optval = 1;
    setsockopt(updSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)); // разрешаем реюзать порт

    std::shared_ptr<addrinfo> a = std::make_shared<addrinfo>();

    addrinfo *multicastAddr = resolveGroupAddress(groupAddress, IPv4_mode);


    std::shared_ptr<sockaddr> ptr = makeSocketAddr(groupAddress, IPv4_mode);
    std::cout << ptr->sa_family << " " << (int)ptr->sa_data[0] << " " << (int)ptr->sa_data[1] << std::endl;


//    if (bind(updSocket, multicastAddr->ai_addr, sizeof(*(multicastAddr->ai_addr))) == -1) {
//        perror("BIND ERROR");
//        free(multicastAddr);
//        exit(22);
//    }

    char msg[] = "PRIVET YOBA!";

    char buf[123];
//    while (true) {
//        sendto(updSocket, msg, sizeof(msg), 0, (sockaddr*) multicastAddr->ai_addr,
//               sizeof(*(multicastAddr->ai_addr)));
//        int cnt = recv(updSocket, buf, 123, 0);
//        if (cnt > 0) {
//            std::cout << cnt << " " << buf[0] << std::endl;
//        }
//        sleep(5);
//    }

//    free(multicastAddr); // TODO : по хорошему нужно бы обработку сигнала добавить
}

