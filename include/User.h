#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "PopularTag.h"

class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    virtual void addToHisory(Watchable*);
protected:
    std::vector<Watchable*> history;
private:
    const std::string name;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
private:
    float _avgWatchableLen;
    void updateAverage();
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
private:
    int lastRecIdx;
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
private:
    std::map<std::string, &PopularTag> _popularTagsMap;
    std::vector<popularTag> _popularTagsVector;
    void updatePopularTags();
};

#endif
