#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <imgui/imgui.h>
#include <nlohmann/json.hpp>

#include "Overlay.h"
#include "MemoryReader.h"
#include "Utils.h"
#include "Renderer.h"
#include "Offsets.h"

#include <memory>
#include "TraitObject.h"

#include <cpr/cpr.h>

#include <sstream>

#include <cctype>
#include <string>
#include <algorithm>

void MainLoop(Overlay&, MemoryReader&, ServerConnector&);
double QPC(bool mode, LARGE_INTEGER(&qpc)[3]);

bool showOverlay = false;

inline std::string trim(const std::string& s)
{
    auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) {return std::isspace(c); });
    auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) {return std::isspace(c); }).base();
    return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

int main() {
    ServerConnector connector = ServerConnector();
    std::string token;
    bool isValidToken = false;

    std::cout << "=== TFT Overlay Companion App === \n" << std::endl;

    while (!isValidToken) {
        std::cout << "Please enter login token: ";
        std::getline(std::cin >> std::ws, token);

        token = trim(token);

        if (token.find(' ') != std::string::npos) {
            std::cout << "Your token should not contain any spaces, please double check" << std::endl;
            continue;
        }

        connector.setToken(token);
        isValidToken = connector.checkToken();

        if (!isValidToken) {
            std::cout << "Invalid token. Please double check and try again." << std::endl;
            std::cout << "Token entered: " << token << std::endl;
            std::cout << std::endl;
        }
    }

    std::cout << "Login token has been validated!\n";

    try {
        Overlay overlay = Overlay();
        MemoryReader reader = MemoryReader();
     
        MainLoop(overlay, reader, connector);
    }
    catch (std::runtime_error exception) {

        std::cout << "Uh oh, Tricksters Glass has crashed: " << "\n";
        std::cout << exception.what() << "\n";
        std::cout << "Try restarting the application and see if the problem goes away. If the app keeps crashing, close it and message the developer on discord at:  " << "\n";
    }
}

void MainLoop(Overlay& overlay, MemoryReader& reader, ServerConnector& connector) {

    bool isHooked = false;
    MemSnapshot snapshot{};
    int SEND_TO_SERVER_INTERVAL_MS = 1000;  

    LARGE_INTEGER timeMeasurements[3];
    QPC(true, timeMeasurements);

    std::cout << "Waiting for TFT game..." << std::endl;

    while (true) {
        try {
            if (!isHooked) {
                reader.HookToProcess();
                isHooked = true;
                snapshot = MemSnapshot();
                std::cout << "Hooked into TFT!\n";

                if (showOverlay) {
                    overlay.Init();
                }

            }
            else {
                if (!reader.IsHookedToProcess()) {
                    isHooked = false;
                    std::cout << "TFT game has closed.\n";
                    std::cout << "Waiting for TFT game...\n";


                    if (showOverlay) {
                        overlay.CleanUp();
                    }

                }
                else {
                    if (QPC(false, timeMeasurements) > SEND_TO_SERVER_INTERVAL_MS) {

                        reader.MakeSnapshot(snapshot);
                        if (reader.gameStartedFlag) {
                            nlohmann::json msJson = snapshot;
                            //std::cout << msJson.dump(4) << std::endl;

                            if (showOverlay) {
                                overlay.StartFrame();
                                overlay.DrawMinions(snapshot);
                                overlay.RenderFrame();
                            }

                            if (connector.isServerRunning) {
                                std::thread sendDataThread{ &ServerConnector::sendDataToServer, &connector, msJson.dump() };
                                sendDataThread.detach();

                            }
                            else {
                                connector.checkIfServerRunning();
                            }
                        }
                        QPC(true, timeMeasurements);
                    }
                }
            }
            
        }
        catch (WinApiException exception) {
            //std::cout << exception.what() << "\n";
            isHooked = false;
            //std::cout << "Sleeping for 10 seconds, maybe do an exponential increase" << "\n";
            Sleep(10000);
        }
        catch (std::runtime_error exception) {
            //std::cout << exception.what() << "\n";
        }
    }
}

double QPC(bool mode, LARGE_INTEGER(&qpc)[3]) {
    if (mode) {
        QueryPerformanceFrequency(&qpc[0]); //freq
        QueryPerformanceCounter(&qpc[1]); //start
    }
    else if (!mode) {
        QueryPerformanceCounter(&qpc[2]); //stop
        LONGLONG diff = qpc[2].QuadPart - qpc[1].QuadPart;
        double duration = (double)diff * 1000 / (double)qpc[0].QuadPart;

        return duration;
    }

    return 0;
}


