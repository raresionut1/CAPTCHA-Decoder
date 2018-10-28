Translates certain Bitmap CAPTCHAs to text by applying different filters on the image and splitting it into small pixel matrices.
//311CD Constantin Rares

Voi explica in mare cum am gandit problema, deoarece aceasta este explicata mai
in detaliu, pe bucati, sub forma de commentarii, in fisierul sursa.

La aceasta problema nu am mai abordat modelul "Divide et impera", nefolosiind
functii pentru fiecare task in parte, deoarece am folosit harta de pixeli din
fisierul bmp pentru mai multe taskuri simultan.



TASK1:
	Dupa ce m-am ocupat de 'file header' si de 'info header', pur si simplu
 am verificat fiecare pixel in parte daca este alb sau nu, modificandu-i 
culoarea daca nu este alb.



TASK2:
	Am redat harta de pixeli sub forma unei matrice, tocmai pentru a fi mai
usor de lucrat pe ea si de a identifica cifrele mai usor.
	Pentru identificarea cifrelor, am cautat in matrice de la dreapta la
dreapta la stanga (aveam nevoie sa le descopar in ordinea asta din cauza
abordarii mele de a verifica cate un singur pixel pentru a descoperi cifra.
Pe baza unor diferente intre pixelii albi ai cifrelor de 5x5 am reusit sa
identific cifrele folosind un numar minim de if-uri. (Da stiu ca am folosit
'goto', dar in cazul asta chiar era mai eficient asa si este destul de evident
saltul).
	Dupa ce a fost identificata cifra, aceasta se "sterge" din matrice,
pentru a nu mai gasi pixeli din aceasta.



TASK3:
	Am folosit matricea de la task2 pentru a modifica harta de pixeli, iar
mai multe structuri pentru a memora coordonatele cifrelor, valorea acestora si
culoarea acestora.
	Am citit cate un caracter din fisier tocmai pentru ca nu stiam cate
cifre trebuie eliminate, oprindu-se la final de rand (la intalnirea
caracterului '\n'.
	La intalnirea unei cifre ce trebuie eliminate, toate cifrele din
dreapta acesteia au fost mutate cu 1 pozitie la stanga, si de asemenea
datele despre coordonate ale acestora au fost mutate.
	La final am creat noua imagine bmp, pe baza noii matrice si a datelor
despre fiecare cifra.
	
