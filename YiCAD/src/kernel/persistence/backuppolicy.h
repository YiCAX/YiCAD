#ifndef PERSISTENCE_BACKUP_POLICY_H
#define PERSISTENCE_BACKUP_POLICY_H

#include <regex>
#include <FileInfo.h>

// Helper class to handle different backup policies
class BackupPolicy
{
public:
    enum Policy
    {
        Standard,
        TimeStamp
    };

    BackupPolicy();
    ~BackupPolicy();
    void setPolicy(Policy p);
    void setNumberOfFiles(int count);
    void useBackupExtension(bool on);
    void setDateFormat(const std::string& fmt);
    void apply(const std::string& sourcename, const std::string& targetname);

private:
    void applyStandard(const std::string& sourcename, const std::string& targetname);    
    void applyTimeStamp(const std::string& sourcename, const std::string& targetname);
    
    static bool fileComparisonByDate(const FileInfo& i, const FileInfo& j);
    bool startsWith(const std::string& st1, const std::string& st2) const;
    bool checkValidString(const std::string& cmpl, const std::regex& e) const;
    bool checkValidComplement(const std::string& file, const std::string& pbn, const std::string& ext) const;
    bool checkDigits(const std::string& cmpl) const;
    bool renameFileNoErase(FileInfo fi, const std::string& newName);

private:
    Policy policy;
    int numberOfFiles;
    bool useBakExtension;
    std::string saveBackupDateFormat;
};
#endif //PERSISTENCE_BACKUP_POLICY_H