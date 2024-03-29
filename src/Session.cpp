#include "../include/Session.h"
#include <fstream>


Session::Session(const std::string& configFilePath) :
        content(std::vector<Watchable*>()),
        actionsLog(std::vector<BaseAction*>()),
        userMap(std::unordered_map<std::string, User*>()),
        activeUser(nullptr) {
    this->fillContentFromJson(configFilePath);
    User* defaultUser = new LengthRecommenderUser("default");
    userMap["default"] = defaultUser;
    activeUser = defaultUser;
}

Session::~Session() {
    this->clean();
}


void Session::split(std::string& actionString, std::vector<std::string>& actionParams,
                    std::string delimiter) {
    size_t start = 0U;
    size_t end = actionString.find(delimiter);
    while (end != std::string::npos) {
        actionParams.push_back(actionString.substr(start, end - start));
        start = end + delimiter.length();
        end = actionString.find(delimiter, start);
    }
    actionParams.push_back(actionString.substr(start, actionString.size() - start + 1));

}

ActionStringsEnum Session::strToEnum(const std::string &actionString) {
    if (actionString == "createuser")
        return CREATE_USER;
    else if (actionString == "changeuser")
        return CHANGE_USER;
    else if (actionString == "deleteuser")
        return DELETE_USER;
    else if (actionString == "dupuser")
        return DUP_USER;
    else if (actionString == "content")
        return LIST_CONTENT;
    else if (actionString == "watchhist")
        return WATCH_LIST;
    else if (actionString == "watch")
        return WATCH;
    else if (actionString == "log")
        return LOG_ACTIONS;
    else
        return EXIT_LOOP; //if actionString == "exit" or invalid input, program will exit
}

void Session::start() {
    std::cout << "SPLFLIX is now on!" << std::endl;
    std::string actionString;
    std::vector<std::string> actionParams;
    ActionStringsEnum action;
    do {
        // receive action from user and parse it.
        std::getline(std::cin, actionString);
        actionParams.clear();
        split(actionString, actionParams, ACTION_PARAMS_DELIMITER);
        action = strToEnum(actionParams[0]);
        BaseAction* actionObj;
        // according to user request, create action object to perform desired.
        switch (action) {
            case CREATE_USER: {
                actionObj = new CreateUser(actionParams[1], actionParams[2]);
                actionsLog.push_back(actionObj);
                actionObj->act(*this);
                break;
            }
            case CHANGE_USER: {
                actionObj = new ChangeActiveUser(actionParams[1]);
                actionsLog.push_back(actionObj);
                actionObj->act(*this);
                break;
            }
            case DELETE_USER: {
                actionObj = new DeleteUser(actionParams[1]);
                actionsLog.push_back(actionObj);
                actionObj->act(*this);
                break;
            }
            case DUP_USER: {
                actionObj = new DuplicateUser(actionParams[1], actionParams[2]);
                actionsLog.push_back(actionObj);
                actionObj->act(*this);
                break;
            }
            case LIST_CONTENT: {
                actionObj = new PrintContentList();
                actionsLog.push_back(actionObj);
                actionObj->act(*this);
                break;
            }
            case WATCH_LIST: {
                actionObj = new PrintWatchHistory();
                actionsLog.push_back(actionObj);
                actionObj->act(*this);
                break;
            }
            case WATCH: {
                long nextId = std::stol(actionParams[1]) - 1;
                std::string continueWatch;
                Watch* watchObj = new Watch(*(content[nextId]));
                actionsLog.push_back(watchObj);
                watchObj->act(*this);
                // because implicit Watch may be initialized, inner loop is required.
                // as long as there is content to recommend on, and the user wish to watch it, keep creating
                // watch objects.
                while ((nextId = watchObj->getNextWatchableId()) != NOTHING_TO_RECOMMEND) {
                    // check for implicit new watch object (user wishes to continue watching)
                    std::getline(std::cin, continueWatch);
                    if (continueWatch != CONTINUE) {
                        break;
                    }
                    watchObj = new Watch(*(content[nextId]));
                    actionsLog.push_back(watchObj);
                    watchObj->act(*this);
                }
                break;
            }
            case LOG_ACTIONS: {
                actionObj = new PrintActionsLog(actionsLog);
                actionObj->act(*this); //act first because log shouldn't be printed to log
                actionsLog.push_back(actionObj);
                break;
            }
            case EXIT_LOOP: {
                actionObj = new Exit();
                actionsLog.push_back(actionObj);
                actionObj->act(*this);
                break;
            }
        }
    } while (action != EXIT_LOOP);
}

std::vector<std::string> Session::extractTags(nlohmann::json& tagList) {
    std::vector<std::string> tags;
    for (auto tag : tagList) {
        tags.push_back(tag);
    }
    return tags;
}

void Session::fillContentFromJson(const std::string& configFilePath) {
    std::ifstream stream(configFilePath);
    nlohmann::json contentJson;
    stream >> contentJson;

    long currentId = 0;

    for (auto movieDesc: contentJson["movies"]) //iterate over movies and add them to content
    {
        Movie* movieObj = new Movie(currentId, movieDesc["name"],
                                    movieDesc["length"],
                                    this->extractTags(movieDesc["tags"]));
        content.push_back(movieObj);
        currentId++;
    }
    //iterate over tv series'. for each series and season add the corresponding episode
    for (auto seriesDesc: contentJson["tv_series"]) {
        // size() returns the size of the array, which in this case is the number of seasons
        for (uint i = 0; i < seriesDesc["seasons"].size(); i++) {
            // Convert the content of the array into ints to use in the for loop
            uint episodesNum = seriesDesc["seasons"][i];
            for (uint j = 0; j < episodesNum; j++) {
                int nextEpisodeId = currentId + 1;
                if (j == episodesNum - 1 && i == seriesDesc["seasons"].size() - 1) {
                    nextEpisodeId = 0;
                }
                //i+1 and j+1 because seasons and episodes start from 1
                Episode* episodeObj = new Episode(currentId, seriesDesc["name"],
                                                  seriesDesc["episode_length"],
                                                  i + 1, j + 1, nextEpisodeId,
                                                  this->extractTags(seriesDesc["tags"]));
                content.push_back(episodeObj);
                currentId++;
            }
        }
    }
}


User& Session::getActiveUser() {
    return *activeUser;
}

void Session::setActiveUser(User& newUser) {
    activeUser = &newUser;
}

const std::vector<Watchable*>& Session::getContent() const {
    return content;
}

const std::unordered_map<std::string, User*>& Session::getUsers() const {
    return userMap;
}

void Session::addToUserMap(User* user) {
    //Assuming this function is called after checking whether the addition is legal
    userMap[user->getName()] = user;
}

void Session::deleteUserFromMap(const std::string& name) {
    delete userMap[name];
    userMap.erase(name);
}

template<typename T>
void Session::cleanIterable(T* toDelete) //T should be iterable
{
    for (auto w: *toDelete) {
        delete w;
        w = nullptr;
    }
}

void Session::cleanUserMap() {
    for (auto w: userMap) {
        delete w.second;
        w.second = nullptr;
    }
}

void Session::clean() {
    activeUser = nullptr;
    cleanIterable(&content);
    cleanIterable(&actionsLog);
    cleanUserMap();
}

Session::Session(const Session& rhs) : content(), actionsLog(), userMap(), activeUser() {
    this->deepCopyPointerVector(rhs.content, this->content);
    this->deepCopyPointerVector(rhs.actionsLog, this->actionsLog);
    this->deepCopyUsers(rhs.userMap);
    this->activeUser = this->userMap[rhs.activeUser->getName()];
}

void Session::deepCopyUsers(const std::unordered_map<std::string, User*>& other) {
    for (auto u: other) {
        User* newUser = u.second->clone();
        this->userMap[u.first] = newUser;
        newUser->fixHistory(content);
    }
}

//Assuming that ourV is empty
template<typename T>
void Session::deepCopyPointerVector(const std::vector<T*>& newV, std::vector<T*>& ourV) {
    for (auto x: newV) {
        //create a new T object using T's copy constructor
        ourV.push_back(x->clone());
    }
}

Session& Session::operator=(const Session& rhs) {
    if (this != &rhs) {
        clean();
        this->deepCopyPointerVector(rhs.content, this->content);
        this->deepCopyPointerVector(rhs.actionsLog, this->actionsLog);
        this->deepCopyUsers(rhs.userMap);
        this->activeUser = userMap[rhs.activeUser->getName()];
    }
    return *this;
}

Session::Session(Session&& rhs) = default;

Session& Session::operator=(Session&& rhs) = default;
