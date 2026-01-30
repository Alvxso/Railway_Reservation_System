#pragma once

#include <string>
#include <iostream>

class User
{
private:
	std::string Flogin;
	std::string Fpassword;
	int Fid;
protected:
	User(std::string login, std::string password, int id) : Flogin(login), Fpassword(password), Fid(id) {}
public:
	virtual ~User() = default;

	virtual void showMenu() = 0;

	virtual std::string getType() const = 0;

	std::string get_Flogin() const {return Flogin;}
	std::string get_Fpassword() const { return Fpassword; }
	int get_Fid() const { return Fid; }
};

