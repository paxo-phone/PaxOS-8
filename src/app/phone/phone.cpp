#include "phone.hpp"
#include "../../interface/interface.hpp"

void Phone::make_a_call(std::string number)
{
    gsm.makeCall(number);
    during_calling(number);
}

void Phone::during_calling(std::string number)
{
    Window win("calling");

    Label* number_label = new Label(AUTO, AUTO, AUTO, AUTO, number);
    number_label->setFontSize(30);
    number_label->setHorizontalAlignment(CENTER_ALIGNMENT);
    win.addChild(number_label);

    Button* hangOff = new Button(20, 480-50-25-40, 320-80, 50, "hang off");
    hangOff->setBackgroundColor(COLOR_WARNING);
    win.addChild(hangOff);

    while (gsm.callEnded()==false)
    {
        win.updateAll();

        if (home_button.pressed() || hangOff->isTouched())
        {
            gsm.answerCall(false); // hang off
            break;
        }

        home_button.resetStandbyMod();

        #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
            SDL_Delay(20);
        #endif
    }
}

void Phone::get_a_call()
{
    std::string number = gsm.numberiscalling;
    Window win("calling");

    Label* number_label = new Label(AUTO, AUTO, AUTO, AUTO, number);
    number_label->setFontSize(30);
    number_label->setHorizontalAlignment(CENTER_ALIGNMENT);
    win.addChild(number_label);

    Button* hangOff = new Button(10, 480-50-25-40, 130, 50, "hang off");
    hangOff->setBackgroundColor(COLOR_WARNING);
    win.addChild(hangOff);

    Button* hangOn = new Button(320-40-130, 480-50-25-40, 130, 50, "hang on");
    hangOn->setBackgroundColor(COLOR_SUCCESS);
    win.addChild(hangOn);

    while (gsm.callEnded()==false)
    {
        win.updateAll();

        if (home_button.pressed() || hangOff->isTouched())
        {
            gsm.answerCall(false); // hang off
            break;
        }
        if (hangOn->isTouched())
        {
            gsm.answerCall(true);
            during_calling(number);
            break;
        }
        #if defined(__linux__) || defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
            SDL_Delay(20);
        #endif
    }

    //print("exiting");
}
