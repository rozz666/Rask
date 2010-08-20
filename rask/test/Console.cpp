#ifdef _WIN32
#include <Windows.h>
#endif /* _WIN32 */

#include <rask/test/Console.hpp>

namespace rask
{

namespace test
{

#ifdef _WIN32
namespace 
{

class Console
{
    HANDLE handle;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

public:

    Console()
    {
        handle = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(handle, &csbi);
    }

    void reset_color()
    {
        SetConsoleTextAttribute(handle, csbi.wAttributes);
    }

    void set_color(WORD color)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi = this->csbi;

        csbi.wAttributes &= (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
        csbi.wAttributes |= color & (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);

        SetConsoleTextAttribute(handle, csbi.wAttributes);
    }

} console;

}
#endif /* _WIN32 */

std::ostream& rc(std::ostream& os)
{
    os.flush();
#ifdef _WIN32
    console.reset_color();
#endif /* _WIN32 */
    return os;
}

std::ostream& green(std::ostream& os)
{
    os.flush();
#ifdef _WIN32
    console.set_color(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#endif /* _WIN32 */
    return os;
}

std::ostream& red(std::ostream& os)
{
    os.flush();
#ifdef _WIN32
    console.set_color(FOREGROUND_RED | FOREGROUND_INTENSITY);
#endif /* _WIN32 */
    return os;
}

}
}

