# GothicRichPresence
This is [Discord Rich Presence](https://discord.com/rich-presence) made for the video games Gothic I and II made in [Union](https://worldofplayers.ru/threads/40376/).

### Preview
![Preview](https://i.imgur.com/1fBTsKt.gif)

### Usage
- Download and install [Union](https://worldofplayers.ru/threads/40376/)
- Download `GDPR.vdf` file from the [latest release](https://github.com/Lavalierre/GothicRichPresence/releases) and place it in `<GothicPath>\Data` folder
- Remember to add Gothic to games detected by Discord if you haven't already

### Modding support

You can easily implement this plugin into your mod. Just download `GothicRichPresence.dll` from the [latest release](https://github.com/Lavalierre/GothicRichPresence/releases) and place it in your `.mod/.vdf` file in `System/autorun` folder.
Now you're ready for setting it up.

## Config file

Config file section is completely optional, but you can change it by your own. Just write this section in your mod config file in any place.

![Config](https://i.imgur.com/9ZNG9zg.png)

1. `Lang` option is used to set language of the plugin. Currently supported values: EN, RU, PL
2. `AppKey` option is used to set up your own Discord application (for more info check the "Discord App" section)
3. `RPCConfigFile` option is uset to set up your own RPC config file, whice contains custom location names and images (for more info check the "RPC Config" section)

## Discord App

You can create your own Discord application with your custom location images. How to:

1. Log-In to [Discord Developer Portal](https://discord.com/developers/applications) and go to "Applications" section
2. Click on the `New Application` button and set a name for your new app
3. Copy `Application ID` and paste it in the config file, at the `AppKey` option

Now your application ready to go and will be used by plugin on the next launch. You can also upload and set your own location images (check the `RPC Config` section), but some images have constant names and you need to upload it for proper usage:

- Image with name `menu` will be displayed when player currently is in menu, or in the unknown location
- Image with name `info` will be displayed at the bottom-right corner of your location pic, and it contains information about game time

## RPC Config

RPC Config is a custom json file, which you can place in the `<GothicFolder>\System\` folder, or right into your .vdf/.mod file in the `System\` folder. Example of the config file you can see below:

![Config](https://i.imgur.com/IWf8TQL.png)

- `worldList` string contains list of your world. NOTE: location names must be the same, as the location objects below, and it's case sensitive.
- Each location object must have 2 strings: `aliases` and `image`.
- `aliases` is your location names. Just write them with "," separator. Tags `{ru}` `{en}` `{pl}` represents which language it is.
- `image` is your location image. Copy image name from your Discord application and paste it there.
