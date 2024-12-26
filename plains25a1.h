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

    /**
     * Move a herd from the empty herds tree to the non-empty herds tree.
     * @param herdId ID of the herd to move.
     * @param nodeToRemove node to remove from the empty herds tree.
     * @return new node in the non-empty herds tree.
     */
    Node<Herd>* moveHerdFromEmpty(int herdId, Node<int> *nodeToRemove);

    /**
     * Move a herd to the empty herds tree.
     * @param herdId ID of the herd to move.
     */
    void moveHerdToEmpty(int herdId);

    /**
     * Get the leader of a horse.
     * @param horse horse whose leader is to be found.
     * @return leader of the horse.
     */
    static Horse *getLeader(Horse *horse);

    /**
     * Reset the path IDs of all horses in a herd.
     * @param herd herd whose horses' path IDs are to be reset.
     */
    static void resetPaths(Herd *herd);

    /**
    * Check if a horse leads another horse through chain and mark the path.
    * @param horse horse to start from.
    * @param candidate candidate horse to be checked.
    * @return True if the candidate can be reached through a lead chain, false otherwise.
    */
    static bool trackFollowingConnection(Horse* horse, Horse* candidate);


    /**
    * @brief Reset a horse's attributes to the default values.
    * @param horse horse to reset.
    */
    static void resetHorse(Horse *horse);

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
