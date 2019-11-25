#include "../include/Session.h"
#include <fstream>

Session::Session(const std::string &configFilePath):
    content(std::vector<Watchable*>()),
    actionsLog(std::vector<BaseAction*>()),
    userMap(std::unordered_map<std::string, User*>()),
    activeUser(nullptr)
{
    this->fillContentFromJson(configFilePath);
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

void Session::clean()
{
    activeUser = nullptr;
    cleanIterable(&content);
    cleanIterable(&actionsLog);
    cleanUserMap();
}

Session::Session(const Session &rhs)
: content(rhs.content), actionsLog(rhs.actionsLog), userMap(rhs.userMap),
 activeUser(userMap[rhs.activeUser->getName()]) 
{}

Session::~Session()
{
    this->clean();
}

Session& Session::operator=(const Session& rhs)
{
    if(this != &rhs)
    {
        this->clean();
        this->content = rhs.content;
        this->userMap = rhs.userMap;
        this->actionsLog = rhs.actionsLog;
        this->activeUser = userMap[rhs.activeUser->getName()];
    }
    return *this;
}

Session::Session(Session && rhs)
: content(rhs.content), actionsLog(rhs.actionsLog), userMap(rhs.userMap),
 activeUser(userMap[rhs.activeUser->getName()])
{
   purgeSession(rhs);
}

Session& Session::operator=(Session && rhs)
{
    if(this != &rhs)
    {
        this->clean();
        this->content = rhs.content;
        this->actionsLog = rhs.actionsLog;
        this->userMap = rhs.userMap;
        purgeSession(rhs);
    }
    return *this;
}

void Session::purgeSession(Session& s)
{
     /* Since the vectors are held by value we have to give it an empty one,
        which will be quickly deleted anyway
    */
    s.content = std::vector<Watchable*>();
    s.actionsLog = std::vector<BaseAction*>();
    s.userMap = std::unordered_map<std::string, User*>();
    s.activeUser = nullptr;
}
