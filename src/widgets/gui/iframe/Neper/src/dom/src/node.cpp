#include <iostream>

#include "../include/node.hpp"

namespace dom
{

    Node::Node() : m_tag(""), m_value(""), m_parent(nullptr)
    {

    }

    Node::Node(Node* parent) : m_tag(""), m_value("")
    {
        if(parent != nullptr)
            parent->addChild(this);
    }

    Node::~Node()
    {
        for(size_t i = 0; i < this->m_children.size(); i++)
        {
            if(m_children[i] != nullptr)
                delete m_children[i];
        }
    }

    // children

    Node* Node::child(size_t i)
    {
        return this->m_children[i];
    }

    void Node::addChild(Node* child)
    {
        this->m_children.push_back(child);
        child->m_parent = this;
    }

    size_t Node::children(void)
    {
        return this->m_children.size();
    }

    // tag

    void Node::setTag(const std::string& tag)
    {
        this->m_tag = tag;
    }

    void Node::setTag(const char c)
    {
        this->m_tag = c;
    }

    std::string Node::tag(void)
    {
        return this->m_tag;
    }

    // value

    void Node::setValue(const std::string& value)
    {
        this->m_value = value;
    }

    void Node::setValue(const char c)
    {
        this->m_value = c;
    }

    std::string Node::value(void)
    {
        return this->m_value;
    }

    // parent

    Node* Node::parent(void)
    {
        return this->m_parent;
    }

    bool Node::hasParent(void)
    {
        return ( parent() != nullptr );
    }

    bool Node::isRoot(void)
    {
        return ( ! hasParent() );
    }

    // others

    Node* Node::root(void)
    {
        if(m_parent == nullptr)
            return this;
        
        return m_parent->root();
    }

    // empty 

    bool Node::empty(void)
    {
        return 
        (
            m_tag.empty() &&
            m_value.empty() && 
            m_parent == nullptr && 
            m_children.empty() &&
            attributes.empty() &&
            styles.empty()
        );
    }

    size_t Node::depth(void)
    {
        if( this->parent() == nullptr )
            return 0;
        else
            return (1 + parent()->depth());
    }

    Node* Node::previousSibling(void)
    {
        if( hasParent() )
        {
            for(size_t i = 0; i < parent()->children(); i++)
            {
                if( parent()->child(i) == this )
                {
                    if( i == 0 )
                        return nullptr;
                    else
                        return parent()->child(i-1);
                }
            }
        }

        return nullptr;
    }

    Node* Node::nextSibling(void)
    {
        if( hasParent() )
        {
            for(size_t i = 0; i < parent()->children(); i++)
            {
                if( parent()->child(i) == this )
                {
                    if( i == parent()->children() - 1 )
                        return nullptr;
                    else
                        return parent()->child(i+1);
                }
            }
        }

        return nullptr;
    }
    
    #ifdef DEBUG
        std::ostream& operator<<(std::ostream& os, Node& node)
        {
            std::string spacer = "";

            for(size_t i = 0; i < node.depth(); i++)
            {
                spacer += "|   ";
            }


            // printing attributes

            os << spacer << "Attributes : ";

            os << "(";

            for(auto& ite : node.attributes) {
                os << ite.first << "=" << ite.second << ", ";
            }

            os << ")" << std::endl;

            // printing styles

            os << spacer << "Styles : ";

            os << "(";

            for(auto& ite : node.styles) {
                os << ite.first << "=" << ite.second << ", ";
            }

            os << ")" << std::endl;

            // values

            os << spacer << "Tag : " << node.m_tag << std::endl;
            os << spacer << "Value : " << node.m_value << std::endl;

            // Children

            for(auto& ite : node.m_children)
            {
                std::cout << (*ite) << std::endl;
            }

            return os;
        }
    #endif 


} // namespace dom