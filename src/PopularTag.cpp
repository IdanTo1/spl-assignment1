//
// Created by idanto on 20/11/2019.
//

#include "../include/Session.h"


PopularTag::PopularTag() : _name(""), _count(0) {}

PopularTag::PopularTag(std::string name) : _name(name), _count(1) {}

PopularTag::PopularTag(const PopularTag& other) : _name(other._name), _count(other._count) {}

PopularTag::~PopularTag() {}

const std::string& PopularTag::getName() const {
    return _name;
}

int PopularTag::getCount() const {
    return _count;
}

void PopularTag::increaseCount() {
    _count++;
}

// overloaded < operator
bool PopularTag::operator<(const PopularTag& pt) {
    // check if pt is more popular, has bigger count (meaning the user has more Watchables in his history
    // containing this tag)
    if (this->_count < pt._count) {
        return true;
    }
    else if (this->_count > pt._count) {
        return false;
    }
    // if both tags have same popularity, compare by the classic string opertor>
    // that's because we want to sort count from large to small, but name from small to large
    else {
        return (this->_name > pt._name);
    }
}
