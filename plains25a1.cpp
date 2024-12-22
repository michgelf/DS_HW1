// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"
#include <exception>


Plains::Plains() : clock(0), horses(), herds(), emptyHerds() {}

Plains::~Plains() = default;

StatusType Plains::add_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (herds.find(herdId) || emptyHerds.find(herdId)) {
        return StatusType::FAILURE;
    }
    try {
        emptyHerds.insert(herdId, unique_ptr<int>(new int(herdId)));
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Plains::remove_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    auto herdToRemoveNode = emptyHerds.find(herdId);
    if (!herdToRemoveNode) {
        return StatusType::FAILURE;
    }
    try {
        emptyHerds.remove(herdToRemoveNode);
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
        horses.insert(horseId, unique_ptr<Horse>(new Horse(horseId, speed)));
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
    // todo if found herd ID dont check empty herds
    auto emptyHerdNode = emptyHerds.find(herdId);
    if (!herdNode && !emptyHerdNode) {
        return StatusType::FAILURE;
    }
    assert ((!herdNode && emptyHerdNode) || (herdNode && !emptyHerdNode));

    try {
        if (emptyHerdNode) {
            herdNode = moveHerdFromEmpty(herdId, emptyHerdNode);
        }
        Horse* horse = horseNode->data.get();
        assert(horse->leader == nullptr && horse->followTime == -1 && horse->joinHerdTime == -1);
        Herd* herd = herdNode->data.get();

        herd->horses.insert(horseId, unique_ptr<Horse*>(new Horse*(horse)));
        horse->herd = herdNode->data.get();
        horse->joinHerdTime = this->clock;
        clock++;
    } catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType Plains::follow(int horseId, int horseToFollowId) {
    if (horseId <= 0 || horseToFollowId <= 0 || horseId == horseToFollowId) {
        return StatusType::INVALID_INPUT;
    }
    auto horseNode = horses.find(horseId);
    auto horseToFollowNode = horses.find(horseToFollowId);
    if (!horseNode || !horseToFollowNode || !horseNode->data->herd ||
        horseNode->data->herd != horseToFollowNode->data->herd) {
        return StatusType::FAILURE;
    }
    Horse* horse = horseNode->data.get();
    horse->leader = horseToFollowNode->data.get();
    assert(horseToFollowNode->data->joinHerdTime < this->clock);
    horse->followTime = this->clock;
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
    Herd* herd = horseNode->data->herd;
    try {
        herd->horses.remove(herd->horses.find(horseId));
        if (herd->isEmpty()) {
            moveHerdToEmpty(herd->id);
        }
    } catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
    resetHorse(horseNode->data.get());
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

    Horse* horse = horseNode->data.get();
    Horse* otherHorse = otherHorseNode->data.get();
    if (!horse->herd || !otherHorse->herd || horse->herd != otherHorse->herd) {
        return false;
    }

    resetPaths(horse->herd);
    return checkAndPlant(horse, otherHorse);
}


output_t<bool> Plains::can_run_together(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    auto herdNode = herds.find(herdId);
    if (!herdNode) {
        return StatusType::FAILURE;
    }

    Herd* herd = herdNode->data.get();

    auto candidateNode = herd->horses.findFirstMatchingNode(
            [](Horse* horse) { return getLeader(horse) == nullptr; });
    if (!candidateNode) {
        return false;
    }
    auto candidateHorse = *(candidateNode->data);

    resetPaths(herd);
    return herd->horses.applyFuncBool(
            [candidateHorse](Horse* horse) { return checkAndPlant(horse, candidateHorse); });
}

// private methods

// functions are used inside try
Node<Herd>* Plains::moveHerdFromEmpty(int herdId, Node<int>* nodeToRemove) {
    emptyHerds.remove(nodeToRemove);
    return herds.insert(herdId, unique_ptr<Herd>(new Herd(herdId)));
}

// functions are used inside try
void Plains::moveHerdToEmpty(int herdId) {
    herds.remove(herds.find(herdId));
    emptyHerds.insert(herdId, unique_ptr<int>(new int(herdId)));
}

Horse* Plains::getLeader(Horse* horse) {
    if (!horse->leader || horse->herd != horse->leader->herd ||
        horse->leader->joinHerdTime >= horse->followTime) {
        return nullptr;
    }
    return horse->leader;
}

void Plains::resetPaths(Herd* herd) {
    herd->horses.applyFunc([](Horse* horse) { horse->pathId = 0; });
}

bool Plains::checkAndPlant(Horse* horse, Horse* candidate) {
    if (horse->pathId != 0 || horse == candidate) { // already seen horse or the same horse
        return true;
    }
    horse->pathId = horse->id;

    Horse* currHorse = horse;
    Horse* currLeader = getLeader(horse);
    while (true) {
        if (currLeader == candidate) {
            return true;
        }
        if (currLeader == nullptr) {
            return false;
        }
        if (currLeader->pathId != 0) {
            if (currHorse->pathId == currLeader->pathId) { // there is a loop
                return false;
            }
            return true; // already seen currHorse
        }

        currLeader->pathId = horse->id; // planting
        currHorse = currLeader;
        currLeader = getLeader(currHorse);
    }
}

void Plains::resetHorse(Horse* horse) {
    horse->herd = nullptr;
    horse->leader = nullptr;
    horse->followTime = -1;
    horse->joinHerdTime = -1;
}

