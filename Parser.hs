module Parser where

type Parser a = String -> [(a, String)]

result :: a -> Parser a
result v = \inp -> [(v, inp)]

zero :: Parser a
zero = \inp -> []

item :: Parser Char
item = \inp -> case inp of
                    []     -> []
                    (x:xs) -> [(x, xs)]

pseq :: Parser a -> Parser b -> Parser (a, b)
p `pseq` q = \inp -> [((v, w), inp'') | (v, inp')  <- p inp
                                      , (w, inp'') <- q inp']

bind :: Parser a -> (a -> Parser b) -> Parser b
p `bind` f = \inp -> concat [f v inp' | (v, inp') <- p inp]

sat :: (Char -> Bool) -> Parser Char
sat p = item `bind` \x -> if p x then result x else zero
