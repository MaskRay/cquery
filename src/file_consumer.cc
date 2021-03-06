#include "file_consumer.h"

#include "clang_utils.h"
#include "indexer.h"
#include "platform.h"
#include "utils.h"

#include <loguru.hpp>

namespace {

optional<std::string> GetFileContents(const std::string& path,
                                      FileContentsMap* file_contents) {
  auto it = file_contents->find(path);
  if (it == file_contents->end()) {
    optional<std::string> content = ReadContent(path);
    if (content)
      (*file_contents)[path] = FileContents(path, *content);
    return content;
  }
  return it->second.content;
}

}  // namespace

bool operator==(const CXFileUniqueID& a, const CXFileUniqueID& b) {
  return a.data[0] == b.data[0] && a.data[1] == b.data[1] &&
         a.data[2] == b.data[2];
}

bool FileConsumerSharedState::Mark(const std::string& file) {
  std::lock_guard<std::mutex> lock(mutex);
  return used_files.insert(file).second;
}

void FileConsumerSharedState::Reset(const std::string& file) {
  std::lock_guard<std::mutex> lock(mutex);
  auto it = used_files.find(file);
  if (it != used_files.end())
    used_files.erase(it);
}

FileConsumer::FileConsumer(FileConsumerSharedState* shared_state,
                           const std::string& parse_file)
    : shared_(shared_state), parse_file_(parse_file) {}

IndexFile* FileConsumer::TryConsumeFile(CXFile file,
                                        bool* is_first_ownership,
                                        FileContentsMap* file_contents) {
  assert(is_first_ownership);

  CXFileUniqueID file_id;
  if (clang_getFileUniqueID(file, &file_id) != 0) {
    EmitError(file);
    return nullptr;
  }

  // Try to find cached local result.
  auto it = local_.find(file_id);
  if (it != local_.end()) {
    *is_first_ownership = false;
    return it->second.get();
  }

  std::string file_name = FileName(file);

  // No result in local; we need to query global.
  bool did_insert = shared_->Mark(file_name);
  *is_first_ownership = did_insert;
  local_[file_id] =
      did_insert ? MakeUnique<IndexFile>(
                       file_name, GetFileContents(file_name, file_contents))
                 : nullptr;
  return local_[file_id].get();
}

IndexFile* FileConsumer::ForceLocal(CXFile file,
                                    FileContentsMap* file_contents) {
  // Try to fetch the file using the normal system, which will insert the file
  // usage into global storage.
  {
    bool is_first;
    IndexFile* cache = TryConsumeFile(file, &is_first, file_contents);
    if (cache)
      return cache;
  }

  // It's already been taken before, just create a local copy.
  CXFileUniqueID file_id;
  if (clang_getFileUniqueID(file, &file_id) != 0) {
    EmitError(file);
    return nullptr;
  }

  auto it = local_.find(file_id);
  if (it == local_.end() || !it->second) {
    std::string file_name = FileName(file);
    local_[file_id] = MakeUnique<IndexFile>(
        file_name, GetFileContents(file_name, file_contents));
  }
  assert(local_.find(file_id) != local_.end());
  return local_[file_id].get();
}

std::vector<std::unique_ptr<IndexFile>> FileConsumer::TakeLocalState() {
  std::vector<std::unique_ptr<IndexFile>> result;
  for (auto& entry : local_) {
    if (entry.second)
      result.push_back(std::move(entry.second));
  }
  return result;
}

void FileConsumer::EmitError(CXFile file) const {
  std::string file_name = ToString(clang_getFileName(file));
  // TODO: Investigate this more, why can we get an empty file name?
  if (!file_name.empty()) {
    LOG_S(ERROR) << "Could not get unique file id for " << file_name
                 << " when parsing " << parse_file_;
  }
}