#pragma once

#include <Windows.h>
#include <iostream>
#include <string>
#include <set>
#include <stdexcept>
#include <nlohmann/json.hpp>

class WinApiException : public std::runtime_error {

public:
	WinApiException(const char* message)
		:std::runtime_error(message) {

		errorCode = GetLastError();
		this->message = message;
	}

	std::string GetErrorMessage() {
		std::string msg = std::string(message);

		if (errorCode > 0) {
			char winapiError[255];
			FormatMessageA(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				errorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				winapiError,
				(sizeof(winapiError) / sizeof(wchar_t)),
				NULL);

			msg.append("(");
			msg.append(winapiError);
			msg.append(")");
		}

		return msg;
	}

private:
	const char* message = nullptr;
	int            errorCode = 0;
};

class ServerConnector {
public :
	bool checkIfServerRunning();
	void sendDataToServer(std::string data);
	//void loadSettings();
	void setToken(std::string token);
	bool checkToken();

public:
	bool isServerRunning = true;

private:
	std::string baseUrl = "http://localhost:8000/";
	std::string postEndpoint = "pubsub/";
	std::string loginCheck = "login/";
	std::string healthEndpoint = "health/";
	std::string loginToken = "";
	std::string channelName = "";
};

struct Vector2 {
	Vector2() {};
	Vector2(float _x, float _y) {
		x = _x;
		y = _y;
	}

	float x;
	float y;

	friend void to_json(nlohmann::json& j, const Vector2& obj) {
		j = nlohmann::json{
			{"x", obj.x},
			{"y", obj.y}
		};
	}
};

struct Vector3 {
	Vector3() {};
	Vector3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	float x;
	float y;
	float z;

	friend void to_json(nlohmann::json& j, const Vector3& obj) {
		j = nlohmann::json{
			{"x", obj.x},
			{"y", obj.y},
			{"z", obj.z}
		};
	}
};



struct Vector4 {
	Vector4() {};
	Vector4(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	float x;
	float y;
	float z;
	float w;
};

struct BoundingBox {
	BoundingBox() {};
	BoundingBox(Vector2 _corner1, Vector2 _corner2) {
		corner1 = _corner1;
		corner2 = _corner2;
	}

	Vector2 corner1; // top-left corner
	Vector2 corner2; // bottom-right corner

	friend void to_json(nlohmann::json& j, const BoundingBox& obj) {
		j = nlohmann::json{
			{"corner1", obj.corner1},
			{"corner2", obj.corner2},
		};
	}
};

namespace Memory {
	// Read an address at a specific memory address
	DWORD64 ReadAddress(HANDLE hProcess, DWORD64 addr, std::string name);

	// Read an arbitrary structure (int, float, struct, etc) at a specific memory address
	void ReadStructure(HANDLE hProcess, DWORD64 addr, void* structure, int size, std::string name);

	std::string ReadString(HANDLE hProcess, DWORD64 addr, unsigned int length);

}

namespace Board {
	int GetRow(float y);
	int GetColumn(float x);
	int GetSquare(int row, int col);
}

namespace nlohmann
{
	template <typename T>
	struct adl_serializer<std::shared_ptr<T>>
	{
		static void to_json(json& j, const std::shared_ptr<T>& opt)
		{
			if (opt)
			{
				j = *opt;
			}
			else
			{
				j = nullptr;
			}
		}

		static void from_json(const json& j, std::shared_ptr<T>& opt)
		{
			if (j.is_null())
			{
				opt = nullptr;
			}
			else
			{
				opt.reset(new T(j.get<T>()));
			}
		}
	};
}
