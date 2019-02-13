#include "traitement/position.h"

#include <hl_communication/utils.h>
#include <hl_monitoring/field.h>
#include <hl_monitoring/monitoring_manager.h>
#include <hl_monitoring/utils.h>


namespace traitement

/*Afficher la position du robot depuis les logs*/
{
	Position::Position{
		float x = 0;
		float y = 0;
		float uncertainty = 0;
	}

	}/*on fait des position.get à partir des logs pui position.draw pour add à l'image*/