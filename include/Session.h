#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
#include "User.h"
#include "Watchable.h"
#include "PopularTag.h"
#include "../include/json.hpp"


class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    void start();

    const std::vector<Watchable*>& getContent() const;
    const User& getActiveUser() const;
    void setActiveUser(User&);
    Session(const Session& rhs);
    Session& operator=(const Session& rhs);
    Session(Session&& rhs);
    Session& operator=(Session&& rhs);
    void addToUserMap(User* user);
    void deleteUserFromMap(const std::string& name);
    const std::unordered_map<std::string,User*>& getUsers() const;
private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;

    std::vector<std::string> extractTags(nlohmann::json& tagList);
    void fillContentFromJson(const std::string &configFilePath);
    template<typename T>
    void cleanIterable(T* toDelete);
    void clean();
    void cleanUserMap();
};
#endif
