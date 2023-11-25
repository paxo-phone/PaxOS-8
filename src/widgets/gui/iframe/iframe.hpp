#ifndef IFRAME_HPP
#define IFRAME_HPP

#include <string>

#include "../../gui.hpp"
#include "../label.hpp"
#include "Neper/src/neper.hpp"


class Iframe : public Gui
{
    public:
    
    GUI_TYPE getType() { return IFRAME_TYPE; }

    Iframe();
    Iframe(int16_t x, int16_t y, int16_t width, int16_t height);
    Iframe(const std::string& html, int16_t x, int16_t y, int16_t width, int16_t height);

    void loadhtml(const std::string& html);

    void draw();
    void free(){};

    private:
        
    Gui* build_squeleton( dom::Node* node );
    void set_positions_and_dimensions(Gui* root);


};

Iframe::Iframe() : Gui()
{
    verticalSlide = true;
}

Iframe::Iframe(int16_t x, int16_t y, int16_t width, int16_t height) : Gui(x, y, width, height)
{
    verticalSlide = true;
}

Iframe::Iframe(const std::string& html, int16_t x, int16_t y, int16_t width, int16_t height) : Gui(x, y, width, height)
{
    verticalSlide = true;
    this->loadhtml(html);
}

void Iframe::draw()
{}



inline bool isHiddenTag(const std::string& tag)
{
    return (tag == "area"     || 
            tag == "datalist" || 
            tag == "head"     || 
            tag == "link"     || 
            tag == "param"    || 
            tag == "script"   || 
            tag == "style"    || 
            tag == "title"      );
}

Gui* Iframe::build_squeleton( dom::Node* node )
{
    std::string tag = node->tag();

    if( isHiddenTag(tag) )
        return nullptr;

    Gui* gui;

    if (tag == "text")
    {
        gui = new Label(0, 0, 0, 0, node->value());
    }
    else // default
    {
        gui = new Box(0, 0, 0, 0);
    }

    for(uint8_t i = 0; i < node->children(); i++)
    {
        Gui* guichild = build_squeleton(node->child(i));
        if(guichild != nullptr) 
            gui->addChild( guichild );
    }

    return gui;   
}

void Iframe::set_positions_and_dimensions(Gui* root)
{

    for(auto& child : root->children)
        set_positions_and_dimensions(child);

    /*
        L'algorithme suppose pour le moment que tous les éléments sont en "display: block",
        ce qui signifie que leur largeur est fixée à celle de l'écran et leur hauteur déterminée automatiquement par leur contenu. 
        Cette approximation grossière est évidemment temporaire.
    */


   /* width */

    root->setWidth(this->getWidth()); // setwidth(100%)

    /* height */

    uint16_t height = 0;

    if(root->getType() != LABEL_TYPE)
    {
        for(auto& child : root->children)
            height += child->getHeight();
    }
    else
    {

        /* Il faudrait ici déterminer automatiquement la hauteur du label */
        ((Label*)root)->updateSizes();
        height = ((Label*)root)->getTextHeight();
    }

    root->setHeight(height);

    /* x */

    root->setX(0);

    /* y */

    uint16_t y = 0;

    for(auto& child : root->parent->children)
    {
        if(child == root)
            break;
        
        y += child->getHeight();
    }

    root->setY(y);
}

void Iframe::loadhtml(const std::string& html)
{
    dom::Node* node = html::parseHTML(html);

    /* build squeleton */
    this->addChild(build_squeleton(node));

    /* set_dimensions_and_positions */
    for(auto& child: this->children)
        set_positions_and_dimensions(child);


    delete node;
}

#endif