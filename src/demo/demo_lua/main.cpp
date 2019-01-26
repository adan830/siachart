#include <iostream>
#include <deps/sol2/sol.hpp>
#include "SkString.h"

bool parseCommon(sol::table& t) {
    for (auto i : t) {
        // std::cout << i;
    }
    return false;
}

bool loadFile(const SkString& file) {
    sol::state lua;

    lua.open_libraries(sol::lib::base);
    // ����/���/ִ��/�����
    sol::load_result code = lua.load_file(file.c_str());
    if (!code.valid() || !code().valid()) {
        return false;
    }

    // �ȶ�ȫ�ֵ�����
    sol::table global = lua["global"];
    if (global.valid()) {
        if (!parseCommon(global)) {
            return false;
        }
    }

    // �������Լ�������
    return true;
}


int main() {

    // loadFile(SkString("./paint.lua"));

    sol::state lua;
    lua.open_libraries(sol::lib::base);
    lua.script("myval='hello world! good!'");
    lua.script("print(myval)");

    std::string tmp;
    std::cin >> tmp;
    return 0;
}
