#include "../include/Session.h"

BaseAction::BaseAction() : errorMsg(""), status(PENDING) {}

BaseAction::~BaseAction() {}

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

CreateUser::~CreateUser() {}

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
    return ("CreateUser " + getStatusString());
}

ChangeActiveUser::ChangeActiveUser(const std::string& name): BaseAction(), _newUser(name) {}

ChangeActiveUser::~ChangeActiveUser() {}

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
    return ("ChangeUser " + getStatusString());
}

DeleteUser::DeleteUser(const std::string& name): BaseAction(), _userName(name) {}

DeleteUser::~DeleteUser() {}

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
    return ("DeleteUser " + getStatusString());
}

DuplicateUser::DuplicateUser(const std::string& newUser, const std::string& oldUser): BaseAction(),
                                                                                      _newUserName(newUser),
                                                                                      _oldUserName(oldUser){}

DuplicateUser::~DuplicateUser() {}

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
    return ("DuplicateUser " + getStatusString());
}


PrintContentList::~PrintContentList() {}

void PrintContentList::act(Session& sess) {
    const std::vector<Watchable*>& content = sess.getContent();
    for(auto watchable : content)
    {
        // +1 for easier readabillity, where lists start at 1
        std::cout << std::to_string(watchable->getId()+1) << ". " << watchable->toString() << std::endl;
    }
}

std::string PrintContentList::toString() const {
    return "PrintContentList " + getStatusString();
}

PrintWatchHistory::~PrintWatchHistory() {}

void PrintWatchHistory::act(Session& sess) {
    const std::vector<Watchable*> history = sess.getActiveUser().get_history();
    int i = 1; // 1 for easier readabillity, where lists start at 1
    for(auto watchable: history)
    {
        std::cout << std::to_string(i) << ". " << watchable->toStringName() << std::endl;
        i++;
    }
    complete();
}

std::string PrintWatchHistory::toString() const {
    return "PrintWatchHistory " + getStatusString();
}

Watch::Watch(Watchable& toWatch): BaseAction(),
                                 _toWatch(toWatch),
                                  _nextWatchableId(-1) {}

Watch::~Watch() {}

void Watch::act(Session& sess) {
    User& activeUser = sess.getActiveUser();
    std::cout << "Watching " << _toWatch.toString() << std::endl;
    activeUser.addToHistory(_toWatch);
    complete(); //Because this is the end of watching the content
    Watchable* nextWatchable = _toWatch.getNextWatchable(sess);
    /* If the Watchable returns a nullptr as the next in order then we ask
     the user object for recommendation
     */
    if(nextWatchable == nullptr){
        nextWatchable = activeUser.getRecommendation(sess);
    }
    if(nextWatchable == nullptr) {
        return;
    }
    else{
        std::cout << "We recommend watching " << nextWatchable->toString() <<
                                                 ", continue watching? [y/n]" << std::endl;
        _nextWatchableId = nextWatchable->getId();
    }
}

long Watch::getNextWatchableId() const
{
    return _nextWatchableId;
}

std::string Watch::toString() const {
    return "Watch "+getStatusString();
}

PrintActionsLog::PrintActionsLog(const std::vector<BaseAction*>& actionsLog): BaseAction(),
                                                                              _actionsLog(actionsLog) {}

PrintActionsLog::~PrintActionsLog() {}

void PrintActionsLog::act(Session& s) {
    for (auto i = _actionsLog.rbegin(); i != _actionsLog.rend(); ++i ) {
            std::cout << (*i)->toString() << std::endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return "PrintActionsLog "+getStatusString();
}

Exit::~Exit() {}

void Exit::act(Session& sess) {
    complete();
}

std::string Exit::toString() const {
    return "Exit "+getStatusString();
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
