#include <allegro.h>
#include <time.h>
#include <iostream>
#include <ctime>
#include <sstream>
#include <time.h>
#include <fstream>
#include "cabecera.h"
using namespace std;

BITMAP *buffer;
BITMAP *fondo_1;
BITMAP *fondo_2;
BITMAP *fondo_3;
BITMAP *fondo_4;
BITMAP *cursor;
BITMAP *pared;
BITMAP *piso;
BITMAP *cesped;
BITMAP *adorno;
BITMAP *snakeybmp;
BITMAP *snakey;
BITMAP *cuerpo_iz;
BITMAP *comida;
BITMAP *vidass;
BITMAP *vida;
BITMAP *bonus;
MIDI *musica;
SAMPLE *comer;
SAMPLE *muerte;
int vel=100;
int pausa=0;
int nom=0;

int main() {
	allegro_init();
	install_mouse();
	install_keyboard();
	//inicializa el audio 
	if(install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL) !=0){
		allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
		return 1;
	}
	//configura el volumen
	set_volume(70,70);
	musica = load_midi("musica/Springtime.mid");
	comer = load_wav("musica/snake_animado.wav");
	muerte = load_wav("musica/muerte.wav");
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 900, 600, 0, 0);
	//ALLEGRO_EVENT_QUEUE* event_queue = create_event_queue();
	srand(time(NULL));
	set_window_title("Snakey");
	buffer = create_bitmap(900,600);
	pared = load_bitmap("sprite/pared.bmp",NULL);
	piso = load_bitmap("sprite/piso.bmp",NULL);
	cesped = load_bitmap("sprite/cesped.bmp",NULL);
	adorno = load_bitmap("sprite/adorno.bmp",NULL);
	snakeybmp = load_bitmap("sprite/snakey_c.bmp",NULL);
	snakey = create_bitmap(30,30);
	cuerpo_iz = load_bitmap("sprite/cuerpo_iz.bmp",NULL);
	comida = load_bitmap("sprite/fruta.bmp",NULL);
	vidass = load_bitmap("sprite/vidas.bmp",NULL);
	vida = create_bitmap(30,30);
	fondo_1 = load_bitmap("sprite/pantalla_1.bmp",NULL);
	fondo_2 = load_bitmap("sprite/pantalla_2.bmp",NULL);
	fondo_3 = load_bitmap("sprite/pantalla_3.bmp",NULL);
	fondo_4 = load_bitmap("sprite/pantalla_4.bmp",NULL);
	bonus = load_bitmap("sprite/pantalla_puntaje.bmp",NULL);
	cursor = load_bitmap("sprite/cursor.bmp",NULL);
	bool salida=false;
	play_midi(musica,1);
	while(!salida){
		if(mouse_x > 18 && mouse_x < 360 && mouse_y > 502 && mouse_y < 575){
			blit(fondo_2,buffer,0,0,0,0,900,600);
			vel=100;
			if(mouse_b & 1){
				while(nom!=1){
					//escribirArchivo();
					nom=1;
				}
				nom=0;
				clear(buffer);
				reinicio(&vel);
				dibujar_mapa(buffer,pared,piso,cesped,adorno);
				generar_snake();
				generar_comida();
				dibujar_snake(snakeybmp,snakey,buffer,cuerpo_iz);
				dibujar_comida(buffer,comida);
				dibujar_puntos(buffer);
				dibujar_vida(buffer,vidass,vida);
				pantalla(buffer,screen);
				while(!key[KEY_ESC] || get_salir()==0){
					actualizar_direccion();
					mover_snake(&vel,comer,muerte);
					dibujar_mapa(buffer,pared,piso,cesped,adorno);
					dibujar_snake(snakeybmp,snakey,buffer,cuerpo_iz);
					dibujar_comida(buffer,comida);
					dibujar_puntos(buffer);
					dibujar_vida(buffer,vidass,vida);
					pantalla(buffer,screen);
					rest(vel);			
				}
				//escribirArchivo();
				//rest (2000);
				//leerArchivo();
			}
		}else{
			if(mouse_x > 561 && mouse_x < 809 && mouse_y > 488 && mouse_y < 587){
				blit(fondo_3,buffer,0,0,0,0,900,600);
				if(mouse_b & 1){
					salida = true;
				}
			}else{
				if(mouse_x > 181 && mouse_x < 706 && mouse_y > 27 && mouse_y < 210){
					blit(fondo_4,buffer,0,0,0,0,900,600);
					if(mouse_b & 1){
						pausa=1;
					}
					if(pausa==1){
						blit(bonus,buffer,0,0,0,0,900,600);
						//textout_ex(bonus,font,"RECORDS",45*9,10*7,0xFFFFFF,0x000000);
						//leerArchivo(bonus);
					}
					if(key[KEY_ENTER]){
						pausa=0;	
					}		
				}else{
					blit(fondo_1,buffer,0,0,0,0,900,600);
				}
			}
		}
		masked_blit(cursor,buffer,0,0,mouse_x,mouse_y,13,22);
		blit(buffer,screen,0,0,0,0,900,600);
	}
	
	return 0;
}
END_OF_MAIN();

