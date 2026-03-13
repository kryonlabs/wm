/*
 * Menu System - 9front-style Right-Click Menu
 * C89/C90 compliant
 *
 * Provides a text-based menu overlay for window management
 */

#ifndef WM_MENU_H
#define WM_MENU_H

#include "graphics.h"

/*
 * Menu item callback function
 */
typedef void (*MenuAction)(void);

/*
 * Menu item structure
 */
typedef struct MenuItem {
    const char *label;        /* Item text */
    MenuAction action;        /* Action callback */
} MenuItem;

/*
 * Menu structure
 */
typedef struct Menu {
    MenuItem *items;          /* Array of menu items */
    int item_count;           /* Number of items */
    int x, y;                 /* Menu position on screen */
    int width, height;        /* Menu dimensions */
    int selected;             /* Currently selected item (-1 if none) */
    int visible;              /* 0 = hidden, 1 = visible */
} Menu;

/*
 * Create and display menu at screen position
 *
 * Parameters:
 *   items - Array of menu items
 *   count - Number of items
 *   x, y - Screen position
 *
 * Returns menu pointer, or NULL on error
 */
Menu *menu_create(MenuItem *items, int count, int x, int y);

/*
 * Render menu to screen buffer
 *
 * Parameters:
 *   menu - Menu to render
 *   screen - Screen buffer to render to
 */
void menu_render(Menu *menu, Memimage *screen);

/*
 * Update menu selection based on mouse position
 *
 * Parameters:
 *   menu - Menu to update
 *   mx, my - Mouse position
 *
 * Returns 1 if selection changed, 0 otherwise
 */
int menu_update_selection(Menu *menu, int mx, int my);

/*
 * Check if menu item was clicked
 *
 * Parameters:
 *   menu - Menu to check
 *   mx, my - Mouse position
 *
 * Returns index of clicked item, or -1 if no item clicked
 */
int menu_handle_click(Menu *menu, int mx, int my);

/*
 * Hide menu
 *
 * Parameters:
 *   menu - Menu to hide
 */
void menu_hide(Menu *menu);

/*
 * Free menu resources
 *
 * Parameters:
 *   menu - Menu to destroy
 */
void menu_destroy(Menu *menu);

/*
 * Check if point is inside menu
 *
 * Parameters:
 *   menu - Menu to check
 *   x, y - Point to test
 *
 * Returns 1 if inside, 0 otherwise
 */
int menu_contains(Menu *menu, int x, int y);

#endif /* WM_MENU_H */
