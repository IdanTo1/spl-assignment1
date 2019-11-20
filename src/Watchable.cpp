#include "../include/Watchable.h"
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

Watchable* Movie::getNextWatchable(Session& s)
{
    User& activeUser = s.getActiveUser();
    return activeUser.getRecommendation(s);
}

std::string Movie::toString() const
{
    return name+" "+length+" minutes "+getTagsString(); // According to the "Inglorious Basterds 153 minutes [War, Western]" format
}

Episode::Episode(long id, std::string& seriesName, int length, int season, int episode, const std::vector<std::string>& tags)
: Watchable(id, length, tags), seriesName(seriesName), season(season), episode(episode), nextEpisodeId(id+1) {}

std::string Episode::toString()
{
    return name+" S"+season+"E"+episode+" "+length+" minutes "+getTagsString(); // According to the "Game of Thrones S01E02 56 minutes [Fantasy, Drama]" format
}

Watchable* Episode::getNextWatchable(Session& s)
{
    if(nextEpisodeId == 0) //If there isn't a next episode return the user's recommendation
    {
        User& activeUser = s.getActiveUser();
        return activeUser.getRecommendation(s);
    }
    std::vector<Watchable*>& content = s.getContent(); //If there is a next episode return it
    return content[nextEpisodeId];
}
