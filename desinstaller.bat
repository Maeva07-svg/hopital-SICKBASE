@echo off
echo ========================================
echo    DESINSTALLATION HOSPITALIER
echo ========================================
echo.
echo Cette action va supprimer:
echo 1. Le programme Hopital System
echo 2. Les raccourcis du bureau et menu Demarrer
echo 3. Le dossier d'installation
echo.
set /p reponse="Voulez-vous continuer? (O/N): "
if /i not "%reponse%"=="O" goto annuler

echo Suppression des raccourcis...
del "%USERPROFILE%\Desktop\Hopital System.lnk" 2>nul
rmdir "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Hopital System" /s /q 2>nul

echo Suppression des fichiers...
rmdir "C:\Users\MAEVA\Desktop\PROJET INFO C\hopital" /s /q 2>nul

echo Desinstallation terminee!
pause
exit
:annuler
echo Desinstallation annulee.
pause
