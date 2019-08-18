#pragma once
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <memory>
#include "json.h"
#include "request.h"

class BusManager;

class RequestManager
{
private:
	std::istream &input;
	std::ostream &output;

	BusManager *bus_manager;
	Json::Document document;
	Json::Document responce_doc;

	int precision;
public:
	RequestManager(std::istream &_in, std::ostream &_out)
		: input(_in), output(_out), bus_manager(nullptr), precision(0)
	{}
	void ReadJsonRequests(); // Читаем сначала запросы на добавление, затем на вывод
	void ProcessRequests(); // Обрабатываем запросы
	void PrintRecponces() const;
	void SetBusManager(BusManager *_bus_manager) { bus_manager = _bus_manager; }
	void SetPrecision(int pr) { precision = pr; }
};
// tests ========================================
#ifdef _DEBUG

void Case1();

#endif // _DEBUG
