#ifndef BASE_FILEINFO_H
#define BASE_FILEINFO_H

#include <filesystem>
#include <string>
#include <vector>
#include <TimeInfo.h>

/** File name unification
  * This class handles everything related to file names
  * the file names are internal generally UTF-8 encoded on
  * all platforms.
  */
class FileInfo
{
public:
    enum Permissions {
        WriteOnly = 0x01,
        ReadOnly = 0x02,
        ReadWrite = 0x03,
    };

    /// Construction
    FileInfo (const char* _FileName="");
    FileInfo (const std::string &_FileName);
    /// Set a new file name
    void setFile(const char* name);
    /// Set a new file name
    void setFile(const std::string &name){setFile(name.c_str());}


    /** @name extraction of information */
    //@{
    /// Returns the file name, including the path (which may be absolute or relative).
    std::string filePath () const;
    /// Returns the dir path name (which may be absolute or relative).
    std::string dirPath () const;
    /// Returns the name of the file, excluding the path, including the extension.
    std::string fileName () const;
    /// Returns the name of the file, excluding the path and the extension.
    std::string fileNamePure () const;
    /// Convert the path name into a UCS-2 encoded wide string format.
    std::wstring toStdWString() const;
    /** Returns the extension of the file.
     * The extension consists of all characters in the file after (but not including)
     * the last '.' character.
     *@code
     *  FileInfo fi( "/tmp/archive.tar.gz" );
     *  ext = fi.extension();   // ext = "gz"
     *@endcode
     */
    std::string extension () const;
    /** Returns the complete extension of the file.
     * The complete extension consists of all characters in the file after (but not including)
     * the first '.' character.
     *@code
     *  FileInfo fi( "/tmp/archive.tar.gz" );
     *  ext = fi.completeExtension();   // ext = "tar.gz"
     *@endcode
     */
    std::string completeExtension () const;
    /// Checks for a special extension, NOT case sensitive
    bool hasExtension (const char* Ext) const;
    //@}

    /** @name methods to test the status of the file or dir */
    //@{
    /// Does the file exist?
    bool exists () const;
    /// Checks if the file exist and is readable
    bool isReadable () const;
    /// Checks if the file exist and is writable
    bool isWritable () const;
    /// Tries to set the file permission
    bool setPermissions (Permissions);
    /// Checks if it is a file (not a directory)
    bool isFile () const;
    /// Checks if it is a directory (not a file)
    bool isDir () const;
    /// The size of the file
    unsigned int size () const;
    /// Returns the time when the file was last modified.
    TimeInfo lastModified() const;
    /// Returns the time when the file was last read (accessed).
    TimeInfo lastRead() const;
    //@}

    /** @name Directory management*/
    //@{
    /// Creates a directory. Returns true if successful; otherwise returns false.
    bool createDirectory( ) const;
    /// Creates a directory and all its parent directories. Returns true if successful; otherwise returns false.
    bool createDirectories() const;
    /// Get a list of the directory content
    std::vector<FileInfo> getDirectoryContent() const;
    /// Delete an empty directory
    bool deleteDirectory() const;
    /// Delete a directory and all its content.
    bool deleteDirectoryRecursive() const;
    //@}

    /// Delete the file
    bool deleteFile() const;
    /// Rename the file
    bool renameFile(const char* NewName);
    /// Rename the file
    bool copyTo(const char* NewName) const;

    /** @name Tools */
    //@{
    /// Get a unique File Name in the given or (if 0) in the temp path
    static std::string getTempFileName(const char* FileName=nullptr, const char* path=nullptr);

    /// Get the path to the dir which is considered to temp files
    static const std::string &getTempPath();

    /// Convert from filesystem path to string
    static std::string pathToString(const std::filesystem::path& p);

    /// Convert from string to filesystem path
    static std::filesystem::path stringToPath(const std::string& str);
    //@}

protected:
    std::string FileName;
};

#endif // BASE_FILEINFO_H
