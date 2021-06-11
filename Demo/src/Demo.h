#pragma once

#include "Common/src/Singleton.h"
#include "Common/src/System.h"

namespace sandbox {
    namespace __internal {

        class Demo : public common::Singleton<Demo>, public common::System {

            public:

            Demo();

            virtual void OnPreInit() override;
            virtual void OnPostInit() override;
            virtual void OnFrame(double) override;
            virtual void OnMissionEnded() override;

        };

    }
}