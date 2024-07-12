#ifndef SDFAT_LOGGER_FS_HANDLER_IMPLEMENTATION_H_
#define SDFAT_LOGGER_FS_HANDLER_IMPLEMENTATION_H_

#if (__has_include(<SdFat.h>))

#include <SdFat.h>
#include "LoggerFSImplementations/LoggerFSHandler.h"

class SdFat_LoggerFSHandlerImplementation : public LoggerFSHandler
{
private:
	SdFat *_SD;
	SdFile _file;
public:
	SdFat_LoggerFSHandlerImplementation(SdFat *SD) : _SD(SD) {}

	bool exists(const char *path) override
	{
		return _SD->exists(path);
	}

	bool mkdir(const char *path) override
	{
		return _SD->mkdir(path);
	}

	void mkfile(const char *path) override
	{
		_file.open(path, O_WRITE | O_CREAT);
		_file.close();
	}

	bool open(const char *path) override
	{
		return _file.open(path, O_WRITE | O_APPEND);
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

#endif // __has_include(<SdFat.h>)
#endif // SDFAT_LOGGER_FS_HANDLER_IMPLEMENTATION_H_