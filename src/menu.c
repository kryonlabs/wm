/*
 * Menu System Implementation
 * C89/C90 compliant
 */

#include "menu.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Menu appearance constants
 */
#define MENU_ITEM_HEIGHT     24   /* Height of each menu item in pixels */
#define MENU_ITEM_PADDING    8    /* Padding around text */
#define MENU_BORDER_WIDTH    1    /* Border width */
#define MENU_CORNER_RADIUS   4    /* Corner radius (future use) */

/*
 * Menu colors (9front-style)
 */
#define MENU_BG_COLOR        0xE6E6E6FF   /* Light gray background */
#define MENU_BORDER_COLOR    0x000000FF   /* Black border */
#define MENU_TEXT_COLOR      0x000000FF   /* Black text */
#define MENU_SELECT_COLOR    0x0000FFFF   /* Blue selection (like 9front) */
#define MENU_SELECT_TEXT     0xFFFFFFFF   /* White text when selected */

/*
 * Create and display menu at screen position
 */
Menu *menu_create(MenuItem *items, int count, int x, int y)
{
    Menu *menu;
    int i;
    int max_width;

    if (items == NULL || count <= 0) {
        return NULL;
    }

    /* Allocate menu structure */
    menu = (Menu *)malloc(sizeof(Menu));
    if (menu == NULL) {
        return NULL;
    }

    /* Initialize menu */
    menu->items = items;
    menu->item_count = count;
    menu->x = x;
    menu->y = y;
    menu->selected = -1;
    menu->visible = 1;

    /* Calculate menu width */
    max_width = 0;
    for (i = 0; i < count; i++) {
        int len = strlen(items[i].label);
        if (len > max_width) {
            max_width = len;
        }
    }

    /* Width = (max chars * 8 pixels/char) + padding */
    /* Assuming 8 pixels per character width */
    menu->width = (max_width * 8) + (MENU_ITEM_PADDING * 2);

    /* Height = item count * item height */
    menu->height = count * MENU_ITEM_HEIGHT;

    return menu;
}

/*
 * Render menu to screen buffer
 */
void menu_render(Menu *menu, Memimage *screen)
{
    int i;
    Rectangle menu_rect;
    Rectangle item_rect;
    Point text_pos;

    if (menu == NULL || screen == NULL || !menu->visible) {
        return;
    }

    /* Calculate menu rectangle */
    menu_rect.min.x = menu->x;
    menu_rect.min.y = menu->y;
    menu_rect.max.x = menu->x + menu->width;
    menu_rect.max.y = menu->y + menu->height;

    /* Draw menu background */
    memfillcolor_rect(screen, menu_rect, MENU_BG_COLOR);

    /* Draw menu border */
    {
        Rectangle border;

        /* Top border */
        border.min.x = menu_rect.min.x;
        border.min.y = menu_rect.min.y;
        border.max.x = menu_rect.max.x;
        border.max.y = menu_rect.min.y + MENU_BORDER_WIDTH;
        memfillcolor_rect(screen, border, MENU_BORDER_COLOR);

        /* Bottom border */
        border.min.y = menu_rect.max.y - MENU_BORDER_WIDTH;
        border.max.y = menu_rect.max.y;
        memfillcolor_rect(screen, border, MENU_BORDER_COLOR);

        /* Left border */
        border.min.x = menu_rect.min.x;
        border.min.y = menu_rect.min.y;
        border.max.x = menu_rect.min.x + MENU_BORDER_WIDTH;
        border.max.y = menu_rect.max.y;
        memfillcolor_rect(screen, border, MENU_BORDER_COLOR);

        /* Right border */
        border.min.x = menu_rect.max.x - MENU_BORDER_WIDTH;
        border.max.x = menu_rect.max.x;
        memfillcolor_rect(screen, border, MENU_BORDER_COLOR);
    }

    /* Draw menu items */
    for (i = 0; i < menu->item_count; i++) {
        /* Calculate item rectangle */
        item_rect.min.x = menu->x + MENU_BORDER_WIDTH;
        item_rect.min.y = menu->y + (i * MENU_ITEM_HEIGHT) + MENU_BORDER_WIDTH;
        item_rect.max.x = menu->x + menu->width - MENU_BORDER_WIDTH;
        item_rect.max.y = item_rect.min.y + MENU_ITEM_HEIGHT - MENU_BORDER_WIDTH;

        /* Highlight selected item */
        if (i == menu->selected) {
            memfillcolor_rect(screen, item_rect, MENU_SELECT_COLOR);
        }

        /* Draw item text */
        text_pos.x = item_rect.min.x + MENU_ITEM_PADDING;
        text_pos.y = item_rect.min.y + MENU_ITEM_PADDING + 2;  /* +2 for vertical centering */

        if (i == menu->selected) {
            memdraw_text(screen, text_pos, menu->items[i].label, MENU_SELECT_TEXT);
        } else {
            memdraw_text(screen, text_pos, menu->items[i].label, MENU_TEXT_COLOR);
        }
    }
}

/*
 * Update menu selection based on mouse position
 */
int menu_update_selection(Menu *menu, int mx, int my)
{
    int new_selected;
    int relative_y;

    if (menu == NULL || !menu->visible) {
        return 0;
    }

    /* Check if mouse is inside menu */
    if (!menu_contains(menu, mx, my)) {
        if (menu->selected != -1) {
            menu->selected = -1;
            return 1;
        }
        return 0;
    }

    /* Calculate which item is under mouse */
    relative_y = my - menu->y;
    new_selected = relative_y / MENU_ITEM_HEIGHT;

    /* Clamp to valid range */
    if (new_selected < 0) {
        new_selected = 0;
    } else if (new_selected >= menu->item_count) {
        new_selected = menu->item_count - 1;
    }

    /* Update selection if changed */
    if (new_selected != menu->selected) {
        menu->selected = new_selected;
        return 1;
    }

    return 0;
}

/*
 * Check if menu item was clicked
 */
int menu_handle_click(Menu *menu, int mx, int my)
{
    if (menu == NULL || !menu->visible) {
        return -1;
    }

    /* Check if mouse is inside menu */
    if (!menu_contains(menu, mx, my)) {
        return -1;
    }

    /* Return selected item */
    return menu->selected;
}

/*
 * Hide menu
 */
void menu_hide(Menu *menu)
{
    if (menu != NULL) {
        menu->visible = 0;
    }
}

/*
 * Free menu resources
 */
void menu_destroy(Menu *menu)
{
    if (menu != NULL) {
        free(menu);
    }
}

/*
 * Check if point is inside menu
 */
int menu_contains(Menu *menu, int x, int y)
{
    if (menu == NULL || !menu->visible) {
        return 0;
    }

    return (x >= menu->x && x < menu->x + menu->width &&
            y >= menu->y && y < menu->y + menu->height);
}
