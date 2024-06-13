#include "Logger.h"

char *Logger::log_message_buffer;
char *Logger::current_timestamp_buffer;
const char *Logger::log_path = "/logs/log.log";
fs::FS *Logger::_fs;
std::function<void(char *current_timestamp_buffer)> Logger::_get_timestamp;

void Logger::create_file_in_folder(const char *folder_path, const char *file_path)
{
    if (!_fs)
    {
        return; // If no Filesystem was specified skip fs functionality and only print to Serial
    }

    // Check if the file alread exists if not create it
    if (!_fs->exists(folder_path))
    {
        // The log dir does not yet exist
        if (!_fs->mkdir(folder_path))
        {
            Serial.println("[Filesystem] Failed to create dir");
            return;
        }
    }
    // Create logfile if it doesnt exist
    File file = _fs->open(file_path, FILE_WRITE);

    if (!file)
    {
        Serial.println("[Filesystem] Failed to create file");
        return;
    }
    file.close();
}

void Logger::get_current_timestamp()
{
    _get_timestamp(current_timestamp_buffer);
}

unsigned long Logger::previous_millis;
unsigned long Logger::delay;

void Logger::begin(fs::FS *filesystem, std::function<void(char *current_timestamp_buffer)> get_timestamp)
{
    _fs = filesystem;
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
    int timestamp_length = strnlen(log_message_buffer, TOQIX_LOGGER_LOG_MESSAGE_BUFFER_SIZE);

    va_list args;
    va_start(args, format);
    vsnprintf(log_message_buffer + timestamp_length, TOQIX_LOGGER_LOG_MESSAGE_BUFFER_SIZE - (timestamp_length + 1), format, args); // Overwrite the null terminator...
    va_end(args);

    strncat(log_message_buffer, "\n", 1);
    Serial.write(log_message_buffer, strnlen(log_message_buffer, TOQIX_LOGGER_LOG_MESSAGE_BUFFER_SIZE));
    write_to_file(log_message_buffer);
}

void Logger::log_and_print(const char *message)
{
    get_current_timestamp();
    strlcpy(log_message_buffer, current_timestamp_buffer, 20);
    strncat(log_message_buffer, message, TOQIX_LOGGER_LOG_MESSAGE_BUFFER_SIZE - 1); //-1 to keep space for the newline character
    strncat(log_message_buffer, "\n", 1);

    Serial.write(log_message_buffer, strnlen(log_message_buffer, TOQIX_LOGGER_LOG_MESSAGE_BUFFER_SIZE));
    write_to_file(log_message_buffer);
}

void Logger::write_to_file(const char *message)
{
    if (!_fs)
    {
        return; // If no Filesystem was specified skip fs functionality and only print to Serial
    }

    File file = _fs->open(log_path, FILE_APPEND);

    if (!file)
    {
        Serial.println("[Filesystem] Failed to write log message to File");
        return;
    }

    file.println(log_message_buffer);
    file.close();
}
