## Version History

### Version 1.3.0.0 [September, 2026]

1. StayAwake will use a random pick from a roster of user-specified key codes for each Awake event. See enhancement request: [#15](https://github.com/shriprem/StayAwake/issues/15).

2. StayAwake will use random interval between minimum & maximum seconds for each Awake event. See enhancement request: [#15](https://github.com/shriprem/StayAwake/issues/15).

3. StayAwake will use `StayAwake.ini` configuration file located in the same directory as the `StayAwake.exe` executable file regardless of how the application is invoked. Resolves issue: [#10](https://github.com/shriprem/StayAwake/issues/10).

#### Notes for users upgrading from previous versions of StayAwake
1. The user's choice for Key Simulation in the previous versions will be used as the only enabled key code in this new version. Users will need to use the [Select multiple Key Codes](#select-multiple-key-codes) popup to specify multiple key codes to enable Awakes with random key simulations.

2. The user-specified value for the *Seconds between Awakes* field in the previous versions will be used to fill both *Minimum Awake Seconds* and *Maximum Awake Seconds* fields in this new version. Users will need to specify a different value for these two new fields to enable a random interval between Awake events.

**Release:** [1.3.0.0 Release](https://github.com/shriprem/StayAwake/releases/tag/v1.3.0.0)

---

### Version 1.2.1.0 [January 1, 2026]

1. Added a checkbox option to have StayAwake automatically minimize to System Tray upon launch. See issue: [#8](https://github.com/shriprem/StayAwake/issues/8)
2. Fixed: Pressing ENTER key was causing StayAwake application to exit. See issue: [#9](https://github.com/shriprem/StayAwake/issues/9)

**Release:** [1.2.1.0 Release](https://github.com/shriprem/StayAwake/releases/tag/v1.2.1.0)

---

### Version 1.2.0.0 [October 8, 2025]

#### Enhancements


1. StayAwake executable is now digitally signed by the kind folks at [OSSign](https://www.ossign.org). Thanks to the code signing, there is *NO* need for users to override the Windows Defender Smartscreen or [unblock the executable file](https://stackoverflow.com/questions/48946680/how-to-avoid-the-windows-defender-smartscreen-prevented-an-unrecognized-app-fro) to launch StayAwake.

   <a title="OSSign - Code Signing for Open Source" href="https://www.ossign.org"><img style=" width:150px; height:128px;" src="images/OSSign.png"/></a>

**Release:** [1.2.0.0 Release](https://github.com/shriprem/StayAwake/releases/tag/v1.2.0.0)

---

### Version 1.1.0.0 [September 27, 2025]

#### Enhancements
1. StayAwake now offers a choice from [12 keycode simulations](https://github.com/shriprem/StayAwake?tab=readme-ov-file#key-simulation-options) to better serve individual user situations.

2. Added [Pause/Resume](https://github.com/shriprem/StayAwake?tab=readme-ov-file#pause-button) button to pause or resume the timer-based key simulations.


**Release:** [1.1.0.0 Release](https://github.com/shriprem/StayAwake/releases/tag/v1.1.0.0)

---

### Version 1.0.0.3 [March 15, 2025]
* Updated README.md with information about the application configuration file.
* Added range check while reading the **TimerIntervalInSeconds** setting in the application configuration file.

**Release:** [1.0.0.3 Release](https://github.com/shriprem/StayAwake/releases/tag/v1.0.0.3)

---

### Version 1.0.0.2 [December 25, 2024]
Initial Public Release
**Release:** [1.0.0.2 Release](https://github.com/shriprem/StayAwake/releases/tag/v1.0.0.2)

