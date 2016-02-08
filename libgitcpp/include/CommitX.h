#pragma once

#include "FileStat.h"
#include <vector>
#include <string>

class CommitX {
public:
  void setCommitMessage(const std::string& msg);
  std::string getCommitMessage() const; 
  void addFile(const FileStat& file);
  std::vector<FileStat> getFiles() const;
  void setAuthor(const std::string& author);
  std::string getAuthor() const;
  void setAuthorMail(const std::string& mail);
  std::string getAuthorMail() const;
  void setTime(const int time);
  int getTime() const;
private:
  std::string m_message;
  std::vector<FileStat> m_files;
  std::string m_author;
  std::string m_mail;
  int m_timestamp;
};
