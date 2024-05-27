#include "Logger.h"
#include <ArduinoJson.h>

char* Logger::log_message_buffer;
char *Logger::current_timestamp_buffer;
const char* Logger::log_path = "/logs/log.log";
SdFile Logger::_file;
SdFat *Logger::_SD;
std::function<void(char *current_timestamp_buffer)> Logger::_get_timestamp;

void Logger::create_file_in_folder(const char *folder_path, const char *file_path)
{
    if(!_SD) {
        return; // If no SD-Card was specified skip sd functionality and only print to Serial
    }

    // Check if the file alread exists if not create it
    if (!_SD->exists(folder_path))
    {
        // The log dir does not yet exist
        if (!_SD->mkdir(folder_path))
        {
            Serial.println("[SD-Card] Failed to create dir");
            return;
        }
    }
    // Create logfile if it doesnt exist
    if (!_SD->exists(file_path))
    {
        if (!_file.open(file_path, O_WRONLY | O_CREAT))
        {
            Serial.println("[SD-Card] Failed to create file");
            return;
        }
        _file.close();
    }
}

void Logger::get_current_timestamp()
{
    _get_timestamp(current_timestamp_buffer);
}

unsigned long Logger::previous_millis;
unsigned long Logger::delay;

void Logger::begin(SdFat *SD, std::function<void(char *current_timestamp_buffer)> get_timestamp)
{
    _SD = SD;
    _get_timestamp = get_timestamp;
    log_message_buffer = new char[TOQIX_LOGGER_LOG_MESSAGE_BUFFER_SIZE]();
    current_timestamp_buffer = new char[20]();
    create_file_in_folder("/logs", log_path);

    delay = 2000;
}

void Logger::end()
{
    delete[] log_message_buffer;
    delete[] current_timestamp_buffer;
}

void Logger::log_and_printf(const char *format, ...)
{
    get_current_timestamp();
    strlcpy(log_message_buffer, current_timestamp_buffer, 20); // Null terminator at index 19

    va_list args;
    va_start(args, format);
    vsnprintf(log_message_buffer + 19, TOQIX_LOGGER_LOG_MESSAGE_BUFFER_SIZE - 20, format, args); // Overwrite the null terminator...
    va_end(args);

    strncat(log_message_buffer, "\n", 1);
    Serial.write(log_message_buffer, strnlen(log_message_buffer, TOQIX_LOGGER_LOG_MESSAGE_BUFFER_SIZE));
    write_to_file(log_message_buffer);
}

void Logger::log_and_print(const char *message)
{
    get_current_timestamp();
    strlcpy(log_message_buffer, current_timestamp_buffer, 20);
    strncat(log_message_buffer, message, TOQIX_LOGGER_LOG_MESSAGE_BUFFER_SIZE-1); //-1 to keep space for the newline character
    strncat(log_message_buffer, "\n", 1);

    Serial.write(log_message_buffer, strnlen(log_message_buffer, TOQIX_LOGGER_LOG_MESSAGE_BUFFER_SIZE));
    write_to_file(log_message_buffer);
}

void Logger::write_to_file(const char *message)
{
    if(!_SD) {
        return; // If no SD-Card was specified skip sd functionality and only print to Serial
    }

    if (!_file.open(log_path, O_WRITE | O_AT_END))
    {
        Serial.println("[SD-Card] Failed to write log message to SD");
        return;
    }

    _file.println(log_message_buffer);
    _file.close();
}
