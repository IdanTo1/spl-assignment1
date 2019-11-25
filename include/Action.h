#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>

class Session;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};


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
	CreateUser(const std::string& name, const std::string& recommendationType);
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
private:
	std::string name;
	std::string recommendationType;
};

class ChangeActiveUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
};

class DeleteUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
};


class DuplicateUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
};

class PrintContentList : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
};

class PrintWatchHistory : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
};


class Watch : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
};


class PrintActionsLog : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
	PrintActionsLog(const std::vector<BaseAction*>& actionsLog);
private:
	const std::vector<BaseAction*>& actionsLog;
};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone() const;
};
#endif
