#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "voyc/token.hpp"

namespace voyc{

struct LexResult{
    // 역할: string_view로 된 소스코드를 읽어서 토큰으로 분해하는 역할
    std::vector<Token> tokens;
    std::vector<std::string> errors;
};

LexResult lexSource(const std::string_view source);

} // namespace voyc