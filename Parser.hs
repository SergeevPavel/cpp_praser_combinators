module Parser where

import Control.Monad

data Parser a = Parser {apply :: (String -> [(a, String)])}

instance Monad Parser where
    (>>=) = bind
    return = result

instance MonadPlus Parser where
    mzero = zero
    mplus = plus

result :: a -> Parser a
result v = Parser (\inp -> [(v, inp)])

zero :: Parser a
zero = Parser (\inp -> [])

item :: Parser Char
item = Parser (\inp -> case inp of
                            []     -> []
                            (x:xs) -> [(x, xs)])

pseq :: Parser a -> Parser b -> Parser (a, b)
Parser p `pseq` Parser q = Parser (\inp -> [((v, w), inp'') | (v, inp')  <- p inp
                                                            , (w, inp'') <- q inp'])

bind :: Parser a -> (a -> Parser b) -> Parser b
Parser p `bind` f = Parser (\inp -> concat [apply (f v) inp' | (v, inp') <- p inp])

sat :: (Char -> Bool) -> Parser Char
sat p = item `bind` \x -> if p x then result x else zero

plus :: Parser a -> Parser a -> Parser a
Parser p `plus`  Parser q = Parser (\inp -> (p inp ++ q inp))
