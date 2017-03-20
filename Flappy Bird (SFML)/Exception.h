#pragma once
#include <exception>
#include <string>
#include <iostream>

class FileNotFound : std::exception
{
public:
	std::string filepath;
	FileNotFound(std::string file) { filepath = file; }
};

class MapNotLoaded : std::exception
{
public:
	MapNotLoaded() { }
};