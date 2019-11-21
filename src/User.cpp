#include "../include/Session.h"

// User's related methods implementation.
User::User(const std::string& name): name(name) {}

std::string User::getName() const {
    return name;
}

std::vector<Watchable*> User::get_history() const {
    return history;
}

void User::addToHisory(Watchable* watchable) {
    history.push_back(watchable);
}


// Length Recommender User's methods implementation.
LengthRecommenderUser::LengthRecommenderUser(const std::string& name): User(name), _avgWatchableLen(0.0f) {}

void LengthRecommenderUser::updateAverage() {
    // we assume, that when updateAverage is called, new watchable have already been added to the history vector.
    // hence, we decrease by 1 history.size(), in the multiplication, and using history.size() in the
    // division
    _avgWatchableLen = (_avgWatchableLen * (history.size() - 1) + history.back()->getLength()) / history.size();
}

void LengthRecommenderUser::addToHisory(Watchable* watchable) {
    history.push_back(watchable);
    //  update average according to newly watched
    updateAverage();
}

Watchable* LengthRecommenderUser::getRecommendation(Session& s) {
    // create an iterator to parse over content.
    std::vector<Watchable*> content = s.getContent();
    std::vector<Watchable*>::iterator iter = content.begin();

    // set the recommended movie to the first one in content.
    float minDiff = abs((*iter)->getLength() - _avgWatchableLen);
    Watchable* recommendedWatchable = *iter; //TODO what if first element in history? fix.

    //iterate over all content vector, and compare it to min diff found
    float curDiff;
    for (iter = iter +1; iter != content.end(); iter++)
    {
        curDiff =  abs((*iter)->getLength() - _avgWatchableLen);
        if (curDiff < minDiff) { //TODO and not in history!
            recommendedWatchable = *iter;
            minDiff = curDiff;
        }
    }
    return recommendedWatchable;
}

// Rerun Recommender User's methods implementation.
RerunRecommenderUser::RerunRecommenderUser(const std::string& name): User(name), lastRecIdx(-1) {}
lastRecIdx

RerunRecommenderUser::getRecommendation(Session& s) {
    if (lastRecIdx == -1) {
        return history.begin()
    }
    lastRecIdx = (lastRecIdx+1) % history.size()
    return history[lastRecIdx]
}

GenreRecommenderUser::GenreRecommenderUser(const std::string& name): User(name),

void GenreRecommenderUser::updatePopularTags() {
    std::vector<std::string> lastWatchedTags = history.back()->getTags;
    //define iter, go through every tag, and update it in the data structure.
    for (std::vector<string>::iterator iter = lastWatchedTags.begin(); iter != lastWatchedTags.end(); iter++) {
        // if tag doesn't exists in the map, add it
        if (!popularTagsMap.count(*iter)) {
            PopularTag newTag(*iter);
            _popularTagsVector.push_back(newTag);
            _popularTagsMap[*iter] = newTag;
        }
        else {
            _popularTagsMap[*iter].increaseCount();
        }
        // sorts by PopularTag's operator<()
        sort(popularTagsVector.begin(), popularTagsVector.end());
    }
}


void GenreRecommenderUser::addToHisory(Watchable* watchable) {
    history.push_back(watchable);
    // update average according to newly watched
    updatePopularTags();

    std::vector<Watchable*> content = s.getContent();
    std::vector<Watchable*>::iterator contentIter;
    std::vector<PopularTag>::iterator popularTagsIter;
    // iterate over all user's popular tags, from most to least popular.
    for (popularTagsIter = _popularTagsVector.begin(), popularTagsIter != _popularTagsVector.end(); popularTagsIter++) {
        // iterate over the content, to check whether there is a Watchable containing this tag.
        for (contentIter = content.begin(); contentIter != content.end(); contentIter++) {
            // extract the Watchable's tags vector.
            vactor <std::string> &watchableTags = (*contentIter)->tags;
            // check if popular tag exist in Watchable's tags vector.
            if (std::find(watchableTags.begin(), watchableTags.end(), popularTagsIter->getName())
                    != watchableTags.end()) {  //TODO and not in history!!
                return *contentIter;
            }

        }
    }
    return nullptr
}