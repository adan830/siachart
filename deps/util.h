#pragma once

#include <fstream>
#include <chrono>

namespace util {

inline bool readFile(const char* file_path, SkString& out) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        // std::cout << "Error opening file" << std::endl;
        return false;
    }
    //read file
    std::string str((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    file.close();

    out.set(str.c_str());
    return true;
}

//�жϸ������Ƿ�Ϊ0
template<typename T>
inline bool isZero(T a) {
    return (a == 0) || (abs(a) < 10e-10);
}

// alhpa:���� beta����ĸ
template<typename T>
inline T divide(T a, T b) {
    return isZero(b) ? 0 : (a / b);
}

//ȡ��
template<typename T>
inline T mod(T a, T b) {
    return isZero(b) ? 0 : (a % b);
}

//�жϸ��������Ƿ����
template<typename T>
inline bool isEqual(T a, T b) {
    return isZero(a - b);
}

// ʱ��ռ�õļ��
using namespace std::chrono;

template<typename Payload>
class StopWatch {
public:
    struct Record {
        Payload pl;
        high_resolution_clock::time_point tp;
    };

    StopWatch() {
    }

    ~StopWatch() {
    }

    inline void reset() {
        tp_.clear();
    }

    inline void record(const Payload& p) {
        Record r;
        r.pl = p;
        r.tp = high_resolution_clock::now();
        tp_.push_back(r);
    }

    // ʱ���ȥ�ĺ�����
    inline double totalDuration()const {
        if (recordCount() < 2) {
            return 0;
        }
        auto ret = duration_cast<duration<double>>(tp_.back().tp - tp_.front().tp).count() * 1000;
        std::cout << "time duration:" << ret << std::endl;
        return ret;
    }

    inline int recordCount()const {
        return (int)tp_.size();
    }

private:
    std::vector<Record> tp_;
};

}

