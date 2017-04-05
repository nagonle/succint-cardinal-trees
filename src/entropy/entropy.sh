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

./entropy u32 ~/random5Kv1K 
./entropy u32 ~/random50Kv1K
./entropy u32 ~/random1Mv200K

./entropy u32 /data/wikipedia/LZ78/wikipedia-50MB-DFUDS
./entropy u32 /data/wikipedia/LZ78/wikipedia-500MB-DFUDS
./entropy u32 /data/wikipedia/LZ78/wikipedia-1GB-DFUDS
