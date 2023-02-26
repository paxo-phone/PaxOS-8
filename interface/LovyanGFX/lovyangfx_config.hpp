class LGFX : public lgfx::LGFX_Device
{
  lgfx::Panel_ILI9488     _panel_instance;
  lgfx::Bus_SPI       _bus_instance;   // SPIバスのインスタンス
  lgfx::Light_PWM     _light_instance;
  lgfx::Touch_XPT2046          _touch_instance;

public:
  LGFX(void)
  {
    {
      auto cfg = _bus_instance.config();

      cfg.spi_host = VSPI_HOST;
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000;
      cfg.freq_read  = 16000000;
      cfg.spi_3wire  = true; // changed
      cfg.use_lock   = true;
      cfg.pin_sclk = 18;
      cfg.pin_mosi = 23;
      cfg.pin_dc   = 2;
     
     
     _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();

      cfg.pin_cs           =    5;
      cfg.pin_rst          =    -1;
      cfg.pin_busy         =    -1;

      cfg.panel_width      =   320; 
      cfg.panel_height     =   480; 
      cfg.offset_x         =     0; 
      cfg.offset_y         =     0; 
      cfg.offset_rotation  =     0; 
      cfg.dummy_read_pixel =     8; 
      cfg.dummy_read_bits  =     0;//1 
      cfg.readable         =  true; 
      cfg.invert           = false; 
      cfg.rgb_order        = false; 
      cfg.dlen_16bit       = false; 
      cfg.bus_shared       =  true; 

      cfg.memory_width     =   320; 
      cfg.memory_height    =   480; 

      _panel_instance.config(cfg);
    }
    {
      auto cfg = _light_instance.config();

      cfg.pin_bl = -1;
      cfg.invert = false;
      cfg.freq   = 44100;
      cfg.pwm_channel = 25;

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);  // バックライトをパネルにセットします。
    }
    {
      auto cfg = _touch_instance.config();

      cfg.x_min      = 0;    // タッチスクリーンから得られる最小のX値(生の値)
      cfg.x_max      = 319;  // タッチスクリーンから得られる最大のX値(生の値)
      cfg.y_min      = 0;    // タッチスクリーンから得られる最小のY値(生の値)
      cfg.y_max      = 479;  // タッチスクリーンから得られる最大のY値(生の値)
      cfg.pin_int    = -1;   // INTが接続されているピン番号
      cfg.bus_shared = true; // 画面と共通のバスを使用している場合 trueを設定
      cfg.offset_rotation = 0;// 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定

      cfg.spi_host = VSPI_HOST;// 使用するSPIを選択 (HSPI_HOST or VSPI_HOST)
      cfg.freq = 1000000;     // SPIクロックを設定
      cfg.pin_sclk = 18;     // SCLKが接続されているピン番号
      cfg.pin_mosi = 23;     // MOSIが接続されているピン番号
      cfg.pin_miso = 19;     // MISOが接続されているピン番号
      cfg.pin_cs   =  21;     //   CSが接続されているピン番号

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);  // タッチスクリーンをパネルにセットします。
    }

    setPanel(&_panel_instance); // 使用するパネルをセットします。
  }
};