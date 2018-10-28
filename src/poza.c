//311CD Constantin Rares

#include <stdio.h>
#include <string.h>
#include "bmp_header.h" //includerea headerului continand structurile pt bmp

struct BGR { //structura ce memoreaza culoarea unui pixel
	unsigned char B; //blue
	unsigned char G; //green
	unsigned char R; //red
};

struct cifra { //structura ce memoreaza pozitia unei cifre si valoarea ei
	int y;
	int x;
	int valoare;
};

char *strrev(char *str)
//a trebuit sa declar functia strrev pentru ca C-ul de pe linux nu o contine
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

int main(){

	char nume[30], nume1[30], nume2[30], nume3[30]; //numele fisierelor
	unsigned int b, g, r; //culorile
	int i, j;
	struct BGR pixel, curent, culori[100];
	FILE *input, *file, *task1, *task2, *task3;
	fpos_t poz;

	input = fopen("input.txt", "rt"); //deschiderea fisierului de input

	fscanf(input, "%s", nume); //citirea numelui bmp-ului

	fscanf(input, "%d %d %d", &b, &g, &r);
	//citirea noii culori

	pixel.B = (unsigned char)b;
	pixel.G = (unsigned char)g;
	pixel.R = (unsigned char)r;

	
	file = fopen(nume, "rb"); //deschiderea fisierului bmp

	nume[strlen(nume)-4] = '\0'; //stergerea extensiei din nume

	strcpy(nume1, nume);
	strcat(nume1, "_task1.bmp"); //numele pentru primul task

	task1 = fopen(nume1, "wb"); //crearea fisierului pt taskul 1

	
	struct bmp_fileheader header;
	struct bmp_infoheader info; 

	fread(&header, sizeof(struct bmp_fileheader), 1, file);
	//citirea File Headerului din bmp
	fread(&info, sizeof(struct bmp_infoheader), 1, file);
	//citirea Info Headerului din bmp
	fgetpos(file, &poz);
	//memorarea pozitiei unde incepe harta imaginii in bmp
	int padding = (4 - (info.width * 3)%4)%4;
	//calcularea paddingului

	fwrite(&header, sizeof(struct bmp_fileheader), 1, task1);
	//scrierea File Headerului pentru task1
	fwrite(&info, sizeof(struct bmp_infoheader), 1, task1);
	//scrierea Info Headerului pentru task1

	int x = 0, y = 0;

	int a[info.height][info.width];
	//declararea unei matrice pentru memorarea hartii imaginii


	for(y = 0; y < info.height; y++)
		{for(x=0; x<info.width;x++)
			a[y][x]=0;}
			//umplerea acesteia cu 0-uri

	x=0;
	y=0;


	for(y=0; y<info.height; y++){
		for(x=0; x<info.width; x++){

			fread(&curent, sizeof(struct BGR), 1, file);
			//citirea unui pixel

			if( (curent.B == curent.G) && 
				(curent.B == curent.R) &&
				(curent.B == 255 )){ //verificarea daca acesta este alb

				fwrite(&curent, sizeof(struct BGR), 1, task1);
				a[info.height - y - 1][x] = 0;
			}
			else {
				fwrite(&pixel, sizeof(struct BGR), 1, task1);
				a[info.height - y - 1][x] = 1;
				//daca nu este alb atunci pixelul respectiv va fi introdus
				//in noua imagine cu o noua culoare (cea specificata)
			}

		}
		
		fseek(file, padding, 1);
		//sarirea peste padding

		int temp = padding;
		char k = 0;
		while(temp){
			fwrite(&k, sizeof(k), 1, task1);
			temp--;}
			//scrierea paddingului in noua imagine
	}


	int m[info.height][info.width];
	//declararea unei noi matrice pentru task2

	memcpy(&m, &a, info.height*info.width*sizeof(int));
	//copierea primei matrice in cea de-a doua

	char s[100]; //sir folosit pentru memorarea cifrelor din captcha
	struct cifra cifre[400]; //memoreaza cifrele descoperite
	int contor=0; //numara cate cifre au fost descoperite
		
	for(x = info.width - 1; x >= 0; x--)
		for(y = 0; y < info.height; y++) //cautarea in matrice de la dreapta 
										 //la stanga

			if( a[y][x] == 1 ){//verifica daca pixelul nu este alb

				i = y;
				j = (x - 4); //colt stanga sus cifra

				cifre[contor].y = y; //memoreaza pozitia cifrei descoperite
				cifre[contor].x = x;

				int deplasare = (3 * info.width + padding)*
								(info.height - y -1) + 3 * x;
								//calculeaza distanta pana la pixelul de unde
								//incepe cifra

				fsetpos(file, &poz);
				fseek(file, deplasare, 1);
				//ajunge la pixelul de unde incepe cifra
				fread(&culori[contor], sizeof(struct BGR), 1, file);
				//memoreaza culoarea cifrei

				if(a[i][j] == 0){ //verifica daca cifra este 1
					strcat(s, "1");
					cifre[contor].valoare = 1;
					goto checked;
				}

				if(a[i][j+1] == 0){ //verifica daca cifra este 4
					strcat(s, "4");
					cifre[contor].valoare = 4;
					goto checked;
				}

				if(a[i+4][j] == 0){ //verifica daca cifra este 7
					strcat(s, "7");
					cifre[contor].valoare = 7;
					goto checked;
				}

				if(a[i+2][j+2] == 0){ //verifica daca cifra este 0
					strcat(s, "0");
					cifre[contor].valoare = 0;
					goto checked;
				}

				if(a[i+1][j] == 0	&& //verifica daca cifra este 3
				   a[i+3][j] == 0){
					strcat(s, "3");
					cifre[contor].valoare = 3;
					goto checked;
				}

				if(a[i+1][j] == 0){ //verifica daca cifra este 2
					strcat(s, "2");
					cifre[contor].valoare = 2;
					goto checked;
				}

				if(a[i+1][j+4] == 0	&& //verifica daca cifra este 5
				   a[i+3][j] == 0){
					strcat(s, "5");
					cifre[contor].valoare = 5;
					goto checked;
				}

				if(a[i+1][j+4] == 0){ //verifica daca cifra este 6
					strcat(s, "6");
					cifre[contor].valoare = 6;
					goto checked;
				}

				if(a[i+3][j] == 0){ //verifica daca cifra este 9
					strcat(s, "9");
					cifre[contor].valoare = 9;
					goto checked;
				}

				strcat(s, "8"); //daca se ajunge aici, atunci cifra este 8
				cifre[contor].valoare = 8;

				checked:	

					contor++;

					for(i = y; i <= (y + 4); i++)
						for(j = (x - 4); j <= x; j++){
							a[i][j] = 0;
							if(m[i][j])
								m[i][j] = contor;
							//dupa ce cifra a fost identificata, aceasta se
							//"sterge" din matrice, pentru a putea cauta 
							//urmatoarea cifra
						}

				
				
			}

			

	strcpy(nume2, nume);
	strcat(nume2, "_task2.txt"); //numele pentru task2

	task2 = fopen(nume2, "wt");	 //crearea fisierului pentru task2

	fprintf( task2, "%s", strrev(s) );
	//scrierea numarului din captcha (am folosit strrev deoarece am descoperit
	//cifrele de la dreapta la stanga)

	char c;
    int nr;
    int k, l;

	do {
        c = fgetc(input); // ajungerea la randul urmator din fisier
   	 } while (c != '\n' && c != EOF); 

		

    c = fgetc(input); //citirea cate unui char
    while (c!='\n'){ //verificarea daca s-a ajuns la enter
      if(c!=' '){
        nr = c - '0'; //transformarea in int

        for(i=0; i<contor; i++){
		
	}
        
        for(k=0; k<contor; k++){

       	if(cifre[k].valoare == nr){ //verifica daca cifra trebuie eliminata
        		for(l=k; l>0; l--){

        			for(i=0; i<5; i++)
        				for(j=0; j<5; j++){
        					m[cifre[l].y +i][cifre[l].x - 4 +j] =
        					 m[cifre[l-1].y + i][cifre[l-1].x - 4 + j];
        					cifre[l].valoare = cifre[l-1].valoare;
        				} //se muta cifrele

        			
        		}

        		for(i=0; i<5; i++)
        				for(j=0; j<5; j++)
        					m[cifre[0].y +i][cifre[0].x - 4 +j] = 0;
        				//se elimina cifra

        		for(l=0; l<(contor-1); l++)
        			cifre[l] = cifre[l+1]; //se muta informatiile despre cifre

        		k--;
        		contor--; //se micsoreaza numarul de cifre din noul captcha
        		
        		
        	}

        }



      }
        c = fgetc(input); //citirea unui nou char din fisier
    }


	strcpy(nume3, nume);
	strcat(nume3, "_task3.bmp"); //numele pentru task3

	task3 = fopen(nume3, "wb"); //crearea fisierului pentru task3

	fwrite(&header, sizeof(struct bmp_fileheader), 1, task3);
	//scrierea File Headerului pentru task3
	fwrite(&info, sizeof(struct bmp_infoheader), 1, task3);
	//scrierea Info Headerului pentru task3

	struct BGR alb; //un pixel alb
	alb.B = 255;
	alb.G = 255;
	alb.R = 255;

	for(y = (info.height - 1); y >= 0; y--){
		for(x = 0; x < info.width; x++)
			if(m[y][x]) 
				fwrite(&culori[m[y][x]-1], sizeof(struct BGR), 1, task3);
			else
				fwrite(&alb, sizeof(struct BGR), 1, task3);
			//scrierea noii harti de pixeli pentru imaginea de la task3
		
		int temp = padding;
		char k = 0;
		while(temp){
			fwrite(&k, sizeof(k), 1, task3); //scrierea paddingului
			temp--;}
	}


	fclose(input); //inchiderea tuturor fisierelor
    fclose(file);
    fclose(task1);
    fclose(task2);
    fclose(task3);
    
	return 0;
}	
