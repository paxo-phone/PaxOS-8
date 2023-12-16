#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <vector>
#include "../app.hpp"


class Contact : public CApp
{
public:
    [[nodiscard]] const char* getAppName() const override { return "Contact"; }
    void main() override;
    
    static void loadContacts(bool force = false);
    static void saveContacts();
    static int16_t contactPage();
    static void editContact(bool create = false, int16_t index = -1);
    static void showContact(int16_t index);

    struct OneContact
    {
        std::string name;
        std::string number;
    };

    static std::vector<OneContact> contacts;
};

#endif
