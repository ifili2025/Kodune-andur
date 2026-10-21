# Kodune temperatuuri- ja õhuniiskuse andur

## Vajalik ristvara

| # | Komponent | Kogus | Kirjeldus | Pinge / toide | Rolli projektis |
|---|-----------|------:|-----------|---------------|-----------------|
| 1 | Arduino Uno R3 | 2 | ATmega328P põhine mikrokontrolleri plaat | Tööpinge 5 V; väline toide 7–12 V (soovituslik) | Loeb andurit, juhib LCD-d ja nuppu; teine plaat on varuks või teise ruumi jaoks |
| 2 | 16×2 LCD-ekraan koos I2C-adapteriga | 1 | Tähemärgid: 16 × 2; PCF8574 I/O adapter | 5 V DC | Kuvab temperatuuri ja õhuniiskuse väärtused |
| 3 | DHT22 / AM2302 andur | 1 | Digitaalne temperatuuri- ja õhuniiskuseandur | 3,3–5,5 V DC | Mõõdab temperatuuri ja suhtelist õhuniiskust |
| 4 | Nupp (taktvõti) | 1 | Hetkeline kontakt | 5 V loogikaga ühilduv | Taustvalguse juhtimine, kuvarežiimi vahetamine või mõõtmise käivitamine |

## Toide
- Arduino toide?
- LCD ja DHT22 saavad toite Arduino 5 V viigust.

Kogu ristvara ülikoolis olemas
