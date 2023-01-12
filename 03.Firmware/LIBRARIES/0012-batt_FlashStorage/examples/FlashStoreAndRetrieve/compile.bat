
@echo off
:: Board a usar en la compilacion
set board=adafruit:samd:adafruit_feather_m0
set libraries="C:/Users/Javi/Team Dropbox/JRODRIGUEZ/Repositorios/01.Rover/03.Firmware/Libraries"
:: Comando para compilar y exportar los archivos binarios en la carpeta residente de este .bat
arduino-cli compile -e -t --fqbn %board% -v --libraries %libraries%

