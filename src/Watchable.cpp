#include "../include/Session.h"


Watchable::Watchable(long id, int length, const std::vector<std::string>& tags)
: id(id), length(length), tags(tags){}

std::string Watchable::getTagStrings() // auxillary to avoid code duplication
{
    std::string totalString = "[";
    for(auto tag : tags)
    {
        totalString += tag;
    }
    return totalString+"]";
}

Movie::Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags)
: Watchable(id, length, tags), name(name)
{}

Watchable* Movie::getNextWatchable(Session& s) const
{
  return nullptr;
}

std::string Movie::toString() const
{
    return name+" "+length+" minutes "+getTagsString(); // According to the "Inglorious Basterds 153 minutes [War, Western]" format
}

Episode::Episode(long id, std::string& seriesName, int length, int season, int episode, const std::vector<std::string>& tags)
: Watchable(id, length, tags), seriesName(seriesName), season(season), episode(episode), nextEpisodeId(id+1) 
{}

std::string Episode::toString() const
{
    return name+" S"+season+"E"+episode+" "+length+" minutes "+getTagsString(); // According to the "Game of Thrones S01E02 56 minutes [Fantasy, Drama]" format
}

Watchable* Episode::getNextWatchable(Session& s) const
{
    if(nextEpisodeId == 0) //If there isn't a next episode return nullptr
    {
        return nullptr;
    }
    std::vector<Watchable*>& content = s.getContent(); //If there is a next episode return it
    return content[nextEpisodeId];
}

Episode::Episode(long id, const std::string& seriesName,int length, int season, int episode, bool isLastEpisode, const std::vector<std::string>& tags)
: Episode(id, seriesName, length, season, episode, tags)
{
    if(isLastEpisode)
        nextEpisodeId = 0;
}
