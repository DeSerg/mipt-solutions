#!/usr/local/bin/python3

import os
import sys

INDEX_H_CONTENTS = '''#pragma once

constexpr char kProjectTitle[] = "Additional Project";

'''

path = sys.argv[1]
index_path = os.path.join(path, 'index.h')
with open(index_path, 'w') as f:
    f.write(INDEX_H_CONTENTS)

