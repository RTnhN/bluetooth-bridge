# bluetooth-bridge

![Demo](https://github.com/RTnhN/bluetooth-bridge/blob/main/doc/demo.gif)

This repo uses an arduino as a bridge device to an iphone so that I can type on my computer and use it as a input device for my phone. I use Reflector 4 as an airplay receiver, so this allows me to type stuff on my phone on my monitor. Since I only want to type stuff into my phone while engaging with my phone , this checks if the Reflector 4 window is selected. 

Hardware wise, I use an ESP32. The version that I use is the HiLetgo wifi with a screen that is totally overkill for the project, but it is all I had around for now. You could probably find one that plugs directly into your computer. 

For the arduino code, you will need to install [this ESP32 BLE library](https://github.com/T-vK/ESP32-BLE-Keyboard) for your arduino IDE.

When connecting to my phone with the bluetooth keyboard, I could not actually connect to it using the normal bluetooth connection menu. I needed to download an app that inspected the bluetooth connections. This was able to connect to the ESP32 and receive the keyboard commands. Specifically, the app is called [Bluetooth Inspector](https://apps.apple.com/cm/app/bluetooth-inspector/id1509085044?l=en). 




