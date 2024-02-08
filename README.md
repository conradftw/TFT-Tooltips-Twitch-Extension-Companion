
# TFT Tooltips Companion App

The TFT Tooltips Companion App is the essential bridge between the TFT game you are streaming and the interactive experience provided by the TFT Tooltips Twitch Extension. By installing this application on your computer, viewers can interact with your stream as if they were the ones playing the game! 

Here is a video showing the extension in action: https://www.youtube.com/watch?v=1kJyM_D6no0

## How it works

The TFT Tooltips Companion App works behind the scenes while you are streaming, reading live TFT game data from memory. This data is then ultimately sent to the Twitch extension, enabling real-time interaction for viewers. As viewer's hover over elements on the screen, the extension responds by displaying informative tooltips relevant to the in-game action. If you want a more technical explanation on how the whole project works, click here.

Note: the TFT Tooltips Companion uses "passive memory reading" in order to get live data. Apps like Blitz.gg, Overwolf, and others also use this technique to get data that the Riot API does not provide. Riot's policy on passive memory reading is that it is allowed as long as no competitive advantage is provided. The TFT Tooltips Companion App is only sending data based on the streamer's current POV, thus providing no competitive advantage for the user. Riot is aware of this project and has said they are okay with me continuing work on this project. That said, the TFT Tooltips Companion App is not affiliated with Riot and Riot's stance on memory reading may change at any time so use this project at your own risk.

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

## Technical details: How the whole project works (🤓)

### Project Overview:

The TFT Tooltips project consists of the three components: the TFT Tooltips Companion app that is run alongside a streamer’s TFT game, the server that authenticates and processes gamestate data, and the TFT Tooltips Twitch Extension that is overlayed onto a TFT stream. 

Here is a data flow diagram showing the three components
![data-flow](https://i.imgur.com/W2tjEry.png    "data-flow")

### TFT Tooltips Companion App

Since a Riot API for accessing live TFT game data does not exist, the TFT Tooltips Companion App utilizes memory reading to retrieve real-time info, such as:

-   Traits on the current POV
-   What units are in the shop for the current POV
-   What units are on the current POV. For each unit in this list, also read their name, position coordinates, health, mana, and other information
    

This gamestate data is sent to a server where it will be validated before being forwarded to Twitch Pubsub.

#### LView64

My project uses the LView64 project as a base. The MemoryReader structure, WorldtoScreen function, as well as how the app hooks into League of Legends is based on how LView64 did it. The boost libraries and all scripting functionality was removed and the ImGui overlay setup was modified. If anyone wants a much more complete reference for a C++ League of Legends Memory Reading project, I highly recommend reading LView64 to see how it works (link at the bottom).

#### TFT Memory Reading and Offsets

So this section is an intro to memory reading for beginners, written by a beginner, and is based on what I learned these past couple of months. I’m gonna include links at the bottom of this README to many of the resources I used to learn about memory reading and reverse engineering, (some are League/TFT specific, some aren’t) so if anyone reading is interested in learning more, check out those links. Also if anyone notices mistakes in what I wrote, please let me know so I can correct it, thanks.

---

How does memory reading work? The idea is when you run TFT, the game will get loaded into memory at some base address with a consistent memory layout. This memory layout follows well-defined conventions determined by Windows and decides things like where program instructions are stored in memory and also, importantly to us, where the data/variables used in the program are stored. At the specific section of memory for data/variables, you can find things like your local player object, a list of what traits you have active, what units are in your shop, and basically every other data structure needed for your TFT game to work. 

For example: if you use a memory scanner to look at TFT’s memory when its running, you might see the base address for TFT is at 0x100000 and the address for the local player object is at 0x150000. Now say you restart your game and look again, this time TFT’s base address might be 0x200000 and the player’s gold address is 0x250000. So you can see the actual addresses can change on each launch, but what remains the same is the distance between the two addresses (0x50000 bytes). Remember, when TFT is loaded into memory at some base address, its memory layout is consistent, meaning if you start at TFT's base address and add 0x50000 bytes, you will ALWAYS arrive at the address for the local player object.

This 0x50000 value is known as a memory offset (offset from some other address), and is how you can read TFT’s memory.

So now the problem becomes how to find an offset for a value I’m interested in? For example, I want to read live TFT game data, so I want to find an offset for the player’s shop, for the player’s traits, for the units on board, and more.

The first thing to do is check if offsets for these values and structures already exist. TFT is a gamemode inside League of Legends, and there is an active “““reverse engineering””” (xdd) community for League of Legends online. Many of the offsets I needed, for example units in game and the camera pov, have already been found for League, and it just so happens that TFT uses these offsets as well since it's a part of League. However, the reverse engineering community for TFT specific things is very small, so there weren’t any public TFT offsets shared online, so I had to find these on my own.

In order to find these offsets, you have to do some reverse engineering. There are two types of reverse engineering I learned about: static analysis and dynamic analysis.

Static analysis usually refers to looking at a program’s assembly code to figure out how it works. To do this for TFT, you would use a process dumper to dump TFT’s memory as it’s running, into an binary format, then use a disassembler program (like IDA or Ghidra) to disassemble the binary file into assembly, which you can now simply read (lol) to figure out how TFT works!

Except not really, because static analysis is incredibly difficult and way easier said than done. I was actually very lucky that to get most of my project working, I was able to use dynamic analysis instead of static, because to reverse engineer TFT using static analysis takes a skillset that I am currently still working on as a beginner.

Here are some things you need to know to be good at to perform static analysis:

-   You need to understand how a program is converted from high-level code to assembly code to machine code, and the different sections of a program (.text, .data, .bss, heap, stack)
-   You need to understand the x86/x64 assembly language. So stuff like common x86 instructions (MOV, ADD, LEA, etc), the common registers and what they are usually used for (RAX/EAX, RBX/EBX, etc), the common x86 calling conventions (cdecl, stdcall, fastcall) which decide how arguments are pushed to the stack and who cleans up the stack for a function call.
-   You need to be able to identify what common C program structures and statements look like in assembly. So you need to be able to recognize what a loop looks like in assembly, when you’re iterating an array, when a function is being called or cleaned up, etc.
    

And so much more. Static analysis really requires a deep understanding of low-level programming concepts, and a lot of time and patience that I unfortunately don't have atm. 

Luckily for me, I didn’t have to use static analysis to find my most of the offsets I wanted; I used dynamic analysis, which is basically inspecting a program’s memory as it is running using some sort of memory scanner, to find offsets. I used ReClass and Cheat Engine, which both have memory scanning capabilities, but Cheat Engine is what I used mainly for scanning. ReClass is really great for inspecting an entire region of memory whereas Cheat Engine has more features for finding pointers and offsets, like pointer scanning; links at the bottom for how to set those up.

One of the “Hello World” examples for dynamic analysis is usually finding the address for your player’s health using Cheat Engine. What you do is you scan for your player’s inital health, which can returns thousands of results. Then you take some damage, and do another scan for the new value. You repeat this process until you are left with only a couple of addresses. [Here is a youtube video](https://www.youtube.com/watch?v=YaFlh2pIKAg) showing the concept.

If you’re lucky, you might end up with a green static address that doesn’t change when the game restarts. However most of the time, you will be left with dynamic addresses, that change everytime the program is restarted. What you do here is you use Cheat Engine’s pointer scan functionality, which is basically a brute forces all pointer paths to the address you are searching for. Here is an example of a pointer scan result, showing all the pointer paths starting from the TFT base address that lead to the address we are looking for. What you do now is restart the game and do the same process of finding the player health address. Once you have narrowed down the list of addresses to only a couple, use the pointer scan result and search for these new addresses. If you find a pointer path to this new address, you have most likely found a consistent offset you can use to read this value. [Here is an screenshot of one of my pointer scan results.](https://i.imgur.com/dol6aJs.png) In a previous patch when this offset worked, I was able to follow this pointer chain to the value I wanted consistently.

So this is the process I followed for finding the displayed traits as well as the units in shop. For the shop, I scanned for a unit’s cost, rerolled, then scanned for the new cost. For the traits, I did it with the active trait count. When I was left with a few addresses, I pointer scanned for these addresses and after restarting my game multiple times, I eventually found consistent offsets for the TFT structures I was interested in.

However, not everything can be found using dynamic analysis. For example, one offset I really wanted to find was the equipped items on a unit. However, dynamic analysis was ill-suited for this purpose since there wasn’t a clear value to scan for.

The static analysis approach would be to see the instructions that happen when an item is equipped onto a unit, and then read the code around those instructions to see how it works. However, like I mentioned earlier, I currently don’t have the skillset needed to do this, so I wasn’t able to find the equipped items offset rip.

----------

So what game data did I read from memory? Here are some of the offsets I used for the project and what data I used them to read:

-   “LocalPlayer” - Not actually used, but contains information about the player.
    
-   “ViewProjMatrices” - The “World to Screen” matrix used by League of Legends to convert 3D in-game coordinates into a 2D screen coordinate. I used this Matrix to draw a bounding box around each unit on screen which is how mouse clicks are detected.
    
-   “HeroList” - Not actually used, but contains a list of the 8 players in the current game.
    
-   “MinionList” - A list of “minions” in the current game. This list contains most of the game objects I’m interested in since TFT units and items are both considered minions, so all units on board and bench can be found here.
    
-   “HudInstance_Camera” - The camera object, used to figure out where the player is currently looking, which is how I filter what units are on the current screen
    
-   Renderer - Used to get the current resolution of the player’s game
    
-   Obj<Field> - For each unit on the current screen, I use these Obj-prefixed sub-offsets to get a certain attribute from the unit. Such as the unit’s name, health, mana, attack damage, etc.
    
-   BuffManager - BuffManager contains the list of buffs a unit has. I wasn’t able to find a star level offset for units. So what did end up working is reading their buffs, turns out each unit has a “tft_unitbuff_starX” buff corresponding to their star level.
    
-   CharData - Each unit has a CharData section that lists that unit’s base stats as well as their health bar height, which is what I used for the bounding box height
    
-   HudShopBase - This is the shop shown on screen containing info like the 5 units in shop, their costs, and other stuff like the current roll cost. This offset works for both when a streamer is playing TFT as well as using the spectate function. So if they are playing TFT, this will only show their shop, but if they were spectating a friend, this shop will update when they change POVs.
    
-   StageStr - This is a string showing what stage and round the game is at. I didn’t play old TFT but apparently back then instead of stage and round it was just one number, so maybe I could try searching for that number instead of reading a string (if that is how it still works behind the scenes)
    
-   TraitsList - This list of traits corresponds to the traits a player has, sorta… This structure always has 18 pointers to trait objects, even if the player doesn’t have 18 traits. So if the player has 3 traits in (Disco, Guardian, Dazzler), then the first 3 pointers in the list will point to the Disco, Guardian, and Dazzler trait objects. However, the next 15 traits will just be repeats of the Dazzler trait object. I tried looking for a “real length” value that tells me what traits in this list are actually valid, but I wasn’t able to find them… What this means then is I don’t actually know how many traits a player has in at any time. I just know that if the player X traits in, then the first X pointers in this list will be pointers to those traits. Another problem is that in Set 10, Riot added a “next page” button for traits, so if a player has more than 9 traits in, then they can press the button to see the remaining traits. However, I wasn’t able to find a boolean or another value to indicate when the player clicked this button, so that means I don’t know when the 9 traits being displayed changed to the second page’s traits. So at the moment, I am only sending the first 9 traits from the list, since I noticed majority of the TFT streamers I watch don’t click that button usually lol. So yeah I would prefer to find better offsets in the future, but this is the best I got for now.
    

So this a quick overview of the data I am reading. Most of the data read is ready to be sent to the server, but one modification needed is to add a bounding box for each unit, which looks something like this.

This bounding box is how the Twitch extension knows whether a unit is being clicked or not, and is roughly estimated using the unit’s health bar height and a constant gameplay radius of 65 units ([https://leagueoflegends.fandom.com/wiki/Size](https://leagueoflegends.fandom.com/wiki/Size) ). I’m using a value of 65 units because most champions in League of Legends have a gameplay radius of 65 units and also during my testing, I found 65 units works for basically every unit in Set 10; however if users report clicks not registering, then I can always make every unit have a custom radius radius.

Anyways to represent the box, i need two points, the top-left corner of the box and the bottom-right corner of the box. 

To create the top-left corner of the box, I start from the center of the unit *in-game*,
1. Go towards the -X axis by (65 units * boundingBoxMultiplier) 
2. Then go up towards +Y by the unit’s (health bar height * boundingBoxMultiplier). 
3. Z usually doesn't change unless it's carousel

To create the bottom-right corner of the box:
4. Go towards the +X axis by (65 units * boundingBoxMultiplier) 
5. Y is the Y on the ground
6. Z usually doesn't change unless it's carousel

These 3D coordinates now gets translated to a screen coordinate using the game’s View-Projection matrix, which will give me a pair of 2D coordinates on the screen.

What is the `boundingBoxMultiplier` value? So units in TFT can increase in size in a variety of ways. They naturally get bigger when they are starred-up (so 3 star unit > 2 star unit > 1 star unit), they gain size if they have Titan's equipped with max stacks and they can have up to three of these equipped, and also some units like Chogath from Set 9 also gain size from their ultimate. So this boundingBoxMultiplier takes into account their natural size gain from starring up as well as the unit's scale value read from memory. I found multiplying the box height and width with this multiplier helps the box match the unit's size increase.

Finally, with the bounding box calculated, I can package this memory snapshot into a json file using to nlohmann json library to send to the server. [Here is a pastebin example of a memory snapshot](https://pastebin.com/ZsYw444V) json file (I removed all units but one to make it easier to read).

### TFT Tooltips Twitch Extension

The actual Twitch extension can be found here in its own repo, which contains a write-up on how the twitch extension uses this live game data to display tooltips.

https://github.com/conradftw/TFT-Tooltips-Twitch-Extension-Frontend/blob/main/README.md

### Links and References 

Great intro guide to reversing League - https://www.unknowncheats.me/forum/league-of-legends/567734-beginner-tutorial.html

WorldToScreen and vector spaces - https://www.codereversing.com/archives/530

Undetected Cheat Engine - https://www.unknowncheats.me/forum/anti-cheat-bypass/504191-undetected-cheat-engine-driver-2023-bypass-anticheats-eac.html

Signatures and pattern scannig - https://www.youtube.com/watch?v=wTk6iDzv15Q

x86 instructions - https://www.felixcloutier.com/x86/index.html



