#include "pch.h"

#include "Core/src/Core.h"

using namespace sandbox;


int intercept::api_version() {
  return INTERCEPT_SDK_API_VERSION;
}



void intercept::register_interfaces() {

}

sandbox::Core instance;

void intercept::pre_start() {
    sandbox::Core::Get().OnPreStart();
}

void intercept::post_start() {
    sandbox::Core::Get().OnPostStart();
}

void intercept::pre_init() {
    sandbox::Core::Get().OnPreInit();
}

void intercept::post_init() {
    sandbox::Core::Get().OnPostInit();
}

void intercept::on_frame() {
    if (!Core::IsMissionRunning())
        return;

    if (intercept::sqf::is_game_paused() != Core::IsGamePaused()) {
        if (Core::IsGamePaused())
            Core::Get().OnMissionUnpaused();
        else
            Core::Get().OnMissionPaused();
    }

    sandbox::Core::Get().OnFrame();
}

void intercept::mission_ended() {
    sandbox::Core::Get().OnMissionEnded();
}

void intercept::handle_unload() {
    sandbox::Core::Get().OnPluginUnloaded();
}