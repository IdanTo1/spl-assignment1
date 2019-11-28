#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Watchable.h"

class Session;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};

const std::string USER_EXISTS_ERR = "User already exists";
const std::string INVALID_ALG_ERR = "Invalid algorithm was given";
const std::string USER_DOESNT_EXISTS_ERR = "Requested user does not exist";

class BaseAction{
public:
    BaseAction();
    virtual ~BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
    virtual BaseAction* clone() const = 0;
protected:
    void complete();
    void error(const std::string& errorMsg);
    std::string getErrorMsg() const;
    const std::string getStatusString() const;
private:
    std::string errorMsg;
    ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
    CreateUser(const std::string& name, const std::string& recommendationAlg);
    ~CreateUser();
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone() const;
private:
    std::string _userName;
    std::string _recommendationAlg;
};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser(const std::string& name);
    ~ChangeActiveUser();
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
private:
    std::string _newUser;
};

class DeleteUser : public BaseAction {
public:
    DeleteUser(const std::string& name);
    ~DeleteUser();
	virtual void act(Session & sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
private:
    std::string _userName;
};


class DuplicateUser : public BaseAction {
public:
    DuplicateUser(const std::string&, const std::string&);
    ~DuplicateUser();
	virtual void act(Session & sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
private:
    const std::string _oldUserName;
    const std::string _newUserName;
};

class PrintContentList : public BaseAction {
public:
    ~PrintContentList();
	virtual void act (Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
};

class PrintWatchHistory : public BaseAction {
public:
    ~PrintWatchHistory();
	virtual void act (Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
};


class Watch : public BaseAction {
public:
	Watch(Watchable& toWatch);
	~Watch();
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
	long getNextWatchableId() const;
private:
	Watchable& _toWatch;
	long _nextWatchableId;
};


class PrintActionsLog : public BaseAction {
public:
    ~PrintActionsLog();
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
	PrintActionsLog(const std::vector<BaseAction*>& actionsLog);
private:
	const std::vector<BaseAction*>& _actionsLog;
};

class Exit : public BaseAction {
public:
    ~Exit();
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
};
#endif
