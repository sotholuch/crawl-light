#ifdef USE_TILE
#ifndef TILEREG_INV_H
#define TILEREG_INV_H

#include "tilereg-grid.h"

class InventoryRegion : public GridRegion
{
public:
    InventoryRegion(const TileRegionInit &init);

    virtual void update();
    virtual int handle_mouse(MouseEvent &event);
    virtual bool update_tip_text(std::string &tip);
    virtual bool update_tab_tip_text(std::string &tip, bool active);
    virtual bool update_alt_text(std::string &alt);

    virtual const std::string name() const { return "Inventory"; }

protected:
    virtual void pack_buffers();
    virtual void draw_tag();
    virtual void activate();
};

#endif
#endif
