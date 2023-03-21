#pragma once

#include <string_view>

namespace ds::tests
{
    enum class Color
    {
        Red,
        Green,
        Blue,
        Yellow,
        Default
    };

    class Console
    {
    public:
        auto print (std::string_view) -> void;
        auto print (std::string_view, Color) -> void;
        auto print (std::string_view, Color, int) -> void;
        auto println (std::string_view) -> void;
        auto println (std::string_view, Color) -> void;
        auto println (std::string_view, Color, int) -> void;

    private:
        auto set_next_token_width (int) -> void;
        auto set_color (Color) -> void;
        auto reset_color () -> void;

    public:
#if defined(_WIN32) || defined(_WIN64)
        Console();
#endif

    private:
#if defined(_WIN32) || defined(_WIN64)
        unsigned short defaultColor_;
#endif
    };
}