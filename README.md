# GothicRichPresence

This is [Discord Rich Presence](https://discord.com/rich-presence) made for the video games Gothic I and II made in [Union](https://worldofplayers.ru/threads/40376/).

## Preview

![Preview](https://i.imgur.com/1fBTsKt.gif)

# Usage for regular users

1. Download and install [Union](https://worldofplayers.ru/threads/40376/).
2. Download `GDPR.vdf` file from the [latest release](https://github.com/Lavalierre/GothicRichPresence/releases) and place it in `<GothicPath>\Data\` folder.
3. Remember to add Gothic to games detected by Discord if you haven't already.

# Usage for mod developers

You can easily implement this plugin into your mod. Just download `GothicRichPresence.dll` from the [latest release](https://github.com/Lavalierre/GothicRichPresence/releases) and place it inside your `.mod/.vdf` file in `System/Autorun/` folder.
Now you're ready for setting it up.

## Configuration

**Config is completely optional.** If not set, plugin will just default options but with just few steps you can add your own images or names for worlds in your mod! Add those lines into your **mod.ini**. Plugin reads data from files specific for each mod instead from gothic.ini and for that reason, also player needs to run mod through GothicStarter for this to work. If something with configuration isn't right, plugin will run with default settings.

**Example:**

```
[GothicRichPresence]
Lang = EN
AppKey = 831893873164288031
ConfigFile = worlds.json
```

1. `Lang` option is used to set language of the plugin. **This can be left empty and plugin will detect default system language.** Currently supported values: EN, RU, PL
2. `AppKey` option is used to set up your own [Discord application](#Discord-App).
3. `ConfigFile` option is to point to the config file that your Discord App will use. See more in [RPC Config](#RPC-Config) section.

### Discord App

First you have to create your own Discord application:

1. Log-In to [Discord Developer Portal](https://discord.com/developers/applications) and go to `Applications` section.
2. Click on the `New Application` button and set a name for your new app. This name will be displayed on rich presence and should be named `Gothic I` or `Gothic II`.
   - **Note:** There is no need to putting your mod name there since plugin read `Title` name from your `mod.ini` and displays it in second line of Rich Presence.
3. Copy `Application ID` and paste it in the config file, at the `AppKey` option.

Now your application ready to go and will be used by plugin on the next launch. Now you can upload and set your own images for worlds in [Config File](#Config-File), but some images have constant names, and you need to upload it for proper usage:

- Image with name `menu` will be displayed when player currently is in menu, or in unknown location.
- Image with name `info` will be displayed in the bottom-right corner of your location pic, and it contains information about game time.
- One image for every `zen` in your mod. Those images will be used as a cover when player is in this location.
- Remember to also set `App Icon` in `General Information` section as it is displayed when game is starting.

### Config File

Now you need to create a custom json file, which you can place in the `<GothicFolder>\System\` folder, or right into your `.vdf/.mod` file in the `System\` folder. Example of the config file you can see below:

```json
{
  "worldList": "newworld, oldworld",
  "newworld": {
    "aliases": "{ru} Хоринис, {en} Khorinis, {pl} Khorinis",
    "image": "khorinis_day"
  },
  "oldworld": {
    "aliases": "{en} Valley of Mines, {ru} Долина Рудников, {pl} Górnicza Dolina",
    "image": "gomezdidnothingwrong"
  }
}
```

- `worldList` string contains list of your worlds.
  - Location names must be the same, as the location objects below, and it's **case sensitive**.
- Each location object must have 2 strings: `aliases` and `image`.
  - The `aliases` are your location names. Just write them with "," separator. Tags `{ru}` `{en}` `{pl}` represents which language it is.
  - And `image` is cover image of your location. Copy image name from your Discord application and paste it there.
