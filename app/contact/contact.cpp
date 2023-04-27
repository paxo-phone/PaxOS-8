#include "contact.hpp"

void Contact::loadContacts(bool force)
{
    storage::LFile file("apps/contact/contact.txt", storage::OPEN_MODE::READ);
    std::string data = file.read();
    file.close();

    std::vector<OneContact>().swap(contacts);

    uint i = 0;

    while(data.find("\"", i) != -1)
    {
        OneContact newContact;
        i = data.find("\"", i);
        newContact.name = data.substr(i+1, data.find("\"", i+1) -i-1);
        i = data.find(":", i);
        newContact.number = data.substr(i+1, data.find("\n", i+1) -i-2);
        contacts.push_back(newContact);
    }
}

void Contact::saveContacts()
{
    std::string buffer = "";

    for (int i = 0; i < contacts.size(); i++)
    {
        buffer+="\""+contacts[i].name+"\":"+contacts[i].number+";\n";
    }

    storage::LFile file("apps/contact/contact.txt", storage::OPEN_MODE::WRITE);
    file.write(buffer);
    file.close();

    loadContacts(true);
}

void Contact::main()
{
    showContact(contactPage());
}

uint Contact::contactPage()
{
    Window win("contact");
    win.setMarginX(0);
    win.setMarginY(CONTROL_BAR_SIZE);

    loadContacts();

    Label* label = new Label(35, 12, 165, 33, "Contacts");
        label->enabledBackground=true;
        label->setHorizontalAlignment(CENTER_ALIGNMENT);
        label->setBorderSize(0);
        label->setRadius(0);
        label->fontHeight=30;
        win.addChild(label);

    std::vector<Gui*> contactList;

    for (int i = 0; i < contacts.size(); i++)
    {
        Label* label = new Label(35, 71 + i*43, 220, 25, contacts[i].name);
        label->enabledBackground=true;
        label->setBorderSize(0);
        label->setRadius(0);
        label->fontHeight=20;
        label->bold=true;
        contactList.push_back(label);
        win.addChild(label);
    }

    Image* add = new Image("apps/contact/add.png", 272, 13, 25, 25);
    add->load();
    win.addChild(add);

    while (true)
    {
        win.updateAll();

        for (int i = 0; i < contactList.size(); i++)
        {
            if(contactList[i]->isTouched())
            {
                return i;
            }
        }

        if (add->isTouched())
        {
            editContact(true);
        }

        if(home_button.pressed())
        {
            return -1;
        }
    }
}

void Contact::showContact(uint index)
{
    Window win("new contact");
    win.setMarginX(0);
    win.setMarginY(CONTROL_BAR_SIZE);

    Label *name = new Label(75, 59, 210, 38, contacts[index].name);
    name->enabledBackground=false;
    name->setTextColor(COLOR_BLACK);
    name->fontHeight=20;
    name->bold = true;
    win.addChild(name);

    Label *number = new Label(75, 119, 210, 28, contacts[index].number);
    number->enabledBackground=false;
    number->setTextColor(COLOR_BLACK);
    number->fontHeight=20;
    number->bold = true;
    win.addChild(number);

    Image* contactImage = new Image("apps/contact/con_name.png", 45, 66);
    contactImage->load();
    win.addChild(contactImage);

    Image* numImage = new Image("apps/contact/con_num.png", 45, 121);
    numImage->load();
    win.addChild(numImage);

    Button* edit = new Button(70, 187, 180, 53, "Edit");
    win.addChild(edit);

    Image* back = new Image("system/return.png", 17, 6); back->load(); win.addChild(back);

    while (true)
    {
        win.updateAll();

        if(back->isTouched())
            return;

        if(edit->isTouched())
        {
            editContact(false, index);
        }

        if(home_button.pressed())
        {
            return;
        }
    }
}

void Contact::editContact(bool create, uint index)
{
    Window win("new contact");
    win.setMarginX(0);
    win.setMarginY(CONTROL_BAR_SIZE);

    Label *name = new Label(75, 59, 210, 38, (create)?(""):(contacts[index].name));
    name->setBackgroundColor(COLOR_LIGHT);
    name->enabledBackground=true;
    name->setTextColor(COLOR_BLACK);
    name->fontHeight=20;
    name->bold = true;
    win.addChild(name);

    Label *number = new Label(75, 119, 210, 28, (create)?("+33"):(contacts[index].number));
    number->setBackgroundColor(COLOR_LIGHT);
    number->enabledBackground=true;
    number->setTextColor(COLOR_BLACK);
    number->fontHeight=20;
    number->bold = true;
    win.addChild(number);

    Image* contactImage = new Image("apps/contact/con_name.png", 45, 66);
    contactImage->load();
    win.addChild(contactImage);

    Image* numImage = new Image("apps/contact/con_num.png", 45, 121);
    numImage->load();
    win.addChild(numImage);

    Button* saveButton = new Button(70, 187, 180, 53, "Save Contact");
    win.addChild(saveButton);

    Keyboard *kb = new Keyboard();
    win.addChild(kb);

    while (true)
    {
        win.updateAll();

        if(win.isTouched())
            kb->link(nullptr);
        if(name->isTouched())
            kb->link(name);
        if(number->isTouched())
            kb->link(number);

        if(saveButton->isTouched())
        {
            OneContact n;
            n.name = name->getText();
            n.number = number->getText();

            if(create)
            {
                contacts.push_back(n);
            }else
            {
                contacts[index] = n;
            }

            saveContacts();

            return;
        }

        if(home_button.pressed())
        {
            return;
        }
    }
}