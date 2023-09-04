/**
 * Application Window class
 *
 * @package     neix
 * @author      Thomas Schwarz
 * @copyright   Copyright (c) 2020, Thomas Schwarz
 * @license     -
 * @since       Version 0.1.0
 * @filesource
 */

#include <ncurses.h>
#include "application/ApplicationWindow.h"

using namespace neix;


/**
 * Constructor
 */
ApplicationWindow::ApplicationWindow()
{
    this->enableHighlight = true;
    this->scrollAlways = false;
    this->created = false;
    this->y = 0;
    this->x = 0;
    this->height = 0;
    this->width = 0;
    this->offsetTop = 0;
    this->highlight = 0;
    this->win = nullptr;
    this->pad = nullptr;
}


/**
 * Destructor
 */
ApplicationWindow::~ApplicationWindow() = default;


/**
 * Print main window
 */
void ApplicationWindow::_printWindow()
{
    this->hide();

    box(this->win, 0, 0);
    wrefresh(this->win);
}


/**
 * Print pad
 */
void ApplicationWindow::_printPad()
{
    prefresh(this->pad, this->offsetTop, 0,
        this->y+1, this->x+1, this->height+this->y-2, this->width+this->x);
}

/**
 * Print content of window
 */
void ApplicationWindow::_printContent()
{
    werase(this->pad);
    int x = 0, y = 0, counter = 0;
    for (auto & line: this->content)
    {
        if (this->enableHighlight && this->highlight == counter)
        {
            wattron(this->pad, A_REVERSE);
            mvwprintw(this->pad, y, x, "%s", line.c_str());
            wattroff(this->pad, A_REVERSE);
        }
        else
        {
			mvwaddstr(this->pad, y, x, (line + "\n").c_str());
		}
        y++;
        counter++;
    }

    this->_printPad();
}

/**
 * Create application window
 *
 * @return  true on success, false else
 */
bool ApplicationWindow::_create()
{
    this->win = newwin(this->height, this->width, this->y, this->x);
    keypad(this->win, TRUE);

    this->pad = newpad(200, this->width-4);
    keypad(this->pad, TRUE);

    return this->win != nullptr && this->pad != nullptr;
}

/**
 * Show application window
 */
void ApplicationWindow::show()
{
    if (!this->created)
    {
        this->created = this->_create();
    }

    this->_printWindow();
    this->_printContent();
}

/**
 * Hide application window
 */
void ApplicationWindow::hide()
{
    refresh();
    werase(this->pad);
    werase(this->win);
}


/**
 * Update window
 */
void ApplicationWindow::update()
{
    this->_printContent();
    this->_printPad();
}


/**
 * Reset window
 */
void ApplicationWindow::reset()
{
    this->offsetTop = 0;
    this->resetContent();
    werase(this->pad);
}

void ApplicationWindow::resetContent()
{
    this->content.clear();
}


/**
 * Reset highlight
 */
void ApplicationWindow::resetHighlight()
{
    this->highlight = 0;
}


/**
 * Increase top offset
 */
void ApplicationWindow::_increaseOffsetTop()
{
    this->offsetTop++;
}


/**
 * Decrease top offset
 */
void ApplicationWindow::_decreaseOffsetTop()
{
    this->offsetTop--;
    if (this->offsetTop < 0)
    {
        this->offsetTop = 0;
    }
}


/**
 * Scroll down depending on pushed content elements
 */
void ApplicationWindow::_scrollDownDependingOnContentSize()
{
    if (this->highlight == 0)
    {
        this->offsetTop = 0;
    }
    else if (this->highlight >= this->height-2)
    {
        this->_increaseOffsetTop();
    }
}


/**
 * Scroll up depending on pushed content elements
 */
void ApplicationWindow::_scrollUpDependingOnContentSize()
{
    int count = this->content.size();
    if (this->highlight == count-1)
    {
        this->offsetTop = count - (this->height - 2);
    }
    else if (this->offsetTop > 0)
    {
        this->_decreaseOffsetTop();
    }
}


/**
 * Scroll window down
 */
void ApplicationWindow::scrollDown()
{
    if (this->scrollAlways)
    {
        this->_increaseOffsetTop();
    }
    else
    {
        this->_scrollDownDependingOnContentSize();
    }

    this->_printPad();
}


/**
 * Scroll window up
 */
void ApplicationWindow::scrollUp()
{
    if (this->scrollAlways)
    {
        this->_decreaseOffsetTop();
    }
    else
    {
        this->_scrollUpDependingOnContentSize();
    }


    this->_printPad();
}


/**
 * Set position of window
 *
 * @param y
 * @param x
 */
void ApplicationWindow::setPosition(int y, int x)
{
    this->y = y;
    this->x = x;

    if (this->created)
    {
        mvwin(this->win, this->y, this->x);
        mvwin(this->pad, this->y+1, this->x+2);
    }
}


/**
 * Set dimensions of window
 *
 * @param height
 * @param width
 */
void ApplicationWindow::setDimensions(int height, int width)
{
    this->height = height;
    this->width = width;

    if (this->created)
    {
        wresize(this->win, this->height, this->width);
        wresize(this->pad, 200, this->width-4);
    }
}


/**
 * Push new content
 *
 * @param c
 */
void ApplicationWindow::pushContent(string c)
{
    this->content.push_back(c);
}


/**
 * Increase choice
 */
void ApplicationWindow::increaseHighlight()
{
    int count = this->content.size();
    if (this->highlight == count-1)
    {
        this->highlight = 0;
    }
    else
    {
        this->highlight++;
    }
}


/**
 * Decrease choice
 */
void ApplicationWindow::decreaseHighlight()
{
    int count = this->content.size();
    if (this->highlight == 0)
    {
        this->highlight = count-1;
    }
    else
    {
        this->highlight--;
    }
}


/**
 * Get ncurses window
 */
WINDOW *ApplicationWindow::getWindow()
{
    return this->win;
}
