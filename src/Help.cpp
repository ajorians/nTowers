#include "Help.h"

TowersHelp::TowersHelp(SDL_Surface* pScreen)
: m_pScreen(pScreen)
{
	m_pFont = nSDL_LoadFont(NSDL_FONT_VGA, 0, 0, 0);
}

TowersHelp::~TowersHelp()
{
	nSDL_FreeFont(m_pFont);
}

bool TowersHelp::Loop()
{
	//Handle keypresses
	if( PollEvents() == false )
		return false;
	
	UpdateDisplay();
	
	return true;
}

bool TowersHelp::PollEvents()
{
	SDL_Event event;
	
	/* Poll for events. SDL_PollEvent() returns 0 when there are no  */
	/* more events on the event queue, our while loop will exit when */
	/* that occurs.                                                  */
	while( SDL_PollEvent( &event ) )
	{
		/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
		switch( event.type )
		{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) 
				{
					case SDLK_ESCAPE:
					case SDLK_RETURN:
					case SDLK_SPACE:
						return false;
					break;
					
					default:
						break;
				}

				break;
			
			//Called when the mouse moves
			case SDL_MOUSEMOTION:
				break;
			
			case SDL_KEYUP:
				break;
			
			default:
				break;
		}
	}
	return true;
}

void TowersHelp::UpdateDisplay()
{
	SDL_FillRect(m_pScreen, NULL, SDL_MapRGB(m_pScreen->format, 153, 153, 255));

	nSDL_DrawString(m_pScreen, m_pFont, 15, 20, 
"Towers is a puzzle game.\n\
The object is to have the numbers\n\
from 1 to the width/height in\n\
every row and every column but\n\
also to meet the indicator\n\
requirement.  The indiator\n\
states the count of visible numbers\n\
from that position.  A higher number\n\
will block out a lower number.\n\
\n\
Also have fun!");		
	
	SDL_UpdateRect(m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}




