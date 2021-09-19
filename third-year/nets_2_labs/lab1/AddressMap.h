#pragma once

#include <iostream>
#include <set>
#include <map>
#include <ctime>
#include <arpa/inet.h>

#define timeout 4 // timeout = 4 seconds

class AddressMap {
private:
    std::map<std::string, timespec> addressMap; //  пара(сетевой сокет, pid) + время
    bool hasUnhandledUpdate = false;

    void add(const std::string& hostInf, timespec recvTime) {
        addressMap[hostInf] = recvTime;
    }

    bool contains(const std::string& hostInf) {
        return addressMap.count(hostInf) > 0;
    }

public:
//    AddressMap() = default;
//    ~AddressMap() = default; // хз скорее всего они и так автомат генерятся

    void handleNewContact(const std::string& hostInf, timespec recvTime) {
        if (!contains(hostInf)) {
            hasUnhandledUpdate = true;
        }
        add(hostInf, recvTime);
    }

    [[nodiscard]] bool hasUpdate() const {
        return hasUnhandledUpdate;
    }

    void check() {
        timespec curTime{};
        clock_gettime(CLOCK_REALTIME, &curTime);

        bool hasChange = false;
        for (auto iterator = addressMap.begin(); iterator != addressMap.end(); ++iterator) {
            if (curTime.tv_sec - iterator->second.tv_sec > timeout) {
                hasChange = true;
                iterator = addressMap.erase(iterator);
            }
        }

        if (hasChange) {
            print();
        }
    }

    void print() {
        hasUnhandledUpdate = false;
        for (const auto &address : addressMap) {
            std::cout << address.first << '\n';
        }
        std::cout << "---------------------------------------------------------" << std::endl ;
    }
};