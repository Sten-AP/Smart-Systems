# Analyse
Sten Hulsbergen<br/>
2ITIOT<br/>

## Probleemstelling

Het grootste probleem wat er is, is dat er een volledig nieuwe wagen ontworpen moet worden. 
Deze moet twee UART connectoren bevatten, drie I2C connectoren, de regelaar 7805 vervangen door een LDL1117 van 3.3V en 5V, polariteitsbeveiliging voorzien, motordriver aanpassen naar een SMD-type en een OLED-scherm met I2C sturing. 
Naast de sturing moeten een aantal sensoren voorzien worden, een linetracker, IR- en ultrasone afstandssensoren. Ook zullen er verkeerssignalisaties ontwikkeld moeten worden zoals verkeerslichten, slagbomen,... 
Als laatste moet de wagen manueel alsook automatisch bestuurd kunnen worden.

## Mindmap

![](Mind_map.svg)

## Hardware analyse
### Hardware blokdiagram
![](Blokdiagram.svg)

### Specificatie tabel
| Blok | Specificatie | Min | Nominaal | Max |
|:------:|:-----------------:|:-----:|:--------------:|:------:|
| Batterij | Spanning |  | 7.4V |  |
|  | Stroom | 700mA |  | 2A |
|  | Capaciteit |  | 1200mAh |  |
| TC78H621FNG | Spanning | 2.5V | 5V | 15V |
|  | Stroom |  | 0.8A | 1.1A |
| LDL1117S50R | Spanning |  | 5V |  |
|  | Stroom |  | 2A |  |
| LDL1117S33R | Spanning |  | 3.3V |  |
|  | Stroom | 500mA |  |  |
| ESP32 Wrover | Spanning | 2.7V | 3.3V | 3.6V |
|  | Stroom |  |  | 200mA |
| ATmega328p | Spanning | 1.8V | 5V | 5.5V |
|  | Stroom |  |  | 200mA |
| MJKDZ MIR-3.0Y | Spanning | 3 |  | 5 |
|  | Bereik |  | 40mm |  |
| GP2Y0A21YK0F | Spanning | 4.5V | 5V | 5.5V |
|  | Stroom |  | 30mA |  |
| HC-SR04 | Spanning |  | 5 V |  |
|  | Stroom | 2mA |  | 15mA |
| SSD1306 | Spanning | 3.3V |  | 5V |
| CP2102N-A02-GQFN28R | Spanning | 3V | 3.3V | 3.6V |
| USB4085-GF-A | Spanning |  | 5V |  |
|  | Stroom |  | 3A |  |

### Argumentatie en alternatieven tabel

| Blok | Argumentatie | Alternatieven |
|:------:|:-----------------:|:-----:|
| ESP32 Wrover | De Wrover wordt rechtstreeks geplaatst op de PCB, dit maakt de gehele PCB goedkoper aangezien de ESP32 zelf ontworpen word met alleen alle benodigheden van de schakeling. |  |
| MJKDZ MIR-3.0Y | Om de wagen steeds recht te kunnen laten rijden en de baan zelfstandig te laten volgen, moeten we steeds kunnen weten waar de weg zich bevind. |  |
| GP2Y0A21YK0F | Deze IR-sensor is beschikbaar in grote hoeveelheden op school en is vrij makkelijk om mee te werken. Dient om de afstand naar een bepaald object te meten op kortere afstanden. | GP2Y0A41SK0F |
| HC-SR04 | Deze ultrasone sensor is beschikbaar in grote hoeveelheden op school en is vrij makkelijk om mee te werken. Dient om de afstand naar een bepaald object te meten op langere afstanden. |  |
| TC78H621FNG | Deze motor driver is gekozen omdat het SMD component is. De driver heeft twee inputs en outputs, dit maakt het makkelijker om te ontwerpen aangezien het een enkele component is en geen twee. | Twee motor drivers LA6583MC-AH |

### Elektrisch schema
#### Sturingsschakeling
![](SturingsschakelingSCHEMA.png)

#### Sensorschakeling
![](SensorschakelingSCHEMA.png)

### PCB ontwerp
#### Sturingsschakeling
![](SturingsschakelingPCB.png)

#### Sensorschakeling
![](SensorschakelingPCB.png)

## Software analyse
### Data In -en Outputs
| Blok | Data In | Data Out |
|:----:|:-------:|:--------:|
| IR-sensor | Digitaal IR aanzetten | Analoge spanningswaarde van ontvanger |
| Linetracker | Digitaal IR aanzetten | Analoge spanningswaarde van elke ontvanger |
| Ultrasoonsensor | Digitaal puls versturen | Digitaal puls ontvangen |
| OLED | I2C | I2C |
| Motor driver | Analoog PWM signaal | versterkt PWM signaal |
### State diagram
#### Manueel
![](Manueel.svg)

#### Automatisch
![](Automatisch.svg)

### Flowchart


## Release plan



