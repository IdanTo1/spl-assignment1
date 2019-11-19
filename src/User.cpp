#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"

User::User(const std::string& name): name(name) {}
std::string User::  getName() const {
    return this->name;
}
std::vector<Watchable*> User::get_history() const {
    return this->history;
}

LengthRecommenderUser::LengthRecommenderUser(const std::string& name): name(name), _avgWatchableLen(0.0f) {}

virtual Watchable* LengthRecommenderUser::getRecommendation(Session& s) {
    float minDiff = std::numeric_limits<float>::max();
    float curDiff = 0.0f
    Watchable* recommendedWatchable = nullptr;
    for (Watchable* watchable : s.getContent())
    {
        curDiff =  abs(watchable.length - this->_avgWatchableLen);
        if (curDiff < minDiff) {
            recommendedWatchable = watchable;
            minDiff = curDiff;
        }
    }
    return recommendedWatchable;
}


virtual Watchable* LengthRecommenderUser::updateUser(Session& s) {
    addWatchableToHistory(curWatchable);
    updateAverage(curWatchable);
    return getRecommendation(s);
}