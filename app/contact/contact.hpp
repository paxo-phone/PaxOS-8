#ifndef CONTACT_HPP
#define CONTACT_HPP

class Contact : public App
{
    public:
    void main()
    {
        Window win("contact");

        

        while (true)
        {
            win.updateAll();
        }
    }
};

#endif