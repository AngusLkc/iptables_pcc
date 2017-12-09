# Xtables-addons extensions

## Extensions
* **pcc** match - Per Connection Classifier match, idea based on [MikroTik PCC](http://wiki.mikrotik.com/wiki/Manual:PCC): *PCC matcher will allow you to divide traffic into equal streams with ability to keep packets with specific set of options in one particular stream.* Read [How PCC works](http://wiki.mikrotik.com/wiki/How_PCC_works_(beginner)).

## How to install
After unpack the [xtables-addons](http://xtables-addons.sourceforge.net/) tarball, add [this URL](https://raw.githubusercontent.com/TMarques1000/xa/master/) to the _sources_ file and run the _xa-download-more_ script to retrieve and unpack this extension (if you get an error about Crypt::SSLeay, install libcrypt-ssleay-perl).
```
# cd xtables-addons
xtables-addons# echo https://raw.githubusercontent.com/TMarques1000/xa/master/ >> sources
xtables-addons# ./xa-download-more
  GET     https://raw.githubusercontent.com/TMarques1000/xa/master/xa-index.txt
  GET     https://raw.githubusercontent.com/TMarques1000/xa/master/pcc.tar.bz2
  UNPACK  downloads/pcc.tar.bz2
```
Then follow the instructions in xtabless-addons [INSTALL](http://sourceforge.net/p/xtables-addons/xtables-addons/ci/master/tree/INSTALL) file.
