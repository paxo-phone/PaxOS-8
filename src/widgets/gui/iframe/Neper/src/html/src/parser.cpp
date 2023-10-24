#include <iostream>
#include <string>

#include "../include/parser.hpp"

#include "../../dom/dom.hpp"
#include "../../utils/utils.hpp"


namespace html
{

    /* 
        Return true if the element is auto closing 
    */

    bool isautoclosing(const std::string& tag)
    {
        return 
        (
            tag == "area"     ||
            tag == "base"     ||
            tag == "br"       ||
            tag == "col"      ||
            tag == "embed"    ||
            tag == "hr"       ||
            tag == "img"      ||
            tag == "input"    ||
            tag == "link"     ||
            tag == "meta"     ||
            tag == "param"    ||
            tag == "source"   ||
            tag == "track"    ||
            tag == "wbr"      ||
            tag == "command"  ||
            tag == "keygen"   ||
            tag == "menuitem"
        );
    }

    bool parseAttributes(size_t& i, const std::string& str, dom::Node* node)
    {
        std::string currKey, currValue;

        std::string temp = "";

        while(1)
        {
            ignorewhitespaces(i, str);

            while( isalpha(str[i]) )
                temp += str[i++];

            // if( temp.empty() )
            // {}

            currKey = temp;
            temp.clear();

            ignorewhitespaces(i, str);

            if( str[i] == '=' )
            {
                i++;
                ignorewhitespaces(i, str);
                
                bool betweenquotes = false;
                while( !(str[i] == ' ' || str[i] == '>' || str[i] == '/') || 
                    betweenquotes )
                {
                    if(str[i] == '"')
                        betweenquotes = !betweenquotes;
                    else 
                        temp += str[i];
                    
                    i++;
                }

                // if( temp.empty() )
                // {}

                currValue = temp;
                temp.clear();
            }
            
            else if( iswhitespace(str[i]) || str[i] == '>' || str[i] == '/')
            {
                // ignore
            }

            // else
            // {}

            if( ! ( currKey.empty() && currValue.empty() ) )
                node->attributes[currKey] = currValue;
                
            currKey.clear(); 
            currValue.clear();

            if(str[i] == '>')
                return isautoclosing( node->tag() ) ;

            if(str[i] == '/' && str[i+1] == '>')
                return true;
        }   

        return false;
    }

    dom::Node* parseHTML(const std::string& str)
    {

        dom::Node* root = new dom::Node();
        root->setTag("root");

        dom::Node* currNode = root;

        std::string temp = "";

        size_t i = 0;
        while ( i < str.size() )
        {
            if( iswhitespace(str[i]) && temp.empty())
            {
                i++;
                continue;
            }

            if(str[i] == '<')
            {
                // comment

                if( startwith("<!--", str, i) )
                {
                    while( ! startwith("-->", str, i) )
                        i++;

                    i += 3;

                    continue;
                }

                // DOCTYPE

                if( str[i+1] == '!')
                {
                    // ignored for the moment

                    while(str[i] != '>')
                        i++;

                    i++;
                    continue;
                }

                // classic node

                if( ! temp.empty() )
                {
                    dom::Node* textNode = new dom::Node(currNode);
                    textNode->setTag("text");
                    textNode->setValue(temp);

                    temp.clear(); // j'adore
                }

                if(str[i+1] == '/') // end node
                {
                    i += 2;
                    while(str[i] != '>')
                        temp += str[i++];

                    if(temp != currNode->tag())
                    { 
                        std::cerr << "Missmatching" << std::endl;
                        delete root;
                        return nullptr;
                    }

                    temp.clear();

                    currNode = currNode->parent();
                    i++;
                }
                else 
                {
                    dom::Node* node = new dom::Node(currNode);

                    i++;
                    while(str[i] != '>' && str[i] != ' ')
                        temp += str[i++];

                    // if( temp.empty() )
                    // {}

                    node->setTag(temp);
                    temp.clear();

                    bool isautoclosing = parseAttributes(i, str, node);

                    if( !isautoclosing )
                    {
                        currNode = node;
                    }

                    i++;
                }
            }
            else
            {
                temp += str[i++];
            }
        }

        return root;
    }

} // namespace html