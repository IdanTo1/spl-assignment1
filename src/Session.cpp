#include "../include/Session.h"
#include <fstream>

Session::Session(const std::string &configFilePath)
{
    this->fillContentFromJson(configFilePath);
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
        Movie* movieObj = new Movie(currentId, movieDesc["name"], movieDesc["length"], this->extractTags(movieDesc["tags"]));
        content.push_back(movieObj);
        currentId++;
    }

    for(auto seriesDesc: contentJson["tv_series"]) //iterate over tv series'. for each series and season add the corresponding episode
    {
        for(uint i = 0; i < seriesDesc["seasons"].size(); i++)
        {
            for(uint j = 0; j < seriesDesc["seasons"][i]; j++)
            {
                Episode* episodeObj= nullptr;
                if(j == seriesDesc["seasons"][i]-1)
                    episodeObj = new Episode(currentId, seriesDesc["name"], seriesDesc["episode_length"], i+1, j+1, true, this->extractTags(seriesDesc["tags"])); //i+1 and j+1 because we want the first episode to be S1E1
                else episodeObj = new Episode(currentId, seriesDesc["name"], seriesDesc["episode_length"], i+1, j+1, this->extractTags(seriesDesc["tags"]));
                content.push_back(episodeObj);
                currentId++;
            }
        }
    } 
}

std::vector<std::string> Session::extractTags(nlohmann::json& tagList)
{
    std::vector<std::string> tags;
    for(auto tag : tagList){
        tags.push_back(tag);
    }
    return tags;
}

const User& Session::getActiveUser() const
{
    return *activeUser;
}

const std::vector<Watchable*>& Session::getContent() const
{
    return content;
}
