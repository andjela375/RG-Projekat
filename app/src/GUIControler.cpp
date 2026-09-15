//
// Created by andjela375 on 9/11/26.
//

#include "GUIControler.h"

#include "MainControler.h"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/platform/PlatformController.hpp"
#include "imgui.h"

namespace app {

void GUIControler::initialize() {
    set_enable(false);
}

void GUIControler::poll_events() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_F).state() == engine::platform::Key::State::JustPressed) {
        set_enable(!is_enabled());
    }
}


void GUIControler::draw() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    graphics->begin_gui();

    ImGui::Begin("Camera info");

    ImGui::Text("Camera position: (%f, %f, %f)", camera->Position.x, camera->Position.y, camera->Position.z);

    auto main_controler = engine::core::Controller::get<MainControler>();
    if (main_controler) {
        static float light_color[3] = {1.0f, 1.0f, 1.0f};
        if (ImGui::ColorEdit3("Light color", light_color)) {
            main_controler->set_light_color(glm::vec3(light_color[0], light_color[1], light_color[2]));
        }

        static float cube_pos[3] = {0.0f, -2.0f, -3.0f};
        if (ImGui::SliderFloat3("Cube position", cube_pos, -10.0f, 10.0f)) {
            main_controler->set_cube_position(glm::vec3(cube_pos[0], cube_pos[1], cube_pos[2]));
        }

        if (ImGui::Button("Trigger action")) {
            main_controler->trigger_action();
        }
    }

    ImGui::End();

    graphics->end_gui();
}


}// namespace app