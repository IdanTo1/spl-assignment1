#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;

class Watchable {
    public:
        Watchable(long id, int length, const std::vector <std::string>& tags);
        virtual ~Watchable();
        int getLength() const;
        int getId() const;
        virtual Watchable* getNextWatchable(Session&) const = 0;
        virtual std::string toString() const = 0;
        virtual std::string toStringName() const = 0;
        const std::string getTagsString() const;
        const std::vector <std::string>& getTags() const;
        virtual Watchable* clone() const = 0;
    private:
        const long id;
        int length;
        std::vector <std::string> tags;
};

class Movie : public Watchable {
    public:
        Movie(long id, const std::string& name, int length, const std::vector <std::string>& tags);
        ~Movie();
        virtual Watchable* getNextWatchable(Session&) const;
        virtual std::string toString() const;
        virtual std::string toStringName() const;
        virtual Watchable* clone() const;
    private:
        std::string name;
};


class Episode : public Watchable {
    public:
        Episode(long id, const std::string& seriesName, int length, int season, int episode,
                const std::vector <std::string>& tags);
        Episode(long id, const std::string& seriesName, int length, int season, int episode,
                int nextEpisodeId, const std::vector <std::string>& tags);
        ~Episode();
        virtual Watchable* getNextWatchable(Session&) const;
        virtual std::string toString() const;
        virtual std::string toStringName() const;
        virtual Watchable* clone() const;
    private:
        std::string seriesName;
        int season;
        int episode;
        long nextEpisodeId;
};

#endif
