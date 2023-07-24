#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <vector>

class Contact : public App
{
    public:
    void main();
    static void loadContacts(bool force = false);
    static void saveContacts();
    static uint contactPage();
    static void editContact(bool create = false, uint index = -1);
    static void showContact(uint index);

    struct OneContact
    {
        std::string name;
        std::string number;
    };

    static std::vector<OneContact> contacts;
};

std::vector<Contact::OneContact> Contact::contacts = {};

#endif