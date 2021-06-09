#pragma once

namespace sandbox {
    namespace common {

        template<typename T>
        class Singleton {
            public:

            Singleton(const Singleton&) = delete;
            Singleton(Singleton&&) = delete;
            Singleton& operator=(const Singleton&) = delete;
            Singleton& operator=(Singleton&&) = delete;

            inline static T& Get() {
                static T instance;
                return instance;
            }

            protected:

            Singleton() {}
        };

    }
}