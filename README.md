# TEMPO LUMINO
Bonjour, bienvenue sur cette page de présentation de notre projet. 

![Imgur](https://i.imgur.com/XwlHCOU.jpg)

# COMPOSITION DU GROUPE
Tout d'abord, voici la composition de notre groupe:
**Charles TARDY**
**Pierre HERNANDEZ**
**Adrian PETRICEVIC**

# ETAT DE L'EXISTANT
Source pouvant nous guider:

https://selfhostedhome.com/wifi-rgb-led-strip-music-visualization/

https://www.robot-maker.com/forum/topic/8199-traitement-audio-via-arduino/

https://create.arduino.cc/projecthub/Joao_Claro/arduino-beat-detector-d0a21f

http://wiki.openmusiclabs.com/

https://github.com/G6EJD/ESP32-8-Octave-Audio-Spectrum-Display

# DESCRIPTION DU PROJET

Notre projet **TEMPO-LUMINO** est une boîte lumineuse adaptable en fonction des sonorités et des rythmes par rapport aux sons d'un instrument de musique.
Pour faire simple, des LEDs seront accrochées à une guitare (ainsi que la WEMOS) et permettront grâce aux différents sons procurés par l'instrument de produire une lumière d'ambiance s'adaptant au rythme d'une musique.

## Particularités de notre projet

Ce n'est pas une simple boîte à lumière qui réagit en fonction du son, elle approfondit réellement le coté ambiance grâce à l'attenuation de lumière lors de la génération de son aiguë ou grave.
Il y a aussi un système de sauvegarde qui permettra de garder les données ingérées et ainsi, même sans son, de pouvoir régénérer le flux lumineux que l'enregistrement peut procurer.

## Matériel envisagé

Bread Board
WEMOS R1 D1 Mini *2
LED ou plaque à LED
Micro Piezzo (capacité à capter le sons aigue)
Micro Amplificateur de son (micro à membrane passif permettant la capture de son à basse fréquence)

## Schéma de placement du matériel sur bread Board

![Imgur](https://i.imgur.com/VsRkwRn.png)

## Librairies utilisées

https://github.com/espressif/arduino-esp32

https://github.com/squix78/esp8266-oled-ssd1306

https://github.com/kosme/arduinoFFT 

https://github.com/esp8266/Arduino
