#pragma once

#include "pch.h"

#include "Common/src/Singleton.h"
#include "System.h"

namespace sandbox {
    namespace common {

        class Common : public Singleton<Common>, public System {

         public:

            Common() : m_initialized(false) {}

            void OnPostStart() override;

         private:

            void Initialize();
            void CacheSides();

            bool m_initialized;

        };

    }
}