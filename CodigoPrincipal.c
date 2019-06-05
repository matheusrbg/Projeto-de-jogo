#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

//Pode mudar dependendo do tamanho da tela do computador
#define Altura_tela 700
#define Largura_tela 900

//Structs de objetos 
typedef struct{
    int x, y;
    int pos_x, pos_y;
    int mov_x, mov_y;
    int vivo;
    int defendendo;
    int bola;
    int tempo;
    int mover;
}Jogadores;

typedef struct{
    int x, y;
    int pos_x, pos_y;
    int mov_x, mov_y;
    int tempo;
    int segurada;
    int mover;
}bola;

typedef struct{
    int x, y;
    int pos_x, pos_y;
    int mover;
}obstaculo;

//Divide por 10 e mutiplica por 2;
#define largura_sprite 407
#define altura_sprite 434
#define largura_new 81.4
#define altura_new 86.8

//Parametros principais
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_BITMAP *piso = NULL;
ALLEGRO_BITMAP *parede_topo = NULL;
ALLEGRO_BITMAP *parede_lados = NULL;
ALLEGRO_BITMAP *parede_embaixo = NULL;
ALLEGRO_BITMAP *sprite = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_EVENT evento;
int sair=1;

//RGB
#define vermelho 0
#define verde 123
#define azul 95

//Fecha tudo
void finalizar(){
    al_destroy_display(janela);
    al_destroy_bitmap(piso);
    al_destroy_bitmap(parede_lados);
    al_destroy_bitmap(parede_topo);
    al_destroy_bitmap(parede_embaixo);
    al_destroy_bitmap(sprite);
    al_destroy_event_queue(fila_eventos);
}

//Ideia de mensagem de erro
void erro(char *mensagem){
    al_show_native_message_box(NULL,"ERRO",
        "Ocorreu o seguinte erro e o programa sera finalizado:",
        mensagem,NULL,ALLEGRO_MESSAGEBOX_ERROR);
}


//Faz os principais e os checa
int inicializar(){
    al_init();
    al_init_image_addon();
    janela = al_create_display(Largura_tela,Altura_tela);
    piso = al_load_bitmap("Sprites/Piso.png");
    parede_embaixo = al_load_bitmap("Sprites/Paredes1.png");
    parede_topo = al_load_bitmap("Sprites/Paredes2.png");
    parede_lados = al_load_bitmap("Sprites/Paredes3.png");
    fila_eventos = al_create_event_queue();
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    return 1;
}

void imprimirPiso(){
    int parar=0;
    int i, j=0;
    int altura=al_get_bitmap_height(piso);
    int largura=al_get_bitmap_width(piso);
    
    for (i=0;!parar;){
        al_draw_bitmap(piso, i, j, 0);
        i+=largura;
        if (i>=Largura_tela){
            i=0;
            j+=altura;
        }
        if (j>=Altura_tela){
            parar=1;
        }
    }
}

void imprimirParedes(){
    int i, j;
    int altura, largura;
    int limite;

    //Imprime o topo
    largura = al_get_bitmap_width(parede_topo);
    altura = al_get_bitmap_height(parede_topo);
    for (i=0, j=0;i<=Largura_tela;) {
        al_draw_bitmap(parede_topo, i, j, 0);
        i+=largura;
    }

    //Imprime de lados
    largura = al_get_bitmap_width(parede_lados);
    altura = al_get_bitmap_height(parede_lados);
    limite = al_get_bitmap_height(parede_embaixo);
    for (i=0, j=0+al_get_bitmap_height(parede_topo); j<=Altura_tela-limite;){
        al_draw_bitmap(parede_lados, i, j, 0);
        j+=altura;
    }

    for (i=Largura_tela-al_get_bitmap_width(parede_lados), j=0+al_get_bitmap_height(parede_topo); j<=Altura_tela-limite;){
        al_draw_bitmap(parede_lados,i,j,0);
        j+=altura;
    }

    //imprime embaixo;
    largura = al_get_bitmap_width(parede_embaixo);
    altura = al_get_bitmap_height(parede_embaixo);
    for (i=0, j=Altura_tela-altura;i<=Largura_tela;){
        al_draw_bitmap(parede_embaixo,i,j,0);
        i+=largura;
    }
    
}

void imprimirSprites(){
    sprite = al_load_bitmap("Sprites/Personagens/Boy.jpg");
    al_convert_mask_to_alpha(sprite, al_map_rgb(vermelho,verde,azul));
    al_draw_scaled_bitmap(sprite, 0, 0, largura_sprite, altura_sprite, 50, 100, 81.4, 86.8, 0);
}

void eventos(){
    while(!al_is_event_queue_empty(fila_eventos)){//Permanecerá até todos os eventos serem tratados
    al_wait_for_event(fila_eventos, &evento);
        if (evento.type==ALLEGRO_EVENT_DISPLAY_CLOSE){
            int resp = al_show_native_message_box(janela,"Fechar",
				"Deseja sair do programa?","",NULL,ALLEGRO_MESSAGEBOX_YES_NO);
			if (resp==1) sair=0;
        }
    }
}

int main(){
    inicializar(); //Função com a parte de sprites e ponteiros
    al_set_window_title(janela, "Queimada");
    
    while(sair){
    imprimirPiso();
    imprimirParedes();
    imprimirSprites();
    al_flip_display();
    eventos();
    }
    

    finalizar();//Função para acabar com todos os processos de ponteiros.
    return 0;
}