## Input Simulation Options

![Input Options UI](images/StayAwakeInputOptions.png)

StayAwake can randomly pick any of the 26 inputs (24 key codes & 2 mouse moves) during each simulation. In this popup, users can specify _at least_ one OR a select few OR all of the 26 inputs to be included in the simulation roster.

StayAwake simulates events using the `SendInput` API instead of the deprecated `keybd_event` API. `SendInput` API allows batching of multiple key or mouse events in a single atomic call to the Windows OS, thus yielding jitter-free key or mouse simulations.

As can be noted from the screenshot, [Zero-pixel Mouse Move](#zero-pixel-mouse-move) and [One-pixel Mouse Jiggle](#one-pixel-mouse-jiggle) are my two preferred input options.

#### Scroll Lock cycling
Scroll Lock toggling will work just fine for most users. However, some users using Remote Desktop alongside have reported spurious keystrokes being transmitted between the connected Windows devices. Users in such situations should explore other key simulation options.

#### Volume Up & Down
With this option, StayAwake will simulate a _Volume Down_ key press, immediately followed by a _Volume Up_ key press, thereby resulting in no net change in volume level.

Volume Up & Down simulation will preserve the muted sound state. Prior to each Volume Up & Down simulation event, StayAwake will detect if the sound is in a muted state or not. If the sound is found muted, Volume Mute key will also be added after the Volume Down & Up keys in the `SendInput` API call. Since all these key events are sent in a single atomic API call, any sound jitter that may still occur will be the briefest possible.

A minor benefit with this option is the visual feedback. Each StayAwake key simulation event will cause the Windows Volume Slider flyout/toast OSD (on-screen display) to show for a few seconds. While this input option is useful for debugging or verification of StayAwake functionality, it will get too distracting over long term.

**Tip**: You can verify StayAwake in action by *temporarily* enabling only the *Volume Up & Down* key code and setting both *Minimum Awake Seconds* and *Maximum Awake Seconds* fields to 10.

#### Unassigned Key (1 to 10)
With these options, StayWake will simulate key press of an unassigned keycode.

These unassigned keycodes have been obtained from this Microsoft article on [Virtual-Key Codes](https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes).

Microsoft may start utilizing any of these unassigned codes in the future -- but not all of them in one go. Hence all 10 of the currently unassigned codes have been included in the [Input Simulation Options](#input-simulation-options) popup so that users can select other still available unassigned keycodes.

See also: [Google Sheets clears active cell content on unassigned Windows virtual-key events](https://github.com/shriprem/StayAwake/discussions/21)

#### Extended Function Keys (F13 to F24)
These 12 options are safe to use since most applications do not bind F13–F24 keys to any action. However, there are some rare exceptions such as:
* Some DAWs (digital audio workstations)
* Some macro software
* Some enterprise kiosk systems
* Some accessibility tools

See also: [Google Sheets clears active cell content on unassigned Windows virtual-key events](https://github.com/shriprem/StayAwake/discussions/21)

#### One-pixel Mouse Jiggle
With this option, StayAwake will simulate a one-pixel mouse jiggle. The mouse pointer will move one pixel diagonally right & down, and then back to its original position. This one-pixel jiggle is jitter-free since the two mouse moves are batched into a single atomic `SendInput` API call. So, the mouse pointer will not seem to move at all with this option.

#### Zero-pixel Mouse Move
With this option, StayAwake will simulate a zero-pixel mouse move. The mouse pointer will not move at all. At least on the few Windows 11 devices that I have tested, this option was found to be remarkably effective. If this option does not work on your Windows device, use the [One-pixel Mouse Jiggle](#one-pixel-mouse-jiggle) option instead.


#### Buttons
* _Select None_ button will deselect all 26 input options.
* _Select All Unassigned_ button will select all 10 Unassigned Key options.
* _Select All Ext. FN Keys_ button will select all 12 Extended Function Key options.
* _Select All_ button will select all 26 input options.
* _Apply_ will save the selected input options and close the popup.
* _Cancel_ will close the popup without saving any changes.

