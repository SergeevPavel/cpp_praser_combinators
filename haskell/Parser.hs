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
sat p = do
    x <- item
    if p x then return x else zero

plus :: Parser a -> Parser a -> Parser a
Parser p `plus`  Parser q = Parser (\inp -> (p inp ++ q inp))

many :: Parser a -> Parser [a]
many p = (do
    x  <- p
    xs <- many p
    return (x:xs)) `plus` return []

many1 :: Parser a -> Parser [a]
many1 p = (do
    x  <- p
    xs <- many p
    return (x:xs))

sepby1 :: Parser a -> Parser b -> Parser [a]
p `sepby1` sep = do
    x <- p
    xs <- many (do 
                 _ <- sep
                 y <- p
                 return y)
    return (x:xs)

sepby :: Parser a -> Parser b -> Parser [a]
p `sepby` sep = (p `sepby1` sep) `plus` (result [])

brackets :: Parser a -> Parser b -> Parser c -> Parser b
brackets open p close = do
                        open
                        x <- p
                        close
                        return x

chainl1 :: Parser a -> Parser (a -> a -> a) -> Parser a
p `chainl1` op = do
            x   <- p
            fys <- many (do
                            f <- op
                            y <- p
                            return (f, y))
            return $ foldl (\x (f, y) -> f x y) x fys

chainr1 :: Parser a -> Parser (a -> a -> a) -> Parser a
p `chainr1` op = do
            x   <- p
            fys <- many (do
                            f <- op
                            y <- p
                            return (f, y))
            return $ foldr (\(f, y) x -> f x y) x fys

ops :: [(Parser a, b -> b -> b)] -> Parser (b -> b -> b)
ops xs = foldr1 plus [(do 
                        _ <- p
                        return op)  | (p, op) <- xs]
