#include "../include/Session.h"

BaseAction::BaseAction() : errorMsg(""), status(PENDING)
{}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

std::string BaseAction::getStatusString()
{
    if(status == COMPLETED) return "COMPLETED";
    else if(status == PENDING) return "PENDING";
    else return "ERROR: "+getErrorMsg();
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

std::string Exit::toString()
{
    return "Exit "+getStatusString();
}



//clones

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
