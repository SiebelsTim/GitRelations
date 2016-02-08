#include "CommitX.h"
#include <vector>

void CommitX::setCommitMessage(const std::string& msg) {
  m_message = msg;
}

std::string CommitX::getCommitMessage() const {
  return m_message;
}

void CommitX::addFile(const FileStat& file) {
  m_files.push_back(file);
}

std::vector<FileStat> CommitX::getFiles() const {
  return m_files;
}

void CommitX::setAuthor(const std::string& author) {
  m_author = author;
}

std::string CommitX::getAuthor() const {
  return m_author;
}

void CommitX::setAuthorMail(const std::string& mail) {
  m_mail = mail;
}

std::string CommitX::getAuthorMail() const {
  return m_mail;
}

void CommitX::setTime(const int time) {
  m_timestamp = time;
}

int CommitX::getTime() const {
  return m_timestamp;
}
