#include "BlackList.h"
#include <fcntl.h>
#include <stddef.h>
#include "DllMain.h"
#include "file_reader.h"
#include "fileop.h"
#include "str_util.h"

BlackList::BlackList() {
    load();
}

void BlackList::load() {
    m_blackList.clear();
    auto dll_path = GetDllPath();
    auto config_path = fileop::dirname(fileop::dirname(fileop::dirname(dll_path)));
    loadFile(fileop::join(config_path, "blacklist.txt"));
    loadFile(fileop::join(config_path, "blacklist.user.txt"));
}

void BlackList::loadFile(std::string file) {
    if (!fileop::exists(file)) return;
    int fd;
    if (fileop::open(file, fd, _O_RDONLY | _O_BINARY, _SH_DENYWR, _S_IREAD)) {
        return;
    }
    FILE* f;
    if (!(f = fileop::fdopen(fd, "r"))) {
        fileop::close(fd);
        return;
    }
    auto fi = create_file_reader(f, 0);
    while (true) {
        char* data = NULL;
        size_t data_len = 0;
        if (file_reader_read_line(fi, &data, &data_len)) {
            if (data) free(data);
            break;
        }
        if (data) {
            std::string d(data, data_len);
            free(data);
            data = NULL;
            if (d.empty()) continue;
            auto loc = d.find('#');
            if (loc != std::string::npos) {
                d = d.substr(0, loc);
            }
            while (d.front() == ' ') {
                d = d.substr(1);
            }
            while (d.back() == ' ') {
                d.pop_back();
            }
            if (d.empty()) continue;
            append(d);
        }
    }
    if (fi) free_file_reader(fi);
    if (f) fileop::fclose(f);
    return;
}

void BlackList::append(std::string& entry) {
    if (entry.size() != 128) return;
    std::string tmp;
    if (str_util::tolowercase(entry, tmp)) entry = tmp;
    if (!isInclude(entry)) m_blackList.push_back(entry);
}

bool BlackList::isInclude(std::string& entry) {
    if (entry.size() == 64) {
        return isInclude(str_util::str_hex(entry));
    }
    if (entry.size() != 128) return false;
    for (auto i = m_blackList.begin(); i != m_blackList.end(); i++) {
        auto &s = *i;
        if (s == entry) return true;
    }
    return false;
}
