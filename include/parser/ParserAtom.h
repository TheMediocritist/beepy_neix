/**
 * Atom parser class.
 *
 * @package     CRSS
 * @author      Thomas Schwarz
 * @copyright   Copyright (c) 2020, Thomas Schwarz
 * @license     -
 * @since       Version 0.1.0
 * @filesource
 */

#ifndef CRSS_PARSERATOM_H
#define CRSS_PARSERATOM_H

#include "rapidxml/rapidxml.hpp"
#include "feed/FeedLoader.h"
#include "Parser.h"

using namespace rapidxml;

namespace crss
{
    class ParserAtom : public Parser
    {
    public:
        explicit ParserAtom(struct rawRss content);
        ~ParserAtom();

        struct rssItem* getFeedItem() override;
    };
}

#endif //CRSS_PARSERATOM_H