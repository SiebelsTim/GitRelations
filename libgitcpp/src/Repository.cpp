#include "Repository.h"
#include "Reference.h"
#include "Commit.h"
#include "CommitIterator.h"
#include "error.h"
#include <cstring>
#include <unistd.h> 
#include <assert.h>
#include <memory>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

// trim from start
static inline std::string ltrim(std::string s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string rtrim(std::string s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string trim(std::string s) {
        return ltrim(rtrim(s));
}


Repository::Repository(const std::string& path): m_path(path) {
  git_libgit2_init();
  git_repository* repository = nullptr;
  int error = git_repository_open(&repository, path.c_str());
  repo = std::shared_ptr<git_repository>(repository, git_repository_free);
  throw_on_error(error);
}

Repository::Repository(const Repository& repo):
  repo(repo.repo) {
  git_libgit2_init();
}

Repository::~Repository() {
  repo.reset();
  git_libgit2_shutdown();
}

std::string Repository::getName() const {
  return m_path.substr(m_path.find_last_of('/') + 1);
}

bool Repository::operator==(const Repository& rep) const {
  const char* workdir = git_repository_workdir(repo.get());
  const char* other_workdir = git_repository_workdir(rep.get());
  return strcmp(workdir, other_workdir) == 0;
}

bool Repository::operator!=(const Repository& rep) const {
  return !operator==(rep);
}

// 0th path only
std::vector<Commit> Repository::getAllCommits() const {
  Commit commit = head().toCommit();
  std::vector<Commit> ret;
  while (true) {
    ret.push_back(commit);
    if (commit.parentCount() == 0) break;
    commit = commit.parent(0);
  }
  return ret;
}

std::vector<CommitX*> Repository::getAllCommitsX(int since /* = 0 */, int until /* = 0 */) const {
  char* cwd = get_current_dir_name();
  if (chdir(m_path.c_str()) != 0) perror("getAllCommitsX");
  std::string cmd = "git log --stat=10000 --date=raw";
  if (since > 0) {
    cmd += " --since=" + std::to_string(since);
  }
  if (until > 0) {
    cmd += " --until=" + std::to_string(until);
  }

  std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
  if (!pipe) throw std::exception(); 
  std::vector<CommitX*> ret;
  CommitX* currentCommit = nullptr;
  char buffer[255];
  while (!feof(pipe.get())) {
    if (fgets(buffer, 255, pipe.get()) == nullptr) continue;
    
    std::string buf(buffer);
    if (buf.substr(0, 6) == "commit") {
      if (currentCommit != nullptr) {
        ret.push_back(currentCommit);
      }
      currentCommit = new CommitX;
    } else if (buf.substr(0, 7) == "Author:") {
      auto mail_open = buf.find_first_of('<');
      std::string name = buf.substr(8, mail_open - 9);
      std::string mail = buf.substr(mail_open + 1, buf.find_first_of('>') - mail_open - 1);
      currentCommit->setAuthor(name);
      currentCommit->setAuthorMail(mail);
    } else if (buf.substr(0, 5) == "Date:") {
      std::string timestr = trim(buf.substr(6));
      timestr = timestr.substr(0, timestr.find_first_of(' ')); // Strip tz
      currentCommit->setTime(stoi(timestr));
    } else if (*(buffer+1) != ' ') { // file
      FileStat file;
      auto pipe = buf.find_first_of('|');
      if (pipe != std::string::npos) {
        file.filename = trim(buf.substr(0, pipe));
        std::string stats = buf.substr(pipe+2);
        file.linesChanged = atoi(stats.substr(0, stats.find_first_of(' ')).c_str());
        currentCommit->addFile(file);
      }
    }
  }
  
  if (currentCommit != nullptr) { // might be none due to since/until
    ret.push_back(currentCommit);
  }

  if (chdir(cwd) != 0) perror("getAllCommitsX");
  free(cwd);
 
  return ret; 
}

git_repository* Repository::get() const {
  return repo.get();
}

Reference Repository::head() const {
  git_reference* ref = nullptr;
  int error = git_repository_head(&ref, get());
  throw_on_error(error);
  return Reference(ref, *this);
}

Repository::iterator Repository::iter() const {
  return iterator(*this);
}

CommitIterator Repository::iterator::begin() const {
  auto iter = CommitIterator(repo);
  iter.setFilter(filter);
  return iter;
}

CommitIterator Repository::iterator::end() const {
  return CommitIterator(repo, true);
}
