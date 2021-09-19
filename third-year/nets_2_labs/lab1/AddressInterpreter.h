#pragma once

#include <sstream>

class AddressInterpreter {
private:
    bool IPv4_mode;
public:
    explicit AddressInterpreter(bool IPv4_mode) {
        this->IPv4_mode = IPv4_mode;
    }

    [[nodiscard]] std::string convertAddress(const sockaddr &senderAddr, pid_t senderPid) const {
        char senderIP[IPv4_mode? INET_ADDRSTRLEN : INET6_ADDRSTRLEN]; // константы длины "человеческой" записи адресов

        int senderPort = ntohs(IPv4_mode? ((sockaddr_in *) &senderAddr)->sin_port
                                         : ((sockaddr_in6 *) &senderAddr)->sin6_port);

        std::stringstream ss;
        ss << "IP: " << inet_ntop(IPv4_mode? AF_INET : AF_INET6,
                                  IPv4_mode? (void *) &((sockaddr_in *) &senderAddr)->sin_addr
                                  : (void *) &((sockaddr_in6 *) &senderAddr)->sin6_addr,
                                  senderIP,IPv4_mode? INET_ADDRSTRLEN : INET6_ADDRSTRLEN)
                                  << ' ' << "Port: " << senderPort << ' ' << "Pid: " << senderPid; // TODO
        return ss.str();
    }
};