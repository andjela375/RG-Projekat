//
// Created by andjela375 on 9/9/26.
//

#include "GUIControler.h"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "spdlog/spdlog.h"

#include <MainControler.h>

namespace app {

class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
public:
    void on_mouse_move(engine::platform::MousePosition position) override;
};

void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    auto gui_controler = engine::core::Controller::get<GUIControler>();
    if (!gui_controler->is_enabled()) {
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        camera->rotate_camera(position.dx, position.dy);
    }
}

void MainControler::initialize() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
    engine::graphics::OpenGL::enable_depth_testing();
}

bool MainControler::loop() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
        return false;
    }
    return true;
}

void MainControler::update_camera() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    float dt = platform->dt();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto camera = graphics->camera();
    auto gui_controler = engine::core::Controller::get<GUIControler>();
    if (gui_controler->is_enabled()) {
        return;
    }

    if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
    }

    if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
    }

    if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
    }

    if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
    }
}

void MainControler::update_darkness() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

    if (platform->key(engine::platform::KeyId::KEY_P).state() == engine::platform::Key::State::JustPressed) {
        m_is_dark = !m_is_dark;
    }

    float dt = platform->dt();
    float target = m_is_dark ? 1.0f : 0.0f;
    float speed = 1.5f;

    if (m_darkness < target) {
        m_darkness = std::min(m_darkness + speed * dt, target);
    } else if (m_darkness > target) {
        m_darkness = std::max(m_darkness - speed * dt, target);
    }
}

void MainControler::update() {
    update_camera();
    update_darkness();
}

void MainControler::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MainControler::draw_skybox() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto skybox = resources->skybox("skybox");
    auto shader = resources->shader("skybox");
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();

    shader->use();
    shader->set_float("darkness", m_darkness);

    graphics->draw_skybox(shader, skybox);
}

void MainControler::draw() {
    naruto();
    draw_terrain();
    draw_skybox();
}

void MainControler::end_draw() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    platform->swap_buffers();
}

void MainControler::draw_terrain() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    engine::resources::Model *terrain = resources->model("terrain");

    engine::resources::Shader *shader = resources->shader("basic");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    glm::vec3 cam_pos = graphics->camera()->Position;
    glm::vec3 cam_dir = graphics->camera()->Front;
    shader->set_vec3("spotLightPos", cam_pos);
    shader->set_vec3("spotLightDir", cam_dir);
    shader->set_vec3("spotLightColor", m_light_color);
    shader->set_float("spotCutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("spotOuterCutOff", glm::cos(glm::radians(17.5f)));
    shader->set_float("darkness", m_darkness);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -9.0f, -3.0f));
    model = glm::scale(model, glm::vec3(0.3f));
    shader->set_mat4("model", model);
    terrain->draw(shader);
}

void MainControler::naruto() {
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    float current_time = static_cast<float>(platform->frame_time().current);

    if (platform->key(engine::platform::KeyId::KEY_N).state() == engine::platform::Key::State::JustPressed) {
        m_show_sage_naruto = !m_show_sage_naruto;
    }

    if (platform->key(engine::platform::KeyId::KEY_R).state() == engine::platform::Key::State::JustPressed) {
        if (!m_is_animating && !m_is_waiting) {
            m_is_waiting = true;
            m_wait_start_time = current_time;
        }
    }

    if (m_is_waiting) {
        float elapsed = current_time - m_wait_start_time;
        if (elapsed >= m_wait_duration) {
            m_is_animating = true;
            m_is_waiting = false;
            m_animation_start_time = current_time;
        }
    }

    if (m_is_animating) {
        float elapsed = current_time - m_animation_start_time;
        if (elapsed >= m_animation_duration) {
            m_is_animating = false;
            m_naruto_position = glm::vec3(0.0f, -6.5f, -20.0f);
        } else {
            float t = elapsed / m_animation_duration;
            float height = sin(t * glm::pi<float>()) * 4.0f;
            m_naruto_position.y = -6.5f + height;
        }
    }

    if (m_show_sage_naruto) {
        draw_naruto_sage();
    } else {
        draw_naruto();
    }
}


void MainControler::draw_naruto() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    engine::resources::Model *naruto = resources->model("Naruto");

    engine::resources::Shader *shader = resources->shader("naruto");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("lightColor", m_light_color);
    shader->set_vec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
    shader->set_vec3("viewPos", graphics->camera()->Position);
    glm::vec3 cam_pos = graphics->camera()->Position;
    glm::vec3 cam_dir = graphics->camera()->Front;
    shader->set_vec3("spotLightPos", cam_pos);
    shader->set_vec3("spotLightDir", cam_dir);
    shader->set_vec3("spotLightColor", m_light_color);
    shader->set_float("spotCutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("spotOuterCutOff", glm::cos(glm::radians(17.5f)));
    shader->set_float("darkness", m_darkness);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_naruto_position);
    model = glm::scale(model, glm::vec3(6.0f));

    if (m_is_animating) {
        float elapsed = static_cast<float>(platform->frame_time().current) - m_animation_start_time;
        float angle = elapsed * 2.0f;
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    shader->set_mat4("model", model);
    naruto->draw(shader);
}

void MainControler::draw_naruto_sage() {
    auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    engine::resources::Model *narutoSage = resources->model("NarutoSage");

    engine::resources::Shader *shader = resources->shader("naruto");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()->view_matrix());
    shader->set_vec3("lightColor", m_light_color);
    shader->set_vec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
    shader->set_vec3("viewPos", graphics->camera()->Position);
    glm::vec3 cam_pos = graphics->camera()->Position;
    glm::vec3 cam_dir = graphics->camera()->Front;
    shader->set_vec3("spotLightPos", cam_pos);
    shader->set_vec3("spotLightDir", cam_dir);
    shader->set_vec3("spotLightColor", m_light_color);
    shader->set_float("spotCutOff", glm::cos(glm::radians(12.5f)));
    shader->set_float("spotOuterCutOff", glm::cos(glm::radians(17.5f)));
    shader->set_float("darkness", m_darkness);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_naruto_position);
    model = glm::scale(model, glm::vec3(8.5f));

    if (m_is_animating) {
        float elapsed = static_cast<float>(platform->frame_time().current) - m_animation_start_time;
        float angle = elapsed * 2.0f;
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    shader->set_mat4("model", model);
    narutoSage->draw(shader);
}


}// namespace app