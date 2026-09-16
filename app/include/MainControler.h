//
// Created by andjela375 on 9/9/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLER_H
#define MATF_RG_PROJECT_MAINCONTROLER_H
#include "algorithm"
#include "cmath"
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
    void set_light_color(glm::vec3 color) { m_light_color = color; }
    glm::vec3 get_light_color() const { return m_light_color; }

private:
    bool m_show_sage_naruto = false;
    glm::vec3 m_light_color = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 m_naruto_position = glm::vec3(0.0f, -6.5f, -20.0f);

    bool m_is_animating = false;
    float m_animation_start_time = 0.0f;
    float m_animation_duration = 3.5f;

    bool m_is_waiting = false;
    float m_wait_start_time = 0.0f;
    float m_wait_duration = 1.0f;

    bool m_is_dark = false;
    float m_darkness = 0.0f;
};

}// namespace app

#endif//MATF_RG_PROJECT_MAINCONTROLER_H
