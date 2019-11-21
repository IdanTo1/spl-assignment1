//
// Created by idanto on 20/11/2019.
//

#ifndef POPULARTAG_H
#define POPULARTAG_H


class PopularTag {
    public:
        PopularTag(std::string&);
        const std::string& getName() const;
        int getCount() const;
        void increaseCount();
        // overloaded < operator
        bool operator <(const PopularTag&);
    private:
        const std::string _name; // i won't use reference to prevent conflicts.
        int _count;
};


#endif //POPULARTAG_H
