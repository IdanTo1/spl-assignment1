#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <limits>
#include "PopularTag.h"

class Watchable;
class Session;

class User{
public:
    User(const std::string& name);
    virtual ~User();
    virtual User* clone() const = 0;
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    virtual void addToHistory(Watchable&);
    bool isInHistory(const Watchable&);
protected:
    std::vector<Watchable*> history;
private:
    std::string name;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual User* clone() const;
    virtual Watchable* getRecommendation(Session& s);
    virtual void addToHistory(Watchable&);
private:
    float _avgWatchableLen; // a field containing the average watch time of tue user.
    void updateAverage();
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual User* clone() const;
    virtual Watchable* getRecommendation(Session& s);
private:
    int _lastRecIdx; // a field containing the last watchable that was recommended to the user.
};

class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual User* clone() const;
    virtual Watchable* getRecommendation(Session& s);
    virtual void addToHistory(Watchable&);
private:
    // a map holding tag's name, and a popular tag object + a vector of popular tag objects.
    // It was created like this so that we can update count in O(logn) and sorts the tags in O(nlogn)
    // (but usually better because we change finite number of tags).
    std::map<std::string, PopularTag*> _popularTagsMap;
    std::vector<PopularTag> _popularTagsVector;
    void updatePopularTags();
};

#endif
