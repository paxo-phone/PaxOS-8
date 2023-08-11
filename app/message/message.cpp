#include "message.hpp"

#include "../contact/contact.hpp"

void Message::main()
{
    while(!home_button.pressed())
    {
        uint contactIndex = Contact::contactPage();
        if(contactIndex == -1)
            break;

        std::string number = Contact::contacts[contactIndex].number;
        std::string actual_message = "";
        print (number);
        try
        {
            number = number.substr(number.length()-9, 9);
        } catch (std::out_of_range exception) {}


        bool reload = false;

        while(reload==false && !home_button.pressed())
        {
            Window win("message");
            win.setMarginX(0);
            win.setMarginY(CONTROL_BAR_SIZE);

            print("apps/message/" + number + ".txt");

            std::string data = "";
            storage::LFile file("apps/message/chats/" + number + ".txt", storage::OPEN_MODE::READ);
            if(file.is_open())
            {
                data = file.read();
                file.close();
                notif = false;
            }else
            {
                storage::LFile file("apps/message/chats/" + number + ".txt", storage::OPEN_MODE::WRITE);
                file.close();
                notif = false;
            }

            Box* box = new Box(25, 6, 270, 222);
            box->setBackgroundColor(COLOR_LIGHT);
            box->enabledBackground=true;
            box->verticalSlide=true;
            box->setRadius(0);
            win.addChild(box);

            // parser

            u_long k = -1;

            Label* bull = nullptr;

            while (data.find("~M", k+1)!=-1)
            {
                k = data.find("~M", k+1);
                std::string message = data.substr(k+4, data.find("\n", k) - k-4);

                bull = new Label((data[k+2] == 'I')?(0):(79), AUTO, 190, AUTO, message);
                bull->enabledBackground=true;
                bull->setBackgroundColor(COLOR_EXTRA_LIGHT);
                bull->setBorderSize(0);
                bull->setRadius(15);
                bull->hasEvent=false;
                box->addChild(bull);
            }

            box->determineSize();

            Keyboard *kb = new Keyboard();
            win.addChild(kb);

            Label* text = new Label(0, 233, 261, 60, "");
            text->enabledBackground=true;
            win.addChild(text);

            kb->link(text);

            Box* send = new Box(261, 233, 59, 65);
            win.addChild(send);
            Image* img = new Image("apps/message/send.png", 19, 17);
            img->load();
            img->hasEvent=false;
            send->addChild(img);

            bool resetScrool = true;

            text->setText(actual_message);

            while(true)
            {
                win.updateAll();

                actual_message = text->getText();

                if(resetScrool && bull!=nullptr)
                {
                    resetScrool = false;
                    print("box: " + std::to_string(bull->getRelativeY()));
                    if(bull->getRelativeY() > box->getHeight())
                    {
                        print("too much messages, resizing...");
                        box->scroolY = - bull->getRelativeY() - bull->getHeight() + box->getHeight();
                        box->rendered=false;
                    }
                }

                if (home_button.pressed())
                {
                    reload = true;
                    break;
                }

                if(notif)
                {
                    break;
                }

                if (send->isTouched())
                {
                    gsm.sendNewMessageMODE(Contact::contacts[contactIndex].number, text->getText());
                    storage::LFile file("apps/message/chats/" + number + ".txt", storage::OPEN_MODE::WRITE);
                    file.write(data + "~MO:" + text->getText() + "\n");
                    file.close();
                    actual_message="";
                    break;
                }
                #ifdef BUILD_EMU
                    SDL_Delay(20);
                #endif
            }
        }
    }
}

void Message::saveMessage(std::vector<GSM::Message> messages)
{
    for (uint i = 0; i < messages.size(); i++)
    {
        std::string number = messages[i].number.substr(messages[i].number.length()-9, 9);

        storage::LFile filer("apps/message/chats/" + number + ".txt", storage::OPEN_MODE::READ);
        std::string data = filer.read();
        filer.close();

        data += "~MI:" + messages[i].message + "\n";

        storage::LFile filew("apps/message/chats/" + number + ".txt", storage::OPEN_MODE::WRITE);
        filew.write(data);
        filew.close();
    }
    if(messages.size() != 0)
        notif = true;
}
