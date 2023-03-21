#include <tests/_details/console.hpp>

#include <iostream>
#include <iomanip>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

namespace ds::tests
{
    auto Console::print (std::string_view const str) -> void
    {
        std::cout << str;
    }

    auto Console::print ( std::string_view const str
                        , Color const            color ) -> void
    {
        if (color != Color::Default)
        {
            this->set_color(color);
            this->print(str);
            this->reset_color();
        }
        else
        {
            this->print(str);
        }
    }

    auto Console::print ( std::string_view const str
                        , Color const            color
                        , int const              width ) -> void
    {
        if (color != Color::Default)
        {
            this->set_color(color);
            this->set_next_token_width(width);
            this->print(str);
            this->reset_color();
        }
        else
        {
            this->set_next_token_width(width);
            this->print(str);
        }
    }

    auto Console::println (std::string_view const str) -> void
    {
        std::cout << str << '\n';
    }

    auto Console::println ( std::string_view const str
                          , Color const            color ) -> void
    {
        if (color != Color::Default)
        {
            this->set_color(color);
            this->println(str);
            this->reset_color();
        }
        else
        {
            this->println(str);
        }
    }

    auto Console::println ( std::string_view const str
                          , Color const            color
                          , int const              width ) -> void
    {
        if (color != Color::Default)
        {
            this->set_color(color);
            this->set_next_token_width(width);
            this->println(str);
            this->reset_color();
        }
        else
        {
            this->set_next_token_width(width);
            this->println(str);
        }
    }

    auto Console::set_next_token_width (int const w) -> void
    {
        std::cout << std::left << std::setw(w);
    }

#if defined(_WIN32) || defined(_WIN64)

    namespace
    {
        auto convert (Color const color) -> WORD
        {
            switch (color)
            {
            case Color::Red:
                return FOREGROUND_RED | FOREGROUND_INTENSITY;

            case Color::Green:
                return FOREGROUND_GREEN | FOREGROUND_INTENSITY;

            case Color::Blue:
                return FOREGROUND_BLUE | FOREGROUND_INTENSITY;

            case Color::Yellow:
                return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;

            default:
                return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            }
        }

        auto windows_set_color (WORD color) -> void
        {
            HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(handle, color);
        }
    }

    Console::Console ()
    {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
        GetConsoleScreenBufferInfo(handle, &csbiInfo);
        defaultColor_ = csbiInfo.wAttributes;
    }

    auto Console::set_color (Color const color) -> void
    {
        windows_set_color(convert(color));
    }

    auto Console::reset_color () -> void
    {
        windows_set_color(defaultColor_);
    }

#endif

#if defined(__APPLE__) || defined(__linux__)

    namespace
    {
        auto convert (Color const color) -> std::string_view
        {
            switch (color)
            {
            case Color::Red:
                return "\x1B[91m";

            case Color::Green:
                return "\x1B[92m";

            case Color::Blue:
                return "\x1B[94m";

            case Color::Yellow:
                return "\x1B[93m";

            default:
                return "\x1B[97m";
            }
        }
    }

    auto Console::set_color (Color const color) -> void
    {
        std::cout << convert(color);
    }

    auto Console::reset_color () -> void
    {
        std::cout << "\x1B[0m";
    }

#endif
}