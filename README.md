# Kodune temperatuuri- ja õhuniiskuse andur

## Projekti eesmärk ja loodava seadme lühikirjeldus

Projekti eesmärk on luua kodune temperatuuri- ja õhuniiskuse mõõteseade, mis kuvab mõõtmistulemusi LCD-ekraanil ja annab visuaalse hoiatuse, kui ruumi temperatuur langeb alla 22 °C. Seade on mõeldud kasutamiseks toas ning paigutatakse 3D-printeriga trükitud korpusesse, mis kaitseb elektroonikat ja annab seadmele korraliku välimuse.

Seade kasutab DHT22 andurit ruumi temperatuuri ja suhtelise õhuniiskuse mõõtmiseks. Mõõtmistulemus kuvatakse 16×2 I2C LCD-ekraanil, mille juhtimiseks kasutatakse PCF8574 adapterit.

Vaikimisi kuvab ekraan ruumi temperatuuri. Kui kasutaja vajutab korpuse peal olevat nuppu, vahetub kuvatav väärtus õhuniiskusele. Kui nuppu enam ei vajutata, läheb ekraan 30 sekundi pärast automaatselt tagasi temperatuuri kuvamisele. Kui kasutaja vajutab nuppu uuesti, lülitub ekraan kohe tagasi temperatuurile.

Sama nupp sisaldab LED-valgusdioodi. Kui mõõdetud temperatuur on alla 22 °C, hakkab nupu LED põlema. See annab kasutajale kohe visuaalse märku, et ruumi temperatuur on langenud liiga madalaks. Kui temperatuur tõuseb vähemalt 22 °C-ni, kustub LED automaatselt.

## Vajalik ristvara

| # | Komponent | Kogus | Kirjeldus | Pinge / toide | Rolli projektis |
|---|-----------|------:|-----------|---------------|-----------------|
| 1 | Arduino Uno R3 | 2 | ATmega328P põhine mikrokontrolleri plaat | Tööpinge 5 V; väline toide 7–12 V (soovituslik) | Loeb andurit, juhib LCD-d ja nuppu; teine plaat on varuks või teise ruumi jaoks |
| 2 | 16×2 LCD-ekraan koos I2C-adapteriga | 1 | Tähemärgid: 16 × 2; PCF8574 I/O adapter | 5 V DC | Kuvab temperatuuri ja õhuniiskuse väärtused |
| 3 | DHT22 / AM2302 andur | 1 | Digitaalne temperatuuri- ja õhuniiskuseandur | 3,3–5,5 V DC | Mõõdab temperatuuri ja suhtelist õhuniiskust |
| 4 | LED Nupp (taktvõti) | 1 | Hetkeline kontakt | 5 V loogikaga ühilduv | Taustvalguse juhtimine, kuvarežiimi vahetamine või mõõtmise käivitamine |

## Toide
- Arduino toide?
- LCD ja DHT22 saavad toite Arduino 5 V viigust.
- Kogu ristvara ülikoolis olemas

  
## Nõuded loodavale seadmele

| # | Nõue | Kirjeldus |
|---|------|-----------|
| 1 | Temperatuuri mõõtmine | Seade peab mõõtma ruumi temperatuuri DHT22 anduri abil ja kuvama tulemuse LCD-ekraanil. |
| 2 | Õhuniiskuse mõõtmine | Seade peab mõõtma suhtelist õhuniiskust ja võimaldama seda väärtust ekraanil kuvada. |
| 3 | Vaikimisi kuvamine | Pärast seadme käivitamist kuvab ekraan vaikimisi temperatuuri. |
| 4 | Kuvarežiimi vahetamine | Korpuse peal oleva nupu vajutamisel vahetub ekraan temperatuuri kuvamiselt õhuniiskuse kuvamisele. |
| 5 | Automaatne tagasipöördumine | Kui õhuniiskust kuvatakse 30 sekundit ja nuppu ei vajutata, lülitub ekraan automaatselt tagasi temperatuurile. |
| 6 | Korduv nupuvajutus | Kui kasutaja vajutab nuppu uuesti, lülitub ekraan kohe tagasi temperatuuri kuvamisele. |
| 7 | Madala temperatuuri hoiatus | Kui temperatuur on alla 22 °C, peab nupus olev LED-valgusdiood süttima. |
| 8 | LED-i automaatselt väljalülitamine | Kui temperatuur tõuseb vähemalt 22 °C-ni või sellest kõrgemaks, peab nupus olev LED kustuma. |
| 9 | Kaitsekorpuse paigaldamine | Elektronika ja ekraan peavad paiknema 3D-printeriga trükitud korpuses, mis kaitseb neid mehaaniliste kahjustuste eest. |
| 10 | Kasutatavus | Seade peab olema lihtne kasutada: kasutaja peab saama väärtusi vaadata ja kuvarežiimi vahetada ühe nupuga. |
| 11 | Stabiilne töö | Seade peab töötama stabiilselt 5 V toitel ning DHT22 andurit ei tohi lugeda sagedamini kui kord 5 minuti jooksul. |
| 12 | Nupu ja LED-i töökindlus | Nupp peab olema ühendatud Arduino digitaalsisendisse koos sisemise pull-up takistiga ning LED peab olema kaitstud sobiva eeltakistiga. |
