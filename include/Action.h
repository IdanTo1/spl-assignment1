#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

class Session;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};
const std::string USER_EXISTS_ERR = "User already exists";
const std::string INVALID_ALG_ERR = "Invalid algorithm was given";
const std::string USER_DOESNT_EXISTS_ERR = "Requested user does not exists";

class BaseAction{
public:
    BaseAction();
    virtual ~BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
    virtual BaseAction* clone() = 0;
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
    virtual void act(Session& sess);
    virtual std::string toString() const;
    virtual BaseAction* clone();
private:
    std::string _userName;
    std::string _recommendationAlg;
};

class ChangeActiveUser : public BaseAction {
public:
    ChangeActiveUser(const std::string& name);
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone();

private:
    std::string _newUser;
};

class DeleteUser : public BaseAction {
public:
    DeleteUser(const std::string& name);
	virtual void act(Session & sess);
	virtual std::string toString() const;
	virtual BaseAction* clone();

private:
    std::string _userName;
};


class DuplicateUser : public BaseAction {
public:
    DuplicateUser(const std::string&, const std::string&);
	virtual void act(Session & sess);
	virtual std::string toString() const;
	virtual BaseAction* clone();

private:
    const std::string _newUserName;
    const std::string _oldUserName;
};

class PrintContentList : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone();
};

class PrintWatchHistory : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone();
};


class Watch : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone();
};


class PrintActionsLog : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone();
	PrintActionsLog(const std::vector<BaseAction*>& actionsLog);
private:
	const std::vector<BaseAction*>& actionsLog;
};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone();
};
#endif
