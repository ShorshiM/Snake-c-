#include <allegro.h>
#include "cabecera.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <ctime>
#include <sstream>
#include <time.h>
#define maxfilas 20
#define maxcols  31
#define limite_nodos 200
#define izquierda 0
#define derecha 1
#define arriba 2
#define abajo 3
using namespace std;

int dir=derecha;
int limite_x = 900/40;
int limite_y = 600/40;
int tam = 3;
int comida_x,comida_y;
int vonus_x,vonus_y;
int punto=0;
int vidas=3;
int salir=1;

struct Nodo{
	int x,y;
};
Nodo cuerpo[limite_nodos];

void reinicio(int *vel){
	*vel=100;
	tam=3;
	punto=0;
	vidas=3;
	dir=derecha;
}


char mapa[maxfilas][maxcols] = {
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
	"XOOOOOOOOOOOOOOOOOOOOOOOOGOOGX",
	"XGOOOOGOOOOOOHOOOOOOOOOOOOOOOX",
	"XOOOOOOOOOOOOOOOOOOOGOOOOOOOOX",
	"XOOHOOOOOGOOOOOOOOOOOOOOOOOOHX",
	"XOOOOOOOOOOOOOOOOOOOOOOOOOOOHX",
	"XOOOOOHOOOOOOOHOOOOOOHOOOOOOOX",
	"XOOOOOOOOOOOOOOOOOOOOOOOOOOOOX",
	"XHOOOOOOGOOOOOOOOOGOOOOOOOOOOX",
	"XGOOOOOOOOOOOOOOOOOOOGOOOOOOOX",
	"XOOOHOOOOOOOOOOOOOOOOOOOOOOOGX",
	"XOOOOOOOOOOOHOOOOOOOOHOOOOOOGX",
	"XOOOOOOOOOOOOOOOOOOOOOOOOOOOHX",
	"XOOGOOOOOHOOOOOGOOOOOOOOOOOOGX",
	"XOOOOGOOOOHOOOOOOOOOOOGOOOOOOX",
	"XOOOOOOOOOOOOOOOOOOHOOOOOOOOOX",
	"XOOOOOOOOOOOOOOOOOOOOOOOOOOOHX",
	"XOOOOOOHOOOOOOGOOOOOOOHOOOOOOX",
	"XOOOOOOOOOOOOOOOOOOOOOOOOOOOGX",
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
};


void dibujar_mapa(BITMAP *buffer,BITMAP *pared,BITMAP *piso,BITMAP *cesped,BITMAP *adorno){
	int fil,col;
	for(fil=0;fil<maxfilas;fil++){
		for(col=0;col<maxcols;col++){
			if(mapa[fil][col]=='X'){
				draw_sprite(buffer,pared,col*30,fil*30);
			}
			if(mapa[fil][col]=='O'){
				draw_sprite(buffer,piso,col*30,fil*30);
			}
			if(mapa[fil][col]=='H'){
				draw_sprite(buffer,cesped,col*30,fil*30);
			}
			if(mapa[fil][col]=='G'){
				draw_sprite(buffer,adorno,col*30,fil*30);
			}
		}
	}
}

void pantalla(BITMAP *buffer,BITMAP *screen){
	blit(buffer,screen,0,0,0,0,900,600);
}

void generar_snake(){
	//generar cabeza
	cuerpo[0].x =10;
	cuerpo[0].y =9;
	//generar cuerpo
	for(int i=1;i<=tam;i++){
		cuerpo[i].x=cuerpo[0].x-i;
		cuerpo[i].y=cuerpo[0].y;
	}
}

void dibujar_snake(BITMAP *snakeybmp,BITMAP *snakey,BITMAP *buffer,BITMAP *cuerpo_iz){
	//dibujar cabeza
	switch(dir){
		case izquierda:{
			blit(snakeybmp,snakey,0,0,0,0,30,30);
			break;
		}
		case derecha:{
			blit(snakeybmp,snakey,30,0,0,0,30,30);
			break;
		}
		case arriba:{
			blit(snakeybmp,snakey,60,0,0,0,30,30);
			break;
		}
		case abajo:{
			blit(snakeybmp,snakey,90,0,0,0,30,30);
			break;
		}
	}
	draw_sprite(buffer,snakey,cuerpo[0].x*30,cuerpo[0].y*30);
	//dibujar cuerpo
	for(int i=1;i<=tam;i++){
		//magenta toma como transparencia
		draw_sprite(buffer,cuerpo_iz,cuerpo[i].x*30,cuerpo[i].y*30);
	}
}

void actualizar_direccion(){
	if(key[KEY_UP]){
		if(dir != abajo){
			dir=arriba;
		}
	}else{
		if(key[KEY_DOWN]){
			if(dir != arriba){
				dir=abajo;
			}
		}else{
			if(key[KEY_RIGHT]){
				if(dir !=izquierda){
				dir=derecha;	
				}
			}else{
				if(key[KEY_LEFT]){
					if(dir != derecha){
						dir=izquierda;
					}
				}
			}
		}
	}
}

void mover_snake(int *vel,SAMPLE *comer,SAMPLE *muerte){
	//mover cuerpo
	for(int i=tam;i>0;i--){
		cuerpo[i] =cuerpo[i-1];
	}
	//mover cabeza
	switch(dir){
		case arriba:{
			cuerpo[0].y --;
			break;
		}
		case abajo:{
			cuerpo[0].y ++;
			break;
		}
		case izquierda:{
			cuerpo[0].x --;
			break;
		}
		case derecha:{
			cuerpo[0].x ++;
			break;
		}
	}
	//rutinas para comer
	if(cuerpo[0].x==comida_x && cuerpo[0].y==comida_y){
		play_sample(comer,300,150,1000,0);
		generar_comida();
		tam++;
		cuerpo[tam]=cuerpo[tam-1];
		punto++;
		if(punto%10==0 && punto>0 && *vel>30){
			*vel -=5;
		}
	}	
	//colisiones con bordes
	if(cuerpo[0].x<1 || cuerpo[0].x>(limite_x+6) || cuerpo[0].y<1 || cuerpo[0].y>(limite_y+3)){
		play_sample(muerte,300,150,1000,0);
		perder_vida();
		generar_comida();
		generar_snake();
		dir=derecha;
	}
	//colicion con nuestro cuerpo
	for(int i=1;i<=tam;i++){
		if(cuerpo[0].x == cuerpo[i].x && cuerpo[0].y == cuerpo[i].y){
			play_sample(muerte,300,150,1000,0);
			perder_vida();
			generar_comida();
			generar_snake();
			dir=derecha;
			break;
		}
	}
}

void perder_vida(){
	vidas--;
	if(vidas>0){
		clear(screen);
		textout_ex(screen,font,"Has perdido una vida",40*9,40*7,0xFFFFFF,0x000000);
		rest(1000);
	}else{
		clear(screen);
		textout_ex(screen,font,"Game Over",40*10,40*6,0xFF0000,0x000000);
		rest(2000);
		salir=0;
		//simulate_keypress(key[KEY_ESC]);
		//exit(0);
	}
}

void generar_comida(){
	comida_x=(rand()%(limite_x-2))+1;
	comida_y=(rand()%(limite_y-3))+2;
	int d;
	while(d<=tam){
		if(comida_x == cuerpo[d].x && comida_y == cuerpo[d].y){
			comida_x=(rand()%(limite_x-2))+1;
			comida_y=(rand()%(limite_y-3))+2;
			d=0;
		}else{
			d++;
		}
	}
}

void dibujar_puntos(BITMAP *buffer){
	std::stringstream ss;
	ss<<"Puntos: "<<punto;
	std::string p=ss.str();
	char puntaje[20];
	strcpy(puntaje,p.c_str());
	textout_ex(buffer,font,puntaje,50,5,0xffffff,0x000000);
}

void dibujar_comida(BITMAP *buffer,BITMAP *comida){
	draw_sprite(buffer,comida,comida_x*30,comida_y*30);
}

void dibujar_vida(BITMAP *buffer,BITMAP *vidass,BITMAP *vida){
	textout_ex(buffer,font,"Vidas:",740,5,0xffffff,0x000000);
	blit(vidass,vida,vidas*30,0,0,0,30,30);
	draw_sprite(buffer,vida,2*30+740,0);
}

int get_salir(){
	return salir;
}
/*
void escribirArchivo(){
	ofstream archivo;
	string nombre;
	archivo.open("Jugadores.txt",ios::out|ios::app);
	if (archivo.fail()){
		textout_ex(screen,font,"Error al abrir el archivo",40*9,40*7,0xFFFFFF,0x000000);
	}
	clear(screen);
	while(!key[KEY_ENTER]){	
	textout_ex(screen,font,"Ingrese su nombre: ",40*9,40*7,0xFFFFFF,0x000000);
	//getline(cin,nombre);
	cin>>nombre;
	//escribir();
	archivo << nombre <<endl;
	archivo.close();
	}
}

void leerArchivo(BITMAP *bonus){
	ifstream archivo;
	archivo.open("Jugadores.txt",ios::out);
	char linea[200];
	
	if(archivo.fail()){
		cerr<<"Error al abrir el archivo";
	}else{
		while(!archivo.eof()){ 
			archivo.getline(linea,sizeof(linea));
			std::stringstream tt;
			tt<<linea;
			std::string l = tt.str();
			char copy[200];
			strcpy(copy,l.c_str());
			textout_ex(bonus,font,copy,45*9,15*7,0xFFFFFF,0x000000);	
		}
	}
	archivo.close();
}*/
