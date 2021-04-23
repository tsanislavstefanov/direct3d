#include "stdafx.h"
#include "Event.h"

////////////////////////////////
// EVENT DISPATCHER ////////////
////////////////////////////////

size_t EventDispatcher::GenerateTypeId()
{
    static size_t type_id = 0;
    return type_id++;
}