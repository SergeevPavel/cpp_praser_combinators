module Examples where

import Data.Char

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

alphanum :: Parser Char
alphanum = letter `plus` digit

word :: Parser String
word = (do
    x  <- letter
    xs <- word
    return (x:xs)) `plus` return ""

string :: String -> Parser String
string "" = do return ""
string (x:xs) = do
    char x
    string xs
    return (x:xs)

ident :: Parser String
ident = do
    x  <- lower
    xs <- (many alphanum)
    return (x:xs)

nat :: Parser Int
nat = do
    xs <- many1 digit
    return $ eval xs
    where
        eval xs = foldl1 op [ord x - ord '0' | x <- xs]
        m `op` n = 10 * m + n

int :: Parser Int
int = do
    f <- op
    n <- nat
    return (f n)
    where
        op = (do
            char '-'
            return negate) `plus` return id

ints :: Parser [Int]
ints = brackets (char '[') (int `sepby` char ',') (char ']')

expr :: Parser Int
expr = term `chainl1` addop

term :: Parser Int
term = factor `chainr1` expop

factor :: Parser Int
factor = nat `plus` (brackets (char '(') expr (char ')'))

addop :: Parser (Int -> Int -> Int)
addop = ops [(char '+', (+)), (char '-', (-))]

expop :: Parser (Int -> Int -> Int)
expop = ops [(char '^', (^))]
