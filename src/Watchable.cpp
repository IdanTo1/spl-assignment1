#include "../include/Session.h"


Watchable::Watchable(long id, int length,
                     const std::vector<std::string>& tags)
: id(id), length(length), tags(tags){}

// auxillary to avoid code duplication
std::string Watchable::getTagStrings() const 
{
    std::string totalString = "[";
    for(auto tag : tags)
    {
        totalString += tag;
    }
    return totalString+"]";
}

Movie::Movie(long id, const std::string& name, int length,
             const std::vector<std::string>& tags)
: Watchable(id, length, tags), name(name)
{}

Watchable* Movie::getNextWatchable(Session& s) const
{
  return nullptr;
}

std::string Movie::toString() const
{
    // According to the "Inglorious Basterds 153 minutes [War, Western]" format
    return name+" "+length+" minutes "+getTagsString();
}

Episode::Episode(long id, std::string& seriesName, int length, int season,
                 int episode, const std::vector<std::string>& tags)
: Watchable(id, length, tags), seriesName(seriesName), season(season),
     episode(episode), nextEpisodeId(id+1) 
{}

std::string Episode::toString() const
{
    // According to the "Game of Thrones S01E02 56 minutes [Fantasy, Drama]" format
    return name+" S"+season+"E"+episode+" "+length+" minutes "+getTagsString();
}

Watchable* Episode::getNextWatchable(Session& s) const
{
    if(nextEpisodeId == 0) //If there isn't a next episode return nullptr
    {
        return nullptr;
    }
    //If there is a next episode return it
    std::vector<Watchable*>& content = s.getContent();
    return content[nextEpisodeId];
}

Episode::Episode(long id, const std::string& seriesName,int length, int season,
                 int episode, int nextEpisodeId, const std::vector<std::string>& tags)
: Episode(id, seriesName, length, season, episode, tags), nextEpisodeId(nextEpisodeId)
{}

Watchable* Episode::clone()
{
    return new Episode(*this);
}

Watchable* Movie::clone()
{
    return new Movie(*this);
}
