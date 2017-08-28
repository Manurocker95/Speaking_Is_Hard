@echo off
bannertool.exe makebanner -i banner.png -a audio.wav -o banner.bnr
bannertool.exe makesmdh -s "Speaking Is Hard 3DS" -l "Speaking Is Hard 3DS" -p "Manurocker95" -i icon.png  -o icon.icn
echo Finished! Banner built!
pause