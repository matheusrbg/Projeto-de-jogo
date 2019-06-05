#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <allegro5/allegro_primitives.h>


const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;

typedef struct{
    
    float dx, dy, Jx1, Jy1, Jx2, Jy2;
    float alturaPlayer, larguraPlayer;
    float vx, vy;
    int morreu;

}Jogador;

typedef struct{

    float a; //tamanho do quadrado vermelho
    float Rx1, Ry1, Rx2, Ry2;

}qVermelho;

typedef struct{

    int pegou;
    int andando; //se ta andando ou n
    float vx, vy; //Velocidade nos eixos x e y, respectivamente
    float alturaBola , larguraBola; 
    float dx, dy, Px1, Py1, Px2, Py2;

}Bala;

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
    float pegarbox = 30; //Distancia para pegar
    Bala b;
    qVermelho q;
    Jogador p;
    
    //Inicializando Quadrado
    q.a = 50;
    q.Rx1 = (LARGURA_TELA/2)-q.a/2;
    q.Rx2 = (LARGURA_TELA/2)+q.a/2;
    q.Ry1 = (ALTURA_TELA/2)-q.a/2;
    q.Ry2 = (ALTURA_TELA/2)+q.a/2;
 
    //Inicializando bola
    b.pegou=0;
    b.andando = 1; 
    b.alturaBola = 10;
    b.larguraBola = 10;
    b.vx = 4; 
    b.vy = 8;
    b.dy = 0;
    b.dx = 0;
    b.Px1 = 20;
    b.Py1 = 20;
    b.Px2 = b.Px1 + b.larguraBola;
    b.Py2 = b.Py1 + b.alturaBola;

    //Inicializando Jogador
    p.morreu=0;
    p.alturaPlayer = 50;
    p.larguraPlayer = 30;
    p.vx = 8; 
    p.vy = 8;
    p.dy = 0;
    p.dx = 0;
    p.Jx1 = 400;
    p.Jy1 = 400;
    p.Jx2 = p.Jx1 + p.larguraPlayer;
    p.Jy2 = p.Jy1 + p.alturaPlayer;
    
    int tecla=0, sair=0, teclaAnterior=0, teclaAnteriorAnterior =0, i=0,framesParaJogar = 40, teto;
    teto = 2*framesParaJogar;

    while(!sair){ //Jogo

        if(!i%framesParaJogar) teclaAnteriorAnterior = teclaAnterior;

        while(!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;

            al_wait_for_event(fila_eventos, &evento);

            if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(evento.keyboard.keycode)
                {
                case ALLEGRO_KEY_SPACE:
                    tecla = 1;
                    break;
                case ALLEGRO_KEY_W:
                    tecla = 2;
                    break;
                case ALLEGRO_KEY_A:
                    tecla = 3;
                    break;
                case ALLEGRO_KEY_S:
                    tecla = 4;
                    break;
                case ALLEGRO_KEY_D:
                    tecla = 5;
                    break;
                case ALLEGRO_KEY_ENTER:
                    tecla = 6;
                    break;
                case ALLEGRO_KEY_G:
                    tecla = 7;
                    break;
                default : tecla=0;
                }
            }
            else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) sair=1;
            
            switch(tecla){
                case 1: if(((b.Px2 + b.dx >= p.Jx1 + p.dx - pegarbox && b.Px2 + b.dx <= p.Jx2+ p.dx + pegarbox)&&(b.Px1 + b.dx >= p.Jx1 + p.dx- pegarbox && b.Px1 + b.dx <= p.Jx2+ p.dx + pegarbox)) && ((b.Py2 + b.dy >= p.Jy1 + p.dy - pegarbox && b.Py2 +b.dy <= p.Jy2+p.dy + pegarbox)&&(b.Py1 + b.dy >= p.Jy1 + p.dy - pegarbox && b.Py1 +b.dy <= p.Jy2+p.dy + pegarbox))){
                            b.andando = 0;
                            b.pegou=1; 
                        }break;
                case 2: p.dy -= p.vy;
                    if(b.pegou) b.dy -= p.vy; break;
                case 3: p.dx -= p.vx;
                    if(b.pegou) b.dx -= p.vx; break;
                case 4: p.dy += p.vy;
                    if(b.pegou) b.dy += p.vy; break;
                case 5: p.dx += p.vx;
                    if(b.pegou) b.dx += p.vx; break;
                case 6: p.morreu=0; p.dx = 0; p.dy = 0;  break;
                case 7: if(b.pegou){ switch(teclaAnterior){
                            case 2: b.vy = -8;
                                switch(teclaAnteriorAnterior){
                                    case 3: b.vx = -8; break;
                                    case 5: b.vx = 8; break;
                                    default: b.vx = 0;
                                } break;
                            case 3: b.vx = -8;
                                switch(teclaAnteriorAnterior){
                                    case 2: b.vy = -8; break;
                                    case 4: b.vy = 8; break;
                                    default: b.vy = 0;
                                } break;
                            case 4: b.vy = 8;
                                switch(teclaAnteriorAnterior){
                                    case 3: b.vx = -8; break;
                                    case 5: b.vx = 8; break;
                                    default: b.vx = 0;
                                } break;
                            case 5: b.vx = 8;
                                switch(teclaAnteriorAnterior){
                                    case 2: b.vy = -8; break;
                                    case 4: b.vy = 8; break;
                                    default: b.vy = 0;
                                } break;
                        } 
                    b.andando=1; 
                    b.pegou =0;
                    }  break;
                default: tecla = 0;
            } 
        }
        teclaAnterior = tecla;

        if(b.andando){
            b.dx += b.vx;
            b.dy += b.vy;
        }

        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_filled_rectangle( q.Rx1 ,q.Ry1,q.Rx2,q.Ry2, al_map_rgb(255, 0, 0));

        //Colisao jogador com o vermelho
        if((p.Jx2 + p.dx >= q.Rx1 && p.Jx2 + p.dx <= q.Rx2) && ((p.Jy2 + p.dy >= q.Ry1 && p.Jy2 +p.dy <= q.Ry2)||(p.Jy1 + p.dy >= q.Ry1 && p.Jy1 +p.dy <= q.Ry2))) p.dx -= p.vx; //Colisao pela direita
        if((p.Jx1 + p.dx >= q.Rx1 && p.Jx1 + p.dx <= q.Rx2) && ((p.Jy2 + p.dy >= q.Ry1 && p.Jy2 +p.dy <= q.Ry2)||(p.Jy1 + p.dy >= q.Ry1 && p.Jy1 +p.dy <= q.Ry2))) p.dx += p.vx; //Colisao pela esquerda
        if((p.Jy2 + p.dy >= q.Ry1 && p.Jy2 + p.dy <= q.Ry2) && ((p.Jx1 + p.dx >= q.Rx1 && p.Jx1 +p.dx <= q.Rx2)||(p.Jx2 + p.dx >= q.Rx1 && p.Jx2 +p.dx <= q.Rx2))) p.dy -= p.vy; //Colisao por baixo
        if((p.Jy1 + p.dy >= q.Ry1 && p.Jy1 + p.dy <= q.Ry2) && ((p.Jx1 + p.dx >= q.Rx1 && p.Jx1 +p.dx <= q.Rx2)||(p.Jx2 + p.dx >= q.Rx1 && p.Jx2 +p.dx <= q.Rx2))) p.dy += p.vy; //Colisao por cima

        //Colisao jogador com borda
        if((p.Jx2 + p.dx >= LARGURA_TELA || p.Jx2 + p.dx <= 0)) p.dx -= p.vx; //Colisao pela direita
        if((p.Jx1 + p.dx >= LARGURA_TELA || p.Jx1 + p.dx <= 0)) p.dx += p.vx; //Colisao pela esquerda
        if((p.Jy2 + p.dy >= ALTURA_TELA || p.Jy2 + p.dy <= 0)) p.dy -= p.vy; //Colisao por baixo
        if((p.Jy1 + p.dy >= ALTURA_TELA || p.Jy1 + p.dy <= 0)) p.dy += p.vy; //Colisao por cima

        //Colisao bola com o Vermelho
        if((b.Px2 + b.dx >= q.Rx1 && b.Px2 + b.dx <= q.Rx2) && ((b.Py2 + b.dy >= q.Ry1 && b.Py2 +b.dy <= q.Ry2)||(b.Py1 + b.dy >= q.Ry1 && b.Py1 +b.dy <= q.Ry2))) b.vx = -b.vx; //Colisao pela direita
        if((b.Px1 + b.dx >= q.Rx1 && b.Px1 + b.dx <= q.Rx2) && ((b.Py2 + b.dy >= q.Ry1 && b.Py2 +b.dy <= q.Ry2)||(b.Py1 + b.dy >= q.Ry1 && b.Py1 +b.dy <= q.Ry2))) b.vx = -b.vx; //Colisao pela esquerda
        if((b.Py2 + b.dy >= q.Ry1 && b.Py2 + b.dy <= q.Ry2) && ((b.Px1 + b.dx >= q.Rx1 && b.Px1 +b.dx <= q.Rx2)||(b.Px2 + b.dx >= q.Rx1 && b.Px2 +b.dx <= q.Rx2))) b.vy = -b.vy; //Colisao por baixo
        if((b.Py1 + b.dy >= q.Ry1 && b.Py1 + b.dy <= q.Ry2) && ((b.Px1 + b.dx >= q.Rx1 && b.Px1 +b.dx <= q.Rx2)||(b.Px2 + b.dx >= q.Rx1 && b.Px2 +b.dx <= q.Rx2))) b.vy = -b.vy; //Colisao por cima
        
        //Colisao bola com a borda
        if((b.Px2 + b.dx >= LARGURA_TELA || b.Px2 + b.dx <= 0)) b.vx = -b.vx; //Colisao pela direita
        if((b.Px1 + b.dx >= LARGURA_TELA || b.Px1 + b.dx <= 0)) b.vx = -b.vx; //Colisao pela esquerda
        if((b.Py2 + b.dy >= ALTURA_TELA || b.Py2 + b.dy <= 0)) b.vy = -b.vy; //Colisao por baixo
        if((b.Py1 + b.dy >= ALTURA_TELA || b.Py1 + b.dy <= 0)) b.vy = -b.vy; //Colisao por cima
        
        //Bugou a bola com o quadrado
        if(!b.pegou)if(((b.Px2 + b.dx >= q.Rx1 && b.Px2 + b.dx <= q.Rx2)&&(b.Px1 + b.dx >= q.Rx1 && b.Px1 + b.dx <= q.Rx2)) && ((b.Py2 + b.dy >= q.Ry1 && b.Py2 +b.dy <= q.Ry2)&&(b.Py1 + b.dy >= q.Ry1 && b.Py1 +b.dy <= q.Ry2))) {
            b.dx=0; 
            b.dy=0;
            al_draw_filled_rectangle( b.Px1 + b.dx,b.Py1 + b.dy,b.Px2 + b.dx ,b.Py2 + b.dy, al_map_rgb(255, 255, 0));
            al_flip_display();
            al_rest(10.0/60.0);
            al_draw_filled_rectangle( b.Px1 + b.dx,b.Py1 + b.dy,b.Px2 + b.dx ,b.Py2 + b.dy, al_map_rgb(255, 0, 255));
            al_flip_display();
            al_rest(10.0/60.0);
            al_draw_filled_rectangle( b.Px1 + b.dx,b.Py1 + b.dy,b.Px2 + b.dx ,b.Py2 + b.dy, al_map_rgb(0, 255, 255));
            al_flip_display();
            al_rest(10.0/60.0);
        }
        //Bugou a bola com a tela 
        if(((b.Px2 + b.dx >= LARGURA_TELA && b.Px2 + b.dx >= LARGURA_TELA)||(b.Px1 + b.dx <= 0 && b.Px1 + b.dx <= 0)) && ((b.Py2 + b.dy >= ALTURA_TELA && b.Py2 +b.dy >= ALTURA_TELA)||(b.Py1 + b.dy <= 0 && b.Py1 +b.dy <= 0))) {
            b.dx=0; 
            b.dy=0;

            al_draw_filled_rectangle( b.Px1 + b.dx,b.Py1 + b.dy,b.Px2 + b.dx ,b.Py2 + b.dy, al_map_rgb(255, 255, 0));
            al_flip_display();
            al_rest(10.0/60.0);
            al_draw_filled_rectangle( b.Px1 + b.dx,b.Py1 + b.dy,b.Px2 + b.dx ,b.Py2 + b.dy, al_map_rgb(255, 0, 255));
            al_flip_display();
            al_rest(10.0/60.0);
            al_draw_filled_rectangle( b.Px1 + b.dx,b.Py1 + b.dy,b.Px2 + b.dx ,b.Py2 + b.dy, al_map_rgb(0, 255, 255));
            al_flip_display();
            al_rest(10.0/60.0);
        }

        //Colisao bola com o player
        if(((b.Px2 + b.dx >= p.Jx1 + p.dx && b.Px2 + b.dx <= p.Jx2+ p.dx)||(b.Px1 + b.dx >= p.Jx1 + p.dx && b.Px1 + b.dx <= p.Jx2+ p.dx)) && ((b.Py2 + b.dy >= p.Jy1 + p.dy && b.Py2 +b.dy <= p.Jy2+p.dy)||(b.Py1 + b.dy >= p.Jy1 + p.dy && b.Py1 +b.dy <= p.Jy2+p.dy))) {
            b.andando = !b.andando;
            p.morreu = 1;
            b.pegou = 0;
            p.dx = -p.Jx2;
            p.dy = -p.Jy2;

            al_draw_filled_rectangle( b.Px1 + b.dx,b.Py1 + b.dy,b.Px2 + b.dx ,b.Py2 + b.dy, al_map_rgb(255, 255, 0));
            al_flip_display();
            al_rest(10.0/60.0);
            al_draw_filled_rectangle( b.Px1 + b.dx,b.Py1 + b.dy,b.Px2 + b.dx ,b.Py2 + b.dy, al_map_rgb(255, 0, 255));
            al_flip_display();
            al_rest(10.0/60.0);
            al_draw_filled_rectangle( b.Px1 + b.dx,b.Py1 + b.dy,b.Px2 + b.dx ,b.Py2 + b.dy, al_map_rgb(0, 255, 255));
            al_flip_display();
            al_rest(10.0/60.0);
        }

        al_draw_filled_rectangle( b.Px1 + b.dx,b.Py1 + b.dy,b.Px2 + b.dx ,b.Py2 + b.dy, al_map_rgb(255, 255, 255));
        if(!p.morreu) al_draw_filled_rectangle( p.Jx1 + p.dx, p.Jy1 + p.dy, p.Jx2 + p.dx , p.Jy2 + p.dy, al_map_rgb(0, 255, 0));
        
        //Printando até onde o jogador pode pegar a bola
        al_draw_rectangle( p.Jx1 + p.dx - pegarbox, p.Jy1 + p.dy - pegarbox, p.Jx2 + p.dx + pegarbox, p.Jy2 + p.dy + pegarbox,  al_map_rgb(255, 255, 0), 5.5);
        
        al_flip_display();
        i = (i+1)%teto;
        al_rest(1.0/60.0);
    }
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    return 0;
}


