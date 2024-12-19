#ifndef HORSE_H
#define HORSE_H

#include "AVLTree.h"

struct Herd;

struct Horse {
    int id;
    int speed;
    Horse* leader;
    Herd* herd;
    int followTime;
    int joinHerdTime;
    int pathId;

    Horse(int id, int speed) : id(id), speed(speed), leader(nullptr), herd(nullptr), followTime(-1),
                               joinHerdTime(-1), pathId(0) {}

};

struct Herd {
    int id;
    AVLTree<Horse*> horses;

    explicit Herd(int id) : id(id), horses() {}

    bool isEmpty() const { return horses.isEmpty(); }
};


#endif
