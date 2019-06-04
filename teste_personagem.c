#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <allegro5/allegro_primitives.h>


const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL;

int inicializar()
{
    if (!al_init())
    {
        printf("Falha ao inicializar a Allegro.\n");
        return 0;
    }

    if (!al_init_image_addon())
    {
        printf("Falha ao inicializar add-on allegro_image.\n");
        return 0;
    }

    if (!al_install_keyboard())
    {
        printf("Falha ao inicializar o teclado.\n");
        return 0;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        printf("Falha ao criar janela.\n");
        return 0;
    }

    al_set_window_title(janela, "Utilizando o Teclado");

    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        printf("Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return 0;
    }

    if (!al_init_primitives_addon())
    {
        printf("Falha ao inicializar add-on de primitivas.\n");
        return 0;
    }

    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    return 1;
}

int main(void)
{
    if(!inicializar()) return -1;
    float v=10;
    float dx=0, dy=0, Px1 = 20, Py1=20, Px2=30, Py2=30, Rx1=50, Ry1=50, Rx2=100, Ry2=100;
    int tecla=0, sair=0;
    while(!sair){
        while(!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;

            al_wait_for_event(fila_eventos, &evento);

            if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(evento.keyboard.keycode)
                {
                case ALLEGRO_KEY_S:
                    tecla = 1;
                    break;
                case ALLEGRO_KEY_W:
                    tecla = 2;
                    break;
                case ALLEGRO_KEY_A:
                    tecla = 3;
                    break;
                case ALLEGRO_KEY_D:
                    tecla = 4;
                    break;
                }
            }
            else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) sair=1;
            if(tecla){
                 switch(tecla){
                    case 1: dy += v; break;
                    case 2: dy -= v; break;
                    case 3: dx -= v; break;
                    case 4: dx += v; break;
                 }
            }
        }
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_filled_rectangle( Rx1 ,Ry1,Rx2,Ry2, al_map_rgb(255, 0, 0));

        //Colisao com o Vermelho
        if((Px2 + dx >= Rx1 && Px2 + dx <= Rx2) && ((Py2 + dy >= Ry1 && Py2 +dy <= Ry2)||(Py1 + dy >= Ry1 && Py1 +dy <= Ry2))) dx -=2*v; //Colisao pela direita
        if((Px1 + dx >= Rx1 && Px1 + dx <= Rx2) && ((Py2 + dy >= Ry1 && Py2 +dy <= Ry2)||(Py1 + dy >= Ry1 && Py1 +dy <= Ry2))) dx +=2*v; //Colisao pela esquerda
        if((Py2 + dy >= Ry1 && Py2 + dy <= Ry2) && ((Px1 + dx >= Rx1 && Px1 +dx <= Rx2)||(Px2 + dx >= Rx1 && Px2 +dx <= Rx2))) dy -=2*v; //Colisao por baixo
        if((Py1 + dy >= Ry1 && Py1 + dy <= Ry2) && ((Px1 + dx >= Rx1 && Px1 +dx <= Rx2)||(Px2 + dx >= Rx1 && Px2 +dx <= Rx2))) dy +=2*v; //Colisao por cima
        
        //Colisao com a borda
        if((Px2 + dx >= LARGURA_TELA || Px2 + dx <= 0)) dx -=2*v; //Colisao pela direita
        if((Px1 + dx >= LARGURA_TELA || Px1 + dx <= 0)) dx +=2*v; //Colisao pela esquerda
        if((Py2 + dy >= ALTURA_TELA || Py2 + dy <= 0)) dy -=2*v; //Colisao por baixo
        if((Py1 + dy >= ALTURA_TELA || Py1 + dy <= 0)) dy +=2*v; //Colisao por cima
        
        if(((Px2 + dx >= Rx1 && Px2 + dx <= Rx2)&&(Px1 + dx >= Rx1 && Px1 + dx <= Rx2)) && ((Py2 + dy >= Ry1 && Py2 +dy <= Ry2)&&(Py1 + dy >= Ry1 && Py1 +dy <= Ry2))) {
            dx=0; 
            dy=0;
            al_draw_filled_rectangle( Px1 + dx,Py1 + dy,Px2 + dx ,Py2 + dy, al_map_rgb(255, 255, 0));
            al_flip_display();
            al_rest(10.0/60.0);
            al_draw_filled_rectangle( Px1 + dx,Py1 + dy,Px2 + dx ,Py2 + dy, al_map_rgb(255, 0, 255));
            al_flip_display();
            al_rest(10.0/60.0);
            al_draw_filled_rectangle( Px1 + dx,Py1 + dy,Px2 + dx ,Py2 + dy, al_map_rgb(0, 255, 255));
            al_flip_display();
            al_rest(10.0/60.0);
        }
        if(((Px2 + dx >= LARGURA_TELA && Px2 + dx >= LARGURA_TELA)||(Px1 + dx <= 0 && Px1 + dx <= 0)) && ((Py2 + dy >= ALTURA_TELA && Py2 +dy >= ALTURA_TELA)||(Py1 + dy <= 0 && Py1 +dy <= 0))) {
            dx=0; 
            dy=0;
            al_draw_filled_rectangle( Px1 + dx,Py1 + dy,Px2 + dx ,Py2 + dy, al_map_rgb(255, 255, 0));
            al_flip_display();
            al_rest(10.0/60.0);
            al_draw_filled_rectangle( Px1 + dx,Py1 + dy,Px2 + dx ,Py2 + dy, al_map_rgb(255, 0, 255));
            al_flip_display();
            al_rest(10.0/60.0);
            al_draw_filled_rectangle( Px1 + dx,Py1 + dy,Px2 + dx ,Py2 + dy, al_map_rgb(0, 255, 255));
            al_flip_display();
            al_rest(10.0/60.0);
        }

        al_draw_filled_rectangle( Px1 + dx,Py1 + dy,Px2 + dx ,Py2 + dy, al_map_rgb(255, 255, 255));
        al_flip_display();
        al_rest(1.0/60.0);
    }
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    return 0;
}


