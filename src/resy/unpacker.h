#pragma once

#include <string>
#include "resyapi.h"

namespace sia {
namespace resy {

struct ZipFileInfo;

// ��Դ���
class RESY_API Unpacker {
public:
    ~Unpacker();

    bool load(const std::string& file);
    bool unload();
    bool loaded()const;

    // �ͷ�name �� out·��
    bool release2File(const std::string& name, const std::string& out);

    // ��ѹ���ڴ�out��
    bool extract2Mem(const std::string& name, std::string& out);

private:
    bool getFileInfo(ZipFileInfo& info);
    bool readFile(const std::string& file, std::string& out);
    bool gotoFile(const std::string& file);

protected:
    void* hzip_ = nullptr;

    // �ļ���Ϣ
    // std::unordered_map<std::string, ZipFileInfo> files_;
};
}
}
