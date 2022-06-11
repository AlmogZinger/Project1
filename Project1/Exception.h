#pragma once
#include <exception>
using namespace std;
class Exception : public exception
{
	const char* emsg;
public:
	const char* what() const noexcept { return emsg; }
	Exception(const char* e) : emsg(e) {}
};