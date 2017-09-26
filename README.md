# Wikimap
This is a tool that I used in my thesis. It can automatically:
* download and parse a Wikipedia dump,
* compute document embeddings for articles, based on links between them,
using the DeepWalk algorithm,
* compute t-SNE mappings for obtained vectors,

and more.

## Requirements
* python 2.7
* gcc 4.8.4

## Installation
1. Clone this repo. Use --recursive to checkout pybind11 too.
```
git clone --recursive git@github.com:pderkowski/wikimap.git wikimap
```
2. Build C++ libs.
```
cd wikimap
make
```
3. Install Python libs listed in requirements.txt. I recommend using pip and
virtualenv:
```
virtualenv env --no-site-packages
source env/bin/activate
pip install -r requirements.txt
```

## Usage
run.py is the entry point to the application. Type
```
python ./run.py -h
```
to see usage info.

For example, to compute embeddings of 100000 most popular articles from Polish
Wikipedia:
```
python ./run.py -t embed -b builds --lang pl
```