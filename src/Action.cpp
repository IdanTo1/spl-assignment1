#include "../include/Session.h"

BaseAction::BaseAction() : errorMsg(""), status(PENDING)
{}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

const std::string BaseAction::getStatusString() const
{
    switch (status)
    {
    case PENDING:
       return "PENDING";
    case COMPLETED:
        return "COMPLETED";
    case ERROR:
        return "ERROR: "+getErrorMsg();
    default:
        return "Impossible";
    }
}

void BaseAction::complete()
{
    status = COMPLETED;
}

void BaseAction::error(const std::string& errorMsg)
{
    this->errorMsg = errorMsg;
    status = ERROR;
}

std::string BaseAction::getErrorMsg() const
{
    return errorMsg;
}

CreateUser::CreateUser(const std::string& name,
                         const std::string& recommendationType):
                         name(name), recommendationType(recommendationType)
{}

void CreateUser::act(Session& sess)
{
    const std::unordered_map<std::string, User*>& users = sess.getUsers();
    if(users.find(name) != users.end())
    {

    }
}

void PrintContentList::act(Session& sess) {
    const std::vector<Watchable*>& content = sess.getContent();
    int i = 1; //Because human lists start at 1
    for(auto w: content)
    {
        std::cout << std::to_string(i) << w->toString() << std::endl;
        i++;
    }
}

std::string PrintContentList::toString() const {
    return "PrintContentList " + getStatusString();
}

void PrintWatchHistory::act(Session& sess) {
    const std::vector<Watchable*> history = sess.getActiveUser().get_history();
    int i = 1; //Because human lists start at 1
    for(auto w: history)
    {
        std::cout << std::to_string(i) << w->toString() << std::endl;
        i++;
    }
    complete();
}

std::string PrintWatchHistory::toString() const {
    return "PrintWatchHistory " + getStatusString();
}

Watch::Watch(Watchable& toWatch): BaseAction(), _toWatch(toWatch), _nextWatchableId(-1) {}

void Watch::act(Session& sess) {
    User& activeUser = sess.getActiveUser();
    std::cout << "Watching " << _toWatch.toString();
    activeUser.addToHistory(_toWatch);
    Watchable* nextWatchable = activeUser.getRecommendation(sess);
    if(nextWatchable == nullptr)
        error(NO_RECOMMENDATION_ERR);
    else{
        std::cout << "We recommend watching " << nextWatchable->toString() <<
                                                 ", continue watching? [y/n]";
        _nextWatchableId = nextWatchable->getId();
    }

    complete();
}

int Watch::getNextWatchableId() const
{
    return _nextWatchableId;
}

std::string Watch::toString() const {
    return "Watch "+getStatusString();
}

void Exit::act(Session& sess)
{
    complete();
}

//TODO check if this is the right printing format
std::string Exit::toString() const
{
    return "Exit " + getStatusString();
}

PrintActionsLog::PrintActionsLog(const std::vector<BaseAction*>& actionsLog)
: actionsLog(actionsLog) {}

void PrintActionsLog::act(Session& s)
{
    for (auto i = actionsLog.rbegin(); i != actionsLog.rend(); ++i ) {
            std::cout << (*i)->toString() + "\n";
    }
    complete();
}

std::string PrintActionsLog::toString() const
{
    return "PrintActionsLog "+getStatusString();
}

// Clones for polymorphic copying

BaseAction* CreateUser::clone() const
{
    return new CreateUser(*this);
}

BaseAction* ChangeActiveUser::clone() const
{
    return new ChangeActiveUser(*this);
}

BaseAction* DeleteUser::clone() const
{
    return new DeleteUser(*this);
}

BaseAction* DuplicateUser::clone() const
{
    return new DuplicateUser(*this);
}

BaseAction* PrintContentList::clone() const
{
    return new PrintContentList(*this);
}

BaseAction* PrintWatchHistory::clone() const
{
    return new PrintWatchHistory(*this);
}

BaseAction* Watch::clone() const
{
    return new Watch(*this);
}

BaseAction* PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

BaseAction* Exit::clone() const
{
    return new Exit(*this);
}
