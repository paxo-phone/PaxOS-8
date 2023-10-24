#ifndef __DOM_NODE__
#define __DOM_NODE__

#ifdef DEBUG
    #include <iostream>
#endif

#include <string>
#include <vector>
#include <unordered_map>

#include "attribute.hpp"
#include "attributeList.hpp"

#include "style.hpp"
#include "styleList.hpp"

namespace dom
{
    class Node;

    typedef std::vector<Node*> NodeList;

    class Node
    {
        
        public:

            Node();
            Node(Node* parent);

            ~Node();

            // children 

            Node* operator[](const std::string& selector);

            /* 
                return the ith child of the node
            */
            Node* child(size_t i);
            
            /*
                add child to the node
            */
            void addChild(Node* child);

            /*
                returns the number of children of the node
            */
            size_t children(void);

            // tag

            /*
                Set node tag as a string
            */
            void setTag(const std::string& tag);

            /*
                Set node tag as a char
            */
            void setTag(const char c);


            /* 
                Get node tag
            */
            std::string tag(void);

            // value

            /*
                Set node value as string
            */
            void setValue(const std::string& value);

            /*
                Set node value as char
            */
            void setValue(const char c);


            /* 
                Get node value 
            */
            std::string value(void);

            // parent

            /*
                Get node parent 
            */
            Node* parent(void);

            /*
                Return true if the node has parent
            */
            bool hasParent(void);

            /*
                Return true if the node don't have parent / is root node
            */
           bool isRoot(void);

            // others
            
            /*
                Get root node
            */
            Node* root(void);

            /* 
                Return true if the node is empty
            */
            bool empty(void);

            /*
                Return the distance between the node and the root 
            */
            size_t depth(void);


            /*

            */
           Node* previousSibling(void); 

           /*

           */
            Node* nextSibling(void);


        #ifdef DEBUG
            friend std::ostream& operator<<(std::ostream& os, Node& node);
        #endif

            AttributeList attributes;
            StyleList     styles;

        private:

            std::string m_tag;
            std::string m_value;

            Node* m_parent;
            NodeList m_children;

            
    };

} // namespace dom


#endif /* __DOM_NODE__ */