#include "../include/Session.h"

BaseAction::BaseAction() : errorMsg(""), status(PENDING) {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

const std::string BaseAction::getStatusString() const {
    switch (status) {
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

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(const std::string& errorMsg) {
    this->errorMsg = errorMsg;
    status = ERROR;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

CreateUser::CreateUser(const std::string& name, const std::string& recommendationAlg): BaseAction(),
                                                                                        _userName(name),
                                                                    _recommendationAlg(recommendationAlg) {}

void CreateUser::act(Session& sess) {
    if(sess.getUsers().count(_userName)) {
        error(USER_EXISTS_ERR);
        return;
    }
    if (_recommendationAlg == "len") {
        sess.addToUserMap(new LengthRecommenderUser(_userName));
    }
    else if (_recommendationAlg == "rer") {
        sess.addToUserMap(new RerunRecommenderUser(_userName));
    }
    else if (_recommendationAlg == "gen") {
        sess.addToUserMap(new GenreRecommenderUser(_userName));
    }
    else {
        error(INVALID_ALG_ERR);
        return;
    }
    complete();
}
std::string CreateUser::toString() const {
    return ("createuser" + getStatusString()); //TODO complete due to forum answer
}

ChangeActiveUser::ChangeActiveUser(const std::string& name): BaseAction(), _newUser(name) {}

void ChangeActiveUser::act(Session &sess) {
    const std::unordered_map<std::string,User*>& usersMap = sess.getUsers();
    if(!usersMap.count(_newUser)) {
        error(USER_DOESNT_EXISTS_ERR);
    }
    else {
        sess.setActiveUser(*(usersMap.at(_newUser)));
        complete();
    }
}

std::string ChangeActiveUser::toString() const {
    return ("changeuser" + getStatusString()); //TODO
}


DeleteUser::DeleteUser(const std::string& name): _userName(name) {}

void DeleteUser::act(Session &sess) {
    if(!sess.getUsers().count(_userName)) {
        error(USER_DOESNT_EXISTS_ERR);
    }
    else {
        sess.deleteUserFromMap(_userName);
        complete();
    }
}

std::string DeleteUser::toString() const {
    return ("deleteuser" + getStatusString()); // TODO
}


DuplicateUser::DuplicateUser(const std::string& newUser, const std::string& oldUser): _newUserName(newUser),
                                                                                      _oldUserName(oldUser){}

void DuplicateUser::act(Session &sess) {
    const std::unordered_map<std::string,User*>& usersMap = sess.getUsers();
    if(!usersMap.count(_oldUserName)) {
        error(USER_DOESNT_EXISTS_ERR);
    }
    else if (usersMap.count(_newUserName)) {
        error(USER_EXISTS_ERR);
    }
    else {
        User& oldUser = *(usersMap.at(_oldUserName));
        User* newUser = oldUser.clone();
        sess.addToUserMap(newUser);
    }
}

std::string DuplicateUser::toString() const {
    return ("duplicateuser" + getStatusString()); // TODO
}


void Exit::act(Session& sess) {
    complete();
}

//TODO check if this is the right printing format
std::string Exit::toString() const {
    return "Exit "+getStatusString();
}

PrintActionsLog::PrintActionsLog(const std::vector<BaseAction*>& actionsLog): actionsLog(actionsLog) {}

void PrintActionsLog::act(Session& s) {
    for (auto i = actionsLog.rbegin(); i != actionsLog.rend(); ++i ) {
            std::cout << (*i)->toString() + "\n";
    }
    complete();
}

std::string PrintActionsLog::toString() const {
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
