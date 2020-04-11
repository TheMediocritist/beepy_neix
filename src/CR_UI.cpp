/**
 * Terminal UI class.
 *
 * @package     CRSS
 * @author      Thomas Schwarz
 * @copyright   Copyright (c) 2020, Thomas Schwarz
 * @license     -
 * @since       Version 0.1.0
 * @filesource
 */

#include "config.h"
#include "CR_UI.h"
#include "feed/CR_FeedLoader.h"

/**
 * Constructor
 */
CR_UI::CR_UI()
{
    this->initChoices();

    initscr();
    clear();
    noecho();
    cbreak();

    this->windowHeight = LINES - 4;
    this->createFeedWindow();
    this->createArticleWindow();
}


/**
 * Destructor
 */
CR_UI::~CR_UI()
{
    clrtoeol();
    refresh();
    endwin();
}


/**
 * Initialize choices
 */
void CR_UI::initChoices()
{
    this->choice = 1;
    this->articleChoice = 1;
    this->quit = 0;
}



/**
 * Create's feed window
 */
void CR_UI::createFeedWindow()
{
    this->feedWindowWidth = (int) (COLS / 3);
    this->feedWindow = newwin(this->windowHeight, this->feedWindowWidth, 2, 0);
    keypad(this->feedWindow, TRUE);
}



/**
 * Create's article window
 */
void CR_UI::createArticleWindow()
{
    this->articleWindowWidth = (int) (COLS / 3) * 2;
    this->articleWindow = newwin(this->windowHeight, this->articleWindowWidth, 2, this->feedWindowWidth);
    keypad(this->articleWindow, TRUE);
}


/**
 * Show's complete UI
 */
void CR_UI::showUI()
{
    mvprintw(0, 0, "Use vim keys to navigate through articles, Press enter to select a choice or press 'q' to quit.");
    refresh();

    this->printWindows();

    while (1)
    {
        c = wgetch(this->feedWindow);
        switch (c)
        {
            case KEY_UP:
            case KEY_K:
                this->articleChoice = this->decreaseChoice(this->articleChoice);
                break;

            case KEY_DOWN:
            case KEY_J:
                this->articleChoice = this->increaseChoice(this->articleChoice);
                break;

            case KEY_UPPER_K:
                this->articleChoice = 1;
                this->choice = this->decreaseChoice(this->choice);
                break;

            case KEY_UPPER_J:
                this->articleChoice = 1;
                this->choice = this->increaseChoice(this->choice);
                break;

            case ENTER:
            case KEY_Q:
                this->quit = this->choice;
                break;

            default:
                mvprintw(LINES - 1, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
                refresh();
                break;
        }

        this->printWindows();
        if (this->quit != 0)
        {
            break;
        }
    }
}


/**
 * Print UI windows
 */
void CR_UI::printWindows()
{
    this->printFeedsInWindow();
    this->printArticlesInWindow();
}


/**
 * Print's feeds in window
 */
void CR_UI::printFeedsInWindow()
{
    int x = 2, y = 1, i;
    for (i = 0; i < FEEDS_MAX; ++i)
    {
        if (this->choice == i + 1)
        {
            this->printLineHighlightedInWindow(this->feedWindow, y, x, feeds[i]->title);
        }
        else
        {
            this->printLineInWindow(this->feedWindow, y, x, feeds[i]->title);
        }

        wclrtoeol(this->feedWindow);
        ++y;
    }

    box(this->feedWindow, 0, 0);
    wrefresh(this->feedWindow);
}


/**
 * Print's articles in window
 */
void CR_UI::printArticlesInWindow()
{
    int x = 2, y = 1, i;
    int choice = this->choice - 1;
    for (i = 0; i < FEEDS_MAX; i++)
    {
        if (this->articleChoice == i + 1)
        {
            this->printLineHighlightedInWindow(this->articleWindow, y, x, feeds[choice]->items[i]->title);
        }
        else
        {
            this->printLineInWindow(this->articleWindow, y, x, feeds[choice]->items[i]->title);
        }

        wclrtoeol(this->articleWindow);
        ++y;
    }

    box(this->articleWindow, 0, 0);
    wrefresh(this->articleWindow);
}


/**
 * Print standard line
 *
 * @param window
 * @param y
 * @param x
 * @param line
 */
void CR_UI::printLineInWindow(WINDOW *window, int y, int x, char *line)
{
    mvwprintw(window, y, x, "%s", line);
}


/**
 * Print highlighted line
 *
 * @param window
 * @param y
 * @param x
 * @param line
 */
void CR_UI::printLineHighlightedInWindow(WINDOW *window, int y, int x, char *line)
{
    wattron(window, A_REVERSE);
    mvwprintw(window, y, x, "%s", line);
    wattroff(window, A_REVERSE);
}


/**
 * Increase choice
 *
 * @param new_choice
 * @return
 */
int CR_UI::increaseChoice(int new_choice)
{
    if (new_choice == feeds_count)
    {
        new_choice = 1;
    }
    else
    {
        ++new_choice;
    }

    return new_choice;
}


/**
 * Decrease choice
 *
 * @param new_choice
 * @return
 */
int CR_UI::decreaseChoice(int new_choice)
{
    if (new_choice == 1)
    {
        new_choice = feeds_count;
    }
    else
    {
        --new_choice;
    }

    return new_choice;
}