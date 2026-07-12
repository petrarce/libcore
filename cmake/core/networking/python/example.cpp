//
// Created by ivan on 7/4/26.
//

#include <charconv>
#include <string>
#include <iostream>
#include <ostream>
#include <fmt/format.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <core/networking/SocDatagram.h>

#define LOC(msg) fmt::format("[{}:{}:{}] {}", __FILE_NAME__, __LINE__, __PRETTY_FUNCTION__, msg)

namespace py = pybind11;
using namespace pybind11::literals;
using namespace net;
void print_hello(const std::string& message)
{

	std::cout << fmt::format("Print from example python module: {}", message) << std::endl;
	std::flush(std::cout);
}

class MyClass
{

public:
	MyClass() = default;
	MyClass(const std::string& msgToPrint)
		: msg(std::move(msgToPrint))
	{
	}
	void print() { std::cout << LOC(fmt::format("{}", msg)) << std::endl; }
	// static void print(const std::string& msg)
	// {
	// 	std::cout << LOC(fmt::format("{}", msg)) << std::endl;
	// }

private:
	std::string msg;
};

PYBIND11_MODULE(example_python, m)
{
	m.doc() = "pybind11 example plugin"; // optional module docstring

	m.def("print_hello", &print_hello, "A function that print somethingq",
		  py::arg("message") = "Example message as default arg");
	m.def(
		"print_hello_l",
		[](const std::string& msg)
		{
			std::cout << fmt::format("Print from example python module lmbda: {} {} {} {}", msg,
									 __FUNCTION__, __LINE__, __FILE_NAME__)
					  << std::endl;
		},
		"Lambda function alternative", "message"_a = "Example message in lambda");
	py::class_<MyClass>(m, "Printer")
		.def(py::init<const std::string&>())
		.def(py::init())
		.def("print", &MyClass::print);
	py::class_<SocDatagram>(m, "SocDatagram")
		.def(py::init<>())
		.def("Listen", &SocDatagram::Listen)
		.def("Connect", &SocDatagram::Connect)
		.def("Read", &SocDatagram::Read)
		.def("Write", &SocDatagram::Write);
}
