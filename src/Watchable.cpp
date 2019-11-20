#include "../include/Watchable.h"



Watchable::Watchable(long id, int length, const std::vector<std::string>& tags)
: id(id), length(length), tags(tags){}

std::string Watchable::getTagStrings()
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

Watchable* Movie::getNextWatchable(Session& s)
{
    return nullptr;
}

std::string Movie::toString() const
{
    return name+" "+length+" minutes "+getTagsString();
}

Episode::Episode(long id, std::string& seriesName, int length, int season, int episode, const std::vector<std::string>& tags)
: Watchable(id, length, tags), seriesName(seriesName), season(season), episode(episode), nextEpisodeId(id+1) {}

std::string Episode::toString()
{
    return name+" S"+season+"E"+episode+" "+length+" minutes "+getTagsString();
}

Watchable* Episode::getNextWatchable(Session& s)
{
    if(nextEpisodeId == 0) return nullptr;
    std::vector<Watchable*>& content = s.getContent();
    return content[nextEpisodeId];
}
