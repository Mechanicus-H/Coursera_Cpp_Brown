#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <variant>
#include "BusManager.h"
#include "json.h"

//-----------------------------------------------
class Request
{
public:
	enum Type
	{
		Invalid,
		InputStop,
		InputBus,
		OutputBus,
		OutputStop
	};
public:
	static const Type Last_Input_Type = InputBus;
protected:
	Type type = Invalid;
	std::string name; // Имя маршрута/остановки
	int id; // id запроса
public:
	Request(std::map<std::string, Json::Node> params);
	virtual void Process(BusManager &manager) = 0;
	virtual Json::Node GetResponce() = 0;
	virtual ~Request() {}
	Type GetType() const { return type; }
};
//-----------------------------------------------
class InputBus_Request : public Request
{
private:
	BusInfo info;
public:
	InputBus_Request(const std::map<std::string, Json::Node> & params_map);
	void Process(BusManager &storage) override;
	Json::Node GetResponce() override
	{
		throw std::bad_variant_access();
	}
};
//-----------------------------------------------
class InputStop_Request : public Request
{
private:
	StopInfo info;
public:
	~InputStop_Request() {}
	InputStop_Request(const std::map<std::string, Json::Node> & params_map);
	void Process(BusManager &storage) override;
	Json::Node GetResponce() override
	{
		throw std::bad_variant_access();
	}
};
//===============================================
class OutputBus_Request : public Request
{
protected:
	std::map<std::string, Json::Node> responce_map;
public:
	OutputBus_Request(const std::map<std::string, Json::Node> params);
	void Process(BusManager &storage) override;
	Json::Node GetResponce() override
	{
		return Json::Node(move(responce_map));
	}
};
//-----------------------------------------------
class OutputStop_Request : public Request
{
protected:
	std::map<std::string, Json::Node> responce_map;
public:
	OutputStop_Request(const std::map<std::string, Json::Node> params);
	void Process(BusManager &manager) override;
	Json::Node GetResponce() override
	{
		return Json::Node(move(responce_map));
	}
};
//-----------------------------------------------
std::unique_ptr<Request> BuildRequest(const std::map<std::string, Json::Node> &params);
// Tests ========================================
#ifdef _DEBUG

void BuildRequest_test();

#endif // _DEBUG
