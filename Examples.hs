module Examples where

import Parser

getThreeSeq = item `pseq` item `pseq` item

getThreeBind = item `bind` \x1 ->
               item `bind` \x2 ->
               item `bind` \x3 ->
               result (x1, x2, x3)

getThreeMonad = item >>= \x1 ->
                item >>= \x2 ->
                item >>= \x3 ->
                return (x1, x2, x3)

getThreeDo = do
    x1 <- item
    x2 <- item
    x3 <- item
    return (x1, x2, x3)

char :: Char -> Parser Char
char x = sat (\y -> x == y)

digit :: Parser Char
digit = sat (\x -> '0' <= x && x <= '9')

lower :: Parser Char
lower = sat (\x -> 'a' <= x && x <= 'z')

upper :: Parser Char
upper = sat (\x -> 'A' <= x && x <= 'Z')

letter :: Parser Char
letter = lower `plus` upper

word :: Parser String
word = neWord `plus` result ""
        where 
            neWord = letter `bind` \x  ->
                     word   `bind` \xs ->
                     result (x:xs)
