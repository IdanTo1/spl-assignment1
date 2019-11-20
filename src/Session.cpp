#include "../include/Session.h"

const User& Session::getActiveUser()
{
    return activeUser;
}

const std::vector<Watchable*>& Session::getContent()
{
    return content;
}