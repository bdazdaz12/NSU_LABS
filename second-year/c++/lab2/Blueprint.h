//
// Created by yevsyukof on 23.12.2020.
//

#ifndef LAB2_BLUEPRINT_H
#define LAB2_BLUEPRINT_H

#include <list>
#include <map>

class Blueprint {
protected:
    struct blueprint{
        list<int> queue;
        map<int, shared_ptr<IWorker>> blocks;
    };
};


#endif //LAB2_BLUEPRINT_H
