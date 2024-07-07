#ifndef ARDUINOFS_LOGGER_FS_HANDLER_IMPLEMENTATION_H_
#define ARDUINOFS_LOGGER_FS_HANDLER_IMPLEMENTATION_H_

#if (__has_include(<FS.h>))

#include <FS.h>
#include "FSImplementations/LoggerFSHandler.h"

class ArduinoFS_LoggerFSHandlerImplementation : public LoggerFSHandler
{
private:
	fs::FS *_fs;
	fs::File _file;
public:
	ArduinoFS_LoggerFSHandlerImplementation(fs::FS *fs) : _fs(fs) {}

	bool exists(const char *path) override
	{
		return _fs->exists(path);
	}

	bool mkdir(const char *path) override
	{
		return _fs->mkdir(path);
	}

	void mkfile(const char *path) override
	{
		_file = _fs->open(path, FILE_WRITE, true);
		_file.close();
	}

	bool open(const char *path) override
	{
		_file = _fs->open(path, FILE_APPEND);
		return _file;
	}

	void close() override
	{
		_file.close();
	}

	void println(const char *message) override
	{
		_file.println(message);
	}
};

#endif // __has_include(<FS.h>)
#endif // ARDUINOFS_LOGGER_FS_HANDLER_IMPLEMENTATION_H_