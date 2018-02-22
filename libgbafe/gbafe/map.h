#ifndef GBAFE_MAP_H
#define GBAFE_MAP_H

// Rename to bmmap.h? Since it corresponds to bmmap.c from the proto

#include "common.h"

extern Vector2 map_size;       //! FE8U = (0x0202E4D4)

extern uint8_t** map_unit;     //! FE8U = (0x0202E4D8)
extern uint8_t** map_terrain;  //! FE8U = (0x0202E4DC)
extern uint8_t** map_movement; //! FE8U = (0x0202E4E0)
extern uint8_t** map_range;    //! FE8U = (0x0202E4E4)
extern uint8_t** map_fog;      //! FE8U = (0x0202E4E8)
extern uint8_t** map_hidden;   //! FE8U = (0x0202E4EC)
// extern uint8_t** other_movement_map; //! FE8U = (0x0202E4F0)

extern uint16_t** const map_raw_tile_index; //! FE8U = (0x0859A9D4)

#pragma long_calls

int CanUnitCrossTerrain(Unit*, uint8_t terrain); //! FE8U = (0x0801949C+1)

void RefreshTileMaps();         //! FE8U = (0x08019A64+1)
void DrawTileGraphics();        //! FE8U = (0x08019C3C+1)
void InitMapChangeGraphics();   //! FE8U = (0x08019CBC+1)
void RefreshEntityMaps();       //! FE8U = (0x0801A1F4+1)

#pragma long_calls_off

#endif // GBAFE_MAP_H
