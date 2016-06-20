auto constexpr fwdHeader = 1 + R"SRC(
#include "flags.hpp"
#include "handle.hpp"

#include <vulkan/vk_platform.h>

)SRC";


auto constexpr mainHeader = 1 + R"SRC(
#include "fwd.hpp"
#include "enums.hpp"
#include "structs.hpp"
#include "functions.hpp"

)SRC";

auto constexpr structsHeader = 1 + R"SRC(
#include "fwd.hpp"
#include "enums.hpp"

#include <array>
#include <vulkan/vulkan.h>

)SRC";

auto constexpr enumsHeader = 1 + R"SRC(
#include "fwd.hpp"

)SRC";

auto constexpr functionsHeader = 1 + R"SRC(
#include "fwd.hpp"
#include "enums.hpp"
#include "structs.hpp"
#include "call.hpp"
#include "range.hpp"

#include <vector>
#include <vulkan/vulkan.h>

)SRC";

auto constexpr vecFuncTemplate = 1 + R"SRC(
	std::vector<%t> ret;
	%ct count = 0u;
	if(!call::success(VPP_CALL(%f(%a)))) return ret;
	ret.resize(count);
	VPP_CALL(%f(%a));
	return ret;
)SRC";

auto constexpr vecFuncTemplateVoid = 1 + R"SRC(
	std::vector<%t> ret;
	%ct count = 0u;
	%f(%a);
	ret.resize(count);
	%f(%a);
	return ret;
)SRC";

auto constexpr vecFuncTemplateRetGiven = 1 + R"SRC(
	std::vector<%t> ret;
	ret.resize(%c);
	VPP_CALL(%f(%a));
	return ret;
)SRC";

auto constexpr vecFuncTemplateRetGivenVoid = 1 + R"SRC(
	std::vector<%t> ret;
	ret.resize(%c);
	%f(%a);
	return ret;
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
