#include "GraphicAllegro.h"
#include "Config.h"

#include <allegro.h>
#include <stdlib.h> // RANDOM


void graphique::demarer_allegro()
{
    // Initialisation
    allegro_init();
    install_keyboard();
    install_mouse();

    if (install_mouse() == -1) {
        allegro_message("[Erreur fatale] Impossible d'installer la souris");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0);

    // OUVERTURE MODE GRAPHIQUE (ouverture fen�tre allegro)
    set_color_depth(desktop_color_depth());

    if(set_gfx_mode(GFX_AUTODETECT_WINDOWED,DIMY,DIMX,0,0)!=0)
    {
		allegro_message("[Erreur fatale] Probleme ouverture du mode graphique");
		allegro_exit();
		exit(EXIT_FAILURE);
    }

    srand ( time(NULL) );
    show_mouse(screen);
}

void graphique::afficherTablier(Tablier tab)
{
	BITMAP * PAGE, * TABLIER, * PNOIR, * PBLANC;
	PAGE	= create_bitmap(SCREEN_W,SCREEN_H);
	TABLIER	=load_bitmap("images/tablier.bmp",NULL);
	PNOIR	=load_bitmap("images/pionNoir.bmp",NULL);
	PBLANC	=load_bitmap("images/pionBlanc.bmp",NULL);

	clear_to_color(PAGE,makecol(255,255,255)); // On efface le fond blanc
	masked_blit(TABLIER,PAGE, 0,0,0,0,DIM_X_TABLIER, DIM_Y_TABLIER); // On affiche le tablier

	for(int y=0; y<TAB_TAILLE; y++)
	{
		for(int x=0; x<TAB_TAILLE; x++)
		{
			switch(tab.m_tab[x][y])
			{
			case NOIR:	draw_sprite(PAGE, PNOIR, (x*PION_TAILLE), (y*PION_TAILLE)); break;
			case BLANC:	draw_sprite(PAGE, PBLANC, (x*PION_TAILLE), (y*PION_TAILLE));break;
			default:																break;
			}
		}
	}
	blit(PAGE,screen,0,0,0,0,SCREEN_W,SCREEN_H);
}

void graphique::afficherMessage(std::string message)
{
	textprintf_ex(screen, font, GRAPH_X_MESSAGE, GRAPH_Y_MESSAGE, makecol(0,0,0), makecol(255,255,255), message.c_str());
}

void graphique::selectionCase(int &x, int &y)
{
	/// Donn�es
	int click, xClick, yClick;

	/// Traitement
	while(1)
	{
		if(mouse_b&1) {
				click++;
				// En cas de clic
				if(click == 1) {
					xClick = mouse_x; // On r�cup�re les coordonn�es en pixel
					yClick = mouse_y;

					x = (xClick - (xClick%PION_TAILLE))/PION_TAILLE; // Conversion en num�ro de case
					y = (yClick - (yClick%PION_TAILLE))/PION_TAILLE;

					if((x >= 0) && (x < TAB_TAILLE) && (y >= 0) && (y < TAB_TAILLE))
						return;
				}
		}
		else
			click = 0;
	}
}

void graphique::surprise()
{
	/// Donn�es
	BITMAP * BG, * BOJ, * PIOR;
	SAMPLE * STAR;
	int xB = SCREEN_W/3, yB = SCREEN_H/2, dxB = -2, dyB = 1;
	int xP = (2*SCREEN_W)/3, yP = SCREEN_H/2, dxP = 1, dyP = -1;

	int newX, newY;

	BOJ	= load_bitmap("images/benja.bmp",NULL);
	PIOR= load_bitmap("images/piair.bmp",NULL);

	STAR=load_sample("images/star.wav");
	play_sample(STAR, 155, 128, 1000, 0);

	while(1)
	{
		newX = xB + dxB;
		newY = yB + dyB;
		if((newX > 0) && (newX + 39 < SCREEN_W))	xB = newX;
		else dxB = -dxB;
		if((newY > 0) && (newY + 150 < SCREEN_H))	yB = newY;
		else dyB = -dyB;

		newX = xP + dxP;
		newY = yP + dyP;
		if((newX > 0) && (newX + 125 < SCREEN_W))	xP = newX;
		else dxP = -dxP;
		if((newY > 0) && (newY + 100 < SCREEN_H))	yP = newY;
		else dyP = -dyP;

		dyB += (rand() %3) - 1;
		dxB += (rand() %3) - 1;

		dyP += (rand() %3) - 1;
		dxP += (rand() %3) - 1;

		if((dyB > SCREEN_H/2) || (dyB < -SCREEN_H/2)) dyB = 0;
		if((dxB > SCREEN_W/2) || (dxB < -SCREEN_W/2)) dxB = 0;

		if((dyP > SCREEN_H/2) || (dyP < -SCREEN_H/2)) dyP = 0;
		if((dxP > SCREEN_W/2) || (dxP < -SCREEN_W/2)) dxP = 0;


		BG	= load_bitmap("images/background.bmp",NULL);
		rotate_sprite(BG, BOJ, xB, yB, 0);
		rotate_sprite(BG, PIOR, xP, yP, 0);
		blit(BG,screen,0,0,0,0,SCREEN_W,SCREEN_H);
	}
}
