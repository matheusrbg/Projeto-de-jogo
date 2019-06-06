#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <allegro5/allegro_primitives.h>


const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;

typedef struct{
    float dx, dy, Px1, Py1, Px2, Py2;
    float vx, vy;
}Posicao;

typedef struct{

    Posicao posP;
    float alturaPlayer, larguraPlayer;
    int morreu;

}Jogador;

typedef struct{

    float a; //tamanho do quadrado vermelho
    Posicao posQ;

}qVermelho;

typedef struct{

    int pegou;
    int andando; //se ta andando ou n
    float alturaBola , larguraBola;
    Posicao posB;

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

void animacaoBolaColisao(Posicao b){
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

void bugouBolaObstaculo(Bala *b, Posicao *q){
    if(!b->pegou)if(((b->posB.Px2 + b->posB.dx >= q->Px1 + q->dx && b->posB.Px2 + b->posB.dx <= q->Px2 + q->dx)&&(b->posB.Px1 + b->posB.dx >= q->Px1 + q->dx && b->posB.Px1 + b->posB.dx <= q->Px2 + q->dx)) && ((b->posB.Py2 + b->posB.dy >= q->Py1 + q->dy && b->posB.Py2 +b->posB.dy <= q->Py2 + q->dy )&&(b->posB.Py1 + b->posB.dy >= q->Py1 + q->dy && b->posB.Py1 +b->posB.dy <= q->Py2 + q->dy ))) {
            b->posB.dx=0;
            b->posB.dy=0;
            animacaoBolaColisao(b->posB);
    }
}

void bolaAceraPlayer(Bala *b, Jogador *p){
    if(!b->pegou)if(((b->posB.Px2 + b->posB.dx >= p->posP.Px1 + p->posP.dx && b->posB.Px2 + b->posB.dx <= p->posP.Px2 + p->posP.dx)||(b->posB.Px1 + b->posB.dx >= p->posP.Px1 + p->posP.dx && b->posB.Px1 + b->posB.dx <= p->posP.Px2 + p->posP.dx)) && ((b->posB.Py2 + b->posB.dy >= p->posP.Py1 + p->posP.dy && b->posB.Py2 +b->posB.dy <= p->posP.Py2 + p->posP.dy )||(b->posB.Py1 + b->posB.dy >= p->posP.Py1 + p->posP.dy && b->posB.Py1 +b->posB.dy <= p->posP.Py2 + p->posP.dy ))) {
            b->andando = !b->andando;
            p->morreu = 1;
            b->pegou = 0;
            p->posP.dx = -p->posP.Px2;
            p->posP.dy = -p->posP.Py2;
            animacaoBolaColisao(b->posB);
    }
}

void bugouBolaBorda(Posicao *b){
    if(((b->Px2 + b->dx >= LARGURA_TELA && b->Px2 + b->dx >= LARGURA_TELA)||(b->Px1 + b->dx <= 0 && b->Px1 + b->dx <= 0)) && ((b->Py2 + b->dy >= ALTURA_TELA && b->Py2 +b->dy >= ALTURA_TELA)||(b->Py1 + b->dy <= 0 && b->Py1 +b->dy <= 0))) {
            b->dx=0;
            b->dy=0;
            animacaoBolaColisao(*b);
    }
}

void colisaoBolaComBorda(Posicao *b){
    if((b->Px2 + b->dx >= LARGURA_TELA || b->Px2 + b->dx <= 0)) b->vx = -b->vx; //Colisao pela direita
    if((b->Px1 + b->dx >= LARGURA_TELA || b->Px1 + b->dx <= 0)) b->vx = -b->vx; //Colisao pela esquerda
    if((b->Py2 + b->dy >= ALTURA_TELA || b->Py2 + b->dy <= 0)) b->vy = -b->vy; //Colisao por baixo
    if((b->Py1 + b->dy >= ALTURA_TELA || b->Py1 + b->dy <= 0)) b->vy = -b->vy; //Colisao por cima
}

void colisaoComBorda(Posicao *p){
    if((p->Px2 + p->dx >= LARGURA_TELA || p->Px2 + p->dx <= 0)) p->dx -= p->vx; //Colisao pela direita
    if((p->Px1 + p->dx >= LARGURA_TELA || p->Px1 + p->dx <= 0)) p->dx += p->vx; //Colisao pela esquerda
    if((p->Py2 + p->dy >= ALTURA_TELA || p->Py2 + p->dy <= 0)) p->dy -= p->vy; //Colisao por baixo
    if((p->Py1 + p->dy >= ALTURA_TELA || p->Py1 + p->dy <= 0)) p->dy += p->vy; //Colisao por cima
}

void colisaoBolaComObstaculo(Posicao *b, Posicao *q){
    if((b->Px2 + b->dx >= q->Px1 + q->dx  && b->Px2 + b->dx <= q->Px2 + q->dx ) && ((b->Py2 + b->dy >= q->Py1 + q->dy  && b->Py2 +b->dy <= q->Py2 + q->dy )||(b->Py1 + b->dy >= q->Py1 + q->dy  && b->Py1 +b->dy <= q->Py2 + q->dy ))) b->vx = -b->vx; //Colisao pela direita
    if((b->Px1 + b->dx >= q->Px1 + q->dx  && b->Px1 + b->dx <= q->Px2 + q->dx ) && ((b->Py2 + b->dy >= q->Py1 + q->dy  && b->Py2 +b->dy <= q->Py2 + q->dy )||(b->Py1 + b->dy >= q->Py1 + q->dy && b->Py1 +b->dy <= q->Py2 + q->dy ))) b->vx = -b->vx; //Colisao pela esquerda
    if((b->Py2 + b->dy >= q->Py1 + q->dy  && b->Py2 + b->dy <= q->Py2 + q->dy ) && ((b->Px1 + b->dx >= q->Px1 + q->dx  && b->Px1 +b->dx <= q->Px2 + q->dx )||(b->Px2 + b->dx >= q->Px1 + q->dx  && b->Px2 +b->dx <= q->Px2 + q->dx ))) b->vy = -b->vy; //Colisao por baixo
    if((b->Py1 + b->dy >= q->Py1 + q->dy  && b->Py1 + b->dy <= q->Py2 + q->dy ) && ((b->Px1 + b->dx >= q->Px1 + q->dx  && b->Px1 +b->dx <= q->Px2 + q->dx )||(b->Px2 + b->dx >= q->Px1 + q->dx  && b->Px2 +b->dx <= q->Px2 + q->dx ))) b->vy = -b->vy; //Colisao por cima
}

void colisaoComObstaculo(Posicao *p, Posicao *q){
    if((p->Px2 + p->dx >= q->Px1 + q->dx  && p->Px2 + p->dx <= q->Px2 + q->dx ) && ((p->Py2 + p->dy >= q->Py1 + q->dy  && p->Py2 +p->dy <= q->Py2 + q->dy )||(p->Py1 + p->dy >= q->Py1 + q->dy  && p->Py1 +p->dy <= q->Py2 + q->dy ))) p->dx -= p->vx; //Colisao pela direita
    if((p->Px1 + p->dx >= q->Px1 + q->dx  && p->Px1 + p->dx <= q->Px2 + q->dx ) && ((p->Py2 + p->dy >= q->Py1 + q->dy  && p->Py2 +p->dy <= q->Py2 + q->dy )||(p->Py1 + p->dy >= q->Py1 + q->dy && p->Py1 +p->dy <= q->Py2 + q->dy ))) p->dx += p->vx; //Colisao pela esquerda
    if((p->Py2 + p->dy >= q->Py1 + q->dy  && p->Py2 + p->dy <= q->Py2 + q->dy ) && ((p->Px1 + p->dx >= q->Px1 + q->dx  && p->Px1 +p->dx <= q->Px2 + q->dx )||(p->Px2 + p->dx >= q->Px1 + q->dx  && p->Px2 +p->dx <= q->Px2 + q->dx ))) p->dy -= p->vy; //Colisao por baixo
    if((p->Py1 + p->dy >= q->Py1 + q->dy  && p->Py1 + p->dy <= q->Py2 + q->dy ) && ((p->Px1 + p->dx >= q->Px1 + q->dx  && p->Px1 +p->dx <= q->Px2 + q->dx )||(p->Px2 + p->dx >= q->Px1 + q->dx  && p->Px2 +p->dx <= q->Px2 + q->dx ))) p->dy += p->vy; //Colisao por cima
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
    q.posQ.Px1 = (LARGURA_TELA/2)-q.a/2;
    q.posQ.Px2= (LARGURA_TELA/2)+q.a/2;
    q.posQ.Py1 = (ALTURA_TELA/2)-q.a/2;
    q.posQ.Py2 = (ALTURA_TELA/2)+q.a/2;
    q.posQ.dx =0;
    q.posQ.dy =0;

    //Inicializando bola
    b.pegou=0;
    b.andando = 1;
    b.alturaBola = 10;
    b.larguraBola = 10;
    b.posB.vx = 4;
    b.posB.vy = 8;
    b.posB.dy = 0;
    b.posB.dx = 0;
    b.posB.Px1 = 20;
    b.posB.Py1 = 20;
    b.posB.Px2 = b.posB.Px1 + b.larguraBola;
    b.posB.Py2 = b.posB.Py1 + b.alturaBola;

    //Inicializando Jogador
    p.morreu=0;
    p.alturaPlayer = 50;
    p.larguraPlayer = 30;
    p.posP.vx = 8;
    p.posP.vy = 8;
    p.posP.dy = 0;
    p.posP.dx = 0;
    p.posP.Px1 = 400;
    p.posP.Py1 = 400;
    p.posP.Px2 = p.posP.Px1 + p.larguraPlayer;
    p.posP.Py2 = p.posP.Py1 + p.alturaPlayer;

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
                case ALLEGRO_KEY_UP:
                    tecla = 7;
                    break;
                default : tecla=0;
                }
            }
            else if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) sair=1;

            switch(tecla){
                case 1: if(((b.posB.Px2 + b.posB.dx >= p.posP.Px1 + p.posP.dx - pegarbox && b.posB.Px2 + b.posB.dx <= p.posP.Px2+ p.posP.dx + pegarbox)&&(b.posB.Px1 + b.posB.dx >= p.posP.Px1 + p.posP.dx- pegarbox && b.posB.Px1 + b.posB.dx <= p.posP.Px2+ p.posP.dx + pegarbox)) && ((b.posB.Py2 + b.posB.dy >= p.posP.Py1 + p.posP.dy - pegarbox && b.posB.Py2 +b.posB.dy <= p.posP.Py2+p.posP.dy + pegarbox)&&(b.posB.Py1 + b.posB.dy >= p.posP.Py1 + p.posP.dy - pegarbox && b.posB.Py1 +b.posB.dy <= p.posP.Py2+p.posP.dy + pegarbox))){
                            b.andando = 0;
                            b.pegou=1;
                        }break;
                case 2: p.posP.dy -= p.posP.vy;
                    if(b.pegou) b.posB.dy -= p.posP.vy; break;
                case 3: p.posP.dx -= p.posP.vx;
                    if(b.pegou) b.posB.dx -= p.posP.vx; break;
                case 4: p.posP.dy += p.posP.vy;
                    if(b.pegou) b.posB.dy += p.posP.vy; break;
                case 5: p.posP.dx += p.posP.vx;
                    if(b.pegou) b.posB.dx += p.posP.vx; break;
                case 6: p.morreu=0; p.posP.dx = 0; p.posP.dy = 0;  break;
                case 7: if(b.pegou){ switch(teclaAnterior){
                            case 2: b.posB.vy = -8;
                                switch(teclaAnteriorAnterior){
                                    case 3: b.posB.vx = -8; break;
                                    case 5: b.posB.vx = 8; break;
                                    default: b.posB.vx = 0;
                                } break;
                            case 3: b.posB.vx = -8;
                                switch(teclaAnteriorAnterior){
                                    case 2: b.posB.vy = -8; break;
                                    case 4: b.posB.vy = 8; break;
                                    default: b.posB.vy = 0;
                                } break;
                            case 4: b.posB.vy = 8;
                                switch(teclaAnteriorAnterior){
                                    case 3: b.posB.vx = -8; break;
                                    case 5: b.posB.vx = 8; break;
                                    default: b.posB.vx = 0;
                                } break;
                            case 5: b.posB.vx = 8;
                                switch(teclaAnteriorAnterior){
                                    case 2: b.posB.vy = -8; break;
                                    case 4: b.posB.vy = 8; break;
                                    default: b.posB.vy = 0;
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
            b.posB.dx += b.posB.vx;
            b.posB.dy += b.posB.vy;
        }

        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_filled_rectangle( q.posQ.Px1 ,q.posQ.Py1,q.posQ.Px2,q.posQ.Py2, al_map_rgb(255, 0, 0));

        //Colisao jogador com o vermelho
        colisaoComObstaculo(&p.posP, &q.posQ);

        //Colisao jogador com borda
        colisaoComBorda(&p.posP);

        //Colisao bola com o Vermelho
        colisaoBolaComObstaculo(&b.posB, &q.posQ);

        //Colisao bola com a borda
        colisaoBolaComBorda(&b.posB);

        //Bugou a bola com o quadrado
        bugouBolaObstaculo(&b, &q.posQ);

        //Colisao bola com o player
        bolaAceraPlayer(&b,&p);

        al_draw_filled_rectangle( b.posB.Px1 + b.posB.dx,b.posB.Py1 + b.posB.dy,b.posB.Px2 + b.posB.dx ,b.posB.Py2 + b.posB.dy, al_map_rgb(255, 255, 255));
        if(!p.morreu) al_draw_filled_rectangle( p.posP.Px1 + p.posP.dx, p.posP.Py1 + p.posP.dy, p.posP.Px2 + p.posP.dx , p.posP.Py2 + p.posP.dy, al_map_rgb(0, 255, 0));

        //Printando até onde o Pogador pode pegar a bola
        al_draw_rectangle( p.posP.Px1 + p.posP.dx - pegarbox, p.posP.Py1 + p.posP.dy - pegarbox, p.posP.Px2 + p.posP.dx + pegarbox, p.posP.Py2 + p.posP.dy + pegarbox,  al_map_rgb(255, 255, 0), 5.5);

        al_flip_display();
        i = (i+1)%teto;
        al_rest(1.0/60.0);
    }
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    return 0;
}


