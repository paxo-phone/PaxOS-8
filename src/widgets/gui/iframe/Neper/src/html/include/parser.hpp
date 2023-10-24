#ifndef __HTML_PARSER__
#define __HTML_PARSER__

#include <string>

#include "../../dom/dom.hpp"

namespace html
{
    dom::Node* parseHTML(const std::string& str);
} // namespace html

#endif /* __HTML_PARSER___ */