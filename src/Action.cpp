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
    return "Log "+getStatusString();
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
