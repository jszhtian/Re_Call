# Textractor

![Come si vede](screenshot.png)

[English](README.md) ● [Español](README_ES.md) ● [简体中文](README_SC.md) ● [Русский](README_RU.md) ● [한국어](README_KR.md) ● [ภาษาไทย](README_TH.md) ● [Italiano](README_IT.md) ● [日本語](README_JP.md) ● [Bahasa](README_ID.md) ● [Português](README_PT.md)

**Textractor** (a.k.a. NextHooker) è un agganciatore di testi di videogiochi open-source per Windows/Wine basato su[ITHVNR](http://www.hongfire.com/forum/showthread.php/438331-ITHVNR-ITH-with-the-VNR-engine).<br>
Guarda il [video tutorial](https://tinyurl.com/textractor-tutorial) per una sintesi veloce sul suo utilizzo.

[![Donazioni](https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=akashmozumdar%40gmail.com&item_name=Textractor%20development&currency_code=USD)

## Scarica

Le uscite di Textractor possono essere trovate [qui](https://github.com/Artikash/Textractor/releases).<br>
L'ultima uscita di ITHVNR può essere trovata [qui](https://drive.google.com/open?id=13aHF4uIXWn-3YML_k2YCDWhtGgn5-tnO).<br>
Prova ad avviare vcredist se ti esce un errore quando avvi Textractor.

## Caratteristiche

- Altamente estensibile e personalizzabile
- Aggancia automaticamente molti engine di gioco (inclusi alcuni non supportati da VNR!)
- Aggancia il testo utilizzando codici /H "hook" (molti codici AGTH supportati)
- Estrae il testo direttamente usando codici /R "read"

## Supporto

Fatemi sapere su qualunque bug, giochi che Textractor ha problemi nell'agganciare, richieste future, o altri suggerimenti.<br>
Se avete dei problemi nel agganciare un gioco vi prego di inviarmi via email un sito dove posso scaricarlo, o regalatemelo su [Steam](https://steamcommunity.com/profiles/76561198097566313/).

## Estenzioni

Guardate il mio [Progetto Example Extension](https://github.com/Artikash/ExampleExtension) per vedere come costruire un estenzione..<br>
Guardate la cartella delle estenzioni per esempi di cosa possono fare le estenzioni.

## Contributi

Tutti i contributi sono apprezzati! Inviatemi un email a akashmozumdar@gmail.com se avete delle domande sul codebase.<br>
Dovreste usare il processo standard di creare una pull request (fork, branch, commit changes, crea PR dal vostro ramo al mio master).<br>
Contribuire alla traduzione è semplice: traduci le stringhe in text.cpp cosi come questo README.

## Compiling

Prima di compilare *Textractor*, dovresti ottenere Visual Studio con supporto CMAKE, cosi come Qt versione 5.13<br>
Dovresti essere in grado di aprire la cartella in Visual Studio, e costruire. Avvia Textractor.exe

## Architettura del progetto

L'host (guarda la cartella GUI/host) innietta texthook.dll (creato dalla cartella texthook) nel processo e lo connette attraverso due file pipe.<br>
L'host scrive a hostPipe, texthook scrive a hookPipe.<br>
Texthook aspetta per il pipe di essere connesso, poi innietta alcune istruzione in qualunque funzione di immissione del testo (es. TextOut, GetGlyphOutline) che causa il loro input di essere inviato attraverso il pipe.<br>
Informazioni aggiuntive sui ganci soo scambiati attraverso la memorio condivisa.<br>
Il testo che l'host riceve attraverso il pipe è poi processato un poco prima di essere rinviato alla GUI.<br>
Infine, la GUI dispone il testo alle estenzioni prima di mostrarle.

## [Sviluppatori](CREDITS.md)
