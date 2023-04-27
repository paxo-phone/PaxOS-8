#ifndef MESSAGE_HPP
#define MESSAGE_HPP

class Message : public App
{
    public:
    void main()
    {
        Window win("message");

        

        while (true)
        {
            win.updateAll();
        }
    }
};

#endif