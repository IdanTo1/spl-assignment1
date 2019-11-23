#include "../include/Session.h"

BaseAction::BaseAction() : errorMsg(""), status(PENDING)
{}

ActionStatus BaseAction::getStatus() const
{
    return status;
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
    if(sess.getUsers()[name] != nullptr)
    {

    }
}


//clones

virtual BaseAction* CreateUser::clone()
{
    return new CreateUser(*this);
}

virtual BaseAction* ChangeActiveUser::clone()
{
    return new ChangeActiveUser(*this);
}

virtual BaseAction* DeleteUser::clone()
{
    return new DeleteUser(*this);
}

virtual BaseAction* DeleteUser::clone()
{
    return new DeleteUser(*this);
}

virtual BaseAction* DuplicateUser::clone()
{
    return new DuplicateUser(*this);
}

virtual BaseAction* PrintContentList::clone()
{
    return new PrintContentList(*this);
}

virtual BaseAction* PrintWatchHistory::clone()
{
    return new PrintWatchHistory(*this);
}

virtual BaseAction* Watch::clone()
{
    return new Watch(*this);
}

virtual BaseAction* PrintActionsLog::clone()
{
    return new PrintActionsLog(*this);
}

virtual BaseAction* Exit::clone()
{
    return new Exit(*this);
}
