# StayAwake

![GitHub](https://img.shields.io/github/license/shriprem/StayAwake)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/shriprem/StayAwake)
![GitHub all releases](https://img.shields.io/github/downloads/shriprem/StayAwake/total)
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
![GitHub last commit (by committer)](https://img.shields.io/github/last-commit/shriprem/StayAwake)
![GitHub Workflow Status (with event)](https://img.shields.io/github/actions/workflow/status/shriprem/StayAwake/CI_build.yml)
![GitHub issues](https://img.shields.io/github/issues/shriprem/StayAwake)

[Current Version: 1.0.0.2](https://github.com/shriprem/StayAwake/blob/main/VersionHistory.md)

StayAwake is a simple utility that enables you to maintain an _Active_ status on Microsoft Teams (and perhaps other such messaging applications).

If you are using Microsoft Teams, you may have noticed that Teams automatically sets your status to _Away_ after a certain period of inactivity. This can be frustrating if you are still at your desk but perhaps on a business phone call _OR_ outlining a plan on paper _OR_ consulting some reference material.

StayAwake is a lightweight application that can be minimized to the Windows System Tray, and will continue to run in the background until you click the _Exit_ button in the application.

StayAwake does its job by periodically cycling  the _Scroll Lock_ button (_i.e._, toggling once, and then immediately toggling again to restore _Scroll Lock_ back to its original state). Since the _Scroll Lock_ button is not used by most applications or users, this is a safe and non-intrusive way to keep your Microsoft Teams status _Active_.

## User Interface
![StayAwake UI](https://github.com/shriprem/StayAwake/blob/main/images/StayAwakeApp.png)

**Seconds between each toggle** field:
This field allows you to specify the interval between each _Scroll Lock_ toggle. The initial default value for this is 240 seconds (_i.e._, 4 minutes). However, you can change this to any value between 10 and 9990 seconds. Your new value will be saved and used the next time you run the application.

**Set Timer** button: After changing the value for *Seconds between each toggle*, click this button to apply the new setting. Clicking this button will also immediately cycle the _Scroll Lock_ button once.

The time when the app last toggled _Scroll Lock_, and the time when it will do so next are displayed and continually refreshed on the application window.

**Minimize** button: Click this button to minimize the application to the Windows System Tray. See the section below for more information on the System Tray icon.

**Exit** button: Click this button to exit the application.


## System Tray Icon
![StayAwake UI](https://github.com/shriprem/StayAwake/blob/main/images/SystemTray.png)

When you minimize the application, it will be represented by the System Tray icon shown above. You can click on this icon to access the following context menu:

* **Restore**: Click this to restore the application window.

* **Exit**: Click this to exit the application.


## Installation

StayAwake is a portable application and does NOT require installation with administrator privileges. Simply download the latest release from the [Releases](https://github.com/shriprem/StayAwake/releases) page and unzip the contents to a folder of your choice. Run the `StayAwake.exe` application to start the utility.


## See Also
StayAwake is also available as a [plugin for Notepad++](https://github.com/shriprem/StayAwake_NPP_Plugin). This version can be useful if you have Notepad++ open during your typical workday. You will be spared the need to separately run the standalone StayAwake application.

<a href='https://ko-fi.com/S6S417WICS' target='_blank'><img height='36' style='border:0px;height:36px;' src='https://storage.ko-fi.com/cdn/kofi5.png?v=6' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>
