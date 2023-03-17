
@echo off
:: Board a usar en la compilacion
set board=rp2040:rp2040:rpipico
set port=COM5
set sketch=.\VerificacionProduccion.ino
set libraries="C:/Users/javie/Team Dropbox/JRODRIGUEZ/Repositorios/01.Rover/03.Firmware/Libraries"

:: Comando para compilar , exportar y uploadear los archivos binarios en la carpeta residente de este .bat

arduino-cli compile -e -t --fqbn %board% -v --libraries %libraries%

arduino-cli upload %sketch% --fqbn %board% --port %port%

