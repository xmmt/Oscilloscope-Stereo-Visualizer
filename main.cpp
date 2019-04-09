#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <vector>
#include <cmath>
#include <algorithm>

#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>
#include <queue>
#include <thread>
#include <chrono>

using namespace std;

vector < string > getCaptureDevices()
{
    char *list = (char *)alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
    vector < string > CapDevices;
    ALCchar *ptr, *nptr;
    ptr = (ALCchar *)list;
    nptr = ptr;
    while (*(nptr += strlen(ptr)+1) != 0)
    {
        CapDevices.emplace_back(ptr);
        ptr = nptr;
    }
    CapDevices.emplace_back(ptr);
    return CapDevices;
}

char capdata[200000];

int main() {

    ALCdevice *dev;
    ALCcontext *ctx;
    ALuint buf;
    ALint val;

    vector < string > CapDevices = getCaptureDevices();

    alDistanceModel(AL_NONE);

    bool flag = true;
    for (auto & i : CapDevices) {
        if (i.substr(0, 10) == "Monitor of") {
            dev = alcCaptureOpenDevice(i.c_str(), 48000, AL_FORMAT_STEREO16, 10000);
            flag = false;
            break;
        }
    }
    if (flag)
        return 0;

    alcCaptureStart(dev);

    sf::RenderWindow window(sf::VideoMode(600, 600), L"Осциллограф");
    window.setFramerateLimit(60);
    //ImGui::SFML::Init(window);

    deque < sf::CircleShape > qarr;

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        alcGetIntegerv(dev, ALC_CAPTURE_SAMPLES, 1, &val);
        if (val < 3000) {
            this_thread::sleep_for(5ms);
            continue;
        }
        alcCaptureSamples(dev, capdata, val);

        //ImGui::SFML::Update(window, deltaClock.restart());

        //ImGui::ShowTestWindow();

        //ImGui::Begin("Hello, world!");
        //ImGui::Button("Look at this pretty button");
        //ImGui::End();

        for (int i = 0; i < val; ++i) {
            qarr.emplace_back(1, 3);
            qarr.back().setPosition(float(double(int(capdata[4 * i + 1]) * 256 + int(capdata[4 * i])) * 600.0 / 65536.0 + 300.0), float(-double(int(capdata[4 * i + 3]) * 256 + int(capdata[4 * i + 2])) * 600.0 / 65536.0 + 300.0));
            qarr.back().setFillColor(sf::Color::White);
            if (qarr.size() > 5000)
                qarr.pop_front();
        }

//        ImGui::SFML::Update(window, deltaClock.restart());
//
//        ImGui::ShowTestWindow();
//
//        ImGui::Begin("Hello, world!");
//        ImGui::Button("Look at this pretty button");
//        ImGui::End();

        window.clear();
        for (auto & i : qarr)
            window.draw(i);
        //ImGui::SFML::Render(window);
        window.display();
    }

    alcCaptureStop(dev);
    alcCaptureCloseDevice(dev);

    //ImGui::SFML::Shutdown();

    return 0;
}