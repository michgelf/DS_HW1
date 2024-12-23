//
// 234218 Data Structures 1.
// Semester: 2025A (Winter).
// Wet Exercise #1.
//
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
//

#ifndef PLAINS25WINTER_WET1_H_
#define PLAINS25WINTER_WET1_H_

#include <memory>

using namespace std;


#include "wet1util.h"
#include "FarmEntities.h"


class Plains {
private:
    //
    // Here you may add anything you want
    //
    int clock;
    AVLTree<Horse> horses;
    AVLTree<Herd> herds;
    AVLTree<int> emptyHerds;

    Node<Herd>* moveHerdFromEmpty(int herdId, Node<int>* nodeToRemove);

    void moveHerdToEmpty(int herdId);

    static Horse* getLeader(Horse* horse);

    static void resetPaths(Herd* herd);

    static bool checkAndPlant(Horse* horse, Horse* candidate);

    static void resetHorse(Horse* horse);

public:

    // <DO-NOT-MODIFY> {
    Plains();

    ~Plains();

    StatusType add_herd(int herdId);

    StatusType remove_herd(int herdId);

    StatusType add_horse(int horseId, int speed);

    StatusType join_herd(int horseId, int herdId);

    StatusType follow(int horseId, int horseToFollowId);

    StatusType leave_herd(int horseId);

    output_t<int> get_speed(int horseId);

    output_t<bool> leads(int horseId, int otherHorseId);

    output_t<bool> can_run_together(int herdId);
    // } </DO-NOT-MODIFY>
};

#endif // PLAINS25WINTER_WET1_H_
