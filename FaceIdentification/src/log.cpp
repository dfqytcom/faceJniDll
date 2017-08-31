#include "log.h"
#ifdef _WIN32
#include "windows.h"
#endif

ViplLog::~ViplLog() {
#ifdef __VIPL_LOG__
#ifdef _WIN32
  std::string out_str = my_cout_.str();
  std::string sys_cmd = "color 0";
  std::string log_color_str = "LOG_COLOR_";
  std::size_t s_idx = 0, p = out_str.find(log_color_str);
  std::size_t log_color_str_size = log_color_str.size();
  while (p != std::string::npos) {
    std::cout << out_str.substr(s_idx, p - s_idx);
    HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD f_color;
    if (out_str[p + log_color_str_size] == 'R') {
      f_color = FOREGROUND_RED;
    }
    else if (out_str[p + log_color_str_size] == 'G') {
      f_color = FOREGROUND_GREEN;
    }
    else if (out_str[p + log_color_str_size] == 'W') {
      f_color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }
    else if (out_str[p + log_color_str_size] == 'Y') {
      f_color = FOREGROUND_RED | FOREGROUND_GREEN;
    }
    SetConsoleTextAttribute(hdl, f_color | FOREGROUND_INTENSITY);
    s_idx = p + log_color_str_size + 1;
    p = out_str.find(log_color_str, s_idx);
  }
  std::cout << out_str.substr(s_idx) << std::endl;
#else
  std::cout << my_cout_.str() << std::endl;
#endif // _WIN32
#endif // __VIPL_LOG__
}
