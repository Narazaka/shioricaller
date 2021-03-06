#include <iostream>
#include <string>
#include <cstdlib>
#include "shioricaller.hpp"

int main(int argc, char* argv[]){
	if(argc != 3){
		std::cout << "Usage: shioricaller shiori.dll dirpath < request.txt > response.txt" << std::endl;
		return 0;
	}
	auto dll = argv[1];
	auto dirpath = argv[2];

	auto shioriCaller = new ShioriCaller(dll);
	if (shioriCaller->load_error != nullptr) {
		if (shioriCaller->load_error == "LoadLibrary") {
			std::cout << "cannot load dll [" << dll << "] code [" << GetLastError() << "]" << std::endl;
		} else {
			std::cout << "cannot get functions [" << shioriCaller->load_error << "] in [" << dll << "]" << std::endl;
		}
		return 1;
	}

	if(!shioriCaller->load(dirpath)){
		std::cout << "load returns non 1 [" << dll << "]" << std::endl;
		return 1;
	}

	std::string req;
	std::string req_line;
	while(true){
		std::getline(std::cin, req_line);
		if (std::cin.eof()) break;
		req += req_line + "\x0d\x0a";
		if (req_line.empty()) {
			auto res = shioriCaller->request(req.c_str());
			std::cout << res;
			free(res);
			req = "";
		}
	}

	if(!shioriCaller->unload()){
		std::cout << "unload returns non 1 [" << dll << "]" << std::endl;
		return 1;
	}

	return 0;
}
