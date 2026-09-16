//
// Created by andjela375 on 9/11/26.
//

#ifndef MATF_RG_PROJECT_GUICONTROLER_H
#define MATF_RG_PROJECT_GUICONTROLER_H
#include "engine/core/Controller.hpp"

namespace app {

class GUIControler : public engine::core::Controller {
public:
    std::string_view name() const override { return "app::GUI Controler"; }

private:
    void initialize() override;
    void draw() override;
    void poll_events() override;
};
}// namespace app

#endif//MATF_RG_PROJECT_GUICONTROLER_H
