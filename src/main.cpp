#include "basix.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "engine3d.hpp"

int main() {

    Window win{};
    ImGui::SFML::Init(win);
    win.resetGLStates();
    Clock deltaClock;
    Time elapsedTime;
    double rotation = 0.0;

    e3d::Poly cube;
    cube.triangles.push_back({{-1, -1, 1}, {1, -1, -1}, {1, -1, 1}});
    cube.triangles.push_back({{-1, -1, 1}, {-1, -1, -1}, {1, -1, -1}});
    cube.triangles.push_back({{-1, 1, 1}, {1, 1, 1}, {1, 1, -1}});
    cube.triangles.push_back({{-1, 1, 1}, {1, 1, -1}, {-1, 1, -1}});
    cube.triangles.push_back({{-1, -1, 1}, {1, -1, 1}, {1, 1, 1}});
    cube.triangles.push_back({{-1, -1, 1}, {1, 1, 1}, {-1, 1, 1}});
    cube.triangles.push_back({{-1, -1, -1}, {1, 1, -1}, {1, -1, -1}});
    cube.triangles.push_back({{-1, -1, -1}, {-1, 1, -1}, {1, 1, -1}});
    cube.triangles.push_back({{-1, -1, -1}, {-1, 1, 1}, {-1, 1, -1}});
    cube.triangles.push_back({{-1, -1, -1}, {-1, -1, 1}, {-1, 1, 1}});
    cube.triangles.push_back({{1, -1, 1}, {1, -1, -1}, {1, 1, -1}});
    cube.triangles.push_back({{1, -1, 1}, {1, 1, -1}, {1, 1, 1}});

    cube.move(0, 0, 1);
    cube.rotate(0, 0, 0);

    double x = 1, y = 1, z = 1;
    matrix::Matrix4x4 obj2world;

    e3d::Device dev {win, e3d::Camera(1, 10, 0.01f, 100.0f, 89.0f)};
    dev.camera.setPosition(0, 0, 0);
    cout << dev.camera.projectionMatrix() << "\n";

    while (win.isOpen()) {
        Event event;
        while (win.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == Event::Closed) {
                win.close();
            }
            if (event.type == Event::KeyPressed) {
                switch (event.key.code) {
                    case Keyboard::Q:
                        win.close();
                        break;
                    case Keyboard::Left:
                        dev.camera.move(-0.1, 0, 0);
                        break;
                    case Keyboard::Right:
                        dev.camera.move(0.1, 0, 0);
                        break;
                    case Keyboard::Up:
                        dev.camera.move(0, 0, -0.1);
                        break;
                    case Keyboard::Down:
                        dev.camera.move(0, 0, 0.1);
                        break;
                    case Keyboard::A:
                        dev.camera.rotate(0, 0, -0.01);
                        break;
                    case Keyboard::D:
                        dev.camera.rotate(0, 0, 0.01);
                        break;
                    case Keyboard::W:
                        dev.camera.rotate(-0.01, 0, 0);
                        break;
                    case Keyboard::S:
                        dev.camera.rotate(0.01, 0, 0);
                        break;
                }
                cout << dev.camera.rotation << "\n";
            }
        }

        elapsedTime = deltaClock.restart();
        ImGui::SFML::Update(win, elapsedTime);

        rotation += elapsedTime.asSeconds() * 0.2;

        cube.setRotation(0, rotation, 0);

        win.clear(Color::Black);
        ImGui::SFML::Render(win);
        cube.draw(dev);
        win.display();
    }
    ImGui::SFML::Shutdown();
}
