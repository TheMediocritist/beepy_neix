#!/usr/bin/env python
#
# A simple Python script to parse an article's text; for use in neix.conf
#

# NOTE: pip3 install newspaper3k (NOT pip install newspaper!)
import newspaper
import sys
import pathlib

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
