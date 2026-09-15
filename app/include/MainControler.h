//
// Created by andjela375 on 9/9/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLER_H
#define MATF_RG_PROJECT_MAINCONTROLER_H
#include "engine/core/Controller.hpp"
#include "glm/glm.hpp"

namespace app {
class MainControler : public engine::core::Controller {
public:
    void initialize() override;
    bool loop() override;
    void draw_skybox();
    void draw() override;
    void draw_terrain();
    void draw_naruto();
    void draw_naruto_sage();
    void naruto();
    void begin_draw() override;
    void end_draw() override;
    void update_camera();
    void update_darkness();
    void update_jump();
    void update() override;
    std::string_view name() const override { return "app::MainControler"; }
    void set_light_color(glm::vec3 color) {light_color = color;}
    glm::vec3 get_light_color() const {return light_color;}

private:
    bool show_sage_naruto = false;
    glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 naruto_position = glm::vec3(0.0f, -6.5f, -20.0f);

    bool is_animating = false;
    float animation_start_time = 0.0f;
    float animation_duration = 3.5f;

    bool is_waiting = false;
    float wait_start_time = 0.0f;
    float wait_duration = 1.0f;

    bool is_dark = false;
    float darkness = 0.0f;

};

}

#endif//MATF_RG_PROJECT_MAINCONTROLER_H
