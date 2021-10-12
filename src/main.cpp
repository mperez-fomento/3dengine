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

    e3d::Mesh* tetrahedron = new e3d::Mesh{{0, 0, 0}, {0, 1, 0},
                                           {0, 1, 0}, {1, 0, 0},
                                           {1, 0, 0}, {0, 0, 0},
                                           {0, 0, 1}, {0, 0, 0},
                                           {0, 0, 1}, {0, 1, 0},
                                           {1, 0, 0}, {0, 0, 1}};

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

    cube.move(1, 0.5, 10);
    cube.rotate(0, 0, 0);

    double x = 1, y = 1, z = 1;
    matrix::Matrix4x4 obj2world;

    e3d::Device dev {win, e3d::Camera(1, 2, 0.01f, 1000.0f, 5*3.141592f/6.0f)};
    dev.camera.setPosition(0, 1, 5);

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
                else if (event.key.code == Keyboard::Left) {
                    dev.camera.move(-0.01, 0, 0);
                }
                else if (event.key.code == Keyboard::Right) {
                    dev.camera.move(0.01, 0, 0);
                }
                else if (event.key.code == Keyboard::Up) {
                    dev.camera.move(0, 0, -0.01);
                }
                else if (event.key.code == Keyboard::Down) {
                    dev.camera.move(0, 0, 0.01);
                }

            }
        }

        elapsedTime = deltaClock.restart();
        ImGui::SFML::Update(win, elapsedTime);

        rotation += elapsedTime.asSeconds() * 0.2;
        obj2world = e3d::buildRotationMatrix(0, rotation, 0) * e3d::buildTraslationMatrix(x, y, z);

        cube.setRotation(rotation, 0, 0);

        win.clear(Color::Black);
        ImGui::SFML::Render(win);
        dev.draw(*tetrahedron, obj2world);
        cube.draw(dev);
        win.display();
    }
    ImGui::SFML::Shutdown();
}
