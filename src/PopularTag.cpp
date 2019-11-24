//
// Created by idanto on 20/11/2019.
//

#include "../include/PopularTag.h"

PopularTag::PopularTag(std::string name): _name(name), _count(1) {}

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
bool PopularTag::operator <(const PopularTag& pt) {
    // check if pt is more popular, has bigger count (meaning User has more Watchables in is
    //  hisorty containing this tag)
    if(this->_count < pt._count) {
        return true;
    }
    else if (this->_count > pt._count) {
        return  false;
    }
    //if both tags have same popularity, compare by the classic string opertor<
    else {
        return (this->_name < pt._name);
    }
}