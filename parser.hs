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
