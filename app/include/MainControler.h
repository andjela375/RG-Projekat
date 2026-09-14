//
// Created by andjela375 on 9/9/26.
//

#ifndef MATF_RG_PROJECT_MAINCONTROLER_H
#define MATF_RG_PROJECT_MAINCONTROLER_H
#include "engine/core/Controller.hpp"

namespace app {
class MainControler : public engine::core::Controller {
    void initialize() override;
    bool loop() override;
    void draw_skybox();
    void draw() override;
    void draw_terrain();
    void draw_naruto();
    void begin_draw() override;
    void end_draw() override;
    void update_camera();
    void update() override;

public:
    std::string_view name() const override { return "app::MainControler"; }
};

}

#endif//MATF_RG_PROJECT_MAINCONTROLER_H
