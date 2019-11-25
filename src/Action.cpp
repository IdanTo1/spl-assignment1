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

void Exit::act(Session& sess)
{
    complete();
}

std::string Exit::toString() const
{
    return "Exit "+getStatusString();
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

BaseAction* CreateUser::clone()
{
    return new CreateUser(*this);
}

BaseAction* ChangeActiveUser::clone()
{
    return new ChangeActiveUser(*this);
}

BaseAction* DeleteUser::clone()
{
    return new DeleteUser(*this);
}

BaseAction* DuplicateUser::clone()
{
    return new DuplicateUser(*this);
}

BaseAction* PrintContentList::clone()
{
    return new PrintContentList(*this);
}

BaseAction* PrintWatchHistory::clone()
{
    return new PrintWatchHistory(*this);
}

BaseAction* Watch::clone()
{
    return new Watch(*this);
}

BaseAction* PrintActionsLog::clone()
{
    return new PrintActionsLog(*this);
}

BaseAction* Exit::clone()
{
    return new Exit(*this);
}
