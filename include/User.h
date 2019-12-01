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

class User {
    public:
        User(const std::string& name);
        virtual ~User();
        std::string getName() const;
        std::vector<Watchable*> get_history() const;
        void setName(const std::string&);
        virtual void addToHistory(Watchable&);
        virtual Watchable* getRecommendation(Session& s) = 0;
        bool isInHistory(const Watchable&);
        void fixHistory(std::vector<Watchable*>& content);
        virtual User* clone() const = 0;
    protected:
        std::vector<Watchable*> history;
    private:
        std::string name;
};


class LengthRecommenderUser : public User {
    public:
        LengthRecommenderUser(const std::string& name);
        ~LengthRecommenderUser();
        virtual Watchable* getRecommendation(Session& s);
        virtual void addToHistory(Watchable&);
        virtual User* clone() const;
    private:
        float _avgWatchableLen; // a field containing the average watch time of tue user.
        void updateAverage();
};

class RerunRecommenderUser : public User {
    public:
        RerunRecommenderUser(const std::string& name);
        ~RerunRecommenderUser();
        virtual Watchable* getRecommendation(Session& s);
        virtual User* clone() const;
    private:
        int _lastRecIdx; // a field containing the last watchable that was recommended to the user.
};

class GenreRecommenderUser : public User {
    public:
        // Rule of 5 - constructors and destructor.
        GenreRecommenderUser(const std::string& name);
        GenreRecommenderUser(const GenreRecommenderUser& rhs);
        GenreRecommenderUser& operator=(const GenreRecommenderUser& rhs);
        GenreRecommenderUser(GenreRecommenderUser&& rhs);
        GenreRecommenderUser& operator=(GenreRecommenderUser&& rhs);
        ~GenreRecommenderUser();
        virtual Watchable* getRecommendation(Session& s);
        virtual void addToHistory(Watchable&);
        virtual User* clone() const;
    private:
        // a map holding tag's name, and a popular tag object + a vector of popular tag objects.
        // It was created like this so that we can update count in O(logn) and sorts the tags in O(nlogn)
        // (but usually better because we change finite number of tags).
        std::map<std::string, PopularTag*> _popularTagsMap;
        std::vector<PopularTag*> _popularTagsVector;
        void updatePopularTags();
        void cleanPopularTags(bool isDeepClean);
        void deepCopyPopularTags(const std::vector<PopularTag*>& tags);
};

#endif
