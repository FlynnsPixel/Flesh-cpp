#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <sstream>
#include <stdio.h>

#include "../SceneManager.h"

#define log_file debug::Logger(debug::ACTION_INFO | debug::ACTION_SAVE_TO_FILE)
#define log_print debug::Logger(debug::ACTION_INFO | debug::ACTION_PRINT)
#define log_print_file debug::Logger(debug::ACTION_INFO | debug::ACTION_SAVE_TO_FILE | debug::ACTION_PRINT)

#define log_info debug::Logger(debug::ACTION_INFO | debug::ACTION_SAVE_TO_FILE | debug::ACTION_PRINT)
#define log_warning debug::Logger(debug::ACTION_WARNING | debug::ACTION_SAVE_TO_FILE | debug::ACTION_PRINT)
#define log_error debug::Logger(debug::ACTION_WARNING | debug::ACTION_SAVE_TO_FILE | debug::ACTION_PRINT)

#define log_mid debug::Logger(debug::ACTION_MID | debug::ACTION_SAVE_TO_FILE)

namespace debug {

    extern FILE* lfile;

    extern void init_logger();

    enum LogAction {

        ACTION_INFO = 1 << 1, 
        ACTION_WARNING = 1 << 2, 
        ACTION_ERROR = 1 << 3, 
        ACTION_MID = 1 << 4, 

        ACTION_NO_NEW_LINE = 1 << 5, 

        ACTION_SAVE_TO_FILE = 1 << 6,
        ACTION_PRINT = 1 << 7, 
    };

    class Logger {

        public:
            Logger(LogAction laction) {
                action = laction;
            }

            Logger(int laction) {
                action = (LogAction)laction;
            }

            std::stringstream stream;
            LogAction action;

            template<class T> Logger& operator<<(const T& v) {
                stream << v;
                return *this;
            }
            
            Logger& operator<<(const LogAction& v) {
                action = (LogAction)(action | v);
                return *this;
            }

            ~Logger() {
                if (!(action & ACTION_NO_NEW_LINE)) stream << '\n';

                if (action & ACTION_SAVE_TO_FILE) {
                    fwrite(stream.str().c_str(), stream.str().length(), 1, lfile);
                    fflush(lfile);
                }
                if (action & ACTION_PRINT) {
                    if (action & ACTION_INFO) {
                        cc::log(stream.str().c_str());
                    }else if (action & ACTION_WARNING) {
                        cc::log(stream.str().c_str());
                    }else if (action & ACTION_ERROR) {
                        cc::log(stream.str().c_str());
                    }else if (action & ACTION_MID) {
                        cc::log(stream.str().c_str());
                    }
                }
            }
    };
};

#endif