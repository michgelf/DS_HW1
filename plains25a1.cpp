// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"
#include <exception>


Plains::Plains() : clock(0), horses(), herds(), emptyHerds() {}

Plains::~Plains() {

}

StatusType Plains::add_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (herds.find(herdId)) {
        return StatusType::FAILURE;
    }
    try {
        emptyHerds.insert(herdId, Herd(herdId));
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::remove_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    auto herdToRemove = emptyHerds.find(herdId);
    if (herdToRemove == nullptr) {
        return StatusType::FAILURE;
    }
    try {
        emptyHerds.remove(herdToRemove);
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::add_horse(int horseId, int speed) {
    if (horseId <= 0 || speed <= 0) {
        return StatusType::INVALID_INPUT;
    }

    if (horses.find(horseId)) {
        return StatusType::FAILURE;
    }

    try {
        horses.insert(horseId, Horse(horseId, speed));
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

}

StatusType Plains::join_herd(int horseId, int herdId) {
//    if (horseId <= 0 || herdId <= 0) {
//        return StatusType::INVALID_INPUT;
//    }
//    auto horseNode = horses.find(horseId);
//    auto herdNode = horses.find(herdId);
//    auto emptyHerdNode = horses.find(herdId);
//
//    if (horseNode == nullptr || horseNode->data->herd != nullptr) {
//        return StatusType::FAILURE;
//    }
//    //todo: check if there no herd like this
    return StatusType::FAILURE;
}

StatusType Plains::follow(int horseId, int horseToFollowId) {
    return StatusType::FAILURE;
}

StatusType Plains::leave_herd(int horseId) {
    return StatusType::FAILURE;
}

output_t<int> Plains::get_speed(int horseId) {
    return 0;
}

output_t<bool> Plains::leads(int horseId, int otherHorseId) {
    return false;
}

output_t<bool> Plains::can_run_together(int herdId) {
    return false;
}
