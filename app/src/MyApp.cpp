//
// Created by andjela375 on 9/9/26.
//

#include "MyApp.h"

#include "GUIControler.h"
#include "MainControler.h"

#include <spdlog/spdlog.h>

namespace app {
    void MyApp::app_setup() {
        spdlog::info("App setup completed!");
        auto main_controler = register_controller<app::MainControler>();
        auto gui_controler = register_controller<app::GUIControler>();
        main_controler->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
        main_controler->before(gui_controler);
    }

}