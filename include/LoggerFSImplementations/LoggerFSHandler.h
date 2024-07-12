#ifndef LOGGER_FS_HANDLER_H_
#define LOGGER_FS_HANDLER_H_

/**
 * @brief The LoggerFSHandler class provides an interface for handling file system operations for logging.
 */
class LoggerFSHandler
{
public:
	/**
	 * @brief Checks if a file or directory exists at the specified path.
	 * @param path The path to the file or directory.
	 * @return True if the file or directory exists, false otherwise.
	 */
	virtual bool exists(const char *path) = 0;

	/**
	 * @brief Creates a new directory at the specified path.
	 * @param path The path to the new directory.
	 * @return True if the directory was created successfully, false otherwise.
	 */
	virtual bool mkdir(const char *path) = 0;

	/**
	 * @brief Creates a new file at the specified path.
	 * @param path The path to the new file.
	 */
	virtual void mkfile(const char *path) = 0;

	/**
	 * @brief Opens a file at the specified path.
	 * @param path The path to the file to be opened.
	 * 
	 * @return True if the file was opened successfully, false otherwise.
	 * 
	 * @warning The file has to be opened in append mode by the implementation.
	 */
	virtual bool open(const char *path) = 0;

	/**
	 * @brief Closes the currently opened file.
	 */
	virtual void close() = 0;

	/**
	 * @brief Writes a line of text to the currently opened file.
	 * @param message The message to be written.
	 */
	virtual void println(const char *message) = 0;
};

#endif