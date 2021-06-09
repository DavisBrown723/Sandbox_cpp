#pragma once

namespace sandbox {
    namespace common {

        class System {
            public:

            virtual void OnPreStart() {}
            virtual void OnPostStart() {}
            virtual void OnPreInit() {}
            virtual void OnPostInit() {}
            virtual void OnFrame(double) {}
            virtual void OnMissionPaused() {}
            virtual void OnMissionUnpaused() {}
            virtual void OnMissionEnded() {}
            virtual void OnPluginUnloaded() {}
        };

    }
}