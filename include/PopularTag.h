//
// Created by idanto on 20/11/2019.
//

#ifndef POPULARTAG_H
#define POPULARTAG_H

#include <string>


class PopularTag {
    public:
        PopularTag();
        PopularTag(std::string);
        PopularTag(const PopularTag& other);
        ~PopularTag();
        const std::string& getName() const;
        int getCount() const;
        void increaseCount();
        // overloaded < operator
        bool operator<(const PopularTag&);
    private:
        std::string _name;
        int _count;
};

#endif //POPULARTAG_H
