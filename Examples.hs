module Examples where

import Parser

getThreeSeq = item `pseq` item `pseq` item

getThreeBind = item `bind` \x1 ->
               item `bind` \x2 ->
               item `bind` \x3 ->
               result (x1, x2, x3)
