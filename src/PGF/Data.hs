module PGF.Data (module PGF.Data, module PGF.Expr, module PGF.Type, module PGF.PMCFG) where

import PGF.CId
import PGF.Expr hiding (Value, Env)
import PGF.Type
import PGF.PMCFG

import qualified Data.Map as Map
import qualified Data.Set as Set
import qualified Data.IntMap as IntMap
import Data.List

-- internal datatypes for PGF

-- | An abstract data type representing multilingual grammar
-- in Portable Grammar Format.
data PGF = PGF {
  absname   :: CId ,
  cncnames  :: [CId] ,
  gflags    :: Map.Map CId String,   -- value of a global flag
  abstract  :: Abstr ,
  concretes :: Map.Map CId Concr
  }

data Abstr = Abstr {
  aflags  :: Map.Map CId String,      -- value of a flag
  funs    :: Map.Map CId (Type,[Equation]), -- type and def of a fun
  cats    :: Map.Map CId [Hypo],      -- context of a cat
  catfuns :: Map.Map CId [CId]        -- funs to a cat (redundant, for fast lookup)
  }

data Concr = Concr {
  cflags  :: Map.Map CId String,    -- value of a flag
  lins    :: Map.Map CId Term,      -- lin of a fun
  opers   :: Map.Map CId Term,      -- oper generated by subex elim
  lincats :: Map.Map CId Term,      -- lin type of a cat
  lindefs :: Map.Map CId Term,      -- lin default of a cat
  printnames :: Map.Map CId Term,   -- printname of a cat or a fun
  paramlincats :: Map.Map CId Term, -- lin type of cat, with printable param names
  parser  :: Maybe ParserInfo       -- parser
  }

data Term =
   R [Term]
 | P Term Term
 | S [Term]
 | K Tokn
 | V Int
 | C Int
 | F CId
 | FV [Term]
 | W String Term
 | TM String
  deriving (Eq,Ord,Show)




-- merge two GFCCs; fails is differens absnames; priority to second arg

unionPGF :: PGF -> PGF -> PGF
unionPGF one two = case absname one of
  n | n == wildCId     -> two    -- extending empty grammar
    | n == absname two -> one {  -- extending grammar with same abstract
      concretes = Map.union (concretes two) (concretes one),
      cncnames  = union (cncnames one) (cncnames two)
    }
  _ -> one   -- abstracts don't match ---- print error msg

emptyPGF :: PGF
emptyPGF = PGF {
  absname   = wildCId,
  cncnames  = [] ,
  gflags    = Map.empty,
  abstract  = error "empty grammar, no abstract",
  concretes = Map.empty
  }
