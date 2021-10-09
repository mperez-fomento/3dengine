#include "basix.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "engine3d.hpp"

int main() {

    Window win{};
    ImGui::SFML::Init(win);
    win.resetGLStates();
    Clock deltaClock;

    e3d::Mesh cube {{-1, 1, 1},
                    {-1, 1, -1},
                    {-1, -1, -1},
                    {-1, -1, 1},
                    {1, 1, 1},
                    {1, 1, -1},
                    {1, -1, -1},
                    {1, -1, 1}};

    while (win.isOpen()) {
        Event event;
        while (win.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == Event::Closed) {
                win.close();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Q) {
                    win.close();
                }
            }
        }

        ImGui::SFML::Update(win, deltaClock.restart());

        win.clear(Color::Black);
        ImGui::SFML::Render(win);
        win.display();
    }
    ImGui::SFML::Shutdown();
}
