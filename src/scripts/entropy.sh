#! /bin/sh
#
# entropy.sh
# Copyright (C) 2017 nicolas <nicolas@nicolas>
#
# Distributed under terms of the MIT license.
#

./entropy u8 /data/Trees/DFUDS/LZ78/Prots/protsDFUDS
./entropy u8 /data/Trees/DFUDS/LZ78/English/englishDFUDS
./entropy u8 /data/Trees/DFUDS/LZ78/DNA/dnaDFUDS
./entropy u8 /data/Trees/DFUDS/LZ78/Sources/sourcesDFUDS
./entropy u8 /data/Trees/DFUDS/LZ78/XML/xmlDFUDS
./entropy u8 /data/Trees/DFUDS/LZ78/MIDI/midiDFUDS

./entropy u32 /data/Trees/DFUDS/LZ78/Random399/tree_random
./entropy u32 /data/Trees/DFUDS/LZ78/Random1000/tree_random
