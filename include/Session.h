#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include "Action.h"
#include "User.h"
#include "Watchable.h"
#include "PopularTag.h"
#include "json.hpp"


class User;

class Watchable;

enum ActionStringsEnum {
    CREATE_USER,
    CHANGE_USER,
    DELETE_USER,
    DUP_USER,
    LIST_CONTENT,
    WATCH_LIST,
    WATCH,
    LOG_ACTIONS, //because log exists in <vector>.
    EXIT_LOOP // because exit exists in <unordered_map>.
};

const std::string ACTION_PARAMS_DELIMITER = " ";
const std::string CONTINUE = "y";
const long NOTHING_TO_RECOMMEND = -1;

class Session {
    public:
        // Rule of 5 - constructors and destructor.
        Session(const std::string& configFilePath);
        ~Session();
        Session(const Session& rhs);
        Session& operator=(const Session& rhs);
        Session(Session&& rhs);
        Session& operator=(Session&& rhs);

        void start();
        // splits a string according to delimiter, and inserts sub strings to the vector.
        void split(std::string& actionString, std::vector<std::string>& actionParams, std::string delimiter);
        ActionStringsEnum strToEnum(const std::string& actionString);
        const std::vector<Watchable*>& getContent() const;
        User& getActiveUser();
        void setActiveUser(User&);
        void addToUserMap(User* user);
        void deleteUserFromMap(const std::string& name);
        const std::unordered_map<std::string, User*>& getUsers() const;
    private:
        std::vector<Watchable*> content;
        std::vector<BaseAction*> actionsLog;
        std::unordered_map<std::string, User*> userMap;
        User* activeUser;

        std::vector<std::string> extractTags(nlohmann::json& tagList);
        void fillContentFromJson(const std::string& configFilePath);
        template<typename T>
        void deepCopyPointerVector(const std::vector<T*>& newV, std::vector<T*>& ourV);
        void deepCopyUsers(const std::unordered_map<std::string, User*>& other);
        template<typename T>
        void cleanIterable(T* toDelete);
        void cleanUserMap();
        void clean();
};

#endif
