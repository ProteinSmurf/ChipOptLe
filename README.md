# ChipOptLe
8bit Gaming Environment using Chip8 Emulator

This project aims to create a slightly customizable Chip8 Emulator with features like color theme customization, adjustable game speed, adjustable window size and a user-friendly menu for game selection. The emulator utilizes SFML (Simple and Fast Multimedia Library) for rendering the initial menu, the Chip8 screen, and sound playback.

Features
 - Customizable Color Theme: Personalize the emulator interface to your liking.
 - Adjustable Window Size: Adjust the size of the display window to your liking.
 - Adjustable Game Speed: Play games at your preferred pace.
 - Simple & User-friendly Menu: Easily select and play games with the built-in menu system.



Information Sources for Chip-8 Emulation:
 - https://www.cs.columbia.edu/~sedwards/classes/2022/4840-spring/designs/CHIP-8.pdf
 - https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
 - http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
 - https://en.wikipedia.org/wiki/CHIP-8


Information Sources for using SFML features:
 - https://www.sfml-dev.org/tutorials/2.6/   | Specifically: Window, Grahpics and Audio modules

SFML Setup:
 - https://www.youtube.com/watch?v=4fcTqmT0Hhg&t=272s



Due to a mistake force push I have lost about 5 commits.
What has changed in these commits:

  - Created a class named ChipApp in which the menu implementation has been moved to and expanded upon, as well as customizable parameters. Customizable parameters are to be changed by user based on preferences using the menu and limited to soem standard available choices.
  - Refactored ChipManager to remove spaghetti code, began parametrizing it to introduce the customizable features ( Scalability for window size , currently )
  - Removed ununsed code and some comments
