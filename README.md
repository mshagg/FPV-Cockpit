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
- Alternative cockpit screen layouts - DONE SEE SCREENSHOT BELOW
- Template for mounting main OLED for adaptation to other models

<img width="2615" height="2154" alt="PXL_20260702_105902713" src="https://github.com/user-attachments/assets/da9de6ee-6720-4794-9728-9a77d8db5ebb" />


Note: use v 8.3.11 of LVGL, code will not compile with later versions

Hardware needed:

Main display: 

<img width="1072" height="496" alt="Screenshot 2026-07-01 201705" src="https://github.com/user-attachments/assets/0411e0dd-4b9f-40a4-8f70-ee255b5a5e6d" />

Auxillary Display:

<img width="1149" height="586" alt="Screenshot 2026-07-01 201832" src="https://github.com/user-attachments/assets/80100445-eb89-4935-8037-816b0cb65c32" />

HUD:

<img width="1150" height="519" alt="Screenshot 2026-07-01 202048" src="https://github.com/user-attachments/assets/9d4c317e-3906-4610-a1ae-a075077739af" />

<img width="1076" height="504" alt="Screenshot 2026-07-01 201946" src="https://github.com/user-attachments/assets/d91c5d64-450e-403a-92cf-6afa33c5f8b1" />

<img width="1145" height="534" alt="Screenshot 2026-07-01 202142" src="https://github.com/user-attachments/assets/5274445c-a1e2-4446-8c77-7b8b46851932" />

Wiring:

<img width="1619" height="971" alt="f7747981-b990-42b9-abdf-2ae96eff45c6" src="https://github.com/user-attachments/assets/7ad21fff-1822-44da-a19b-87eabe07fa5e" />
