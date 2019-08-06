#pragma once
#include "BusManager.h"
#include <string>
#include <memory>

enum RequestType
{
	Invalid,
	InputStop,
	InputBus,
	OutputBus,
	OutputStop
};
//-----------------------------------------------
class Request
{
public:
	static const RequestType Last_Input_Type = InputBus;
protected:
	RequestType type;
	std::string message;
	int precision;
public:
	virtual void Process(BusManager &storage) {};
	Request(const std::string &str) : type(Invalid), message(str), precision(0) {}
	virtual ~Request() {}
	RequestType GetType() const { return type; }
	const std::string& GetMessage() const { return message; }
	void SetPrecision(int pr) { precision = pr; }
};
//-----------------------------------------------
class InputBus_Request : public Request
{
private:
	std::string bus_number;
	BusInfo info;
public:
	InputBus_Request(const std::string &str, const std::string &number);
	void Process(BusManager &storage) override;
};
//-----------------------------------------------
class InputStop_Request : public Request
{
	std::string stop_name;
	StopInfo info;
public:
	InputStop_Request(const std::string &str, const std::string &_stop_name);
	void Process(BusManager &storage) override;
};
//-----------------------------------------------
class OutputBus_Request : public Request
{
private:
	std::string bus_number;
public:
	OutputBus_Request(const std::string &str, const std::string &number);
	void Process(BusManager &storage) override;
};
//-----------------------------------------------
class OutputStop_Request : public Request
{
private:
	std::string stop_name;
public:
	OutputStop_Request(const std::string &str, const std::string &_stop_name);
	void Process(BusManager &manager) override;
};
//-----------------------------------------------
std::unique_ptr<Request> BuildRequest(std::string str);
// Tests ========================================

#ifdef _DEBUG

void BuildRequest_test();

#endif // _DEBUG
