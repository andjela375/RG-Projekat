//
// Created by andjela375 on 9/9/26.
//

#include "GUIControler.h"
#include "engine/graphics/GraphicsController.hpp"
#include "engine/graphics/OpenGL.hpp"
#include "engine/platform/PlatformController.hpp"
#include "engine/resources/ResourcesController.hpp"
#include "spdlog/logger.h"
#include "spdlog/spdlog.h"
#include "GLFW/glfw3.h"
#include "cmath"

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
            return ;
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

    void MainControler::update() {
        update_camera();
        update_lights();
    }

    void MainControler::update_lights() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        float current_time = static_cast<float>(glfwGetTime());

        if (platform->key(engine::platform::KeyId::KEY_SPACE).state() == engine::platform::Key::State::JustPressed) {
            if (action_time < 0.0f) {
                action_time = current_time;
                action_triggered = true;
            }
        }

        if (action_triggered && action_time > 0.0f) {
            float elapsed = current_time - action_time;

            if (elapsed > 3.0f && elapsed < 3.1f) {
                light_color = glm::vec3(1.0f, 1.0f, 1.0f);
            }
            if (elapsed > 5.0f) {
                cube_position.y += 0.01f;
            }
        }
    }

    void MainControler::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void MainControler::draw_skybox() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto skybox = resources->skybox("skybox");
        auto shader = resources->shader("skybox");
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
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
        engine::resources::Model* terrain = resources->model("terrain");

        engine::resources::Shader* shader = resources->shader("basic");
        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -8.0f, -3.0f));
        model = glm::scale(model, glm::vec3(0.3f));
        shader->set_mat4("model", model);
        terrain->draw(shader);
    }

void MainControler::naruto() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        float current_time = static_cast<float>(glfwGetTime());

        if (platform->key(engine::platform::KeyId::KEY_N).state() == engine::platform::Key::State::JustPressed) {
            show_sage_naruto = !show_sage_naruto;
        }

        if (show_sage_naruto) {
            draw_naruto_sage();
        } else {
            draw_naruto();
        }
    }


void MainControler::draw_naruto() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        engine::resources::Model* naruto = resources->model("Naruto");

        engine::resources::Shader* shader = resources->shader("basic");
        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        shader->set_vec3("lightColor", light_color);
        shader->set_vec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
        shader->set_vec3("viewPos", graphics->camera()->Position);
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(4.5f, -7.5f, -40.0f));
        //model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        //model = glm::rotate(model, glm::radians(55.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, cube_position);
        model = glm::scale(model, glm::vec3(1.0f));

        //if (platform->key(engine::platform::KeyId::KEY_R).is_down()) {
          //  model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
        //}

        shader->set_mat4("model", model);
        naruto->draw(shader);
    }

void MainControler::draw_naruto_sage() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        engine::resources::Model* narutoSage = resources->model("NarutoSage");

        engine::resources::Shader* shader = resources->shader("basic");
        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        shader->set_vec3("lightColor", light_color);
        shader->set_vec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
        shader->set_vec3("viewPos", graphics->camera()->Position);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cube_position);
        model = glm::scale(model, glm::vec3(1.0f));

        //if (platform->key(engine::platform::KeyId::KEY_R).is_down()) {
        //  model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
        //}

        shader->set_mat4("model", model);
        narutoSage->draw(shader);
    }


}