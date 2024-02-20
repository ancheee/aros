# aros

2. Програм за размену текстуалних порука између процеса имплементиран у програмском језику C употребом дељене меморијe (енгл. shared memory) и семафора
3. Програм за размену текстуалних порука између процеса имплементиран у програмском језику C употребом named pipe механизма и семафора

###Projekat obuhvata:
- skelet
- signali
- fifo_server & fifo_client
- log_server & log_client
- sm_writer & sm_reader

##Skelet
Skelet doslovno predstavlja skelet (kostur) ostalih programa koji se nalaze u okviru projekta, odnosno njegov sadržaj se nalazi u svakom od ostalih. Sadrži uključena neophodna zaglavlja za odgovarajuće funkcionisanje programa, makro

##Signali
- Predstavlja najopštiji model za regulisanje signala. Hvata 'SIGINT' i 'SIGTERM' signale i ispisuje ih u PID (najčešće korišćen algoritam kontrole). U main-u, ulazi u petlju gde čeka signale i ispisuje poruke o tome i o hvatanju istih. Izlazi iz petlje ukoliko je postavljen određen flag. 

##Fifo_server & fifo_client
- Jednostavna implementacija named pipe (FIFO (First In First Out)) razmene poruka. Sastoji se iz Fifo servera i Fifo clienta.
- oba dela programa sadrže makroe i funkciju za obradu grešaka, kao i statičku promenljivu koja jednostavno objašnjava kako se program koristi. 



##Log_server & log_client
- U ovom programu implementirana je jednostavna razmena poruka među korisnicima korišćenjem semafora i deljene memorije.
- Oba dela programa uključuju makroe za obradu grešaka (kao i funkciju), strukturu deljene memorije, funkciju kreiranja bloka deljene memorije, semafore, kratku statičku promenljivu o jednostavnom korišćenju programa.

##Sm_writer & sm_reader
- U ovom programu impllementirano je jednostavna razmena niza putem deljene memorije. 

