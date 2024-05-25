#ifndef LOGGER_H_
#define LOGGER_H_

#include <SdFat.h>
#include <stdarg.h>
#include <functional>

#define TOQIX_LOGGER_LOG_MESSAGE_BUFFER_SIZE 256

class Logger
{
private:
    static char *log_message_buffer;
    static char *current_timestamp_buffer; // The TIMESTAMP should be exactly 20 characters long including a null terminator
    static const char *log_path;
    static SdFile _file;
    static SdFat *_SD;
    static std::function<void(char *current_timestamp_buffer)> _get_timestamp;

    static void create_file_in_folder(const char *folder_path, const char *file_path);

    static void get_current_timestamp();

public:
    static unsigned long previous_millis;
    static unsigned long delay; // standard print every 2 seconds

    static void begin(SdFat *SD, std::function<void(char *current_timestamp_buffer)> get_timestamp);
    static void end();

    static void log_and_printf(const char *format, ...);
    static void log_and_print(const char *message);

private:
    static void write_to_file(const char *message);
};
#endif