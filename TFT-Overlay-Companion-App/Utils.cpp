#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cpr/cpr.h>

#include <stdexcept>

#include "Utils.h"

bool ServerConnector::checkIfServerRunning() {
	cpr::Url healthUrl{ baseUrl + healthEndpoint };
	cpr::Response r = cpr::Get(healthUrl);

	isServerRunning = r.status_code == 200;
	return isServerRunning;
}

void ServerConnector::setToken(std::string token) {
	loginToken = token;
}

bool ServerConnector::checkToken() {
	cpr::Url tokenCheckUrl{ baseUrl + loginCheck };
	cpr::Response r = cpr::Get(tokenCheckUrl, 
						cpr::Header{{"Authorization", loginToken}}
					);

	// should differentiate between 500 and 400, maybe
	if (r.status_code == 0) {
		setToken("");
		std::cout << "Server is currently down, please try again later" << std::endl;
		return false;
	}

	if (r.status_code == 429) {
		setToken("");
		std::cout << "Too many login attempts, try again in an hour. Error: " << r.status_code << std::endl;
		return false;
	}

	if (r.status_code != 200) {
		setToken("");
		std::cout << "Error: " << r.status_code << std::endl;
		return false;
	}
		
	size_t firstDotPosition = loginToken.find('.');

	if (firstDotPosition == std::string::npos) {
		return false;
	}

	channelName = loginToken.substr(0, firstDotPosition);
	return true;
}

void ServerConnector::sendDataToServer(std::string data) {
	cpr::Payload payload{ { "data", data} }; 

	cpr::Url postUrl{ baseUrl + postEndpoint + channelName };

	auto future = cpr::PostAsync(
		postUrl, 
		cpr::Body{data},
		cpr::Header{ {"Content-Type", "application/json"}
					,{"Authorization", loginToken}
		}
	);

	future.wait();
	auto response = future.get();

	if (response.status_code != 200) {
		isServerRunning = false;
		std::cout << "Error: " << response.status_code << std::endl;
	}
	else {
		std::cout << "Request Successful" << std::endl;
	}
}

DWORD64 Memory::ReadAddress(HANDLE hProcess, DWORD64 addr, std::string name) {
	if (addr == 0) return 0;

	DWORD64 ptr{};
	SIZE_T bytesRead{ 0 };

	if (!ReadProcessMemory(hProcess, (LPCVOID)addr, &ptr, sizeof(DWORD64), &bytesRead) || bytesRead <= 0) {
		std::cout << "Address ReadProcessMemory failed for: " << name << "\n";
		std::cout << "Tried to read address: " << std::hex << addr << std::dec << "\n";
		std::cout << "GetLastError: " << std::dec << GetLastError() << "\n";
	}

	return ptr;
}

void Memory::ReadStructure(HANDLE hProcess, DWORD64 addr, void* structure, int size, std::string name) {
	SIZE_T bytesRead{ 0 };

	if (!ReadProcessMemory(hProcess, (LPCVOID)addr, structure, size, &bytesRead) || bytesRead <= 0) {
		std::cout << "Structure ReadProcessMemory failed for: " << name << "\n";
		std::cout << "GetLastError: " << std::dec << GetLastError() << "\n";
	}
}

std::string Memory::ReadString(HANDLE hProcess, DWORD64 addr, unsigned int length) {
	std::string str;
	char buffer[128];
	Memory::ReadStructure(hProcess, addr, buffer, 128, "ReadStringBuffer");

	str.resize(length);
	memcpy(&str[0], &buffer[0], length);

	return str;
}

int Board::GetRow(float y) {
	if (y <= 14000 && y >= 11000) {
		//std::cout << "Player is in the top row\n";
		return 3;
	}
	else if (y <= 9000 && y >= 6000) {
		//std::cout << "Player is in the middle row\n";
		return 2;
	}
	else if (y <= 4000 && y >= 1000) {
		//std::cout << "Player is in the bottom row\n";
		return 1;
	}
	else {
		//std::cout << "Player is traveling btwn boards\n";
		return 0;
	}
}

int Board::GetColumn(float x) {
	if (x <= 14000 && x >= 11000) {
		//std::cout << "Player is in the right col\n";
		return 3;
	}
	else if (x <= 9000 && x >= 6000) {
		//std::cout << "Player is in the middle col\n";
		return 2;
	}
	else if (x <= 4000 && x >= 1000) {
		//std::cout << "Player is in the left row\n";
		return 1;
	}
	else {
		//std::cout << "Player is traveling btwn boards\n";
		return 0;
	}
}

int Board::GetSquare(int row, int col) {
	if (!row || !col) return 0;

	int square = (3 * (row - 1)) + col;
	return square;
}
