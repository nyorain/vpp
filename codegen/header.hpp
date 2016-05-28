const auto constexpr fwdHeader = 1 + R"SRC(
#include "flags.hpp"

#include <vulkan/vulkan.h>
#include <cstdint>

)SRC";


const auto constexpr mainHeader = 1 + R"SRC(
#include "fwd.hpp"
#include "enums.hpp"
#include "structs.hpp"
#include "functions.hpp"

)SRC";

const auto constexpr structsHeader = 1 + R"SRC(
#include "fwd.hpp"
#include "enums.hpp"

)SRC";

const auto constexpr enumsHeader = 1 + R"SRC(
#include "fwd.hpp"

)SRC";

const auto constexpr functionsHeader = 1 + R"SRC(
#include "fwd.hpp"
#include "enums.hpp"
#include "structs.hpp"
#include "call.hpp"

)SRC";

constexpr const char* keywords[]= {"alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand",
	"bitor", "bool", "break", "case", "catch", "char", "char16_t", "char32_t", "class", "compl",
	"const", "constexpr", "const_cast", "continue", "decltype", "default", "delete", "do",
	"double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float",
	"for","friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new",
	"noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private",
	"protected public", "register", "reinterpret_cast","return","short","signed","sizeof",
	"static","static_assert","static_cast","struct", "switch", "template", "this","thread_local",
	"throw","true","try","typedef","typeid","typename","union","unsigned", "using","virtual",
	"void", "volatile","wchar_t","while","xor","xor_eq"};
