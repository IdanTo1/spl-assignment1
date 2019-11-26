#include "../include/Session.h"
#include <fstream>

Session::Session(const std::string &configFilePath):
    content(std::vector<Watchable*>()),
    actionsLog(std::vector<BaseAction*>()),
    userMap(std::unordered_map<std::string, User*>()),
    activeUser(nullptr)
{
    this->fillContentFromJson(configFilePath);
    User* defaultUser = new LengthRecommenderUser("default");
    userMap["default"] = defaultUser;
    activeUser = defaultUser;
}

Session::~Session()
{
    this->clean();
}
void Session::split(std::string &actionString, std::vector <string> &actionParams, std::string delimiter) {
    size_t start = 0U;
    size_t end = actionString.find(delimiter);
    while (end != std::string::npos)
    {
        actionParams.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }

}

ActionStringsEnum Session::strToEnum(const std::string &actionString) {
    if (actionString == "createuser")
        return createuser;
    else if (actionString == "changeuser")
        return changeuser;
    else if (actionString == "deleteuser")
        return deleteuser;
    else if (actionString == "dupuser")
        return dupuser;
    else if (actionString == "content")
        return content;
    else if (actionString == "watchlist")
        return watchlist;
    else if (actionString == "watch")
        return watch;
    else if (actionString == "log")
        return log;
    else if (actionString == "exit")
        return exit;
}

void Session::start() {
    std::string actionString;
    std::vector<std::string> actionParams;
    do {
        std::getline(std::cin, actionString);
        actionParams.clear();
        split(actionString, actionParams, ACTION_PARAMS_DELIMITER);
        ActionStringsEnum action = strToEnum(actionParams[0]);
        BaseAction* actionObj;
        switch (action) {
            case createuser:
                actionObj = new CreateUser(actionParams[1], actionParams[2]); //TODO do we assume valid input?
                actionsLog.push(actionObj);
                actionObj->act(this);
            case changeuser:
                actionObj = new ChangeActiveUser(actionParams[1]);
                actionsLog.push(actionObj);
                actionObj->act(this);
            case deleteuser:
                actionObj = new DeleteUser(actionParams[1]);
                actionsLog.push(actionObj);
                actionObj->act(this);
            case dupuser:
                actionObj = new DuplicateUser(actionParams[1], actionParams[2]);
                actionsLog.push(actionObj);
                actionObj->act(this);
            case content:
                actionObj = new PrintContentList();
                actionsLog.push(actionObj);
                actionObj->act(this);
            case watchlist:
                actionObj = new PrintWatchHistory();
                actionsLog.push(actionObj);
                actionObj->act(this);
            case watch:
                int nextId = int(actionParams[1]);
                std::string continueWatch = CONTINUE;
                do {
                    Watch watchObj = new Watch(content[nextId]);
                    actionsLog.push(watchObj);
                    watchObj->act(this);
                    //check for user input, for continue watching.
                    std::getline(std::cin, continueWatch);
                    // check for implicit new watch object (user said yes)
                    if (continueWatch != CONTINUE) {
                        break;
                    }
                    //make sure there is next watchable
                    while ((nextId = watchObj.getNextWatchableId()) != NOTHING_TO_RECCOMAND);
                    case logActions:
                        actionObj = new PrintActionsLog(actionsLog);
                    actionObj->act(this); //act first because log shouldn't be printed to log
                    actionsLog.push(actionObj);
                    case exitLoop:
                        actionObj = new PrintActionsLog(actionsLog);
                    actionsLog.push(actionObj);
                    actionObj->act(this);
                }
        }
    }
    while (action != exitLoop);
}

std::vector<std::string> Session::extractTags(nlohmann::json& tagList)
{
    std::vector<std::string> tags;
    for(auto tag : tagList){
        tags.push_back(tag);
    }
    return tags;
}

void Session::fillContentFromJson(const std::string &configFilePath)
{
    content = std::vector<Watchable*>();
    std::ifstream stream(configFilePath);
    nlohmann::json contentJson;
    stream >> contentJson;

    long currentId = 0;

    for(auto movieDesc: contentJson["movies"]) //iterate over movies and add them to content
    {
        Movie* movieObj = new Movie(currentId, movieDesc["name"],
                                     movieDesc["length"],
                                     this->extractTags(movieDesc["tags"]));
        content.push_back(movieObj);
        currentId++;
    }
    //iterate over tv series'. for each series and season add the corresponding episode
    for(auto seriesDesc: contentJson["tv_series"])
    {
        for(uint i = 0; i < seriesDesc["seasons"].size(); i++)
        {
            for(uint j = 0; j < seriesDesc["seasons"][i].size(); j++)
            {
                int nextEpisodeId = currentId+1;
                /* json operator[] returns a json object, which in this case represents a number
                    The .size() function isi therefore implemented in a way that returns
                    the number represented by the json object
                */
                if(j == seriesDesc["seasons"][i].size()-1 && i==seriesDesc["seasons"].size()-1)
                    nextEpisodeId = 0;
                //i+1 and j+1 because seasons and episodes start from 1
                Episode* episodeObj = new Episode(currentId, seriesDesc["name"],
                                                 seriesDesc["episode_length"],
                                                 i+1, j+1, nextEpisodeId,
                                                 this->extractTags(seriesDesc["tags"]));
                content.push_back(episodeObj);
                currentId++;
            }
        }
    }
}


const User& Session::getActiveUser() const
{
    return *activeUser;
}

void Session::setActiveUser(User& newUser) {
    activeUser = &newUser;
}

const std::vector<Watchable*>& Session::getContent() const
{
    return content;
}

const std::unordered_map<std::string,User*>& Session::getUsers() const
{
    return userMap;
}

void Session::addToUserMap(User* user)
{
    //Assuming this function is called after checking whether the addition is legal
    userMap[user->getName()] = user;
}

void Session::deleteUserFromMap(const std::string &name) {
    delete userMap[name];
    userMap.erase(name);
}

template<typename T>
void Session::cleanIterable(T* toDelete) //T should be iterable
{
    for(auto w: *toDelete)
    {
        delete w;
    }
    delete toDelete;
}

void Session::cleanUserMap()
{
    for(auto w: userMap)
    {
        delete w.second;
    }
}

void Session::clean() {
    activeUser = nullptr;
    cleanIterable(&content);
    cleanIterable(&actionsLog);
    cleanUserMap();
}

Session::Session(const Session &rhs)
: content(rhs.content), actionsLog(rhs.actionsLog), userMap(rhs.userMap),
 activeUser(userMap[rhs.activeUser->getName()])
{}

void Session::deepCopyUsers(const std::unordered_map<std::string, User*>& other)
{
    for(auto u: other)
    {
        *(this->userMap[u.first]) = *(u.second);
    }
}

//Assuming that ourV is empty
template<typename T>
void Session::deepCopyPointerVector(const std::vector<T*>& newV, std::vector<T*>& ourV)
{
    for(auto x: newV)
    {
        //create a new T object using T's copy constructor
        ourV.push_back(x->clone());
    }
}

Session& Session::operator=(const Session& rhs)
{
    if(this != &rhs)
    {
        clean();
        this->deepCopyPointerVector(rhs.content, this->content);
        this->deepCopyPointerVector(rhs.actionsLog, this->actionsLog);
        this->deepCopyUsers(rhs.userMap);
        this->activeUser = userMap[rhs.activeUser->getName()];
    }
    return *this;
}

Session::Session(Session && rhs) = default;

Session& Session::operator=(Session && rhs) = default;
