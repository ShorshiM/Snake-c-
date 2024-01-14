void perder_vida();
void generar_comida();
void dibujar_mapa(BITMAP *buffer,BITMAP *pared,BITMAP *piso,BITMAP *cesped,BITMAP *adorno);
void pantalla(BITMAP *buffer,BITMAP *screen);
void generar_snake();
void dibujar_snake(BITMAP *snakeybmp,BITMAP *snakey,BITMAP *buffer,BITMAP *cuerpo_iz);
void actualizar_direccion();
void mover_snake(int *vel,SAMPLE *comer,SAMPLE *muerte);
void dibujar_puntos(BITMAP *buffer);
void dibujar_comida(BITMAP *buffer,BITMAP *comida);
void dibujar_vida(BITMAP *buffer,BITMAP *vidass,BITMAP *vida);
void escribirArchivo();
void leerArchivo();
void reinicio(int *vel);
int get_salir();
//void leerArchivo(BITMAP *bonus);
//void escribirArchivo();
//void escribir();

