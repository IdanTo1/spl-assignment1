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
