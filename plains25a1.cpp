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
        emptyHerds.insert(herdId, make_unique<Herd>(herdId));
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
        horses.insert(horseId, make_unique<Horse>(horseId, speed));
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

}

StatusType Plains::join_herd(int horseId, int herdId) {
    if (horseId <= 0 || herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    auto horseNode = horses.find(horseId);
    if (!horseNode || horseNode->data->herd) {
        return StatusType::FAILURE;
    }

    auto herdNode = herds.find(herdId);
    auto emptyHerdNode = emptyHerds.find(herdId);
    if (!herdNode && !emptyHerdNode) {
        return StatusType::FAILURE;
    }
    assert ((!herdNode && emptyHerdNode) || (herdNode && !emptyHerdNode));

    try {
        if (emptyHerdNode) {
            herdNode = moveHerdFromEmpty(herdId, emptyHerdNode);
        }
        horseNode->data->herd = herdNode->data.get();
        herdNode->data->horses.insert(horseId, make_unique<Horse*>(horseNode->data.get()));
    }
    catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    horseNode->data->joinHerdTime = this->clock;
    clock++;
    return StatusType::SUCCESS;
}

StatusType Plains::follow(int horseId, int horseToFollowId) {
    if (horseId <= 0 || horseToFollowId <= 0 || horseId == horseToFollowId) {
        return StatusType::INVALID_INPUT;
    }
    auto horseNode = horses.find(horseId);
    auto horseToFollowNode = horses.find(horseToFollowId);
    if (!horseNode || !horseToFollowNode ||
        horseNode->data->herd != horseToFollowNode->data->herd) {
        return StatusType::FAILURE;
    }
    horseNode->data->leader = horseToFollowNode->data.get();
    horseNode->data->followTime = this->clock;
    this->clock++;
    return StatusType::SUCCESS;

}

StatusType Plains::leave_herd(int horseId) {
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    auto horseNode = horses.find(horseId);
    if (!horseNode || !horseNode->data->herd) {
        return StatusType::FAILURE;
    }
    auto herd = horseNode->data->herd;
    try {
        herd->horses.remove(herd->horses.find(horseId));
        if (herd->isEmpty()) {
            moveHerdToEmpty(herd->id);
        }
    } catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    horseNode->data->herd = nullptr;
    return StatusType::SUCCESS;

}

output_t<int> Plains::get_speed(int horseId) {
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    auto horseNode = horses.find(horseId);
    if (!horseNode) {
        return StatusType::FAILURE;
    }
    return horseNode->data->speed;
}

output_t<bool> Plains::leads(int horseId, int otherHorseId) {
    if (horseId <= 0 || otherHorseId <= 0 || horseId == otherHorseId) {
        return StatusType::INVALID_INPUT;
    }
    auto horseNode = horses.find(horseId);
    auto otherHorseNode = horses.find(otherHorseId);
    if (!horseNode || !otherHorseNode) {
        return StatusType::FAILURE;
    }

    Horse* currHorse = horseNode->data.get();
    Horse* currLeader = getLeader(currHorse);
    while (currLeader && currLeader != otherHorseNode->data.get()) {
        currHorse = currLeader;
        currLeader = getLeader(currHorse);
    }
    if (currLeader == otherHorseNode->data.get()) {
        return true;
    }
    return false;
}

output_t<bool> Plains::can_run_together(int herdId) {

    return false;
}

Node<Herd>* Plains::moveHerdFromEmpty(int herdId, Node<Herd>* nodeToRemove) {
    emptyHerds.remove(nodeToRemove);
    return herds.insert(herdId, make_unique<Herd>(herdId));
}

void Plains::moveHerdToEmpty(int herdId) {
    herds.remove(herds.find(herdId));
    emptyHerds.insert(herdId, make_unique<Herd>(herdId));
}

Horse* Plains::getLeader(Horse* horse) const {
    if (!horse->leader || horse->herd != horse->leader->herd ||
        horse->leader->joinHerdTime >= horse->followTime) {
        return nullptr;
    }
    return horse->leader;

}

int Plains::countNonFollowers(int herdId) const {
    
}