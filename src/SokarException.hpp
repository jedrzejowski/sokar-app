//
// Created by adam on 20.05.2021.
//

#pragma once

#include <string>

class SokarException : std::exception {
private:
	std::string msg;
	std::string file;
	std::string where;
	int line;

public:

	SokarException(const char *file, int line, const char *where, std::string &msg) :
			where(where), file(file), line(line), msg(msg) {
	};

	SokarException(const char *file, int line, const char *where, const char *msg) :
			where(where), file(file), line(line), msg(msg) {
	};

	SokarException(const char *file, int line, const char *where, char *msg) :
			where(where), file(file), line(line), msg(msg) {
	};

	[[nodiscard]] const std::string &getFile() const {
		return file;
	}

	[[nodiscard]] const int &getLine() const {
		return line;
	}

	[[nodiscard]] const char *what() const noexcept override {
		return msg.c_str();
	};
};


#define sokarException(where, what) SokarException(__FILE__, __LINE__, where, what)
