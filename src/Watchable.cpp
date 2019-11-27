#include "../include/Session.h"


Watchable::Watchable(long id, int length,
                     const std::vector<std::string>& tags)
: id(id), length(length), tags(tags){}

// auxillary to avoid code duplication
const std::string Watchable::getTagsString() const
{
    std::string totalString = "[";
    for(auto tag = tags.begin(); tag!=tags.end(); tag++)
    {
        if(tag+1 == tags.end())
            totalString = *tag;
        totalString += (*tag)+", ";
    }
    return totalString+"]";
}

const std::vector<std::string>& Watchable::getTags() const
{
    return this->tags;
}

int Watchable::getLength() const
{
    return length;
}

int Watchable::getId() const
{
    return id;
}

Movie::Movie(long id, const std::string& name, int length,
             const std::vector<std::string>& tags)
: Watchable(id, length, tags), name(name)
{}

Watchable* Movie::getNextWatchable(Session& s) const
{
  return nullptr;
}

std::string Movie::toStringName() const
{
    // According to the "Inglorious Basterds 153 minutes" format
    return name+" ";
}

std::string Movie::toString() const {
    return toStringName() + std::to_string(getLength())+" minutes " + getTagsString();
}

/* A second constructor for Episode is implemented with an extra paramter.
    The latter is stronger and can be called here instead of
    implementing the entire thing again
*/
Episode::Episode(long id, const std::string& seriesName, int length, int season,
                 int episode, const std::vector<std::string>& tags):
                 Episode(id, seriesName, length, season, episode, id+1, tags)
{}

std::string Episode::toStringName() const
{
    // According to the "Game of Thrones S01E02 56 minutes" format
    return seriesName+" S"+std::to_string(season)+"E"+std::to_string(episode);
}

std::string Episode::toString() const {
    return toStringName() + " "+std::to_string(getLength())+" minutes " + getTagsString();
}

Watchable* Episode::getNextWatchable(Session& s) const
{
    if(nextEpisodeId == 0) //If there isn't a next episode return nullptr
    {
        return nullptr;
    }
    //If there is a next episode return it
    const std::vector<Watchable*>& content = s.getContent();
    return content[nextEpisodeId];
}

Episode::Episode(long id, const std::string& seriesName,int length, int season,
                 int episode, int nextEpisodeId, const std::vector<std::string>& tags)
:  Watchable(id, length, tags), seriesName(seriesName), season(season),
     episode(episode), nextEpisodeId(nextEpisodeId)
{}

Watchable* Episode::clone() const
{
    return new Episode(*this);
}

Watchable* Movie::clone() const
{
    return new Movie(*this);
}
