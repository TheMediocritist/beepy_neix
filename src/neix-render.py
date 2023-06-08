#!/usr/bin/env python
#
# A simple Python script to parse an article's text; for use in neix.conf
# with mtmiron's fork (https://github.com/mtmiron/neix).
#
# neix does not pass a URL to the "renderText" command; it passes the
# name of a cache file containing the feed URL data, not the full article
# URL or any data that could be fetched from it.  It does pass the URL to
# the "openCommand" line if triggered by a user keypress, though; reading
# and displaying the cache of HTML-stripped feed data, after triggering a
# config-specified openCommand, is harmless even if that command isn't
# this script.
#

# NOTE: pip3 install newspaper3k (NOT pip install newspaper!)
import newspaper
import sys
import pathlib

# The file location that neix reads from to display the console text
NEIX_TMP_RENDERED = pathlib.Path(".config", "neix", "tmp-rendered.txt")


def write_content(content, filename = NEIX_TMP_RENDERED.home().joinpath(NEIX_TMP_RENDERED)):
    with open(filename, "w") as f:
        f.write(content)

def fetch_content(url):
    article = newspaper.Article(url)
    article.download()
    article.parse()
    return article.text, article


if __name__ == "__main__":
    content, _ = fetch_content(sys.argv[1])
    write_content(content)
    sys.exit(0)
