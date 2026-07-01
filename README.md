This is a fork of the UAV cockpit design by mason334

<img width="1536" height="2040" alt="PXL_20260701_104142952 PORTRAIT" src="https://github.com/user-attachments/assets/95ad420e-708c-4a3b-93be-fa641528a1f3" />
  
Main changes:

- Beluga cockpit redesign.
   - Original design by mason334 occupied all of the battery area
   - Added fixtured for printers where larger parts need to be split and assembled post print
   - Increased space on top of dashboard for more room to fit HUD OLED
- Removed lower screen.  Limited functionality for INAV and incurred on battery area.  Relevant sections of code removed.
- Code optimisation
   - Converted chinese characters to english
   - Reduced amount of trigonometry to improve performance
   - Changed to built in fonts to improve performance
   - Tidied up HUD to make more clear

To come:

- Detailed compiling instructions
- Detailed wiring insturctions in english
- Alternative cockpit screen layouts
- Template for mounting main OLED for adaptation to other models


Note: use v 8.3.11 of LVGL, code will not compile with later versions
