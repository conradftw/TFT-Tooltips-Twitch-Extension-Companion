
# TFT Tooltips Companion App

The TFT Tooltips Companion App is the essential bridge between the TFT game you are streaming and the interactive experience provided by the TFT Tooltips Twitch Extension. By installing this application on your computer, viewers can interact with your stream as if they were the ones playing the game! 

## How it works

The TFT Tooltips Companion App works behind the scenes while you are streaming, reading live TFT game data from memory. This data is then ultimately sent to the Twitch extension, enabling real-time interaction for viewers. As viewer's hover over elements on the screen, the extension responds by displaying informative tooltips relevant to the in-game action. If you want a more technical explanation on how the whole project works, click here.

Note: the TFT Tooltips Companion uses "passive memory reading" in order to get live data. Apps like Blitz.gg, Overwolf, and others also use this technique to get data that the Riot API does not provide. Riot's policy on passive memory reading is that it is allowed as long as no competitive advantage is provided. The TFT Tooltips Companion App is only sending data based on the streamer's current POV, thus providing no competitive advantage for the user. 

That said, the TFT Tooltips Companion App is not affiliated with Riot and Riot's stance on memory reading may change at any time.

## Setup 
0. Requirements:
  - Windows 10. Mac and Linux not supported. Windows 11 hasn't been tested but might work?
  - TFT game MUST be 1920x1080 resolution. This app will not fully work with other resolution at this time.

1. Add the TFT Tooltips Extension to your channel by:
	- Creator Dashboard -> Extensions (on the left) to go to the Extensions Discovery page.
	- In the "Search all extensions" search box, type "TFT Tooltips" and you should see it in the results, then click "Add".
	- The extensions configuration page should pop up now. On the bottom right, click "Activate" then "Set as Overlay 1". 
	- If the configuration page did not pop up, then instead click "My Extensions" at the top next to "Discovery". You should now see a list of extensions added to your channel. Find the "TFT Tooltips" extension and then click "Activate" then "Set as Overlay 1". 
	- Now the extension has been enabled for your channel!
2. Sign up for a key for your channel by messaging the developer on Discord. You will be asked to verify that you own this twitch channel. Once verified, the developer will send you a key on Discord; please keep this key private. If you lose or forget your key, you will need to do this step again. 
3.  Download and run the companion app:
	- Go to the [releases page](https://github.com/conradftw/TFT-Tooltips-Twitch-Extension-Companion/releases/) and download the latest zip file. Unzip anywhere on your computer.
	- Double-click on "TFT-Overlay-Companion-App.exe" to run the app
	- Enter the key you got from step 2 by copying it and right-clicking anywhere on window. If it says "Login token has been validated" then you are all done! The Companion App will automatically detect when a TFT game is running and send live data to the Twitch Extension. 
		- If you see a different message, please make sure you have the right key and that entered it correctly. If this problem persists, create a ticket on the extension Discord.
	- The companion app has to be running for the Twitch extension to work. If you are not streaming, you can simply close this app. You will have to re-enter your key every time you open the app.
 
## Features

1. Hover over the traits on screen to get that trait's description, effects, and units with that trait. Unhover to close the tooltip.
2. Hover over the units in shop to see that unit's ability description. Unhover to close the tooltip.
3. LEFT-CLICK on a unit on screen to a get tooltip containing the following info for that unit (click anywhere else on the screen to hide the tooltip):
	- Name
	- Star level
	- Traits (Hovering a trait will show that trait's tooltip)
	- Cost
	- Current and total health
	- Current and total mana
	- Ability (Hover the ability icon to show the ability's description and scalings + calculations)
	- Positioning
	- Range
	- Stats: Attack Damage, Ability, Armor, Magic Resistance, Attack Speed, Crit Chance, and Crit Damage. Hover any of these stats to get their base, bonus, and total values. 

	Currently, not everything that is right-clickable ingame can be left-clicked in the extension. The following things are supported for left-clicking:
	- All buyable units
	- Hecarim
	- Target Dummy

[Here is a video demo showing the current features.](https://www.youtube.com/)

Feature screenshots:
1. Traits
![TraitList](https://i.imgur.com/ZgsEeKu.png "TraitList")

2. Shop unit ![shop](https://i.imgur.com/0Z0Kbe7.png "shop")

3 Unit tooltips 
- Unit tooltip
![unit](https://i.imgur.com/kj6hTJN.png "unit")

- Unit Ability
![ability](https://i.imgur.com/3DvWJZH.png  "ability")

- unit traits
![unit traits](https://i.imgur.com/nhyR6HW.png  "unit traits")

- unit stats
![unit stats](https://i.imgur.com/sWUu90A.png  "unit stats")

## Discord 
If you have any questions, comments, bug reports, etc, please use the Extension Discord server and I'll get back to you when I can.
- Extension Discord: [invite link](https://discord.gg/8Mq2tUuZEb)
- You can also email <tftooltips@hotmail.com> for support (slower response time than discord).

## Technical details: how the whole project works (🤓)
sooon tm
