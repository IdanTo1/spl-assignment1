#include "../include/Session.h"

// User's related methods implementation.
User::User(const std::string& name): history(), name(name) {}

User::~User() {}

std::string User::getName() const {
    return name;
}

std::vector<Watchable*> User::get_history() const {
    return history;
}

void User::addToHistory(Watchable& watchable) {
    history.push_back(&watchable);
}

bool User::isInHistory(const Watchable& watchable) {
    return std::find(history.begin(), history.end(), &watchable) != history.end();
}

void User::setName(const std::string& newName) {
    name = newName;
}

void User::fixHistory(std::vector<Watchable*>& content)
{
    int index = 0;
    for(auto w : history)
    {
        history[index++] = content[w->getId()];
    }
}

// Length Recommender User's methods implementation.
LengthRecommenderUser::LengthRecommenderUser(const std::string& name): User(name), _avgWatchableLen(0.0f) {}

User* LengthRecommenderUser::clone() const {
    return new LengthRecommenderUser(*this);
}
LengthRecommenderUser::~LengthRecommenderUser() {}

void LengthRecommenderUser::updateAverage() {
    // we assume, that when updateAverage is called, new watchable has already been added to the history
    // vector. hence, we decrease by 1 history.size(), in the multiplication, and using history.size() in
    // the division
    _avgWatchableLen = (_avgWatchableLen * (history.size() - 1) + history.back()->getLength())
                       / history.size();
}


void LengthRecommenderUser::addToHistory(Watchable& watchable) {
    history.push_back(&watchable);
    //  update average according to newly watched
    updateAverage();
}

Watchable* LengthRecommenderUser::getRecommendation(Session& s) {
    std::vector<Watchable*> content = s.getContent();
    // set min difference to be max value before iterating, and recommended watchable to be null
    float minDiff = std::numeric_limits<float>::max();
    Watchable* recommendedWatchable = nullptr;
    //iterate over all content vector, and compare it to min diff found
    float curDiff;
    for (auto iter = content.begin(); iter != content.end(); iter++)
    {
        curDiff =  abs((*iter)->getLength() - _avgWatchableLen);
        if (curDiff < minDiff && !isInHistory(**iter)) {
            recommendedWatchable = *iter;
            minDiff = curDiff;
        }
    }
    return recommendedWatchable;
}

// Rerun Recommender User's methods implementation.
RerunRecommenderUser::RerunRecommenderUser(const std::string& name):
        User(name), _lastRecIdx(-1) {}

RerunRecommenderUser::~RerunRecommenderUser() {}

User* RerunRecommenderUser::clone() const{
    return new RerunRecommenderUser(*this);
}

Watchable* RerunRecommenderUser::getRecommendation(Session& s) {
    if (_lastRecIdx == -1) {
        _lastRecIdx ++;
        return *(history.begin());
    }
    _lastRecIdx = (_lastRecIdx+1) % history.size();
    return history[_lastRecIdx];
}


// Genre Recommender User's methods implementation.
GenreRecommenderUser::GenreRecommenderUser(const std::string& name): User(name), _popularTagsMap(),
                                                                     _popularTagsVector() {}

GenreRecommenderUser::~GenreRecommenderUser() {}

User* GenreRecommenderUser::clone() const{
    return new GenreRecommenderUser(*this);
}

void GenreRecommenderUser::updatePopularTags() {
    std::vector<std::string> lastWatchedTags = history.back()->getTags();
    //define iter, go through every tag, and update it in the data structure.
    for (auto iter = lastWatchedTags.begin(); iter != lastWatchedTags.end(); iter++) {
        // if tag doesn't exists in the map, add it
        if (!_popularTagsMap.count(*iter)) {
            _popularTagsVector.push_back(PopularTag(*iter));
            _popularTagsMap[*iter] = &(_popularTagsVector.back());
        }
        else {
            _popularTagsMap[*iter]->increaseCount();
        }
        // sorts by PopularTag's operator<()
        std::sort(_popularTagsVector.begin(), _popularTagsVector.end());
    }
}


void GenreRecommenderUser::addToHistory(Watchable& watchable) {
    history.push_back(&watchable);
    // update popular tags according to newly watched
    updatePopularTags();
}

Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    std::vector<Watchable*> content = s.getContent();
    std::vector<Watchable*>::iterator contentIter;
    std::vector<PopularTag>::iterator popularTagsIter;
    // iterate over all user's popular tags, from most to least popular.
    for (popularTagsIter = _popularTagsVector.begin(); popularTagsIter != _popularTagsVector.end();
         popularTagsIter++) {
        // iterate over the content, to check whether there is a Watchable containing this tag.
        for (contentIter = content.begin(); contentIter != content.end(); contentIter++) {
            // check that the watchable wasn't seen already
            if (!isInHistory(**contentIter)) {
                // extract the Watchable's tags vector.
                const std::vector<std::string> &watchableTags = (*contentIter)->getTags();
                // check if popular tag exist in Watchable's tags vector
                if (std::find(watchableTags.begin(), watchableTags.end(), popularTagsIter->getName())
                    != watchableTags.end()) {
                    return *contentIter;
                }
            }
        }
    }
    return nullptr;
}
